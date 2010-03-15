/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.util.Date;
import java.text.*;
import java.io.*;
import java.util.zip.*;
import java.util.Iterator;
import org.apache.commons.beanutils.DynaBean;
import org.apache.commons.beanutils.RowSetDynaClass;
//import com.amazon.s3.*;

/**
 *
 * @author Edwin
 * 
 */
public class clsBuildXMLFile {

    // Initialize object Variables
    private clsBuildMappingTable oMappingTable;
    private clsModificationMassMapping oModMassMapTable;
    private clsAmazon oAmazonS3;
    private static String sXMLOutputFilePath_Lin = "";
    private static String sCurrentSearchFile = "";
    private static String sCurrentSearchFilePath = "";
    private static String sCurrentXMLOutputFile = "";
    private static String sStorage_Location = "";
    private Date errTime;
    //private EmailDevTeam emailError = new EmailDevTeam();
    public static boolean isCompressed = false;

    public clsBuildXMLFile() {

        // Instatiate private variables
        oMappingTable = null;

    }

    public static String SearchOutputLocation() {
        return sXMLOutputFilePath_Lin;
    }

    public static String CurrentSearchFile() {
        return sCurrentSearchFile;
    }

    public static String CurrentXMLOutputFile() {
        return sCurrentXMLOutputFile;
    }

    public String Input_XML_Contents(Integer iParamKey, Integer iToMSFileKey) {

        // This procedure builds the XML file content string.

        // Initialize Variables       
        int iRecordCount = 0;
        int iSetCount = 0;
        int iMapCount = 0;
        int iStringTrim = 0;

        String sXMLFileContent = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n";
        String sXMLFileContentValue = "";
        String[] sSetString = {};
        String sSetValues = "";
        String sTempDir = System.getProperty("java.io.tmpdir");
        String sFileToDownload = "";    // Edwin Nov 2009: This will hold the name of the file to download

        RowSetDynaClass rsRecordset = null;
        //ResultSet rsRecordset = null;

        // Connect to the MySQL database
        try {

            // Get the mapping table for the fields
            oMappingTable = new clsBuildMappingTable();

            String[][] arMapValues = oMappingTable.GetMapTable();

            // Get the mapping table for the Modification Mass Changes
            oModMassMapTable = new clsModificationMassMapping();

            String[][] arModMassMapValues = oModMassMapTable.GetMapTable();

            // Get the storage location to determine whether to use Amazon S3 or NAS.
            sStorage_Location = Main.objDataAccess.GetOutputPath("storage_location");

            // Build the string
            sXMLFileContent = sXMLFileContent + "<bioml>\n";

            // Get the parameters from the database
            rsRecordset = Main.objDataAccess.getRecordSet("SELECT * FROM tm_xtandem_params WHERE tmx_key = " + iParamKey.toString());

            // Check if a recordset was returned
            if (rsRecordset.getRows().isEmpty()) {
                //objDataAccess.killMySQLConnection();
                return ("");
            }

            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Processing Search Profile Parameters...");

            // Reset the counter
            iRecordCount = 0;

            // Get the multiple values from the field
            Iterator iter = rsRecordset.getRows().iterator();

            DynaBean dbDataRow = (DynaBean) iter.next();

            // Loop through  the recordset and store the values in the Process List
            do {

                // If the field is one of the set fields with multiple selections, process differently
                if ("tmx_protein_mods_set,tmx_potential_mods_set,tmx_unimod_round1_set,tmx_unimod_round2_set,tmx_unimod_round3_set,tmx_unimod_round4_set".contains(arMapValues[0][iRecordCount])) {

                    //sSetString = rsRecordset.getObject(rsRecordset.findColumn(arMapValues[0][iRecordCount])).toString().trim().split(",");
                    sSetString = dbDataRow.get(arMapValues[0][iRecordCount]).toString().trim().split(",");

                    // Reset the counter and variable
                    iSetCount = 0;
                    sSetValues = "";

                    do {

                        // Reset the counter
                        iMapCount = 0;

                        do {

                            if (sSetString[iSetCount].contentEquals(arModMassMapValues[0][iMapCount])) {
                                sSetValues += arModMassMapValues[1][iMapCount] + ",";
                            }

                            iMapCount += 1;

                        } while (iMapCount < arModMassMapValues[0].length);

                        iSetCount += 1;

                    } while (iSetCount < sSetString.length);

                    // Determine how much cropping of the string is required
                    if (sSetValues.length() == 0) {
                        iStringTrim = 0;
                    } else {
                        iStringTrim = 1;
                    }

                    // Build the string
                    sXMLFileContentValue = sSetValues.substring(0, sSetValues.length() - iStringTrim);

                    //sXMLFileContent = sXMLFileContent + "\t<note type=\"input\" label=\"" + arMapValues[1][iRecordCount] + "\">" + sSetValues.substring(0, sSetValues.length() - iStringTrim) + "</note>\n";

                } else {

                    // If tmx_eukaryotes, the ensure that "human", "rat" or "mouse" is what is written
                    if ("tmx_eukaryotes".contains(arMapValues[0][iRecordCount])) {

                        // Get the field value
                        sXMLFileContentValue = dbDataRow.get(arMapValues[0][iRecordCount]).toString().trim();

                        // Display the appropriate "specimen" based on contents
                        if (sXMLFileContentValue.contains("human")) {
                            // Build the string
                            sXMLFileContentValue = "human";
                        }
                    } else {
                        // Build the string
                        sXMLFileContentValue = dbDataRow.get(arMapValues[0][iRecordCount]).toString().trim();

                        //sXMLFileContent = sXMLFileContent + "\t<note type=\"input\" label=\"" + arMapValues[1][iRecordCount] + "\">" + rsRecordset.getObject(rsRecordset.findColumn(arMapValues[0][iRecordCount])).toString().trim() + "</note>\n";
                    }

                }

                // Remove the "trypsin," part
                if (sXMLFileContentValue.toLowerCase().contains("trypsin,")) {
                    sXMLFileContentValue = sXMLFileContentValue.substring(sXMLFileContentValue.indexOf(",") + 1).trim();
                }

                // If the entry is "Da", display Daltons
                if (sXMLFileContentValue.equalsIgnoreCase("Da")) {
                    sXMLFileContentValue = "Daltons";
                }

                // If the entry is "0", display empty string
                if (sXMLFileContentValue.equalsIgnoreCase("0")) {
                    sXMLFileContentValue = "";
                }

                // Build the string
                sXMLFileContent = sXMLFileContent + "\t<note type=\"input\" label=\"" +
                        arMapValues[1][iRecordCount] + "\">" + sXMLFileContentValue + "</note>\n";

                iRecordCount++;

            } while (iRecordCount < arMapValues[0].length);

            // Close the recordset
            rsRecordset = null;

            // Build the node for the Input Path
            rsRecordset = Main.objDataAccess.getRecordSet("SELECT mzXML_file_location, mzXML_file_name FROM vo_ms_file_for_search WHERE to_ms_file_key = " + iToMSFileKey.toString() + " AND tmx_key = " + iParamKey.toString() + " AND mzXML_file_location != '?'");

            // Check if a recordset was returned
            if (rsRecordset.getRows().isEmpty()) {
                //objDataAccess.killMySQLConnection();
                return ("");
            }

            iter = rsRecordset.getRows().iterator();

            dbDataRow = (DynaBean) iter.next();

            // Store the name and location of the file being searched
            sCurrentSearchFile = dbDataRow.get("mzXML_file_name").toString().trim();
            sCurrentSearchFilePath = dbDataRow.get("mzXML_file_location").toString().trim();

            // Edwin Nov 2009
            // If the search file has been gzipped, trim off the .gz extension
            sFileToDownload = sCurrentSearchFile;   // Store the name of the file to download

            //Check if the file was compressed before uploading
            if (sCurrentSearchFile.endsWith(".gz")) {
                sCurrentSearchFile = sCurrentSearchFile.substring(0, sCurrentSearchFile.length() - 3);
            }
            // Edwin Nov 2009

            // Build the node for the Output, Title node
            sXMLFileContent = sXMLFileContent + "\t<note type=\"input\" label=\"output, title\">Models from '" +
                    sCurrentSearchFile + "'</note>\n";

            // Build the node for the Input Path
            //sXMLFileContent = sXMLFileContent + "\t<note type=\"input\" label=\"spectrum, path\">" + rsRecordset.getString("mzXML_file_location").toString().trim() + sCurrentSearchFile + "</note>\n";
            sXMLFileContent = sXMLFileContent + "\t<note type=\"input\" label=\"spectrum, path\">" +
                    sCurrentSearchFilePath + sCurrentSearchFile + "</note>\n";

            // Build the node for the Output Path.
            sXMLOutputFilePath_Lin = Main.objDataAccess.GetOutputPath("search_output_location_lf");
            //Create the path if it does not exist

            //File dir1 = new File (".");
            //File dir2 = new File ("..");
            //String currentdir = System.getProperty("user.dir");
            //System.out.println ("Current dir : " + dir1.getCanonicalPath());
            //System.out.println ("Parent  dir : " + dir2.getCanonicalPath());
            //System.out.println ("Current Working directory : " + currentdir);


            File outputDir = new File(sXMLOutputFilePath_Lin);
            if (!outputDir.exists()) {
                boolean dirCreated = outputDir.mkdirs();
                if (dirCreated == true) {
                    errTime = new Date();
                    System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Xtandem output path has been successfully created.");
                } else {
                    errTime = new Date();
                    System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Xtandem Output path does not exist and creating has failed.");
                }
            }

            //rsRecordset = objDataAccess.getRecordSet("SELECT tsd.search_ouput_location, tsd.search_output_file_name FROM to_search_details tsd INNER JOIN to_ms_file tms ON tsd.to_ms_file_key = tms.to_ms_file_key WHERE tms.to_ms_file_key = " + iToMSFileKey.toString() + " AND tms.tmx_key = " + iParamKey.toString() + " AND tsd.search_ouput_location IS NOT NULL LIMIT 0,1");

            // Check if a recordset was returned
            //if (rsRecordset == null) {
            //return ("");
            //}

            //sSearchOutputLocation = rsRecordset.getString("search_ouput_location").toString().trim() + rsRecordset.getString("search_output_file_name").toString().trim();

            // Build the string
            sXMLFileContent = sXMLFileContent + "\t<note type=\"input\" label=\"output, path\">" + sXMLOutputFilePath_Lin.toLowerCase();

            if (sCurrentSearchFile.toLowerCase().endsWith(".mgf")) {
                sXMLFileContent = sXMLFileContent + sCurrentSearchFile.substring(0, sCurrentSearchFile.length() - 4).toLowerCase() + "_out.xml" + "</note>\n";
                sCurrentXMLOutputFile = sCurrentSearchFile.substring(0, sCurrentSearchFile.length() - 4) + "_out.xml";
            } else {
                sXMLFileContent = sXMLFileContent + sCurrentSearchFile.substring(0, sCurrentSearchFile.length() - 6).toLowerCase() + "_out.xml" + "</note>\n";
                sCurrentXMLOutputFile = sCurrentSearchFile.substring(0, sCurrentSearchFile.length() - 6) + "_out.xml";
            }

            // Build string for iontrap.xml file
            //SIK29092009 - Change location to one that can stick on an AMI between rebuilds
            sXMLFileContent = sXMLFileContent + "\t<note type=\"input\" label=\"list path, default parameters\">" +
                    "/data/data_share/common/xtandem/iontrap.xml</note>\n";

            // Add Output XSL file path
            sXMLFileContent = sXMLFileContent + "\t<note type=\"input\" label=\"output, xsl path\">" +
                    "/thegpm/tandem/tandem-style.xsl</note>\n";

            // Complete the XML file
            sXMLFileContent = sXMLFileContent + "</bioml>\n";

            // Close the recordset
            rsRecordset = null;

            // If we are using Amazon S3, pull down the file from the S3 storage
            if (sStorage_Location.contains("AS3")) {

                // Amazon S3 storage
                oAmazonS3 = new clsAmazon();    // Instantiate new custom Amazon S3 object
                oAmazonS3.ConnectToAmazon();    // Connect to Amazon S3
                oAmazonS3.SetBucket(sCurrentSearchFile);

                // Pull down file from Bucket. If it fails, set the returned content to empty string.
                // Edwin Nov 2009: if(!oAmazonS3.DownloadFile(sCurrentSearchFilePath.toLowerCase() + sCurrentSearchFile.toLowerCase())) {
                if (!oAmazonS3.DownloadFile(sCurrentSearchFilePath.toLowerCase() + sFileToDownload.toLowerCase())) {
                    errTime = new Date();
                    System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Trashed input_dmn.xml file contents...");
                    sXMLFileContent = "";
                }

                // Kill the Amazon object
                oAmazonS3 = null;
            }
            //objDataAccess.killMySQLConnection();
            return sXMLFileContent;

        } catch (Exception eX) {
            //objDataAccess.killMySQLConnection();
            errTime = new Date();
            Main.currErrorMessage = "Error occured in class 'clsBuildXMLFile': " + eX.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsBuildXMLFile': ");
            eX.printStackTrace();
            rsRecordset = null;
            Main.emailError.SendEmail();
            System.exit(1);
            return ("");
        }

    }

    /**
     * This procedure decompresses a zipped file with a .gz or .gzip extension and may retain or delete the 
     * original compressed file.
     * 
     * @param inputFile The complete path to the file we wish to decompress
     * @param outputFile The complete path to where we wish to store the uncompressed file
     * @param keepCompressed Whether to retain the original compressed file
     * @return A boolean value indicating success or failure of the decompression
     */
    public boolean uncompressFile(String inputFile, String outputFile, boolean keepCompressed) {
        String source;
        GZIPInputStream inputStream;
        String target;
        OutputStream out;
        byte[] buf;
        int len;
        boolean isUnzipped = false;
        File delSource;

        try {
            isUnzipped = false;
            source = inputFile;
            inputStream = new GZIPInputStream(new FileInputStream(source));
            target = outputFile;
            out = new FileOutputStream(target);
            buf = new byte[1024];
            while ((len = inputStream.read(buf)) > 0) {
                out.write(buf, 0, len);
            }
            inputStream.close();
            out.close();

            //If keepCompressed is false delete the source file otherwise keep them both
            if (keepCompressed == false) {
                delSource = new File(source);
                delSource.delete();
            }

            isUnzipped = true;
        } catch (SecurityException se) {
            //Will occur if the user does not have delete rights
            errTime = new Date();
            //Main.currErrorMessage = "Error occured in class 'clsBuildXMLFile' while uncompressing file: " + inputFile + " " + se.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsBuildXMLFile': ");
            se.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            return true; //Because we have unzipped ok only deletion failed
        } catch (Exception ex) {
            errTime = new Date();
            Main.currErrorMessage = "Error occured in class 'clsBuildXMLFile' while uncompressing file: " + inputFile + " " + ex.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsBuildXMLFile': ");
            ex.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            return false;
        }
        return isUnzipped;
    }
}

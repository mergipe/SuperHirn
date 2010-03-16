/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.text.*;
import java.util.Date;
import java.util.zip.*;
import java.io.*;
import net.sf.saxon.Transform;

/**
 *
 * @author User
 */
public class clsSearchItem {

    // Declare private variables
    private int iRecordKey;
    private int iMSFileKey;
    private String sEngineType;
    private String sParameters;
    private String sProcessingStatus;
    private String sOutputFilename;
    private String sOutputLocation;
    private String sStageDatabase;
    private String sTargetDatabase;
    private String sStorage_Location = "";
    private Date errTime;
    //private EmailDevTeam emailError = new EmailDevTeam();
    // private static Connection conMySQL = null;
    Transform transformPeptide = new Transform();
    Transform transformProtein = new Transform();
    Transform transformPTM = new Transform();
    private clsAmazon oAmazonS3;

    public clsSearchItem() {

        // Initialize variables
        iRecordKey = 0;
        iMSFileKey = 0;
        sEngineType = "";
        sParameters = "";
        sProcessingStatus = "";
        sOutputFilename = "";
        sOutputLocation = "";
        sStageDatabase = "";
        sTargetDatabase = "";

    }

    public void SetRecordKey(int iRecKey) {
        iRecordKey = iRecKey;
    }

    public void SetMSFileKey(int iMSKey) {
        iMSFileKey = iMSKey;
    }

    public void SetEngineType(String sEngine) {
        sEngineType = sEngine;
    }

    public void SetParameters(String sParams) {
        sParameters = sParams;
    }

    public void SetProcessStatus(String sProcess) {
        sProcessingStatus = sProcess;
    }

    public void SetOutputFilename(String sOutputFile) {
        sOutputFilename = sOutputFile;
    }

    public void SetOutputLocation(String sLocation) {
        sOutputLocation = sLocation;
    }

    public void SetStageDatabase(String sStageDB) {
        sStageDatabase = sStageDB;
    }

    public void SetTargetDatabase(String sTargetDB) {
        sTargetDatabase = sTargetDB;
    }

    public void TransformXMLFile() {
        // Declare variables
        //Connection conMySQL = null;
        int iRecordCount = 0;
        String sSaxonCommand_Peptide = "";
        String sSaxonCommand_Protein = "";
        String sSaxonCommand_PTM = "";
        String sBatchContents = "";
        int exitVal = 0;
        int iDoubleQuote_Ascii = 34;
        char sDoubleQuote = (char) iDoubleQuote_Ascii;

        // Connect to the MySQL database
        try {
            Main.objDataAccess.SetDatabase(sTargetDatabase);
            //conMySQL = Main.objDataAccess.connectToMySQL();

            // Check if a connection was made
            //if (conMySQL == null) {
            //Main.objDataAccess.killMySQLConnection();
            //Main.numThreadErrors++;
            //return;
            //}
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "Preparing to transform XML file: " + sOutputLocation + sOutputFilename + "...");

            //Statement sSQLSelect = conMySQL.createStatement();

            // If we are working with Amazon, then we have to pull the file down from S3

            // Get the storage location to determine whether to use Amazon S3 or NAS.
            sStorage_Location = Main.objDataAccess.GetOutputPath("storage_location");

            // If we are using Amazon S3, pull down the file from the S3 storage
            if (sStorage_Location.contains("AS3")) {

                // Amazon S3 storage
                oAmazonS3 = new clsAmazon();    // Instantiate new custom Amazon S3 object
                oAmazonS3.ConnectToAmazon();    // Connect to Amazon S3
                oAmazonS3.SetBucket(sOutputFilename);

                // Pull down file from Bucket. If it fails, set the returned content to empty string.
                if (!oAmazonS3.DownloadFile(sOutputLocation.toLowerCase() + sOutputFilename.toLowerCase())) {
                    iRecordCount = Main.objDataAccess.runUpdateStmt("UPDATE " + sTargetDatabase + ".to_search_details SET processing_status = 'ERROR TRANSFORMING XML' WHERE to_search_details_key = " + iRecordKey);
                    //Main.numThreadErrors++;
                    return;
                }

                // Kill the Amazon object
                oAmazonS3 = null;
            }


            // Update the "processing_status" field of the table to show that its processing

            iRecordCount = Main.objDataAccess.runUpdateStmt("UPDATE " + sTargetDatabase + ".to_search_details SET processing_status = 'TRANSORMING XML' WHERE to_search_details_key = " + iRecordKey);

            // Execute Saxon code 
            try {
                //SIK14102009 Check OS to enable this code run in both Windows and linux
                String os = System.getProperty("os.name").toLowerCase();
                String sPath;
                if (os.indexOf("win") >= 0) {
                    sPath = "c:\\ppp_analyzer\\";
                } else {
                    sPath = "/usr/local/applications/saxon/";
                }
                //if file is compressed uncompress it
                if (sOutputFilename.endsWith(".gz")) {
                    //if (uncompressFile(sOutputLocation.toLowerCase() + sOutputFilename.toLowerCase(), sOutputLocation.toLowerCase() + sOutputFilename.toLowerCase().substring(0, sOutputFilename.lastIndexOf(".")), true) == true) {
                    // Delete the .gz file
                    File flDelSource = new File(sOutputFilename);
                    flDelSource.delete();

                    // Rename the file
                    this.SetOutputFilename(sOutputFilename.substring(0, sOutputFilename.lastIndexOf(".")));

                    //}
                }

                //Execute peptide
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "Transforming peptide XML file: " + sOutputLocation + sOutputFilename + "...");

                //String[] sPeptideArgs = new String[] {sOutputLocation + sOutputFilename ,  sOutputLocation + "ts_xTandem_Peptide.xslt", "driver=com.mysql.jdbc.Driver", "database=jdbc:mysql://63.147.127.70:3306/"  + sStageDatabase , "user=" + objDataAccess.GetUserName(), "password=" + objDataAccess.GetPassword() , "SourceXMLFile=" + sOutputFilename , "XMLFileLocation=" + sOutputLocation };
                String[] sPeptideArgs = new String[]{sOutputLocation.toLowerCase() + sOutputFilename, sPath + "ts_xTandem_Peptide.xslt", "driver=com.mysql.jdbc.Driver", "database=jdbc:mysql://" + Main.objDataAccess.GetHostName() + ":" + Main.objDataAccess.GetPortNumber() + "/" + sStageDatabase, "user=" + Main.objDataAccess.GetUserName(), "password=" + Main.objDataAccess.GetPassword(), "SourceXMLFile=" + sOutputFilename, "XMLFileLocation=" + sOutputLocation.toLowerCase()};
                //String[] sPeptideArgs = new String[] {sOutputLocation + sOutputFilename ,  "d:\\saxon\\ts_xTandem_Peptide.xslt", "driver=com.mysql.jdbc.Driver", "database=jdbc:mysql://" + Main.objDataAccess.GetHostName() + ":" + Main.objDataAccess.GetPortNumber() + "/"  + sStageDatabase , "user=" + objDataAccess.GetUserName(), "password=" + objDataAccess.GetPassword() , "SourceXMLFile=" + sOutputFilename , "XMLFileLocation=" + sOutputLocation };
                //String[] sPeptideArgs = new String[] {sOutputLocation + sOutputFilename ,  "/usr/local/applications/saxon/ts_xTandem_Peptide.xslt", "driver=com.mysql.jdbc.Driver", "database=jdbc:mysql://" + Main.objDataAccess.GetHostName() + ":" + Main.objDataAccess.GetPortNumber() + "/"  + sStageDatabase , "user=" + objDataAccess.GetUserName(), "password=" + objDataAccess.GetPassword() , "SourceXMLFile=" + sOutputFilename , "XMLFileLocation=" + sOutputLocation };

                transformPeptide.main(sPeptideArgs);

                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "Peptide XML transform succeeded,proceeding to transform protein XML");
                //Execute protein

                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "Transforming Protein XML file: " + sOutputLocation + sOutputFilename + "...");
                String[] sProteinArgs = new String[]{sOutputLocation.toLowerCase() + sOutputFilename, sPath + "ts_xTandem_Protein.xslt", "driver=com.mysql.jdbc.Driver", "database=jdbc:mysql://" + Main.objDataAccess.GetHostName() + ":" + Main.objDataAccess.GetPortNumber() + "/" + sStageDatabase, "user=" + Main.objDataAccess.GetUserName(), "password=" + Main.objDataAccess.GetPassword(), "SourceXMLFile=" + sOutputFilename, "XMLFileLocation=" + sOutputLocation.toLowerCase()};
                //String[] sProteinArgs = new String[] {sOutputLocation + sOutputFilename ,  "d:\\saxon\\ts_xTandem_Protein.xslt", "driver=com.mysql.jdbc.Driver", "database=jdbc:mysql://" + Main.objDataAccess.GetHostName() + ":" + Main.objDataAccess.GetPortNumber() + "/"  + sStageDatabase , "user=" + objDataAccess.GetUserName(), "password=" + objDataAccess.GetPassword() , "SourceXMLFile=" + sOutputFilename , "XMLFileLocation=" + sOutputLocation };
                //String[] sProteinArgs = new String[] {sOutputLocation + sOutputFilename ,  "/usr/local/applications/saxon/ts_xTandem_Protein.xslt", "driver=com.mysql.jdbc.Driver", "database=jdbc:mysql://" + Main.objDataAccess.GetHostName() + ":" + Main.objDataAccess.GetPortNumber() + "/"  + sStageDatabase , "user=" + objDataAccess.GetUserName(), "password=" + objDataAccess.GetPassword() , "SourceXMLFile=" + sOutputFilename , "XMLFileLocation=" + sOutputLocation };
                transformProtein.main(sProteinArgs);

                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "Protein XML transform succeeded,proceeding to transform PTM XML");

                //Execute PTM

                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "Transforming PTM XML file: " + sOutputLocation + sOutputFilename + "...");
                String[] sPtmArgs = new String[]{sOutputLocation.toLowerCase() + sOutputFilename, sPath + "ts_xTandem_PTM.xslt", "driver=com.mysql.jdbc.Driver", "database=jdbc:mysql://" + Main.objDataAccess.GetHostName() + ":" + Main.objDataAccess.GetPortNumber() + "/" + sStageDatabase, "user=" + Main.objDataAccess.GetUserName(), "password=" + Main.objDataAccess.GetPassword(), "SourceXMLFile=" + sOutputFilename, "XMLFileLocation=" + sOutputLocation.toLowerCase()};
                //String[] sPtmArgs = new String[] {sOutputLocation + sOutputFilename ,  "d:\\saxon\\ts_xTandem_PTM.xslt", "driver=com.mysql.jdbc.Driver", "database=jdbc:mysql://" + Main.objDataAccess.GetHostName() + ":" + Main.objDataAccess.GetPortNumber() + "/"  + sStageDatabase , "user=" + objDataAccess.GetUserName(), "password=" + objDataAccess.GetPassword() , "SourceXMLFile=" + sOutputFilename , "XMLFileLocation=" + sOutputLocation };
                //String[] sPtmArgs = new String[] {sOutputLocation + sOutputFilename ,  "/usr/local/applications/saxon/ts_xTandem_PTM.xslt", "driver=com.mysql.jdbc.Driver", "database=jdbc:mysql://" + Main.objDataAccess.GetHostName() + ":" + Main.objDataAccess.GetPortNumber() + "/"  + sStageDatabase , "user=" + objDataAccess.GetUserName(), "password=" + objDataAccess.GetPassword() , "SourceXMLFile=" + sOutputFilename , "XMLFileLocation=" + sOutputLocation };
                transformPTM.main(sPtmArgs);

                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "PTM XML transform succeeded.");


                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "All XML transformations  completed successfully");
                iRecordCount = Main.objDataAccess.runUpdateStmt("UPDATE " + sTargetDatabase + ".to_search_details SET processing_status = 'XML TRANSFORMED SUCCESSFULLY' WHERE to_search_details_key = " + iRecordKey);

                transformPeptide = null;
                transformProtein = null;
                transformPTM = null;

                //return;
            } catch (IOException ioe) {
                errTime = new Date();
                Main.currErrorMessage = ioe.toString();
                System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " " + ioe.getMessage());
                iRecordCount = Main.objDataAccess.runUpdateStmt("UPDATE " + sTargetDatabase + ".to_search_details SET processing_status = 'ERROR TRANSFORMING XML' WHERE to_search_details_key = " + iRecordKey);
                Main.numThreadErrors++;
                Main.emailError.SendEmail();
                System.exit(1);
            } //Added to catch any wrong arguments that may have been passed to the main method of class Transform
            catch (IllegalArgumentException iaEx) {
                errTime = new Date();
                Main.currErrorMessage = iaEx.toString();
                System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " " + iaEx.getMessage() + " More Details: " + iaEx.toString());
                iaEx.printStackTrace();
                iRecordCount = Main.objDataAccess.runUpdateStmt("UPDATE " + sTargetDatabase + ".to_search_details SET processing_status = 'ERROR TRANSFORMING XML' WHERE to_search_details_key = " + iRecordKey);
                Main.numThreadErrors++;
                Main.emailError.SendEmail();
                System.exit(1);
            } catch (Exception ex) {
                errTime = new Date();
                Main.currErrorMessage = ex.toString();
                System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " " + ex.getMessage() + " More Details: " + ex.toString());
                iRecordCount = Main.objDataAccess.runUpdateStmt("UPDATE " + sTargetDatabase + ".to_search_details SET processing_status = 'ERROR TRANSFORMING XML' WHERE to_search_details_key = " + iRecordKey);
                Main.numThreadErrors++;
                Main.emailError.SendEmail();
                System.exit(1);
            } catch (Throwable t) {
                errTime = new Date();
                Main.currErrorMessage = t.toString();
                t.printStackTrace();
                iRecordCount = Main.objDataAccess.runUpdateStmt("UPDATE " + sTargetDatabase + ".to_search_details SET processing_status = 'ERROR TRANSFORMING XML' WHERE to_search_details_key = " + iRecordKey);
                Main.numThreadErrors++;
                Main.emailError.SendEmail();
                System.exit(1);
            }

            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "XML file transformation completed...");


            // Check if a connection is active and close
            //if (conMySQL != null) {
            //    conMySQL.close();
            //}

        } catch (Exception eX) {
            errTime = new Date();
            Main.currErrorMessage = eX.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsSearchItem': ");
            eX.printStackTrace();
            Main.numThreadErrors++;
            Main.emailError.SendEmail();
            System.exit(1);
            return;
        }
    }

    /* public boolean CreateTransformationBatchFile(String sBatchContents, String sLocation) throws IOException {

    // Create return value variable
    boolean blnSuccess = false;

    // Check if variable(s) have been provided
    if (sBatchContents.length() == 0) {
    return blnSuccess;
    }

    System.out.println("Creating transformation batch file...");

    // Build the batch file
    try {
    java.io.FileWriter oBatchFile = new FileWriter(new File(sLocation + "TransformXML.bat"));

    // Write text to file
    oBatchFile.write(sBatchContents);
    oBatchFile.close();

    } catch (IOException e){
    e.printStackTrace();
    return blnSuccess;
    }

    // Reset the return value
    blnSuccess = true;
    return blnSuccess;

    }*/
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
            Main.currErrorMessage = "Error occured in class 'clsSearchItem' while uncompressing file: " + inputFile + " " + se.toString();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsSearchItem': ");
            se.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            return true; //Because we have unzipped ok only deletion failed
        } catch (Exception ex) {
            errTime = new Date();
            Main.currErrorMessage = "Error occured in class 'clsSearchItem' while uncompressing file: " + inputFile + " " + ex.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsSearchItem': ");
            ex.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            return false;
        }

        return isUnzipped;
    }
}

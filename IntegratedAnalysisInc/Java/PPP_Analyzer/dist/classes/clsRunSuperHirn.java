/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.io.*;
import java.util.Date;
import java.text.*;

import org.apache.commons.beanutils.DynaBean;
import org.apache.commons.beanutils.RowSetDynaClass;

/**
 * Class to executes label-free qunatification workflow for PASS. This include 
 * the SuperHirn processing (feature extraction, ms2 to ms1 mapping, LC-MS alignment, etc.), integration
 * of SuperHirn XML results into PASS database, tandem2pepXML conversion and finally data cleanup:
 * - Commands runSuperHirn: feature extraction of an mzXML input file
 * - Commands runSuperHirnDBPusher: parsing and importing of SuperHirn results into PASS database
 * - Commands runCleanUp...: clean up (deletion) of intermediate files
 * - etc.
 *
 * For testing, process can also be run in VERBOSE (see attribute VERBOSE and set to true) mode, where commands are only printed to screen.
 * @author Lukas N. Mueller
 */
public class clsRunSuperHirn{

    private Date errTime;
    private EmailDevTeam emailError = new EmailDevTeam();
    
    

    /**
     * Set this attribute to true for running the class in testing mode.
     * Otherwise set to false.
     */
    private boolean VERBOSE = false;
    
    /**
     * command to start SuperHirn
     */
    static private String SuperHirnCommand = "SuperHirnv03 ";

    /**
     * Path to SuperHirn output directory
     */
    static private String SuperHirnOutPutPath = "ANALYSIS_TestData/LC_MS_RUNS/ ";

    /**
     * Path to SuperHirn parameter file
     */
    static private String SuperHirnParameterFilePath = "-PARAM /root/param.def";
    
    /**
     * Path for xtandem2xml conversion file 
     */
    //static private String PepXMLFileFolder = "AS3/jenny_55555/SITT/xtandem_output/";
    
    /**
     * command to start Superhirn result to database import process
     */
    static private String dbPusherCommand = "java -Xmx1g -Xms1g -jar Java/SuperDBPusher/SuperDbPusher.jar ";
    
    /**
     * command to cleanup Superhirn results 
     */
    static private String tandemToXMLCommand = "Tandem2XML ";

    /**
     * path to the pepXML converions output directory
     */
    static private String tandemToXMLOutputDir = "TestingData/pepXML/";
    
    /**
     * command to cleanup Superhirn results 
     */
    static private String cleanupSHCommand = "rm -rf ";
    
    
    
    /**
     * Get the path to the mzXML defined by it key from the database table
     * @param iFileKey String table (primary)key of tuple containing the the mzXML file path
     */
    private String getMzXMLFile( String iFileKey){
    	
    	String mzXMLTarget = new String("");
    	try
    	{ 	
    		// get the mzXML file path from the table:
			RowSetDynaClass mzXMLFileRecord;
			mzXMLFileRecord = Main.objDataAccess
					.getRecordSet("SELECT mzXML_file_name, mzXML_file_location from to_ms_file WHERE to_ms_file_key = "
							+ iFileKey);
			
			// Check if a recordset was returned
			if (!mzXMLFileRecord.getRows().isEmpty()) {

				DynaBean dbDataRow = (DynaBean) mzXMLFileRecord.getRows()
						.get(0);
				// Build the load strings
				errTime = new Date();
				mzXMLTarget = (String) dbDataRow.get("mzXML_file_location") + (String) dbDataRow.get("mzXML_file_name");
				System.out.println(DateFormat.getDateTimeInstance(
						DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
						.toString()
						+ " clsRunSuperHirn: processing file " + mzXMLTarget);

			}

		} catch (NotAvailableException e) {
			errTime = new Date();
			System.out
					.println(DateFormat.getDateTimeInstance(
							DateFormat.MEDIUM, DateFormat.MEDIUM)
							.format(errTime).toString()
							+ " Unable to get mzXML file for SuperHirn processing. Process will exit.");

      
       
    	}
        
        return mzXMLTarget;
        
    }
    
    
    /**
     * Get the path to the tandem file defined by it key from the database table
     * @param iFileKey String table (primary)key of tuple containing the tandem file path
     */
    private String getTandemFile( String iFileKey){
    	
    	String mzXMLTarget = new String("");
    	try
    	{
     
    		// get the mzXML file path from the table:
			RowSetDynaClass mzXMLFileRecord;
			mzXMLFileRecord = Main.objDataAccess
					.getRecordSet("SELECT search_output_file_name, search_ouput_location " +
							"from to_search_details WHERE to_ms_file_key = "
							+ iFileKey);
			
			// Check if a recordset was returned
			if (!mzXMLFileRecord.getRows().isEmpty()) {

				DynaBean dbDataRow = (DynaBean) mzXMLFileRecord.getRows()
						.get(0);
				// Build the load strings
				errTime = new Date();
				mzXMLTarget = (String) dbDataRow.get("search_ouput_location") + (String) dbDataRow.get("search_output_file_name");
				System.out.println(DateFormat.getDateTimeInstance(
						DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
						.toString()
						+ " clsRunSuperHirn: processing file " + mzXMLTarget);

			}

		} catch (NotAvailableException e) {
			errTime = new Date();
			System.out
					.println(DateFormat.getDateTimeInstance(
							DateFormat.MEDIUM, DateFormat.MEDIUM)
							.format(errTime).toString()
							+ " Unable to get tandem file for SuperHirn processing. Process will exit.");

      
       
    	}
        
		return mzXMLTarget;
        
    }
    

    /**
     * Performs a label free quantification workflow on a mzXML file which is defined by its
     * mysql primary tuple key.
     * @param mzXMLFileKey String primary key for identifying the corresponding tuple in the table to_ms_file
     * @return int 
     */
    public int labelfreeQuantificationOnMzXMLFile(String mzXMLFileKey, String tandemXMLFileKey) 
    {
		// Initialize return value
		int exitVal = 1;

		
		/*
		 * copy files from amazon to the instance:
		 */
		String xTandem = this.downloadXTandemFile( mzXMLFileKey );
		if( xTandem == null ){
			return -1;
		}
			
		String mzXML = this.downloadMzXMLFile( mzXMLFileKey );
		if( mzXML == null ){
			return -1;
		}
		
		// conversion of xtandem to pepxml format:
		String pepXML = this.convertTandemToPepXML( xTandem );
		if( pepXML == null){
			return -1;
		}

		// run feature extraction:
		exitVal = this.runSuperHirnFeatureExtraction(mzXML);
		if( exitVal == -1){
			return exitVal;
		}
		
		// import to database:
		exitVal = this.runSuperHirnDBPusher();
		if( exitVal == -1){
			return exitVal;
		}
		
		// clean up superhirn results:
		exitVal = this.cleanUpSuperHirnResults();
		if( exitVal == -1){
			return exitVal;
		}
		
		// clean up pepXML conversion files:
		exitVal = this.cleanUpFile(pepXML);
		exitVal = this.cleanUpFile(mzXML);
		exitVal = this.cleanUpFile(mzXML + ".gz");

		return exitVal;
    }

    /**
     * Runs SuperHirn feature extraction on the input mzXML file 
     * @param mzXMLFileKey String mzXML file path
     * @return int 
     */
    private int runSuperHirnFeatureExtraction(String mzXMLFile) 
    {
    	String command = new String(clsRunSuperHirn.SuperHirnCommand + " -FE "
    			+ mzXMLFile + " " + clsRunSuperHirn.SuperHirnParameterFilePath);
		return this.runCommand(command);
    }
    
    
    /**
     * Runs the SuperHirn XML result parsing and importing into PASS database
     * @return int 
     */
    private int runSuperHirnDBPusher() 
    {
		String command = new String(clsRunSuperHirn.dbPusherCommand + " " + clsRunSuperHirn.SuperHirnOutPutPath);
		return this.runCommand(command);
    }
    
    /**
     * Runs the SuperHirn XML result cleanup
     * @return int 
     */
    private int cleanUpSuperHirnResults() 
    {
		String command = new String(clsRunSuperHirn.cleanupSHCommand + " " + SuperHirnOutPutPath);
		return this.runCommand(command);
    }

    /**
     * Clean up of a file 
     * @return int 
     */
    private int cleanUpFile(String iFile) 
    {    
		String command = new String("rm -rf " + iFile);
		return this.runCommand(command);
    }

    /**
     * Converts a xTandem result file to pepXML file
     * @param xtandemFile String path to the xtandem file
     * @return int 
     */
    private String convertTandemToPepXML(String iFile) 
    {
    	String pepXML = iFile.substring(
    			iFile.lastIndexOf( "/" ) + 1);
    	pepXML = clsRunSuperHirn.tandemToXMLOutputDir + pepXML;

    	String command = new String(clsRunSuperHirn.tandemToXMLCommand 
				+ " " + iFile
				+ " " + pepXML);
		if( this.runCommand(command) != -1 )
		{
			this.cleanUpFile(iFile);
			return pepXML;
		}
		return null;
    }
    
    /**
     * Executes a command, prints the command output to the console and returns an integer. The integer
     * represents the status of the command exectution (see Process)
     * @return int 
     */
    private int runCommand(String Command)
    {
    	
    	/*
    	 * Runnning SuperHirn processing in verbose mode where commands are not execute
    	 */
    	if( VERBOSE )
    	{
    		System.out.println( Command );
    		return 1;
    	}
    	
    	int exitVal = -1;
		try {
	    	Process p;
			p = Runtime.getRuntime().exec(Command, null);

			BufferedReader stdInput = new BufferedReader(new InputStreamReader(
					p.getInputStream()));
			BufferedReader stdError = new BufferedReader(new InputStreamReader(
					p.getErrorStream()));

			// read the output from the command
			String s = null;
			errTime = new Date();
			System.out.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ " Here is the standard output of the command:\n");
			while ((s = stdInput.readLine()) != null) {
				System.out.println(s);
			}

			// Wait for process to complete
			exitVal = p.waitFor();

			// Wait for process to complete
			// p.waitFor();
			// exitVal = p.exitValue();

			errTime = new Date();
			System.out.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ " Process Exit Value : " + exitVal);

			// read any errors from the attempted command
			errTime = new Date();
			System.out.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ " Here is the standard error of the command (if any):\n");
			while ((s = stdError.readLine()) != null) {
				System.out.println(s);
			}

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch(InterruptedException e)
		{
		}

		return exitVal;

    }
    
    
    private String downloadMzXMLFile( String iKey)
    {
    	String fileName = this.getMzXMLFile(iKey);      	
    	fileName = fileName.toLowerCase();
    	if( !this.getFromAmazon(fileName) )
    	{
    		return null;
    	}
    	    	
    	// remove .gz and add .mzXML:
    	fileName = fileName.substring(0, fileName.lastIndexOf("."));
    	return fileName;  
    }

    private String downloadXTandemFile( String iKey)
    {
    	String fileName = this.getTandemFile(iKey); 
    	fileName = fileName.toLowerCase();
    	if( !this.getFromAmazon(fileName) )
    	{
    		return null;
    	}
    	// remove .gz and add .mzXML:
    	fileName = fileName.substring(0, fileName.lastIndexOf("."));
    	return fileName;  
 	
    }
    
    
    private boolean getFromAmazon( String iFile){
   
    	if( VERBOSE )
    	{
    		System.out.println( "Downloading from amazon file " + iFile);
    		return true;
    	}
		// Amazon S3 storage
		clsAmazon oAmazonS3 = new clsAmazon();  
		oAmazonS3.ConnectToAmazon(); 
		oAmazonS3.SetBucket(iFile);
		if( !oAmazonS3.DownloadFile(iFile) )
		{
			errTime = new Date();
			System.out
					.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
							DateFormat.MEDIUM).format(errTime).toString()
							+ " Unable to download file " + iFile); 
			return false;
		}
		
    	this.cleanUpFile(iFile);
		return true;
		
		
		
		
		/*
		oAmazonS3.SetBucket(objBuildXMLFile
				.CurrentXMLOutputFile());

		// Patrick Compress the file which will add
		// a .gz to the filename
		zippedFileName = objBuildXMLFile
				.CurrentXMLOutputFile();
		if (compressFile(
				objBuildXMLFile
						.SearchOutputLocation()
						.toLowerCase()
						+ objBuildXMLFile
								.CurrentXMLOutputFile(),
				objBuildXMLFile
						.SearchOutputLocation()
						.toLowerCase()
						+ objBuildXMLFile
								.CurrentXMLOutputFile()
						+ ".gz", false) == true) {
			zippedFileName = objBuildXMLFile
					.CurrentXMLOutputFile()
					+ ".gz";
		}
		// Upload file to Bucket. If it fails, set
		// the processing status fields accordingly
		if (oAmazonS3.UploadFile(objBuildXMLFile
				.SearchOutputLocation()
				.toLowerCase()
				+ zippedFileName.toLowerCase())) {
			searchDate = new java.util.Date();
			i_UPDATE_Result = Main.objDataAccess
					.WriteRecord("UPDATE to_ms_file SET transmission_status = 'SEARCHED SUCCESSFULLY',end_search_datetime = '"
							+ dateFormat
									.format(searchDate)
							+ "' WHERE to_ms_file_key = "
							+ arSearchList[0][iSearchListCount]
							+ " AND tmx_key = "
							+ arSearchList[1][iSearchListCount]);
			String deletedFile = oAmazonS3
					.delTempFile();
			if (deletedFile.compareTo("\n") != 0) {
				errTime = new Date();
				System.out.println(DateFormat
						.getDateTimeInstance(
								DateFormat.MEDIUM,
								DateFormat.MEDIUM)
						.format(errTime).toString()
						+ deletedFile);
			}
		} else {
			searchDate = new java.util.Date();
			i_UPDATE_Result = Main.objDataAccess
					.WriteRecord("UPDATE to_ms_file SET transmission_status = 'ERROR SEARCHING',end_search_datetime = '"
							+ dateFormat
									.format(searchDate)
							+ "' WHERE to_ms_file_key = "
							+ arSearchList[0][iSearchListCount]
							+ " AND tmx_key = "
							+ arSearchList[1][iSearchListCount]);
			iXTandemSearchResult = 1;
		}

		// Kill the Amazon object
		oAmazonS3 = null;
	*/
	}
    
}

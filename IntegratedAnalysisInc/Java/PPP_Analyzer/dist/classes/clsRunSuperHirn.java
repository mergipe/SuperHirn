/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.io.*;
import java.util.Date;
import java.util.Vector;
import java.util.zip.GZIPOutputStream;
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
    private boolean VERBOSE = true;
    
    /**
     * command to start SuperHirn
     */
    static private String SuperHirnCommand = "SuperHirnv03 ";

    /**
     * Path to SuperHirn output directory
     */
    static private String SuperHirnOutPutPath = "ANALYSIS_TestData/LC_MS_RUNS/";

    /**
     * Path to SuperHirn MasterMap
     */
    static private String SuperHirnMasterMap = "ANALYSIS_TestData/PROCESSED_MASTER.xml";

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
     * Get the path to the XML file defined by it key from the database table
     * @param iFileKey String table (primary)key of tuple containing the tandem file path
     */
    private String getXMLFile( String iFileKey){
    	
    	String xmlTarget = new String("");
    	try
    	{
     
    		// get the mzXML file path from the table:
			RowSetDynaClass xmlFileRecord;
			xmlFileRecord = Main.objDataAccess
					.getRecordSet("SELECT mzXML_file_location, SH_APML_file_name, SH_XML_file_name " +
							"from to_search_details WHERE to_ms_file_key = "
							+ iFileKey);
			
			// Check if a recordset was returned
			if (!xmlFileRecord.getRows().isEmpty()) {

				DynaBean dbDataRow = (DynaBean) xmlFileRecord.getRows()
						.get(0);
				// Build the load strings
				errTime = new Date();
				xmlTarget = (String) dbDataRow.get("mzXML_file_location") + (String) dbDataRow.get("SH_XML_file_name");
				System.out.println(DateFormat.getDateTimeInstance(
						DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
						.toString()
						+ " clsRunSuperHirn: processing file " + xmlTarget);

			}

		} catch (NotAvailableException e) {
			errTime = new Date();
			System.out
					.println(DateFormat.getDateTimeInstance(
							DateFormat.MEDIUM, DateFormat.MEDIUM)
							.format(errTime).toString()
							+ " Unable to get tandem file for SuperHirn processing. Process will exit.");

      
       
    	}
        
		return xmlTarget;
        
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
		int exitVal = 0;

		/*
		 * copy files from amazon to the instance:
		 */
		String xTandem = this.downloadXTandemFile( mzXMLFileKey );
		if( xTandem == null ){
			System.out.println( "Error in xtandem download/unzipping, stop this thread");	
			return -1;
		}
		System.out.println( "Get XTandem ok:" + exitVal);	
		
		String mzXML = this.downloadMzXMLFile( mzXMLFileKey );
		if( mzXML == null ){
			System.out.println( "Error in mzXML download/unzipping, stop this thread");	
			return -1;
		}
		System.out.println( "Get MZXML ok:"  + exitVal);	
		
		// conversion of xtandem to pepxml format:
		String pepXML = this.convertTandemToPepXML( xTandem );
		if( pepXML == null){
			System.out.println( "Error in xtandem conversion, stop this thread");	
			return -1;
		}
		System.out.println( "Convert PepXML ok:"  + exitVal);	

		// run feature extraction:
		exitVal = this.runSuperHirnFeatureExtraction(mzXML);
		if( exitVal == -1){
			System.out.println( "Error in superhirn processing, stop this thread");	
			return exitVal;
		}
		System.out.println( "SuperHirn processing ok: " + exitVal);	
		
		// import to database:
		//exitVal = this.runSuperHirnDBPusher();
		if( exitVal == -1){
			System.out.println( "Error in SuperHirn data import, stop this thread");	
			return exitVal;
		}
		System.out.println( "SuperHirn data import ok: " + exitVal);	
		
		
		// clean up superhirn results:
		exitVal = this.uploadXMLFiles();
		if( exitVal == -1){
			return exitVal;
		}
		System.out.println( "Upload XML SuperHirn results ok: " + exitVal);	
		
		
		
		// clean up superhirn results:
		exitVal = this.cleanUpSuperHirnResults();
		if( exitVal == -1){
			return exitVal;
		}
		System.out.println( "Clean SuperHirn results ok: " + exitVal);	
		
		// clean up pepXML conversion files:
		exitVal = this.cleanUpFile(pepXML);
		System.out.println( "Clean PepXML results ok: " + exitVal);	
		exitVal = this.cleanUpFile(mzXML);
		System.out.println( "Clean mzXML results ok: " + exitVal);	
		exitVal = this.cleanUpFile(mzXML + ".gz");
		System.out.println( "Clean mzXML.gz results ok: " + exitVal);	

		return exitVal;
    }
    
    /**
     * Performs a alignment of input file
     * @param String[] xmlFiles array of download paths from the AMI
     * @return int 
     */
    public int lcmsAlignment(Vector<Integer> xmlFiles) 
    {
		// Initialize return value
		int exitVal = 0;

		/*
		 * copy XML files from amazon to the instance:
		 */
		exitVal = this.downloadXMLFiles( xmlFiles );
		if( exitVal == -1 ){
			System.out.println( "Error in xtandem download/unzipping, stop this thread");	
			return exitVal;
		}
		System.out.println( "Getting XML files ok:" + exitVal);	
		

		// run feature extraction:
		exitVal = this.runSuperHirnAlignment();
		if( exitVal == -1){
			System.out.println( "Error in superhirn alignment, stop this thread");	
			return exitVal;
		}
		System.out.println( "SuperHirn processing ok: " + exitVal);	
		
		// import to database:
		// adopt here the database pusher:
		// exitVal = this.runSuperHirnDBPusher();
		if( exitVal == -1){
			System.out.println( "Error in SuperHirn data import, stop this thread");	
			return exitVal;
		}
		System.out.println( "SuperHirn data import ok: " + exitVal);	
		
		// clean up superhirn xml results:
		exitVal = this.cleanUpSuperHirnResults();
		if( exitVal == -1){
			return exitVal;
		}
		System.out.println( "Clean SuperHirn xml results ok: " + exitVal);	
		
		// clean up the MasterMap:
		exitVal = this.cleanUpFile(SuperHirnMasterMap);
		System.out.println( "Clean MasterMap: " + exitVal);	
	
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
     * Runs SuperHirn alignment 
     * @return int 
     */
    private int runSuperHirnAlignment() 
    {
    	String command = new String(clsRunSuperHirn.SuperHirnCommand + " -CM "
    			+ clsRunSuperHirn.SuperHirnParameterFilePath);
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
    	String pepXMLFileName = iFile.substring(
    			iFile.lastIndexOf( "/" ) + 1);
    	
    	// move first file one folder up:
    	// this is added to move the file one folder up to where the mzXML is
    	// this because for tandem2xml conversion the mzXML needs to be in the same folder as the xtandem file:
    	String movedPepXML = iFile.substring( 0,
    			iFile.lastIndexOf( "/" ) );
    	movedPepXML = movedPepXML.substring( 0,
    			movedPepXML.lastIndexOf( "/" ) + 1) + pepXMLFileName;
    	
    	String mvCommand = "mv " + iFile + " " + movedPepXML;
		if( this.runCommand(mvCommand) != 0 )
		{
			return null;
		}
    	
    	
    	String pepXML = clsRunSuperHirn.tandemToXMLOutputDir + pepXMLFileName;

    	String command = new String(clsRunSuperHirn.tandemToXMLCommand 
				+ " " + movedPepXML
				+ " " + pepXML);
		if( this.runCommand(command) == 0 )
		{
			this.cleanUpFile( movedPepXML );
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
    		return 0;
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


			// read any errors from the attempted command
			if( exitVal != 0){
				errTime = new Date();

				System.out.println(DateFormat.getDateTimeInstance(
						DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
						.toString()
						+ " Process Exit abnormaly with value: " + exitVal);

				System.out
						.println(DateFormat.getDateTimeInstance(
								DateFormat.MEDIUM, DateFormat.MEDIUM).format(
								errTime).toString()
								+ " Here is the standard error of the command (if any):\n");
				while ((s = stdError.readLine()) != null) {
					System.out.println(s);
				}
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
    
    private int downloadXMLFiles( Vector<Integer> iKeys)
    {
    	
    	for( int i=0;i<iKeys.size(); i++ )
    	{
    		// get file from database:
    		String key = new String();
    		key = key + iKeys.get(i);
    		String fileName = this.getXMLFile(key); 
			if (!this.getFromAmazon(fileName.toLowerCase())) {
				return -1;
			}
		}
    	
    	return 0;
 	
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
	
    }
    
    private int uploadXMLFiles()
    {      	
    	
    	File file = new File( clsRunSuperHirn.SuperHirnOutPutPath );
    	if( file.isDirectory() )
    	{
    		
        	String[] files = file.list();
        	System.out.println( file.getAbsolutePath() + "is a directory with files:" + files.length);	

    		String location = "";
    		// get the storage location:
    		if( files.length > 0){
    			location = "as3/jenny_55555/sitt/";    			
    		}
    		
    	
    		for( int i=0; i<files.length; i++)
    		{
        		if( this.uploadFile(clsRunSuperHirn.SuperHirnOutPutPath, files[i], location) != 0 ){
        			return -1;
        		}
    		}
    	
    	}
    	else{
        	System.out.println( file.getAbsolutePath() + "is NOT a directory");	
    	}
    	return 0;
    }
 
    
    
    private int uploadFile(String iSourceLocation, String iFile, String iTargetLocation)
    {      	
    	

    	// Amazon S3 storage
    	clsAmazon oAmazonS3 = new clsAmazon(); // Instantiate
												// new
												// custom
												// Amazon S3
												// object
    	oAmazonS3.ConnectToAmazon(); // Connect to
    	oAmazonS3.SetBucket( iFile);

    	String gzFile = iFile.toLowerCase() + ".gz";
    	
    	System.out.println( "Compressing to " + gzFile);	
		
    	if( this.compressFile( iSourceLocation + iFile, gzFile, false ) )
		{
			
	    	System.out.println( "Uploading file" + gzFile + " to " + iTargetLocation);	

	    	// Upload file to Bucket. If it fails, set
			// the processing status fields accordingly
			oAmazonS3.UploadFile(gzFile);
			return 0;

		}
		return -1;
    }
	
    
	
    /**
	 * This procedure compresses a file to the .gz or .gzip zip format and may
	 * retain or delete the original uncompressed file.
	 * 
	 * @param inputFile
	 *            The complete path to the file we wish to decompress
	 * @param outputFile
	 *            The complete path to where we wish to store the uncompressed
	 *            file
	 * @param keepCompressed
	 *            Whether to retain the original compressed file
	 * @return A boolean value indicating success or failure of the
	 *         decompression
	 */
	public boolean compressFile(String inputFile, String outputFile,
			boolean keepOriginal) {
		GZIPOutputStream outStream;
		FileInputStream inStream;
		byte[] buf;
		int len;
							boolean isZipped = false;
		File delSource;

		try {
			isZipped = false;
			// The file to write out.
			outStream = new GZIPOutputStream(new FileOutputStream(outputFile));

			// The original file that we shall compress;
			inStream = new FileInputStream(inputFile);

			buf = new byte[1024];
			while ((len = inStream.read(buf)) > 0) {
				outStream.write(buf, 0, len);
			}
			inStream.close();

			outStream.finish();
			outStream.close();

			// If keepCompressed is false delete the source file otherwise keep
			// them both
			if (keepOriginal == false) {
				delSource = new File(inputFile);
				delSource.delete();
			}

			isZipped = true;
		} catch (SecurityException se) {
			// Will occur if the user does not have delete rights
			errTime = new Date();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ " Error occured in class 'clsProcessItem': ");
			se.printStackTrace();
			Main.emailError.SendEmail();
			System.exit(1);
			return true; // Because we have unzipped ok only deletion failed
		} catch (Exception ex) {
			errTime = new Date();
			Main.currErrorMessage = "Error occured in class 'clsProcessItem' while uncompressing file: "
					+ inputFile + " " + ex.toString();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ " Error occured in class 'clsProcessItem': ");
			ex.printStackTrace();
			Main.emailError.SendEmail();
			System.exit(1);
			return false;
		}
		return isZipped;
	}
    
}

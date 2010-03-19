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
 * @author Lukas N. Mueller
 */
public class clsRunSuperHirn{

    private Date errTime;
    private EmailDevTeam emailError = new EmailDevTeam();
    
    

    /**
     * command to start SuperHirn
     */
    //static private String SuperHirnCommand = "/Users/muellelu/bin/SuperHirnv03 ";
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
    static private String PepXMLFileFolder = "/root/DATA/pepXML/";

    /**
     * command to start Superhirn result to database import process
     */
    static private String dbPusherCommand = "java -jar Java/SuperHirnDBPusher/distributions/SuperDbPusherv0.1/SuperDbPusher.jar ";
    
    /**
     * command to cleanup Superhirn results 
     */
    static private String tandemToXMLCommand = "tandem2xml ";

    /**
     * command to cleanup Superhirn results 
     */
    static private String cleanupPepXMLCommand = "rm -rf ";
    
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
					.getRecordSet("SELECT mzXML_file_name from to_ms_file WHERE to_ms_file_key = "
							+ iFileKey);
			
			// Check if a recordset was returned
			if (!mzXMLFileRecord.getRows().isEmpty()) {

				DynaBean dbDataRow = (DynaBean) mzXMLFileRecord.getRows()
						.get(0);
				// Build the load strings
				errTime = new Date();
				mzXMLTarget = (String) dbDataRow.get("mzXML_file_name");
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
     * Performs a label free quantification workflow on a mzXML file which is defined by its
     * mysql primary tuple key.
     * @param mzXMLFileKey String primary key for identifying the corresponding tuple in the table to_ms_file
     * @return int 
     */
    public int labelfreeQuantificationOnMzXMLFile(String mzXMLFileKey) 
    {
		// Initialize return value
		int exitVal = 1;

		// conversion of xtandem to pepxml format:
		/*
		exitVal = this.convertTandemToPepXML(xtandemFile);
		if( exitVal != 0){
			return exitVal;
		}
		*/

		// run feature extraction:
		exitVal = this.runSuperHirnFeatureExtraction(mzXMLFileKey);
		if( exitVal != 0){
			return exitVal;
		}
		
		// import to database:
		exitVal = this.runSuperHirnDBPusher();
		if( exitVal != 0){
			return exitVal;
		}
		
		// clean up superhirn results:
		exitVal = this.cleanUpSuperHirnResults();
		if( exitVal != 0){
			return exitVal;
		}
		
		// clean up pepXML conversion files:
		exitVal = this.cleanUpPepXMLFiles();
		if( exitVal != 0){
			return exitVal;
		}

		return exitVal;
    }

    /**
     * Runs SuperHirn feature extraction on the input mzXML file table key
     * @param mzXMLFileKey String mzXML file path
     * @return int 
     */
    private int runSuperHirnFeatureExtraction(String mzXMLFileKey) 
    {
		// Initialize return value
		int exitVal = 1;
		String mzXMLTarget = this.getMzXMLFile(mzXMLFileKey);
		if (!mzXMLTarget.isEmpty()) {
			String command = new String( 
					clsRunSuperHirn.SuperHirnCommand + " -FE TestingData/mzXML/" + mzXMLTarget 
					+ " " +
					clsRunSuperHirn.SuperHirnParameterFilePath
					);
			
			exitVal = this.runCommand(command);
		}
		return exitVal;
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
     * Runs the pepXML converions file cleanup
     * @return int 
     */
    private int cleanUpPepXMLFiles() 
    {
		String command = new String(clsRunSuperHirn.cleanupPepXMLCommand 
				+ " " + clsRunSuperHirn.PepXMLFileFolder + "*");
		return this.runCommand(command);
    }

    /**
     * Converts a xTandem result file to pepXML file
     * @param xtandemFile String path to the xtandem file
     * @return int 
     */
    private int convertTandemToPepXML(String xtandemFile) 
    {
		String command = new String(clsRunSuperHirn.tandemToXMLCommand 
				+ " " + xtandemFile
				+ " " + clsRunSuperHirn.PepXMLFileFolder + xtandemFile);
		return this.runCommand(command);
    }
    
    /**
     * Executes a command, prints the command output to the console and returns an integer. The integer
     * represents the status of the command exectution (see Process)
     * @return int 
     */
    private int runCommand(String Command)
    {
    	
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
}

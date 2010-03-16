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
 * Class which executes the SuperHirn command by either:
 * - passing a single mzXML file to SuperHirn
 *
 * @author Lukas N Mueller
 */
public class clsRunSuperHirn{

    private Date errTime;
    private EmailDevTeam emailError = new EmailDevTeam();
    
    

    /*
     * command to start SuperHirn
     */
    static private String SuperHirnCommand = "/Users/muellelu/bin/SuperHirnv03 ";

    /*
     * Path to SuperHirn output directory
     */
    static private String SuperHirnOutPutPath = "ANALYSIS_TestData/LC_MS_RUNS/ ";

    /*
     * Path to SuperHirn parameter file
     */
    static private String SuperHirnParameterFilePath = "-PARAM /root/param.def";
    
    /*
     * command to start Superhirn result to database import process
     */
    static private String dbPusherCommand = "java -jar Java/SuperHirnDBPusher/distributions/SuperDbPusherv0.1/SuperDbPusher.jar ";
    
    /*
     * command to cleanup Superhirn results 
     */
    static private String cleanupSHCommand = "rm -rf ";
    
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

    public int runSuperHirn(String mzXMLFileKey) 
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
    
    
    public int runSuperHirnDBPusher() 
    {
		String command = new String(clsRunSuperHirn.dbPusherCommand + " " + clsRunSuperHirn.SuperHirnOutPutPath);
		return this.runCommand(command);
    }
    
    public int cleanUpSuperHirnResults() 
    {
		String command = new String(clsRunSuperHirn.cleanupSHCommand + " " + SuperHirnOutPutPath);
		return this.runCommand(command);
    }
    
    
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

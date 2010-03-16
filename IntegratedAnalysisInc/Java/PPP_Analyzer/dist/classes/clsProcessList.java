/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.text.*;
import java.util.Date;
import java.util.Iterator;
import org.apache.commons.beanutils.DynaBean;
import org.apache.commons.beanutils.RowSetDynaClass;

/**
 *
 * @author User
 */
public class clsProcessList {

    // Declare private variables
    private clsProcessItem[] colProcessItems;
    //private clsProcessItem oProcessItem;
    private String sTargetDatabase, mainDatabase = null;
    public clsProcessItem oProcessItem; //Patrick 09092009 changed to public. This is so that we can get the active database to get Email addresses shouls an exception occur
    private Date errTime;
    //private EmailDevTeam emailError = new EmailDevTeam();

    public clsProcessList(String mainDb) {
        // Instantiate the class
        colProcessItems = new clsProcessItem[0];
        sTargetDatabase = "";
        mainDatabase = mainDb;
    }

    public String ActiveTargetDatabase() {
        return sTargetDatabase;
    }

    public clsProcessItem[] ProcessList() {
        return colProcessItems;
    }

    public boolean LoadProcessList(String sProcess) {

        // Declare variables
        RowSetDynaClass rsRecordset = null;
        boolean returnValue = true;

        int iRecordCount = 0;

        // Check that a Process has been specified
        if (sProcess.length() == 0) {
            errTime = new Date();
            Main.currErrorMessage = "Please specify the process to execute e.g. 'XTANDEM'.";
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Please specify the process to execute e.g. 'XTANDEM'.");
            Main.emailError.SendEmail();
            return false;
        }

        // Connect to the MySQL database
        try {
            Main.objDataAccess.SetDatabase(mainDatabase);
            rsRecordset = Main.objDataAccess.getRecordSet("SELECT * FROM tm_process_list WHERE process_name = '" + sProcess + "' AND status = 'ACTIVE'");

            // Get the number of rows to process
            colProcessItems = new clsProcessItem[rsRecordset.getRows().size()];

            // Check if the array has elements
            if (colProcessItems.length == 0) {
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " There is no list to process...");
                rsRecordset = null;
                return false;
            }

            // Loop through  the recordset and store the values in the Process List
            //do {

            Iterator iter = rsRecordset.getRows().iterator();

            while(iter.hasNext()){

                DynaBean dbDataRow = (DynaBean) iter.next();

                // Instantiate a Process Item object
                oProcessItem = new clsProcessItem();

                // Store data from recordset
                oProcessItem.SetStageDatabase((String) dbDataRow.get("db_stage"));
                oProcessItem.SetTargetDatabase((String) dbDataRow.get("db_target"));
                oProcessItem.SetProcessName((String) dbDataRow.get("process_name"));
                oProcessItem.SetStatus((String) dbDataRow.get("status"));

                // Add to collection
                colProcessItems[iRecordCount] = oProcessItem;
                iRecordCount++;
            }


            //} while (rsRecordset.next());

            // Close the recordset
            rsRecordset = null;

            // Loop through the Process Items list and load the search list for each    
            for (int iProcessCount = 0; iProcessCount <= colProcessItems.length - 1; iProcessCount++) {
                oProcessItem = colProcessItems[iProcessCount];
                sTargetDatabase = oProcessItem.ActiveTargetDatabase();

                
                //SIK09092009 Branch out - LoadSearchList for both processes are significantly different and have been seperated
                //for now. Refactoring may be required later
                if (sProcess.toLowerCase().equals("xtandem")) {
                    oProcessItem.LoadSearchList();
                } 
                else if (sProcess.toLowerCase().equals("generic_script_mode")) {
                    oProcessItem.LoadSuperHirnList();
                } else {
                    oProcessItem.LoadSaxonSearchList();
                }
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "Finished processing " + sTargetDatabase + "...");
            }
            returnValue = true;
        } catch (Exception eX) {
            errTime = new Date();
            Main.currErrorMessage = "Error occured in class 'clsProcessList': " + eX.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsProcessList': ");
            rsRecordset = null;
            //SIK09092009 increament threaderrors in case we are using Saxon. No harm even if we are not.
            eX.printStackTrace();
            Main.numThreadErrors++;
            Main.emailError.SendEmail();
            System.exit(1);
            returnValue = false;
        }
        return returnValue;
    }
}

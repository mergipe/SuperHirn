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
 * @author Edwin
 */
public class clsSearchList {

    //Local variables
    private Date errTime;
    //private EmailDevTeam emailError = new EmailDevTeam();

    public clsSearchList() {
    }

    public String[][] GetSearchList() {

        // Initialize Variables       
        int iRecordCount = 0;
        //int[][] arSearchList = {{},{}};
        String s_To_MS_File_Key = "";
        String s_Tmx_Key = "";
        int iStringTrim = 0;
        RowSetDynaClass rsRecordset = null;
        //SIK String to hold the instance ID
        String ec2_Instance_ID = "", arSearchList[][] = null;

        try {

            //SIK Determine the private DNS Name for the instance running this and only pick items marked to be searched using this instance
            java.net.InetAddress i = java.net.InetAddress.getLocalHost();
            ec2_Instance_ID = i.getHostName(); //getHostAddress();

            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Instance ID" + ec2_Instance_ID);

            // get the list for searching
            //SIK append InstanceID to the where clause so that we only read search items for this instance.
            rsRecordset = Main.objDataAccess.getRecordSet("SELECT to_ms_file_key, tmx_key FROM to_ms_file WHERE to_ms_file_key > 1 AND transmission_status = 'READY FOR SEARCH' AND ec2_instance_id= '" + ec2_Instance_ID + "'");

            // Check if a recordset was returned
            if (rsRecordset.getRows().isEmpty()) {
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " clsSearchList.GetSearchList : No records returned for searching");
                return null;
            }

            //System.out.println("Loading Search List...");

            // Reset the counter
            iRecordCount = 0;
            Iterator iter = rsRecordset.getRows().iterator();

            while(iter.hasNext()){

                DynaBean dbDataRow = (DynaBean) iter.next();

                // Build the load strings
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " clsSearchList.GetSearchList : Build the load strings");
                s_To_MS_File_Key = s_To_MS_File_Key + (Integer) dbDataRow.get("to_ms_file_key") + ",";
                s_Tmx_Key = s_Tmx_Key + (Integer) dbDataRow.get("tmx_key") + ",";

            }

            // Load the Keys array
            // Determine how much cropping of the string is required
            if (s_To_MS_File_Key.length() == 0) {
                iStringTrim = 0;
            } else {
                iStringTrim = 1;
            }

            // Build the string
            s_To_MS_File_Key = s_To_MS_File_Key.substring(0, s_To_MS_File_Key.length() - iStringTrim);
            s_Tmx_Key = s_Tmx_Key.substring(0, s_Tmx_Key.length() - iStringTrim);

            String arSearchListTmp[][] = {s_To_MS_File_Key.split(","), s_Tmx_Key.split(",")};
            arSearchList = arSearchListTmp;

            // Close the recordset
            rsRecordset = null;

            //objDataAccess.killMySQLConnection();
            //return arSearchList;

        } catch (Exception eX) {
            errTime = new Date();
            Main.currErrorMessage = eX.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsSearchList': ");
            eX.printStackTrace();
            rsRecordset = null;
            Main.emailError.SendEmail();
            System.exit(1);
            arSearchList = null;
        }
        return arSearchList;
    }
}

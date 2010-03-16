/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package PPP_Analyzer;

/**
 *
 * @author Edwin
 */

import java.sql.*;
import java.text.*;
import java.util.Date;

public class clsResultSet {

    // Declare private variables
    private static int iRowCount = 0;
    private String[][] sDataArray = null;
    private static Date writeTime;
    //private static EmailDevTeam emailError = new EmailDevTeam();

    public clsResultSet(ResultSet rsDataArray) {

        // Store the contents of the provided resultset into the internal array

        try {

            // If the resullset is empty, exit
            if (rsDataArray == null || rsDataArray.isClosed()) {
                sDataArray = null;
                return;
            }

            // Populate the array
            
        }
        catch (SQLException exSQL){
            writeTime = new Date();
            Main.currErrorMessage = exSQL.toString();
            System.err.println("\n" + DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " An empty ResultSet was passed to the custom ResultSet object.");
            System.err.println("SQLState: " + exSQL.getSQLState());
            System.err.println("MySQL Error: " + exSQL.getErrorCode());
            System.err.println("SQLException: " + exSQL.getMessage());
            Main.emailError.SendEmail();
            System.exit(1);
            return;
        } catch (Exception exError) {
            writeTime = new Date();
            Main.currErrorMessage = " Error occurred in custom ResultSet object. " + exError.getMessage();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occurred in custom ResultSet object. " + exError.getMessage());
            exError.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            return;
        }
    }

    public void First(){

        // Move to the first row of the array
        iRowCount = 0;

    }

    public void Last(){

        // Move to the last row of the array
        iRowCount = sDataArray.length - 1;
        
    }

    public void Next(){

        // Move to the next row of the array
        if (iRowCount < sDataArray.length - 1) {
            iRowCount++;
        }
        else {
            Last();
        }

    }

    public void Previous(){

        // Move to the previous row of the array
        if (iRowCount > 0) {
            iRowCount--;
        }
        else {
            First();
        }

    }

}

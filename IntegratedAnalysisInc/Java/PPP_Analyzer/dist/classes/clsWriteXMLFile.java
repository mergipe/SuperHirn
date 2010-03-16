/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.io.*;
import java.util.Date;
import java.text.*;

/**
 * This class write out a string containing XML file code to a specified file.
 * 
 * @author Edwin
 */
public class clsWriteXMLFile {

    // Declare private variables
    private static String sXMLPath;
    private static String sXMLCode;
    private Date errTime;
    //private EmailDevTeam emailError = new EmailDevTeam();

    public clsWriteXMLFile(String sXMLFilePath, String sXMLFileCode) {

        // Store parameters to private variables
        sXMLPath = sXMLFilePath.toLowerCase();
        sXMLCode = sXMLFileCode;

    }

    public boolean WriteFile(String sStorage_Location) {

        // Declare variable
        FileWriter outFile = null;

        try {

            // If the sXMLCode variable is empty, return a false
            if (sXMLCode.length() == 0) {
                // Display success message
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " The XML file was not created!");
                return false;
            }

            if (sStorage_Location.contains("AS3")) {
                outFile = new FileWriter("./" + sXMLPath);
            } else {
                outFile = new FileWriter(sXMLPath);
            }

            PrintWriter out = new PrintWriter(outFile);

            // Write text to file
            out.println(sXMLCode);

            out.close();
        } catch (IOException e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            e.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
        }

        // Return positive result
        return true;
    }
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.io.*;
import java.util.Date;
import java.text.*;

/**
 *
 * @author Edwin
 */
public class clsRunXtandem {

    private Date errTime;
    private EmailDevTeam emailError = new EmailDevTeam();

    public int clsRunXtandem(String s_XML_File) {

        // Initialize return value
        int exitVal = 1;

        // Check that the parameter has been specified
        if (s_XML_File.length() == 0) {
            return exitVal;
        }

        try {

            Process p = Runtime.getRuntime().exec("/usr/local/applications/xtandem/bin/tandem.exe " + s_XML_File.toLowerCase(), null);

            BufferedReader stdInput = new BufferedReader(new InputStreamReader(p.getInputStream()));
            BufferedReader stdError = new BufferedReader(new InputStreamReader(p.getErrorStream()));

            // read the output from the command
            String s = null;
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Here is the standard output of the command:\n");
            while ((s = stdInput.readLine()) != null) {
                System.out.println(s);
            }

            // Wait for process to complete
            exitVal = p.waitFor();

            // Wait for process to complete
            //p.waitFor();
            //exitVal = p.exitValue();

            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Process Exit Value : " + exitVal);

            // read any errors from the attempted command
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Here is the standard error of the command (if any):\n");
            while ((s = stdError.readLine()) != null) {
                System.out.println(s);
            }

            return exitVal;

        } catch (IOException ioe) {
            errTime = new Date();
            Main.currErrorMessage = ioe.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " " + ioe.getMessage());
            Main.emailError.SendEmail();
            System.exit(1);
            return exitVal;
        } catch (Throwable t) {
            errTime = new Date();
            Main.currErrorMessage = t.toString();
            t.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            return exitVal;
        }

    }
}

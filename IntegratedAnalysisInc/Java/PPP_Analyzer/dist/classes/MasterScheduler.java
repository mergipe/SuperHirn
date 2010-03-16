/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.util.Date;
import java.text.*;

/**
 *
 * @author Irungu.Kamau
 */
public class MasterScheduler {

    static clsScheduler objScheduler;
    private Date errTime;
    //private EmailDevTeam emailError = new EmailDevTeam();

    public MasterScheduler() {
    }

    /*
    public boolean Schedule() {
        //added boolean return value
        try {
            //java.net.InetAddress i = java.net.InetAddress.getLocalHost();
            //System.out.println("IP address for this Instance : " + i.getHostName());
            //System.exit(1);

            objScheduler = new clsScheduler();

            objScheduler.StartSearchProcess();

            return true;
        } catch (Exception e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : MasterScheduler.Schedule: ");
            e.printStackTrace();
            emailError.SendEmail();
            return false;
        }
    }

    public boolean Schedule(String database, String username, String password) {
        //added boolean return value
        try {
            //java.net.InetAddress i = java.net.InetAddress.getLocalHost();
            //System.out.println("IP address for this Instance : " + i.getHostName());
            //System.exit(1);

            objScheduler = new clsScheduler(database, username, password);

            objScheduler.StartSearchProcess();

            return true;
        } catch (Exception e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : MasterScheduler.Schedule: ");
            e.printStackTrace();
            emailError.SendEmail();
            return false;
        }
    }
    */

    public boolean Schedule(String hostName, String database, String username, String password, int port) {
        //added boolean return value
        try {
            //java.net.InetAddress i = java.net.InetAddress.getLocalHost();
            //System.out.println("IP address for this Instance : " + i.getHostName());
            //System.exit(1);

            objScheduler = new clsScheduler(hostName, database, username, password, port);

            objScheduler.StartSearchProcess();

            return true;
        } catch (Exception e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : MasterScheduler.Schedule: ");
            e.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            return false;
        }
    }
}

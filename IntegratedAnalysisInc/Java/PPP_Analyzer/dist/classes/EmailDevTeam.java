/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.net.InetAddress;
import java.net.UnknownHostException;
import javax.mail.Authenticator;
import java.text.*;
import java.util.Date;
import java.util.Properties;
import javax.mail.Message;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
import java.util.Iterator;
import javax.mail.MessagingException;
import javax.mail.PasswordAuthentication;
import org.apache.commons.beanutils.DynaBean;
import org.apache.commons.beanutils.RowSetDynaClass;

/**This class will send an email to a predetermined group of people
 *The list of names will be read from a table where the email addresses are
 * comma separated. Should the database be unavailable the email will still be
 * sent since the same addresses are hardcoded into the class. (Once they change
 * the program will need to be recompiled)
 *
 * @author Patrick
 * @version 1.0.0
 * Date: September 09, 2009
 */
public class EmailDevTeam {
    //Class Declarations

    //private String[] defaultAddressees = {"pnjuguna@i-a-inc.com", "njugunapn@hotmail.com", "slabedo@i-a-inc.com"};
    //private String[] defaultAddressees = {"emulwa@integratedanalysisinc.com", "gsaxena@integratedanalysisinc.com", "mmwangi@i-a-inc.com", "pnjuguna@i-a-inc.com","sirungu@i-a-inc.com"};
    clsDataAccess DataAccess = Main.objDataAccess;
    private String userEmail = "schedulererror@gmail.com", userPwd = "DashItAgain", localAddressees[] = null, defaultAddressees[] = {"slabedo@i-a-inc.com"};
    private Date today;
    private static int emailAttempts = 0;

    /**This procedure accesses the database and retrieves from the tm_variables
     * table the value in the email_addressees variable and then puts them into
     * a string array.
     *
     * @param objDataAccess The class that will be used to connect to the database
     * to retrieve the email addresses that we are going to mail the errors to.
     * @return an array of email addresses or a null string
     */
    private String[] getAddressees() {
        // Declare variables
        //Connection conMySQL = null;
        String[] defAddressees = null;
        String strSQL;
        String rawAddresses = null;
        clsProcessItem localProcessList;
        RowSetDynaClass rsRecordset = null;
        defAddressees = defaultAddressees;

        if (true) {
            /*
             * Using hard corded emails only.
             * Comment out to use database emails as well
             */
            return defAddressees;
        }

        try {
            localProcessList = Main.objProcessList.oProcessItem;
            if (localProcessList != null) {
                Main.objDataAccess.SetDatabase(localProcessList.ActiveTargetDatabase());
                //Get email record from database
                strSQL = "SELECT fn_get_variable_st('email_addressees') as Addresses"; //"SELECT String_Val FROM tm_variables WHERE Var_Name = 'email_addressees'";
                //rawAddresses = sSQLSelect.executeQuery(strSQL).toString();
                rsRecordset = Main.objDataAccess.getRecordSet(strSQL);
                Iterator iter = rsRecordset.getRows().iterator();
                DynaBean dbDataRow = (DynaBean) iter.next();
                rawAddresses = (String) dbDataRow.get("Addresses");
                defAddressees = rawAddresses.split(",");
            }
        } catch (NotAvailableException ex) {
            today = new Date();
            Main.currErrorMessage = "Error: Not Available Exception in Email Class";
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(today).toString() + "Error: Not Available Exception in Email Class\n" + ex.getMessage());
        } catch (Exception ex) {
            today = new Date();
            Main.currErrorMessage = "Error getting email addresses";
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(today).toString() + "Failed to get email addresses\n" + ex.getMessage());
        }
        return defAddressees;
    }

    /**
     * This function sends an email to a group of predetermined people that are
     * held in a database table.
     *
     */
    public void SendEmail(boolean blnGetAddressFromDB) {
        //String[] localAddressees = null;
        String from = userEmail; //"pnjuguna@i-a-inc.com";
        String subject;
        String bodyText;
        Properties props;
        Authenticator auth;
        Transport transport;
        Message message;
        InternetAddress[] recipients;
        boolean bSuccessStatus = false;

        today = new Date();
        subject = Main.processName + " Process: Attention Errors have occurred in DB: " + Main.objDataAccess.GetDatabase();
        bodyText = "Errors have occurred while processing files. Please refer to the error log file to resolve the issues.\n";
        bodyText = bodyText + "The most recent error occurred on " + DateFormat.getDateInstance(DateFormat.LONG).format(today).toString() + " at " + DateFormat.getTimeInstance(DateFormat.MEDIUM).format(today).toString();
        if (Main.currErrorMessage != null) {
            bodyText = bodyText + "\n The error is described below. \n" + Main.currErrorMessage;
        }

        // A properties to store mail server smtp information such as the host
        // name and the port number. With these properties we create a Session
        // object from which we'll create the Message object.
        props = new Properties();
        props.put("mail.smtp.starttls.enable", "true");
        props.put("mail.transport.protocol", "smtp");
        props.put("mail.smtp.host", "smtp.gmail.com"); //"smtp.gmail.com");
        props.put("mail.smtp.port", "587"); //587
        props.put("mail.smtp.auth", "true");
        props.put("mail.user", userEmail);
        props.put("mail.password", userPwd);
        props.put("mail.smtp.connectiontimeout", 120000);
        props.put("mail.smtp.timeout", 120000);

        Session session = Session.getInstance(props, new SMTPAuthenticator(userEmail, userPwd));

        do {
            int sendTo = 0;
            try {
                //
                // Message is a mail message to be sent through the Transport object.
                // In the Message object we set the sender address and the recipient
                // address(es). Both of these addresses are a type of InternetAddress.
                // For the recipient address we can also set the type of recipient,
                // the value can be TO, CC or BCC.
                InetAddress addr = InetAddress.getLocalHost();
                transport = session.getTransport();
                message = new MimeMessage(session);
                message.setFrom(new InternetAddress(from));

                /*
                 * if blnGetAddressFromDB is true, use email addresses in database, else use hard corded emails
                 */
                if (blnGetAddressFromDB && localAddressees == null) {
                    localAddressees = getAddressees();
                } else {
                    localAddressees = defaultAddressees; //Get and use the defaultAddressees
                }

                recipients = new InternetAddress[localAddressees.length];
                for (String Addressee : localAddressees) {
                    recipients[sendTo] = new InternetAddress(Addressee);
                    sendTo++;
                }

                message.addRecipients(Message.RecipientType.TO, recipients);
                message.setSubject(subject);
                message.setContent("IP Address: \t " + addr.getHostAddress() + " \n HostName: \t " + addr.getCanonicalHostName() + "\n" + bodyText, "text/plain");

                // Send the message to the recipient.
                transport.connect();
                transport.send(message);
                transport.close();

                bSuccessStatus = true;

            } catch (MessagingException ex) {
                today = new Date();
                Main.currErrorMessage = "\nError: MessagingException Occured Sending mail in Email Class";
                System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(today).toString() + ": Error: MessagingException Occured Sending mail in Email Class\n" + ex.getMessage());
                //System.exit(1);
            } catch (UnknownHostException ex) {
                today = new Date();
                Main.currErrorMessage = "\nError: UnknownHostException Occured Sending mail in Email Class";
                System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(today).toString() + ": Error: UnknownHostException Occured Sending mail in Email Class\n" + ex.getMessage());
                //System.exit(1);
            }
            try {
                Thread.currentThread().sleep(20000);
            } catch (InterruptedException ix) {
                System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(today).toString() + ": Sleep routine errored in UnknownHostException catch block within EmailDevTeam class within SendEmail(boolean) method.");
            }
            System.out.println("Sending Mail Attempt: " + (emailAttempts + 1));
        } while (++emailAttempts < 10 && bSuccessStatus == false);

        today = new Date();
        if (bSuccessStatus) {
            Main.currErrorMessage = "\nInfo: Email sent successfully on " + emailAttempts + " attempts in Email Class";
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(today).toString() + ": Info: Email sent successfully on " + emailAttempts + " attempts in Email Class");
        } else {
            Main.currErrorMessage = "\nError: Connection attempts exceeded Sending mail in Email Class";
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(today).toString() + ": Error: Connection attempts exceeded Sending mail in Email Class");
        }
        System.exit(1);
    }

    public void SendEmail() {
        this.SendEmail(true);
    }

    /*
    public void SendEmail() {
    String[] localAddressees = null;
    String from = userEmail; //"pnjuguna@i-a-inc.com";
    int sendTo = 0;
    String subject;
    String bodyText;
    Properties props;
    Authenticator auth;
    Transport transport;
    Message message;
    InternetAddress[] recipients;

    today = new Date();
    //subject = "Java/XTANDEM/SAXON: Attention Errors have occurred in DB: "+Main.objDataAccess.GetDatabase();
    subject = Main.processName + " Process: Attention Errors have occurred in DB: " + Main.objDataAccess.GetDatabase();
    bodyText = "Errors have occurred while processing files. Please refer to the error log file to resolve the issues.\n";
    bodyText = bodyText + "The most recent error occurred on " + DateFormat.getDateInstance(DateFormat.LONG).format(today).toString() + " at " + DateFormat.getTimeInstance(DateFormat.MEDIUM).format(today).toString();
    if (Main.currErrorMessage != null) {
    bodyText = bodyText + "\n The error is described below. \n" + Main.currErrorMessage;
    }

    // A properties to store mail server smtp information such as the host
    // name and the port number. With these properties we create a Session
    // object from which we'll create the Message object.
    props = new Properties();
    props.put("mail.smtp.starttls.enable", "true");
    props.put("mail.transport.protocol", "smtp");
    props.put("mail.smtp.host", "smtp.gmail.com"); //"smtp.gmail.com");
    props.put("mail.smtp.port", "587"); //587
    props.put("mail.smtp.auth", "true");
    props.put("mail.user", userEmail);
    props.put("mail.password", userPwd);
    props.put("mail.smtp.connectiontimeout", 120000);
    props.put("mail.smtp.timeout", 120000);

    //auth = new SMTPAuthenticator();
    //Session session = Session.getDefaultInstance(props);session = Session.getInstance(props,

    //Session session = Session.getDefaultInstance(props, auth); //getInstance(props, new Authenticator( userEmail,userPwd)); //
    Session session = Session.getInstance(props, new SMTPAuthenticator(userEmail, userPwd));

    try {
    //
    // Message is a mail message to be sent through the Transport object.
    // In the Message object we set the sender address and the recipient
    // address(es). Both of these addresses are a type of InternetAddress.
    // For the recipient address we can also set the type of recipient,
    // the value can be TO, CC or BCC.
    InetAddress addr = InetAddress.getLocalHost();
    transport = session.getTransport();
    message = new MimeMessage(session);
    message.setFrom(new InternetAddress(from));
    localAddressees = getAddressees();

    /*Set the Addressees: in case we can't access the database to get the
     * variable  email_addressees we use the hardcoded variable
     * defaultAddressees
     *\/
    if (localAddressees == null) {
    localAddressees = this.defaultAddressees; //Get and use the defaultAddressees
    }

    recipients = new InternetAddress[localAddressees.length];
    for (String Addressee : localAddressees) {
    recipients[sendTo] = new InternetAddress(Addressee);
    sendTo++;
    }
    //recipients = recipients + Addressee + ",";

    //InternetAddress[] address={new InternetAddress(recipients)};
    //message.setRecipient(Message.RecipientType.TO, new InternetAddress(to));
    //message.setRecipients(Message.RecipientType.TO, address);
    message.addRecipients(Message.RecipientType.TO, recipients);
    message.setSubject(subject);
    message.setContent("IP Address: \t " + addr.getHostAddress() + " \n HostName: \t " + addr.getCanonicalHostName() + "\n" + bodyText, "text/plain");

    // Send the message to the recipient.
    transport.connect();
    transport.send(message);
    transport.close();

    } catch (Exception ex) {
    today = new Date();
    Main.currErrorMessage = "Error: Exception Occured Sending mail in Email Class";
    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(today).toString() + "Error: Exception Occured Sending mail in Email Class\n" + ex.getMessage());
    System.exit(1);
    }
    }
     */
    private class SMTPAuthenticator extends javax.mail.Authenticator {
        /*
        @Override
        public javax.mail.PasswordAuthentication getPasswordAuthentication() {
        javax.mail.PasswordAuthentication auth = null;
        String userName = userEmail;
        String passWord = userPwd;

        auth = new javax.mail.PasswordAuthentication(userName, passWord);
        return auth;
        }
         */

        String user;
        String pw;

        public SMTPAuthenticator(String username, String password) {
            super();
            this.user = username;
            this.pw = password;
        }

        public PasswordAuthentication getPasswordAuthentication() {
            return new PasswordAuthentication(user, pw);
        }
    }
}

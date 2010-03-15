/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
//import java.util.Collections;
//import java.util.ArrayList;
import java.util.*;
import com.xerox.amazonws.ec2.Jec2;
import com.xerox.amazonws.ec2.LaunchConfiguration;
import com.xerox.amazonws.ec2.ReservationDescription;
import com.xerox.amazonws.ec2.ReservationDescription.Instance;
import java.util.Date;
import java.text.*;

/**
 *
 * @author Irungu.Kamau
 */
public class EC2 {

    //SIK Set variables required to fire up an instance
    private String sAWSKey = "1X1Z0AMH5909XBVJ3QR2";
    private String sAWSSecretKey = "TM3affMISvWh8Ipg0/D+N+t+2AvCc1Fegstcw6z8";
    private String sKeyName = "scheduler";
    private String sSecurityGroup = "IA";
    //In future, if we want to fire instances of different AMIs, we can make this a settable property of this class
    //Right now we can hardcode the AMI that has X!Tandem
    private String sImageName = "ami-6f4aa706";//"ami-145fbd7d";//ami-9a33d1f3";		//"ami-20d83a49";     //"ami-7f35d616";
    private Instance instance;
    private String sInstanceID, sPublicDNS,sPrivateDNS,availabilityZone;
    private Date errTime;
    //private EmailDevTeam emailError = new EmailDevTeam();

    public EC2(String selectedAmi, String availabilityZone) {
        this.sImageName = selectedAmi;
        this.availabilityZone=availabilityZone;
    }

    /*
    public EC2() {
    }
     */
    public String getInstanceID() {
        return sInstanceID;
    }

    public void setInstanceID(String newvalue) {
        this.sInstanceID = newvalue;
    }

    public String getPublicDNS() {
        return sPublicDNS;
    }

    public String getPrivateDNS() {
        return sPrivateDNS;
    }

    public boolean LaunchEC2Instance() {
        boolean retValue = false;
        try {
            //Specify required details for the instance we want - AMIID, Type
            //We won't worry about Availability zone right now as we are not attaching EBS
            Jec2 jec2 = new Jec2(sAWSKey, sAWSSecretKey);
            LaunchConfiguration launchConfiguration = new LaunchConfiguration(sImageName);
            launchConfiguration.setImageId(sImageName);
            launchConfiguration.setKeyName(sKeyName);
            launchConfiguration.setInstanceType(launchConfiguration.getInstanceType().XLARGE);
            //launchConfiguration.setAvailabilityZone("us-east-1d");
            launchConfiguration.setAvailabilityZone(availabilityZone);

            //Launch the instance
            ReservationDescription reservationDescription = jec2.runInstances(launchConfiguration);
            instance = reservationDescription.getInstances().get(0);
            while (!instance.isRunning() && !instance.isTerminated()) {
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "Instance still starting up; sleeping " + 60000 + "ms");
                //System.out.println("Instance still starting up; sleeping " + 60000 + "ms");
                Thread.sleep(60000);
                //Check if instance has been created
                reservationDescription = jec2.describeInstances(Collections.singletonList(instance.getInstanceId())).get(0);
                instance = reservationDescription.getInstances().get(0);
            }

            if (instance.isRunning()) {
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "EC2 instance is now running : " + instance.getDnsName());

                //Return the instance's Public DNS
                sPublicDNS = instance.getDnsName();
                sInstanceID = instance.getInstanceId();
                sPrivateDNS = instance.getPrivateDnsName();
//  This was used to generate an instance id and name for testing

//                    DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SSS");
//                    java.util.Date date= new java.util.Date();
//                    sPrivateDNS="domU-" + dateFormat.format(date);
//                    sPublicDNS="ec2-" + dateFormat.format(date);
//                    sInstanceID="i-" + dateFormat.format(date);
                retValue = true;
            }
            /*
            No need to thro an exception if we need to terminate system every time exception occurs
            else {
            throw new IllegalStateException("Failed to start a new instance");
            }
             */

        } catch (Exception e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : EC2.LaunchEC2Instance: ");
            e.printStackTrace();
            Main.emailError.SendEmail();
            sPublicDNS = "";
            sPrivateDNS = "";
            sInstanceID = "";
            //System.exit(1);
        }
        return retValue;
    }

    public void TerminateEC2Instances(String sInstances) throws Exception {
        try {
            Jec2 jec2 = new Jec2(sAWSKey, sAWSSecretKey);
            jec2.terminateInstances(new String[]{sInstances});
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "TerminateEC2Instance : Finished terminating instance(s) :" + sInstances);
        } catch (Exception ex) {
            errTime = new Date();
            Main.currErrorMessage = ex.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : EC2.TerminateEC2Instances: ");
            ex.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
        }
    }
}


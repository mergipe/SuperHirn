/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

//import com.amazon.s3.*;
//import org.apache.commons.httpclient.*;
import org.jets3t.service.security.AWSCredentials;
import org.jets3t.service.impl.rest.httpclient.RestS3Service;
import org.jets3t.service.S3Service;
import org.jets3t.service.utils.ServiceUtils;
import org.jets3t.service.multithread.S3ServiceSimpleMulti;
import org.jets3t.service.multithread.DownloadPackage;
import org.jets3t.service.model.S3Bucket;
import org.jets3t.service.model.S3Object;
import java.io.*;
import java.security.*;
import java.util.Date;
import java.text.*;
import org.jets3t.service.S3ServiceException;

/**
 *
 * @author Edwin
 */
public class clsAmazon {

    // Declare variables
    //private AWSAuthConnection connAmazon = null;
    private String sActiveBucket = "";
    private S3Service s3Service = null;
    private static Date errTime;
    //private static EmailDevTeam emailError = new EmailDevTeam();
    File objUploadFile = null;

    public clsAmazon() {
    }

    boolean createBucket(String bucketName) {
        /*
         * testing bucket creation
         */
        ConnectToAmazon();
        try {
            S3Bucket objActiveBucket = s3Service.createBucket(bucketName);
            return true;
        } catch (S3ServiceException ex) {
            return false;
        }
    }

    public void SetBucket(String sCurrentSearchFile) {
        // Set the active bucket based on the type of file being processed
        try {
            // MGF Format
            // Edwin Nov 2009
            if (sCurrentSearchFile.toLowerCase().contains(".mgf")) {
                sActiveBucket = "ia_mgf";
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Set 'ia_mgf' as active Bucket.");
            }

            // mzXML Format
            // Edwin Nov 2009
            if (sCurrentSearchFile.toLowerCase().contains(".mzxml")) {
                sActiveBucket = "ia_mzxml";
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Set 'ia_mzxml' as active Bucket.");
            }

            // mzML
            // Edwin Nov 2009
            if (sCurrentSearchFile.toLowerCase().contains(".mzml")) {
                sActiveBucket = "ia_mzml";
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Set 'ia_mzml' as active Bucket.");
            }

            // XML
            // Edwin Nov 2009
            if (sCurrentSearchFile.toLowerCase().contains(".xml")) {
                sActiveBucket = "ia_xtandem_output";
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Set 'ia_xtandem_output' as active Bucket.");
            }
        } catch (Exception eAM) {
            errTime = new Date();
            Main.currErrorMessage = eAM.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsAmazon.SetBucket': ");
            eAM.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
        }


    }

    public void ConnectToAmazon() {

        try {
            // Connect to the Amazon S3
            //AWSAuthConnection connAmazon = new AWSAuthConnection("AKIAIOOWLQSYJ64HDNEA", "9MfPgKBZMWjoIhZxlzPESRWaaJNhDPKcpZ4Gg21i");
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Initializing connection to Amazon S3...");

            String awsAccessKey = "AKIAIOOWLQSYJ64HDNEA";
            String awsSecretKey = "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";

            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Obtaining Amazon S3 credentials...");
            AWSCredentials awsCredentials = new AWSCredentials(awsAccessKey, awsSecretKey);

            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Connecting to Amazon S3...");
            s3Service = new RestS3Service(awsCredentials);
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Connection to Amazon S3 established!");

        } catch (Exception eAM) {
            errTime = new Date();
            Main.currErrorMessage = eAM.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsAmazon.ConnectToAmazon': ");
            eAM.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
        }

    }

    public boolean DownloadFile(String sFileToDownload) {

        String sCheckSum = "";
        boolean blnValidDownload = false;

        // Get the object
        try {

            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Preparing to download file from Amazon S3...");

            // Check if a bucket has been specified
            if (sActiveBucket.length() == 0) {
                errTime = new Date();
                System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Please specify the bucket from which you want to download files.");
                Main.currErrorMessage = DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Please specify the bucket from which you want to download files.";
                Main.emailError.SendEmail();
                return blnValidDownload;
            }

            S3Bucket objActiveBucket = s3Service.getBucket(sActiveBucket);
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Active bucket has been determined as: " + sActiveBucket);

            S3Object objDownloadFile = s3Service.getObject(objActiveBucket, sFileToDownload);
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Download file has been obtained from S3 bucket.\n\nDownload starting shortly...");

            // The code below will download the file to the hard drive
            // Create a simple multi-threading service based on our existing S3Service
            S3ServiceSimpleMulti simpleMulti = new S3ServiceSimpleMulti(s3Service);

            // Create a DownloadPackage for each object, to associate the object with an output file.
            DownloadPackage[] downloadPackages = new DownloadPackage[1];
            downloadPackages[0] = new DownloadPackage(objDownloadFile, new File(objDownloadFile.getKey()));

            // Download the object.
            simpleMulti.downloadObjects(objActiveBucket, downloadPackages);
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Downloaded file '" + sFileToDownload + "' successfully.");

            // Verify that the downloaded object  was downloaded ok using CheckSum
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Validating the download...");
            //sCheckSum = ServiceUtils.readInputStreamToString(objDownloadFile.getDataInputStream(), "UTF-8");
            //blnValidDownload = objDownloadFile.verifyData(sCheckSum.getBytes("UTF-8"));
            blnValidDownload = objDownloadFile.verifyData(downloadPackages[0].getDataFile());

            // Edwin Nov 2009
            // If the data was verified and was gzipped, gunzip
            if (blnValidDownload && sFileToDownload.endsWith(".gz")) {
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Starting to uncompress file: " + sFileToDownload);

                int iFileGunZipped = GunZipFile(sFileToDownload);

                if (iFileGunZipped == 1) {

                    errTime = new Date();
                    System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Failed to Gunzip file " + sFileToDownload);

                    blnValidDownload = false;
                }
            }

            if (blnValidDownload == false) {
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " File failed the validation check!");

            }
            // Edwin Nov 2009
        } catch (NoSuchAlgorithmException eAM) {
            errTime = new Date();
            Main.currErrorMessage = "Checksum failed. File will not be processed. " + eAM.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Checksum failed. File will not be processed.");
            eAM.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            return false;
        } catch (FileNotFoundException eAM) {
            errTime = new Date();
            Main.currErrorMessage = "Checksum failed. File will not be processed. " + eAM.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Checksum failed. File will not be processed.");
            eAM.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            return false;
        } catch (IOException eAM) {
            errTime = new Date();
            Main.currErrorMessage = "Checksum failed. File will not be processed. " + eAM.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Checksum failed. File will not be processed.");
            eAM.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            return false;
        } catch (Exception eAM) {
            errTime = new Date();
            Main.currErrorMessage = "Error occured in class 'clsAmazon.DownloadFile': " + eAM.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsAmazon.DownloadFile': ");
            eAM.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            return false;
        }

        // Return the status
        return blnValidDownload;

    }

    public boolean DownloadFile(String downloadsFolder, String sFileToDownload, String downloadBucket) {
        // Get the object
        try {
            // Check if a bucket has been specified
            if (downloadBucket.length() == 0) {
                System.out.println("\nPlease specify the bucket from which you want to download files.\n");
                return false;
            }
            if (downloadsFolder.length() == 0) {
                System.out.println("\nPlease specify where to downlad files to.\n");
                return false;
            }

            S3Bucket objActiveBucket = s3Service.getBucket(downloadBucket);
            System.out.println("\nActive bucket has been determined as: " + downloadBucket + "\n");
            S3Object objDownloadFile = s3Service.getObject(objActiveBucket, sFileToDownload);

            System.out.println("\nDownload file has been obtained from S3 bucket.\n\nDownload starting shortly...\n");
            // The code below will download the file to the hard drive
            // Create a simple multi-threading service based on our existing S3Service
            S3ServiceSimpleMulti simpleMulti = new S3ServiceSimpleMulti(s3Service);

            // Create a DownloadPackage for each object, to associate the object with an output file.
            DownloadPackage[] downloadPackages = new DownloadPackage[1];
            //downloadPackages[0] = new DownloadPackage(objDownloadFile, new File(objDownloadFile.getKey()));
            //downloadFilePath is used to specify a downloads location on local file system
            downloadPackages[0] = new DownloadPackage(objDownloadFile, new File(downloadsFolder + "" + objDownloadFile.getKey().substring(objDownloadFile.getKey().lastIndexOf("/"))));

            // Download the object.
            simpleMulti.downloadObjects(objActiveBucket, downloadPackages);
            System.out.println("\nDownloaded file '" + sFileToDownload + "' successfully.\n");


            // Verify that the downloaded object  was downloaded ok using CheckSum
            //sCheckSum = ServiceUtils.readInputStreamToString(objDownloadFile.getDataInputStream(), "UTF-8");
            //blnValidDownload = objDownloadFile.verifyData(sCheckSum.getBytes("UTF-8"));
            //sCheckSum = ServiceUtils.readInputStreamToString(objDownloadFile.getMd5HashAsHex(), "UTF-8");

            System.out.println("\nValidating the download...\n");
            return objDownloadFile.verifyData(downloadPackages[0].getDataFile());
        } catch (NoSuchAlgorithmException eAM) {
            errTime = new Date();
            //Main.currErrorMessage = eAM.toString();
            Main.currErrorMessage = "\n" + DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " NoSuchAlgorithmException: Checksum failed. File will not be processed.\n" + eAM.toString() + "\n";
            System.err.println("\n" + DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " NoSuchAlgorithmException: Checksum failed. File will not be processed.\n" + eAM.toString() + "\n");
            Main.emailError.SendEmail();
            return false;
        } catch (FileNotFoundException eAM) {
            errTime = new Date();
            //Main.currErrorMessage = eAM.toString();
            Main.currErrorMessage = "\n" + DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " FileNotFoundException: File Creation/Retrieval Failed. File will not be processed.\n" + eAM.toString() + "\n";
            System.err.println("\n" + DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " FileNotFoundException: File Creation/Retrieval Failed. File will not be processed.\n" + eAM.toString() + "\n");
            Main.emailError.SendEmail();
            return false;
        } catch (IOException eAM) {
            System.out.println(eAM.getMessage() + "\nDownload: " + sFileToDownload + "\tS3Key: " + downloadBucket);
            eAM.printStackTrace();
            errTime = new Date();
            //Main.currErrorMessage = eAM.toString();
            System.err.println("\n" + DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " IOException: File download failed due to an error.\n" + eAM.toString() + "\n");
            Main.currErrorMessage = "\n" + DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " IOException: File download failed due to an error.\n" + eAM.toString() + "\n";
            Main.emailError.SendEmail();
            return false;
        } catch (S3ServiceException eAM) {
            errTime = new Date();
            eAM.printStackTrace();
            System.out.println(eAM.getMessage() + "\nDownload: " + sFileToDownload + "\tS3Key: " + downloadBucket);
            System.err.println("\n" + DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsAmazon.DownloadFile': \n");
            Main.currErrorMessage = "\n" + DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsAmazon.DownloadFile': \n";
            Main.emailError.SendEmail();//.SendEmail(");
            return false;
        }
    }

    public boolean UploadFile(String sFileToUpload) {
        int processCnt = 0;
        boolean blnValidUpload = false;

        // Get the object
        try {

            // Check if something has been specified
            if (sFileToUpload.length() == 0) {
                return blnValidUpload;
            }

// Check if a bucket has been specified
            if (sActiveBucket.length() == 0) {
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Please specify the bucket to which you want to upload files.");
                Main.currErrorMessage = DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Please specify the bucket to which you want to upload files.";
                Main.emailError.SendEmail();
                return blnValidUpload;
            }

// Open the bucket
            processCnt = 1;
            S3Bucket objActiveBucket = s3Service.getBucket(sActiveBucket);
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Bucket " + sActiveBucket + " has been opened..");

            // Create the S3 Object
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Preparing to upload file: " + sFileToUpload + " to S3.");

            // Get the file that you want to upload
            processCnt =
                    2;
            objUploadFile =
                    new File(sFileToUpload.toLowerCase());
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Upload file has been grabbed.");

            // Generate MD5 Checksum for the file
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Getting ready to compute md5 Checksum for file: " + sFileToUpload);
            processCnt =
                    3;
            byte[] md5Hash = ServiceUtils.computeMD5Hash(new FileInputStream(objUploadFile));
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " MD5 Checksum value has been computed.");

            // Create an S3 object for uploading the file to S3
            processCnt =
                    4;
            S3Object oAMObject = new S3Object(objUploadFile);
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Upload file assigned converted to S3 object.");

            // Attach the Checksum
            processCnt =
                    5;
            oAMObject.setMd5Hash(md5Hash);
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Checksum value has been attached.");

            // Set the file containing the data content to associate with the S3 object.
            processCnt =
                    6;
            oAMObject.setDataInputFile(objUploadFile);
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " S3 object's DataInputFile property has been set.");

            // Define the name used to identify the file in the bucket
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Setting Key for file: " + sFileToUpload);
            processCnt =
                    7;
            oAMObject.setKey(sFileToUpload);
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Bucket Key has been defined.");

            // Upload to S3
            processCnt =
                    8;
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Uploading file to bucket. Please wait...");
            s3Service.putObject(objActiveBucket, oAMObject);

            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " File: " + sFileToUpload.toUpperCase() + " uploaded successfully to S3.");

            // Reset return value
            blnValidUpload =
                    true;

        } catch (Exception eAM) {
            errTime = new Date();
            Main.currErrorMessage = " Error occured in class 'clsAmazon.UploadFile' in process no.: " + Integer.toString(processCnt) + " " + eAM.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsAmazon.UploadFile'' in process no.: " + Integer.toString(processCnt));
            eAM.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            return false;
        }

// Return the status
        return blnValidUpload;
    }

    public boolean UploadFile(String sFileToUpload, String keyToSet, String uploadBucket) {

        boolean blnValidUpload = false;
        File objUploadFile = null;

        if (keyToSet.compareTo("") == 0) {
            keyToSet = sFileToUpload;
        }

        System.out.println("\n\n*********************s3 Key: " + keyToSet + "*******************\n\n");
        try {
            // Check if something has been specified
            if (sFileToUpload.length() == 0) {
                return blnValidUpload;
            }

// Check if a bucket has been specified
            if (uploadBucket.length() == 0) {
                System.out.println("Please specify the bucket to which you want to upload files.");
                return blnValidUpload;
            }

// Open the bucket
            S3Bucket objActiveBucket = s3Service.getBucket(uploadBucket);
            System.out.println("Upload folder " + uploadBucket + " has been opened...\n\t" +
                    "Preparing to upload file: " + sFileToUpload + " to Storage Server");

            // Get the file that you want to upload

            objUploadFile =
                    new File(sFileToUpload.toLowerCase());
            System.out.println("Upload file has been grabbed.");

            // Generate MD5 Checksum for the file
            byte[] md5Hash = ServiceUtils.computeMD5Hash(new FileInputStream(objUploadFile));
            System.out.println("MD5 Checksum value has been computed.");

            // Create an S3 object for uploading the file to S3
            S3Object oAMObject = new S3Object(objUploadFile);
            System.out.println("Upload file assigned converted to Storage Server object.");

            // Attach the Checksum
            oAMObject.setMd5Hash(md5Hash);
            System.out.println("Checksum value has been attached.");

            // Set the file containing the data content to associate with the S3 object.
            oAMObject.setDataInputFile(objUploadFile);
            System.out.println("Storage Server object's DataInputFile property has been set.");

            // Define the name used to identify the file in the bucket
            //Redefine key according to s3 standard
            //oAMObject.setKey(sFileToUpload);
            oAMObject.setKey(keyToSet);
            System.out.println("Bucket Key has been defined.");

            // Upload to S3
            System.out.println("Uploading file to bucket. Please wait...");
            s3Service.putObject(objActiveBucket, oAMObject);
            System.out.println("File: " + sFileToUpload.toUpperCase() + " uploaded successfully to Storage Server.");

            // Reset return value
            blnValidUpload =
                    true;
        } catch (Exception eAM) {
            System.out.println("File upload failed due to an error");
            errTime =
                    new Date();
            //Main.currErrorMessage = eAM.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error occured in class 'clsAmazon.UploadFile': ");
            eAM.printStackTrace();
            Main.emailError.SendEmail();
            blnValidUpload =
                    false;
        }
// Return the status

        return blnValidUpload;
    }

    public int GunZipFile(String sFileToGunZip) {

        // Edwin Nov 2009

        // Initialize return value
        int exitVal = 1;

        // Check that the parameter has been specified
        if (sFileToGunZip.length() == 0) {
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " file name too short quiting ...");
            return exitVal;
        }

        try {
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Attempting to Gunzip file " + sFileToGunZip + "...");

            Process p = Runtime.getRuntime().exec("gunzip " + sFileToGunZip.toLowerCase(), null);

            BufferedReader stdInput = new BufferedReader(new InputStreamReader(p.getInputStream()));
            BufferedReader stdError = new BufferedReader(new InputStreamReader(p.getErrorStream()));

            // read the output from the command
            String s = null;
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Here is the standard output of the command:\n");
            while ((s = stdInput.readLine()) != null) {
                System.out.println(s);
            }

// Wait for process to complete
            exitVal = p.waitFor();

            // Wait for process to complete
            errTime =
                    new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Process Exit Value : " + exitVal);

            // read any errors from the attempted command
            errTime =
                    new Date();
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

    String delTempFile() {
        if (objUploadFile == null) {
            return "\n";
        } else {
            if (objUploadFile.delete()) {
                return objUploadFile.getName() + " Deleted";
            } else {
                return "\n";
            }
        }
    }
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.DateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Iterator;
import org.apache.commons.beanutils.DynaBean;
import org.apache.commons.beanutils.RowSetDynaClass;
//import java.nio.file.StandardOpenOption.*;

/**
 *
 * @author Silas
 */
public class ClusterClass {

    CDHitClusterOutputParser cdhitParser = null;
    Date errTime = null;
    private String projectOwner = "", curDate = "", s3ClusterFileKeyPath = "", s3FastaFileKeyPath = "", mainDatabase = null, clusterReadyDatabases[] = null, projectsForClustering[] = null, clusterParameters[] = null, fastaFilePath = null, clusterFilePath = null;
    boolean parseClusterOutput = true;
    /*
     * clusterParameters[] array holds value settings for clustering and broken down as:
     * index 0: Cluster name, otherwise null
     * index 1: Cluster output file location, otherwise null
     * index 2: Cluster output file name, otherwise null
     * index 3: Simillarity percentage, otherwise null
     * index 4: Mode type, otherwise null
     */

    public ClusterClass(String mainDatabase) {
        this.mainDatabase = mainDatabase;
        if (curDate.length() == 0 || curDate.compareTo("") == 0) {
            Calendar cdr = Calendar.getInstance();
            curDate = cdr.get(Calendar.YEAR) + "-" + (1 + cdr.get(Calendar.MONTH)) + "-" + cdr.get(Calendar.DAY_OF_MONTH) + " " + cdr.get(Calendar.HOUR_OF_DAY) + ":" + cdr.get(Calendar.MINUTE) + ":" + cdr.get(Calendar.SECOND);
        }
    }

    public boolean setClusterReadyDatabases() {
        System.out.println("***\nSetting Cluster Ready Databases\n***");
        try {
            RowSetDynaClass resultSet = Main.objDataAccess.getRecordSet("SELECT * FROM " + mainDatabase + ".tm_process_list WHERE process_name = 'CLUSTER' AND status = 'ACTIVE'");
            if (resultSet.getRows().isEmpty()) {
                clusterReadyDatabases = null;
                logErrorRoutine("There are no databases set for clustering");
                return false;
            } else {
                clusterReadyDatabases = new String[resultSet.getRows().size()];
                Iterator iter = resultSet.getRows().iterator();
                int dbPointer = 0;
                if (iter.hasNext()) {
                    DynaBean dbDataRow = null;
                    do {
                        dbDataRow = (DynaBean) iter.next();
                        clusterReadyDatabases[dbPointer++] = dbDataRow.get("db_target").toString();
                    } while (iter.hasNext());
                    return true;
                } else {
                    logErrorRoutine("Error occured retrieving databases set for clustering");
                    return false;
                }
            }
        } catch (NotAvailableException ex) {
            catchRoutine(ex, "Error: NotAvailableException occured retrieving databases ready for clustering");
            return false;
        }
    }

    public boolean processClusterReadyDatabases() {
        System.out.println("***\nProcessing Cluster Ready Databases\n***");
        if (this.clusterReadyDatabases.length > 0) {
            boolean retVal = true, clusterStatus = true;
            int dbPointer = 0, clusterProjectsArrayCounter = 0;
            String logMsg = "";
            do {
                Main.objDataAccess.SetDatabase(clusterReadyDatabases[dbPointer]);
                try {
                    RowSetDynaClass resultSet = Main.objDataAccess.getRecordSet("select count(to_ms_file_key) filesBeingProcessed from " + Main.objDataAccess.GetDatabase() + ".to_ms_file where (transmission_status!='SEARCHED SUCCESSFULLY' || to_ms_file_key in(select to_ms_file_key from " + Main.objDataAccess.GetDatabase() + ".to_search_details where processing_status!='ETL LOADED SUCCESSFULLY'))");
                    Iterator iter = resultSet.getRows().iterator();
                    //if (iter.hasNext()) {
                    DynaBean dbDataRow = null;
                    dbDataRow = (DynaBean) iter.next();
                    if (resultSet.getRows().isEmpty() || Integer.parseInt(dbDataRow.get("filesBeingProcessed").toString()) == 0) {
                        /*
                         * If query returns no record, it implies all files have transmission_status of SEARCHED
                         * SUCCESSFULLY in to_ms_file table and ETL LOADED SUCCESSFULLY in the to_search_details table
                         * Otherwise, clustering shouldn't proceed
                         */
                        if (setProjectsForClustering(clusterReadyDatabases[dbPointer])) {
                            clusterProjectsArrayCounter = 0;
                            do {
                                System.out.println("- - - - - - Currently Processing Project: " + projectsForClustering[clusterProjectsArrayCounter] + " - - - - - -");
                                /*
                                 * Initialize cluster variables into clusterParameters array and;
                                 * Retrieve/create new faster file for current project being processed
                                 */
                                if (initProjectClusterVariables(clusterReadyDatabases[dbPointer], projectsForClustering[clusterProjectsArrayCounter])) {
                                    //if (fastaFilePath != null) {
                                    if (clusterParameters[4] != null && clusterParameters[4].compareTo("New Mode") == 0) {
                                        //Execute clustering in "New Mode"
                                        clusterStatus = startClustering(fastaFilePath, "New_Mode", projectsForClustering[clusterProjectsArrayCounter]);
                                        if (!clusterStatus) {
                                            sendMailRoutine("New Mode clustering failed for project: " + projectsForClustering[clusterProjectsArrayCounter] + " within database: " + clusterReadyDatabases[dbPointer]);
                                            retVal = false;
                                        }
                                    } else if (clusterParameters[4] != null && clusterParameters[4].compareTo("Incremental Mode") == 0) {
                                        //Execute clustering in "Incremental Mode"
                                        clusterStatus = startClustering(fastaFilePath, "Incremental_Mode", projectsForClustering[clusterProjectsArrayCounter]);
                                        if (!clusterStatus) {
                                            sendMailRoutine("Incremental Mode clustering failed for project: " + projectsForClustering[clusterProjectsArrayCounter] + " within database: " + clusterReadyDatabases[dbPointer]);
                                            retVal = false;
                                        }
                                    } else {
                                        //"Unknown Mode", hence terminate clustering for current project
                                        sendMailRoutine("Clustering failed for project: " + projectsForClustering[clusterProjectsArrayCounter] + " within database: " + clusterReadyDatabases[dbPointer] + " due to an un-identifiable cluster mode");
                                        retVal = false;
                                        break;
                                    }

                                    if (clusterStatus) {
                                        /*
                                         * s3KeyPath for uploading fasta file for storage in s3 shall be defined by:
                                         * ia_fasta_files/Project_Name/Date_Of_Clustering
                                         */
                                        s3FastaFileKeyPath = projectOwner + "/" + projectsForClustering[clusterProjectsArrayCounter] + "/" /*+ curDate.substring(0, curDate.indexOf(" ")).replace("-", "_").trim() + "_" + dateArray[3].replace(":", ":") + "_" + dateArray[4]*/ + "/" + fastaFilePath.substring(1 + fastaFilePath.lastIndexOf("/"));
                                        System.out.println("Fasta File Key: " + s3FastaFileKeyPath + "\t" + errTime.toString());
                                        if (!uploadFiles("fasta")) {
                                            sendMailRoutine("Failed to upload fasta files: " + projectsForClustering[clusterProjectsArrayCounter] + " within database: " + clusterReadyDatabases[dbPointer]);
                                        }

                                        /*
                                         * Upon successful clustering, parse cluster file/output and store values in a database
                                         */
                                        if (parseClusterOutput) {
                                            if (!parseClstrFile()) {
                                                sendMailRoutine("Parsing of cluster file:" + clusterFilePath + " failed for project: " + projectsForClustering[clusterProjectsArrayCounter] + " within database: " + clusterReadyDatabases[dbPointer]);
                                            }
                                        } else {
                                            sendMailRoutine("Could not Parse cluster file: \"" + clusterFilePath + "\" for project: " + projectsForClustering[clusterProjectsArrayCounter] + " within database: " + clusterReadyDatabases[dbPointer] + ": Error occured while running clustering");
                                        }
                                    }

                                    /*
                                     * Update respective databases verifying the final status of the clustering
                                     */
                                    if (!updateRespectiveTables(clusterReadyDatabases[dbPointer], projectsForClustering[clusterProjectsArrayCounter])) {
                                        retVal = false;
                                        sendMailRoutine("Updating of respective tables failed for project: " + projectsForClustering[clusterProjectsArrayCounter] + " within database: " + clusterReadyDatabases[dbPointer]);
                                    }
                                } else {
                                    if (fastaFilePath == null) {
                                        sendMailRoutine("Failed to retrieve/initialize cluster variables for project: " + projectsForClustering[clusterProjectsArrayCounter] + " within database: " + clusterReadyDatabases[dbPointer]);
                                    }
                                    retVal = false;
                                }
                            } while (++clusterProjectsArrayCounter < projectsForClustering.length);
                        } else {
                            logMsg = "Failed to retrieve/set projects ready for clustering for database: " + clusterReadyDatabases[dbPointer];
                            retVal = false;
                        }
                    } else {
                        logMsg = "There are files being processed for database: " + clusterReadyDatabases[dbPointer] + ", clustering can't proceed";
                        retVal = false;
                    }
                } catch (NotAvailableException ex) {
                    catchRoutine(ex, "Error: NotAvailableException occured retrieving databases ready for clustering");
                    retVal = false;
                }
            } while (++dbPointer < clusterReadyDatabases.length);
            if (!(logMsg.trim().compareTo("") == 0 || logMsg.trim().length() == 0)) {
                sendMailRoutine(logMsg);
            }
            return retVal;
        } else {
            sendMailRoutine("Clustering attempted while zero databases are ready for cluster");
            return false;
        }
    }

    private boolean setProjectsForClustering(String database) {
        System.out.println("***\nSetting Projects For Clustering\n***");
        /*
         * Query database and retrieve projects that are to be clustered.
         * Projects to be initialized into projectsForClustering[] variable array
         */
        try {
            RowSetDynaClass resultSet = Main.objDataAccess.getRecordSet("SELECT * FROM " + database + ".to_projects WHERE clustering_status='READY_FOR_CLUSTERING'");
            if (resultSet.getRows().isEmpty()) {
                sendMailRoutine("There are no projects that are ready for clustering");
                return false;
            } else {
                Iterator iter = resultSet.getRows().iterator();
                if (iter.hasNext()) {
                    DynaBean dbDataRow = null;
                    int clusterProjectsArrayCounter = 0;
                    String[] tmpArray = null;
                    projectsForClustering = new String[0];
                    do {
                        tmpArray = projectsForClustering;
                        projectsForClustering = new String[clusterProjectsArrayCounter + 1];
                        dbDataRow = (DynaBean) iter.next();


                        if (Main.objDataAccess.runUpdateStmt("update " + database + ".to_projects set clustering_status='CLUSTERING_IN_PROGRESS' where project_name='" + dbDataRow.get("project_name").toString() + "'") != 1) {
                            sendMailRoutine("Failed to update cluster status to \"CLUSTERING_IN_PROGRESS\" for project: " + projectsForClustering[clusterProjectsArrayCounter] + " within database: " + database);
                            projectsForClustering = null;
                            return false;
                        }

                        projectsForClustering[clusterProjectsArrayCounter++] = dbDataRow.get("project_name").toString();
                        for (int i = 0; i < tmpArray.length; i++) {
                            projectsForClustering[i] = tmpArray[i];
                        }
                    } while (iter.hasNext());
                    return true;
                } else {
                    sendMailRoutine("Error: Failed to retrieve cluster ready projects from database: " + database);
                    return false;
                }
            }
        } catch (NotAvailableException ex) {
            catchRoutine(ex, "Error: NotAvailableException occured retrieving cluster ready projects from database: " + database);
            return false;
        }
    }

    public boolean initProjectClusterVariables(String database, String project) {
        System.out.println("***\nInitializing Cluster Variables\n***");
        Main.objDataAccess.SetDatabase(database);
        /*
         * Retrieve parameters for clustering and store in variable
         * Retrieve/Create fasta file
         */
        try {
            /*
             * Retrieve parameters/variables from database and initialize clusterParameters[] array
             */
            DynaBean dbDataRow = null;
            RowSetDynaClass resultSet = Main.objDataAccess.getRecordSet("SELECT cluster_name,cluster_output_location,cluster_output_filename,similarity_percentage FROM " + database + ".to_cluster WHERE to_cluster_pk=(SELECT to_cluster_pk FROM " + database + ".to_projects WHERE project_name='" + project + "')");
            if (resultSet.getRows().isEmpty()) {
                //sendMailRoutine("Cluster settings missing for project: " + project + " within database: " + database);
            } else {
                Iterator iter = resultSet.getRows().iterator();
                if (iter.hasNext()) {
                    clusterParameters = new String[5];
                    /*
                     * clusterParameters[] array holds value settings for clustering and broken down as:
                     * index 0: Cluster name, otherwise null
                     * index 1: Cluster output file location, otherwise null
                     * index 2: Cluster output file name, otherwise null
                     * index 3: Simillarity percentage, otherwise null
                     * index 4: Mode type, otherwise null
                     */
                    dbDataRow = (DynaBean) iter.next();
                    if (dbDataRow.get("cluster_name").toString().trim().length() == 0 || dbDataRow.get("cluster_name").toString() == null) {
                        clusterParameters[0] = null;
                    } else {
                        clusterParameters[0] = dbDataRow.get("cluster_name").toString();
                    }

                    if (dbDataRow.get("cluster_output_location").toString().trim().length() == 0 || dbDataRow.get("cluster_output_location").toString() == null) {
                        clusterParameters[1] = null;
                    } else {
                        clusterParameters[1] = dbDataRow.get("cluster_output_location").toString();
                    }

                    if (dbDataRow.get("cluster_output_filename").toString().trim().length() == 0 || dbDataRow.get("cluster_output_filename").toString() == null) {
                        clusterParameters[2] = null;
                    } else {
                        clusterParameters[2] = dbDataRow.get("cluster_output_filename").toString();
                    }

                    if (dbDataRow.get("similarity_percentage").toString().trim().length() == 0 || dbDataRow.get("similarity_percentage").toString() == null) {
                        clusterParameters[3] = null;
                    } else {
                        clusterParameters[3] = dbDataRow.get("similarity_percentage").toString();
                    }

                    if ((clusterParameters[2] == null && clusterParameters[1] == null) || (clusterParameters[2].trim().length() == 0 && clusterParameters[1].trim().length() == 0)) {
                        clusterParameters[4] = "New Mode";
                    } else {
                        clusterParameters[4] = "Incremental Mode";
                    }
                } else {
                    sendMailRoutine("Error: Failed to retrieve cluster settings for project: " + project + " from database: " + database);
                }


                resultSet = Main.objDataAccess.getRecordSet("SELECT String_Val FROM " + database + ".tm_variables WHERE Var_Name='ftp_dir_search_results'");
                if (resultSet.getRows().isEmpty()) {
                    //sendMailRoutine("Cluster settings missing for project: " + project + " within database: " + database);
                } else {
                    iter = resultSet.getRows().iterator();
                    if (iter.hasNext()) {
                        dbDataRow = (DynaBean) iter.next();
                        projectOwner = dbDataRow.get("String_Val").toString().substring(1+dbDataRow.get("String_Val").toString().indexOf("/"), dbDataRow.get("String_Val").toString().indexOf("/",1+dbDataRow.get("String_Val").toString().indexOf("/")));
                    } else {
                        sendMailRoutine("Error: Failed to retrieve name of the project owner");
                    }
                }
            }

            /*
             * Create/Initialize fasta file
             */
            System.out.println("***\nRetrieving Fasta File Data\n***");
            resultSet = Main.objDataAccess.getRecordSet("select fasta_string from vo_fasta_for_clustering");
            if (resultSet.getRows().isEmpty()) {
                sendMailRoutine("Fasta file data un-available for project: " + project + " within database: " + database);
            } else {
                Iterator iter = resultSet.getRows().iterator();
                if (iter.hasNext()) {
                    //fastaFilePath = "c:/cluster files/file.fasta";
                    fastaFilePath = "c:/cluster files/" + projectOwner + "/" + project.toLowerCase().trim() + "/" + curDate.substring(0, curDate.lastIndexOf(" ")) + ".fasta";

                    System.out.println("\n**********fastaFilePath.getAbsolutePath()\t" + fastaFilePath + "***********\n");

                    File fastaFileObject = new File(fastaFilePath), fastaFileDirectoryPath=new File(fastaFilePath.substring(0,fastaFilePath.lastIndexOf("/")));
                    if(!fastaFileDirectoryPath.isDirectory()){
                        if (fastaFileDirectoryPath.mkdirs());
                    }
                    fastaFileObject.createNewFile();
                    fastaFileObject.setWritable(true);
                    String fastaFileData = "";
                    byte[] fastaFileDataInBytes = null;
                    //OutputStream out=new FileOutputStream("");
                    BufferedOutputStream fastaFileOutputStream = new BufferedOutputStream(new FileOutputStream(fastaFilePath));
                    do {
                        dbDataRow = (DynaBean) iter.next();
                        fastaFileData = dbDataRow.get("fasta_string").toString();
                        fastaFileDataInBytes = fastaFileData.getBytes();
                        fastaFileOutputStream.write(fastaFileDataInBytes, 0, fastaFileDataInBytes.length);
                    } while (iter.hasNext());
                } else {
                    sendMailRoutine("Error: Failed to retrieve fasta file data for project: " + project + " from database: " + database);
                }
            }
            return true;
        } catch (NotAvailableException ex) {
            catchRoutine(ex, "Error: NotAvailableException occured retrieving cluster settings for project: " + project + " from database: " + database);
            return false;
        } catch (IOException ex) {
            catchRoutine(ex, "Error: IOException occured creating fasta file for project: " + project + " from database: " + database);
            return false;
        }
    }

    private boolean startClustering(String inputFastaFile, String clstrMode, String project) {
        System.out.println("***\nStarting Clustering\n***");
        boolean retVal = true;
        clusterFilePath = "C:/cluster files/" + projectOwner + "/" + project.toLowerCase().trim() + "/" + clusterParameters[0] + ".clstr";// curDate.substring(0, curDate.lastIndexOf(" ")) + ".clstr";//clusterParameters[1] + "" + clusterParameters[2];
        // Initialize return value
        int exitVal = 1;

        // Check that the parameter has been specified
        if (this.clusterParameters == null) {
            return false;
        }

        try {
            BufferedReader stdInput = null;
            BufferedReader stdError = null;
            Process p = null;
            //C:\cd-hit-windows>cd_hit.exe -i "C:\cluster files\test_file.fasta" -o "C:\cluster files\output.clstr" -c 0.9 -n 5 -d 50
            if (clstrMode.compareTo("New_Mode") == 0) {
                System.out.println("***\n\"New Mode\" Clustering\n***");
                System.out.println("Cluster File Path: " + clusterFilePath + "\n\n\nCluster CMD: \tC:\\cd-hit-windows\\cd_hit.exe -i \"" + inputFastaFile + "\" -o \"" + clusterFilePath.substring(0, clusterFilePath.lastIndexOf(".clstr")) + "\" -c " + (Float.valueOf(clusterParameters[3]) / 100) + " -n 5 -d 50\n\n");
                p = Runtime.getRuntime().exec("C:\\cd-hit-windows\\cd_hit.exe -i \"" + inputFastaFile + "\" -o \"" + clusterFilePath.substring(0, clusterFilePath.lastIndexOf(".clstr")) + "\" -c " + (Float.valueOf(clusterParameters[3]) / 100) + " -n 5 -d 50", null);

                stdInput = new BufferedReader(new InputStreamReader(p.getInputStream()));
                stdError = new BufferedReader(new InputStreamReader(p.getErrorStream()));

                /*
                 * s3KeyPath for uploading generated cluster file for storage in s3 shall be defined by:
                 * ia_fasta_files/Project_Name/Date_Of_Clustering
                 */
                s3ClusterFileKeyPath = projectOwner + "/" + project + "/" + curDate.substring(0, curDate.indexOf(" ")).replace("-", "_").trim() + "/" + clusterFilePath.substring(1 + clusterFilePath.lastIndexOf("/"));
            } else if (clstrMode.compareTo("Incremental_Mode") == 0) {
                System.out.println("***\n\"Incremental Mode\" Clustering\n***");
                /*
                 * Download existing protein sequence for incremental clustering
                 */
                String downloadedFile = "";
                if (Main.amazonObject == null) {
                    Main.amazonObject = new clsAmazon();
                }
                Main.amazonObject.ConnectToAmazon();
                if (Main.amazonObject.DownloadFile("C:/cluster files/", clusterParameters[1].substring(1 + clusterParameters[1].indexOf("/")) + clusterParameters[2], "ia_cluster_files")) {
                    downloadedFile = "C:/cluster files/" + clusterParameters[2];
                } else {
                    //sendMailRoutine("");
                    return false;
                }
                System.out.println("\n\nCluster CMD: \tC:\\cd-hit-windows\\cd_hit_2d.exe -i \"" + downloadedFile + "\" -i2 \"" + inputFastaFile + "\" -o \"" + clusterFilePath.substring(0, clusterFilePath.lastIndexOf(".")) + "\" -c " + (Float.valueOf(clusterParameters[3]) / 100) + " -n 5 -d 50\n\n");
                p = Runtime.getRuntime().exec("C:\\cd-hit-windows\\cd_hit_2d.exe -i \"" + downloadedFile + "\" -i2 \"" + inputFastaFile + "\" -o \"" + clusterFilePath.substring(0, clusterFilePath.lastIndexOf(".")) + "\" -c " + (Float.valueOf(clusterParameters[3]) / 100) + " -n 5 -d 50", null);

                stdInput = new BufferedReader(new InputStreamReader(p.getInputStream()));
                stdError = new BufferedReader(new InputStreamReader(p.getErrorStream()));

                /*
                 * s3KeyPath for uploading generated cluster file for storage in s3 shall be defined by:
                 * ia_fasta_files/Project_Name/Date_Of_Clustering
                 */
                s3ClusterFileKeyPath = clusterParameters[1].substring(1 + clusterParameters[1].indexOf("/")) + clusterParameters[2];
            }
            // read the output from the command
            String s = null;
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Here is the standard output of the command:\n");
            while ((s = stdInput.readLine()) != null) {
                System.out.println(s);
                //parseClusterOutput = true;
                if (s.trim().contains("0 clustered") && !s.trim().startsWith("....") && s.trim().endsWith("0 clustered")) {
                    parseClusterOutput = false;
                    System.out.println("*** *** ***\n*** ***\t" + s + "\t*** ***\n*** *** ***\n");
                } else {
                    System.out.println("*** *** ***\n*** ***\tNot Matched 1\t*** ***\n*** *** ***\n");
                }
            }

            // Wait for process to complete
            exitVal = p.waitFor();
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Process Exit Value : " + exitVal);

            // read any errors from the attempted command
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Here is the standard error of the command (if any):\n");
            while ((s = stdError.readLine()) != null) {
                System.out.println(s);
                if (s.trim().contains("Error") || s.trim().contains("Program halted")) {
                    parseClusterOutput = false;
                    System.out.println("*** *** ***\n*** ***\t" + s + "\t*** ***\n*** *** ***\n");
                } else {
                    System.out.println("*** *** ***\n*** ***\tNot Matched 2\t*** ***\n*** *** ***\n");
                }
            }

            /*
             * Upon successful clustering, upload generated cluster file to s3
             *
             * s3KeyPath for uploading generated cluster file generated within respective blocks i.e. "New Mode" or "incremental Mode"
             */
            //Calendar cdr = Calendar.getInstance();
            //curDate = cdr.get(Calendar.YEAR) + "-" + cdr.get(Calendar.MONTH) + "-" + cdr.get(Calendar.DAY_OF_MONTH) + " " + cdr.get(Calendar.HOUR_OF_DAY) + ":" + cdr.get(Calendar.MINUTE) + ":" + cdr.get(Calendar.SECOND);
            //s3ClusterFileKeyPath = project + "/" + curDate.substring(0, curDate.indexOf(" ")).replace("-", "_").trim() + "/" + clusterFilePath.substring(1 + clusterFilePath.lastIndexOf("/"));
            System.out.println(s3ClusterFileKeyPath + "\t" + errTime.toString());
            if (parseClusterOutput) {
                if (!uploadFiles("cluster")) {
                    sendMailRoutine("Failed to upload cluster file: " + clusterFilePath + " to s3 key: " + s3ClusterFileKeyPath);
                    retVal = false;
                }
            } else {
                retVal = false;
            }
        } catch (IOException ioe) {
            catchRoutine(ioe, "IOException occured reading standard input and or output");
            retVal = false;
        } catch (InterruptedException ioe) {
            catchRoutine(ioe, "Interrupted exception occured on process P");
            retVal = false;
        }
        return retVal;
    }

    private boolean parseClstrFile() {
        System.out.println("***\nParsing Cluster Output\n***");
        boolean retVal = true;
        cdhitParser = new CDHitClusterOutputParser(clusterFilePath);
        if (!cdhitParser.parseClusterFile(clusterParameters[0])) {
            sendMailRoutine("Parsing of cluster output failed for file: " + clusterFilePath);
            retVal = false;
        }
        return retVal;
    }

    boolean uploadFiles(String fileType) {
        //Local fasta file path shall be retrieved from File object fastaFilePath
        boolean retVal = true;
        if (Main.amazonObject == null) {
            Main.amazonObject = new clsAmazon();
        }
        Main.amazonObject.ConnectToAmazon();
        if (fileType.compareTo("fasta") == 0) {
            System.out.println("***\nUploading File:\t" + s3FastaFileKeyPath + "\n***");
            if (!Main.amazonObject.UploadFile(fastaFilePath, s3FastaFileKeyPath.toLowerCase(), "ia_fasta_files")) {
                System.out.println("***\nLocal File:\t" + fastaFilePath + "\n***");
                retVal = false;
            }
        } else if (fileType.compareTo("cluster") == 0) {
            System.out.println("***\nUploading File:\t" + s3ClusterFileKeyPath + "\n***");
            if (!Main.amazonObject.UploadFile(clusterFilePath, s3ClusterFileKeyPath.toLowerCase(), "ia_cluster_files")) {
                System.out.println("***\nLocal File:\t" + clusterFilePath + "\n***");
                retVal = false;
            }
        }
        return retVal;
    }

    boolean updateRespectiveTables(String database, String project) {
        System.out.println("***\nUpdating Respective Cluster Tables\n***");
        Main.objDataAccess.SetDatabase(database);
        boolean retVal = true;
        /*
         * Update to_cluster_table with new output_filename and location
         *
         * clusterParameters[] array holds value settings for clustering and broken down as:
         * index 0: Cluster name, otherwise null
         * index 1: Cluster output file location, otherwise null
         * index 2: Cluster output file name, otherwise null
         * index 3: Simillarity percentage, otherwise null
         * index 4: Mode type, otherwise null
         */
        if (clusterParameters[4].compareTo("New Mode") == 0) {
            if (Main.objDataAccess.runUpdateStmt("update " + database + ".to_cluster set cluster_output_filename='" + s3ClusterFileKeyPath.substring(1 + s3ClusterFileKeyPath.lastIndexOf("/")).toLowerCase() + "', cluster_output_location='ia_cluster_files/" + s3ClusterFileKeyPath.substring(0, s3ClusterFileKeyPath.lastIndexOf("/")).toLowerCase() + "/', ia_last_update_timestamp='" + curDate + "', ia_last_updated_user ='" + Main.objDataAccess.GetUserName() + "' where to_cluster_pk=(SELECT to_cluster_pk FROM " + database + ".to_projects WHERE project_name='" + project + "')") != 1) {
                retVal = false;
            }
        } else if (clusterParameters[4].compareTo("Incremental Mode") == 0) {
            // Main.objDataAccess.runUpdateStmt("");
        } else {
            retVal = false;
        }

        /*
         * Call stored procedure in MySQL for ETL load
         */

        /*
         * Update the to_project.cluster_status field with 'CLUSTERED' status
         */
        if (Main.objDataAccess.runUpdateStmt("update " + database + ".to_projects set clustering_status='CLUSTERED' where project_name='" + project + "'") != 1) {
            retVal = false;
        }
        return retVal;
    }

    private void logErrorRoutine(String errMessage) {
        errTime = new Date();
        Main.currErrorMessage = DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + ": " + errMessage;
        System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + ": " + errMessage);
    }

    private void sendMailRoutine(String mailMessage) {
        errTime = new Date();
        Main.currErrorMessage = DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + ": " + mailMessage;
        System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + ": " + mailMessage);
        Main.emailError.SendEmail();
    }

    private void catchRoutine(Exception ex, String accompanyingMessage) {
        errTime = new Date();
        Main.currErrorMessage = DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + ": " + accompanyingMessage + "; " + ex.getMessage();
        System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + ": " + accompanyingMessage + "; " + ex.getMessage());
        ex.printStackTrace();
        Main.emailError.SendEmail();
        System.exit(1);
    }
}

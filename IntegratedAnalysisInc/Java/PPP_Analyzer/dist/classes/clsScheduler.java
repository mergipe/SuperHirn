/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.text.*;
import java.util.Calendar;
import java.util.Date;
import java.util.concurrent.TimeUnit;
import java.util.Iterator;
import org.apache.commons.beanutils.DynaBean;
import org.apache.commons.beanutils.RowSetDynaClass;

/**
 *
 * @author Irungu.Kamau
 */
public class clsScheduler {

    //static clsDataAccess objDataAccess;
    EC2 objec2;
    String availabilityZone, sHostName, sTargetDatabase = "", mainDatabase = null, dbUserName = null, dbPassWord = null, selectedAmi = null;
    int dbPort = 0, maxNoOfFiles = 15, maxMySQLConSleepTime = 15, schedulerSleepTime = 120000;
    private Date errTime;
    //private EmailDevTeam emailError = new EmailDevTeam();
    String[] targetDatabases = null;

    public clsScheduler(String sHostName, String mainDatabase, String dbUserName, String dbPassWord, int dbPort) {
        this.mainDatabase = mainDatabase;
        this.dbPassWord = dbPassWord;
        this.dbUserName = dbUserName;
        this.dbPort = dbPort;
        this.sHostName = sHostName;
        Main.objDataAccess = new clsDataAccess(sHostName, dbUserName, dbPassWord, dbPort);
        setTargetDatabases();
    }

    //public clsScheduler(String mainDatabase, String dbUserName, String dbPassWord) {
    //   this.mainDatabase = mainDatabase;
    ///    this.dbPassWord = dbPassWord;
    //    this.dbUserName = dbUserName;
    //    setTargetDatabases();
    //}
    //public clsScheduler() {
    //   setTargetDatabases();
    // }
    // public clsDataAccess InstantiateDataAccess() {
    //objDataAccess = new clsDataAccess("root", "msijo1970");
    //objDataAccess = new clsDataAccess("edwin_pass_tgt", "polk9876");
    //objDataAccess.SetDatabase("db_dn_pass_dev");
    //if (dbUserName == null || dbPassWord == null || dbPort == 0) {
    //objDataAccess = new clsDataAccess("edwin_pass_tgt", "polk9876");
    //} else {
    ///Main.objDataAccess = new clsDataAccess(sHostName, dbUserName, dbPassWord, dbPort);
    //}
    //objDataAccess.connectToMySQL();
    //return objDataAccess;
    //}
    public void setTargetDatabases() {
        /*
         * initializes the targetDatabases variable with databases that have been set as active
         */
        RowSetDynaClass rsRecordset = null;

        try {
            //Main.objDataAccess = InstantiateDataAccess();
            //objDataAccess.SetDatabase("db_dn_pass_dev");
            if (mainDatabase == null) {
                System.out.println("mainDatabase is Not Set");
                errTime = new Date();
                Main.currErrorMessage = "mainDatabase is Not Set";
                System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : Main database is not set in clsScheduler class ");
                Main.emailError.SendEmail();
                System.exit(1);
                //Main.objDataAccess.SetDatabase("db_dn_pass_dev");
            } else {
                System.out.println("mainDatabase is Set");
                Main.objDataAccess.SetDatabase(mainDatabase);
            }
            //conMySQL = objDataAccess.connectToMySQL();
            //sSQLSelect = conMySQL.createStatement();
            rsRecordset = Main.objDataAccess.getRecordSet("SELECT * FROM tm_process_list WHERE process_name = 'XTANDEM' AND status = 'ACTIVE'");

            if (rsRecordset.getRows().isEmpty()) {
                targetDatabases = null;
                return;
            }
            //if (rsRecordset.getRows().) {
            //moves pointer to the last row of the Recordset so as to help determine the number of rows available
            targetDatabases = new String[rsRecordset.getRows().size()];
            //}
            Iterator iter = rsRecordset.getRows().iterator();

            int currentDB = 0;
            while (iter.hasNext()) {

                DynaBean dbDataRow = (DynaBean) iter.next();

                targetDatabases[currentDB++] = dbDataRow.get("db_target").toString();

            }

            /*
             * Query tm_variables tables and retrieve values for max_no_of_files an instance can process,
             * max_mysql_con_sleep_time, scheduler_sleep_time, selected_ami
             */
            rsRecordset = Main.objDataAccess.getRecordSet("SELECT * FROM tm_variables");

            if (!rsRecordset.getRows().isEmpty()) {

                iter = rsRecordset.getRows().iterator();
                while (iter.hasNext()) {

                    DynaBean dbDataRow = (DynaBean) iter.next();

                    /*
                    if ((String) dbDataRow.get("Var_Name") == "max_no_of_files") {
                    maxNoOfFiles = (Integer) dbDataRow.get("String_Val");
                    } else if ((String) dbDataRow.get("Var_Name") == "max_mysql_con_sleep_time") {
                    maxMySQLConSleepTime = (Integer) dbDataRow.get("String_Val");
                    } else if ((String) dbDataRow.get("Var_Name") == "scheduler_sleep_time") {
                    schedulerSleepTime = (Integer) dbDataRow.get("String_Val");
                    } else if ((String) dbDataRow.get("Var_Name") == "selected_ami") {
                    selectedAmi = (String) dbDataRow.get("String_Val");
                    }
                     */

                    if (dbDataRow.get("Var_Name").toString().compareTo("max_no_of_files") == 0) {
                        maxNoOfFiles = Integer.parseInt(dbDataRow.get("String_Val").toString());
                    } else if (dbDataRow.get("Var_Name").toString().compareTo("max_mysql_con_sleep_time") == 0) {
                        maxMySQLConSleepTime = Integer.parseInt(dbDataRow.get("String_Val").toString());
                    } else if (dbDataRow.get("Var_Name").toString().compareTo("scheduler_sleep_time") == 0) {
                        schedulerSleepTime = Integer.parseInt(dbDataRow.get("String_Val").toString());
                    } else if (dbDataRow.get("Var_Name").toString().compareTo("selected_ami") == 0) {
                        selectedAmi = dbDataRow.get("String_Val").toString();
                    } else if (dbDataRow.get("Var_Name").toString().compareTo("availability_zone") == 0) {
                        availabilityZone = dbDataRow.get("String_Val").toString();
                    }
                }
            }
            System.out.println("Tm_Variables Set Are: \n\tmax_no_of_files = " + maxNoOfFiles + " Files\n\tmax_mysql_con_sleep_time = " + maxMySQLConSleepTime + " Seconds\n\tscheduler_sleep_time = " + schedulerSleepTime + " Milli Seconds\n\tselected_ami = " + selectedAmi+ " \n\tAvailability Zone = " + availabilityZone);

            /*            if (Recordset != null && Recordset.next()) {
            do {
            if (Recordset.getString("Var_Name").compareTo("max_no_of_files") == 0) {
            maxNoOfFiles = Integer.parseInt(Recordset.getString("String_Val"));
            } else if (Recordset.getString("Var_Name").compareTo("max_mysql_con_sleep_time") == 0) {
            maxMySQLConSleepTime = Integer.parseInt(Recordset.getString("String_Val"));
            } else if (Recordset.getString("Var_Name").compareTo("scheduler_sleep_time") == 0) {
            schedulerSleepTime = Integer.parseInt(Recordset.getString("String_Val"));
            } else if (Recordset.getString("Var_Name").compareTo("selected_ami") == 0) {
            selectedAmi = Recordset.getString("String_Val");
            }
            } while (Recordset.next());
            System.out.println("Tm_Variables Set Are: \n\tmax_no_of_files = " + maxNoOfFiles + " Files\n\tmax_mysql_con_sleep_time = " + maxMySQLConSleepTime + " Seconds\n\tscheduler_sleep_time = " + schedulerSleepTime + " Milli Seconds\n\tselected_ami = " + selectedAmi);
            } else {
            ;
            }
             */
            rsRecordset = null;

            //if (conMySQL != null) {
            // objDataAccess.closeCon();
            //  conMySQL = null;
            // }
        } catch (Exception e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : clsScheduler.setTargetDatabases: ");
            e.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
        }
    }

    public void StartSearchProcess() {
        int iSearchRecords;  //Records ready for processing
//    boolean bSearchMore;  // This boolean was meant to make teh looping more effective - it is currently not being used
        // but has been left instact as it may be required later
        String sActiveInstanceDNS; //This string will hold an instance ID of an active instance which has not been fully loaded
        try {
            do {
                if (targetDatabases == null) {
                    continue;
                }
                int activeDbIndex = 0;
                /*
                 * do{}while() loop allows scheduler to iterate through the various databases
                 * set as active
                 */
                do {
                    System.out.println("Target DB Seting to: " + targetDatabases[activeDbIndex]);
                    /*
                     * sets the sTargetDatabase variable so that every time a connection is
                     * made to the database, the variable would be holding the expected database
                     */
                    sTargetDatabase = targetDatabases[activeDbIndex];
                    do {
                        System.out.println("Checking records for search in database " + targetDatabases[activeDbIndex]);
                        //Determine number of records ready for search - transmission_status ='READY FOR SEARCH' and no instance assigned
                        iSearchRecords = CheckRecordsToSearch();
                        if (iSearchRecords > 0) {
                            //Determine if there is ann available instance already running and with less than 15 files assigned
                            sActiveInstanceDNS = ReturnAvailableEC2Instance();

                            if (sActiveInstanceDNS.equals("")) {
                                //No active instance, launch a new one
                                objec2 = new EC2(selectedAmi, availabilityZone);
                                if (objec2.LaunchEC2Instance()) {
                                    RecordThisInstance(objec2.getInstanceID(), objec2.getPrivateDNS(), objec2.getPublicDNS());
                                    sActiveInstanceDNS = objec2.getPrivateDNS();
                                }
                            }
                            //Assign files to the instance - maximum 15
                            if (MarkSearchItemsForThisInstance(sActiveInstanceDNS)) {
                                System.out.println("");
                            }
                        }
                    } while (iSearchRecords > 0); //while(true);
                } while (++activeDbIndex < targetDatabases.length);
            } while (LoopCounter());
        } catch (Exception e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : clsScheduler.StartSearchProcess: ");
            e.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
        }
    }

    public boolean LoopCounter() {

        // Delay for two minutes then return
        try {
            Thread.currentThread().sleep(schedulerSleepTime);
            if (targetDatabases != null && CheckInstancesToTerminate());
            setTargetDatabases();
        } catch (InterruptedException ie) {
            return false;
        } catch (Exception e) {
            return false;
        }

        return true;

    }

    private boolean MarkSearchItemsForThisInstance(String sInstanceDNS) {
        /*
         * uses the sInstanceDNS variable to determine number of files currently being processed
         * by instance before allocating more file upto a specified maximum e.g. 15 as determined
         * by the iCommittedRecords variable
         */
        boolean retValue = true;
        Main.objDataAccess.SetDatabase(sTargetDatabase);
        RowSetDynaClass rsRecordset = null;
        long iCommittedRecords = 0; //Record already being processed by active Instance
        try {
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "SELECT count(*) from to_ms_file WHERE transmission_status='READY FOR SEARCH' AND ec2_instance_id= '" + sInstanceDNS + "'");
            //Retrieve count of records already being processed by this specific instance

            int i = 0;
            do {
                rsRecordset = Main.objDataAccess.getRecordSet("SELECT count(*) RecordCount from " + targetDatabases[i] + ".to_ms_file WHERE (transmission_status='READY FOR SEARCH' || transmission_status='SEARCHING') AND ec2_instance_id= '" + sInstanceDNS + "'");

                Iterator iter = rsRecordset.getRows().iterator();

                while (iter.hasNext()) {

                    DynaBean dbDataRow = (DynaBean) iter.next();
                    iCommittedRecords += (Long) dbDataRow.get("RecordCount");
                }

            } while (++i < targetDatabases.length);

            //Recordset = objDataAccess.getRecordSet("SELECT count(*) from to_ms_file WHERE transmission_status='READY FOR SEARCH' AND ec2_instance_id= '" + sInstanceDNS + "'");
            //iCommittedRecords = Recordset.getInt(1);
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "Records already being processed by " + sInstanceDNS + " - " + iCommittedRecords);
            //Ideally execution will only reach here if the instance is only serving less that 15 files
            //However just do a check for it and only add files if instance has less
            if (iCommittedRecords < maxNoOfFiles) {
                //Top up instance's files to 15
                Main.objDataAccess.WriteRecord("UPDATE to_ms_file SET ec2_instance_id='" + sInstanceDNS + "' WHERE transmission_status='READY FOR SEARCH' and ec2_instance_id='' LIMIT " + (maxNoOfFiles - iCommittedRecords));
                System.out.println("Maximun Number Of Files Set: " + maxNoOfFiles + "\tCommitted: " + (maxNoOfFiles - iCommittedRecords) + " FIles\n\tCommited: " + iCommittedRecords + "\nUPDATE to_ms_file SET ec2_instance_id='" + sInstanceDNS + "' WHERE transmission_status='READY FOR SEARCH' and ec2_instance_id='' LIMIT " + (maxNoOfFiles - iCommittedRecords));
            }
            retValue = true;

            rsRecordset = null;

        } catch (Exception e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : clsScheduler.MarkSearchItemsForThisInstance: ");
            e.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            retValue = false;
        }

        return retValue;
    }

    private int CheckRecordsToSearch() {
        /*
         * queries current database for any files that are ready for searching
         */
        Main.objDataAccess.SetDatabase(sTargetDatabase);
        RowSetDynaClass rsRecordset = null;
        int iResults = 0;
        try {
            rsRecordset = Main.objDataAccess.getRecordSet("SELECT count(*) RecordCount from to_ms_file WHERE transmission_status='READY FOR SEARCH' AND ec2_instance_id = ''");
            if (rsRecordset.getRows().isEmpty()) {
                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "CheckRecordsToSearch: No records ready for Search");
                iResults = 0;
            } else {
                Iterator iter = rsRecordset.getRows().iterator();

                while (iter.hasNext()) {

                    DynaBean dbDataRow = (DynaBean) iter.next();
                    //iResults = (Integer) dbDataRow.get("RecordCount");
                    iResults = Integer.parseInt(String.valueOf(dbDataRow.get("RecordCount")));
                }

                errTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "CheckRecordsToSearch: Records ready for Search : " + iResults);
            }
        } catch (Exception e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : clsScheduler.CheckRecordsToSearch: ");
            e.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            iResults = 0;
        }

        return iResults;
    }

    private String ReturnAvailableEC2Instance() {
        /*
         * queries the database for any EC2 instances that are considered available by determining
         * wether any instances are running and having less than the recommended maximum no. of files
         */
        Main.objDataAccess.SetDatabase(sTargetDatabase);
        RowSetDynaClass rsRecordset = null;
        long noOfRunningFiles = 0;
        String instanceId = "", retVal = "";
        try {
            System.out.println("select * from db_dn_pass_dev.to_ami_instances where to_instance_active =1 order by to_instid desc");
            rsRecordset = Main.objDataAccess.getRecordSet("select * from db_dn_pass_dev.to_ami_instances where to_instance_active =1 order by to_instid desc");

            if (!rsRecordset.getRows().isEmpty()) {
                /*
                 * instanceIdArray holds all the running instances. the do{}while() loop then
                 * iterated through the Recordset, populating the instanceIdArray variable
                 */
                String[] instanceIdArray = null;
                //if (Recordset.last()) {
                instanceIdArray = new String[rsRecordset.getRows().size()];
                //}
                int instanceIdArrayCounter = 0, selectedInstanceId = 0;

                Iterator iter = rsRecordset.getRows().iterator();

                while (iter.hasNext()) {

                    DynaBean dbDataRow = (DynaBean) iter.next();

                    instanceIdArray[instanceIdArrayCounter++] = (String) dbDataRow.get("to_instance_privatedns");
                }

                /*                if (Recordset.first()) {
                do {
                instanceIdArray[instanceIdArrayCounter++] = Recordset.getString("to_instance_privatedns");
                } while (Recordset.next());
                } else {
                retVal = "";
                }
                 */
                instanceIdArrayCounter = 0;
                /*
                 * this do{}while() loop retrieves an instance one at a time, determing if the
                 * number of running files is less than 15. if true, the method returns that
                 * instance as available
                 */
                do {
                    selectedInstanceId = instanceIdArrayCounter;
                    System.out.println("Checking no. of files on " + instanceIdArray[instanceIdArrayCounter]);
                    noOfRunningFiles = returnNoOfRunningFiles(instanceIdArray[instanceIdArrayCounter]);
                    if (noOfRunningFiles == -1 || noOfRunningFiles == -2 || noOfRunningFiles > maxNoOfFiles) {
                        instanceId = "";
                    } else if (noOfRunningFiles < maxNoOfFiles) {
                        instanceId = instanceIdArray[instanceIdArrayCounter];
                        break;
                    } else {
                        instanceId = "";
                        System.out.println("Couldn't retrieve any instances due to possibly an error");
                    }
                } while (++instanceIdArrayCounter < instanceIdArray.length);
                System.out.println("No. of files on " + instanceIdArray[selectedInstanceId] + " instance = " + noOfRunningFiles);
                retVal = instanceId;
            } else {
                System.out.println("Failed to retrieve instances");
                return "";
            }
        } catch (Exception e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : clsScheduler.ReturnAvailableEC2Instance: ");
            e.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            retVal = "";
        }

        return retVal;
    }

    long returnNoOfRunningFiles(String instanceId) {
        /*
         * given the instanceId, this method returns the number of files currently being processed
         * by the instance denoted by instanceId
         */
        Main.objDataAccess.SetDatabase(sTargetDatabase);
        RowSetDynaClass rsRecordset = null;
        long retVal = 0;
        long iNumRunningFiles = 0;

        try {
            /*
             * The for(){} loop iterates through all databases retrieving and summing up the total
             * number of files being processed or will be processed by the given instance
             */
            for (int targetDatabasesIterator = 0; targetDatabasesIterator < targetDatabases.length; targetDatabasesIterator++) {

                System.out.println("select count(ec2_instance_id) noOfRunningFiles from " + targetDatabases[targetDatabasesIterator] + ".to_ms_file where (transmission_status='READY FOR SEARCH' || transmission_status='SEARCHING') and ec2_instance_id='" + instanceId + "'");
                rsRecordset = Main.objDataAccess.getRecordSet("select count(ec2_instance_id) noOfRunningFiles from " + targetDatabases[targetDatabasesIterator] + ".to_ms_file where (transmission_status='READY FOR SEARCH' || transmission_status='SEARCHING') and ec2_instance_id='" + instanceId + "'");

                if (!rsRecordset.getRows().isEmpty()) {

                    Iterator iter = rsRecordset.getRows().iterator();

                    while (iter.hasNext()) {

                        DynaBean dbDataRow = (DynaBean) iter.next();

                        iNumRunningFiles = (Long) dbDataRow.get("noOfRunningFiles");
                        retVal = (retVal + iNumRunningFiles);
                    }
                } else {
                    retVal = -1;
                    System.out.println("Exited with " + iNumRunningFiles + " files in DB " + targetDatabases[targetDatabasesIterator]);
                    break;
                }
                System.out.println("Found " + iNumRunningFiles + " files in DB " + targetDatabases[targetDatabasesIterator]);
            }

            errTime = new Date();
            if (retVal > 0) {
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "ReturnAvailableEC2Instance: Instance ID : " + instanceId + " found " + retVal + " files");
            } else if (retVal == 0) {
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "ReturnAvailableEC2Instance: No File(s) Found");
            } else {
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "ReturnAvailableEC2Instance: An error ocured retrieving number of files");
            }
        } catch (Exception e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : clsScheduler.ReturnAvailableEC2Instance: " + e.getMessage());
            e.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            retVal = -2;
        }

        return retVal;
    }

    private boolean CheckInstancesToTerminate() {
        /*
         * queries database for instances that are deemed fit for termination. in essence, the
         * instance must not be processing any file or have files waiting in its queue and
         * should have been running for a specified amount of time e.g. 50 minutes
         */
        Main.objDataAccess.SetDatabase(sTargetDatabase);
        System.out.println("Checking instances to terminate in DB");
        Calendar instanceStartTime = null, curTime = null;
        RowSetDynaClass rsRecordset = null;
        DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        java.util.Date date = new java.util.Date();
        try {
            /*
             * dbScript is used to generate a query that includes all active databases so that
             * each database can be checked at once for files waiting in any instance's queue
             * or already being processed
             */
            String dbScript = "";
            for (int targetDatabasesIterator = 0; targetDatabasesIterator < targetDatabases.length; targetDatabasesIterator++) {
                if (targetDatabasesIterator == (targetDatabases.length - 1)) {
                    dbScript += "to_instance_privatedns not in(select distinct ec2_instance_id from " + targetDatabases[targetDatabasesIterator] + ".to_ms_file where (transmission_status='READY FOR SEARCH' || transmission_status='SEARCHING'))";
                } else {
                    dbScript += "to_instance_privatedns not in(select distinct ec2_instance_id from " + targetDatabases[targetDatabasesIterator] + ".to_ms_file where (transmission_status='READY FOR SEARCH' || transmission_status='SEARCHING')) and ";
                }
            }
            //querry returns only instances that do not have files being processed or in queue
            System.out.println("select to_started,to_instanceId from db_dn_pass_dev.to_ami_instances where to_instance_active=1 and " + dbScript + "");
            rsRecordset = Main.objDataAccess.getRecordSet("select to_started,to_instanceId from db_dn_pass_dev.to_ami_instances where to_instance_active=1 and " + dbScript + "");

            if (!rsRecordset.getRows().isEmpty()) {
                int instanceCounter = 0;
                /*
                 * instanceArray is a two dimension array holding in the first column the date
                 * and time an instance was started, and in the next column, the instanceId
                 * The date and time field is used to build a Calendar instance of the time at
                 * which the instance was started which is then used to compute the instance's
                 * total running time
                 */
                String[][] instanceArray = new String[rsRecordset.getRows().size()][2];

                Iterator iter = rsRecordset.getRows().iterator();

                while (iter.hasNext()) {

                    DynaBean dbDataRow = (DynaBean) iter.next();

                    instanceArray[instanceCounter][0] = dbDataRow.get("to_started").toString();
                    instanceArray[instanceCounter][1] = dbDataRow.get("to_instanceId").toString();
                    ++instanceCounter;
                }

                /*                if (Recordset.first()) {
                do {
                instanceArray[instanceCounter][0] = Recordset.getString("to_started");
                instanceArray[instanceCounter][1] = Recordset.getString("to_instanceId");
                ++instanceCounter;
                } while (Recordset.next());
                }
                 */
                System.out.println("Retrieved " + instanceCounter + " Rows");
                instanceCounter = 0;
                /*
                 * do{}while() loop iterates through instanceArray, computing each instances'
                 * total run time and determining if instance if fit for termination. If yes,
                 * then it terminates the instance and moves to the next till when all are done
                 */
                do {
                    if (!instanceArray[instanceCounter][1].equals("")) {
                        String dateStr = instanceArray[instanceCounter][0], retMsg = "";
                        int year = Integer.parseInt(dateStr.substring(0, 4)), month = Integer.parseInt(dateStr.substring(5, 7)), day = Integer.parseInt(dateStr.substring(8, 10)), hr = Integer.parseInt(dateStr.substring(11, 13)), min = Integer.parseInt(dateStr.substring(14, 16)), sec = Integer.parseInt(dateStr.substring(17, 19));
                        instanceStartTime = Calendar.getInstance();
                        instanceStartTime.set(year, month - 1, day, hr, min, sec);
                        curTime = Calendar.getInstance();
                        System.out.println("Instance Running Time: " + String.valueOf(TimeUnit.MILLISECONDS.toMinutes(curTime.getTimeInMillis()) - TimeUnit.MILLISECONDS.toMinutes(instanceStartTime.getTimeInMillis())) + " Minutes");
                        instanceStartTime.add(Calendar.MINUTE, 50);
                        if (TimeUnit.MILLISECONDS.toMinutes(curTime.getTimeInMillis()) > TimeUnit.MILLISECONDS.toMinutes(instanceStartTime.getTimeInMillis())) {
                            System.out.println("Terminating Instance " + instanceArray[instanceCounter][1] + "");
                            objec2 = new EC2(selectedAmi, availabilityZone);
                            objec2.TerminateEC2Instances(instanceArray[instanceCounter][1]);
                            errTime = new Date();
                            //System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM , DateFormat.MEDIUM).format(errTime).toString() + "update db_dn_pass_dev.to_ami_instances set to_instance_active=0,to_terminated='" + dateFormat.format(date)+ "' where to_instanceID='"+instanceArray[selectedInstanceId][1]+"'");// in (" + sTerminateInstances + ")");
                            Main.objDataAccess.WriteRecord("update db_dn_pass_dev.to_ami_instances set to_instance_active=0,to_terminated='" + dateFormat.format(date) + "' where to_instanceID='" + instanceArray[instanceCounter][1] + "'");//in (" + sTerminateInstances + ")");
                        }
                    } else {
                        System.out.println("No Instances Found With Non Empty to_instanceId");
                    }
                } while (++instanceCounter < instanceArray.length);
            } else {
                System.out.println("No running instances have been found");
            }
        } catch (Exception e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : clsScheduler.CheckInstancesToTerminate: ");
            e.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
        }

        return true;
    }

    private void RecordThisInstance(String sID, String sPrivateDNS, String sPublicDNS) {
        /*
         * Saves an instance to the database
         */
        Main.objDataAccess.SetDatabase(sTargetDatabase);
        DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        java.util.Date date = new java.util.Date();
        System.out.println(Calendar.getInstance().getTime());
        try {
            errTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + "insert into db_dn_pass_dev.to_ami_instances (to_instanceId,to_instance_privatedns,to_instance_publicdns,to_instance_active,to_started) values ('" + sID + "','" + sPrivateDNS + "','" + sPublicDNS + "',1,'" + dateFormat.format(date) + "')");
            Main.objDataAccess.WriteRecord("insert into db_dn_pass_dev.to_ami_instances (to_instanceId,to_instance_privatedns,to_instance_publicdns,to_instance_active,to_started) values ('" + sID + "','" + sPrivateDNS + "','" + sPublicDNS + "',1,'" + dateFormat.format(date) + "')");
        } catch (Exception e) {
            errTime = new Date();
            Main.currErrorMessage = e.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + " Error : clsScheduler.RecordThisInstance: ");
            e.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
        }

    }
}

package PPP_Analyzer;

import java.sql.*;
import java.text.*;
import java.util.Date;
import java.util.Iterator;
import org.apache.commons.beanutils.DynaBean;
import org.apache.commons.beanutils.RowSetDynaClass;

/**
 * Static synchronized connector to MySQL
 * @author philip
 *
 */
public class clsDataAccess {

    /** how we format dates for MySQL (e.g., 2005-08-30 13:01:55) */
    private static final DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    // Declare private variables
    private static String sUser, sPassword, sDatabaseName, sHostName;
    private Connection conMySQL = null;
    private static int iRecordCount = 0, dbPort = 3306;
    private static Date writeTime;
    //private static EmailDevTeam emailError = new EmailDevTeam();
    private RowSetDynaClass rsdcObject = null;

    public clsDataAccess(String hostName, String sDBUser, String sDBPassword, int port) {
        // Store parameters to private variables
        sUser = sDBUser;
        sPassword = sDBPassword;
        sDatabaseName = "";
        dbPort = port;
        sHostName = hostName;
    }

    private static DateFormat getMySQLDateFormat() {
        return dateFormat;
    }

    public String GetDatabase() {
        return sDatabaseName;
    }

    public void SetDatabase(String sDBase) {
        sDatabaseName = sDBase;
    }

    public static String GetUserName() {
        return sUser;
    }

    public static String GetPassword() {
        return sPassword;
    }

    public static String GetHostName() {
        return sHostName;
    }

    public static Integer GetPortNumber() {
        return dbPort;
    }

    /*
     * Connect to MySQL server
     */
    private synchronized boolean connectToMySQL() throws NotAvailableException {
        //Connection conn = null;
        writeTime = new Date();
        System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Connecting to " + sDatabaseName + "\n");
        int connectionAttempts = 0;
        boolean bSuccessStatus = false;

        do {
            try {
                Class.forName("com.mysql.jdbc.Driver").newInstance();
                conMySQL = DriverManager.getConnection("jdbc:mysql://" + sHostName + ":" + dbPort + "/" + sDatabaseName + "?user=" + sUser + "&password=" + sPassword);
                System.out.println("Connection now set to database: " + sDatabaseName);
                bSuccessStatus = true;
            } catch (ClassNotFoundException ex) {
                writeTime = new Date();
                Main.currErrorMessage = "Unable to connect to " + sDatabaseName + "\nFailed to load driver, error message was: " + ex.toString();
                System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " ,ClassNotFoundException: Unable to connect to " + sDatabaseName);
                System.err.println("\nFailed to load driver, error message was: " + ex.toString());
                connectionAttempts = 10;

            } catch (SQLException ex) {
                writeTime = new Date();
                Main.currErrorMessage = ex.toString();
                System.err.println("\n" + DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + ", SQLException: Failed to connect to server, error message was: ");
                System.err.println("SQLState: " + ex.getSQLState());
                System.err.println("MySQL Error: " + ex.getErrorCode());
                System.err.println("Using user: " + sUser);
                System.err.println("using pass: " + sPassword);
                System.err.println("SQLException: " + ex.getMessage());

                try{
                    Thread.currentThread().sleep(10000);
                }
                catch(InterruptedException exInt){
                    writeTime = new Date();
                    Main.currErrorMessage = " Error: InterruptedException occurred connecting to " + sDatabaseName + ": " + ex.getMessage();
                    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + ", Error: InterruptedException occurred connecting to " + sDatabaseName + ": " + ex.getMessage());
                }
            } catch (Exception e) {
                writeTime = new Date();
                Main.currErrorMessage = " Error occurred connecting to " + sDatabaseName + ": " + e.getMessage();
                System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occurred connecting to " + sDatabaseName + ": " + e.getMessage());

                try{
                    Thread.currentThread().sleep(10000);
                }
                catch(InterruptedException ex){
                    writeTime = new Date();
                    Main.currErrorMessage = " Error: InterruptedException occurred connecting to " + sDatabaseName + ": " + ex.getMessage();
                    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + ", Error: InterruptedException occurred connecting to " + sDatabaseName + ": " + ex.getMessage());
                }
            }
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " : Connection Attempt " + (1+connectionAttempts) + ", to database: " + sDatabaseName);
        } while (++connectionAttempts < 10 && bSuccessStatus == false);

        if (!bSuccessStatus) {
            Main.emailError.SendEmail(false);
            conMySQL = null;
            System.exit(1);
        }
        
        return bSuccessStatus;
    }

    public int WriteRecord(String sWriteString) {
        // Declare variables
        int iRecordKey = 0;
        try {
            // Get connection to MySQL then execute statement, otherwise print error message
            if (connectToMySQL()) {
                // Execute the statement
                Statement sSQLSelect = conMySQL.createStatement();
                iRecordKey = sSQLSelect.executeUpdate(sWriteString.replace("\\", "/"), 1);

                // Close statement and connection
                sSQLSelect.close();
                conMySQL.close();
            }
        } catch (Exception eX) {
            writeTime = new Date();
            Main.currErrorMessage = "Error occured in class 'clsDataAccess', Method 'WriteRecord': " + eX.toString() + "\n\nCurrent SQL Statement: " + sWriteString;
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess', Method 'WriteRecord': " + eX.toString());
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Current SQL Statement: " + sWriteString);
            eX.printStackTrace();
            Main.emailError.SendEmail();
            iRecordKey = 0;
            System.exit(1);
        }
        return iRecordKey;

    }

    public boolean ExecuteStoredProcedure(String strStoredProc) throws NotAvailableException {

        // Declare return value
        boolean blnStatus = false;

        try {
            if (connectToMySQL()) {
                // Get the data from the database table
                CallableStatement csStoredProc = conMySQL.prepareCall(strStoredProc);
                // Execute the stored procedure
                csStoredProc.execute();

                // Close the statement and connection
                csStoredProc.close();
                conMySQL.close();

                blnStatus = true;
            }
        } catch (SQLException eX) {
            writeTime = new Date();
            Main.currErrorMessage = eX.toString();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess': ");
            eX.printStackTrace();
            Main.emailError.SendEmail();
            blnStatus = false;
            System.exit(1);
        }

        return blnStatus;
    }

    public RowSetDynaClass getRecordSet(String sSELECTString) throws NotAvailableException {
        // Get the data from the database table
        Statement sSQLSelect = null;
        ResultSet rsRecordset = null;

        try {
            if (connectToMySQL()) {
                // Get the data from the database table
                sSQLSelect = conMySQL.createStatement();
                rsRecordset = sSQLSelect.executeQuery(sSELECTString.toString());

                // Generate RowSetDynaClass object
                rsdcObject = new RowSetDynaClass(rsRecordset, false);

                // Close the statement, recordset and connection
                rsRecordset.close();
                sSQLSelect.close();
                conMySQL.close();

                /*                // Get the number of rows to process
                rsRecordset.last();
                iRecordCount = rsRecordset.getRow();
                rsRecordset.first();
                // Check if a record was retrieved for processing
                if (iRecordCount == 0) {
                writeTime = new Date();
                System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + "There was no data extracted from\n\nDatabase: " + sDatabaseName + "\n\nusing\n\nStatement: " + sSELECTString);
                rsRecordset.close();
                rsRecordset = null;
                return null;
                }
                 */
            }

        } catch (Exception eX) {
            writeTime = new Date();
            Main.currErrorMessage = " Error occured in class 'clsDataAccess', Method 'getRecordSet': " + eX.toString() + "\n\nCurrent SQL Statement: " + sSELECTString;
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess', Method 'getRecordSet': " + eX.toString());
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Current SQL Statement: " + sSELECTString);
            eX.printStackTrace();
            rsRecordset = null;
            Main.emailError.SendEmail();
            rsdcObject = null;
            System.exit(1);
        }

        // Return the obecjt to the calling class
        return rsdcObject;

    }

    public int runUpdateStmt(String sqlStmt) {
        int recordCount = 0;
        try {
            if (this.connectToMySQL()) {
                Statement sSQLSelect = conMySQL.createStatement();
                recordCount = sSQLSelect.executeUpdate(sqlStmt);

                // Close statement and connection
                sSQLSelect.close();
                conMySQL.close();

            }
        } catch (Exception eX) {
            writeTime = new Date();
            Main.currErrorMessage = " Error occured in class 'clsDataAccess', Method 'runUpdateStmt': " + eX.toString() + "\n\nCurrent SQL Statement: " + sqlStmt;
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess', Method 'runUpdateStmt': " + eX.toString());
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Current SQL Statement: " + sqlStmt);
            eX.printStackTrace();
            Main.emailError.SendEmail();
            recordCount = 0;
            System.exit(1);
        }

        return recordCount;
    }

    public String GetOutputPath(String sPathSpecs) {

        String sOutputPath = "";
        String sSELECTString = "SELECT string_val FROM tm_variables WHERE LOWER(var_name) = '" + sPathSpecs + "'";

        RowSetDynaClass rsdcOutputPath = null;
        try {

            // Retrieve the output path
            rsdcOutputPath = getRecordSet(sSELECTString);

            Iterator iter = rsdcOutputPath.getRows().iterator();

            DynaBean dbDataRow = (DynaBean) iter.next();

            writeTime = new Date();
            System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Obtaining Output Path specifications for: " + sPathSpecs);
            sOutputPath = (String) dbDataRow.get("String_Val");

            // Nullify Dyna* objects
            dbDataRow = null;
            rsdcOutputPath = null;

        } catch (Exception eX) {
            writeTime = new Date();
            Main.currErrorMessage = " Error occured in class 'clsDataAccess', Method 'GetOutputPath': " + eX.toString() + "\n\nCurrent SQL Statement: " + sSELECTString;
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess', Method 'GetOutputPath': " + eX.toString());
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Current SQL Statement: " + sSELECTString);
            eX.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
        }

        return sOutputPath;
    }

    /*
    int getProcessId() {
    try {
    if (conMySQL == null || conMySQL.isClosed()) {
    System.out.println("Connection object, conMySQL is either null or closed");
    return 0;
    }
    Statement sSQLSelect = conMySQL.createStatement();
    ResultSet Recordset = sSQLSelect.executeQuery("SELECT connection_id() id");
    if (Recordset != null && Recordset.next()) {
    String processId = Recordset.getString("id");
    System.out.println("Process ID: " + processId);
    Recordset.close();
    return Integer.parseInt(processId);
    } else {
    Recordset.close();
    return 0;
    }
    } catch (Exception e) {
    //errTime = new Date();
    Main.currErrorMessage = e.toString();
    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(new Date()).toString() + " Error : clsScheduler.ReturnAvailableEC2Instance: ");
    e.printStackTrace();
    emailError.SendEmail();
    return 0;
    }
    }

    boolean killMySQLConnection() {
    /*
     * Retrieves any connections that are in iddle or sleep state and manually kills them
     * without having to wait for MySQL to run out the waittimeout for killing a thread
     *\/
    int processId = getProcessId();
    boolean retValue = false;

    try {
    if (conTerminator == null || conTerminator.isClosed()) {
    System.out.println("Connection object, conTerminator is either null or closed");
    //return false;
    }

    if (processId == 0) {
    System.out.println("Process ID Unretrievable");
    return false;
    }
    Statement stmt = conTerminator.createStatement();
    stmt.execute("kill " + processId);
    System.out.println("Process with ID " + processId + " killed successfully");
    retValue = true;
    } catch (SQLException ex) {
    writeTime = new Date();
    Main.currErrorMessage = ex.toString();
    System.err.println(ex.toString() + DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess': ");
    retValue = false;
    } catch (NotAvailableException ex) {
    writeTime = new Date();
    Main.currErrorMessage = ex.toString();
    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess': ");
    ex.printStackTrace();
    retValue = false;
    }

    if (closeCon()) {
    System.out.println("Connection to database has been closed");
    } else {
    System.out.println("Connection to database has NOT been closed");
    }

    cleanSleepingMySQLConnsFromHost();
    return retValue;
    }

    boolean cleanSleepingMySQLConnsFromHost() {
    boolean retValue = true;
    try {
    if (conTerminator == null || conTerminator.isClosed()) {
    System.out.println("Connection object is either null or closed");
    //return false;
    initTerminateConnToMySQL();
    }

    int processesToKillCounter = 0;
    String processId = "", connectingUser = "", conState = "", dbConnectedTo = "", connectingHost = "", processesToKill[][] = null;
    InetAddress addr = InetAddress.getLocalHost();

    Statement stmt = conTerminator.createStatement();
    ResultSet resultset = stmt.executeQuery("show processlist");

    if (resultset != null) {
    if (resultset.next()) {
    if (resultset.last()) {//Get last row of resultset
    processesToKill = new String[resultset.getRow()][5];
    }
    if (resultset.first());//Move resultset back to the first row

    do {
    processesToKill[processesToKillCounter][0] = resultset.getString("Id");
    processesToKill[processesToKillCounter][1] = resultset.getString("Command");
    processesToKill[processesToKillCounter][2] = resultset.getString("db");
    processesToKill[processesToKillCounter][3] = resultset.getString("Host");
    processesToKill[processesToKillCounter][4] = resultset.getString("User");
    processesToKillCounter++;
    } while (resultset.next());

    processesToKillCounter = 0;
    do {
    processId = processesToKill[processesToKillCounter][0];
    conState = processesToKill[processesToKillCounter][1];
    dbConnectedTo = processesToKill[processesToKillCounter][2];
    connectingHost = processesToKill[processesToKillCounter][3];
    connectingUser = processesToKill[processesToKillCounter][4];
    if (connectingUser.toLowerCase().compareTo(sUser.toLowerCase()) == 0 && conState.compareTo("Sleep") == 0 && connectingHost.toLowerCase().contains(addr.getHostName().toLowerCase()) && ((dbConnectedTo == null) || sDatabaseName.toLowerCase().compareTo(dbConnectedTo.toLowerCase()) == 0)) {// && processId > 0) {
    stmt.execute("kill " + processId);
    System.out.println("ID " + processId + " killed successfully");
    }
    } while (++processesToKillCounter < processesToKill.length);
    } else {
    retValue = true;
    }
    } else {
    System.out.println("RS is null");
    retValue = false;
    }
    } catch (SQLException ex) {
    writeTime = new Date();
    Main.currErrorMessage = ex.toString();
    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess': ");
    ex.printStackTrace();
    retValue = false;
    } catch (NotAvailableException ex) {
    writeTime = new Date();
    Main.currErrorMessage = ex.toString();
    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess': ");
    ex.printStackTrace();
    retValue = false;
    } catch (UnknownHostException ex) {
    writeTime = new Date();
    Main.currErrorMessage = ex.toString();
    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess': ");
    ex.printStackTrace();
    retValue = false;
    }
    return retValue;
    }

    boolean cleanResidualMySQLConnsFromHost() {
    boolean retValue = true;
    try {
    if (conTerminator == null || conTerminator.isClosed()) {
    System.out.println("Connection object is either null or closed");
    //return false;
    initTerminateConnToMySQL();
    }

    int processesToKillCounter = 0;
    String processId = "", connectingUser = "", conState = "", connectingHost = "", processesToKill[][] = null;
    InetAddress addr = InetAddress.getLocalHost();

    Statement stmt = conTerminator.createStatement();
    ResultSet resultset = stmt.executeQuery("show processlist");

    if (resultset != null) {
    if (resultset.next()) {
    if (resultset.last()) {//Get last row of resultset
    processesToKill = new String[resultset.getRow()][5];
    }
    if (resultset.first());//Move resultset back to the first row

    do {
    processesToKill[processesToKillCounter][0] = resultset.getString("Id");
    processesToKill[processesToKillCounter][1] = resultset.getString("Command");
    processesToKill[processesToKillCounter][2] = resultset.getString("db");
    processesToKill[processesToKillCounter][3] = resultset.getString("Host");
    processesToKill[processesToKillCounter][4] = resultset.getString("User");
    processesToKillCounter++;
    } while (resultset.next());

    processesToKillCounter = 0;
    do {
    processId = processesToKill[processesToKillCounter][0];
    conState = processesToKill[processesToKillCounter][1];
    connectingHost = processesToKill[processesToKillCounter][3];
    connectingUser = processesToKill[processesToKillCounter][4];
    if (connectingUser.toLowerCase().compareTo(sUser.toLowerCase()) == 0 && conState.compareTo("Sleep") == 0 && connectingHost.toLowerCase().contains(addr.getHostName().toLowerCase())) {// || dbConnectedTo == null) || databaseConnectedTo.toLowerCase().compareTo(dbConnectedTo.toLowerCase()) == 0) {// && processId > 0) {
    stmt.execute("kill " + processId);
    System.out.println("ID " + processId + " killed successfully");
    }
    } while (++processesToKillCounter < processesToKill.length);
    } else {
    retValue = true;
    }
    } else {
    System.out.println("RS is null");
    retValue = false;
    }
    } catch (SQLException ex) {
    writeTime = new Date();
    Main.currErrorMessage = ex.toString();
    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess': ");
    ex.printStackTrace();
    retValue = false;
    } catch (NotAvailableException ex) {
    writeTime = new Date();
    Main.currErrorMessage = ex.toString();
    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess': ");
    ex.printStackTrace();
    retValue = false;
    } catch (UnknownHostException ex) {
    writeTime = new Date();
    Main.currErrorMessage = ex.toString();
    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess': ");
    ex.printStackTrace();
    retValue = false;
    }
    return retValue;
    }
     */

    /*
    boolean cleanSleepingMySQLConns() {
    /*
     * Retrieves any connections that are in iddle or sleep state and manually kills them
     * without having to wait for MySQL to run out the waittimeout for killing a thread
     *\/
    boolean retValue = true;
    try {
    if (conMySQL == null || conMySQL.isClosed()) {
    System.out.println("Connection object is either null or closed");
    return false;
    }
    int sleepTime = 0, processId = 0;
    String conState = "";
    Statement stmt = null;
    ResultSet resultset = this.getRecordSet("show processlist");
    if (resultset != null) {
    if (resultset.next()) {
    do {
    processId = Integer.parseInt(resultset.getString("Id"));
    sleepTime = Integer.parseInt(resultset.getString("Time"));
    conState = resultset.getString("Command");
    if (conState.compareTo("Sleep") == 0 && sleepTime > MasterScheduler.objScheduler.maxMySQLConSleepTime && processId > 0) {
    stmt = conMySQL.createStatement();
    if (stmt.execute("kill " + processId)) {
    System.out.println("Process with ID " + processId + " killed successfully");
    } else {
    retValue = false;
    System.out.println("Process with ID " + processId + " has not been killed successfully");
    }
    }
    } while (resultset.next());
    } else {
    retValue = true;
    }
    } else {
    System.out.println("RS is null");
    retValue = false;
    }
    } catch (NotAvailableException ex) {
    writeTime = new Date();
    Main.currErrorMessage = ex.toString();
    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess': ");
    ex.printStackTrace();
    retValue = false;
    } catch (SQLException ex) {
    writeTime = new Date();
    Main.currErrorMessage = ex.toString();
    System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime).toString() + " Error occured in class 'clsDataAccess': ");
    ex.printStackTrace();
    retValue = false;
    }
    return retValue;
    }
     */
}

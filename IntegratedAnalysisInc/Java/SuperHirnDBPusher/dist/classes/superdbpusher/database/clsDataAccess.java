package ch.superdbpusher.database;

import java.sql.*;
import java.text.*;
import java.util.Date;
import java.util.Iterator;
import org.apache.commons.beanutils.DynaBean;
import org.apache.commons.beanutils.RowSetDynaClass;

import ch.superdbpusher.Manager;

/**
 * Static synchronized connector to MySQL
 * 
 * @author philip
 * 
 */
public class clsDataAccess {
	
	

	static private Boolean DEBUG_SQL = false;



	/** how we format dates for MySQL (e.g., 2005-08-30 13:01:55) */
	private static final DateFormat dateFormat = new SimpleDateFormat(
			"yyyy-MM-dd HH:mm:ss");
	// Declare private variables
	private static String sUser, sPassword, sDatabaseName, sHostName;
	private Connection conMySQL = null;
	private static int iRecordCount = 0, dbPort = 3306;
	private static Date writeTime;
	// private static EmailDevTeam emailError = new EmailDevTeam();
	private RowSetDynaClass rsdcObject = null;

	public clsDataAccess(String hostName, String sDBUser, String sDBPassword,
			int port, String iDatabase) {
		// Store parameters to private variables
		sUser = sDBUser;
		sPassword = sDBPassword;
		sDatabaseName = iDatabase;
		dbPort = port;
		sHostName = hostName;
	}

	/*
	 * public clsDataAccess(String sDBUser, String sDBPassword) { // Store
	 * parameters to private variables sUser = sDBUser; sPassword = sDBPassword;
	 * sDatabaseName = ""; }
	 */
	private static DateFormat getMySQLDateFormat() {
		return dateFormat;
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

	/*
	 * public boolean closeCon() {
	 * 
	 * //Closes the current connection to the database
	 * 
	 * try { if (conMySQL != null) { conMySQL.close(); return true; } else {
	 * return false; } } catch (SQLException ex) { return false; } finally { if
	 * (conMySQL != null) { try { conMySQL.close(); return true; } catch
	 * (SQLException ex) { return false; } } } }
	 */

	/*
	 * Connect to MySQL server
	 */
	private synchronized boolean connectToMySQL()
			throws DatabaseAccessException {
		// Connection conn = null;
		writeTime = new Date();
		System.out.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
				DateFormat.MEDIUM).format(writeTime).toString()
				+ " Connecting to " + sDatabaseName + "\n");

		try {
			Class.forName("com.mysql.jdbc.Driver").newInstance();

			conMySQL = DriverManager.getConnection("jdbc:mysql://" + sHostName
					+ ":" + dbPort + "/" + sDatabaseName + "?user=" + sUser
					+ "&password=" + sPassword);
			// System.out.println("Connection now set to database: " +
			// sDatabaseName);
		} catch (ClassNotFoundException ex) {
			writeTime = new Date();
			Manager.currErrorMessage = "Unable to connect to " + sDatabaseName
					+ "\nFailed to load driver, error message was: "
					+ ex.toString();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime)
					.toString()
					+ " Unable to connect to " + sDatabaseName);
			System.err.println("\nFailed to load driver, error message was: "
					+ ex.toString());
			// emailError.SendEmail();
			conMySQL = null;
			System.exit(1);
		} catch (SQLException ex) {
			writeTime = new Date();
			Manager.currErrorMessage = ex.toString();
			System.err.println("\n"
					+ DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
							DateFormat.MEDIUM).format(writeTime).toString()
					+ " Failed to connect to server, error message was: ");
			System.err.println("SQLState: " + ex.getSQLState());
			System.err.println("MySQL Error: " + ex.getErrorCode());
			System.err.println("Using user: " + sUser);
			System.err.println("using pass: " + sPassword);
			System.err.println("SQLException: " + ex.getMessage());
			// emailError.SendEmail();
			conMySQL = null;
			System.exit(1);
		} catch (Exception e) {
			writeTime = new Date();
			Manager.currErrorMessage = " Error occurred connecting to "
					+ sDatabaseName + ": " + e.getMessage();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime)
					.toString()
					+ " Error occurred connecting to "
					+ sDatabaseName
					+ ": "
					+ e.getMessage());
			// emailError.SendEmail();
			conMySQL = null;
			System.exit(1);
		}

		return true;
	}

	/*
	 * Connect to MySQL server and return the connection instance.
	 * 
	 * @return Connection, null if the connection could not be established
	 */
	private synchronized Connection getConnection()
			throws DatabaseAccessException {
		try {

			if (this.conMySQL == null) {

				Class.forName("com.mysql.jdbc.Driver").newInstance();
				conMySQL = DriverManager.getConnection("jdbc:mysql://"
						+ sHostName + ":" + dbPort + "/" + sDatabaseName
						+ "?user=" + sUser + "&password=" + sPassword);
				return this.conMySQL;
			} else {
				return this.conMySQL;
			}

		} catch (ClassNotFoundException ex) {
			writeTime = new Date();
			System.out.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime)
					.toString()
					+ " Connecting to " + sDatabaseName + "\n");
			writeTime = new Date();
			Manager.currErrorMessage = "Unable to connect to " + sDatabaseName
					+ "\nFailed to load driver, error message was: "
					+ ex.toString();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime)
					.toString()
					+ " Unable to connect to " + sDatabaseName);
			System.err.println("\nFailed to load driver, error message was: "
					+ ex.toString());
			// emailError.SendEmail();
			conMySQL = null;
			System.exit(1);
		} catch (SQLException ex) {
			writeTime = new Date();
			Manager.currErrorMessage = ex.toString();
			System.err.println("\n"
					+ DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
							DateFormat.MEDIUM).format(writeTime).toString()
					+ " Failed to connect to server, error message was: ");
			System.err.println("SQLState: " + ex.getSQLState());
			System.err.println("MySQL Error: " + ex.getErrorCode());
			System.err.println("Using user: " + sUser);
			System.err.println("using pass: " + sPassword);
			System.err.println("SQLException: " + ex.getMessage());
			// emailError.SendEmail();
			conMySQL = null;
			System.exit(1);
		} catch (Exception e) {
			writeTime = new Date();
			Manager.currErrorMessage = " Error occurred connecting to "
					+ sDatabaseName + ": " + e.getMessage();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime)
					.toString()
					+ " Error occurred connecting to "
					+ sDatabaseName
					+ ": "
					+ e.getMessage());
			// emailError.SendEmail();
			conMySQL = null;
			System.exit(1);
		}

		return null;
	}

	public int WriteRecord(String sWriteString) {
		// Declare variables
		int iRecordKey = 0;
		try {
			// Get connection to MySQL then execute statement, otherwise print
			// error message
			if (connectToMySQL()) {
				// Execute the statement
				Statement sSQLSelect = conMySQL.createStatement();
				iRecordKey = sSQLSelect.executeUpdate(sWriteString.replace(
						"\\", "/"), 1);

				// Close statement and connection
				sSQLSelect.close();
				conMySQL.close();
			}
		} catch (Exception eX) {
			writeTime = new Date();
			Manager.currErrorMessage = "Error occured in class 'clsDataAccess': "
					+ eX.toString();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime)
					.toString()
					+ " Error occured in class 'clsDataAccess': ");
			eX.printStackTrace();
			// emailError.SendEmail();
			iRecordKey = 0;
			System.exit(1);
		}
		return iRecordKey;

	}

	public boolean ExecuteStoredProcedure(String strStoredProc)
			throws DatabaseAccessException {

		// System.out.println(strStoredProc);
		// return true;

		// Declare return value
		boolean blnStatus = false;

		try {
			if (connectToMySQL()) {
				// Get the data from the database table
				CallableStatement csStoredProc = conMySQL
						.prepareCall(strStoredProc);
				// Execute the stored procedure
				csStoredProc.execute();

				// Close the statement and connection
				csStoredProc.close();
				conMySQL.close();

				blnStatus = true;
			}
		} catch (SQLException eX) {
			writeTime = new Date();
			// Main.currErrorMessage = eX.toString();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime)
					.toString()
					+ " Error occured in class 'clsDataAccess': ");
			eX.printStackTrace();
			// emailError.SendEmail();
			blnStatus = false;
			System.exit(1);
		}

		return blnStatus;
	}

	/*
	 * Performing sql statements directly
	 * 
	 * @param String sql statement
	 * 
	 * @return boolean
	 */

	public boolean performSQLStatement(String sql)
			throws DatabaseAccessException {
			
		if( DEBUG_SQL ){
			System.out.println( sql );
			return true;
		}

		// Declare return value
		boolean blnStatus = false;
		//System.out.println(sql);

		try {
			Connection myConnection = this.getConnection();
			if (myConnection != null) {

				Statement stmt = myConnection.createStatement();
				stmt.executeUpdate(sql);

				blnStatus = true;
			}
		} catch (SQLException eX) {

			writeTime = new Date();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime)
					.toString()
					+ " Error occured in class 'clsDataAccess': ");
			eX.printStackTrace();
			// emailError.SendEmail();
			blnStatus = false;

			System.out.println(sql);

			System.exit(1);
		}

		return blnStatus;
	}

	public void close() {
		try {
			if (this.conMySQL != null) {
				this.conMySQL.close();
			}
			this.conMySQL = null;
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	public RowSetDynaClass getRecordSet(String sSELECTString)
			throws DatabaseAccessException {
			
		// Get the data from the database table
		Statement sSQLSelect = null;
		ResultSet rsRecordset = null;
		
		try {
		
			if( DEBUG_SQL ){
				System.out.println( sSELECTString );
				return null;
			}

			Connection myConnection = this.getConnection();
			if (myConnection != null) {
		
				// Get the data from the database table
				sSQLSelect = myConnection.createStatement();
				rsRecordset = sSQLSelect.executeQuery(sSELECTString.toString());

				// Generate RowSetDynaClass object
				rsdcObject = new RowSetDynaClass(rsRecordset, false);

				// Close the statement, recordset
				rsRecordset.close();
				sSQLSelect.close();
			}

		} catch (Exception eX) {
			writeTime = new Date();
			Manager.currErrorMessage = eX.toString();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime)
					.toString()
					+ " Error occured in class 'clsDataAccess': ");
			eX.printStackTrace();
			rsRecordset = null;
			// emailError.SendEmail();
			rsdcObject = null;
			System.exit(1);
		}

		// Return the obecjt to the calling class
		return rsdcObject;

	}

	public int runUpdateStmt(String sqlStmt) {
		int recordCount = 0;
		
		if( DEBUG_SQL ){
			System.out.println( sqlStmt );
			return 0;
		}

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
			Manager.currErrorMessage = eX.toString();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime)
					.toString()
					+ " Error occured in class 'clsDataAccess': ");
			eX.printStackTrace();
			// emailError.SendEmail();
			recordCount = 0;
			System.exit(1);
		}

		return recordCount;
	}

	public String GetOutputPath(String sPathSpecs) {
		String sOutputPath = "";
		RowSetDynaClass rsdcOutputPath = null;
		try {

			// Retrieve the output path
			rsdcOutputPath = getRecordSet("SELECT string_val FROM tm_variables WHERE LOWER(var_name) = '"
					+ sPathSpecs + "'");

			Iterator iter = rsdcOutputPath.getRows().iterator();

			DynaBean dbDataRow = (DynaBean) iter.next();

			writeTime = new Date();
			System.out.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime)
					.toString()
					+ " Obtaining Output Path specifications for: "
					+ sPathSpecs);
			sOutputPath = (String) dbDataRow.get("String_Val");

			// Nullify Dyna* objects
			dbDataRow = null;
			rsdcOutputPath = null;

		} catch (Exception eX) {
			writeTime = new Date();
			Manager.currErrorMessage = eX.toString();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(writeTime)
					.toString()
					+ " Error occured in class 'clsDataAccess': ");
			eX.printStackTrace();
			// emailError.SendEmail();
			System.exit(1);
		}

		return sOutputPath;
	}

	/*
	 * int getProcessId() { try { if (conMySQL == null || conMySQL.isClosed()) {
	 * System
	 * .out.println("Connection object, conMySQL is either null or closed");
	 * return 0; } Statement sSQLSelect = conMySQL.createStatement(); ResultSet
	 * Recordset = sSQLSelect.executeQuery("SELECT connection_id() id"); if
	 * (Recordset != null && Recordset.next()) { String processId =
	 * Recordset.getString("id"); System.out.println("Process ID: " +
	 * processId); Recordset.close(); return Integer.parseInt(processId); } else
	 * { Recordset.close(); return 0; } } catch (Exception e) { //errTime = new
	 * Date(); Main.currErrorMessage = e.toString();
	 * System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
	 * DateFormat.MEDIUM).format(new Date()).toString() +
	 * " Error : clsScheduler.ReturnAvailableEC2Instance: ");
	 * e.printStackTrace(); emailError.SendEmail(); return 0; } }
	 * 
	 * boolean killMySQLConnection() { /* Retrieves any connections that are in
	 * iddle or sleep state and manually kills them without having to wait for
	 * MySQL to run out the waittimeout for killing a thread\/ int processId =
	 * getProcessId(); boolean retValue = false;
	 * 
	 * try { if (conTerminator == null || conTerminator.isClosed()) {
	 * System.out.
	 * println("Connection object, conTerminator is either null or closed");
	 * //return false; }
	 * 
	 * if (processId == 0) { System.out.println("Process ID Unretrievable");
	 * return false; } Statement stmt = conTerminator.createStatement();
	 * stmt.execute("kill " + processId); System.out.println("Process with ID "
	 * + processId + " killed successfully"); retValue = true; } catch
	 * (SQLException ex) { writeTime = new Date(); Main.currErrorMessage =
	 * ex.toString(); System.err.println(ex.toString() +
	 * DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
	 * DateFormat.MEDIUM).format(writeTime).toString() +
	 * " Error occured in class 'clsDataAccess': "); retValue = false; } catch
	 * (NotAvailableException ex) { writeTime = new Date();
	 * Main.currErrorMessage = ex.toString();
	 * System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
	 * DateFormat.MEDIUM).format(writeTime).toString() +
	 * " Error occured in class 'clsDataAccess': "); ex.printStackTrace();
	 * retValue = false; }
	 * 
	 * if (closeCon()) {
	 * System.out.println("Connection to database has been closed"); } else {
	 * System.out.println("Connection to database has NOT been closed"); }
	 * 
	 * cleanSleepingMySQLConnsFromHost(); return retValue; }
	 * 
	 * boolean cleanSleepingMySQLConnsFromHost() { boolean retValue = true; try
	 * { if (conTerminator == null || conTerminator.isClosed()) {
	 * System.out.println("Connection object is either null or closed");
	 * //return false; initTerminateConnToMySQL(); }
	 * 
	 * int processesToKillCounter = 0; String processId = "", connectingUser =
	 * "", conState = "", dbConnectedTo = "", connectingHost = "",
	 * processesToKill[][] = null; InetAddress addr =
	 * InetAddress.getLocalHost();
	 * 
	 * Statement stmt = conTerminator.createStatement(); ResultSet resultset =
	 * stmt.executeQuery("show processlist");
	 * 
	 * if (resultset != null) { if (resultset.next()) { if (resultset.last())
	 * {//Get last row of resultset processesToKill = new
	 * String[resultset.getRow()][5]; } if (resultset.first());//Move resultset
	 * back to the first row
	 * 
	 * do { processesToKill[processesToKillCounter][0] =
	 * resultset.getString("Id"); processesToKill[processesToKillCounter][1] =
	 * resultset.getString("Command");
	 * processesToKill[processesToKillCounter][2] = resultset.getString("db");
	 * processesToKill[processesToKillCounter][3] = resultset.getString("Host");
	 * processesToKill[processesToKillCounter][4] = resultset.getString("User");
	 * processesToKillCounter++; } while (resultset.next());
	 * 
	 * processesToKillCounter = 0; do { processId =
	 * processesToKill[processesToKillCounter][0]; conState =
	 * processesToKill[processesToKillCounter][1]; dbConnectedTo =
	 * processesToKill[processesToKillCounter][2]; connectingHost =
	 * processesToKill[processesToKillCounter][3]; connectingUser =
	 * processesToKill[processesToKillCounter][4]; if
	 * (connectingUser.toLowerCase().compareTo(sUser.toLowerCase()) == 0 &&
	 * conState.compareTo("Sleep") == 0 &&
	 * connectingHost.toLowerCase().contains(addr.getHostName().toLowerCase())
	 * && ((dbConnectedTo == null) ||
	 * sDatabaseName.toLowerCase().compareTo(dbConnectedTo.toLowerCase()) == 0))
	 * {// && processId > 0) { stmt.execute("kill " + processId);
	 * System.out.println("ID " + processId + " killed successfully"); } } while
	 * (++processesToKillCounter < processesToKill.length); } else { retValue =
	 * true; } } else { System.out.println("RS is null"); retValue = false; } }
	 * catch (SQLException ex) { writeTime = new Date(); Main.currErrorMessage =
	 * ex.toString();
	 * System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
	 * DateFormat.MEDIUM).format(writeTime).toString() +
	 * " Error occured in class 'clsDataAccess': "); ex.printStackTrace();
	 * retValue = false; } catch (NotAvailableException ex) { writeTime = new
	 * Date(); Main.currErrorMessage = ex.toString();
	 * System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
	 * DateFormat.MEDIUM).format(writeTime).toString() +
	 * " Error occured in class 'clsDataAccess': "); ex.printStackTrace();
	 * retValue = false; } catch (UnknownHostException ex) { writeTime = new
	 * Date(); Main.currErrorMessage = ex.toString();
	 * System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
	 * DateFormat.MEDIUM).format(writeTime).toString() +
	 * " Error occured in class 'clsDataAccess': "); ex.printStackTrace();
	 * retValue = false; } return retValue; }
	 * 
	 * boolean cleanResidualMySQLConnsFromHost() { boolean retValue = true; try
	 * { if (conTerminator == null || conTerminator.isClosed()) {
	 * System.out.println("Connection object is either null or closed");
	 * //return false; initTerminateConnToMySQL(); }
	 * 
	 * int processesToKillCounter = 0; String processId = "", connectingUser =
	 * "", conState = "", connectingHost = "", processesToKill[][] = null;
	 * InetAddress addr = InetAddress.getLocalHost();
	 * 
	 * Statement stmt = conTerminator.createStatement(); ResultSet resultset =
	 * stmt.executeQuery("show processlist");
	 * 
	 * if (resultset != null) { if (resultset.next()) { if (resultset.last())
	 * {//Get last row of resultset processesToKill = new
	 * String[resultset.getRow()][5]; } if (resultset.first());//Move resultset
	 * back to the first row
	 * 
	 * do { processesToKill[processesToKillCounter][0] =
	 * resultset.getString("Id"); processesToKill[processesToKillCounter][1] =
	 * resultset.getString("Command");
	 * processesToKill[processesToKillCounter][2] = resultset.getString("db");
	 * processesToKill[processesToKillCounter][3] = resultset.getString("Host");
	 * processesToKill[processesToKillCounter][4] = resultset.getString("User");
	 * processesToKillCounter++; } while (resultset.next());
	 * 
	 * processesToKillCounter = 0; do { processId =
	 * processesToKill[processesToKillCounter][0]; conState =
	 * processesToKill[processesToKillCounter][1]; connectingHost =
	 * processesToKill[processesToKillCounter][3]; connectingUser =
	 * processesToKill[processesToKillCounter][4]; if
	 * (connectingUser.toLowerCase().compareTo(sUser.toLowerCase()) == 0 &&
	 * conState.compareTo("Sleep") == 0 &&
	 * connectingHost.toLowerCase().contains(addr.getHostName().toLowerCase()))
	 * {// || dbConnectedTo == null) ||
	 * databaseConnectedTo.toLowerCase().compareTo(dbConnectedTo.toLowerCase())
	 * == 0) {// && processId > 0) { stmt.execute("kill " + processId);
	 * System.out.println("ID " + processId + " killed successfully"); } } while
	 * (++processesToKillCounter < processesToKill.length); } else { retValue =
	 * true; } } else { System.out.println("RS is null"); retValue = false; } }
	 * catch (SQLException ex) { writeTime = new Date(); Main.currErrorMessage =
	 * ex.toString();
	 * System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
	 * DateFormat.MEDIUM).format(writeTime).toString() +
	 * " Error occured in class 'clsDataAccess': "); ex.printStackTrace();
	 * retValue = false; } catch (NotAvailableException ex) { writeTime = new
	 * Date(); Main.currErrorMessage = ex.toString();
	 * System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
	 * DateFormat.MEDIUM).format(writeTime).toString() +
	 * " Error occured in class 'clsDataAccess': "); ex.printStackTrace();
	 * retValue = false; } catch (UnknownHostException ex) { writeTime = new
	 * Date(); Main.currErrorMessage = ex.toString();
	 * System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
	 * DateFormat.MEDIUM).format(writeTime).toString() +
	 * " Error occured in class 'clsDataAccess': "); ex.printStackTrace();
	 * retValue = false; } return retValue; }
	 */

	/*
	 * boolean cleanSleepingMySQLConns() { /* Retrieves any connections that are
	 * in iddle or sleep state and manually kills them without having to wait
	 * for MySQL to run out the waittimeout for killing a thread\/ boolean
	 * retValue = true; try { if (conMySQL == null || conMySQL.isClosed()) {
	 * System.out.println("Connection object is either null or closed"); return
	 * false; } int sleepTime = 0, processId = 0; String conState = "";
	 * Statement stmt = null; ResultSet resultset =
	 * this.getRecordSet("show processlist"); if (resultset != null) { if
	 * (resultset.next()) { do { processId =
	 * Integer.parseInt(resultset.getString("Id")); sleepTime =
	 * Integer.parseInt(resultset.getString("Time")); conState =
	 * resultset.getString("Command"); if (conState.compareTo("Sleep") == 0 &&
	 * sleepTime > MasterScheduler.objScheduler.maxMySQLConSleepTime &&
	 * processId > 0) { stmt = conMySQL.createStatement(); if
	 * (stmt.execute("kill " + processId)) {
	 * System.out.println("Process with ID " + processId +
	 * " killed successfully"); } else { retValue = false;
	 * System.out.println("Process with ID " + processId +
	 * " has not been killed successfully"); } } } while (resultset.next()); }
	 * else { retValue = true; } } else { System.out.println("RS is null");
	 * retValue = false; } } catch (NotAvailableException ex) { writeTime = new
	 * Date(); Main.currErrorMessage = ex.toString();
	 * System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
	 * DateFormat.MEDIUM).format(writeTime).toString() +
	 * " Error occured in class 'clsDataAccess': "); ex.printStackTrace();
	 * retValue = false; } catch (SQLException ex) { writeTime = new Date();
	 * Main.currErrorMessage = ex.toString();
	 * System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM,
	 * DateFormat.MEDIUM).format(writeTime).toString() +
	 * " Error occured in class 'clsDataAccess': "); ex.printStackTrace();
	 * retValue = false; } return retValue; }
	 */
}

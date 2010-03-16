/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.util.Date;
import java.text.*;

/**
 * 
 * @author Edwin
 */
public class Main {

	// Declare public variables and objects
	public static clsDataAccess objDataAccess;
	public static clsAmazon amazonObject;
	public static clsProcessList objProcessList;
	// SIK09092009 variables to support saxon implementation
	public static clsProcessItem[] oProcessList;
	public static clsProcessItem oProcessItem;
	public static MasterScheduler oMasterScheduler;
	public static int numThreadErrors = 0;
	public static String strStoredProc = "", processName = "";
	//
	public static String sXMLFilePath = "";
	// SIK09092009 Change the command to be either SAXON or XTANDEM
	private static final String commandHelp = "Please provide the process to run - 'XTANDEM' , 'SAXON' or 'SCHEDULER'";
	public static String currErrorMessage = null;
	private static Date errTime;
	protected static EmailDevTeam emailError = new EmailDevTeam();
	ClusterClass clusterObj;

	/**
	 * @param args
	 *            the sCommand line arguments
	 */
	public static void main(String[] args) {

		// SIK09092009 Support Saxon logic for connecting to target DBs
		// Connection conMySQL = null;
		//

		if (args.length < 6) {
			errTime = new Date();
			currErrorMessage = "Too few arguments.\n" + commandHelp;
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ " Too few arguments.\n" + commandHelp);
			emailError.SendEmail();
			System.exit(1);
		}

		String sCommand = args[0].toLowerCase();
		processName = sCommand;

		if (sCommand.equals("scheduler")) {
			oMasterScheduler = new MasterScheduler();
			boolean isScheduled = oMasterScheduler.Schedule(args[1], args[2],
					args[3], args[4], Integer.parseInt(args[5]));
			/*
			 * if(args.length>4){ isScheduled=oMasterScheduler.Schedule(args[1],
			 * args[2], args[3],Integer.parseInt(args[4])); } else
			 * if(args.length>3){ isScheduled=oMasterScheduler.Schedule(args[1],
			 * args[2], args[3]); } else{
			 * isScheduled=oMasterScheduler.Schedule(); }
			 */
			if (!isScheduled) {
				errTime = new Date();
				currErrorMessage = "Failed to start scheduler";
				System.err.println(DateFormat.getDateTimeInstance(
						DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
						.toString()
						+ "Failed to start scheduler");
				emailError.SendEmail();
				System.exit(1);
			}
		}
		// SIK09092009 determine whether correct process (xtandem or saxon has
		// been specified
		boolean process;
		process = sCommand.equals("xtandem") || sCommand.equals("generic_script_mode") || sCommand.equals("saxon")
				|| sCommand.equals("cluster");

		if (process) {
			// Instantiate public Data Access object
			// SIKTest
			// objDataAccess = new clsDataAccess("edwin_pass_tgt", "polk9876");
			objDataAccess = new clsDataAccess(args[1], args[3], args[4],
					Integer.parseInt(args[5]));

			// Load the Process List class
			try {
				// java.net.InetAddress i = java.net.InetAddress.getLocalHost();
				// System.out.println("IP address for this Instance : " +
				// i.getHostName());
				// Class.forName("create_input_xml.clsBuildXMLFile");
				Class.forName("PPP_Analyzer.clsProcessList");
			} catch (Exception e) {
				errTime = new Date();
				currErrorMessage = " Failed to instantiate the processing list. The program will terminate.";
				System.out
						.println(DateFormat.getDateTimeInstance(
								DateFormat.MEDIUM, DateFormat.MEDIUM).format(
								errTime).toString()
								+ " Failed to instantiate the processing list. The program will terminate.");
				emailError.SendEmail();
				// objDataAccess.killMySQLConnection();
				// Kill the data access object
				objDataAccess = null;
				emailError.SendEmail();
				System.exit(1);
			}

			// Run the program in a continuous loop
			do {
				/*
				 * ************************************************* This
				 * section added to aid in faster testing of the cluster related
				 * classes, i.e. ClusterClass and CDHitClusterOutputParser
				 */
				/*
				 * if (amazonObject == null) { amazonObject = new clsAmazon(); }
				 * if (amazonObject.createBucket("ia_cluster_files")) {
				 * System.out.println("bucket created"); System.exit(0); } else
				 * { System.exit(0); }
				 * 
				 * 
				 * 
				 * 
				 * if (new Main().doCluster(args[2])) { continue; } else if
				 * (true) { break; }
				 */
				// **************************************************

				// SIK09092009
				numThreadErrors = 0;
				// Instantiate Process List object
				objProcessList = new clsProcessList(args[2]);
				// Execute the class
				// SIK09092009 Execute correct process depending on user
				// specification
				if (sCommand.equals("xtandem")) {
					objProcessList.LoadProcessList("XTANDEM");
				}
				/*
				 * Lukas: Adding a new condition for superhirn running mode
				 */
				else if (sCommand.equals("generic_script_mode")) {
					objProcessList.LoadProcessList("GENERIC_SCRIPT_MODE");
				} 
				else {
					if (objProcessList.LoadProcessList("SAXON")) {
						// Write code here to call the SPs in the target DB
						if (numThreadErrors == 0) {
							try {
								// Loop through the Process Items list and call
								// the SPs for each database
								oProcessList = objProcessList.ProcessList();
								for (int iProcessCount = 0; iProcessCount <= oProcessList.length - 1; iProcessCount++) {
									oProcessItem = oProcessList[iProcessCount];
									objDataAccess.SetDatabase(oProcessItem
											.ActiveTargetDatabase());
									errTime = new Date();
									System.out.println(DateFormat
											.getDateTimeInstance(
													DateFormat.MEDIUM,
													DateFormat.MEDIUM).format(
													errTime).toString()
											+ "calling "
											+ oProcessItem
													.ActiveTargetDatabase()
											+ ".sp_load_all...");

									// Prepare the callable statement
									strStoredProc = "{ call "
											+ oProcessItem
													.ActiveTargetDatabase()
											+ ".sp_load_all() }";
									// CallableStatement csStoredProc =
									// conMySQL.prepareCall(strStoredProc);
									// Execute the stored procedure
									// csStoredProc.execute();

									if (objDataAccess
											.ExecuteStoredProcedure(strStoredProc))
										;
									errTime = new Date();
									System.out
											.println(DateFormat
													.getDateTimeInstance(
															DateFormat.MEDIUM,
															DateFormat.MEDIUM)
													.format(errTime).toString()
													+ "\nfinished executing sp_load_all on "
													+ oProcessItem
															.ActiveTargetDatabase()
													+ "...");
								}
							} catch (NotAvailableException ne) {
								errTime = new Date();
								Main.currErrorMessage = ne.toString();
								// if (conMySQL == null) {
								System.err.println(DateFormat
										.getDateTimeInstance(DateFormat.MEDIUM,
												DateFormat.MEDIUM).format(
												errTime).toString()
										+ " Database not available");
								emailError.SendEmail();
								System.exit(1);
							}
						} else {
							errTime = new Date();
							currErrorMessage = "Could not call sp_load_all because one of threads returned an error.";
							System.err
									.println(DateFormat.getDateTimeInstance(
											DateFormat.MEDIUM,
											DateFormat.MEDIUM).format(errTime)
											.toString()
											+ " Could not call sp_load_all because one of threads returned an error.");
							emailError.SendEmail();
							System.exit(1);
						}
					}
					/*
					 * Start cluster routine after saxon has finished executing
					 */

					if (new Main().doCluster(args[2])) {
						continue;
					} else {
						errTime = new Date();
						currErrorMessage = "Failed to initiate Clustering process";
						System.err.println(DateFormat.getDateTimeInstance(
								DateFormat.MEDIUM, DateFormat.MEDIUM).format(
								errTime).toString()
								+ ": Failed to initiate Clustering process");
						emailError.SendEmail();
						// break;
					}
				}
			} while (LoopCounter());
		}
	}

	boolean doCluster(String mainDatabase) {
		objDataAccess.SetDatabase(mainDatabase);
		clusterObj = new ClusterClass(mainDatabase);
		if (clusterObj.setClusterReadyDatabases()
				&& clusterObj.processClusterReadyDatabases()) {
			return true;
		} else {
			return false;
		}

		// For testing the CDHitClusterOutputParser class

		/*
		 * objDataAccess.SetDatabase("db_pass_gautam_stg");
		 * CDHitClusterOutputParser parser = newCDHitClusterOutputParser(
		 * "C:\\cluster files\\Original_clustered_uni2.clstr"); if
		 * (parser.parseClusterFile() || true) { System.exit(0); }
		 */

		// For testing the ClusterClass
		/*
		 * if (clusterObj.initProjectClusterVariables("db_pass_gautam_tgt",
		 * "")||true) { System.exit(0); return true; }
		 */

	}

	public static boolean LoopCounter() {

		// Delay for two minutes then return
		try {
			Thread.currentThread().sleep(120000);
		} catch (InterruptedException e) {
			errTime = new Date();
			currErrorMessage = "LoopCounter Interruped";
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ ": LoopCounter Interrupted\n" + e.getMessage());
			emailError.SendEmail();
			System.exit(1);
			return false;
		} catch (Exception e) {
			errTime = new Date();
			currErrorMessage = "LoopCounter Exception Error";
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ ": LoopCounter Exception Error\n" + e.getMessage());
			emailError.SendEmail();
			System.exit(1);
			return false;
		}
		return true;
	}
}

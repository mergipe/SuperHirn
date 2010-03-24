/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.text.*; //SIK - to allow use of dateFormat
import java.util.Date;
import java.io.*;
import java.util.zip.*;
import java.util.Iterator;
import org.apache.commons.beanutils.DynaBean;
import org.apache.commons.beanutils.RowSetDynaClass;

//import com.amazon.s3.*;

/**
 * 
 * @author User
 */
public class clsProcessItem {

	// Declare private variables for properties
	private String sProcessName;
	private String sStageDatabase;
	private String sTargetDatabase;
	private String zippedFileName; // Patrick to hold the zipped File Name and
									// location
	private String sStatus;
	public static clsSearchList objSearchList;
	public static clsBuildXMLFile objBuildXMLFile;
	public static clsWriteXMLFile objXMLWriter;
	public static clsRunXtandem objRunXtandem;
	public static clsRunSuperHirn objRunSuperHirn;
	private clsAmazon oAmazonS3;
	private Date errTime;
	// private EmailDevTeam emailError = new EmailDevTeam();
	private clsSearchItem[] objSearchItemList;
	private clsSearchItem objSearchItem;
	// Variables to store the paths of the Source search files (mzXML, mfg, etc)
	// and the
	// output XML (*_out.xml) files
	public static String sXMLFilePath = "";
	public static String sSearchFilesPath_Win = "";
	public static String sSearchFilesPath_Lin = "";
	public static String sXMLOutputFilePath_Win = "";
	public static String sStorage_Location = "";

	public clsProcessItem() {

		// Initialize the private variables
		sProcessName = "";
		sStageDatabase = "";
		sTargetDatabase = "";
		sStatus = "";

	}

	public void SetProcessName(String sProcess) {
		sProcessName = sProcess;
	}

	public void SetStageDatabase(String sStageDB) {
		sStageDatabase = sStageDB;
	}

	public void SetTargetDatabase(String sTargetDB) {
		sTargetDatabase = sTargetDB;
	}

	public void SetStatus(String sStat) {
		sStatus = sStat;
	}

	public String ActiveTargetDatabase() {
		return sTargetDatabase;
	}

	public void LoadSaxonSearchList() {

		// Declare variables
		RowSetDynaClass rsRecordset = null;
		int iRecordCount = 0;

		// Connect to the MySQL database
		try {
			Main.objDataAccess.SetDatabase(sTargetDatabase);
			// conMySQL = objDataAccess.connectToMySQL();

			// Check if a connection was made
			// if (conMySQL == null) {
			// Main.numThreadErrors++;
			// return;
			// }

			// Get the data from the database table
			// Statement sSQLSelect = conMySQL.createStatement();
			rsRecordset = Main.objDataAccess
					.getRecordSet("SELECT * FROM to_search_details WHERE processing_status = 'READY FOR XML TRANSFORM'");

			// Get the number of rows to process
			// rsRecordset.last();
			objSearchItemList = new clsSearchItem[rsRecordset.getRows().size()];
			// rsRecordset.first();

			// Check if array has elements
			if (objSearchItemList.length == 0) {
				// System.out.println("No Search List to process...");
				// Main.numThreadErrors++;
				// conMySQL.close();
				return;
			}

			// System.out.println("Loading Search List...");

			// Loop through the recordset and store the values in the Process
			// List
			Iterator iter = rsRecordset.getRows().iterator();

			while (iter.hasNext()) {

				DynaBean dbDataRow = (DynaBean) iter.next();

				// Instantiate a Search Item object
				objSearchItem = new clsSearchItem();

				// Store data from recordset
				objSearchItem.SetRecordKey((Integer) dbDataRow
						.get("to_search_details_key"));
				objSearchItem.SetMSFileKey((Integer) dbDataRow
						.get("to_ms_file_key"));
				objSearchItem.SetEngineType((String) dbDataRow
						.get("search_engine_type"));
				// objSearchItem.SetParameters((String)
				// dbDataRow.get("search_parmeters"));
				objSearchItem.SetProcessStatus((String) dbDataRow
						.get("processing_status"));
				objSearchItem.SetOutputFilename((String) dbDataRow
						.get("search_output_file_name"));
				objSearchItem.SetOutputLocation((String) dbDataRow
						.get("search_ouput_location"));
				objSearchItem.SetStageDatabase(sStageDatabase);
				objSearchItem.SetTargetDatabase(sTargetDatabase);

				// Add to collection
				objSearchItemList[iRecordCount] = objSearchItem;
				iRecordCount++;

			}

			// Close the recordset
			rsRecordset = null;

			// Close connection
			// conMySQL.close();

			// Loop through the Search List Items and process the xml files
			for (int iSearchItemCount = 0; iSearchItemCount <= objSearchItemList.length - 1; iSearchItemCount++) {
				objSearchItem = objSearchItemList[iSearchItemCount];
				objSearchItem.TransformXMLFile();
			}
		} catch (Exception eX) {
			errTime = new Date();
			Main.currErrorMessage = eX.toString();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ " Error occured in class 'clsProcessList': ");
			eX.printStackTrace();
			Main.numThreadErrors++;
			Main.emailError.SendEmail();
			System.exit(1);
		}
	}

	public void LoadSearchList() {
		// Connection conMySQL = null;
		int iToSearchDetailsKey;
		int i_UPDATE_Result;
		int iSearchListCount = 0;
		int iXTandemSearchResult;
		// Process pDelTempFile = null;
		String sInputXMLFileContent = "";
		// SIK allow evaluation of date for update of to_ms_file's start search
		// and end search columns
		DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		java.util.Date searchDate = new java.util.Date();

		try {

			Main.objDataAccess.SetDatabase(sTargetDatabase);
			// conMySQL = objDataAccess.connectToMySQL();

			// Check if a connection was made
			// if (conMySQL == null) {
			// return;
			// }

			// Get the storage location to determine whether to use Amazon S3 or
			// NAS.
			sStorage_Location = Main.objDataAccess
					.GetOutputPath("storage_location");

			// Get the Path of the search file from the tm_variables table
			sSearchFilesPath_Lin = Main.objDataAccess.GetOutputPath(
					"ftp_dir_search_results_lf").toLowerCase();

			// Get the Path of the output xtandem file from the tm_variables
			// table
			sXMLOutputFilePath_Win = Main.objDataAccess
					.GetOutputPath("search_output_location");

			// Get the search list
			objSearchList = new clsSearchList();
			String arSearchList[][] = objSearchList.GetSearchList();

			errTime = new Date();
			System.out.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ " About to start searching");
			// if (arSearchList[0].toString().isEmpty()) {
			if (arSearchList == null) {
				errTime = new Date();
				System.out.println(DateFormat.getDateTimeInstance(
						DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
						.toString()
						+ " There is nothing available for searching.");

				// Silas-Connection process termination/
				// conMySQL.close();
				// conMySQL = null;
				return;
			}

			// Process the search list
			do {
				// Silas

				// Check if a connection was made
				// if (conMySQL == null || conMySQL.isClosed()) {
				Main.objDataAccess.SetDatabase(sTargetDatabase);
				// conMySQL = objDataAccess.connectToMySQL();
				// }

				errTime = new Date();
				System.out.println(DateFormat.getDateTimeInstance(
						DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
						.toString()
						+ " Preparing to Search: " + (iSearchListCount + 1));

				// Instantiate XML File Builder object
				objBuildXMLFile = new clsBuildXMLFile();

				// Generate the contents of the input_dmn.xml file to be used
				// for the X!Tandem search
				sInputXMLFileContent = objBuildXMLFile.Input_XML_Contents(
						Integer.parseInt(arSearchList[1][iSearchListCount]
								.toString()), Integer
								.parseInt(arSearchList[0][iSearchListCount]
										.toString()));

				// Run the search only if the input_dmn.xml file was created
				if (sInputXMLFileContent.length() > 0) {

					// Instantiate XML File Writer
					// objXMLWriter = new clsWriteXMLFile(sSearchFilesPath_Lin +
					// "/input_dmn_" + arSearchList[0][iSearchListCount] +
					// ".xml",
					// objBuildXMLFile.Input_XML_Contents(objDataAccess,Integer.parseInt(arSearchList[1][iSearchListCount].toString()),Integer.parseInt(arSearchList[0][iSearchListCount].toString())));
					objXMLWriter = new clsWriteXMLFile(sSearchFilesPath_Lin
							+ "/input_dmn_" + arSearchList[0][iSearchListCount]
							+ ".xml", sInputXMLFileContent);

					// Execute the class
					if (objXMLWriter.WriteFile(sStorage_Location)) {

						// Display success message
						errTime = new Date();
						System.out
								.println(DateFormat.getDateTimeInstance(
										DateFormat.MEDIUM, DateFormat.MEDIUM)
										.format(errTime).toString()
										+ " The XML file has been successfully created.\n\nPreparing to run X!Tandem search...\n\n");

						// UPDATE record in to_ms_file table with
						// transmission_status of "SEARCHING"
						// SIK - Update start_search_datetime
						searchDate = new java.util.Date();
						i_UPDATE_Result = Main.objDataAccess
								.WriteRecord("UPDATE to_ms_file SET transmission_status = 'SEARCHING',start_search_datetime = '"
										+ dateFormat.format(searchDate)
										+ "' WHERE to_ms_file_key = "
										+ arSearchList[0][iSearchListCount]
										+ " AND tmx_key = "
										+ arSearchList[1][iSearchListCount]);

						// Check that the execution was ok
						if (i_UPDATE_Result == 0) {
							errTime = new Date();
							System.out
									.println(DateFormat.getDateTimeInstance(
											DateFormat.MEDIUM,
											DateFormat.MEDIUM).format(errTime)
											.toString()
											+ " Unable to set transmission status in to_ms_file table to 'SEARCHING'. Process will exit.");
						} else {

							// Display progress message
							errTime = new Date();
							System.out.println(DateFormat.getDateTimeInstance(
									DateFormat.MEDIUM, DateFormat.MEDIUM)
									.format(errTime).toString()
									+ " Executing X!Tandem search...\n\n");

							// Execute X!Tandem from command prompt using the
							// generated XML file as parameter
							objRunXtandem = new clsRunXtandem();
							if (sStorage_Location.contains("AS3")) {
								iXTandemSearchResult = objRunXtandem
										.clsRunXtandem("./"
												+ sSearchFilesPath_Lin
												+ "/input_dmn_"
												+ arSearchList[0][iSearchListCount]
												+ ".xml");
							} else {
								iXTandemSearchResult = objRunXtandem
										.clsRunXtandem(sSearchFilesPath_Lin
												+ "/input_dmn_"
												+ arSearchList[0][iSearchListCount]
												+ ".xml");
							}

							// UPDATE record in to_ms_file table with
							// transmission_status of "SEARCHED SUCCESSFULLY"
							// SIK - Update end_search_datetime also to record
							// time search completed
							// - Update this field even if search is not
							// successful - it will show when the AMI instance
							// was freed
							if (iXTandemSearchResult == 0) {
								// Store the output xml file on Amazon S3 if
								// destination is AS3

								if (sStorage_Location.contains("AS3")) {

									// Amazon S3 storage
									oAmazonS3 = new clsAmazon(); // Instantiate
																	// new
																	// custom
																	// Amazon S3
																	// object
									oAmazonS3.ConnectToAmazon(); // Connect to
																	// Amazon S3
									oAmazonS3.SetBucket(objBuildXMLFile
											.CurrentXMLOutputFile());

									// Patrick Compress the file which will add
									// a .gz to the filename
									zippedFileName = objBuildXMLFile
											.CurrentXMLOutputFile();
									if (compressFile(
											objBuildXMLFile
													.SearchOutputLocation()
													.toLowerCase()
													+ objBuildXMLFile
															.CurrentXMLOutputFile(),
											objBuildXMLFile
													.SearchOutputLocation()
													.toLowerCase()
													+ objBuildXMLFile
															.CurrentXMLOutputFile()
													+ ".gz", false) == true) {
										zippedFileName = objBuildXMLFile
												.CurrentXMLOutputFile()
												+ ".gz";
									}
									// Upload file to Bucket. If it fails, set
									// the processing status fields accordingly
									if (oAmazonS3.UploadFile(objBuildXMLFile
											.SearchOutputLocation()
											.toLowerCase()
											+ zippedFileName.toLowerCase())) {
										searchDate = new java.util.Date();
										i_UPDATE_Result = Main.objDataAccess
												.WriteRecord("UPDATE to_ms_file SET transmission_status = 'SEARCHED SUCCESSFULLY',end_search_datetime = '"
														+ dateFormat
																.format(searchDate)
														+ "' WHERE to_ms_file_key = "
														+ arSearchList[0][iSearchListCount]
														+ " AND tmx_key = "
														+ arSearchList[1][iSearchListCount]);
										String deletedFile = oAmazonS3
												.delTempFile();
										if (deletedFile.compareTo("\n") != 0) {
											errTime = new Date();
											System.out.println(DateFormat
													.getDateTimeInstance(
															DateFormat.MEDIUM,
															DateFormat.MEDIUM)
													.format(errTime).toString()
													+ deletedFile);
										}
									} else {
										searchDate = new java.util.Date();
										i_UPDATE_Result = Main.objDataAccess
												.WriteRecord("UPDATE to_ms_file SET transmission_status = 'ERROR SEARCHING',end_search_datetime = '"
														+ dateFormat
																.format(searchDate)
														+ "' WHERE to_ms_file_key = "
														+ arSearchList[0][iSearchListCount]
														+ " AND tmx_key = "
														+ arSearchList[1][iSearchListCount]);
										iXTandemSearchResult = 1;
									}

									// Kill the Amazon object
									oAmazonS3 = null;
								} else {
									searchDate = new java.util.Date();
									i_UPDATE_Result = Main.objDataAccess
											.WriteRecord("UPDATE to_ms_file SET transmission_status = 'SEARCHED SUCCESSFULLY',end_search_datetime = '"
													+ dateFormat
															.format(searchDate)
													+ "' WHERE to_ms_file_key = "
													+ arSearchList[0][iSearchListCount]
													+ " AND tmx_key = "
													+ arSearchList[1][iSearchListCount]);
								}
							} else {
								searchDate = new java.util.Date();
								i_UPDATE_Result = Main.objDataAccess
										.WriteRecord("UPDATE to_ms_file SET transmission_status = 'ERROR SEARCHING',end_search_datetime = '"
												+ dateFormat.format(searchDate)
												+ "' WHERE to_ms_file_key = "
												+ arSearchList[0][iSearchListCount]
												+ " AND tmx_key = "
												+ arSearchList[1][iSearchListCount]);
							}

							if (i_UPDATE_Result == 0) {
								errTime = new Date();
								System.out
										.println(DateFormat
												.getDateTimeInstance(
														DateFormat.MEDIUM,
														DateFormat.MEDIUM)
												.format(errTime).toString()
												+ " Unable to reset the transmission status in to_ms_file table.\n\nProcess did not complete successfully.");
							} else {

								if (iXTandemSearchResult == 0) {
									// INSERT record in to_search_details table
									if (objBuildXMLFile.CurrentSearchFile()
											.toLowerCase().endsWith(".mgf")) {
										iToSearchDetailsKey = Main.objDataAccess
												.WriteRecord("INSERT INTO to_search_details(to_ms_file_key,search_engine_type,processing_status,search_output_file_name,search_ouput_location) VALUES("
														+ arSearchList[0][iSearchListCount]
														+ ",'XTANDEM','READY FOR XML TRANSFORM','"
														+ objBuildXMLFile
																.CurrentSearchFile()
																.substring(
																		0,
																		objBuildXMLFile
																				.CurrentSearchFile()
																				.length() - 4)
														+ "_out.xml.gz','"
														+ sXMLOutputFilePath_Win
														+ "')");
									} else {
										iToSearchDetailsKey = Main.objDataAccess
												.WriteRecord("INSERT INTO to_search_details(to_ms_file_key,search_engine_type,processing_status,search_output_file_name,search_ouput_location) VALUES("
														+ arSearchList[0][iSearchListCount]
														+ ",'XTANDEM','READY FOR XML TRANSFORM','"
														+ objBuildXMLFile
																.CurrentSearchFile()
																.substring(
																		0,
																		objBuildXMLFile
																				.CurrentSearchFile()
																				.length() - 6)
														+ "_out.xml.gz','"
														+ sXMLOutputFilePath_Win
														+ "')");
									}
									// System.out.println("XML Output file saved to: "
									// + sXMLOutputFilePath_Win);

									if (iToSearchDetailsKey == 0) {
										errTime = new Date();
										System.out
												.println(DateFormat
														.getDateTimeInstance(
																DateFormat.MEDIUM,
																DateFormat.MEDIUM)
														.format(errTime)
														.toString()
														+ " Unable to set transmission status in to_ms_file table to 'SEARCHED SUCCESSFULLY'.\n\nProcess did not complete successfully.");
									} else {
										// Display progress message
										errTime = new Date();
										System.out
												.println(DateFormat
														.getDateTimeInstance(
																DateFormat.MEDIUM,
																DateFormat.MEDIUM)
														.format(errTime)
														.toString()
														+ " X!Tandem search complete.\n");

										// Delete the files from the temporary
										// location if we used a file from
										// Amazon S3
										/*
										 * if
										 * (sStorage_Location.contains("AS3")) {
										 * // Delete the input_dmn_?.xml file
										 * pDelTempFile =
										 * Runtime.getRuntime().exec("rm " +
										 * sSearchFilesPath_Lin + "/input_dmn_"
										 * + arSearchList[0][iSearchListCount] +
										 * ".xml", null);
										 * 
										 * // If we succeeded, delete the
										 * searched file
										 * if(pDelTempFile.waitFor() == 0) { //
										 * Delete the mgf/mzxml file
										 * pDelTempFile =
										 * Runtime.getRuntime().exec("rm " +
										 * sSearchFilesPath_Lin +
										 * objBuildXMLFile.CurrentSearchFile(),
										 * null);
										 * 
										 * if(pDelTempFile.waitFor() > 0) {
										 * System
										 * .out.println("Unable to delete file "
										 * + sSearchFilesPath_Lin +
										 * objBuildXMLFile.CurrentSearchFile() +
										 * " from the temporary folder."); } }
										 * else {System.out.println(
										 * "Unable to delete file "+
										 * sSearchFilesPath_Lin + "/input_dmn_"
										 * + arSearchList[0][iSearchListCount] +
										 * ".xml from the temporary folder."); }
										 * }
										 */

									}
								}

							}

						}
					}
				}
				// Delete the mgf/mzxml file
				File TempFile = new File(sSearchFilesPath_Lin
						+ objBuildXMLFile.CurrentSearchFile());
				if (TempFile.delete()) {
					errTime = new Date();
					System.out.println(DateFormat.getDateTimeInstance(
							DateFormat.MEDIUM, DateFormat.MEDIUM).format(
							errTime).toString()
							+ " Deleted File: "
							+ sSearchFilesPath_Lin
							+ objBuildXMLFile.CurrentSearchFile()
							+ " from the temporary folder.");
				} else {
					errTime = new Date();
					System.out.println(DateFormat.getDateTimeInstance(
							DateFormat.MEDIUM, DateFormat.MEDIUM).format(
							errTime).toString()
							+ " Unable to delete file "
							+ sSearchFilesPath_Lin
							+ objBuildXMLFile.CurrentSearchFile()
							+ " from the temporary folder.");
				}
				// Increment search list counter
				iSearchListCount += 1;
			} while (iSearchListCount < arSearchList[0].length);
			// Close the connection
			// conMySQL.close();
			// conMySQL = null;

		} catch (Exception eX) {
			errTime = new Date();
			Main.currErrorMessage = eX.toString();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ " Error occured in class 'clsProcessItem': ");
			eX.printStackTrace();
			Main.emailError.SendEmail();
			System.exit(1);
		}
		/*
		 * // Declare variables Connection conMySQL = null; int iRecordCount =
		 * 0;
		 * 
		 * // Connect to the MySQL database try {
		 * objDataAccess.SetDatabase(sTargetDatabase); conMySQL =
		 * objDataAccess.connectToMySQL();
		 * 
		 * // Check if a connection was made if (conMySQL == null) { return; }
		 * 
		 * // Get the data from the database table Statement sSQLSelect =
		 * conMySQL.createStatement(); ResultSet rsRecordset =
		 * sSQLSelect.executeQuery(
		 * "SELECT * FROM to_search_details WHERE processing_status = 'READY FOR XML TRANSFORM'"
		 * );
		 * 
		 * // Get the number of rows to process rsRecordset.last();
		 * objSearchList = new clsSearchItem[rsRecordset.getRow()];
		 * rsRecordset.first();
		 * 
		 * // Check if the array has elements if(objSearchList.length == 0) {
		 * System.out.println("There is no list to process..."); return; }
		 * 
		 * System.out.println("Loading Search List...");
		 * 
		 * // Loop through the recordset and store the values in the Process
		 * List do {
		 * 
		 * // Instantiate a Search Item object objSearchItem = new
		 * clsSearchItem();
		 * 
		 * // Store data from recordset
		 * objSearchItem.SetRecordKey(rsRecordset.getInt
		 * ("to_search_details_key"));
		 * objSearchItem.SetMSFileKey(rsRecordset.getInt("to_ms_file_key"));
		 * objSearchItem
		 * .SetEngineType(rsRecordset.getString("search_engine_type"));
		 * objSearchItem
		 * .SetParameters(rsRecordset.getString("search_parmeters"));
		 * objSearchItem
		 * .SetProcessStatus(rsRecordset.getString("processing_status"));
		 * objSearchItem
		 * .SetOutputFilename(rsRecordset.getString("search_output_file_name"));
		 * objSearchItem
		 * .SetOutputLocation(rsRecordset.getString("search_ouput_location"));
		 * objSearchItem.SetStageDatabase(sStageDatabase);
		 * objSearchItem.SetTargetDatabase(sTargetDatabase);
		 * 
		 * // Add to collection objSearchList[iRecordCount] = objSearchItem;
		 * iRecordCount++;
		 * 
		 * } while(rsRecordset.next());
		 * 
		 * // Close the recordset rsRecordset.close();
		 * 
		 * // Loop through the Search List Items and process the xml files
		 * for(int iSearchItemCount = 0; iSearchItemCount <=
		 * objSearchList.length - 1; iSearchItemCount++){ objSearchItem =
		 * objSearchList[iSearchItemCount];
		 * objSearchItem.TransformXMLFile(objDataAccess); }
		 * 
		 * 
		 * } catch (SQLException eSQL) {
		 * System.err.println("\nFailed to load Process list from database." );
		 * eSQL.printStackTrace(); return; } catch (Exception eX) {
		 * System.err.println("Error occured in class 'clsProcessList': ");
		 * eX.printStackTrace(); return; }
		 */
	}

	
	/*
	 * Method for running APP_Analyzer in SuperHirn mode.
	 * 
	 * @author Lukas N. Mueller
	 */
	public void LoadSuperHirnList() {

		// Connection conMySQL = null;
		int iToSearchDetailsKey;
		int i_UPDATE_Result;
		int iSearchListCount = 0;
		int iSuperHirnResult;

		// Process pDelTempFile = null;
		String sInputXMLFileContent = "";
		// SIK allow evaluation of date for update of to_ms_file's start search
		// and end search columns
		DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		java.util.Date searchDate = new java.util.Date();

		try {

			Main.objDataAccess.SetDatabase(sTargetDatabase);
			
			// Get the search list
			objSearchList = new clsSearchList();
			String arSearchList[][] = objSearchList.GetSuperHirnList();

			errTime = new Date();
			if (arSearchList == null) {
				errTime = new Date();
				System.out.println(DateFormat.getDateTimeInstance(
						DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
						.toString()
						+ " There is nothing to process for SuperHirn.");
				return;
			}
			else
			{

				System.out.println(DateFormat.getDateTimeInstance(
						DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
						.toString()
						+ " SuperHirn processing about to start for " + arSearchList[0].length + " items.");
			}

			// Process the search list
			do {

				Main.objDataAccess.SetDatabase(sTargetDatabase);
				errTime = new Date();

				String pmzXML = arSearchList[0][iSearchListCount];
				String tandemXML = arSearchList[1][iSearchListCount];

				System.out.println(DateFormat.getDateTimeInstance(
						DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
						.toString()
						+ " Preparing SuperHirn processing of mzXML /tandemXML file: " + pmzXML + " / " + tandemXML);

				// Display success message
				errTime = new Date();

				// UPDATE record in to_ms_file table with
				// transmission_status of "SEARCHING"
				// SIK - Update start_search_datetime
				searchDate = new java.util.Date();
				
				/*
				i_UPDATE_Result = Main.objDataAccess
						.WriteRecord("UPDATE to_ms_file SET SuperhirnStatus = 'DONE_FE', start_search_datetime = '"
								+ dateFormat.format(searchDate)
								+ "' WHERE to_ms_file_key = "
								+ pmzXML );
								*/
				i_UPDATE_Result = 1;
				
				
				// Check that the execution was ok
				if (i_UPDATE_Result == 0) {
					errTime = new Date();
					System.out
							.println(DateFormat.getDateTimeInstance(
									DateFormat.MEDIUM, DateFormat.MEDIUM)
									.format(errTime).toString()
									+ " Unable to set SuperHirnStatus in to_ms_file table to 'DONE_FE'. Process will exit.");
				} else {

					// Display progress message
					errTime = new Date();
					System.out.println(DateFormat.getDateTimeInstance(
							DateFormat.MEDIUM, DateFormat.MEDIUM).format(
							errTime).toString()
							+ " Executing SuperHirn processing...\n\n");

					
					/**********************
					 * Execute the SuperHirn command for each mzXML file
					 */
					objRunSuperHirn = new clsRunSuperHirn();
					if (sStorage_Location.contains("AS3")) {
						iSuperHirnResult = objRunSuperHirn
								.labelfreeQuantificationOnMzXMLFile(pmzXML, tandemXML);
					} else {
						iSuperHirnResult = objRunSuperHirn
						.labelfreeQuantificationOnMzXMLFile(pmzXML, tandemXML);
					}

						
					// UPDATE record in to_ms_file table with
					// transmission_status of "SEARCHED SUCCESSFULLY"
					// SIK - Update end_search_datetime also to record time
					// search completed
					// - Update this field even if search is not successful
					// - it will show when the AMI instance was freed
					if (iSuperHirnResult == 0) {

						System.out.println( "SuperHirn processing finished correc, updating DONE_FE on " + pmzXML);	
						i_UPDATE_Result = Main.objDataAccess
								.WriteRecord("UPDATE to_ms_file SET SuperhirnStatus = 'DONE_FE', start_search_datetime = '"
										+ dateFormat.format(searchDate)
										+ "' WHERE to_ms_file_key = " + pmzXML);

					} else {

						System.out.println( "ERROR: SuperHirn processing finished incorrect, updating ERROR_FE on " + pmzXML);	

						i_UPDATE_Result = Main.objDataAccess
								.WriteRecord("UPDATE to_ms_file SET SuperhirnStatus = 'ERROR_FE', start_search_datetime = '"
										+ dateFormat.format(searchDate)
										+ "' WHERE to_ms_file_key = " + pmzXML);
				
					}

				}

				// Increment search list counter
				iSearchListCount += 1;
			} while (iSearchListCount < arSearchList[0].length);

		} catch (Exception eX) {
			errTime = new Date();
			Main.currErrorMessage = eX.toString();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ " Error occured in class 'clsProcessItem': ");
			eX.printStackTrace();
			Main.emailError.SendEmail();
			System.exit(1);
		}
	}

	/**
	 * This procedure compresses a file to the .gz or .gzip zip format and may
	 * retain or delete the original uncompressed file.
	 * 
	 * @param inputFile
	 *            The complete path to the file we wish to decompress
	 * @param outputFile
	 *            The complete path to where we wish to store the uncompressed
	 *            file
	 * @param keepCompressed
	 *            Whether to retain the original compressed file
	 * @return A boolean value indicating success or failure of the
	 *         decompression
	 */
	public boolean compressFile(String inputFile, String outputFile,
			boolean keepOriginal) {
		GZIPOutputStream outStream;
		FileInputStream inStream;
		byte[] buf;
		int len;
		boolean isZipped = false;
		File delSource;

		try {
			isZipped = false;
			// The file to write out.
			outStream = new GZIPOutputStream(new FileOutputStream(outputFile));

			// The original file that we shall compress;
			inStream = new FileInputStream(inputFile);

			buf = new byte[1024];
			while ((len = inStream.read(buf)) > 0) {
				outStream.write(buf, 0, len);
			}
			inStream.close();

			outStream.finish();
			outStream.close();

			// If keepCompressed is false delete the source file otherwise keep
			// them both
			if (keepOriginal == false) {
				delSource = new File(inputFile);
				delSource.delete();
			}

			isZipped = true;
		} catch (SecurityException se) {
			// Will occur if the user does not have delete rights
			errTime = new Date();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ " Error occured in class 'clsProcessItem': ");
			se.printStackTrace();
			Main.emailError.SendEmail();
			System.exit(1);
			return true; // Because we have unzipped ok only deletion failed
		} catch (Exception ex) {
			errTime = new Date();
			Main.currErrorMessage = "Error occured in class 'clsProcessItem' while uncompressing file: "
					+ inputFile + " " + ex.toString();
			System.err.println(DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime)
					.toString()
					+ " Error occured in class 'clsProcessItem': ");
			ex.printStackTrace();
			Main.emailError.SendEmail();
			System.exit(1);
			return false;
		}
		return isZipped;
	}
}

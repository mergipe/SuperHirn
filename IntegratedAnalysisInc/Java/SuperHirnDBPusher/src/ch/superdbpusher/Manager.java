package ch.superdbpusher;

import java.io.File;
import java.io.FileFilter;
import java.util.Date;
import java.util.Iterator;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.Vector;
import java.util.Map.Entry;

import org.apache.commons.beanutils.DynaBean;
import org.apache.commons.beanutils.RowSetDynaClass;

import ch.superdbpusher.database.clsDataAccess;
import ch.superdbpusher.database.DatabaseAccessException;
import ch.superdbpusher.proteomics.LCMS;
import ch.superdbpusher.proteomics.MS1Feature;
import ch.superdbpusher.proteomics.MS2Info;
import ch.superdbpusher.xmlParsing.LCMSReader;
import ch.superdbpusher.xmlParsing.XMLFilter;

/**
 * Manager organizes the process of XML parsing and sending data to the RDS. It
 * starts from either a folder or xml file. For each file, an instance of the
 * class LCMS is generated which is then parsed to the RDS class LCMSRDSFeeder
 * which imports then the data of the LCMS class in the to database. Database
 * table names are defined by the static attributes LCMSTableName,
 * featureTableName, ms2IdTableName and alignedFeatureTableName.
 * <br><br>
 * The Manager class decides after the parsing of a XML file if the file is from
 * a single mzXML file, i.e. it is a results from a feature extraction SuperHirn
 * process or it is an aligned map, i.e it contains features that are aligned
 * across multiple mzXML file (MasterMap). This is done by checking if the LCMS
 * class contains child entries, ie the aligned mzXML files. The data import is
 * then adjusted accordingly.
 * <br><br>
 * If a feature extracted LCMS file is processed then MS1 Features are imported
 * into the table 'featureTableName' and an entry for the mzXML file is created
 * in LCMSTableName. If the MS1 features contaiens a MS/MS if, this peptide id
 * is stored in the table ms2IdTableName.
 * <br><br>
 * On the other hand, if the LCMS run is an aligned one, a new entry is created
 * in LCMSTableName where the attribute 'LCMS_Type' is set to ALIGNMENT. For
 * each aligned MS1 Feature, a new entry is created in the table
 * alignedFeatureTableName and the unique id of this entry 'idMS1_FEATURE'
 * serves as a primary key to which aligned MS1 Features are associated from the
 * table featureTabeName using the forgein key fkIDAlignedFeature
 * 
 * @author Lukas N. Mueller (Lukas.Mueller@imsb.biol.ethz.ch)
 */

public class Manager {

	private File target;
	public static String currErrorMessage;

	/**
	 * Table containing mzXML files processed by feature extraction or an aligned superirn result file
	 */
	static private String LCMSTableName = "LC_MS_RUNS_ALIGNMENT_TEST";

	/**
	 * Table storing extracted MS1 features from the feature extraction process of SuperHirn
	 */
	static private String featureTableName = "MS1_FEATURES_ALIGNMENT_TEST";

	/**
	 * Table storing MS/MS assignments of MS1 features
	 */
	static private String ms2IdTableName = "MS2_ASSIGNMENTS_ALIGNMENT_TEST";
	
	/**
	 * Table storing aligned features from SuperHirn Alignment processes. MS1 Features are associated to these.
	 */
	static private String alignedFeatureTableName = "ALIGNED_FEATURES";

	/**
	 * These are the database access parameters:
	 */
	static private String host = "poc-instance.cwlyzzqu4y8r.us-east-1.rds.amazonaws.com";
	static private String user = "gsaxena";
	static private String passwd = "polk0912888";
	static private String database = "db_pass_gautam_tgt";

	// static private String host =
	// "ins-pass-uniprot.cwlyzzqu4y8r.us-east-1.rds.amazonaws.com";
	// static private String user = "lmueller";
	// static private String passwd = "relleuml";
	// static private String database = "db_lukas_dev";

	static private int port = 3306;

	private clsDataAccess rdsAccess;

	/**
	 * Constructor with argument path to a file or directory
	 * 
	 * @param iPath
	 *            file or directory of XML files to parse
	 */
	public Manager(String iPath) {
		this.target = new File(iPath);
		this.rdsAccess = null;
	}

	/**
	 * Starts the main process of parsing and imporing data - establishes a
	 * connection to the database - parses XML file - deconnects from database
	 */
	public void start() {

		// connect to the database:
		this.rdsAccess = new clsDataAccess(host, // host
				user, // user
				passwd, // password
				port, // port
				database // database
		);

		// start XML parsing of either file or directory:
		this.startXMLParsing();

		if (this.rdsAccess != null) {
			this.rdsAccess.close();
		}
	}

	private void startXMLParsing() {
		Vector<File> parsingFiles = this.defineXMLFiles();
		Iterator<File> I = parsingFiles.iterator();
		while (I.hasNext()) {
			this.processXMLFile((File) I.next());
		}
	}

	private Vector<File> defineXMLFiles() {

		Vector<File> out = new Vector<File>();
		if (this.target.exists()) {

			FileFilter filter = new XMLFilter();

			if (this.target.isDirectory()) {

				File[] files = this.target.listFiles(filter);
				int i = 0;
				while (i < files.length) {
					out.add(files[i]);
					i++;
				}

			} else {
				if (filter.accept(this.target)) {
					out.add(this.target);
				}
			}

			Iterator<File> I = out.iterator();
			while (I.hasNext()) {
				File f = (File) I.next();
				System.out.println("\t- File '" + f.getName() + "' detected");
			}

		} else {
			System.out.println("Input file/directory '" + this.target.getPath()
					+ "' does not exist or can not be accessed.");
		}

		return out;

	}

	/**
	 * Processes a file by parsing it and then sending the extracted LCMS
	 * instance to the database module.
	 * 
	 * @param iFile
	 *            file to the XML superhirn file
	 */
	private void processXMLFile(File iFile) {
		try {
			
			LCMSReader reader = new LCMSReader(iFile);
			reader.startLCMSXMLparsing();
			LCMS run = reader.lcms();
			this.processLCMS(run);
			// send it to the database module:
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/**
	 * Processes a LCMS run by extracting the data and input it into the
	 * database.
	 * 
	 * @param LCMS
	 *            iRun instance of the LCMS run
	 */
	private void processLCMS(LCMS iRun) {

		System.out.println("\nImporting LC-MS '" + iRun.name()
				+ "' to database:");
		System.out.println("\t-" + iRun.nbFeatures() + " MS1 Features / "
				+ iRun.identifiedFeatures() + " MS2 identified");
		System.out.println("\t-" + iRun.getChildLCMS().size()
				+ " Aligned LC-MS runs");

		int lcMS_ID = this.write(iRun);
		this.writeFeatures(lcMS_ID, iRun);

		System.out.println("\nImporting finished.");

	}
	
	private String checkRunType(LCMS iRun)
	{
		if (iRun.getChildLCMS().isEmpty()) {
			return "FEATURE_EXTRACTION";
		} else {
			return "ALIGNMENT";
		}
	}

	private int write(LCMS iRun) {
		try {

			String type = this.checkRunType(iRun);
			if (type == "FEATURE_EXTRACTION") {
				// check first if data has been previously stored with this name
				// then remove it:
				this.checkIfExistsInDatabase(iRun);
			} else {
				// check first if data has been previously stored with this name
				// then remove it:
				this.cleanUpPreviousAlignment(iRun);
			}
			System.out.println( "Start importing...");

			// now insert new and continues
			String query = "INSERT INTO " + Manager.LCMSTableName
					+ " (mzXML_Name, LCMS_TYPE) VALUES(";
			query = query + "'" + iRun.name() + "',";
			query = query + "'" + type + "'";
			query = query + ")";

			this.rdsAccess.performSQLStatement(query);

			String select = "SELECT idLC_MS_RUN FROM " + Manager.LCMSTableName
					+ " WHERE mzXML_Name=";
			select = select + "'" + iRun.name() + "'";
			// get the inserted LC_MS id back:
			RowSetDynaClass rec = this.rdsAccess.getRecordSet(select);
			if (rec == null) {
				return -1;
			} else if (rec.getRows().size() != 1) {
				System.out.print("Multiple LC-MS entries with same name "
						+ iRun.name());
			}

			DynaBean dbDataRow = (DynaBean) rec.getRows().get(0);
			int LCMS_table_id = (Integer) dbDataRow.get("idLC_MS_RUN");
			return LCMS_table_id;

		} catch (DatabaseAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return -1;
	}
	
	/**
	 * Method that takes an ordered map where the aligned mzXML files are
	 * ordered according to an internal ID of SUperHirn. This ordering is also reflected in the way MS1 featuers
	 * are ordered in an aligned feature. The method translates this map to a map where the superhirn internal id is mapped to
	 * the primary keys of the LC_MS_RUN table in the database. 
	 * 
	 * @param SortedMap
	 *            <Integer, String>
	 * @return SortedMap<Integer, Integer>
	 */
	private SortedMap<Integer, Integer> mapMzXMLNamesToTablePrimaryKeys(  SortedMap<Integer, String> lcMsRuns)
	{
		SortedMap<Integer, Integer> out = new TreeMap<Integer, Integer>();
		
		Iterator I = lcMsRuns.entrySet().iterator();
		while( I.hasNext())
		{
			Entry e  = (Entry) I.next();
			Integer lcMSID = (Integer) e.getKey();
			String name = (String)e.getValue();
			
			String select = "SELECT idLC_MS_RUN FROM " + Manager.LCMSTableName +
			" WHERE mzXML_Name='" + name + "'";

			RowSetDynaClass rec;
			try {
				rec = this.rdsAccess.getRecordSet(select);
				if ( (rec == null) || (rec.getRows().size() != 1) ) {
				}
				
				DynaBean dbDataRow = (DynaBean) rec.getRows().get(0);
				int key = (Integer) dbDataRow.get("idLC_MS_RUN");
				out.put(lcMSID, key);
				
			} catch (DatabaseAccessException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		}

		return out;
	}
	

	private void writeFeatures(int LC_MS_ID, LCMS iRun) {
		
		Iterator<MS1Feature> fIt = iRun.featureIterator();
		String mode = this.checkRunType(iRun);
		
		/*
		 * check the processing mode 
		 */
		if (mode == "ALIGNMENT") {

			/*
			 * for efficient processing, create an index array to quickly retrieved
			 * the primary keys of a LC_MS_RUNS table entry by LC_MS id of an aligned feature:
			 */
			SortedMap<Integer, Integer> lcMSKeys = 
				this.mapMzXMLNamesToTablePrimaryKeys( iRun.getChildLCMS());			
			
			while (fIt.hasNext()) {
			
				MS1Feature feature = (MS1Feature) fIt.next();
				
				/*
				 * insert the alignment feature and get is primary key:
				 */
				int alignedID = this.writeAlignedFeature(LC_MS_ID, feature);

				// set the aligned IDs if the aligned features to the primary
				// key of the aligned feature::
				//String mzXMLName = iRun.getChildLCMS().get(iFeature.lcMSID());
				//int tableID = this.getMS1FeatureID(mzXMLName, iFeature);
				//this.setAlignedMS1FeatureID(tableID, alignedID);

				int lcMSID = lcMSKeys.get( feature.lcMSID());
				this.setAlignmentOfMS1Feature(feature, lcMSID, alignedID);
				
				Iterator I = feature.alignedFeatures().entrySet().iterator();
				while (I.hasNext()) {
					Entry<Integer, MS1Feature> e = (Entry<Integer, MS1Feature>) I
							.next();
					MS1Feature featureAligned  = e.getValue();
					lcMSID = lcMSKeys.get( featureAligned.lcMSID());
					this.setAlignmentOfMS1Feature(featureAligned, lcMSID, alignedID);

					//mzXMLName = iRun.getChildLCMS().get(iFeature.lcMSID());
					//tableID = this.getMS1FeatureID(mzXMLName, iFeature);
					//this.setAlignedMS1FeatureID(tableID, alignedID);
				}

			}
		} else {
			while (fIt.hasNext()) {
				this.writeSingleFeature(LC_MS_ID, (MS1Feature) fIt.next(), -1);
			}
		}
	}

	private int writeAlignedFeature(int LC_MS_ID, MS1Feature iFeature) {
		int alignedID = -1;
		try {

			String query = "INSERT INTO " + Manager.alignedFeatureTableName
					+ " (mz, retentionTime, z, fkLC_MS_ID, featureID) VALUES (";
			query = query + iFeature.mz() + "," + iFeature.tr() + ","
					+ iFeature.z() + "," + LC_MS_ID + ", " + iFeature.id()
					+ ")";
			this.rdsAccess.performSQLStatement(query);

			// get the assigned primary id key back:
			String select = "SELECT idMS1_FEATURE FROM "
					+ Manager.alignedFeatureTableName + " WHERE featureID="
					+ iFeature.id();
			select = select + " and fkLC_MS_ID=" + LC_MS_ID;

			// get the inserted LC_MS id back:
			RowSetDynaClass rec = this.rdsAccess.getRecordSet(select);
			if (rec == null) {
				return alignedID;
			} else if (rec.getRows().size() != 1) {
				throw new Exception("No unique ID for aligned feature!");
			}

			DynaBean dbDataRow = (DynaBean) rec.getRows().get(0);
			alignedID = (Integer) dbDataRow.get("idMS1_FEATURE");

		} catch (DatabaseAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return alignedID;

	}

	private void setAlignmentOfMS1Feature(MS1Feature iFeature, Integer lcMSID,
			Integer alignmentID) {
		try {

			// get the assigned primary id key back:
			String update = "UPDATE " + Manager.featureTableName
					+ " SET fkIDAlignedFeature=" + alignmentID +
					" WHERE " 
					+ "z=" + iFeature.z()
					+ " AND " + "ionCurrent=" + iFeature.peakArea() 
					+ " AND " + "apexScan=" + iFeature.apexScan() 
					+ " AND " + "startScan=" + iFeature.startScan() 
					+ " AND " + "endScan=" + iFeature.endScan()
					+ " AND " + "fkLC_MS_ID=" + lcMSID;
			
			// get the inserted LC_MS id back:
			this.rdsAccess.performSQLStatement(update);

		} catch (DatabaseAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	

	
	private int getMS1FeatureID(String mzXMLName, MS1Feature iFeature) {
		int id = -1;
		try {

			// get the assigned primary id key back:
			String select = "SELECT idMS1_FEATURE FROM "
					+ Manager.featureTableName + " LEFT JOIN (" + Manager.LCMSTableName + ") ON "
					+ "(" + Manager.featureTableName + ".fkLC_MS_ID=" + Manager.LCMSTableName + ".idLC_MS_RUN) WHERE "
					+ Manager.LCMSTableName + ".mzXML_Name='" + mzXMLName + "' AND "
					+ Manager.featureTableName + ".z=" + iFeature.z() + " AND "
					+ Manager.featureTableName + ".ionCurrent=" + iFeature.peakArea() + " AND "
					+ Manager.featureTableName + ".apexScan=" + iFeature.apexScan() + " AND "
					+ Manager.featureTableName + ".startScan=" + iFeature.startScan() + " AND "					
					+ Manager.featureTableName + ".endScan=" + iFeature.endScan();

			// get the inserted LC_MS id back:
			RowSetDynaClass rec = this.rdsAccess.getRecordSet(select);
			if (rec == null) {
				throw new Exception("No ID for MS1 feature found!");
			} 
			DynaBean dbDataRow = (DynaBean) rec.getRows().get(0);
			id = (Integer) dbDataRow.get("idMS1_FEATURE");

		} catch (DatabaseAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return id;

	}
	
	

	private void setAlignedMS1FeatureID(int MS1Id, int alignedID) {
		int id = -1;
		try {
			String update = "UPDATE " + Manager.featureTableName +
			" SET fkIDAlignedFeature=" + alignedID + " WHERE "
			+ Manager.featureTableName + ".idMS1_FEATURE=" + MS1Id;
			this.rdsAccess.performSQLStatement(update);

		} catch (DatabaseAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	private void writeSingleFeature(int LC_MS_ID, MS1Feature iFeature,
			int iAlignedID) {
		try {

			String query = "INSERT INTO "
					+ Manager.featureTableName
					+ " (mz, retentionTime, z, ionCurrent, "
					+ "startScan, endScan, startRetentionTime, endRetentionTime, apexScan, "
					+ "fkLC_MS_ID, signalToNoise, featureID, fkIDAlignedFeature) VALUES (";
			query = query + iFeature.mz() + "," + iFeature.tr() + ","
					+ iFeature.z() + "," + iFeature.peakArea();
			query = query + ",";
			query = query + iFeature.startScan() + "," + iFeature.endScan()
					+ "," + iFeature.retentionTimeStart() + ","
					+ iFeature.retentionTimeEnd() + "," + iFeature.apexScan();
			query = query + ",";
			query = query + LC_MS_ID + "," + iFeature.signalToNoise() + ","
					+ iFeature.id();
			query = query + ", -1)";
			this.rdsAccess.performSQLStatement(query);
			this.writeMS2(LC_MS_ID, iFeature);

		} catch (DatabaseAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	private void writeMS2(int LC_MS_ID, MS1Feature iFeature) {
		try {
			SortedMap<Double, Vector<MS2Info>> peps = iFeature.peptideIds();
			if (peps != null) {

				String statement = "INSERT INTO " + Manager.ms2IdTableName;
				statement = statement
						+ " ( fkLC_MS_ID, fkMS1_ID, scan_number) VALUES(";

				Iterator I = peps.entrySet().iterator();
				while (I.hasNext()) {
					Entry E = (Entry<Double, Vector<MS2Info>>) I.next();
					Vector<MS2Info> vec = (Vector<MS2Info>) E.getValue();
					Iterator vI = vec.iterator();
					while (vI.hasNext()) {
						MS2Info inf = (MS2Info) vI.next();
						String sql = statement;
						sql = sql + LC_MS_ID + ",";
						sql = sql + iFeature.id() + ",";
						sql = sql + inf.get_scan_start() + ")";
						this.rdsAccess.performSQLStatement(sql);
					}
				}
			}

		} catch (DatabaseAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	private void checkIfExistsInDatabase(LCMS iRun) {
		try {

			String select = "SELECT idLC_MS_RUN FROM " + Manager.LCMSTableName
					+ " WHERE mzXML_Name=";
			select = select + "'" + iRun.name() + "'";

			// get the inserted LC_MS id back:

			RowSetDynaClass rec = this.rdsAccess.getRecordSet(select);
			if (rec == null) {
				return;
			} else if (rec.getRows().size() == 0) {
				System.out.println("No FEATURE EXTRATION data associated with LCMS name "
						+ iRun.name());
			} else {

				System.out.println("Remove data associated with LCMS name "
						+ iRun.name());
			}

			for (int i = 0; i < rec.getRows().size(); i++) {

				DynaBean dbDataRow = (DynaBean) rec.getRows().get(0);
				int LCMS_key = (Integer) dbDataRow.get("idLC_MS_RUN");

				// delete in MS2_ASSIGNMENTS where foreign key column is
				// fkLC_MS_ID:
				String delete = "DELETE FROM " + Manager.ms2IdTableName
						+ " WHERE fkLC_MS_ID=" + LCMS_key;

				this.rdsAccess.performSQLStatement(delete);

				// delete in MS1_FEATURES where foreign key column is
				// fkLC_MS_ID:
				delete = "DELETE FROM " + Manager.featureTableName
						+ " WHERE fkLC_MS_ID=" + LCMS_key;
				this.rdsAccess.performSQLStatement(delete);

				// delete in LC_MS_RUNS where foreign key column is
				// idLC_MS_RUN:
				delete = "DELETE FROM " + Manager.LCMSTableName
						+ " WHERE idLC_MS_RUN=" + LCMS_key;
				this.rdsAccess.performSQLStatement(delete);
			}
		} catch (DatabaseAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	private void cleanUpPreviousAlignment(LCMS iRun) {
		try {

			// check if an ID with this name exists:
			String select = "SELECT idLC_MS_RUN FROM " + Manager.LCMSTableName
					+ " WHERE mzXML_Name=";
			select = select + "'" + iRun.name() + "'";

			RowSetDynaClass rec = this.rdsAccess.getRecordSet(select);
			if (rec == null) {
				return;
			} else if (rec.getRows().size() == 0) {
				System.out.println("No ALIGNMENT data associated with LCMS name: "
						+ iRun.name());
				return;
			} else {

				System.out.println("Remove data associated with ALIGNMENT named: "
						+ iRun.name());
			}

			for (int i = 0; i < rec.getRows().size(); i++) {

				DynaBean dbDataRow = (DynaBean) rec.getRows().get(0);
				int LCMS_key = (Integer) dbDataRow.get("idLC_MS_RUN");

				// reset alignment ids from MS1 features:
				select = "SELECT idMS1_FEATURE FROM "
						+ Manager.alignedFeatureTableName
						+ " WHERE fkLC_MS_ID=" + LCMS_key;
				RowSetDynaClass recAligned = this.rdsAccess
						.getRecordSet(select);
				if (recAligned == null) {
					return;
				} else {
					for (int j = 0; j < recAligned.getRows().size(); j++) {

						DynaBean row = (DynaBean) recAligned.getRows().get(j);
						int id = (Integer) row.get("idMS1_FEATURE");

						// reset now all alinedIDs of MS1 Feautures to -1:
						// delete all aligned features:
						String delete = "UPDATE "
								+ Manager.featureTableName
								+ " SET fkIDAlignedFeature=-1"
								+ " WHERE fkIDAlignedFeature=" + id;
						this.rdsAccess.performSQLStatement(delete);
					}

					// delete all aligned features:
					String delete = "DELETE FROM "
							+ Manager.alignedFeatureTableName
							+ " WHERE fkLC_MS_ID=" + LCMS_key;

					this.rdsAccess.performSQLStatement(delete);

				}

				// delete the ALIGNMENT run finally:
				String delete = "DELETE FROM " + Manager.LCMSTableName
						+ " WHERE idLC_MS_RUN=" + LCMS_key;
				this.rdsAccess.performSQLStatement(delete);
			}
		} catch (DatabaseAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}

package ch.superdbpusher;

import java.io.File;
import java.io.FileFilter;
import java.util.Iterator;
import java.util.SortedMap;
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
 * Manager organizes the process of XML parsing and sending data to the RDS. It starts from either a folder or xml file. 
 * For each file, an instance of the class LCMS is generated which is then passed to the RDS class LCMSRDSFeeder which imports the 
 * data of the LCMS class in the to database.
 * 
 * @author Lukas N. Mueller (Lukas.Mueller@imsb.biol.ethz.ch)
 */

public class Manager {

	private File target;
	public static String currErrorMessage;

	static private String LCMSTableName = "LC_MS_RUNS";
	static private String featureTableName = "MS1_FEATURES";
	static private String ms2IdTableName = "MS2_ASSIGNMENTS";

	/** 
	 * These are the database access parameters:
	 */
	static private String host = "ins-pass-uniprot.cwlyzzqu4y8r.us-east-1.rds.amazonaws.com";
	static private String user = "lmueller";
	static private String passwd = "relleuml";
	static private String database = "db_lukas_dev";
	static private int port = 3306;

	private clsDataAccess rdsAccess;

	/**
	 * Constructor with argument path to a file or directory
	 * 
	 * @param iPath file or directory of XML files to parse
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

	public void startXMLParsing() {
		Vector<File> parsingFiles = this.defineXMLFiles();
		Iterator<File> I = parsingFiles.iterator();
		while (I.hasNext()) {
			this.processXMLFile((File) I.next());
		}
	}

	public Vector<File> defineXMLFiles() {

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
	 * @param iFile file to the XML superhirn file
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
	 * @param LCMS iRun instance of the LCMS run
	 */
	private void processLCMS(LCMS iRun) {

		System.out.println("\nImporting LC-MS '" + iRun.name()
				+ "' to database:");
		System.out.println("\t-" + iRun.nbFeatures() + " MS1 Features / "
				+ iRun.identifiedFeatures() + " MS2 identified");

		int lcMS_ID = this.write(iRun);

		Iterator<MS1Feature> fIt = iRun.featureIterator();
		while (fIt.hasNext()) {
			MS1Feature fe = (MS1Feature) fIt.next();
			this.write(lcMS_ID, fe);
		}

		System.out.println("\nImporting finished.");

	}

	private int write(LCMS iRun) {
		try {

			String query = "INSERT INTO " + Manager.LCMSTableName
					+ " (mzXML_Name) VALUES(";
			query = query + "'" + iRun.name() + "'";
			query = query + ")";

			this.rdsAccess.performSQLStatement(query);
			
			String select = "SELECT idLC_MS_RUN FROM " + Manager.LCMSTableName
			+ " WHERE mzXML_Name=";
			query = query + "'" + iRun.name() + "'";
			query = query + ")";
			// get the inserted LC_MS id back:
			RowSetDynaClass rec = this.rdsAccess.getRecordSet(select);
			if( rec.getRows().size()!= 1 )
			{	
				System.out.print("Multiple LC-MS entries with same name " + iRun.name());
			}
			DynaBean dbDataRow = (DynaBean) rec.getRows().get(0);
			int LCMS_table_id = Integer.parseInt(			
					(String) dbDataRow.get("idLC_MS_RUN") );
			return LCMS_table_id;
			
			
		} catch (DatabaseAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return -1;
	}

	private void write(int LC_MS_ID, MS1Feature iFeature) {
		try {

			String query = "INSERT INTO "
					+ Manager.featureTableName
					+ " (mz, retentionTime, z, ionCurrent, "
					+ "startScan, endScan, startRetentionTime, endRetentionTime, apexScan, "
					+ "fkLC_MS_ID, signalToNoise, featureID) VALUES (";
			query = query + iFeature.mz() + "," + iFeature.tr() + ","
					+ iFeature.z() + "," + iFeature.peakArea();
			query = query + ",";
			query = query + iFeature.startScan() + "," + iFeature.endScan()
					+ "," + iFeature.retentionTimeStart() + ","
					+ iFeature.retentionTimeEnd() + "," + iFeature.apexScan();
			query = query + ",";
			query = query + "'" + LC_MS_ID + "'" + ","
					+ iFeature.signalToNoise() + "," + iFeature.id();
			query = query + ")";
			this.rdsAccess.performSQLStatement(query);

			this.writeMS2(LC_MS_ID,iFeature);

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
						+ " ( fk_LC_MS_ID, fkMS1FeatureID, scan_number) VALUES(";

				Iterator I = peps.entrySet().iterator();
				while (I.hasNext()) {
					Entry E = (Entry<Double, Vector<MS2Info>>) I.next();
					Vector<MS2Info> vec = (Vector<MS2Info>) E.getValue();
					Iterator vI = vec.iterator();
					while (vI.hasNext()) {
						MS2Info inf = (MS2Info) vI.next();
						String sql = statement;
						sql = sql + "'" + LC_MS_ID + "'" + ",";
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

}

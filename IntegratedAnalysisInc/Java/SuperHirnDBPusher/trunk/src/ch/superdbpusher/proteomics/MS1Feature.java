package ch.superdbpusher.proteomics;

//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS feature:
// 
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

import java.io.Serializable;
import java.util.Comparator;
import java.util.Iterator;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.Vector;
import java.util.Map.Entry;

//NOTE by Vagisha: made Serializable for applet. 
public class MS1Feature implements Cloneable, Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	/*
	 * Class comparator for MS1 feature based on m/z comparisons
	 */
	static private class MyComparator implements Comparator<MS1Feature> {

		public int compare(MS1Feature A, MS1Feature B) {

			if (A.mz() == B.mz() ) {
				return 0;
			} else if (A.mz() > B.mz()) {
				return 1;
			}
			return -1;
		}

	}

	/*
	 * Class comparator for MS1 feature based on ppm m/z comparisons
	 */
	static public boolean compareMassAtPPMLevel(double mzA, double mzB) {

		// take the average mass:
		double avMass = (mzA + mzB) / 2.0;

		// define the parts per million:
		double ppmValue = avMass / 1000000.00;
		double ppmDeltaTol = ppmValue * MS1Feature.PPMTolerance;

		double deltaMass = Math.abs(mzA - mzB);
		if (deltaMass > ppmDeltaTol) {
			return false;
		}

		return true;
	}

	/*
	 * compare retention time values:
	 */
	static public boolean compareRetentionTimes(double rtA, double rtB) {

		double deltaTR = Math.abs(rtA - rtB);
		if (deltaTR > TrTolerance) {
			return false;
		}
		return true;
	}

	/*
	 * get the PPM value of a double at a cetrain Mas
	 */
	static public double getMassPPMLevel(double iMz, double value) {
		// define the parts per million:
		double ppmUnit = iMz / 1000000;
		return value / ppmUnit;

	}

	/*
	 * get PPM value for a certain mass
	 */
	static public double getMassAtPPMLevel(double mz) {

		// define the parts per million:
		double ppmValue = mz / 1000000.00;
		return ppmValue * MS1Feature.PPMTolerance;

	}

	static final double _MONO_H = 1.00794;

	static final double _MONO_O = 15.99943;

	public static double MS1MzTolerance;

	public static double PPMTolerance;

	public static double TrTolerance;

	private double mz;

	private String mzXMLName;

	private int id;

	private int lcmsID;

	private int apexScan;

	private int endScan;

	private int startScan;

	private int z;

	private double peakArea;

	private double apexIntensity;

	private double tr;

	private double trStart;

	private double trEnd;

	private double signaToNoise;

	private double BackgroundNoiseLevel;

	private SortedMap<Integer, MS1Feature> matches;

	private SortedMap<Double, Vector<MS2Info>> ms2Ids;

	public MS1Feature(double iMz, double iRT, double iRTStart, double iRTEnd, int iApexScan, int iStartScan,
			int iEndScan, int iZ, double iArea, double iApexIntens, int iID) {
		this.mz = iMz;
		this.id = iID;
		this.apexScan = iApexScan;
		this.endScan = iEndScan;
		this.startScan = iStartScan;
		this.z = iZ;
		this.tr = iRT;
		this.trStart = iRTStart;
		this.trEnd = iRTEnd;
		this.peakArea = iArea;
		this.apexIntensity = iApexIntens;
		this.signaToNoise = 1000;

	}

	/*
	 * override the clone methode:
	 */
	@SuppressWarnings("unchecked")
	public MS1Feature clone() {
		try {
			MS1Feature MYcopy = (MS1Feature) super.clone();

			MYcopy.apexIntensity = this.apexIntensity;
			MYcopy.z = this.z;
			MYcopy.id = this.id;
			MYcopy.mz = this.mz;
			MYcopy.startScan = this.startScan;
			MYcopy.endScan = this.endScan;
			MYcopy.apexScan = this.apexScan;
			MYcopy.lcmsID = this.lcmsID;
			MYcopy.peakArea = this.peakArea;
			MYcopy.mzXMLName = this.mzXMLName;
			MYcopy.tr = this.tr;
			MYcopy.signaToNoise = this.signaToNoise;
			MYcopy.BackgroundNoiseLevel = this.BackgroundNoiseLevel;

			if (this.matches != null) {
				MYcopy.matches = new TreeMap<Integer, MS1Feature>();
				Iterator I = this.matches.entrySet().iterator();
				while (I.hasNext()) {
					Entry E = (Entry) I.next();
					MYcopy.matches.put((Integer) E.getKey(), ((MS1Feature) E
							.getValue()).clone());
				}
			} else {
				// NOTE by Vagisha: Instead of setting it to null we initialize
				// it
				// as an empty map, otherwise
				// it leads to NullPointerExceptions in some functions.
				this.matches = new TreeMap<Integer, MS1Feature>();
			}

			// store ms2 info:
			if (this.ms2Ids != null) {
				MYcopy.ms2Ids = new TreeMap<Double, Vector<MS2Info>>();
				Iterator I = this.ms2Ids.entrySet().iterator();
				while (I.hasNext()) {
					Entry E = (Entry) I.next();
					Vector<MS2Info> tmp = new Vector<MS2Info>();

					Iterator v = ((Vector<MS2Info>) E.getValue()).iterator();
					while (v.hasNext()) {
						MS2Info inf = (MS2Info) v.next();
						tmp.add(inf.clone());
					}

					MYcopy.ms2Ids.put((Double) E.getKey(), tmp);

				}
			} else {
				MYcopy.ms2Ids = null;
			}
			return MYcopy;

		} catch (CloneNotSupportedException e) {
			throw new Error(" Error in cloning fct of MS1 feature");
		}
	}

	// ////////////////////////////////////////////////
	// show the content of the spectra:
	public void info() {

		String SEP = "";
		// print MS-peak info:

		System.out.printf("\t#:%d,%s", this.id, SEP);
		System.out.printf("m/z:%.3f%s", this.mz, SEP);
		System.out.printf("[+%d],%s", this.z, SEP);
		System.out.printf("Area:%.2f%s", this.peakArea, SEP);
		System.out.printf(",Tr:%.2f[%.1f:%.1f][%d:%d:%d]%s",
				this.tr(), this.trStart, this.trEnd, this.startScan, this.apexScan, this.endScan, SEP);
		System.out.printf(",LC-MS Id:%d", this.lcMSID());
		System.out.printf("\n");

		MS2Info INFO = this.bestMSMS();
		if (INFO != null) {
			INFO.show_info();
		}
	}

	// ////////////////////////////////////////////////
	// add MS/MS info to the feature:
	@SuppressWarnings("unchecked")
	public void addMSMS(MS2Info IN) {

		IN.setRetentionTime(this.tr);

		// check if probability already inserted:
		if (this.ms2Ids == null) {
			this.ms2Ids = new TreeMap<Double, Vector<MS2Info>>();
		}

		Vector<MS2Info> F = this.ms2Ids.get(IN.get_PEP_PROB());

		if (F == null) {

			Vector<MS2Info> TMP = new Vector<MS2Info>();

			TMP.addElement(IN);

			this.ms2Ids.put(IN.get_PEP_PROB(), (Vector<MS2Info>) TMP.clone());

		}

		else {

			F.addElement(IN);

		}

		// update also the matched features:
		// to implement:

	}

	// ////////////////////////////////////////////////
	// functions to set/access machted features:
	public void addFeature(MS1Feature iFeature) {

		if (this.matches == null) {
			this.matches = new TreeMap<Integer, MS1Feature>();
		}

		if (iFeature.matches != null) {
			Iterator I = iFeature.matches.entrySet().iterator();
			while (I.hasNext()) {
				Entry E = (Entry) I.next();
				this.addFeature((MS1Feature) E.getValue());
			}
			iFeature.matches.clear();
		}

		matches.put(iFeature.id(), iFeature);
	}

	// ////////////////////////////////////////////////
	// get the best ms2 scan with highest peptide probability:
	public MS2Info bestMSMS() {

		MS2Info OUT = null;
		if (this.ms2Ids != null) {
			double R = this.ms2Ids.lastKey();
			Vector<MS2Info> T = this.ms2Ids.get(R);
			OUT = T.get(0);

			if (this.matches != null) {
				for (SortedMap.Entry<Integer, MS1Feature> P : this.matches
						.entrySet()) {
					MS2Info TMP = P.getValue().bestMSMS();
					if (TMP != null) {
						if (TMP.get_PEP_PROB() > OUT.get_PEP_PROB()) {
							OUT = TMP;
						}
					}
				}
			}
		}
		return OUT;
	}

	// ////////////////////////////////////////////////
	// get the profile over all matched features:
	public SortedMap<Integer, MS1Feature> alignedFeatures() {
		return this.matches;
	}
	
	
	// get all identifed peptides associated to this ms1 feature:
	public SortedMap<Double, Vector<MS2Info>> peptideIds() {
		return this.ms2Ids;
	}

	// /////////////////////////////////////////////////
	// get a feature by ID:
	public MS1Feature getFeatureByID(int ID) {

		if (this.id == ID) {
			return this;
		} else {
			if (this.matches != null) {
				return this.matches.get(ID);

			}
		}

		return null;
	}

	// /////////////////////////////
	// start here all the get / set
	// function to access the
	// variables of the class

	// access the parent mass of feature, calculated from the SQ
	public double mz() {
		return this.mz;
	}
	
	/// access the mzXML file name
	/// @return string, mzXML file name
	public String mzXMLName() {
		return this.mzXMLName;
	}
	
	/*
	 *  Set the mzXML file name
	 *  @param String
	 */
	public void setMzXMLName(String iName) {
		this.mzXMLName = iName;
	}

	public int z() {
		return this.z;
	}

	public double peakArea() {
		return this.peakArea;
	}

	public double apexIntensity() {
		return this.apexIntensity;
	}

	public double tr() {
		return this.tr;
	}

	/*
	 * set /get signal to noise value:
	 */
	public void setSignalToNoise(double in) {
		this.signaToNoise = in;
	}

	/*
	 * set / get background noise value:
	 */
	public void setBackgroundNoiseLevel(double in) {
		this.BackgroundNoiseLevel = in;
	}

	public double backgroundNoise() {
		return this.BackgroundNoiseLevel;
	}

	public double signalToNoise() {
		return this.signaToNoise;
	}

	public int id() {
		return this.id;
	}
	
	public int startScan() {
		return this.startScan;
	}
	public int endScan() {
		return this.endScan;
	}
	public int apexScan() {
		return this.apexScan;
	}
	public double retentionTimeStart() {
		return this.trStart;
	}
	public double retentionTimeEnd() {
		return this.trEnd;
	}
	public void setLCMSID(int iID) {
		this.lcmsID = iID;
	}

	public int lcMSID() {
		return this.lcmsID;
	}

	static public MyComparator getMS1FeatureComparator() {
		return new MyComparator();
	}

	/*
	 * defines if two features are equal(0) of not (1)
	 */
	static public Boolean FeatureEqualComparator(MS1Feature A, MS1Feature B) {
		MyComparator comp = getMS1FeatureComparator();
		if (comp.compare(A, B) == 0) {
			return true;
		} else {
			return false;
		}
	}

	/*
	 * defines if two features are equal(0) of not (1)
	 */
	static public int FeatureComparator(MS1Feature A, MS1Feature B) {
		MyComparator comp = getMS1FeatureComparator();
		return comp.compare(A, B);
	}

}


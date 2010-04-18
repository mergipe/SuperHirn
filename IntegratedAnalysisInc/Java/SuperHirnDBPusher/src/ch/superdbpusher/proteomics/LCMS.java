package ch.superdbpusher.proteomics;

import java.io.Serializable;
import java.util.Collections;
import java.util.Iterator;
import java.util.ListIterator;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.Vector;
import java.util.Map.Entry;

/**
 * The class LCMS represents a liquid chromatography coupled to mass spectrometry measurement containing detected
 * MS1 features and asssociated MS/MS peptide identifications. 
 * 
 * @author Lukas N. Mueller (Lukas.Mueller@imsb.biol.ethz.ch)
 * @see MS1feature, MS2identification
 * 
 */

public class LCMS implements Cloneable, Serializable {

	/***
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private String name;

	private Vector<MS1Feature> features;
	
	private SortedMap<Integer, String> childLCMS = new TreeMap<Integer, String>();

	// a unique specrum id to identify a spectrum:
	private int id;

	/**
	 * Constructor of the class LCMS
	 * 
	 * @param iName
	 *            name of the LCMS run
	 * 
	 * @param iID
	 *            identifier of the LCM run
	 */
	public LCMS(String iName, int iID) {
		this.name = iName;
		this.id = iID;
		this.features = new Vector<MS1Feature>();
	}

	public void addChildLCMS(String iName, int iID) {
		this.childLCMS.put(iID, iName);
	}
	
	public SortedMap<Integer, String> getChildLCMS() {
		return this.childLCMS;
	}
	
	/**
	 * Override of the clone methode to copy the name, id and features of this
	 * LCMS instance. throws CloneNotSupportedException
	 */
	public LCMS clone() {
		try {
			LCMS MYcopy = (LCMS) super.clone();

			MYcopy.name = this.name;
			MYcopy.id = this.id;

			MYcopy.features = new Vector<MS1Feature>();
			Iterator I = this.featureIterator();
			while (I.hasNext()) {
				MS1Feature f = (MS1Feature) I.next();
				MYcopy.features.add(f.clone());
			}
			return MYcopy;

		} catch (CloneNotSupportedException e) {
			throw new Error(" Error in cloning method of LCMS");
		}
	}

	/**
	 * Order MS1 features of the LCMS run by increasing m/z value.
	 */
	public void orderFeaturesByMz() {
		// order first all Features according to Molecular Mass
		Collections.sort(this.features, MS1Feature.getMS1FeatureComparator());
	}

	/**
	 * Get the number of MS1 features with an associated MS/MS identification
	 */
	public int identifiedFeatures() {
		int count = 0;
		Iterator<MS1Feature> P = this.featureIterator();
		while (P.hasNext()) {
			MS1Feature fe = (MS1Feature) P.next();
			if (fe.bestMSMS() != null) {
				count++;
			}
		}

		return count;
	}

	/**
	 * Print the information about this LCMS instance to the console.
	 */
	public void info() {
		String text;
		text = String.format(">LC/MS id=%d name=%s", this.id, this.name);
		System.out.println(text);
		text = String.format("#features: %d, #MS2 ids: %d", this.nbFeatures(),
				this.identifiedFeatures());
		System.out.println(text);
		if( !this.childLCMS.isEmpty())
		{
			Iterator I = this.childLCMS.entrySet().iterator();
			while( I.hasNext())
			{
				Entry<Integer, String> e = (Entry<Integer, String>) I.next();
				text = String.format("Child Run: %s (id=%d)", e.getValue(), e.getKey());
				System.out.println(text);
			}		
			
		}
		
		
	}

	/**
	 * find a fetaure by its feature id in the map
	 */
	public MS1Feature getFeatureByID(int id) {
		Iterator I = this.featureIterator();
		while (I.hasNext()) {
			MS1Feature fe = (MS1Feature) I.next();
			if (fe.id() == id) {
				return fe;
			}
		}

		return null;
	}

	/**
	 * Find a MS1 feature in this LCMS by the identifier
	 * 
	 * @param id
	 *            identifier of the searched MS1 feature
	 */
	public MS1Feature findFeature(int id) {
		Iterator P = this.featureIterator();
		while (P.hasNext()) {
			MS1Feature TMP = (MS1Feature) P.next();
			if (id == TMP.id()) {
				return TMP;
			}
		}
		return null;
	}

	/**
	 * Get an iterator of the feature list
	 * 
	 * @return iterator
	 */
	public Iterator featureIterator() {
		return features.iterator();
	}

	/**
	 * Get a list iterator of the feature list
	 * 
	 * @return ListIterator
	 */
	public ListIterator featureListIterator() {
		return features.listIterator();
	};

	/**
	 * Get a feature at a specific index
	 * 
	 * @param index
	 * @return feature
	 */

	public MS1Feature feature(int index) {
		return features.get(index);
	}

	/**
	 * Add a feature to the LCMS run
	 * 
	 * @param iFeature
	 *            feature to add
	 */
	public void addFeature(MS1Feature iFeature) {
		iFeature.setMzXMLName(this.name);
		features.add(iFeature);
	}

	/**
	 * Remove a feature at this index
	 * 
	 * @param index
	 */
	public void removefeature(int index) {
		features.removeElementAt(index);
	}

	/**
	 * Remove all feature in the LC-MS run
	 */
	public void removeAllFeatures() {
		features.clear();
	}

	/**
	 * Get the number of feature stored in the LC-MS run
	 * 
	 * @return int
	 */
	public int nbFeatures() {
		return features.size();
	}

	/**
	 * Get the LC-MS id
	 * 
	 * @return int
	 */
	public int id() {
		return id;
	}

	/**
	 * Get the LC-MS name
	 * 
	 * @return String
	 */
	public String name() {
		return name;
	}

}

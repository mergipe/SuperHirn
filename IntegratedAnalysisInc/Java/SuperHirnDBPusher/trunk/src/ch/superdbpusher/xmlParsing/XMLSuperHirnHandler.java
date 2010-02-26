///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS LC_MS_XML_reader:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

package ch.superdbpusher.xmlParsing;

import java.util.Iterator;


import org.dom4j.Element;
import org.dom4j.ElementPath;
import org.dom4j.ElementHandler;


import ch.superdbpusher.proteomics.LCMS;
import ch.superdbpusher.proteomics.MS1Feature;
import ch.superdbpusher.proteomics.MS2Info;


public class XMLSuperHirnHandler implements ElementHandler {

	private LCMS lcms = null;

	public XMLSuperHirnHandler() {
	}

	private void parseLCMSSummary(ElementPath ROOT) {

		// parse the LC-MS element:
		Element rootElement = (Element) ROOT.getCurrent();
		Iterator rootIterator = ((org.dom4j.Element) rootElement)
				.elementIterator();
		Element lcmsElement = (Element) rootIterator.next();

		// create the LCMS instance:
		String name = lcmsElement.attributeValue("name");
	//	int nbFeatures = Integer.parseInt( lcmsElement.attributeValue("number_of_features") );
		this.lcms = new LCMS(name, 0);

		// parse the LC-MS element:
		Iterator I = lcmsElement.elementIterator();

		Element childRuns = (Element) I.next();
		/*
		 * Element childRuns = (Element) I.next(); Iterator K =
		 * childRuns.elementIterator(); while (K.hasNext()) { Element childRun =
		 * (Element) K.next(); String id = } childRun.attributeValue("ID");
		 * this.THIS_LC_MS.add_raw_spec_name(Integer.parseInt(id),
		 * childRun.attributeValue("name")); }
		 */

		// parse the MS1 feature elements:
		this.parseMS1features((Element) I.next());

	}

	/*
	 * function to extrat all MS1 features:
	 */
	private void parseMS1features(Element ROOT) {

		Iterator F_IT = ROOT.elementIterator();
		while (F_IT.hasNext()) {
			Element tmp = (Element) F_IT.next();
			this.parseAndStoreMS1feature(tmp);
		}
	}

	/*
	 * extracts a ms1 feature and stores it in the master run:
	 */
	private void parseAndStoreMS1feature(Element IN) {

		// extract first the root feature:
		MS1Feature FEA = this.parseMS1Feature(IN);
		// add it to the current LC/MS
		this.storeMS1FeatureInLCMS(FEA);

	}

	/*
	 * parse a MS1 feature form XML:
	 */
	protected MS1Feature parseMS1Feature(Element element) {

		if (element != null) {

			double mz = Double.parseDouble(element.attributeValue("m_z"));
			int z = Integer.parseInt(element.attributeValue("charge_state"));
			//int lcmsID = Integer.parseInt(element.attributeValue("LC_MS_ID"));
			double Tr = Double.parseDouble(element.attributeValue("Tr"));
			double startTr = Double.parseDouble(element.attributeValue("Tr_Start"));
			double endTr = Double.parseDouble(element.attributeValue("Tr_End"));
			int id = Integer.parseInt(element.attributeValue("Feature_ID"));

			double snRatio = 10;
			if (element.attributeValue("snRatio") != null) {
				snRatio = Double.parseDouble(element.attributeValue("snRatio"));
			}
			double BGLevel = 100;
			if (element.attributeValue("BGnoise") != null) {
				BGLevel = Double.parseDouble(element.attributeValue("BGnoise"));
			}

			// LC information:
			MS1Feature FEA = null;
			Iterator featureIterator = element.elementIterator();
			Element LC_INFO = (Element) featureIterator.next();
			if (LC_INFO.getName().equals("LC_INFO")) {
				int SCAN_APEX = Integer
						.parseInt(LC_INFO.attributeValue("apex"));
				int SCAN_START = Integer.parseInt(LC_INFO
						.attributeValue("start"));
				int SCAN_END = Integer.parseInt(LC_INFO.attributeValue("end"));
				double PEAK_AREA = Double.parseDouble(LC_INFO
						.attributeValue("AREA"));

				double APEX_INTENSITY = -1;
				if (LC_INFO.attributeValue("apex_intensity") != null) {
					APEX_INTENSITY = Double.parseDouble(LC_INFO
							.attributeValue("apex_intensity"));
				}

				// here all the info is available, construct the feature:
				FEA = new MS1Feature(mz, Tr, startTr, endTr, SCAN_APEX, SCAN_START, SCAN_END,
						z, PEAK_AREA, APEX_INTENSITY, id);
				FEA.setSignalToNoise(snRatio);
				FEA.setBackgroundNoiseLevel(BGLevel);
			}

			if (FEA != null) {

				while (featureIterator.hasNext()) {

					Element featureChildElement = (Element) featureIterator
							.next();

					// add the MS2 info:
					if (featureChildElement.getName().equals("MS2_INFO")) {
						Iterator ms2 = featureChildElement.elementIterator();
						while (ms2.hasNext()) {
							extractMS2Info((Element) ms2.next(), FEA);
						}
					}

					// check for MS1 elution profile:
					else if (featureChildElement.getName().equals(
							"LC_ELUTION_PROFILE")) {
						// extractLCprofileFromXML(featureChildElement, FEA);
					}

					// check for matched features
					else if (featureChildElement.getName().equals(
							"MATCHED_FEATURES")) {

						Iterator I = featureChildElement.elementIterator();
						while (I.hasNext()) {
							MS1Feature tmp = this.parseMS1Feature((Element) I
									.next());
							FEA.addFeature(tmp);
						}
					}

				}
			}
			return FEA;
		} else {
			return null;
		}
	}


	/*
	 * extract a MS2 scan and add to the MS1 fetaure:
	 */
	private void extractMS2Info(Element MS2_INFO, MS1Feature IN) {

		int SCAN_MS2 = Integer.parseInt(MS2_INFO.attributeValue("MS2_scan"));

		String SQ = MS2_INFO.attributeValue("SQ");

		double PEP_PROB = Double.parseDouble(MS2_INFO
				.attributeValue("PeptideProbability"));
		MS2Info TMP = new MS2Info(SQ, PEP_PROB, IN.z(), SCAN_MS2);

		String AC = null;
		if (MS2_INFO.attributeValue("AC") != null) {
			AC = MS2_INFO.attributeValue("AC");
			TMP.set_AC(AC);
		}

		if (MS2_INFO.attributeValue("precursor_m_z") != null) {
			double MONO_MZ = Double.parseDouble(MS2_INFO
					.attributeValue("precursor_m_z"));
			TMP.set_precursor_mz(MONO_MZ);
		}

		if (MS2_INFO.attributeValue("retention_time") != null) {
			double tr = Double.parseDouble(MS2_INFO
					.attributeValue("retention_time"));
			TMP.setRetentionTime(tr);
		}

		if (MS2_INFO.attributeValue("charge_state") != null) {
			int CHRG = Integer
					.parseInt(MS2_INFO.attributeValue("charge_state"));
			TMP.set_chrg(CHRG);
		}

		if (MS2_INFO.attributeValue("interact_file") != null) {
			TMP.set_pepXML(MS2_INFO.attributeValue("interact_file"));
		} else {
			TMP.set_pepXML("NA");
		}

		if (MS2_INFO.attributeValue("ms2_type_tag") != null) {
			TMP.set_MS2_TYPE_TAG(MS2_INFO.attributeValue("ms2_type_tag"));
		} else {
			TMP.set_MS2_TYPE_TAG("NA");
		}

		if (MS2_INFO.attributeValue("prev_AA") != null) {
			TMP.set_PREV_AA(MS2_INFO.attributeValue("prev_AA"));
		} else {
			TMP.set_PREV_AA("NA");
		}

		Iterator ms2ChildIterator = MS2_INFO.elementIterator();
		while (ms2ChildIterator.hasNext()) {
			Element ms2Child = (Element) ms2ChildIterator.next();

			// check for alternative proteins ACs:
			if (ms2Child.getName().equals("ALTERNATIVE_PROTEIN")) {
				AC = ms2Child.attributeValue("AC");
				if (ms2Child.attributeValue("ProteinProb") != null) {
					TMP.set_AC(AC, Double.parseDouble(ms2Child
							.attributeValue("ProteinProb")));
				} else {
					TMP.set_AC(AC);
				}

			}

			// extract AA modifications
			if (ms2Child.getName().equals("MODIFICATIONS")
					&& ms2Child.elements().size() > 0) {

				Iterator K = ms2Child.elementIterator();
				while (K.hasNext()) {
					Element my_mod = (Element) K.next();
					int pos = Integer.parseInt(my_mod.attributeValue("pos"));
					double value = Double.parseDouble(my_mod
							.attributeValue("value"));
					TMP.add_AA_mod(pos, value);
				}
			}
		}

		IN.addMSMS(TMP);

	}

	/*
	 * store a feature in teh lc-ms map before check its validity:
	 */
	private void storeMS1FeatureInLCMS(MS1Feature IN) {
		this.lcms.addFeature(IN);
	}

	// function to get the parsed LC/MS run
	public LCMS lcms() {
		return this.lcms;
	}

	public void onEnd(ElementPath element) {
		this.parseLCMSSummary(element);
	}

	public void onStart(ElementPath element) {
	}

}

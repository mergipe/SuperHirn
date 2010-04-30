package ch.parameterSettings;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Iterator;
import java.util.Properties;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.Vector;
import java.util.Map.Entry;

/**
 * 
 * Example Property Utility Class. This class can be used by any other class to
 * retrieve property values. The properties are loaded from a file where the
 * values can be set by a client user to addapt them to his needs. If the file
 * does not exist, it is created and initialised with default properties
 * (MyProperties.createDefaultValues()). Thus, the first time a user launches
 * the application a propertyu file is created. From then on he may adapt the
 * values in that file which will be read the next time he launches the
 * application. The file can be deleted any time in order to restore default
 * behaviour. Alternatively, different properties files may be distributed and
 * copied into the location where the file is expected in order to activate
 * themn the next time the application is run. See
 * 
 * http://java.sun.com/docs/books/tutorial/essential/environment/properties.html
 * for more details
 * 
 * about properties.
 * 
 * 
 * 
 * @author Lukas Mueller
 * 
 * @version 1.0
 */

public class ParameterInterface {

	/*
	 * used as a separator for the tag of a parameter:
	 */
	static public String parameterTagSeparator = "_";

	/**
	 * Location where the property file will be stored. See Java API of System
	 * to get other standard locations, e.g. java.user.home or somthg.
	 */
	private static final String DIR = System.getProperty("user.dir");

	/**
	 * Name of the file where properties will be stored.
	 */
	private static final String PARAMETERFILE = ".Properties";

	/**
	 * The property object.
	 */
	private static Properties PROPERTIES;

	/*
	 * list of all default parameterClasses added:
	 */
	private static SortedMap<String, BasicParameterClass> allParameterClasses = new TreeMap<String, BasicParameterClass>();

	/**
	 * Application specific properties. These are the keys and have been put
	 * here so that any class retrieving a property can access the keys and if
	 * the key names change we only need to change them here.
	 */
	// this block will be called when the class is loaded first time.
	static {
		ParameterInterface.loadProperties();
	}

	/**
	 * This method loads the properties from a file if it exists or creates a
	 * new file with the default properties declared in getDefaultProperties().
	 */

	private static void loadProperties() {

		// create and load default properties

		ParameterInterface.PROPERTIES = new Properties();

		File file = new File(ParameterInterface.DIR,
				ParameterInterface.PARAMETERFILE);

		try {

			if (file.exists()) {

				// file exists, load properties from there
				FileInputStream in = new FileInputStream(file);
				ParameterInterface.PROPERTIES.loadFromXML(in);
				in.close();

				// convert now the loaded properties to BasciParameterClass:
				ParameterInterface
						.createUserSpecificParametersFromProperties();

			}

		} catch (final IOException e) {
			System.out.println(e.getMessage());
		}

	}

	/**
	 * Takes all input properties and transfers them to a basic parameter class
	 */
	 static private void createUserSpecificParametersFromProperties() {
	 
	 // go through all the properties and create a user parameter:
		Iterator I = ParameterInterface.PROPERTIES.entrySet().iterator();
		while (I.hasNext()) {
			Entry E = (Entry) I.next();
			ParameterInterface.addNewParameter(
					(String) E.getKey(), (String) E.getValue());
		}
	 }

	/**
	 * Saves all properties currently set in PROPERTIES object to a file as XML.
	 * 
	 * 
	 */
	/*
	 * private static void savePopertiesParametersToXML() {
	 * 
	 * try { FileOutputStream out; out = new FileOutputStream(new
	 * File(BasicParameterInterface.DIR,
	 * BasicParameterInterface.PARAMETERFILE));
	 * 
	 * BasicParameterInterface.PROPERTIES.storeToXML(out,
	 * "--- MyApplication Properties ---");
	 * 
	 * out.close(); } catch (FileNotFoundException e) { // TODO Auto-generated
	 * catch block e.printStackTrace(); } catch (IOException e) { // TODO
	 * Auto-generated catch block e.printStackTrace(); }
	 * 
	 * }
	 */

	/*
	 * public function to tell the interface to back up all user specific
	 * parameters into the XML file
	 */
	/*
	 * static public void storeAllUserSpecificParametersToXML() {
	 * 
	 * BasicParameterInterface.PROPERTIES.clear();
	 * 
	 * // check first if really all parameters are represented // in the
	 * Poperties class: if (BasicParameterInterface.allUserParameterClasses !=
	 * null) {
	 * 
	 * Iterator I = BasicParameterInterface.allUserParameterClasses
	 * .entrySet().iterator(); while (I.hasNext()) { Entry E = (Entry) I.next();
	 * BasicParameterInterface
	 * .storeUserSpecificParameterToXML((BasicParameterClass) E .getValue()); }
	 * }
	 * 
	 * // now all user specific parameters should be stored in the Properties //
	 * class // -> save them to XML:
	 * BasicParameterInterface.savePopertiesParametersToXML();
	 * 
	 * }
	 */
	

	/*
	 * saves a parameter in the property class to save it later on in the XML:
	 */
	/*
	private static void storeUserSpecificParameterToXML(
			BasicParameterClass userP) {

		BasicParameterInterface.PROPERTIES.setProperty(
				userP.getParameterName(), userP.getParameterValue());
	}
	*/

	/**
	 * Access to the user specific parameters:
	 */
	/*
	static public Iterator getUserParameterIterator() {
		return BasicParameterInterface.allUserParameterClasses.entrySet()
				.iterator();
	}
	*/

	/**
	 * Method to retrieve property value that is of type string. first check in
	 * the user specific set, if not there check the properties:
	 * 
	 * @param key
	 * @return
	 */
	private static String getBasicStringValue(String key) {
		BasicParameterClass thisP = null;

		// get parameter from the user specific parameters:
		/*
		 * thisP = BasicParameterInterface.getUserSpecificParameter(key); if
		 * (thisP != null) { return thisP.getParameterValue(); }
		 */

		// if not found check the properties (would be here if the
		// parameter was extracted from xml:
		String Value = ParameterInterface.PROPERTIES.getProperty(key);
		return Value;
	}

	/**
	 * Method to retrieve a user specific parameter: checks if this parameter is
	 * available
	 * 
	 * @param key
	 * @return
	 */
	/*
	public static BasicParameterClass getUserSpecificParameter(final String key) {

		if (BasicParameterInterface.allUserParameterClasses != null) {
			BasicParameterClass userP = BasicParameterInterface.allUserParameterClasses
					.get(key);
			return userP;
		} else {
			return null;
		}
	}
	*/

	
	/**
	 * Method to retrieve a user specific parameter: by its outside name
	 * (string, access via getparameterOutsiedName())
	 * 
	 * @param key
	 * @return
	 */
	/*
	public static BasicParameterClass getUserSpecificParameterByOutsideName(
			final String key) {

		if (BasicParameterInterface.allUserParameterClasses != null) {

			Iterator I = BasicParameterInterface.allUserParameterClasses
					.entrySet().iterator();
			while (I.hasNext()) {
				Entry E = (Entry) I.next();
				BasicParameterClass param = (BasicParameterClass) E.getValue();
				if (param.getParameterOutsideName().equals(key)) {
					return param;
				}
			}
		}

		return null;
	}
*/
	
	/**
	 * Method to retrieve a user specific parameter: by its description (string,
	 * access via getparameterOutsiedName())
	 * 
	 * @param key
	 * @return
	 */
/*
	public static BasicParameterClass getUserSpecificParameterByDescription(
			final String key) {

		if (BasicParameterInterface.allUserParameterClasses != null) {

			Iterator I = BasicParameterInterface.allUserParameterClasses
					.entrySet().iterator();
			while (I.hasNext()) {
				Entry E = (Entry) I.next();
				BasicParameterClass param = (BasicParameterClass) E.getValue();
				if (param.getParameterDescription().equals(key)) {
					return param;
				}
			}
		}

		return null;
	}
*/
	/*
	 * reset the user specific parameters to the default parameters
	 */
	/*
	static public void resetAllParametersToDefaultValues() {

		Iterator I = BasicParameterInterface.allDefaultParameterClasses
				.entrySet().iterator();
		while (I.hasNext()) {
			Entry E = (Entry) I.next();
			BasicParameterClass defaultP = (BasicParameterClass) E.getValue();
			BasicParameterClass userP = BasicParameterInterface
					.getUserSpecificParameter(defaultP.getParameterName());
			if (userP != null) {
				userP.setParameterValue(defaultP.getParameterValue());
			}
		}
	}
*/
	/**
	 * Method to retrieve property value extraed via teh class
	 * BasicParameterClass.
	 * 
	 * @param BasicParameterClass
	 * @return
	 */

	public static String getStringValue(final String key) {
		return ParameterInterface.getBasicStringValue(key);
	}

	/**
	 * Method to retrieve property value that is of type int.
	 * 
	 * @param key
	 * @return
	 * @throws Exception
	 */

	public static int getIntegerValue(final String key) 
	{
		int result = Integer.parseInt(ParameterInterface.getBasicStringValue(key));
		return result;
	}

	/**
	 * Method to retrieve property value that is of type Boolean.
	 * 
	 * @param key
	 * @return
	 * @throws Exception
	 */

	public static Boolean getBooleanValue(final String key) {

		return Boolean.parseBoolean(ParameterInterface
					.getBasicStringValue(key));
	}

	/**
	 * Method to retrieve property value that is of type double.
	 * 
	 * @param key
	 * @return
	 * @throws Exception
	 */

	public static double getDoubleValue(final String key) {
			double result = Double.parseDouble(ParameterInterface
					.getBasicStringValue(key));
			return result;
	}

	/*
	 * this static functions adds a new paremeter to the basic set: @param pN:
	 * parameter name pV parameter Value pT: parameter Type and parameter full
	 * name
	 */
	/*
	static public void addNewParameter(String pN, double pV, String pFN) {

		BasicParameterClass newParam = new BasicParameterClass(pN, String
				.format("%f", pV), "double", pFN);

		// add to the parameterClass list:
		BasicParameterInterface.addNewParameter(newParam);

	}
*/
	/**
	 * this static functions adds a new paremeter to the basic set: @param pN:
	 * parameter name pV parameter Value pT: parameter Type and parameter full
	 * name
	 */
	/*
	static public void addUserSpecifcParameter(Vector<String> pN, String pV,
			Boolean userView) {

		// create tag name from a vector of subtags:
		String tagName = createParameterTagFromSubTags(pN);
		BasicParameterClass newParam = new BasicParameterClass(tagName, pV,
				"String", tagName);

		// set the pbulic view:
		newParam.setParameterUserPublic(userView);

		// add to the parameterClass list:
		BasicParameterInterface.addUserSpecifcParameter(newParam);

	}
*/
	/**
	 * this static functions deletes a new paremeter to the basic set: @param
	 * pN:
	 */
	/*
	static public void deleteUserSpecifcParameter(Vector<String> pN) {

		// create tag name from a vector of subtags:
		String tagName = createParameterTagFromSubTags(pN);
		deleteUserSpecifcParameter(tagName);

	}
*/
	/**
	 * this static functions deletes a new paremeter to the basic set: @param
	 * pN:
	 */
	/*
	static protected void deleteUserSpecifcParameter(String pN) {

		Iterator I = BasicParameterInterface.getUserParameterIterator();
		while (I.hasNext()) {
			Entry E = (Entry) I.next();
			BasicParameterClass param = (BasicParameterClass) E.getValue();
			String tag = param.getParameterName();

			if (tag.equals(pN)) {
				BasicParameterInterface.allUserParameterClasses.remove(E
						.getKey());
				break;
			}

		}

	}
*/
	/**
	 * retrives for a wildcard all subtags
	 */
	static public SortedMap<String, String> getSubTagsByWildCard(String wCard) {
		SortedMap<String, String> all = new TreeMap<String, String>();

		// go throgh all the user parameters and find matching parameters:
		Iterator I = allParameterClasses.entrySet().iterator();
		while (I.hasNext()) {
			Entry E = (Entry) I.next();
			BasicParameterClass param = (BasicParameterClass) E.getValue();
			// check if wildcard matches:
			if (param.getParameterName().contains(wCard)) {
				all.put(param.getParameterName(), param.getParameterValue());
			}
		}

		return all;
	}


	/**
	 * Adds a new parameter to the basic set: 
	 * @param pN parameter name 
	 * @param pV parameter Value 
	 */
	static public void addNewParameter(String pN, String pV) {

		BasicParameterClass newParam = new BasicParameterClass(pN, pV);
		// add to the parameterClass list:
		ParameterInterface.allParameterClasses.put(newParam.getParameterName(),newParam);

	}

	/**
	 * this static functions adds a new paremeter to the basic set: @param pN:
	 * parameter name pV parameter Value pT: parameter Type
	 */
	/*
	static public void addNewParameter(String pN, Boolean pV, String pFN) {

		BasicParameterClass newParam;
		int convert = 0;
		if (pV == true) {
			convert = 1;
		}
		newParam = new BasicParameterClass(pN, String.format("%d", convert),
				"Boolean", pFN);

		// add to the parameterClass list:
		BasicParameterInterface.addNewParameter(newParam);

	}
	*/


	/*
	 * help function to create a main tags from a vector of subtags:
	 */
	static private String createParameterTagFromSubTags(Vector<String> subtags) {
		Iterator I = subtags.iterator();
		String sub = (String) I.next();

		String mainTag = String.format("%s", sub);

		while (I.hasNext()) {
			sub = (String) I.next();
			mainTag = String.format("%s%s%s", mainTag,
					ParameterInterface.parameterTagSeparator, sub);
		}

		return mainTag;
	}

}

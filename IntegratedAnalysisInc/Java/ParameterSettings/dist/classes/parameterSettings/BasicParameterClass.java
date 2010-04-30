package ch.parameterSettings;

import java.util.Iterator;
import java.util.Vector;

/**
 * Stores a parameter contains the value of the parameter in form of a string
 * and the type of the parameter (double, int, string), see parameterTypes
 */

public class BasicParameterClass {


	static private Vector<String> PARMETERTYPES;

	private String parameterType;

	private String parameterValue;

	private String parameterName;

	private String parameterFullName;

	private String parameterDescription;

	private Boolean userPublic;

	/**
	 * constructor
	 */
	public BasicParameterClass(String pN, String pV, String pT, String pFN) {

		// check parameter type:
		if (BasicParameterClass.checkParameterType(pT)) {
			this.parameterName = pN;
			this.parameterType = pT;
			this.parameterValue = pV;
			this.parameterFullName = pFN;
			this.parameterDescription = null;
			this.userPublic = true;
		} 

	}

	/**
	 * constructor
	 */
	public BasicParameterClass(String pN, String pV) {

		this.parameterName = pN;
		this.parameterValue = pV;
		this.parameterType = null;
		this.parameterFullName = null;
		this.parameterDescription = null;
		this.userPublic = true;
	}


	/**
	 * access to the parameter value
	 */
	public String getParameterValue() {
		return this.parameterValue;
	}

	/**
	 * access to the parameter user public
	 */
	public Boolean getParameterUserPublic() {
		return this.userPublic;
	}

	/**
	 * access to the parameter user public
	 */
	public void setParameterUserPublic(Boolean in) {
		this.userPublic = in;
	}

	
	/**
	 * set the parameter value
	 * @param in
	 */
	public void setParameterValue(String in) {
		this.parameterValue = in;
	}

	/**
	 * access to the parameter value
	 */
	public String getParameterName() {
		return this.parameterName;
	}

	/**
	 * access to the parameter type
	 */
	public String getParameterType() {
		return this.parameterType;
	}

	/**
	 * access to the parameter type
	 */
	public void setParameterType(String pT) {
		this.parameterType = pT;
	}

	/**
	 * access to the parameter full name
	 */
	public String getParameterFullName() {
		if (this.parameterFullName == null) {
			return this.parameterName;
		} else {
			return this.parameterFullName;
		}
	}

	/**
	 * access to the parameter full name
	 */
	public String getParameterOutsideName() {
		return this.getParameterFullName();
	}

	/**
	 * access to the parameter full name
	 */
	public void setParameterFullName(String in) {
		this.parameterFullName = in;
	}

	/**
	 * access to the parameter description
	 */
	public String getParameterDescription() {
		return this.parameterDescription;
	}

	/**
	 * access to the parameter description
	 */
	public void setParameterDescription(String in) {
		this.parameterDescription = in;
	}

	static {
		// this block will be called when the class is loaded first time.
		BasicParameterClass.initParameterTypes();
	}

	/**
	 * add the basic parameter types to the class:
	 */
	static private void initParameterTypes() {
		BasicParameterClass.PARMETERTYPES = new Vector<String>();
		// double
		BasicParameterClass.PARMETERTYPES.add("double");

		// int
		BasicParameterClass.PARMETERTYPES.add("int");

		// double
		BasicParameterClass.PARMETERTYPES.add("String");

		// Boolean
		BasicParameterClass.PARMETERTYPES.add("Boolean");

	}

	/**
	 * checks if this type is a valuable parametertype
	 */
	static public Boolean checkParameterType(String type) {
		Iterator I = BasicParameterClass.PARMETERTYPES.iterator();
		while (I.hasNext()) {
			String validType = (String) I.next();
			if (validType.equals(type)) {
				return true;
			}
		}
		return false;
	}

	/**
	 * update parameter (not the value and name)
	 */
	public void updateParameter(BasicParameterClass in) {

		if (this.getParameterType() == null) {
			this.setParameterType(in.getParameterType());
		}
		if (this.getParameterDescription() == null) {
			this.setParameterDescription(in.getParameterDescription());
		}
		if (this.getParameterFullName() == null) {
			this.setParameterFullName(in.getParameterFullName());
		}

		this.userPublic = in.getParameterUserPublic();

	}

}

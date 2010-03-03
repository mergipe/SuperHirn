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

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

import org.dom4j.DocumentException;
import org.dom4j.io.SAXReader;

import ch.superdbpusher.proteomics.LCMS;

/**
 * LCMSReader is a DOM based XML parser for files generated by SuperHirn. It
 * uses the handler XMLSuperHirnHandler for the parsing.
 * 
 * @author Lukas N Mueller, Zurich, Switzerland
 * @see XMLSuperHirnHander
 */

public class LCMSReader {

	protected File xmlFile;
	protected LCMS lcms;

	/*
	 * Public constructor of LCMSReader.
	 * 
	 * @param iFile file to the XML file which will be parsed
	 */
	public LCMSReader(File iFile) {
		this.xmlFile = iFile;
	}

	/**
	 * Start the XML parsing process.
	 * 
	 * @throws Exception
	 * @throws IOException
	 */
	public void startLCMSXMLparsing() throws Exception {

		if (this.xmlFile.exists()) {
			this.parseLargeXML(this.xmlFile);
		} else {
			throw new FileNotFoundException(this.xmlFile.getAbsolutePath());
		}
	}

	/*
	 * Start parsing of the input file using a SAX parser
	 * 
	 * @param iFile file of the XML to be parsed throws DocumentException throws
	 * Exception
	 */
	public void parseLargeXML(File iFile) throws DocumentException, Exception {
		SAXReader reader = new SAXReader();
		XMLSuperHirnHandler handler = new XMLSuperHirnHandler();
		reader.addHandler("/MASTER_RUN_SUMMARY", handler);
		@SuppressWarnings("unused")
		org.dom4j.Document doc = reader.read(iFile);
		this.lcms = handler.lcms();
	}

	/*
	 * Get the parsed LCMS run
	 * 
	 * @return LCMS
	 */

	public LCMS lcms() {
		return this.lcms;
	};

}
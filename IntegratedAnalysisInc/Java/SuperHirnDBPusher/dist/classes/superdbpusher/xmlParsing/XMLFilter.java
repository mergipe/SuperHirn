package ch.superdbpusher.xmlParsing;

import java.io.File;
import java.io.FileFilter;

/**
 * XMLFiler implements FileFilter and accepts directories and files ending with .xml.
 * 
 * @author Lukas N Mueller, Zurich, Switzerland
 * @see XMLSuperHirnHander
 */


public class XMLFilter implements FileFilter {

	/**
	 * Accepts directories and files ending with .xml
	 * @param iFile File input file to check
	 * @return boolean
	 * @see java.io.FileFilter#accept(java.io.File)
	 */
	public boolean accept(File iFile) {
		if (iFile.isDirectory()) {
			return true;
		} else if (iFile.getAbsolutePath().contains(".xml")) {
			return true;
		} else {

			return false;
		}
	}
}

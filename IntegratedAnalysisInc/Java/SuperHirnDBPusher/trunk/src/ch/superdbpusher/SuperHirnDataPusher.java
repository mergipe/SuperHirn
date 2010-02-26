package ch.superdbpusher;

/**
 * @brief SuperHirnDataPusher parses XML output files of the program SuperHirn
 *        and imports a subset of this data in the a relational database system.
 * 
 * @author Lukas N. Mueller
 */
public class SuperHirnDataPusher {

	/**
	 * @param args
	 *            the command line arguments
	 */
	public static void main(String[] args) {
		
		if (args.length == 1) {
			System.out.println("Starting to process file or directory '" + args[0] + "'");
			Manager mana = new Manager(args[0]);
			mana.start();
		}
		else{
			System.out.println("No argument for the input xml SupeHirn result file/folder provide.");
		}
	}

}

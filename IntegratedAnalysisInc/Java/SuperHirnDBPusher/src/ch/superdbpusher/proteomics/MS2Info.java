
 package ch.superdbpusher.proteomics;

import java.lang.String;	
import java.util.Iterator;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.Map.Entry;
import java.util.Set;

 
 
 
 
 

/**
	 * @author Lukas Mueller
	 * @version L2J0.1, 24.8.06
	 * 
	 * 
	 */
public class MS2Info implements Cloneable{
	
	
	// static members:
	static public Boolean TheoreticalMzValue = true;
	static public double MS2MzTolerance;
	static public double peptideProphetThreshold = -1.0;

	
	// here all the parameters for the MS2 scan
	private int scan_start;
	private int scan_end;
	private int chrg;
	
	// precursor mass
	private double mono_mz;
	// theoretical mass
	private double theo_mz;
	// molecular mass
	private double molecular_mass;
	// retention time of the scan:
	private double Tr;
	
	// //////////////////////////
	// MS2 interpretation information
	private String pepXML_file;
	// defines in what way a MS2 scan has been
	// matched to a MS1 features
	private String MS2_match_tag;
	
	private Boolean MS2_interpretation;
	private String SQ;
	private String prev_AA;
	private String FULL_SQ;
	private SortedMap< String , Double> AC;
	private SortedMap<Integer, Double> AA_mods;

	private double pep_prob;
	
	// statische members:
	// static values:
	static public double _MONO_H = 1.00728;
	static public double _MONO_O = 15.99491;
	static public double _MONO_N = 14.00674;
	static public double _MONO_C = 12.0107;
	static public double _MONO_S = 32.066;

	// monoisotopic masses of all amino acids:
	static public final double[] monoisotopic_AA_mass = { 71.03711, 0.0, 103.00919, 115.02694, 129.04259, 147.06841, 57.02146, 137.05891, 113.08406, 0.0, 128.09496, 113.08406, 131.04049, 114.04293, 0.0, 97.05276, 128.05858, 156.10111, 87.03203, 101.04768, 0.0, 99.06841, 186.07931, 0.0, 163.06333, 0.0};
	
/**
	 * Retention time coefficients as defined by Krokhin et al, MCP, 3 (9), 908, 2004
	 */
	static public final double[] retentionTimeCoeffKrokhin = { 0.8, 0, -0.8, -0.5, 0.0, 10.5, -0.9, -1.3, 8.4, 0, -1.9, 9.6, 5.8, -1.2, 0, 0.2, -0.9, -1.3, -0.8, 0.4, 0, 5.0, 11.0, 0, 4.0, 0};
	
/**
	 * Retention time coefficients as defined by Guo et al, refr 40 in Krokhin et al, MCP, 3 (9), 908, 2004
	 */
	static public final double[] retentionTimeCoeffGuo = { 2.0, 0, 2.6, 0.2, 1.1, 8.1, -0.2, -2.1, 7.4, 0, -2.1, 8.1, 5.5, -0.6, 0, 2.0, 0.0, -0.6, -0.2, 0.6, 0, 5.0, 8.8, 0, 4.5, 0};
	
	// one letter code of all amino acids:
	static public final char[] AA = {'A','C','D','E','F','G','H','I','K','L','M','N','P','Q','R','S','T','V','W','Y'};

	static public final double[] aminoAcidFrequencies = { 7.4, 0, 3.3, 5.9, 5.8, 4.0, 7.4, 2.9, 3.8, 0, 7.2, 7.6, 1.8, 4.4, 0, 5.0, 3.7, 4.2, 8.1, 6.2, 0, 6.8, 1.3, 0, 3.3, 0 };

		
	public MS2Info( ){
		MS2_interpretation = false;
		pep_prob = 0;
		this.prev_AA="";
		AC = new TreeMap< String, Double>() ;
		AA_mods = new TreeMap< Integer,Double>();

		set_scan_start ( 0 );					
		set_scan_end( 0 );
		set_chrg( 0 );					
		set_precursor_mz( 0 );	
		this.setRetentionTime(-1.0);
		
	}	
	
	public MS2Info( int IN_scan, double IN_mz, int IN_chrg, String IN_file){
		MS2_interpretation = false;
		pep_prob = 0;
		AC = new TreeMap< String, Double >() ;
		this.prev_AA = "";

		set_scan_start ( IN_scan );					
		set_scan_end( IN_scan );
		set_chrg( IN_chrg);					
		set_precursor_mz( IN_mz);			
		set_pepXML( IN_file);
		this.setRetentionTime(-1.0);

	}
	
	
	public MS2Info( String IN_AC, String IN_SQ, double IN_PP, int IN_CHRG, int IN_scan){
		MS2_interpretation = true;
		pep_prob = IN_PP;
		set_scan_start ( IN_scan );					
		set_scan_end( IN_scan );
		set_chrg( IN_CHRG);

		this.prev_AA = "";
		AC = new TreeMap< String,Double>();
		this.set_AC( IN_AC );
		this.set_SQ(IN_SQ);
		this.setRetentionTime(-1.0);
	}
	
	public MS2Info( String IN_SQ, double IN_PP, int IN_CHRG, int IN_scan){
		MS2_interpretation = true;
		pep_prob = IN_PP;
		set_scan_start ( IN_scan );					
		set_scan_end( IN_scan );
		set_chrg( IN_CHRG);

		this.prev_AA = "";
		this.set_SQ(IN_SQ);
		this.setRetentionTime(-1.0);
	}
	        	
/***
	 * Sets the
	 * 
	 * @param in
	 *            object MS2_scan
	 * 
	 * @param IN
	 *            scan start of a MS2 scan
	 */    	
	public void set_scan_start( int IN){this.scan_start = IN;};
/***
	 * Returns
	 * 
	 * @return
	 * @return start of the MS2 scan
	 */	
	public int get_scan_start( ){	return scan_start ;};
	
/***
	 * Sets the
	 * 
	 * @param in
	 *            object MS2_scan
	 * 
	 * @param IN
	 *            scan start of a MS2 scan
	 */    	
	public void set_scan_end( int IN){	scan_end= IN;};
/***
	 * Returns
	 * 
	 * @return
	 * @return end of the MS2 scan
	 */	
	public int get_scan_end( ){	return scan_end;};
	
/***
	 * Sets the
	 * 
	 * @param in
	 *            object MS2_scan
	 * 
	 * @param IN
	 *            charge state of a MS2 scan
	 */    	
	public void set_chrg( int IN){	chrg = IN;};
/***
	 * Returns
	 * 
	 * @return
	 * @return charge state of the MS2 scan
	 */	
	public int get_chrg( ){	return chrg;};

/***
	 * Sets
	 * 
	 * @param in
	 *            object MS2_scan and copmutes the molecular mass MR of the
	 *            initial peptide
	 * @param IN
	 *            precursor m/z of a MS2 scan
	 */    	
	public void set_precursor_mz( double IN){		  
		mono_mz = IN;
		molecular_mass = IN;
		molecular_mass *= (double) chrg;
		molecular_mass -= (double)chrg * _MONO_H; 
	};
/***
	 * Returns
	 * 
	 * @return
	 * @return m/z precursor
	 */	
	public double get_precursor_mz( ){	return mono_mz;};
	
/***
	 * Sets
	 * 
	 * @param from
	 *            which the MS2 scan has been parsed
	 * @param IN
	 *            pepXML file of a MS2 scan
	 */    	
	public void set_pepXML( String IN){	pepXML_file= IN;};
/***
	 * Returns
	 * 
	 * @return
	 * @return string of the pepXML file (interact)
	 */	
	public String get_pepXML( ){	return pepXML_file;};
	
/***
	 * Sets a modified AA at pos by the mod
	 * 
	 * @param pos
	 *            position of modified AA
	 * @param mod
	 *            mass of modified AA
	 * 
	 */    	
	public void add_AA_mod( int pos, double mod){ 
		
		if( this.AA_mods == null){
			this.AA_mods = new TreeMap<Integer, Double>();
		}
		AA_mods.put( pos, mod); 
		set_theo_mz_from_SQ();
		set_FULL_SQ();
	};
/***
	 * Returns
	 * 
	 * @return at pos
	 * @param AA
	 *            position
	 * @return if available at pos the modifed AA mass
	 */	
	public Double get_AA_mod( int pos){ 
		if( this.AA_mods == null){
			return null;
		}
		else{
			return AA_mods.get( pos ); 
			
		}
		
	}
	
/***
	 * Returns
	 * 
	 * @return at pos
	 * @param
	 * 
	 * @return entry set list of the modifed AA mass
	 */	
	public Set<Entry<Integer, Double> > get_AA_modification_entry_set(){ 
		if( this.AA_mods == null){
			return null;
		}else{
			return AA_mods.entrySet();
		}
	}
	
	public String get_AC( ){ return AC.firstKey();}
	
	
	
	;	
	public void set_AC( String IN ){
		set_AC( IN, -1.0 );
	}
	
	public void set_AC( String IN, double inProb ){
		
		if( AC == null){
			AC = new TreeMap<String, Double>();
		}
		
		AC.put(IN, inProb); 
		MS2_interpretation = true;
		};
	
/***
	 * Returns
	 * 
	 * @return
	 * @return if peptide is proteotypic or not
	 */	
	public Boolean get_proteo_type( ){ 
		if( AC.size() == 1){
			return true;
		}
		else{
			return false;
		}
	};	
	
/***
	 * Returns
	 * 
	 * @return
	 * @return AA seqauence
	 */	
	public String get_SQ( ){ return SQ;}
	
/**
	 * get the retention time of the scan:
	 */
	public Double getRetentionTime(){
		return Tr;
	}
	
/**
	 * set the retention time
	 */
	public void setRetentionTime(double in){
		this.Tr = in;
	}
	
/***
	 * Returns
	 * 
	 * @return
	 * @return full AA seqauence
	 */	
	public String getFullSQ( ){ 
		return String.format("%s.%s", this.prev_AA, this.get_SQ());
		};	
/***
	 * Sets IN
	 * 
	 * @param IN
	 *            Amino acid sequence
	 * 
	 */    	
	public void set_SQ( String IN ){ 
		SQ = IN; 
		MS2_interpretation = true;
		set_theo_mz_from_SQ();
		set_FULL_SQ();
	};

/***
	 * Returns
	 * 
	 * @return
	 * @return get the status if the MS2 scan has been interprated
	 */	
	public Boolean check_MS2_interpretation( ){ return MS2_interpretation; };

	
/***
	 * Compute the theoretical mass from the sequence incorporating available AA
	 * modifications
	 * 
	 * 
	 */    	
	public void set_theo_mz_from_SQ( ){
		
		if( SQ != null){


			this.theo_mz = 0;
	
			double TMP = 0;
	  
			for( int pos = 0; pos < SQ.length(); pos++){
	      
		  
				// check for modification:
				Double MOD = get_AA_mod( pos );
	
				if( MOD == null ){			  
			  
					TMP += getAminoAcidMass( SQ.charAt( pos ));
		  
				}
		  
				else{
			  
					TMP += MOD;
		  
				}
	  
			}
	  
			if( TMP > 0.0 ){
	    
				// add the H20 (+18) and
	    
				// add the charge state:
	    
				TMP += (2.0 * _MONO_H + _MONO_O);
	    
				TMP += chrg  * _MONO_H; 
	    
				TMP /= chrg;
	    
				theo_mz = TMP;
	  
			}
		}
	}
	
	
/***
	 * Compute the theoretical mass from the sequence incorporating available AA
	 * modifications
	 * 
	 * 
	 */    	
	static public double computeTheoMassFromSQ(String sq , int chrg){

		double TMP = 0;
		for( int pos = 0; pos < sq.length(); pos++){
			TMP += getAminoAcidMass( sq.charAt( pos ));
		}
		
		// add the H20 (+18) and
		// add the charge state:
		TMP += (2.0 * _MONO_H + _MONO_O);
		TMP += chrg  * _MONO_H; 
		if( chrg != 0 ){
		TMP /= chrg;
		}
		return TMP;
	}
	
/***
	 * Compute the theoretical retention time coefficient as defined by Krokhein et al., MCP, 2004
	 * 
	 * 
	 */    	
	static public double computeRetentionTimeCoefficient(String sq ){
		
		
		
		// define length correction coefficient KL:
		double KL = 1;
		if( sq.length() < 10 ){
			KL  = 1.0 - 0.027 * ( 10.0 - (double)sq.length()); 
		}
		else if( sq.length() > 20 ){
			KL  = 1.0 - 0.014 * ( (double)sq.length() - 20.0); 
		}
		
		//	copmute first the sum of all Rc values as by Guo et al
		double RC = 0;
		for( int pos = 0; pos < sq.length(); pos++){
			if( Character.isLetter( sq.charAt(pos) ))
			RC += getRetentionTimeCoefficientGuo( sq.charAt( pos ));
		}		
		
		// now the compensation factors for N terminal AA 1-3
		double RCNT = 0;
		for (int i = 0; i < 3; i++){
			
			if( Character.isLetter(sq.charAt(i))){
			
			double factor = 0;
			if( i==0){
				factor = 0.42;
			}
			else if( i == 1){
				factor = 0.22;
			}
			else if( i== 2){
				factor = 0.05;
			}
			RCNT += factor * getRetentionTimeCoefficientKrokhein( sq.charAt(i));	
			}
		}
		
		
		// compute the hydrophobicity:
		double H = KL * ( RC + RCNT);
		if( H >= 38 ){
			H = H - 0.3 * H + 11.4;
		}
		
		
		return H;
	}
	
	
/***
	 * Compute the theoretical molecular mass from the sequence incorporating available AA
	 * modifications
	 * 
	 * 
	 */    	
	static public double computeDehydratedMolecularMassFromSQ(String sq){

		double TMP = 0;
		for( int pos = 0; pos < sq.length(); pos++){
			TMP += getAminoAcidMass( sq.charAt( pos ));
		}
		// add the H20 (+18) and
		// add the charge state:
		return TMP;
	}
	
/***
	 * Compute the theoretical molecular mass from the sequence incorporating available AA
	 * modifications
	 * 
	 * 
	 */    	
	static public double computeMolecularMassFromSQ(String sq){

		double TMP = 0;
		for( int pos = 0; pos < sq.length(); pos++){
			TMP += getAminoAcidMass( sq.charAt( pos ));
		}
		
		// add the H20 (+18) and
		// add the charge state:
		TMP += (2.0 * _MONO_H + _MONO_O);
		return TMP;
	}
	
/**
	 * Compute the molecular mass from a m/z and charge state
	 */
	static public double computeMolecularMass( double iMz, int iZ){
		double MR = iMz * (double)iZ;
		// remove the H+'s
		MR -= ( MS2Info._MONO_H * (double)iZ );
		return MR;
	}
	
	
/**
	 * get the amino acid mass for an amio acid:
	 */
	static public double getAminoAcidMass( char AA){
		 
		
		int nb = 0;

		// ///////////////////////////////////
		// compute THEO M/Z
		if( AA == 'X' ){
		nb = (int)'L' - (int)'A';	          
		  }
		  else{
			  nb = (int)AA - (int)'A';
		  }
		 
		if( ( nb < 0 ) || ( nb >= monoisotopic_AA_mass.length ) ){
			return 0;
		}
		
		
		  return monoisotopic_AA_mass [ nb ];
	}
	
	
/**
	 * get the amino acid mass for an amio acid:
	 */
	static public double getAminoAcidFrequency( char AA){
		 
		int nb = 0;

		// ///////////////////////////////////
		// compute THEO M/Z
		if( AA == 'X' ){
		nb = (int)'L' - (int)'A';	          
		  }
		  else{
			  nb = (int)AA - (int)'A';
		  }
		  
		  return aminoAcidFrequencies[ nb ] / 100.00;
	}
	
/**
	 * get the amino acid mass for an amio acid:
	 */
	static public double getRetentionTimeCoefficientGuo( char AA ){
		 
		int nb = 0;
		if( AA == 'X' ){
			nb = (int)'L' - (int)'A';	          
		}
		else{
			nb = (int)AA - (int)'A'; 
		}
		return retentionTimeCoeffGuo[ nb ];
	}
	
/**
	 * get the amino acid mass for an amio acid:
	 */
	static public double getRetentionTimeCoefficientKrokhein( char AA ){
		 
		int nb = 0;
		if( AA == 'X' ){
			nb = (int)'L' - (int)'A';	          
		}
		else{
			nb = (int)AA - (int)'A'; 
		}
		return retentionTimeCoeffKrokhin[ nb ];
	}
	
	
/***
	 * Returns the theoretical mono isptopic mass of a given AA at pos
	 * 
	 * 
	 */    	

	public double get_MONO_AA_MASS( int POS ){
		int nb = 0;
		switch( SQ.charAt( POS ) ){

		case 'X':
		nb = (int)'L' - (int)'A';
		break;
	default:
		nb = (int)SQ.charAt( POS ) - (int)'A';

	}
	return monoisotopic_AA_mass[nb];
}

	
	
/***
	 * get the full modified AA seuqence
	 * 
	 * 
	 */    	
	public void set_FULL_SQ(){

		if( SQ != null){
		FULL_SQ = new String();
		for( int i=0; i< SQ.length(); i++){ 
			// insert normal AA letter:
			FULL_SQ += SQ.charAt(i);
			// check for modifications:
			Double MOD = get_AA_mod( i );
			if( MOD != null ){
				String S_MOD;
				S_MOD = String.format("[%.2f]", MOD);
				FULL_SQ += S_MOD;
			}	  
		}  
		}

	}
	
/***
	 * print the full information of the MS2_info object
	 * 
	 */    	
	public void show_info(){
		System.out.printf("\t> precursor m/z=%.3f, theo. m/z=%.3f, MS2_SCAN=%d, CHRG=%d ", mono_mz, theo_mz, scan_start, chrg);
		if( check_MS2_interpretation( ) ){
			System.out.printf(", AC=%s(%d), SQ=%s, PepProb=%.2f\n", get_AC(), this.AC.size(), get_TOTAL_SQ(), pep_prob);
		}		
		else{
			System.out.println();
		}
	}
	
	
/***
	 * print the full information of the MS2_info object
	 * 
	 */    	
	public String getInfoString(){

		String info = new String();
		
		info = String.format("%sAC=%s\n", info, this.AC.get(0) );
		info = String.format("%sSQ=%s", info, this.SQ );
		info = String.format("%s[%.1f]\n", info, this.pep_prob );
		
		return info;
	}
	
;

	
	 // /////////////////////////////
	  // start here all the get / set
	  // function to access the
	  // variables of the class
	;;
	 public String get_MOD_SQ(){return FULL_SQ;};
	  public String get_TOTAL_SQ(){return ( get_PREV_AA( )+ "." + get_MOD_SQ());};
;
	  
	  public Iterator get_AC_iterator(){return AC.entrySet().iterator();};
	;
	 ;
		public double get_PEP_PROB(){return pep_prob;};
	  public void set_PEP_PROB(double IN){pep_prob = IN;};
	  
	  public double get_MONO_MZ(){return mono_mz;}
	  public double getTheoreticalMZ(){return this.theo_mz;};
	  
	  public double get_NEUTRAL_MR(){return molecular_mass;};

	  int get_CHRG(){return chrg;};
	  void set_CHRG(int IN){chrg = IN;};

	  int get_SCAN(){return scan_start;};
	  int get_SCAN_START(){return scan_start;};
	  void set_SCAN_START(int IN){scan_start = IN;};

	  int get_SCAN_END(){return scan_end;};
	  void set_SCAN_END(int IN){scan_end = IN;};
	  
	  public void set_MS2_TYPE_TAG(String IN){MS2_match_tag= IN;};
	  public String get_MS2_TYPE_TAG(){return  MS2_match_tag;};

	  public String get_ORIGINAL_INTERACT_FILE(){ return pepXML_file;};
	  public void set_ORIGINAL_INTERACT_FILE( String IN){ pepXML_file = IN;};

	
	 /* 
	  * check the tryptic state of a peptide
	  */
	  public boolean isTrypticPeptide( ){
		  
		  //this.show_info();
		  		  
		  // check first AA:
		  if( (this.prev_AA.compareToIgnoreCase("K") != 0) && (this.prev_AA.compareToIgnoreCase("R") != 0 ) ){
			  return false;
		  }
	
		  if( ( !this.get_MOD_SQ().endsWith("K") ) && ( !this.get_MOD_SQ().endsWith("R")) ){
			  return false;
		  }
		  
		  return true;
	  }
	  
	  
	  
	  
	  public String get_PREV_AA( ){return prev_AA;};
	  public void set_PREV_AA( String IN ){prev_AA = IN;};
	
	
	  /*
		 * override the clone methode:
		 */
		public MS2Info clone() {
			try {
				MS2Info MYcopy = (MS2Info) super.clone();
				
				MYcopy.chrg = this.chrg;
				MYcopy.FULL_SQ = this.FULL_SQ;
				MYcopy.molecular_mass = this.molecular_mass;
				MYcopy.mono_mz = this.mono_mz;
				MYcopy.MS2_interpretation = this.MS2_interpretation;
				MYcopy.MS2_match_tag = this.MS2_match_tag;
				MYcopy.pep_prob = this.pep_prob;
				MYcopy.pepXML_file = this.pepXML_file;
				MYcopy.prev_AA = this.prev_AA;
				MYcopy.scan_end = this.scan_end;
				MYcopy.scan_start = this.scan_start;
				MYcopy.SQ = this.SQ;
				MYcopy.theo_mz = this.theo_mz;
				MYcopy.Tr = this.Tr;
				
				// modifications:
				if( this.AA_mods != null){
				
					MYcopy.AA_mods = new TreeMap<Integer, Double>();
					Iterator I = this.AA_mods.entrySet().iterator();
					while( I.hasNext()){
						SortedMap.Entry E = (SortedMap.Entry) I.next();
						MYcopy.AA_mods.put((Integer)E.getKey(),(Double)E.getValue());
					}
				}
				else{
					MYcopy.AA_mods = null;
				}
				
				// ac names:
				if( this.AC != null){
					MYcopy.AC = new TreeMap<String , Double>();
					MYcopy.AC.putAll(AC);
				}
				else{
					MYcopy.AC = null;
				}
				
				return MYcopy;
				
			} catch (CloneNotSupportedException e) {
				throw new Error(" Error in cloning fct of MS2_info");
			}
		}
	
/**
	 * if a seqeuence is proteotypic
	 */
	public Boolean isProteotypic(){
		if( this.AC.size() == 1){
			return true;
		}
		else{
			return false;
		}
	}
	
/**
	 * get the protein association probability for a AC:
	 */
	public double getACProbability( String iAC){
		return this.AC.get(iAC);
	}
	
	
	

}

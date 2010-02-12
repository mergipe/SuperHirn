///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS LC_MS_XML_writer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef LC_MS_XML_WRITER_H
#define LC_MS_XML_WRITER_H



class LC_MS_XML_writer{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ofstream* WRITER;
  LC_MS* MASTER;
  string out_file;
  string target_dir;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
    
    static bool STORE_FEATURE_ELUTION_PROFILES;
  static bool STORE_BEST_MS2_SCAN_PER_FEATURE;
  static bool STORE_BEST_MS2_SCAN_PER_ALIGNED_FEATURE;
  static int MAXIMAL_NB_ALTERNATIVE_PROTEIN_NAMES;
  static bool STORE_MS2_FRAGMENT_TRACE_DATA;


  // class destructor
  ~LC_MS_XML_writer();
  
  // class constructor
  LC_MS_XML_writer();
  LC_MS_XML_writer( string );
  // class copy constructor
  LC_MS_XML_writer(const LC_MS_XML_writer&);
  // class copy constructor
  LC_MS_XML_writer(const LC_MS_XML_writer*);
  
  
  
  // general XML stuff:
  // extracts the output directory:
  void set_OUT_DIR();
  // opens a file for writing:
  bool open_WRITER();
  // write XML HEADER:
  void write_XML_MAIN_HEADER();
  // write unformatted XML HEADER:
  void write_unformatted_XML_MAIN_HEADER();
  // write unformatted XML HEADER with input XST file:
  void write_unformatted_XML_MAIN_HEADER(string);
  // write close XML HEADER:
  void write_XML_MAIN_HEADER_CLOSE();
  // write comment:
  void write_comment_into_XML(string);

  
  // write out a complete LC/MS run (MASTER OR NOT)
  void WRITE_COMPLETE_LC_MS_2_XML(string, LC_MS*);
  // write the experiment analysis
  void write_MASTER_run_experiment_overview(LC_MS*);
  // write a header for LC/MS run into XML formated text into file:
  void write_LC_MS_run_2_XML_tag(LC_MS*);
  // write a feature tag into XML formated text into file:
  void write_feature_2_XML_tag(feature* IN, bool);
  void write_feature_2_XML_tag(feature* IN );
  // write a feature tag into XML formated text into file:
  void write_feature_2_XML_tag(feature* , ofstream* );

	// write a the MS2-object info into XML formated text into file:
	void write_ms2_info_2_XML_tag(map<double, vector<ms2_info> >::iterator , map<double, vector<ms2_info> >::iterator); 
 void write_ms2_info_2_XML_tag(ms2_info*);
 void write_ms2_info_2_XML_tag(feature*);

  // writes out a feature LC profile:
  void writeLCelutionProfile2XML( featureLCprofile* );
 
  // store all MS/MS spectra of a ms1 feature
  void write_ms2_spectra_2_XML_tag( vector<MS2_feature>*);
  void write_ms2_spectra_2_XML_tag( MS2_feature*);
  
  // stores MS2 fragment trace information:
  // (if option is turned on in (root) parameter file )
  void write_ms2_spectrum_2_XML_tag( MS2_feature*);
   

  // write a close tag for LC/MS run:
  void write_LC_MS_run_close_tag();
  // print similarity assessment info:
  void print_similarity_assessement();

  
  
  ///////////////////////////////////////////////
  // print out the content of MASTER run
  // into tab delimited EXCEL format:
  void write_MASTER_LC_MS_to_EXCEL( LC_MS* );
  // print the summary of the MASTER run
  void PRINT_MASTER_SUMMARY_to_EXCEL( );
  // print the content of the spectra (features)
  void print_EXCEL_feature_table();
  // prints the matched feature list for a peptide:
  void print_feature_to_EXCEL_table( feature* );
  void print_feature_to_EXCEL_table_APML_STYLE( feature* );


  
  
  
  
  //////////////////////////////////////////////////
  // overload operators:
  LC_MS_XML_writer& operator=(const LC_MS_XML_writer&);
  LC_MS_XML_writer& operator<=(const LC_MS_XML_writer&);
  LC_MS_XML_writer& operator>=(const LC_MS_XML_writer&);
  LC_MS_XML_writer& operator<(const LC_MS_XML_writer&);
  LC_MS_XML_writer& operator>(const LC_MS_XML_writer&);
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  ofstream* get_WRITER(){return WRITER;};

};

#endif

    

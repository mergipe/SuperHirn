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


#ifndef LC_MS_XML_READER_H
#define LC_MS_XML_READER_H



class LC_MS_XML_reader{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:

  vector<LC_MS> PARSED_LC_MS_RUNS;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
    static double TR_MIN;
  static double TR_MAX;
  static double FEATURE_MZ_MIN;
  static double FEATURE_MZ_MAX;
  static int FEATURE_CHRG_MIN;
  static int FEATURE_CHRG_MAX;
  // static double PEAK_SCORE_THERSHOLD;
  static double PEAK_INTENSITY_THRESHOLD;
  static bool EXTRACT_MONO_ISOTOPE_PROFILE;
  static double SIGNAL_TO_NOISE_THERSHOLD;
  static string DATA_STORAGE_XML_FORMAT_TYPE;
    
  LC_MS* THIS_LC_MS;
  
  string MAIN_XML_FILE;
  TiXmlDocument* PARSER;
  
  // class destructor
  ~LC_MS_XML_reader();
  
  // class constructor
  LC_MS_XML_reader();
  // class copy constructor
  LC_MS_XML_reader(const LC_MS_XML_reader&);
  // class copy constructor
  LC_MS_XML_reader(const LC_MS_XML_reader*);
  
  
  
  // get initialzation parameters
  void INIT_PARSER();
  // starts parsing of the LC/MS run
  void start_LC_MS_parsing( string );
  // starts parsing of LC/MS runs in a directory
  void start_LC_MS_parsing_of_DIR( string );
  // starts parsing of the LC/MS run of one file
  void start_LC_MS_parsing_of_FILE( string );
  // check if its a LC/MS XML file:
  bool check_LC_MS_XML_file( string );

  // store the LC/MS run in the list:
  void store_LC_MS_RUN( LC_MS* );
  // store a featuer in the current LC/MS run
  void store_FEATURE_in_THIS_LC_MS( feature* );
  // check if feature accepted
  bool check_FEATURE_VALIDITY( feature* );
  // get the LCMS id from the MasterMap or LCMS run by the LCMS run name
  int get_LCM_ID_by_name( string );
  // process the input run:
  void process_validated_LCMS( LC_MS* );



  
  
  //######################################################
  // parsing of MASTER XML encoded runs:
  // starts parsing of the LC/MS run in MASTER XML format
  void start_LC_MS_parsing_MASTER_XML_FORMAT();
  // extracts the LC/MS childs:
  void extract_LC_MS_childs( TiXmlNode* );
  // extract LC/MS features:
  void extract_LC_MS_features( TiXmlNode* );
  // extract LC/MS features:
  void extract_FEATURE( TiXmlNode* );
  // extract and returns a MS1 features:
  feature* extract_and_return_FEATURE( TiXmlNode*, feature*);
  // extract MS2 features info:
  void extract_MS_info( TiXmlNode* , feature*);
  // extracts the modifcation 
  void extract_AA_modifications(TiXmlNode* , ms2_info* );
  // extract now matched feature info:
  void extract_matched_features( TiXmlNode*, feature*);
  // extracts a profile 
  map<int, double> extract_profile(TiXmlNode* );
  // extract MS1 features LC profile:
  void extractLCProfile( TiXmlNode*  , feature* );
  // extract the ms2 conesnus spectrum of a feature
  void extractMS2ConsenusSpectrum( TiXmlNode* , feature* );
  // extract ms2 scans information:
  void extractPerformedMS2ScanInfo( string , feature* );



  
  
  
  //######################################################
  // starts parsing of the LC/MS run in APML XML format
  void start_LC_MS_parsing_APML_XML_FORMAT();
  // extracts the source LC/MS runs of a MASTERMAP
  void extract_LC_MS_source( TiXmlNode* );
  // extract feature list:
  void extract_feature_list( TiXmlNode* );
  // extract aligned feature list:
  void extract_aligned_feature_list( TiXmlNode* );
  // extract features APML:
  void extract_FEATURE_APML( TiXmlNode* );
  void extract_FEATURE_APML( TiXmlNode*, feature* );
  // extract MS2 features info from APML
  void extract_MS2_info_from_APML( TiXmlNode*, feature* );
  // extracts the modifcation 
  void extract_APML_AA_modifications(TiXmlNode*, ms2_info*);

  
  
  
  
  //######################################################
  // parsing of tab delim. format
  void start_LC_MS_parsing_MASTER_TAB_DEL_FORMAT( string );
  // extract the feature:
  void read_tab_del_feature( string );
  
  

  //////////////////////////////////////////////////
  // overload operators:
  LC_MS_XML_reader& operator=(const LC_MS_XML_reader&);
  LC_MS_XML_reader& operator<=(const LC_MS_XML_reader&);
  LC_MS_XML_reader& operator>=(const LC_MS_XML_reader&);
  LC_MS_XML_reader& operator<(const LC_MS_XML_reader&);
  LC_MS_XML_reader& operator>(const LC_MS_XML_reader&);
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

  // get teh parsed LC/MS
  vector<LC_MS> get_parsed_LCMS_list(){return PARSED_LC_MS_RUNS;};
  vector<LC_MS>::iterator get_parsed_LCMS_list_start(){return PARSED_LC_MS_RUNS.begin();};
  vector<LC_MS>::iterator get_parsed_LCMS_list_end(){return PARSED_LC_MS_RUNS.end();};
  bool check_parsed_LCMS_list(){ return !(PARSED_LC_MS_RUNS.empty());};

  string get_MAIN_XML_FILE(){ return MAIN_XML_FILE; };

  //////////////////////////////////////////////////////////
  // get the XML file ending 
  static string getXMLFileEnding();
    
};

#endif

    

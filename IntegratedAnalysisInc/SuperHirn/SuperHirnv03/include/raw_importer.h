///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS raw_importer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef raw_importer_H
#define raw_importer_H



class raw_importer{

    
    ////////////////////////////////////////////////
    // declaration of the private members:

private:

  ////////////////////////////////////////////////
    // declaration of the public members:
  
  Process_Data* raw_DATA;
  string target;
  string ORIGINAL_INTERACT_DIR;
  LCMSCData* PROCESSED_DATA;
  LC_MS* THIS_LC_MS;
  double TR_start, TR_end;
  vector<LC_MS> LC_MS_RUNS;
  string SOURCE_DIR;
  double LC_PEAK_THRESHOLD;
  double MZ2_TOL;
  double PEP_PROPH;
  int SCAN_MS2_TOL;
  bool THEO_MS_MATCH;
  
public:

    static bool CREATE_FEATURE_ELUTION_PROFILES;
  static bool LCelutionPeakDebugging;
  static double LCelutionPeakMassMin;
  static double LCelutionPeakMassMax;

    // class destructor
    ~raw_importer();
  
  // class constructor
  raw_importer(bool);
  // class copy constructor
  raw_importer(const raw_importer&);
  
  // adds an elution peak to the LC/MS run:
  void add_raw_peak_to_LC_MS_run(LC_elution_peak*,int);
  // function to add the elution profile to the feature:
  void addLCelutionProfile( feature* , LC_elution_peak* );

  
  // parses LC/MS from raw data, i.e. mzXML files:
  void parse_from_raw_data();
  
  // parses LC/MS from XML fiels, i.e. .xml files:
  void parse_from_XML_data();
  
  // write teh parseed LC/MS into directory:
  void write_out_parsed_LC_MS();
    
  // matches a MS2-info to a feature:
  // void match_MS2_info_to_feature(LC_MS*, ms2_info*);

  //////////////////////////////////////////////////
  // overload operators:
  raw_importer& operator=(const raw_importer&);
  raw_importer& operator<=(const raw_importer&);
  raw_importer& operator>=(const raw_importer&);
  raw_importer& operator<(const raw_importer&);
  raw_importer& operator>(const raw_importer&);
  
  
  // read mzXML data:
  void read_mzXML_data(int);
  
  // convert to elution peaks:
  void extract_elution_peaks(){
    if(raw_DATA != NULL){
      raw_DATA->extract_elution_peaks();
      PROCESSED_DATA = raw_DATA->get_processed_data();
    }
  };
  
  
  // print parsed raw data:
  void print_RAW_DATA(){
    if(raw_DATA != NULL){
      // raw_DATA->print_raw_DATA();
    }
  };
  
  // process extracted LC_peaks:
  void process_extracted_LC_peaks();
  // process input LC/MS runs:
  void process_input_LC_MS_runs();

  // filter LC peaks in all LC/MS runs by
  // their LC scores:
  void filter_data_by_LC_score();
    
  //////////////////////////
  // write in XML fomrat:
  //
  // writes a LC/MS run into XML formatted file:
  void write_out_LC_MS_run(LC_MS*, string);

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // target file:
  void set_target_file(string IN){target = IN;};
  string get_target_file(){return target;};
  
  // get extracted elution peaks:
  vector<LC_elution_peak> get_MZ_list(int SCAN, int tolerance){return PROCESSED_DATA->get_MZ_list(SCAN,tolerance);};
  vector<LC_elution_peak> get_MZ_list(int SCAN){return PROCESSED_DATA->get_MZ_list(SCAN);};
  
  // get all peaks:
  vector<LC_elution_peak*> get_ALL_peaks(){return PROCESSED_DATA->get_ALL_peak();};
  
  // access the LC/MS
  LC_MS* get_parsed_LC_MS(){return THIS_LC_MS;};
  
  // get the vector of LC/MS runs:
  vector<LC_MS> get_parsed_DATA(){return LC_MS_RUNS;};
  vector<LC_MS>::iterator get_parsed_DATA_START(){return LC_MS_RUNS.begin();};
  vector<LC_MS>::iterator get_parsed_DATA_END(){return LC_MS_RUNS.end();};
};

#endif

    

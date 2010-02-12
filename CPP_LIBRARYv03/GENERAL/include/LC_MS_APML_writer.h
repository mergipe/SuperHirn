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


#ifndef LC_MS_APML_WRITER_H
#define LC_MS_APML_WRITER_H



class LC_MS_APML_writer{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ofstream* WRITER;
  LC_MS* MASTER;
  string out_file;
  string target_dir;
  string PROCESSING_TYPE;
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~LC_MS_APML_writer();
  
  // class constructor
  LC_MS_APML_writer();
  LC_MS_APML_writer( string );
  // class copy constructor
  LC_MS_APML_writer(const LC_MS_APML_writer&);
  // class copy constructor
  LC_MS_APML_writer(const LC_MS_APML_writer*);
  
  
  
  // general XML stuff:
  // extracts the output directory:
  void set_OUT_DIR();
  // opens a file for writing:
  bool open_WRITER();
  // write XML HEADER:
  void write_APML_MAIN_HEADER();
  // write unformatted XML HEADER:
  void write_unformatted_APML_MAIN_HEADER();
  // write unformatted XML HEADER with input XST file:
  void write_unformatted_APML_MAIN_HEADER(string);
  // write close XML HEADER:
  void write_APML_MAIN_HEADER_CLOSE();
  // write comment:
  void write_comment_into_APML(string);
  // write the source runs:
  void write_MASTER_MAP_SOURCE(LC_MS* );
  
  // write out a MASTERMAP LC/MS run (MASTER OR NOT)
  void WRITE_MASTER_LC_MS_2_APML(string , LC_MS* );
  // write a header for LC/MS run into XML formated text into file:
  void write_LC_MS_run_2_APML_tag(LC_MS*);
  // write out a list of LC/MS runs
  void WRITE_COMPLETE_LC_MS_2_APML(string, LC_MS*);
  // write out a LC/MS runs
  void WRITE_COMPLETE_LC_MS_2_APML( LC_MS*);
  // write out a list of LC/MS runs
  void WRITE_COMPLETE_LC_MS_2_APML(string, vector<LC_MS>*);
  
  // MS1 feature:
  // write the feature coordinate
  void write_feature_coordinate(feature*);
  // write the aligned feature coordinate
  void write_aligned_feature_coordinate(feature*);
  // write a feature elution profile
  void write_APML_feature_Elution_Profile(featureLCprofile*);

  /////-------SHOULD RE-NAME the method name ----///////////////////////////////////////
  // write a feature elution profile
  void write_APML_feature_Elution_Profile(featureLCprofile* , feature* );
  
    // write a feature tag into APML formated into file:
  void write_aligned_feature_2_APML_tag(feature*);
  void write_aligned_feature_2_APML_tag(vector<feature*>*);
  // write a feature tag into APML formated into file:
  void write_feature_2_APML_tag(feature*);

  // MS2 scans:
  // write a the MS2-object info into APML formated file:
  void write_ms2_info_2_APML_tag(map<double, vector<ms2_info> >::iterator , map<double, vector<ms2_info> >::iterator, int); 
  void write_ms2_info_2_APML_tag(ms2_info*);
  void write_ms2_info_2_APML_tag( vector<ms2_info>* );
  void write_ms2_info_2_APML_tag( vector<ms2_info*>* );
  
  // write a close tag for LC/MS run:
  void write_LC_MS_run_close_tag();
  
  
  //////////////////////////////////////////////////
  // overload operators:
  LC_MS_APML_writer& operator=(const LC_MS_APML_writer&);
  LC_MS_APML_writer& operator<=(const LC_MS_APML_writer&);
  LC_MS_APML_writer& operator>=(const LC_MS_APML_writer&);
  LC_MS_APML_writer& operator<(const LC_MS_APML_writer&);
  LC_MS_APML_writer& operator>(const LC_MS_APML_writer&);
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  ofstream* get_WRITER(){return WRITER;};
  void set_PROCESSING_TYPE_TO_ALIGNMENT( ){ PROCESSING_TYPE = "alignment";}; 
};

#endif

    

///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS MS1_feature_ratiolizer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef USE_MS1_FEATURE_RATIOLIZER_H
#define USE_MS1_FEATURE_RATIOLIZER_H




class MS1_feature_ratiolizer{

  LC_MS* MASTER;
  int RATIO_TARGET_LC_MS;
  int REPLICATE_NUMBER;
  int CURRENT_LC_MS;
  bool PRINT_OUT;
  double DATA_PRECENTILE;
  double PROBABILITY_THRESHOLD;
  
  
  // peptide level analysis
  vector<peptide_DELTA_group> PEPTIDE_GROUPS;
  vector< PEPTIDE_RATIO> PEPTIDE_GROUP_ANALYSIS; 

  GAUSS_DIST CURRENT_GAUSS; 
  
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
    
    static map<int, vector<double> > LC_MS_Modification_Masses;
  static bool TRYPTIC_PEPTIDE_CHECK;
  static bool MODIFICATION_TIC_PEPTIDE_INTEGRATION;
  static bool SAVE_SINGLE_PROTEINS;

  // class destructor
  ~MS1_feature_ratiolizer();
  
  // class constructor
  MS1_feature_ratiolizer(LC_MS*);
  // class copy constructor
  MS1_feature_ratiolizer(const MS1_feature_ratiolizer&);
  // class copy constructor
  MS1_feature_ratiolizer(const MS1_feature_ratiolizer*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  MS1_feature_ratiolizer& operator=(const MS1_feature_ratiolizer&);
  bool operator==(const MS1_feature_ratiolizer&);
  MS1_feature_ratiolizer& operator<=(const MS1_feature_ratiolizer&);
  MS1_feature_ratiolizer& operator>=(const MS1_feature_ratiolizer&);
  MS1_feature_ratiolizer& operator<(const MS1_feature_ratiolizer&);
  MS1_feature_ratiolizer& operator>(const MS1_feature_ratiolizer&);
  
  // initialize parameters:
  void INIT();
  
  
  // extract the target feature and insert ratios:
  void start_RATIO_ANALYSIS();
  // compute probability of significant ratio 
  double set_Probability( double );

  
  
  ////////////////////////////////////////////////////////////
  // PEPTIDE LEVEL ANALYSIS FUNCTIONS:
  // extract peptide groups (decharging)
  void start_extracing_peptide_DELTA_groups( );
  // extract the ratio for peptide groups :
  void extract_peptide_group_ratios( );
  // make a gauss fit to the ratio histogram:
  void do_GAUSS_FIT( );
  // show peptide group analysis
  void show_peptide_group_analysis();
  // write peptide group analysis to XML file:
  void write_peptide_group_analysis_2_XML(ofstream*);
  // write PEPTIDE_GROUP_ANALYSIS group out:
  void write_PEPTIDE_GROUP_ANALYSIS_2_XML( PEPTIDE_RATIO* , ofstream*);
  // print all peptide ratios to simple text file
  void print_text_peptide_analysis();
  // filter the peptide groups:
  void filter_extracted_peptide_groups();
  // check if this is a good peptide delta group
  bool check_peptide_group_valid( peptide_DELTA_group* );
  // apply if option on the methionine
  // oxidation assembly
  void apply_methionine_oxidation_assembly( peptide_DELTA_group* );
    
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // get the comuted ratios:
  vector< PEPTIDE_RATIO>* get_PEPTIDE_ANALYSIS_LIST(){ return &PEPTIDE_GROUP_ANALYSIS;}; 
  vector< PEPTIDE_RATIO>::iterator get_PEPTIDE_ANALYSIS_LIST_START(){ return PEPTIDE_GROUP_ANALYSIS.begin();}; 
  vector< PEPTIDE_RATIO>::iterator get_PEPTIDE_ANALYSIS_LIST_END(){ return PEPTIDE_GROUP_ANALYSIS.end();}; 

};

#endif

    

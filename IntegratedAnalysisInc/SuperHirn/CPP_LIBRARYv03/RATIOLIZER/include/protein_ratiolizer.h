///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS protein_ratiolizer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef USE_PROTEIN_RATIOLIZER_H
#define USE_PROTEIN_RATIOLIZER_H


typedef struct{
  
  protein_group* TARGET;
  int ID;
  
  double P_RATIO_VALUE;
  double P_RATIO_VALUE_STDEV;
  
  double AVERAGE_PROTEIN_RATIO;
  double AVERAGE_PROTEIN_RATIO_STDEV;
  double NORMALIZED_AVERAGE_PROTEIN_RATIO;
    
  vector< pair< double, PEPTIDE_RATIO* > > ALL_PEPTIDE_RATIOS;
  vector< pair< double, PEPTIDE_RATIO* > > ALL_PEPTIDE_RATIOS_RAW;
  
  bool validAnalysis;
  
}
PROTEIN_RATIO;




class protein_ratiolizer{

  int RATIO_TARGET_LC_MS;
  int CURRENT_LC_MS;
  bool PRINT_OUT;
  bool INDIV_PROTEIN_RATIO_VIEW;
  double DATA_PRECENTILE;
  double PROBABILITY_THRESHOLD;
  
  
  // peptide analysis list:
  vector< PEPTIDE_RATIO>* INPUT_PEP_LIST;
  
  // protein level analysis
  vector<protein_group> PROTEIN_GROUPS;
  vector< PROTEIN_RATIO> PROTEIN_GROUP_ANALYSIS; 
  
  GAUSS_DIST CURRENT_GAUSS; 
  
  string target_dir;
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
    
    static map<int, vector<double> > LC_MS_Modification_Masses;

  
  // class destructor
  ~protein_ratiolizer();
  
  // class constructor
  protein_ratiolizer( );
  // class copy constructor
  protein_ratiolizer(const protein_ratiolizer&);
  // class copy constructor
  protein_ratiolizer(const protein_ratiolizer*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  protein_ratiolizer& operator=(const protein_ratiolizer&);
  bool operator==(const protein_ratiolizer&);
  protein_ratiolizer& operator<=(const protein_ratiolizer&);
  protein_ratiolizer& operator>=(const protein_ratiolizer&);
  protein_ratiolizer& operator<(const protein_ratiolizer&);
  protein_ratiolizer& operator>(const protein_ratiolizer&);
  
  // initialize parameters:
  void INIT();
  // extract the target feature and insert ratios:
  void start_RATIO_ANALYSIS();  
  
  ////////////////////////////////////////////////////////////
  // PEPTIDE LEVEL ANALYSIS FUNCTIONS:
  // create a protein ratio group:
  void start_extracing_protein_groups( );
  // find the corresponding protein ration group:
  bool find_PROTEIN_GROUPS_for_PEPTIDE( PEPTIDE_RATIO* );
  // analyze protein groups:
  void analyze_protein_groups( PROTEIN_RATIO* );
  // make a gauss fit to the ratio histogram:
  void do_GAUSS_FIT( );
  

  // print all protein ratio plot to simple text file
  void print_text_protein_analysis();

  // call all the plotting functions
  void print_to_plot_RATIO_ANALYSIS();
  // print the protein ratio plot:
  void print_protein_analysis(PROTEIN_RATIO*);
  // print all protein ratio plot:
  void print_all_protein_analysis();
  

  /////////////////////////////////////////////////////////////////////
  // write all portein groups analysis to XML file:
  void write_protein_group_analysis_2_XML(ofstream* );
  // write PROTEIN_GROUP_ANALYSIS group out:
  void write_PROTEIN_GROUP_ANALYSIS_2_XML( PROTEIN_RATIO* , ofstream* );



  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  void set_PEPTIDE_ANALYSIS_RATIO_LIST( vector< PEPTIDE_RATIO>* IN ){ INPUT_PEP_LIST = IN;};


};

#endif

    

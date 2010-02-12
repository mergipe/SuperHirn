///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS Spec Manager:
// management of the input LC-MS spectra
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef SUPERHIRN_MANAGER_H
#define SUPERHIRN_MANAGER_H


typedef vector<LC_MS>::iterator LC_MS_ITERATOR;

class SuperHirn_manager{

  
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:

  // mode to decide whether a normal(0) or a mastermap alignment(1) 
  int ALIGNMENT_MODE;

  // stores all the parsed lc-ms runs:
  vector <LC_MS> ALL_LC_MS_RUNS;

  // stores all the parsed Master Map runs:
  vector <LC_MS> ALL_LC_MS_MASTER_MAP;
  
  // LC-MS master map:
  LC_MS* MASTER;
  
  // statistics box:
  statistic_box* STAT;
  
  // project source directory:
  string SOURCE_DIR;
  string mzXMLDataPath;
  string interactPepXMLPath;
  string inclusionPepXMLListPath;
  
  // LC_MS similarity matrix:
  LC_MS_similarity_matrix* SIM_MATRIX;
  
  ////////////////////////////////////////////////
  // declaration of the public members:

public:

  
  ////////////////////////////////////////////////
  // class destructors / constructors:
  // class destructor
  ~SuperHirn_manager();
  // class constructor
  SuperHirn_manager();
  // class constructor
  SuperHirn_manager(vector< LC_MS>);
  
  
  ////////////////////////////////////////////////////
  // display functions
  // show all spectra in the vector:
  void show_spectra();
  // show imported MasterMaps
  void show_MASTER_MAPS();
  // print out the content of the created
  // Master LC_MS to a file
  void print_Master_LC_MS_to_excel();
  void print_Master_LC_MS_to_excel( string );
  // prints the master run, so it does not have to be computed
  // all the time into XML formatted file:
  void print_MASTER_to_XML_file();
  void print_MASTER_to_XML_file( string );
    // prints out a feature
  void print_feature_to_file(feature*, ofstream*);
  // plots a LC/MS run:
  void plot_LC_MS_pair(LC_MS*, LC_MS*, string);
  
  ////////////////////////////////////////////////////
  // convert the master run to excel file
  void convert_Master_2_excel();
  void convert_Master_2_excel( string);

  
  ////////////////////////////////////////////////////
  // PERFORMS DATA ANALYSIS STEPS ON THE MASTER RUN
  void MASTER_LC_MS_DATA_ANALYSIS();

  ////////////////////////////////////////////////////
  // starts delta search analysis of the MASTER
  void start_DELTA_SEARCH_MASTER_analysis();
  
  ////////////////////////////////////////////////////
  // starts ratiolizer analysis
  void start_RATIOLIZER_analysis();
  
  ////////////////////////////////////////////////////////////////////////
  // do intensity normalization of the MasterMap
  void start_INTENSITY_NORMALIZATION( );
    
  ////////////////////////////////////////////////////////////////////////
  // update the mastermap by inlcuions list ms2 scans
  void start_MASTERMAP_INCLUSION_LIST_UPDATING( );
  void start_MASTERMAP_INCLUSION_LIST_UPDATING( string );


  // add a spectrum to the list
  void add_LC_MS_run(LC_MS* IN){
    ALL_LC_MS_RUNS.push_back(*IN);
    IN = NULL;
  };
  
  ////////////////////////////////////////////////////
  // alignment LC-MS runs functions
  // main function to start the pairwise LC/MS alignment 
  void start_PAIRWISE_ALIGNMENT();
  // alignment of 2 spectra as defined by the input
  void alignLCMSpair(LC_MS*, LC_MS*);
  
  
  //////////////////////////////////////////////////////////
  // import data either from XML
  void parse_INPUT_DATA( ); 
  //////////////////////////////////////////////////////////
  // parse already existing MasterMaps
  void parse_MASTER_MAPS( );

  ////////////////////////////////////////////////
  // NOTE by Vagisha: added this method
  // We will not hold any extracted LC_MS objects while doing peak extraction
  // if multiStep == false, currently not used
  void start_peak_extraction( );
  void start_peak_extraction( string );
    
  
  /////////////////////////////////////////////////////////////
  // build guided tree: takes the distance matrix (= matrix of overlap numbers)
  // and builds a tree using the object tree_builder:
  void start_build_guide_tree();  
  
  //////////////////////////////////////////////////////////////		
  // start here to create MASTER MAP for different LC_MS groups
  void start_creating_MASTER_MAP();
  void start_creating_MASTER_MAP(string);

  
  ////////////////////////////////////////////////////////////////////////
  // start the multiple LC_MS merging process 
  void START_MULTIPLE_ALIGNMENT_OF_LC_MS_MAPS();
  
  
  ////////////////////////////////////////////////////////////////////////
  // start here to create the SuperMap from the different MasterMaps
  void start_creating_SUPER_MAP();
  
  ////////////////////////////////////////////////////////////////////////
  // converts a set of input XML formatted runs to APML
  void convert_XML_to_AMPL( string );
  
  
  ////////////////////////////////////////////////////////////////////////
  // starts delta search analysis of the MASTER
  void start_MS2_TRACE_FRAGMENT_analysis();    
  

  //////////////////////////////////////////////////////////////		
  // starts the statistical analysis of the MASTER
  void start_PROFILING_MASTER_analysis();

  ////////////////////////////////////////////////////////
  // analyze profiling data from superhirn k-means clustering
  void start_PROFILING_EVALUATION();

  
  ////////////////////////////////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // access the LC-MS by their id:
  static LC_MS* get_LC_MS_by_ID(void *pInstance, int);
  // delete a LC-MS by their id:
  static void delete_LC_MS_by_ID(void *pInstance,int ID);

  // get number of spectra added:
  int get_nb_LCMS_runs(){return ALL_LC_MS_RUNS.size();};
  // get the number of maps to align
  int get_NB_ALIGNMENT_MAPS();
  // get a reference to the vector of maps to be aligned
  vector<LC_MS>* get_ALIGNMENT_MAP_VECTOR();

  
  //////////////////////////////////////////////////////////
  // access methods to LC-MS runs:
  
  // access the LC/MS list
  LC_MS_ITERATOR get_LC_MS_LIST_start(){return ALL_LC_MS_RUNS.begin();};   
  LC_MS_ITERATOR get_LC_MS_LIST_end(){return ALL_LC_MS_RUNS.end();};   
  
  
};

#endif

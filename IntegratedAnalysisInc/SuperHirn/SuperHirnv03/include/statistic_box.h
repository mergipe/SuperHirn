///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS statistic_box:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef statistic_box_H
#define statistic_box_H



class statistic_box{

    
    ////////////////////////////////////////////////
    // declaration of the private members:

private:
  
    ////////////////////////////////////////////////
   
  // best profile cluster map: 
  vector< profile_group> TARGET_PROFILE_GROUPS;  
  
  LC_MS* MASTER;  
  profile_comparer* PRO_COMP;
  
  // variation profiles, keys is the run ID:
  map<int, map<double,double> > RELATIVE_VARIATIONS; 
  map<int, map<double,double> > ABSOLUTE_VARIATIONS;

public:

    static bool ABSOLUTE_NORMALIZATION_MODE;
  // class destructor
  ~statistic_box();
  
  // class constructor
  statistic_box();
  // class copy constructor
  statistic_box(const statistic_box&);
  
  
  //**************************************************************
  // LC/MS RUN NORMALIZATION FUNCTIONS
  
  // NORMALIZE INTENSITY VARIATION ACROSS SAMPLES:
  // compute variation profiles along TR for every run:
  // and apply normalization to features for each input MASTER run:
  void start_GLOBAL_INTENSITY_NORMALIZATION();
  //**************************************************************
  
  
  
  
  //**************************************************************
  // MS1 FEATURE PROFILE CLUSTERING FUNCTIONS
  
  // start the clustering process:
  void START_MS1_FEATURE_PROFILING();
  
  
  ////////////////////////////////////////////////////
  // analyze feature profiles
  // either by the created profile gropus
  // or rank them according to profile similarity 
  // to teh target profile:
  void ANALYZE_MS1_FEATURE();
  
  // analyze the extracted profile groups
  // - build peptide groups
  // - build protein groups
  // - make a statistic for every profile groups:
  void ANALYZE_MS1_FEATURE_PROFILING();
  
  // extracts from the mastermap a profile group
  // filters for features with min profile lengths
  void EXTRACT_PROFILE_GROUP_FROM_MASTERMAP();
  // extracts profile groups directly from Kmeans groups:
  void EXTRACT_PROFILE_GROUP_FROM_KMEANS();
    

  //**************************************************************
    // search for delta peptide pairs:
  void extract_DELTA_PEPTIDE_PAIRS();
  
  //**************************************************************
  // start ratiolizer analysis:
  void start_RATIOLIZER_ANALYSIS();
  
  
  //**************************************************
  // help functions:
  
  // check if the kmeans fiel is there:
  bool checkKmeanClustringFile();
    
      
  
  
  //////////////////////////////////////////////////
  // overload operators:
  statistic_box& operator=(const statistic_box&);
  statistic_box& operator<=(const statistic_box&);
  statistic_box& operator>=(const statistic_box&);
  statistic_box& operator<(const statistic_box&);
  statistic_box& operator>(const statistic_box&);
  
    
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // get a master run:
  LC_MS* get_MASTER_RUN(){ return MASTER;};
  void set_MASTER_RUN( LC_MS* IN){ MASTER = IN;};
  
  // imports the MASTER LC/MS run:
  void import_MASTER_run();
    
};

#endif

    

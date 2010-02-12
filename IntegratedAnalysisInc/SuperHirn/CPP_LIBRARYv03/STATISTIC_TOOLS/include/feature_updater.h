///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS feature_updater:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef FEATURE_UPDATER_H
#define FEATURE_UPDATER_H



class feature_updater{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  string MAIN_INTERACT_DIR;
  int tot_input_MS2_scans;
  vector<ms2_info> ALL_MS2_INFO_LIST;
  vector<ms2_info> THIS_MS2_INFO_LIST;

  double SCAN_TOL;
  double PEP_PROB_CUT_OFF;
  unsigned int MS2_HITS;
  
  double ALL_HITS, GOOD_HITS, LOW_HITS;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~feature_updater();
  
  // class constructor
  feature_updater();
  // class copy constructor
  feature_updater(const feature_updater&);
  // class copy constructor
  feature_updater(const feature_updater*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  feature_updater& operator=(const feature_updater&);
  bool operator==(const feature_updater&);
  feature_updater& operator<=(const feature_updater&);
  feature_updater& operator>=(const feature_updater&);
  feature_updater& operator<(const feature_updater&);
  feature_updater& operator>(const feature_updater&);
  

  // initialze the main parameters
  void INIT_ALL();
  void INIT_ALL(string);
  // start importing MS2 information from various sources:
  void IMPORT_MS2_INFO();
  // parses all interact files in a certain directory:
  void parse_interact_files_in_DIR( string );



  
  
  // update all extracted features with additional MS2 information:
  void update_feature_identity( profile_group* );
  void update_feature_identity( LC_MS*);
  void update_feature_identity( peptide_DELTA_group* );
  void update_feature_identity( vector<peptide_DELTA_group>* );
  // update an extracted features with additional MS2 information:
  void update_feature_identity( feature* IN);
  // compares a MS1 with MS2, if ok add its to the MS1 features
  bool compare_MS2_with_MS1_feature( feature* , ms2_info*);

  // this structure provides the function to compare
  // in the sorting algorithm:
  struct FEATURE_MZ_OPERATOR{
    // provide the compare function for sort:
    bool operator()(const feature* A, const feature* B) const{
      // check if they have same mass
      if(A->MONO_MZ == B->MONO_MZ){
        return A->TR < B->TR;
      }
      else{
        return A->MONO_MZ < B->MONO_MZ;
      }
    }
  };
  
  
  // update the ms2 matching statistics:
  void update_MS2_matching_statistics( ms2_info* );
  // print summary:
  void print_update_summary();

  
  // add one to the counter
  void increase_MS2_HIT(){MS2_HITS++;};
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

};

#endif

    

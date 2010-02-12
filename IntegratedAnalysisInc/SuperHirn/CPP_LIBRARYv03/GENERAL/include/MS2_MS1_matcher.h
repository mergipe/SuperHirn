///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS MS2_MS1_matcher:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef MS2_MS1_MATCHER_H
#define MS2_MS1_MATCHER_H

// data structre to save potential match candidates
typedef feature* MS2_CANDIDATE;



class MS2_MS1_matcher{
  
  
  ////////////////////////////////////////////////
  // declaration of the private members:
  ms2_info* MS2_SCAN;
  vector< MS2_CANDIDATE > MS2_CANDIDATE_LIST;
  vector< ms2_info* > MISSED_MS2;
  bool POST_ANNOTATION;
  int REQUIRED_LC_MS_ID;
  
  int specific_MS2_counter;
  int specific_MS2_MS1_assignments;
  int highQual_specific_MS2_counter;
  int highQual_specific_MS2_MS1_assignments;

  
private:
    
    ////////////////////////////////////////////////
    // declaration of the public members:

public:
    
  static double PEP_PROB_CUT_OFF;
  static bool NON_MATCH_VIEW;
  static int SCAN_TOL;
  static int POST_SCAN_TOL;
  
  static int AM_APEX_COUNTER;
  static int AM_COUNTER;


    
    // class destructor
    ~MS2_MS1_matcher();
  
  // class constructor
  MS2_MS1_matcher( );
  // class copy constructor
  MS2_MS1_matcher(const MS2_MS1_matcher&);
  MS2_MS1_matcher(const MS2_MS1_matcher*);

  
  //////////////////////////////////////////////////
  // overload operators:
  MS2_MS1_matcher& operator=(const MS2_MS1_matcher&);
  MS2_MS1_matcher& operator<=(const MS2_MS1_matcher&);
  MS2_MS1_matcher& operator>=(const MS2_MS1_matcher&);
  MS2_MS1_matcher& operator<(const MS2_MS1_matcher&);
  MS2_MS1_matcher& operator>(const MS2_MS1_matcher&);
  
  
  
  
  
  
  
  
  //////////////////////////////////////////////////
  // these function here to assign MS2 data accquired 
  // during a LC/MS to MS1 features extracted from 
  // this LC/MS run
  // function which assigns accquired MS2 scans
  // of a LC/MS run into its corresponding MS1 features
  void combine_LC_MS_specific_MS2_to_MS1_data( LC_MS*, ms2_info*);  
  // matches a MS2-info to a feature within a list of features :-)
  void match_MS2_info_to_feature( LC_MS* );
  // set the LC/MS ID fro which the ms2 scan was accquired:
  void set_MS2_scan_LCMS_ID( LC_MS* );

  
  
  
  //////////////////////////////////////////////////
  // function which assigns MS2 scans accquired from 
  // other LC/MS runs into MS1 features
  bool combine_POSTANNOTATION_MS2_to_MS1_data( vector<feature*>* , ms2_info* );
  // matches a MS2-info to a feature within a list of features :-)
  bool match_MS2_info_to_feature( vector<feature*>* );
  
  
  
  // adds a candidate to the lc_peak list: 
  void add_match_candidate( MS2_CANDIDATE );
		
  // get the best match, which is the last one remaining in a vector
  MS2_CANDIDATE get_best_match(){return *MS2_CANDIDATE_LIST.begin();};
  
  
  // matches a MS2-info to a feature within a list of features :-)
  void match_MS2_info_to_feature( ms2_info* );
  

  // check if the ms2 scan possible can fit to the feature:
  bool checkMS12MS2Combination( ms2_info*, feature*);
      

  // match the input lc_peak from LC_MS run B against the extracted region:
  // m/z has already been compared, check now which one is closest to the peak
  // and compare also if you can find similar charge states peptides
  void find_best_match( );
  
  
  // show specific MS2 to MS1 assignment overview:
  void show_annotation_overview();
  // detailed missed annotation summary:
  void showDetailedMissedAnnotationSummary();

  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  int get_nb_candidates(){return MS2_CANDIDATE_LIST.size();};
  int get_SCAN_TOL(){ if(POST_ANNOTATION){ return POST_SCAN_TOL;}else{return SCAN_TOL;};};


  //////////////////////////////////////////////////////////////
  // shows if amigouities have been found:
  static void showAmbiguouities();
  // compare 2 m/z values and decide whether they fall into the m/z tolerance
  // range or not:
  static bool checkMZTolerance( double , double);
  

};

#endif

    

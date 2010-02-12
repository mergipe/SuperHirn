///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS profile_scorer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef PROFILE_SCORER_H
#define PROFILE_SCORER_H

typedef struct{
  int ID;
  double PROFILE;
  double TARGET;
  double DELTA;
  pair<bool,double> PROFILE_STDEV;
}
COMPARE_PROFILE_POINT;

class profile_scorer{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
  map<int, double> TARGET_PROFILE;
  
  // area to compare
  vector< COMPARE_PROFILE_POINT > COMPARE_AREA;
  // in profile of the to be scored profile:
  map<int, double>* IN_PROFILE; 
  map<int, double>* IN_PROFILE_STDEV;
  
  double TOT_SCORE;
  double ALL_PROFILE_SCORE;
  double PROFILE_SCORE;
  double OUTSIDE_SCORE;
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
    static bool PROFILE_INTERPOLATION;
  static bool LOG_INTENSITY_TRANSFORMATION;
  static int MAX_NB_PROILE_DATA_POINTS;


    
  // class destructor
  ~profile_scorer();
  
  // class constructor
  profile_scorer();
  profile_scorer( vector<double> );
  profile_scorer( map<int, double> );
  profile_scorer( map<int, double>* );
  profile_scorer( feature* );
  // class copy constructor
  profile_scorer(const profile_scorer&);
  // class copy constructor
  profile_scorer(const profile_scorer*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  profile_scorer& operator=(const profile_scorer&);
  bool operator==(const profile_scorer&);
  profile_scorer& operator<=(const profile_scorer&);
  profile_scorer& operator>=(const profile_scorer&);
  profile_scorer& operator<(const profile_scorer&);
  profile_scorer& operator>(const profile_scorer&);
  
  
  //////////////////////////////////////////////////
  // copmparer in profile vs target profile:
  double COMPARE_TO_TARGET_PROFILE( map<int, double>* );
  // copmparer in feature profile vs target profile:
  double COMPARE_TO_TARGET_PROFILE( feature* );
  // comparer in profile vs target profile:
  double COMPARE_TO_TARGET_PROFILE( map<int, double>* , map<int, double>* );


  
  
  //////////////////////////////////////////////////
  // extract the overlapping profile data points
  // and normalize by the total area:
  void EXTRACT_COMPARE_PROFILE_AREA();
  // interpolate the profile, fil gaps etc.
  void INTERPOLATE_PROFILE();
  // extract a feature profile::
  // void EXTRACT_NORMALIZED_PROFILE(feature*);


  
  //////////////////////////////////////////////////
  // compute each score:
  void COMPUTE_SCORES();
  // compute the profile score
  void DEFINE_PROFILE_SCORES();
  
  
  // print the compare area:
  void print_compare_AREA();

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

  // set the input profile:
  void set_IN_PROFILE( map<int, double>* IN ){IN_PROFILE = IN;};
  // set the input profile stdev:
  void set_IN_PROFILE_STDEV( map<int, double>* IN ){IN_PROFILE_STDEV = IN;};
  
  // get the total score:
  double get_TOTAL_SCORE();

  map<int,double> get_TARGET_PROFILE(){ return TARGET_PROFILE; };
  map<int,double> get_NORMALIZED_PROFILE(){ return TARGET_PROFILE; };
};

#endif

    

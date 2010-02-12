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

#define USE_PROFILE_SCORER
#include "CLUSTER_LC_MS_ANALYSIS_USE.h"

bool profile_scorer::PROFILE_INTERPOLATION = false;
bool profile_scorer::LOG_INTENSITY_TRANSFORMATION;
int profile_scorer::MAX_NB_PROILE_DATA_POINTS;

////////////////////////////////////////////////
// constructor for the object profile_scorer:
profile_scorer::profile_scorer(){
  TARGET_PROFILE.clear();
  IN_PROFILE_STDEV = NULL;
  TOT_SCORE = 0;
  PROFILE_SCORE = 0;
  OUTSIDE_SCORE = 0;
  ALL_PROFILE_SCORE = 0;  
  IN_PROFILE = NULL;
}

////////////////////////////////////////////////
// constructor for the object profile_scorer:
profile_scorer::profile_scorer( vector<double> IN){
  
  
  // create a profile for teh vector:
  TARGET_PROFILE.clear();
  consens_profile_builder CONS;
  TARGET_PROFILE = CONS.normalize_profile( IN  ); 
 
  
  TOT_SCORE = 0;
  PROFILE_SCORE = 0;
  OUTSIDE_SCORE = 0;
  ALL_PROFILE_SCORE = 0;
  IN_PROFILE_STDEV = NULL;
  IN_PROFILE = NULL;
  
}

////////////////////////////////////////////////
// constructor for the object profile_scorer:
profile_scorer::profile_scorer( feature* IN){
  
  
  // create a profile for teh vector:
  TARGET_PROFILE.clear();
  consens_profile_builder CONS;
  TARGET_PROFILE = CONS.build_feature_profile( IN ); 
 
  IN = NULL;
  TOT_SCORE = 0;
  PROFILE_SCORE = 0;
  OUTSIDE_SCORE = 0;
  ALL_PROFILE_SCORE = 0;
  IN_PROFILE_STDEV = NULL;
  IN_PROFILE = NULL;

}

////////////////////////////////////////////////
// constructor for the object profile_scorer:
profile_scorer::profile_scorer( map<int, double> IN){
  
  TARGET_PROFILE = IN;
  
  if( LOG_INTENSITY_TRANSFORMATION ){
    simple_math MY_MATH;
    map<int, double>::iterator P = TARGET_PROFILE.begin();
    while( P != TARGET_PROFILE.begin() ){
      (*P).second = MY_MATH.log_tranform_F_int( (*P).second );
      P++;
    }
  }
  consens_profile_builder CONS;
  TARGET_PROFILE = CONS.normalize_profile( TARGET_PROFILE );    
  
  TOT_SCORE = 0;
  PROFILE_SCORE = 0;
  OUTSIDE_SCORE = 0;
  ALL_PROFILE_SCORE = 0;
  IN_PROFILE_STDEV = NULL;
  IN_PROFILE = NULL;
}

////////////////////////////////////////////////
// constructor for the object profile_scorer:
profile_scorer::profile_scorer( map<int, double>* IN){
  
  consens_profile_builder CONS;
  TARGET_PROFILE = CONS.normalize_profile( *IN );  
 
  
  TOT_SCORE = 0;
  PROFILE_SCORE = 0;
  OUTSIDE_SCORE = 0;
  ALL_PROFILE_SCORE = 0;
  IN_PROFILE_STDEV = NULL;
  IN_PROFILE = NULL;
}

//////////////////////////////////////////////////
// class desctructor of profile_scorer
profile_scorer::~profile_scorer(){
  TARGET_PROFILE.clear();
  COMPARE_AREA.clear();
  IN_PROFILE_STDEV = NULL;
  IN_PROFILE = NULL;
}

//////////////////////////////////////////////////
// class copy constructor of profile_scorer
profile_scorer::profile_scorer(const profile_scorer& tmp){
  TARGET_PROFILE = tmp.TARGET_PROFILE;
}

//////////////////////////////////////////////////
// class copy constructor of profile_scorer
profile_scorer::profile_scorer(const profile_scorer* tmp){
  TARGET_PROFILE = tmp->TARGET_PROFILE;
}

//////////////////////////////////////////////////
// copy constructor:
profile_scorer& profile_scorer::operator=(const profile_scorer& tmp){
  TARGET_PROFILE = tmp.TARGET_PROFILE;
  return *this;
}


//////////////////////////////////////////////////
// copmparer in feature profile vs target profile:
double profile_scorer::COMPARE_TO_TARGET_PROFILE( feature* IN ){
  
  // create a profile for teh vector:
  consens_profile_builder CONS;
  map<int, double> AREA = CONS.build_feature_profile( IN ); 
  
  // set it:
  set_IN_PROFILE( &AREA );
    
  // compute the individual scores:
  COMPUTE_SCORES();
  
  // compute the main score and return it:
  return get_TOTAL_SCORE();
}

//////////////////////////////////////////////////
// comparer in profile vs target profile:
double profile_scorer::COMPARE_TO_TARGET_PROFILE( map<int, double>* TARGET, map<int, double>* PROFILE ){
  
  TARGET_PROFILE = *TARGET;
  
  // set it:
  set_IN_PROFILE( PROFILE );
  
  // compute the individual scores:
  COMPUTE_SCORES();
  
  // compute the main score and return it:
  return get_TOTAL_SCORE();
}


//////////////////////////////////////////////////
// comparer in profile vs target profile:
double profile_scorer::COMPARE_TO_TARGET_PROFILE( map<int, double>* PROFILE ){
  
  // set it:
  set_IN_PROFILE( PROFILE );
  
  // compute the individual scores:
  COMPUTE_SCORES();
  
  // compute the main score and return it:
  return get_TOTAL_SCORE();
}


/////////////////////////////////////////////////
// compute each score:
void profile_scorer::COMPUTE_SCORES(){

  if( !IN_PROFILE->empty() ){
    
    // reset all scores:
    TOT_SCORE = 0;
    PROFILE_SCORE = 0;
    OUTSIDE_SCORE = 0;
    ALL_PROFILE_SCORE = 0;
    
    // A. process the pofile
    if( PROFILE_INTERPOLATION ){
      INTERPOLATE_PROFILE();
    }
    
    // B. extract the common area:
    EXTRACT_COMPARE_PROFILE_AREA();
    
    // C1. profile score;
    DEFINE_PROFILE_SCORES();

  }
  else{
    printf("\t\t-> WARNING! compare profile empty!::profile_score, 219\n"); 
  }
}



/////////////////////////////////////////////////
// interpolate the profile, fil gaps etc.
void profile_scorer::INTERPOLATE_PROFILE(){
  
  // go through the profile and interpolate missing values:
  // dont do for start /end points !!!
  for( unsigned int ID = 1; ID < (TARGET_PROFILE.size() - 1); ID++){
    
    map<int, double>::iterator F = IN_PROFILE->find( ID );
    if( F == IN_PROFILE->end() ){
      map<int, double>::iterator DOWN = IN_PROFILE->find( ID - 1 );
      map<int, double>::iterator UP = IN_PROFILE->find( ID + 1 );
      
      // check that data is available:
      if( ( UP != IN_PROFILE->end() ) && ( DOWN != IN_PROFILE->end() ) ){
        double NEW = ( (*UP).second + (*DOWN).second ) / 2.0;
        IN_PROFILE->insert( make_pair( ID, NEW ));
      }
      
    }
  }
  
}


//////////////////////////////////////////////////
// extract the overlapping profile data points
// and normalize by the total area:
void profile_scorer::EXTRACT_COMPARE_PROFILE_AREA(){
  
  COMPARE_AREA.clear();
  
  double TOT_PROT = 0;
  double TOT_TARGET = 0;
  map<int, double>::iterator P = TARGET_PROFILE.begin();
  while( P != TARGET_PROFILE.end() ){
    
    double data = 0.0;
    int ID = (*P).first;
    // get the rigth point
    map<int, double>::iterator F = IN_PROFILE->find( ID );
    if( F != IN_PROFILE->end() ){
      data = (*F).second;
    }

    TOT_PROT += data;
    TOT_TARGET += (*P).second;  
    
    ///////
    // create the struct:
    COMPARE_PROFILE_POINT tmp;
    tmp.ID = ID;
    tmp.TARGET = (*P).second;
    tmp.PROFILE = data;
    
    // insert
    COMPARE_AREA.push_back( tmp );
    P++;
  }
  
  /*
  /////////////////////////////////////
  // normalize each profile by the area:
  vector< COMPARE_PROFILE_POINT >::iterator C = COMPARE_AREA.begin();
  while( C != COMPARE_AREA.end() ){
    (*C).PROFILE /= fabs(TOT_PROT);
    (*C).TARGET /= fabs(TOT_TARGET);
    C++;
  }
  */
}



//////////////////////////////////////////////////
// compute the profile score
// score all the values which are not zero!!!
void profile_scorer::DEFINE_PROFILE_SCORES(){

  ALL_PROFILE_SCORE = 0;
  PROFILE_SCORE = 0.0;
  OUTSIDE_SCORE = 0.0;
  double PROFILE_NB_POINTS = 0;
  double OUTSIDE_NB_POINTS = 0;
  double ALL_PROFILE_NB_POINTS = 0;
  
  vector< COMPARE_PROFILE_POINT >::iterator C = COMPARE_AREA.begin();
  while( C != COMPARE_AREA.end() ){
    (*C).DELTA = fabs( (*C).PROFILE - (*C).TARGET );
    
    if( ( (*C).TARGET > 0.0 ) && ( (*C).PROFILE > 0.0 ) ) {
      PROFILE_SCORE += (*C).DELTA; 
      PROFILE_NB_POINTS++;
    }
    
    if( ( (*C).PROFILE == 0.0 ) || ( (*C).TARGET == 0.0 ) ){
      OUTSIDE_SCORE += (*C).DELTA;
      OUTSIDE_NB_POINTS++;
    }
    
    ALL_PROFILE_SCORE += (*C).DELTA;
    ALL_PROFILE_NB_POINTS++;
    
    C++;
  }
  
  if( ALL_PROFILE_NB_POINTS > 0 ){
    ALL_PROFILE_SCORE /= ALL_PROFILE_NB_POINTS;
  }
  
  if( PROFILE_NB_POINTS > 0 ){
    PROFILE_SCORE *= ALL_PROFILE_NB_POINTS;
    PROFILE_SCORE /= PROFILE_NB_POINTS;
  }

  if( OUTSIDE_NB_POINTS > 0 ){
    OUTSIDE_SCORE *= ALL_PROFILE_NB_POINTS;
    OUTSIDE_SCORE /= OUTSIDE_NB_POINTS;
  }
}

//////////////////////////////////////////////////
// get the total score:
double profile_scorer::get_TOTAL_SCORE(){
 
  double OUT = ALL_PROFILE_SCORE;  
  return OUT; 
}



/*

////////////////////////////////////////////////
// extract a feature profile::
void profile_scorer::EXTRACT_NORMALIZED_PROFILE( feature* IN ){
  
  simple_math MY_MATH;
  // extract the area:
  TARGET_PROFILE.clear();
  double area = 0;
  area = IN->get_peak_area();
  if( LOG_INTENSITY_TRANSFORMATION ){
    area = MY_MATH.log_tranform_F_int( area );
  }  
  TARGET_PROFILE.insert( make_pair( IN->get_spectrum_ID(), area ) ); 
  map<int,feature>::iterator FEA = IN->get_match_list_start();
  double TOT = area;
  while( FEA != IN->get_match_list_end() ){
    area = (*FEA).second.get_peak_area();
    if( LOG_INTENSITY_TRANSFORMATION ){
      area = MY_MATH.log_tranform_F_int( area );
    }  
    TARGET_PROFILE.insert( make_pair( (*FEA).second.get_spectrum_ID(), area ) );     
    TOT += area;
    FEA++;
  }
  
  // normalize the area:
  map<int, double>::iterator P = TARGET_PROFILE.begin();
  while( P != TARGET_PROFILE.end() ){
    (*P).second /= fabs(TOT);
    P++;
  }
  IN = NULL;
  TOT_SCORE = 0;
  PROFILE_SCORE = 0;
  OUTSIDE_SCORE = 0;
  ALL_PROFILE_SCORE = 0;  
  IN_PROFILE = &TARGET_PROFILE;
  INTERPOLATE_PROFILE();

  
}


*/

////////////////////////////////////////////////////
// print the compare area:
void profile_scorer::print_compare_AREA(){
  
  vector< COMPARE_PROFILE_POINT >::iterator C = COMPARE_AREA.begin();
  while( C != COMPARE_AREA.end() ){
    printf("%0.3f\t", (*C).TARGET );
    C++;
  }
  printf("\n" );

  C = COMPARE_AREA.begin();
  while( C != COMPARE_AREA.end() ){
    printf("%0.3f\t", (*C).PROFILE );
    C++;
  }
  printf("\n" );
  C = COMPARE_AREA.begin();
  while( C != COMPARE_AREA.end() ){
    printf("%0.3f\t", (*C).DELTA );
    C++;
  }
  printf("\n" );
  
  
}


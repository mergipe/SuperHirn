///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS consens_profile_builder:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_CONSENS_PROFILE_BUILDER
#include "CLUSTER_LC_MS_ANALYSIS_USE.h"

// modus to normalize feature/peptide/profiles
// 1: by manhatten distancs
// 2: by the max of teh profile
int consens_profile_builder::PROFILE_NORMALIZATION_MODE = 1;
bool consens_profile_builder::LOG_INTENSITY_TRANSFORMATION;
int consens_profile_builder::MAX_NB_PROILE_DATA_POINTS;

// parameter defines the replicate ids in a profile
// these points will be averaged and a condensed profile is creates:
map<int, vector<int> > consens_profile_builder::NB_REPLICATE_IDS;



////////////////////////////////////////////////
// constructor for the object consens_profile_builder:
consens_profile_builder::consens_profile_builder(){
}

////////////////////////////////////////////////
// constructor for the object consens_profile_builder:
consens_profile_builder::consens_profile_builder(int IN_nb_points){
  consens_profile_builder::MAX_NB_PROILE_DATA_POINTS = IN_nb_points;
}


//////////////////////////////////////////////////
// class desctructor of consens_profile_builder
consens_profile_builder::~consens_profile_builder(){
}

//////////////////////////////////////////////////
// class copy constructor of consens_profile_builder
consens_profile_builder::consens_profile_builder(const consens_profile_builder& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of consens_profile_builder
consens_profile_builder::consens_profile_builder(const consens_profile_builder* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
consens_profile_builder& consens_profile_builder::operator=(const consens_profile_builder& tmp){
  return *this;
}
 
//////////////////////////////////////////////////////////
// build protein profile consensus
map<int, double> consens_profile_builder::build_MEDIAN_CONSENS(map<double, feature*>* FEATURES){
  
  vector<feature*> TMP;
  map<double, feature*>::iterator F =  FEATURES->begin();
  while( F !=  FEATURES->end()){
    TMP.push_back( (*F).second );
    F++;
  }
  return build_MEDIAN_CONSENS( &TMP );
}

//////////////////////////////////////////////////////////
// build protein profile consensus
map<int, double> consens_profile_builder::build_MEDIAN_CONSENS(vector<feature*>* FEATURES){
  
  simple_math MY_MATH;
  map<int, double> MEDIAN_CONSENS;
  map<int, vector<double> > TMP_MEDIAN_CONSENS;  
  
  if( MAX_NB_PROILE_DATA_POINTS == -1 ){
    printf("\n\t\t WARNING!!! nb of profile data points not set in consens_profile_builder!\n");
    return MEDIAN_CONSENS;
  }
  
  vector<feature*>::iterator M = FEATURES->begin();
  while( M != FEATURES->end() ){
  
    //////////////////////////////
    // extract the data of the feature
    feature* FEA = (*M);
    map<int, double> THIS_DATA = build_feature_profile( FEA );
    
        
    ///////////////////////////////////////
    // go through the data points and insert them
    map<int, double>::iterator T = THIS_DATA.begin();
    while( T != THIS_DATA.end() ){
      
      int ID = (*T).first;
      double point = (*T).second;
     
      // see if already exists:      
      map<int, vector<double> >::iterator F_ID = TMP_MEDIAN_CONSENS.find( ID );
      if( F_ID == TMP_MEDIAN_CONSENS.end() ){
        vector<double> t;
        t.push_back(point);
        TMP_MEDIAN_CONSENS.insert( make_pair( ID, t) );
      }
      else{
        (*F_ID).second.push_back( point );
      }
      T++;
    }
    
    THIS_DATA.clear();
    
    M++;
  }
  
  /////////////////////////////////////////////
  // build the median and insert into the 
  // main CONSENS profile:
  map<int, vector<double> >::iterator THIS_P = TMP_MEDIAN_CONSENS.begin();
  map<int, double> END_PROFILE;
  
  while( THIS_P != TMP_MEDIAN_CONSENS.end() ){
    
    int P_ID = (*THIS_P).first;
    vector<double>* THIS_POINT = &((*THIS_P).second);
    
    //////////////////////////////////////////////////////
    // build here the median from the observed values at this LC/MS ID
    simple_math MY_MATH;
    double END_POINT = MY_MATH.MEDIAN( THIS_POINT );

    // and insert:
    MEDIAN_CONSENS.insert( make_pair( P_ID, END_POINT ) );
    THIS_P++;
  }  
  
  FEATURES = NULL;
  
  ///////////////////////////////////////
  // if the replicate profile number is defined
  // ( i.e. is bigger than 0 ), then make a 
  // condensed "replicate" profile
  // MEDIAN_CONSENS = createReplicateCondensedProfile( MEDIAN_CONSENS );  
  
  return normalize_profile( MEDIAN_CONSENS ); 
}




//////////////////////////////////////////////////////////
// build protein profile consensus
map<int, pair<double, double> > consens_profile_builder::build_AVERAGE_CONSENS(map<double, feature*>* FEATURES){

  vector<feature*> TMP;
  map<double, feature*>::iterator F =  FEATURES->begin();
  while( F !=  FEATURES->end()){
    TMP.push_back( (*F).second );
    F++;
  }
  return build_AVERAGE_CONSENS( &TMP );
}





//////////////////////////////////////////////////////////
// build profile of a feature
map<int, double> consens_profile_builder::build_feature_profile(feature* FEA){

  simple_math MY_MATH;
  map<int, double> PROFILE;

  if( MAX_NB_PROILE_DATA_POINTS == -1 ){
    printf("\n\t\t WARNING!!! nb of profile data points not set in consens_profile_builder!\n");
    return PROFILE;
  }
  
  double TOT_AREA = 0;
  for( int X = 0; X < MAX_NB_PROILE_DATA_POINTS; X++){
    
    double area = 0;    
    if( FEA->get_spectrum_ID() == X ){
      area = FEA->get_peak_area();      
    }
    else{
      // check the matches:
      map<int,feature>::iterator F = FEA->find_match_by_id(X);
      if( F != FEA->get_match_list_end()){
        area = (*F).second.get_peak_area();
      }
    }
    
    ///////////////////////////////////
    // Important:: make here a LOG 
    // transformation to the feature AREA:
    if( LOG_INTENSITY_TRANSFORMATION ){
      area = MY_MATH.log_tranform_F_int( area );
    }
    
    PROFILE.insert( make_pair( X, area ) );      
    TOT_AREA += area;
  }

  FEA = NULL;
  
  
  ///////////////////////////////////////
  // if the replicate profile number is defined
  // ( i.e. is bigger than 0 ), then make a 
  // condensed "replicate" profile
  PROFILE = createReplicateCondensedProfile( PROFILE );  
  
  // normalize and retrun
  return normalize_profile( PROFILE );
}
  



//////////////////////////////////////////////////////////
// build protein profile consensus
map< int, pair<double, double> > consens_profile_builder::build_AVERAGE_CONSENS( vector<feature*>* FEATURES){
  
  map<int, vector< double > > AVERAGE_CONSENS;
  
  if( MAX_NB_PROILE_DATA_POINTS == -1 ){
    printf("\n\t\t WARNING!!! nb of profile data points not set in consens_profile_builder!\n");
    map< int, pair<double, double> > TMP;
    return TMP;
  }
  
  
  for( int X = 0; X < MAX_NB_PROILE_DATA_POINTS; X++){
    vector< double > tmp;
    AVERAGE_CONSENS.insert( make_pair( X, tmp ) );
  }
  
  // go through the features:
  vector<feature*>::iterator M = FEATURES->begin();
  while( M != FEATURES->end() ){
    
    //////////////////////////////
    // extract the normalized profile of the feature
    feature* FEA = (*M);
    map<int, double> THIS_DATA_F = build_feature_profile( FEA );
    
    ///////////////////////////////////////
    // go through the data points and insert them
    map<int, double>::iterator T = THIS_DATA_F.begin();
    while( T != THIS_DATA_F.end() ){
      
      int ID = (*T).first;
      double point = (*T).second;
      
      map<int, vector< double > >::iterator F_ID = AVERAGE_CONSENS.find( ID );
      if( F_ID != AVERAGE_CONSENS.end() ){
        (*F_ID).second.push_back( point );
      }
      else{
        cout<<"*** PROBLEM IN consens_profile_builder::246 ***"<<endl;
      }
      T++;
    }
    
    THIS_DATA_F.clear();
    M++;
  }

  /////////////////////////////////////////////
  // build the median and stdev
  map< int, pair<double, double> > OUT_PROFILE;
  simple_math MY_MATH;
  map<int, vector< double > >::iterator THIS_P = AVERAGE_CONSENS.begin();  
  while( THIS_P != AVERAGE_CONSENS.end() ){
    
    pair<double, double> M;
    if( (*THIS_P).second.size() > 1 ){
      
      // build the mean:
      M = MY_MATH.AVERAGE_and_STDERROR( &((*THIS_P).second) );      
      // insert:
      OUT_PROFILE.insert( make_pair( (*THIS_P).first, M ) ); 
    }
    else if( ! (*THIS_P).second.empty() ){
      M = make_pair( ((*(*THIS_P).second.begin()) ), 0.0 );
      // insert:
      OUT_PROFILE.insert( make_pair( (*THIS_P).first, M ) ); 
    }
    
    
    THIS_P++;
  }  
  
  AVERAGE_CONSENS.clear();
  
  
  ///////////////////////////////////////
  // if the replicate profile number is defined
  // ( i.e. is bigger than 0 ), then make a 
  // condensed "replicate" profile
  //OUT_PROFILE = createReplicateCondensedProfile( OUT_PROFILE );  

  // normalize the profile and retur
  return normalize_profile( OUT_PROFILE ); 
}





//////////////////////////////////////////////////////////
// create a normalized profile:
map< int, double > consens_profile_builder::normalize_profile( vector<double> IN){
  
  // transform to map structure
  map<int, double> MY_P;
  int ID = 0;
  vector<double>::iterator M = IN.begin();
  while( M != IN.end() ){
    MY_P.insert( make_pair( ID, *M ) );
    M++;
    ID++;
  }
  
  // normalize:
  return normalize_profile( MY_P );
}


//////////////////////////////////////////////////////////
// create a normalized profile:
map< int, double > consens_profile_builder::normalize_profile( map<int, double> IN){
  
 
  // decide which profile normalization methode to use:
  switch( PROFILE_NORMALIZATION_MODE ){

    // by manhatten distance
    case 1:
      IN = normalize_profile_by_TotalSUM( IN );
      break;
      // by max of the profile
    case 2:
      IN = normalize_profile_by_MAX( IN );
      break;
    default:
      IN = normalize_profile_by_TotalSUM( IN );
  }
  
  return IN;
}


//////////////////////////////////////////////////////////
// create a normalized profile:
map< int, pair<double, double> > consens_profile_builder::normalize_profile( map<int, pair<double,double> > IN){
  // decide which profile normalization methode to use:
  switch( PROFILE_NORMALIZATION_MODE ){
    
   // by manhatten distance
    case 1:
      IN = normalize_profile_by_TotalSUM( IN );
      break;
      
      // by max of the profile
    case 2:
      IN = normalize_profile_by_MAX( IN );
      break;
    default:
      IN =  normalize_profile_by_TotalSUM( IN );
  }
  
   
  return IN;
}



  


//*********************************************************************
// here start different normalization functinalities

// normalize the profile by dividing by the total sum
// of y-values in the profile
map< int, double > consens_profile_builder::normalize_profile_by_TotalSUM( map<int, double> IN){
  
  map<int, double> NORM_OUT;
  map<int, double>::iterator P = IN.begin();
  double TOT = 0;
  
  while( P != IN.end() ){
    TOT += (*P).second;
    P++;
  }
  
  P = IN.begin();
  while( P != IN.end() ){
    NORM_OUT.insert( make_pair( (*P).first, ((*P).second / TOT ) ) ); 
    P++;
  }
  
  return NORM_OUT;
}


// normalize the profile by dividing by the total sum
// of y-values in the profile
map< int, pair<double, double> > consens_profile_builder::normalize_profile_by_TotalSUM( map<int, pair<double,double> > IN){
  
  map<int, pair<double,double> > NORM_OUT;
  map<int, pair<double,double> >::iterator P = IN.begin();
  double TOT = 0;
  
  while( P != IN.end() ){
    TOT += (*P).second.first;
    P++;
  }
  
  P = IN.begin();
  while( P != IN.end() ){
    pair<double,double> PAIR = make_pair( (*P).second.first / TOT , (*P).second.second / TOT );
    NORM_OUT.insert( make_pair( (*P).first, PAIR ) ); 
    P++;
  }
  
  return NORM_OUT;
}


// normalize the profile by dividing by the Max
// of y-values in the profile
map< int, double > consens_profile_builder::normalize_profile_by_MAX( map<int, double> IN){
  
  map<int, double> NORM_OUT;
  map<int, double>::iterator P = IN.begin();
  double MAX = (*P).second;
  P++;
  while( P != IN.end() ){
    if( MAX < (*P).second ){
      MAX = (*P).second;
    }
    P++;
  }
  
  P = IN.begin();
  while( P != IN.end() ){
    NORM_OUT.insert( make_pair( (*P).first, ((*P).second / MAX ) ) ); 
    P++;
  }
  
  return NORM_OUT;
}


// 2. normalize the profile by dividing by Max
// of y-values in the profile
map< int, pair<double, double> > consens_profile_builder::normalize_profile_by_MAX( map<int, pair<double,double> > IN){
  
  map<int, pair<double,double> > NORM_OUT;
  map<int, pair<double,double> >::iterator P = IN.begin();
  double MAX = (*P).second.first;
  P++;
  while( P != IN.end() ){
    if( MAX < (*P).second.first ){
      MAX = (*P).second.first;
    }
    P++;
  }
  
  P = IN.begin();
  while( P != IN.end() ){
    pair<double,double> PAIR = make_pair( (*P).second.first / MAX , (*P).second.second / MAX );
    NORM_OUT.insert( make_pair( (*P).first, PAIR ) ); 
    P++;
  }
  
  return NORM_OUT;
}



///////////////////////////////////////////////////////////////////////////////////////////
// if the replicate profile number is defined ( i.e. is bigger than 0 ), a 
// condensed "replicate" profile is constrcuted where every n points are replaced by their 
// mean ( n = NB_REPLICATE_POINTS)
map<int, double> consens_profile_builder::createReplicateCondensedProfile( map<int, double> IN ){
  
  if( NB_REPLICATE_IDS.empty() ){
    return IN;
  }
  else{
    
    map<int, double> repProfile;
    
    // go through all the replicates:
    map<int, vector<int> >::iterator P = NB_REPLICATE_IDS.begin();
    while( P != NB_REPLICATE_IDS.end() ){
      
      /////////////
      // go for one replicate set through all its ids
      // and collect the profile points
      vector<double> points;
      vector<int>::iterator R = (*P).second.begin();
      while( R != (*P).second.end() ){
        map<int, double>::iterator F = IN.find( *R);
        if(  F != IN.end() ){
          points.push_back( (*F).second );
        }
        R++; 
      }

      /////////////
      // now create stdev and insert into the replicate profile:
      simple_math myMath;
      pair<double, double> av = myMath.AVERAGE_and_STDEV( &points );
      // insert:
      repProfile.insert( make_pair( (*P).first , av.first ) ); 
      
      P++; 
    }
    
    IN.clear();
    return repProfile;
  }
}


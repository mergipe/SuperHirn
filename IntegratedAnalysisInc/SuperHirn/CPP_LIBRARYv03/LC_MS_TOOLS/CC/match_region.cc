///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS match_region:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_MATCH_REGION
#define USE_MATCH_REGION_CANDIDATE

#include "LC_MS_TOOLS_USE.h"



////////////////////////////////////////////////
// constructor for the object match_region:
match_region::match_region(){
  ms2Correlator = NULL;
}

////////////////////////////////////////////////
// constructor for the object match_region:
match_region::match_region(int CHRG, double IN_Mz, double TB, double T_low, double T_high){
  chrg = CHRG;
  Mz = IN_Mz;
  Tr = TB;
  T_LOW = T_low;
  T_HIGH = T_high;
  TARGET = NULL;
  ms2Correlator = NULL;
}

////////////////////////////////////////////////
// constructor for the object match_region:
match_region::match_region( feature* in){
  TARGET = in;
  chrg = TARGET->get_charge_state();
  Mz = TARGET->get_MZ();
  Tr = TARGET->get_retention_time();
  T_LOW = Tr - TARGET->get_alignment_error_down();
  T_HIGH = Tr + TARGET->get_alignment_error_up();
  ms2Correlator = NULL;
  
  
}


//////////////////////////////////////////////////
// class desctructor of match_region
match_region::~match_region(){

  CANDIDATE_LIST.clear();
  BEST_CANDIDATE.clear();
  TARGET = NULL;
  
  if( ms2Correlator != NULL ){
    delete ms2Correlator;
    ms2Correlator = NULL;
  }
}

//////////////////////////////////////////////////
// class copy constructor of match_region
match_region::match_region(const match_region& tmp){
  
  chrg = tmp.chrg;
  Mz = tmp.Mz;
  Tr = tmp.Tr;
  T_LOW = tmp.T_LOW;
  T_HIGH = tmp.T_HIGH;
  CANDIDATE_LIST = tmp.CANDIDATE_LIST;
  BEST_CANDIDATE = tmp.BEST_CANDIDATE;
  TARGET = tmp.TARGET;
  ms2Correlator = NULL;

  // ms2Correlator = tmp.ms2Correlator;
}

//////////////////////////////////////////////////
// class copy constructor of match_region
match_region::match_region(const match_region* tmp){
  chrg = tmp->chrg;
  Mz = tmp->Mz;
  Tr = tmp->Tr;
  T_LOW = tmp->T_LOW;
  T_HIGH = tmp->T_HIGH;
  CANDIDATE_LIST = tmp->CANDIDATE_LIST;
  BEST_CANDIDATE = tmp->BEST_CANDIDATE;
  TARGET = tmp->TARGET;
  ms2Correlator = NULL;
  // ms2Correlator = tmp->ms2Correlator;
}


//////////////////////////////////////////////////
// copy constructor:
match_region& match_region::operator=(const match_region& tmp){
  chrg = tmp.chrg;
  Mz = tmp.Mz;
  Tr = tmp.Tr;
  T_LOW = tmp.T_LOW;
  T_HIGH = tmp.T_HIGH;
  CANDIDATE_LIST = tmp.CANDIDATE_LIST;
  BEST_CANDIDATE = tmp.BEST_CANDIDATE;
  TARGET = tmp.TARGET;
  ms2Correlator = NULL;

  // ms2Correlator = tmp.ms2Correlator;
  return *this;
}


//////////////////////////////////////////////////
// check if this fetaure is a candidate for the 
// target feature
void match_region::checkFeatureCandidate(feature* IN ){
  
  //////////////////////////////////////////////
  // check the charge state:
  // for feature which originate from MS2 traces
  // the charge state can not be determined (is -1)
  // in these case ignore the charge state for matching:
  bool ChargeCheck = false;
  // in case on feature has z == -1:
  if( (IN->get_charge_state() < 0) || (chrg < 0) ){
      ChargeCheck = true;
  }
  else{
    // otherwise compare the values:
    if( IN->get_charge_state()  == chrg ){
      ChargeCheck = true;
    }
  }    
  
  if( ChargeCheck ){
    
    ///////////////////////////////////////
    // check if mz with in tolerance range:
    if( feature::compareFeatureMassValuesAtPPMLevel( Mz , IN->get_MZ() )){
      
      ///////////////////////////////////////
      // check if within the retention time window:
      if( (IN->get_retention_time() >= T_LOW) && (IN->get_retention_time() <= T_HIGH) ){
        // add this lc_peak to the region: 
        add_match_candidate( IN );
      }
    }
  }
}


//////////////////////////////////////////////////
// adds a candidate to the lc_peak list: 
void match_region::add_match_candidate(feature* in){
  CANDIDATE* tmp = new match_region_candidate( in );
  CANDIDATE_LIST.push_back( tmp );
  delete tmp;
  tmp = NULL;
}

//////////////////////////////////////////////////
// match the input feature from LC_MS run B against the extracted region:
// m/z has already been compared, check now which one is closest to the peak
// and compare also if you can find similar charge states peptides
void match_region::find_best_match( ){
  find_best_match( &(*TARGET) );
}

//////////////////////////////////////////////////
// match the input feature from LC_MS run B against the extracted region:
// m/z has already been compared, check now which one is closest to the peak
// and compare also if you can find similar charge states peptides
void match_region::find_best_match(feature* TARGET){
  
  /*
   bool print = false;
  if( TARGET->get_MS2_info() ){
    if( TARGET->get_SQ() == "VVNSTTGPGEHLR" ){
      TARGET->show_info();
      print = true;
    }
  }
*/
  
 // bool print = true;
/*
  if( ( TARGET->get_MZ() >= 456.565) && ( TARGET->get_MZ() <= 456.575) ){
      TARGET->show_info();
      print = true;
  }
  */
  /* 
   
  if( ( CANDIDATE_LIST.size() == 1 ) ){
    CANDIDATE THIS_F = *CANDIDATE_LIST.begin();
    double tmpCor = getMS2SpectraCorrelation(THIS_F.myFeature );
  }
  
   
   */

  
  // store possible candidates in candidate list:
  BEST_CANDIDATE = CANDIDATE_LIST;
  
  double target_TR = TARGET->get_retention_time();
  double target_MZ = TARGET->get_MZ();
  double target_AREA = TARGET->get_peak_area();
  double diff_TR_min = 100000;
  double diff_AREA_min = 100000000;
  double diff_MZ_min = 1000;
  double targetCorrelation = -10;
  bool MS2CorrelationCheck = true;
  
  //////////////////////////////////////////////////////////////
  // find the one with minimal mZ difference:
    
  vector<CANDIDATE>::iterator P = BEST_CANDIDATE.begin();
  while( (P != BEST_CANDIDATE.end()) && ( BEST_CANDIDATE.size() > 1 ) ){
  //while( P != BEST_CANDIDATE.end()){
      
    CANDIDATE* cand = &(*P);
    
  
    //if( cand->myFeature->get_feature_ID() == 69 ){
    // cand->myFeature->show_info();
    
    
    /////////////////////////////////////
    // compute a spectra correlation:
    cand->ms2Correlation = getMS2SpectraCorrelation( cand->myFeature );
    if(cand->ms2Correlation == -10 ){
      MS2CorrelationCheck = false;
    }
    else if( cand->ms2Correlation > targetCorrelation ){
      targetCorrelation = cand->ms2Correlation;
    }
    
    
    
    //////////////////////////////////////////////
    // find mins in TR / AREA:
    cand->trDiff = fabs( target_TR - cand->myFeature->get_retention_time());		
    // check for the ones with smallest TR differeneces:
    if(cand->trDiff < diff_TR_min){
      diff_TR_min = cand->trDiff;
    }    

    cand->areaDiff = fabs( target_AREA - cand->myFeature->get_peak_area());		
    // check for the ones with smallest AREA differeneces:
    if(cand->areaDiff < diff_AREA_min){
      diff_AREA_min = cand->areaDiff;
    }

    cand->mzDiff = fabs( target_MZ - cand->myFeature->get_MZ());		
    // check for the ones with smallest AREA differeneces:
    if(cand->mzDiff < diff_MZ_min){
      diff_MZ_min = cand->mzDiff;
    }
    
    P++;
  }
  

  
  /////////////////////////////////////////////////////////////////
  // loop now through the ones saved and find minimas:

  // check for the ones with smallest MS2 spectrum correlation:
  if( MS2CorrelationCheck ){
    
    P = BEST_CANDIDATE.begin();
    while( (P != BEST_CANDIDATE.end()) && ( BEST_CANDIDATE.size() > 1 ) ){
      
      CANDIDATE* cand = &(*P);
      if(cand->ms2Correlation < targetCorrelation){
        P = BEST_CANDIDATE.erase( P );
      }
      else{
        P++;
      }
    }
  }
  

  
  // check for the ones with smallest TR diff.:
  P = BEST_CANDIDATE.begin();
  while( (P != BEST_CANDIDATE.end()) && ( BEST_CANDIDATE.size() > 1 ) ){
    
    CANDIDATE* cand = &(*P);
    if(cand->trDiff > diff_TR_min){
      P = BEST_CANDIDATE.erase( P );
    }
    else{
      P++;
    }
  }

  // check for the ones with smallest MZ diff.:
  P = BEST_CANDIDATE.begin();
  while( (P != BEST_CANDIDATE.end()) && (BEST_CANDIDATE.size() > 1)){
    
    CANDIDATE* cand = &(*P);
    if( cand->mzDiff > diff_MZ_min ){
      P = BEST_CANDIDATE.erase( P );
    }
    else{
      P++;
    }
  }
  
  // check for the ones with smallest AREA diff.:
  P = BEST_CANDIDATE.begin();
  while( (P != BEST_CANDIDATE.end()) && ( BEST_CANDIDATE.size() > 1 ) ){
    
    CANDIDATE* cand = &(*P);
    // check for the ones with bigger TR differeneces:
    if(cand->areaDiff > diff_AREA_min){
      P = BEST_CANDIDATE.erase( P );
    }
    else{
      P++;
    }
  }
  

  /*
  if( print ) {
  
    feature* f =  get_best_match();
    if ( f != NULL ){
      f->show_info();
      cout<<endl;
    }
  }
  */
  
 }


///////////////////////////////////////////////////
// compare the charge state pattern of 2 features:
int match_region::compare_charge_states(feature* TARGET, feature* B){
  int score = 0;
  return score;
}


///////////////////////////////////////////////////
// remove a candidate from the list:
void match_region::remove_candidate( feature* IN ){
  
  if( IN != NULL ){
    
    int TARGET_ID = IN->get_feature_ID();
    // bool check = false;
    vector<CANDIDATE>::iterator C = CANDIDATE_LIST.begin();
    while( C != CANDIDATE_LIST.end() ){
      
      if( (*C).myFeature->get_feature_ID() == TARGET_ID ){
        CANDIDATE_LIST.erase( C );
        //check = true;
        break;
      }
      C++;
    }
    
    C = BEST_CANDIDATE.begin();
    while( C != BEST_CANDIDATE.end() ){
      
      if( (*C).myFeature->get_feature_ID() == TARGET_ID ){
        BEST_CANDIDATE.erase( C );
        //check = true;
        break;
      }
      C++;
    }
    
    /*
    if( !check ){
      cout<<CANDIDATE_LIST.size()<<"NOOOOTTTT"<<BEST_CANDIDATE.size()<<endl;
    }
     */
    
    IN = NULL;
  }
 
}

///////////////////////////////////////////////////
// removes the best match from the list:
void match_region::remove_best_match(  ){
  
  if( !BEST_CANDIDATE.empty() ){
    
    int TARGET_ID = get_best_match()->get_feature_ID();
    BEST_CANDIDATE.erase( BEST_CANDIDATE.begin() );
    //bool check = false;
    vector<CANDIDATE>::iterator C = CANDIDATE_LIST.begin();
    while( C != CANDIDATE_LIST.end() ){
      
      if( (*C).myFeature->get_feature_ID() == TARGET_ID ){
        CANDIDATE_LIST.erase( C );
        break;
      }      
      C++;
    }
    
    /*
    if( !check ){
      cout<<CANDIDATE_LIST.size()<<"NOOOOTTTT"<<BEST_CANDIDATE.size()<<endl;
    }
     */
  }
}




/////////////////////////////////////////////////////////
// compare the two best matches of this to another 
// match region and decided who should keep it:
void match_region::compareBestCandidates( match_region* compare){ 
  
  feature* BEST_MATCH = compare->get_best_match();
  feature* TARGET_A = compare->get_TARGET();
  feature* TARGET_B = this->get_TARGET();

  
  /////////////////////////////////////////
  // decide which match region had the 
  // wrong best candidate:
  match_region* LOOSER = NULL;

  
  // debugging:
  
  /*
   bool debug = false;
  if( ( BEST_MATCH->get_feature_ID() == 416) ||
    ( TARGET_A->get_feature_ID() == 0 && TARGET_A->get_spectrum_ID() == 2 ) ||
      ( TARGET_B->get_feature_ID() == 0 && TARGET_B->get_spectrum_ID() == 2 )){
    cout<<endl;
    BEST_MATCH->show_info();
    TARGET_A->show_info();
    TARGET_B->show_info();
    debug = true;
  }
   */

  // compare first via MS2 consensus spectra:
  // compute a spectra correlation:
  
  if( TARGET_A->getMS2Feature() != NULL 
      && TARGET_B->getMS2Feature() != NULL
      && BEST_MATCH->getMS2Feature() != NULL ){
    
    double corA = getMS2SpectraCorrelation( BEST_MATCH, TARGET_A );
    double corB = getMS2SpectraCorrelation( BEST_MATCH, TARGET_B );
    
    /*
    if( debug ){
      cout<<"A: "<<corA<<endl;
      cout<<"B: "<<corB<<endl;
    }
    */
    
    if( corA > corB ){
      LOOSER = this;
    }
    else if( corA < corB ){
      LOOSER = compare;
    }
    
  }
  
  
  /////////////////////////////////////////
  // if not determined yet, find best via 
  // smallest retention time difference
  if( LOOSER == NULL ){
    double delta_TR_A = fabs( TARGET_A->get_retention_time() - BEST_MATCH->get_retention_time() );
    double delta_TR_B = fabs( TARGET_B->get_retention_time() - BEST_MATCH->get_retention_time() );
    if( delta_TR_A < delta_TR_B ){
      LOOSER = this;
    }
    else{
      LOOSER = compare;
    }
  }
  
  //LOOSER->get_TARGET()->show_info();

  
  /////////////////////////////////////////
  // process the looser match region
  // 1. remove the best match
  // 2. find another best match ( if there is ); 
  // LOOSER->remove_candidate( BEST_MATCH );
  /*
  if( debug  ){
    cout<<"Looser"<<endl;
    LOOSER->get_TARGET()->show_info();
  }
  */
  
  LOOSER->remove_best_match( );
  if( LOOSER->get_nb_candidates() > 0 ){
    LOOSER->find_best_match( );
  }
  
  LOOSER = NULL;
  compare = NULL;
}




///////////////////////////////////////////////////
// compute the MS2-MS2 spectra correlation
double match_region::getMS2SpectraCorrelation( feature* check ){
  
  
  
  
  
  
  
  
  /*
  if( ( check->get_feature_ID() == 106 && check->get_spectrum_ID() == 5  ) &&
      ( TARGET->get_feature_ID() == 157 && TARGET->get_spectrum_ID() == 6 ) ){
    cout<<endl;
  }
  
  double out2= getAlignedMS2SpectraCorrelation( TARGET, check );
  // now over the aligned features:
  map<int,feature>::iterator T = TARGET->get_match_list_start();
  while( T != TARGET->get_match_list_end() ){
    
    // check feature
    feature* alignedTarget = &(T->second);
    out2 = getAlignedMS2SpectraCorrelation( alignedTarget, check );
    T++;
  }
  
  out2 /= (double) TARGET->get_replicate_match_nb();
  */
  
  double out = getMS2SpectraCorrelation( TARGET, check );
  
  /*
  if( ( TARGET->get_feature_ID() == 106 && TARGET->get_spectrum_ID() == 5  ) &&
      ( TARGET->get_feature_ID() == 157 && TARGET->get_spectrum_ID() == 6 ) ){
    
    cout<<"score: "<<out<<endl;
    TARGET->show_info();
    check->show_info();
    
  
     cout<<endl<<"overlap correlation: "<<out->getOverlapCorrelation()<<endl;
     cout<<"intens correlation: "<<ms2Correlator->getIntensityCorrelation()<<endl;
     cout<<"main correlation: "<<ms2Correlator->getMainCorrelation()<<endl;
     cout<<"overlap: "<<ms2Correlator->getPeakOverlap()<<endl;
     TARGET->show_info();
     check->show_info();
     
    
  }
*/
  
  return out;
}

///////////////////////////////////////////////////
// compute the aligned MS2-MS2 spectra correlation between two features:
double match_region::getAlignedMS2SpectraCorrelation( feature* target, feature* check ){
  
  double mean = 0;
  
  MS2CorrelationAnalysis* out = getMS2SpectraCorrelationOfTwoFeatures( target, check );
  if( out != NULL ){
    mean += out->mainCorrelationScore;
  }
  
  map<int,feature>::iterator T = check->get_match_list_start();
  while( T != check->get_match_list_end() ){
    
    // check feature
    feature* alignedCheck = &(T->second);
    
    out = getMS2SpectraCorrelationOfTwoFeatures(  target, alignedCheck );
    if( out != NULL ){
      mean += out->mainCorrelationScore;
    }  
    T++;
  }

  return mean / (double) check->get_replicate_match_nb();
}


///////////////////////////////////////////////////
// compute the MS2-MS2 spectra correlation between two features:
double match_region::getMS2SpectraCorrelation( feature* target, feature* check ){
  
  MS2CorrelationAnalysis* out = getMS2SpectraCorrelationOfTwoFeatures( check, target );
  if( out == NULL ){
    return -10;
  }
  else{
    return out->mainCorrelationScore;
  }
}


///////////////////////////////////////////////////
// compute the MS2-MS2 spectra correlation between two features:
MS2CorrelationAnalysis* match_region::getMS2SpectraCorrelationOfTwoFeatures( feature* check , feature* target ){
  

  // do only if both have a spectra of course:
  if( ( target->getMS2Feature() != NULL) && ( check->getMS2Feature() != NULL) ){
    
    MS2SpectraCorrelator* myMS2Correlator = new MS2SpectraCorrelator( target->getMS2Feature() );    
    // correlation of the MS1 features by comparing the 
    // ms2 consensus spectra:
    myMS2Correlator->computeMS2SpectrumCorrelation( check->getMS2Feature() );
    return myMS2Correlator->getMainAnalysis( );

  }
   
  
  return NULL;
}



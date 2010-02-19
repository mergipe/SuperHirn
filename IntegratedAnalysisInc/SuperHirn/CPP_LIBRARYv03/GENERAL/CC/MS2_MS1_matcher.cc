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

#define USE_MS2_MS1_MATCHER
#include "GENERAL_USE.h"

double MS2_MS1_matcher::PEP_PROB_CUT_OFF;
int MS2_MS1_matcher::POST_SCAN_TOL;
int MS2_MS1_matcher::SCAN_TOL;

#ifdef DEBUG
bool MS2_MS1_matcher::NON_MATCH_VIEW = false;
#else
bool MS2_MS1_matcher::NON_MATCH_VIEW = false;
#endif

int MS2_MS1_matcher::AM_APEX_COUNTER = 0;
int MS2_MS1_matcher::AM_COUNTER = 0;


////////////////////////////////////////////////
// constructor for the object MS2_MS1_matcher:
MS2_MS1_matcher::MS2_MS1_matcher( ){
  MS2_SCAN = NULL;
  POST_ANNOTATION = false;
  REQUIRED_LC_MS_ID = -1;
  highQual_specific_MS2_counter = 0;
  highQual_specific_MS2_MS1_assignments = 0;
  specific_MS2_counter = 0;
  specific_MS2_MS1_assignments = 0;
}

//////////////////////////////////////////////////
// class desctructor of MS2_MS1_matcher
MS2_MS1_matcher::~MS2_MS1_matcher(){
  MS2_SCAN = NULL;
  POST_ANNOTATION = false;
  MS2_CANDIDATE_LIST.clear();
  MISSED_MS2.clear();
}

//////////////////////////////////////////////////
// class copy constructor of MS2_MS1_matcher
MS2_MS1_matcher::MS2_MS1_matcher(const MS2_MS1_matcher& tmp){
  MS2_SCAN = tmp.MS2_SCAN;
  POST_ANNOTATION = tmp.POST_ANNOTATION; 
  MS2_CANDIDATE_LIST = tmp.MS2_CANDIDATE_LIST;
  specific_MS2_counter = tmp.specific_MS2_counter;
  specific_MS2_MS1_assignments = tmp.specific_MS2_MS1_assignments;
  highQual_specific_MS2_counter = tmp.specific_MS2_counter;
  highQual_specific_MS2_MS1_assignments = tmp.specific_MS2_MS1_assignments;
}

//////////////////////////////////////////////////
// class copy constructor of MS2_MS1_matcher
MS2_MS1_matcher::MS2_MS1_matcher(const MS2_MS1_matcher* tmp){
  MS2_SCAN = tmp->MS2_SCAN;
  POST_ANNOTATION = tmp->POST_ANNOTATION; 
  MS2_CANDIDATE_LIST = tmp->MS2_CANDIDATE_LIST;
  specific_MS2_counter = tmp->specific_MS2_counter;
  specific_MS2_MS1_assignments = tmp->specific_MS2_MS1_assignments;
  highQual_specific_MS2_counter = tmp->specific_MS2_counter;
  highQual_specific_MS2_MS1_assignments = tmp->specific_MS2_MS1_assignments;
}


//////////////////////////////////////////////////
// copy constructor:
MS2_MS1_matcher& MS2_MS1_matcher::operator=(const MS2_MS1_matcher& tmp){
  MS2_SCAN = tmp.MS2_SCAN;
  POST_ANNOTATION = tmp.POST_ANNOTATION; 
  MS2_CANDIDATE_LIST = tmp.MS2_CANDIDATE_LIST;
  specific_MS2_counter = tmp.specific_MS2_counter;
  specific_MS2_MS1_assignments = tmp.specific_MS2_MS1_assignments;
  highQual_specific_MS2_counter = tmp.specific_MS2_counter;
  highQual_specific_MS2_MS1_assignments = tmp.specific_MS2_MS1_assignments;
  return *this;
}

//////////////////////////////////////////////////
// adds a candidate to the lc_peak list: 
void MS2_MS1_matcher::add_match_candidate(MS2_CANDIDATE tmp){
  MS2_CANDIDATE_LIST.push_back( tmp );
}



//////////////////////////////////////////////////
// function which assigns accquired MS2 scans
// of a LC/MS run into its corresponding MS1 features
void MS2_MS1_matcher::combine_LC_MS_specific_MS2_to_MS1_data( LC_MS* IN, ms2_info* INFO ){
  
  
  if( ( IN ) && ( INFO ) ){
    
    MS2_SCAN = INFO;
    
    specific_MS2_counter++;
    
    if( INFO->get_PEP_PROB() >= PEP_PROB_CUT_OFF){
      highQual_specific_MS2_counter++;
    }
    
    
    // set the LC/MS ID fro which the ms2 scan was accquired:
    set_MS2_scan_LCMS_ID( IN );
    
    // set the correct scan tolerance:
    POST_ANNOTATION = false;
    // matches a MS2-info to a feature within a list of features
    match_MS2_info_to_feature( IN );
    
    MS2_SCAN = NULL;
    INFO = NULL;
  }
  
}

//////////////////////////////////////////////////
// function which assigns MS2 scans accquired from 
// other LC/MS runs into MS1 features
bool MS2_MS1_matcher::combine_POSTANNOTATION_MS2_to_MS1_data( vector<feature*>* IN, ms2_info* INFO ){
  
  if( ( !(IN->empty()) ) && ( INFO ) ){
    
    REQUIRED_LC_MS_ID = -1;
    MS2_SCAN = INFO;
    // set the IL tag:
    MS2_SCAN->set_MS2_TYPE_TAG("IL");
    // set the correct scan tolerance:
    POST_ANNOTATION = true;
    
    
    // matches a MS2-info to a feature within a list of features :-)
    return match_MS2_info_to_feature( IN );
 }
  return false;
  
}



//////////////////////////////////////////////////
// match the input feature from LC_MS run B against the extracted region:
// m/z has already been compared, check now which one is closest to the peak
// and compare also if you can find similar charge states peptides
void MS2_MS1_matcher::find_best_match(){

  // increase the specific MS2 to M1 assignment counter:
  specific_MS2_MS1_assignments++;
  if( MS2_SCAN->get_PEP_PROB() >= PEP_PROB_CUT_OFF){
    highQual_specific_MS2_MS1_assignments++;
  }
  
  
  if( MS2_CANDIDATE_LIST.size() == 1 ){
    (*(MS2_CANDIDATE_LIST.begin()))->add_MS2_info( MS2_SCAN );    
  }
  else if( !MS2_CANDIDATE_LIST.empty() ){
  
    bool MSMSScanCheck = false;
    int target_SCAN = MS2_SCAN->get_SCAN();
    double target_precursor_MZ = MS2_SCAN->get_MONO_MZ();
    double target_theoretical_MZ = MS2_SCAN->get_THEO_MZ();
    int MIN_SCAN_DIFF = 1000000000;
    double MIN_PRE_MZ_DIFF = 100000000;
    double MIN_THEO_MZ_DIFF = 1000000000;
    
    //////////////////////////////////////////////////////////////
    // find the one with minimal scan difference:    
    vector<MS2_CANDIDATE>::iterator P = MS2_CANDIDATE_LIST.begin();
    while(P != MS2_CANDIDATE_LIST.end()){
    
      feature* THIS_P = (*P);
      int SCAN_DIFF = int( fabs( double( target_SCAN - THIS_P->get_scan_number() ) ) );	
      double PRE_MZ_DIFF = fabs( target_precursor_MZ - THIS_P->get_MZ() );		
      double THEO_MZ_DIFF = fabs( target_theoretical_MZ - THIS_P->get_MZ() );		

      //////////////////////////////////////////////
      // check for the ones with smallest scan differeneces:
      if( SCAN_DIFF < MIN_SCAN_DIFF){
        MIN_SCAN_DIFF = SCAN_DIFF;
      }
      //////////////////////////////////////////////
      
      
      //////////////////////////////////////////////
      // check for the smallest prcursor differeneces:
      if( PRE_MZ_DIFF < MIN_PRE_MZ_DIFF){
        MIN_PRE_MZ_DIFF = PRE_MZ_DIFF;
      }

      // check for the smallest prcursor differeneces:
      if( THEO_MZ_DIFF < MIN_THEO_MZ_DIFF){
        MIN_THEO_MZ_DIFF = THEO_MZ_DIFF;
      }
      //////////////////////////////////////////////
      
      
      
      
      ///////////////////////////////////////////////////////////////
      // check with MS/MS scan if this feature has a MS/MS feature
      if( THIS_P->getMS2Feature() != NULL ){
        MS2_feature* ms2F = THIS_P->getMS2Feature();
        if( ms2F->getApexScan() == MS2_SCAN->get_SCAN_START() ){
          MSMSScanCheck = true;
        }
      }
      ///////////////////////////////////////////////////////////////
      
      
      P++;
    }
    
    
    /////////////////////////////////////////////////////////////////
    // check for precise match in case a MS/MS conseuns feature is avaiable: 
    if( MSMSScanCheck ){
      P = MS2_CANDIDATE_LIST.begin();
      while( (P != MS2_CANDIDATE_LIST.end()) && (MS2_CANDIDATE_LIST.size() > 1)){
        
        feature* THIS_P = (*P);
        bool del = true;
        if( THIS_P->getMS2Feature() != NULL ){
          MS2_feature* ms2F = THIS_P->getMS2Feature();
          if( ms2F->getApexScan() != MS2_SCAN->get_SCAN_START() ){
            del = false;
          }
        }
        
        if( del ){
          P = MS2_CANDIDATE_LIST.erase(P);
        }
        else{
          P++;
        }
      }   
    }
    
    
    
    /////////////////////////////////////////////////////////////////
    // loop now through the ones saved and find minimas:
    
    //////////////////////////////////////////////
    // check for the ones closest to apex:
    P = MS2_CANDIDATE_LIST.begin();
    while( (P != MS2_CANDIDATE_LIST.end()) && (MS2_CANDIDATE_LIST.size() > 1)){
      
      feature* THIS_P = (*P);
      
      AM_APEX_COUNTER++;
        
      int tmp = int( fabs( double( target_SCAN - THIS_P->get_scan_number() ) ) );
      if( tmp > MIN_SCAN_DIFF ){
        P = MS2_CANDIDATE_LIST.erase(P);
      }
      else{
        P++;
      }
      
    }
    
    if( MS2_CANDIDATE_LIST.size() > 1 ){
      AM_COUNTER++;
    }
    
    
    
    //////////////////////////////////////////////
    // check for the ones with smallest MZ diff.:
    P = MS2_CANDIDATE_LIST.begin();
    while( (P != MS2_CANDIDATE_LIST.end()) && (MS2_CANDIDATE_LIST.size() > 1)){
      
      feature* THIS_P = (*P);
      
      if( ms2_info::THEO_MATCH_MODUS ){
        double tmp = fabs( target_theoretical_MZ - THIS_P->get_MZ() );
        if( tmp > MIN_THEO_MZ_DIFF ){
          P = MS2_CANDIDATE_LIST.erase(P);
        }
        else{
          P++;
        }
      }
      else{
        double tmp = fabs( target_precursor_MZ - THIS_P->get_MZ() );
        if( tmp > MIN_PRE_MZ_DIFF ){
          P = MS2_CANDIDATE_LIST.erase(P);
        }
        else{
          P++;
        }
        
      }
    }
    
        
    
    // add MS2 info to the remaining ones:
    P = MS2_CANDIDATE_LIST.begin();
    while( P != MS2_CANDIDATE_LIST.end() ){
      // add the info:
      (*P)->add_MS2_info( MS2_SCAN );
      P++;
    } 
  }
  
  MS2_CANDIDATE_LIST.clear();
}



//////////////////////////////////////////////////////////////////////////////////////////////
// matches a MS2-info to a feature within a list of features :-)
void MS2_MS1_matcher::match_MS2_info_to_feature( LC_MS* IN ){
  
  MS2_CANDIDATE_LIST.clear();
  
  // compare now the m/z
  double MZ = 0;
  // check if theoretical mass
  // or precursor mass used for matching
  if( ms2_info::THEO_MATCH_MODUS ){
    MZ = MS2_SCAN->get_THEO_MZ();
  }
  else{
    MZ = MS2_SCAN->get_MONO_MZ();
  }
  double MAX_MZ = MZ + simple_math::getMassErrorAtPPMLevel( MZ, 10.0 * ms2_info::MS2_MZ_PPM_TOLERANCE );
  
  
  // search now the original peak:
  vector<LC_MS_FEATURE>::iterator P = IN->get_feature_list_begin();
  while( P != IN->get_feature_list_end() ){
    
    feature* SEARCH = &(*P);
    
    if( SEARCH->get_MZ() > MAX_MZ ){
      break;
    }
    
    if( ( REQUIRED_LC_MS_ID < 0 ) || ( SEARCH->get_spectrum_ID() == REQUIRED_LC_MS_ID) ){ 
      // check if the ms2 scan possible can fit to the feature:
      if( checkMS12MS2Combination( MS2_SCAN, SEARCH ) ){
        // select as TARGETS:
        MS2_CANDIDATE_LIST.push_back( SEARCH );
      }
    }
    
    P++;
  }
  
  // print if MS2 info could not be added to any peak:
  if( MS2_CANDIDATE_LIST.empty() ){
    if( MS2_SCAN->get_PEP_PROB() >= PEP_PROB_CUT_OFF ){
      MISSED_MS2.push_back( MS2_SCAN );
    }
  }
  else{
    find_best_match();
    MS2_CANDIDATE_LIST.clear();
  }
  
}

//////////////////////////////////////////////////////////////////////////////////////////////
// matches a MS2-info to a feature within a list of features :-)
bool MS2_MS1_matcher::match_MS2_info_to_feature( vector< feature*>* ALL_MS1_FEATURES ){
  
  feature* SEARCH = NULL;
  MS2_CANDIDATE_LIST.clear();
  
  // compare now the m/z
  double MZ = 0;
  // check if theoretical mass
  // or precursor mass used for matching
  if( ms2_info::THEO_MATCH_MODUS ){
    MZ = MS2_SCAN->get_THEO_MZ();
  }
  else{
    MZ = MS2_SCAN->get_MONO_MZ();
  }
  double MAX_MZ = MZ + simple_math::getMassErrorAtPPMLevel( MZ, 10.0 * ms2_info::MS2_MZ_PPM_TOLERANCE );
  
  // search now the original peak:
  vector<feature*>::iterator P = ALL_MS1_FEATURES->begin();
  while( P != ALL_MS1_FEATURES->end() ){
    
    SEARCH = (*P);
    
    if( SEARCH->get_MZ() > MAX_MZ ){
      break;
    }
  
    // check if the ms2 scan possible can fit to the feature:
    if( checkMS12MS2Combination( MS2_SCAN, SEARCH ) ){
      // select as TARGETS:
      MS2_CANDIDATE_LIST.push_back( SEARCH );
    }
  
    P++;
  }
  
  // print if MS2 info could not be added to any peak:
  if( MS2_CANDIDATE_LIST.empty() ){
    if( MS2_SCAN->get_PEP_PROB() >= PEP_PROB_CUT_OFF ){
      MISSED_MS2.push_back( MS2_SCAN );
    } 
    return false;
  }
  else{
    find_best_match();
    MS2_CANDIDATE_LIST.clear();
    return true;
  }
  return false;
}


///////////////////////////////////////////////////////////////////
// set the LC/MS ID fro which the ms2 scan was accquired:
void MS2_MS1_matcher::set_MS2_scan_LCMS_ID( LC_MS* IN ){  
  
  REQUIRED_LC_MS_ID = -1;
  string THIS_RUN = MS2_SCAN->get_ORIGINAL_INTERACT_FILE();
  
  if( THIS_RUN.find( "_" ) != string::npos ){
    THIS_RUN = THIS_RUN.erase( THIS_RUN.find("_"),THIS_RUN.size() - THIS_RUN.find("_"));
  }
  
  if( IN->get_spec_name().find( THIS_RUN ) != string::npos ){
    REQUIRED_LC_MS_ID = IN->get_spectrum_ID();
  }
  
  map<int, string>::iterator LCMS = IN->get_raw_spec_name_start();
  while( LCMS != IN->get_raw_spec_name_end() ){
    
    if( (*LCMS).second.find( THIS_RUN ) != string::npos ){
      REQUIRED_LC_MS_ID = (*LCMS).first;
      break;
    }
    
    LCMS++;
  }
  
}


/////////////////////////////////////////////////////////////////
// show specific MS2 to MS1 assignment overview:
void MS2_MS1_matcher::show_annotation_overview(){
  double high_per = (double)highQual_specific_MS2_MS1_assignments / highQual_specific_MS2_counter;
  high_per *= 100.0;
  double per = (double)specific_MS2_MS1_assignments / specific_MS2_counter;
  per *= 100.0;
  printf("\t\t\t\t\t-> %0.2f percentage of %d high probability MS/MS IDs assigned [for %d no MS1 feature found]\n", high_per, highQual_specific_MS2_counter, (highQual_specific_MS2_counter - highQual_specific_MS2_MS1_assignments) ); 
  printf("\t\t\t\t\t-> %0.2f percentage of %d (all) MS2 scans assigned [for %d no MS1 feature found]\n", per, specific_MS2_counter, (specific_MS2_counter - specific_MS2_MS1_assignments) ); 
  
  
  showDetailedMissedAnnotationSummary();
}


/////////////////////////////////////////////////////////////////
// detailed missed annotation summary:
void MS2_MS1_matcher::showDetailedMissedAnnotationSummary(){
  
  // sort first peptide according to their peptide sequence and show overview afterwards
  map<string, pair<ms2_info*, vector<int> > > mainMap;
  
  vector<ms2_info*>::iterator I = MISSED_MS2.begin();
  while( I != MISSED_MS2.end() ){
    
    ms2_info* ms2 = *I;
    map<string, pair<ms2_info*, vector<int> > >::iterator F = mainMap.find( ms2->get_TOTAL_SQ() );
    if( F == mainMap.end() ){
      vector<int> s;
      s.push_back( ms2->get_SCAN());
      pair<ms2_info*, vector<int> > myPair = make_pair( ms2, s);
      mainMap.insert( make_pair ( ms2->get_TOTAL_SQ(), myPair) ) ;
    }
    else{
      F->second.second.push_back( ms2->get_SCAN() );
    }
    I++; 
  }
  
  
   printf("\t\t\t\t\t-> %d unique peptides not associated to MS1 features \n\n", mainMap.size() ); 
  
  if( MS2_MS1_matcher::NON_MATCH_VIEW ){
    // now print out:
    map<string, pair<ms2_info*, vector<int> > >::iterator F = mainMap.begin( );
    while( F != mainMap.end() ){
      
      ms2_info* ms2 = F->second.first;
      printf("\t\t\t\t\t\t- missed unique peptide:\tm/z=%0.3f\tAC=%s\tSQ=%s\tz=%d:\t(", ms2->get_MONO_MZ(), ms2->get_AC().c_str(), ms2->get_TOTAL_SQ().c_str(), ms2->get_CHRG());     
      vector<int>::iterator V = F->second.second.begin();
      while( V != F->second.second.end() ){
        printf("%d\t ", *V);    
        V++; 
      }
      printf(")\n");    
      F++;
    }
  }
}


//////////////////////////////////////////////////////////////
// shows if amigouities have been found:
void MS2_MS1_matcher::showAmbiguouities(){
  cout<<endl<<"Apex as constraint used in case: "<<AM_APEX_COUNTER<<endl;
  cout<<endl<<"More than one MS1 peak found in case: "<<AM_COUNTER<<endl;
  
}

//////////////////////////////////////////////////////////////
// check if the ms2 scan possible can fit to the feature:
bool MS2_MS1_matcher::checkMS12MS2Combination( ms2_info* ms2, feature* ms1 ){
  
  /*
  if( ms2->get_SCAN_END() == 8919 ){
    ms1->show_info();
  }
  
  
  if( ms1->get_scan_number() == 8908 ){
    ms2->show_info();
  }
  */
  
  
  /////////////////////////////////////////
  // A: charge states check:
  if( ms2->get_CHRG() != ms1->get_charge_state()){
    return false;
  }

  
  /////////////////////////////////////////
  // B: check scan or retention time range:
  if( ms2->getRetentionTime() > 0.0 ){
    // check on teh Tr level:
    if( ( ms1->get_retention_time_START() - ms2_info::MS2_TR_TOL >  ms2->getRetentionTime() ) 
        || ( ms1->get_retention_time_END() + ms2_info::MS2_TR_TOL < ms2->getRetentionTime() ) ){
      return false;
    }
  }
  else{
    // check on the scan level:
    if( ( ms1->get_scan_start() - get_SCAN_TOL() >  ms2->get_SCAN_START() ) 
        || ( ms1->get_scan_end() + get_SCAN_TOL() < ms2->get_SCAN_END() ) ){
      return false;
    }
  }
  
  
  ///////////////////////////////////////////////////////////////
  // B2: check with MS/MS scan if this feature has a MS/MS feature
  if( ms1->getMS2Feature() != NULL ){
    MS2_feature* ms2F = ms1->getMS2Feature();
    if( ms2F->getApexScan() == ms2->get_SCAN_START() ){
      return true;
    }
  }
  
 
  ///////////////////////////////////////////////////////////////
  // C: check with M/Z:
  // compare now the m/z
  double ms2MZ = 0;
  // check if theoretical mass
  // or precursor mass used for matching
  if( ms2_info::THEO_MATCH_MODUS ){
    ms2MZ = MS2_SCAN->get_THEO_MZ();
  }
  else{
    ms2MZ = MS2_SCAN->get_MONO_MZ();
  }
  
  if( !checkMZTolerance( ms2MZ , ms1->get_MZ() ) ){
    return false;
  }
  
  // ok, everything ckeck and ok, so return true: 
  return true;
  
}


///////////////////////////////////////////////////////////////////////
// compare 2 m/z values and decide whether they fall into the m/z tolerance
// range or not:
bool MS2_MS1_matcher::checkMZTolerance( double mzA, double mzB ){
  return simple_math::compareMassValuesAtPPMLevel( mzA, mzB, ms2_info::MS2_MZ_PPM_TOLERANCE);  
}

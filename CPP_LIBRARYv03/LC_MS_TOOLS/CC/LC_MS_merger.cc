///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS Spec MERGER:
// merges 2 spectra which have been preprocessed
// by LC_MS_merger-> common lc_peaks are marked!!!!
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_ALIGN_ERROR
#define USE_MATCH_REGION
#define USE_SEARCHER
#define USE_LC_MS_MERGER

#include "LC_MS_TOOLS_USE.h"

bool LC_MS_merger::MS2_PROGRESSION = true;

bool LC_MS_merger::DEBUG_LCMS_MERGER = false;
double LC_MS_merger::FEATURE_MZ_MIN=785;
double LC_MS_merger::FEATURE_MZ_MAX=786;




////////////////////////////////////////////////
// constructor for the object ana_summarizer:
LC_MS_merger::LC_MS_merger(){
 
  MERGUS = NULL;
  // MZ:
  DEBUG_MZ = -1;
}

//////////////////////////////////////////////////
// class desctructor
LC_MS_merger::~LC_MS_merger(){
   
 /////////////////////////
 // destroy here the spectrum:
  MERGUS = NULL;
  A = NULL;
  B = NULL;
  ALL_MATCH_REGIONS.clear();
  alignment_error.clear();

}

//////////////////////////////////////////////////
// set the spectra A / B which are the source for the
// mmerged spectra:
void LC_MS_merger::set_input_spectra(LC_MS* IN_A, LC_MS* IN_B){
  
  A = IN_A;
  A->order_by_mass();
  B = IN_B;
  B->order_by_mass();
  
  common_counter = 0;
  non_common_counter = 0;

}


//////////////////////////////////////////////////
// initialize the merged spectrum
void LC_MS_merger::init_merged_spectrum(){
	
  // create a new name for the merged spectra:
  char text[50];
  sprintf(text,"%d_%d", A->get_spectrum_ID(), B->get_spectrum_ID());
  string merge_name = text;
  
  // allocate the MERGUS:
  if(MERGUS != NULL){
    delete MERGUS;
    MERGUS = NULL;
  }	
  MERGUS = new LC_MS(merge_name);
  
  // set now the raw data names
  if(A->check_raw_spec_name_empty()){
    MERGUS->add_raw_spec_name(A->get_spectrum_ID(), A->get_spec_name());
  }
  else{
    MERGUS->add_raw_spec_name_map(A->get_raw_spec_name_map());  
  }
  
  if(B->check_raw_spec_name_empty()){
    MERGUS->add_raw_spec_name(B->get_spectrum_ID(), B->get_spec_name());
  }
  else{
    MERGUS->add_raw_spec_name_map(B->get_raw_spec_name_map());  
  }

  // reset match status of every feature:
  vector<LC_MS_FEATURE>::iterator FEA = B->get_feature_list_begin();
  while( FEA != B->get_feature_list_end() ){
    (*FEA).set_feature_match_status( false );
    FEA++;
  }  
  
  FEA = A->get_feature_list_begin();
  while( FEA != A->get_feature_list_end() ){
    (*FEA).set_feature_match_status( false );
    FEA++;
  }  
  ALL_MATCH_REGIONS.clear();
}

////////////////////////////////////////////////
// merger all common lc_peak in two
// spectra A / B into a new LC-MS spectra:
void LC_MS_merger::start_merging(){
  
  /////////////////////////////////////////////
  // initialize the merged spectrum
  init_merged_spectrum();
  
  /////////////////////////////////////////////
  // extract first the common peaks:
  extract_match_regions();  
  PROCESS_MATCH_REGIONS();
  
  /////////////////////////////////////////////
  // add all non matched peaks from LC/MS A and B
  add_non_match_peaks( A );
  add_non_match_peaks( B );
   
  /////////////////////////////////////////////
  // order and remove duplicates peaks:a
  MERGUS->order_by_mass();
 
}

////////////////////////////////////////////////
// extract the overlapping features betwween
// 2 LC/MS runs
void LC_MS_merger::start_extracting_LCMS_overlap(){
  
  /////////////////////////////////////////////
  // initialize the merged spectrum
  init_merged_spectrum();
  
  /////////////////////////////////////////////
  // extract first the common peaks:
  extract_match_regions();  
  PROCESS_MATCH_REGIONS();
    
  /////////////////////////////////////////////
  // order and remove duplicates peaks:
  MERGUS->order_by_mass();
  
}


////////////////////////////////////////////////
// iterates through all peaks in lc_MS run B (the one
// which retention time has been corrected) and searches
// match candidates in run A
void LC_MS_merger::extract_match_regions(){
  
  ALL_MATCH_REGIONS.clear();
  
  // set the iterator which controls matchin of feature in B to A
  // to the start of the feature list in LC/MS run A
  MERGE_ITERATOR_A = A->get_feature_list_begin();
  
  // go throught list of features in LC/MS B and match 
  // them to features in LC/MS run A
  vector<LC_MS_FEATURE>::iterator FEA_B = B->get_feature_list_begin();
  while( FEA_B != B->get_feature_list_end() ){
    
    //---------------------------------------------------------------
    // define now a region, in which we expect the counter lc peak:
    // - mass will m/z +/- mz_tolerance
    // - TR will be TB +/- up/down_alignment_error
    feature* THIS_F = &(*FEA_B);
    
    //////////////////////////
    // debugging:
    if( DEBUG_LCMS_MERGER ){
      if(( THIS_F->get_MZ() <= FEATURE_MZ_MAX) && ( THIS_F->get_MZ() >= FEATURE_MZ_MIN) && (THIS_F->get_scan_number() == 3989 )){
        THIS_F->show_info();
      }
    }
    
    // create the match region:
    match_region* REGION = create_match_region( THIS_F  );
    
    // check if there is a match:
    if(REGION->get_nb_candidates() > 0){ 
      
      // match one MS1 feature from LC/MS run B against
      // all possible MS1 feature within tolerance m/z and Tr window
      REGION->find_best_match();
      
      //////////////////////////
      // debugging:
      if( DEBUG_LCMS_MERGER ){
        if(( THIS_F->get_MZ() <= FEATURE_MZ_MAX) && ( THIS_F->get_MZ() >= FEATURE_MZ_MIN) ){
          cout<<"Best candidate:";
          REGION->get_best_match()->show_info();
        }
      }
       // store the match region:
      ALL_MATCH_REGIONS.push_back( *REGION );  
    }
    
    delete REGION;
    REGION = NULL;
    FEA_B++;
  } 
}

////////////////////////////////////////////////
// process the extracted match regions, check for 
// double matches
void LC_MS_merger::PROCESS_MATCH_REGIONS(){
  
  // order by MZ size:
  sort( ALL_MATCH_REGIONS.begin(), ALL_MATCH_REGIONS.end(), OPERATOR_MATCH_MZ() );

  vector<match_region>::iterator M_REGION_A = ALL_MATCH_REGIONS.begin();
  while( M_REGION_A != ALL_MATCH_REGIONS.end() ){
    
    match_region* REGION = &(*M_REGION_A);
    
    // PROCESS the match regions:
    // check if its not matching to a feature,
    // which better matches to another one
    CHECK_FEATURE_OVERLAP_MATCH( M_REGION_A );
    
    if( REGION->get_nb_candidates() > 0 ){
      
      // add the new common peak:
      feature* F_A = REGION->get_best_match();
      feature* F_B = REGION->get_TARGET();
      
      
      //////////////////////////
      // debugging:
      if( DEBUG_LCMS_MERGER ){
        if(( F_B->get_MZ() <= FEATURE_MZ_MAX) && ( F_B->get_MZ() >= FEATURE_MZ_MIN) ){
          
          if( F_A != NULL){
            cout<<"Validated best candidate:";
            F_A->show_info();
          }
          else{
            cout<<"No match for:";
            F_B->show_info();
          }
        }
      }
      
      
      if( (F_A != NULL) && (F_B != NULL) ){
        
        /////////////////////////////////////////////
        // if this parameter is on, then
        // progress MS2 information along 
        // matched MS1 features
        if( MS2_PROGRESSION ){
          
          // update the info from peak B by A:
          if( ( F_A->get_MS2_info() ) && ( !F_B->get_MS2_info() ) ){
            F_B->add_MS2_info( F_A->get_MS2_SCAN_LIST() );
          }
          
          // update the info from peak A by B:
          else if( (!F_A->get_MS2_info()) && (F_B->get_MS2_info()) ){
            F_A->add_MS2_info( F_B->get_MS2_SCAN_LIST() );
          }
          /////////////////////////////////////////////
        }
        
        ////////////////////////////////////////////
        // average the retention time over the matches
        normalize_retention_time(F_A, F_B);
        
        /////////////////////////////////////////////
        // add the matched peak B to A:
        F_A->set_feature_match_status( true );
        F_B->add_matched_feature( F_A );
             
        /////////////////////////////////////////////
        // add new the peak A to the megred LC_MS
        add_peak_to_MERGUS( F_B );
        
        /////////////////////////////////////////////
        // mark that the feature has been matched:
        F_B->set_feature_match_status( true );
        common_counter++;
      }
      
      F_A = NULL;
      F_B = NULL;
    }
    
    M_REGION_A++;
  }
  
  ALL_MATCH_REGIONS.clear();
}


///////////////////////////////////////////////////////////////////////////
// go now again through both peak lists and add all the not common lc peaks:
void LC_MS_merger::add_non_common_peaks(LC_MS* IN, LC_MS* TARGET){

  
  // set now the raw data names
  if(TARGET->check_raw_spec_name_empty()){
    //////////////////
    printf("\n\t\t\t- LC/MS %s[%d] vs %s[%d]\n", IN->get_spec_name().c_str(), IN->get_spectrum_ID(), TARGET->get_spec_name().c_str(), TARGET->get_spectrum_ID());
  }
  else{
    // this case, its a master run, search every single LC-MS run in the master run:
    map<int, string>::iterator p = TARGET->get_raw_spec_name_start();
    while(p != TARGET->get_raw_spec_name_end()){
      
      //////////////////
      printf("\n\t\t\t- LC-MS %s[%d] vs %s[%d]\n", IN->get_spec_name().c_str(),IN->get_spectrum_ID(), (*p).second.c_str(),(*p).first);
      p++;
    }
  } 
}


///////////////////////////////////////////////////////////////////////////
// adds a common peak to the merged spectrum:
void LC_MS_merger::add_peak_to_MERGUS(feature* IN){
  
  // reset the feature ID:
  IN->set_feature_ID( MERGUS->get_nb_features() );
  // add to the run:
  MERGUS->add_feature( IN );
  IN = NULL;
}

///////////////////////////////////////////////////////////////////////////
// creates a match region which contains all lc peaks falling within the
// category of peaks fullfilling the input conditions:
match_region* LC_MS_merger::create_match_region(feature* IN ){

  // define the constraints:
  double MZ = IN->get_MZ();
  double MZ_MAX = MZ + 5.0 * feature::getFeatureMassErrorAtPPMLevel( MZ );
  double MZ_MIN = MZ - 5.0 * feature::getFeatureMassErrorAtPPMLevel( MZ );

  // create a match region 
  match_region* tmp = new match_region( IN );
  
  
  // debugging:

  /*
   if( IN->get_feature_ID() == 171 ){
   IN->show_info();
   }  
   */
  
  /*
   if( IN->get_MS2_info(0.0) ){
   if( IN->get_SQ(0.0) == "SIGTENTEENR"){
   IN->show_info();
    }
   }
   */
  /*
   bool print = false;
   if( ( IN->get_MZ() >= 426.76) && ( IN->get_MZ() <= 426.77) ){
   IN->show_info();
   print = true;
   }
  */
  
  
  /////////////////////////////
  // search lower m/z features:
  vector<LC_MS_FEATURE>::iterator FEA_A = MERGE_ITERATOR_A;
  while(  FEA_A != A->get_feature_list_end() ){
    
    feature* FEA = &(*FEA_A); 
    // check mass range, if it cant match anymore:
    if( MZ_MIN > FEA->get_MZ() ){
      break;
    }     
    // check if its a candidate for this region:
    tmp->checkFeatureCandidate( FEA );
    if(  FEA_A == A->get_feature_list_begin() ){
      break;
    }
    FEA_A--;
  } 
  
  /////////////////////////////
  // search higher m/z features:
  // vector<feature>::iterator FEA_A = A->get_feature_list_begin();
  FEA_A = MERGE_ITERATOR_A;
  FEA_A++;
  while(  FEA_A != A->get_feature_list_end() ){
    
    feature* FEA = &(*FEA_A); 
   
    // check mass range, if it cant match anymore:
    if( MZ_MAX < FEA->get_MZ() ){
      break;
    }     
    // check if its a candidate for this region:
    tmp->checkFeatureCandidate( FEA );
    FEA_A++;
  } 
  
  // set the start for the next feature match search to the end of this search, 
  // so next search does not need to start all over from the beginning of the
  // list but directly from here:
  if( FEA_A != A->get_feature_list_end() ){
    MERGE_ITERATOR_A = FEA_A;
  }
  
  // return it:
  return tmp;
}


///////////////////////////////////////////////////////////////////////////
// add all features, which have not been matched to the MASTER run:
void LC_MS_merger::add_non_match_peaks( LC_MS* IN ){
  
  // define start / end of non common feature list:
  vector<LC_MS_FEATURE>::iterator FEA = IN->get_feature_list_begin();
  
  // now cycle through the list:
  while( FEA != IN->get_feature_list_end() ){
    
    // add to MASTER if the feature has not 
    // been matched before:
    if( !((*FEA).get_feature_match_status( ))  ) {
      add_peak_to_MERGUS( &(*FEA) );
    }
    
    FEA++;
    //FEA = IN->remove_feature_from_list( FEA );
  }
}


////////////////////////////////////////////////////////////////////////////
// normalize the retention times over the matches:
void LC_MS_merger::normalize_retention_time(feature* A, feature* B){
  
  float nb_match_A = A->get_replicate_match_nb();
  float nb_match_B = B->get_replicate_match_nb();
  float T_A = A->get_retention_time() * nb_match_A;
  float T_B = B->get_retention_time() * nb_match_B;
  float TR = ( T_A + T_B ) / ( nb_match_A + nb_match_B );
  
  // store it:
  //A->set_retention_time(TR);
  B->set_retention_time(TR); 
  
 }



////////////////////////////////////////////////////////////////////////////
// PROCESS the match regions:
// check if its not matching to a feature,
// which better matches to another one
void LC_MS_merger::CHECK_FEATURE_OVERLAP_MATCH( vector<match_region>::iterator IN ){
  
  match_region* REGION = &(*IN);
  
  if( REGION != NULL ){
    feature* MY_BEST = REGION->get_best_match();
    double MZ = REGION->get_MZ();
    double MAX_MZ = MZ + 3.0 * feature::getFeatureMassErrorAtPPMLevel( MZ );
    
    vector<match_region>::iterator M_REGION = IN;
    M_REGION++;
    while( ( M_REGION != ALL_MATCH_REGIONS.end() ) && ( REGION->get_nb_candidates() ) ){
      
      match_region* COMPARE_REGION = &(*M_REGION);
      if( COMPARE_REGION->get_MZ() > MAX_MZ ) {
        break;
      }
      else if( COMPARE_REGION->get_nb_candidates() > 0 ){
        
        // check if they point to the same best matching feature
        if( MY_BEST->get_feature_ID( ) == COMPARE_REGION->get_best_match()->get_feature_ID( ) ){
          // ok, check here which region should have this matched feature
          assign_feature_to_best_region( REGION, COMPARE_REGION );
        }
      }
      
      COMPARE_REGION = NULL;
      M_REGION++;
    }
    
    MY_BEST = NULL;
    REGION = NULL;
  }
}

////////////////////////////////////////////////////////////////
// ok, check here which region should have this matched feature
void LC_MS_merger::assign_feature_to_best_region( match_region* A, match_region* B ){
  A->compareBestCandidates( B );
}

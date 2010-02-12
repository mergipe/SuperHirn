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

#define USE_FEATURE_UPDATER
#include "STATISTIC_TOOLS_USE.h"




////////////////////////////////////////////////
// constructor for the object feature_updater:
feature_updater::feature_updater(){
  
  read_param* def = new read_param();
  // main interact dir:
  MAIN_INTERACT_DIR = def->search_tag( "INCLUSION LIST DIRECTORY" );
  def->search_tag("INCLUSIONS LIST MS2 SCAN tolerance",&SCAN_TOL);
  def->search_tag("Peptide Prophet Threshold",&PEP_PROB_CUT_OFF);
  delete def;
  def = NULL;
  MS2_HITS = 0;
  ALL_HITS = 0;
  tot_input_MS2_scans = 0;
}

//////////////////////////////////////////////////
// class desctructor of feature_updater
feature_updater::~feature_updater(){
  THIS_MS2_INFO_LIST.clear();
  ALL_MS2_INFO_LIST.clear();
  MS2_HITS = 0;
  tot_input_MS2_scans = 0;
}

//////////////////////////////////////////////////
// class copy constructor of feature_updater
feature_updater::feature_updater(const feature_updater& tmp){
  THIS_MS2_INFO_LIST = tmp.THIS_MS2_INFO_LIST;
  ALL_MS2_INFO_LIST = tmp.ALL_MS2_INFO_LIST;
  MS2_HITS = tmp.MS2_HITS;
}

//////////////////////////////////////////////////
// class copy constructor of feature_updater
feature_updater::feature_updater(const feature_updater* tmp){
  THIS_MS2_INFO_LIST = tmp->THIS_MS2_INFO_LIST;
  ALL_MS2_INFO_LIST = tmp->ALL_MS2_INFO_LIST;
  MS2_HITS = tmp->MS2_HITS;
}

//////////////////////////////////////////////////
// copy constructor:
feature_updater& feature_updater::operator=(const feature_updater& tmp){
  THIS_MS2_INFO_LIST = tmp.THIS_MS2_INFO_LIST;
  ALL_MS2_INFO_LIST = tmp.ALL_MS2_INFO_LIST;
  MS2_HITS = tmp.MS2_HITS;
  return *this;
}


////////////////////////////////////////////////////////////////
// initialze the main parameters
void feature_updater::INIT_ALL(){  
  MS2_HITS = 0; 
  IMPORT_MS2_INFO();
}

////////////////////////////////////////////////////////////////
// initialze the main parameters
void feature_updater::INIT_ALL(string in){  
  MS2_HITS = 0; 
  MAIN_INTERACT_DIR = in;
  IMPORT_MS2_INFO();
}


////////////////////////////////////////////////////////////////
// start importing MS2 information from various sources:
void feature_updater::IMPORT_MS2_INFO(){
  // start reading the different directories:
  parse_interact_files_in_DIR( MAIN_INTERACT_DIR );  
}
  

///////////////////////////////////////////////////
// parses all interact files in a certain directory:
void feature_updater::parse_interact_files_in_DIR( string INTERACT_DIR ){
    
  MS2_HITS = 0; 
  tot_input_MS2_scans = 0;
  printf("\n\t\t\t- importing interact files from '%s'\n", INTERACT_DIR.c_str() );
  file_sys* accessor = new file_sys();
  accessor->read_dir( INTERACT_DIR.c_str(), ".xml");
  char* file  = NULL;
  file = accessor->get_next_file();
  
  // now get the ms2_info and transform to features:
  THIS_MS2_INFO_LIST.clear();
  
  while(file != NULL){
      
    
    GeneralMSMSXMLParser* parser = new GeneralMSMSXMLParser( file );
    parser->startMSMSXMLFileParsing();
    // merge parsed ms2 info:
    ALL_MS2_INFO_LIST.insert( ALL_MS2_INFO_LIST.end(), parser->getMSMSListStart(), parser->getMSMSListEnd() );
    delete parser;
    parser = NULL;
    // next:
    file = accessor->get_next_file();
  }
  
  delete accessor;
  accessor = NULL;
  
  tot_input_MS2_scans = ALL_MS2_INFO_LIST.size();
}



////////////////////////////////////////////////////////////////
// update all extracted features with additional MS2 information:
void feature_updater::update_feature_identity( LC_MS* IN){
  
  GOOD_HITS = 0;
  LOW_HITS = 0;
  ALL_HITS = 0;

  // create a list of feature references from the input LC/MS
  vector< feature* > ALL_FEATURES;
  vector<LC_MS_FEATURE>::iterator F = IN->get_feature_list_begin();
  while( F != IN->get_feature_list_end() ){
    ALL_FEATURES.push_back( &(*F) );
    F++;
  }
  
  // order the list:
  sort( ALL_FEATURES.begin(), ALL_FEATURES.end(), FEATURE_MZ_OPERATOR() );
  
  // match them using this class:
  MS2_MS1_matcher* MATCHER = new MS2_MS1_matcher( );
  vector<ms2_info>::iterator START = ALL_MS2_INFO_LIST.begin();
  while( START != ALL_MS2_INFO_LIST.end() ){
    if( MATCHER->combine_POSTANNOTATION_MS2_to_MS1_data( &ALL_FEATURES, &(*START) ) ){
      update_MS2_matching_statistics( &(*START) ); 
    }
  
    START++;
  }
  delete MATCHER;
  MATCHER = NULL;
  
  ALL_FEATURES.clear();
  IN = NULL;
}

////////////////////////////////////////////////////////////////
// update all extracted features with additional MS2 information:
void feature_updater::update_feature_identity( profile_group* IN){
  
  GOOD_HITS = 0;
  LOW_HITS = 0;
  ALL_HITS = 0;

  
  // get the feature list of the profile group:
  vector< feature* >* ALL_FEATURES = IN->get_MEMBER_LIST();
  // order the list:
  sort( ALL_FEATURES->begin(), ALL_FEATURES->end(), FEATURE_MZ_OPERATOR() );
  
  // match them using this class:
  MS2_MS1_matcher* MATCHER = new MS2_MS1_matcher( );
  vector<ms2_info>::iterator START = ALL_MS2_INFO_LIST.begin();
  while( START != ALL_MS2_INFO_LIST.end() ){
    MATCHER->combine_POSTANNOTATION_MS2_to_MS1_data( ALL_FEATURES, &(*START) );
    START++;
  }
  delete MATCHER;
  MATCHER = NULL;
  
  ALL_FEATURES = NULL;
  IN = NULL;
}


////////////////////////////////////////////////////////////////
// update all extracted features with additional MS2 information:
void feature_updater::update_feature_identity( vector<peptide_DELTA_group>* IN){
  
  GOOD_HITS = 0;
  LOW_HITS = 0;
  ALL_HITS = 0;

  
  progress_bar bar( IN->size() );
  vector< peptide_DELTA_group>::iterator PEP = IN->begin();
  while( PEP != IN->end() ){
    update_feature_identity( &(*PEP) );
    // progress bar:
    bar.update_progress();
    PEP++;
  }   
}  


////////////////////////////////////////////////////////////////
// update all extracted features with additional MS2 information:
void feature_updater::update_feature_identity( peptide_DELTA_group* PEP){
  
  // normal members
  vector<feature*>::iterator FEA = PEP->get_members_list_start();
  while( FEA != PEP->get_members_list_end() ){
    update_feature_identity( *FEA );
    FEA++;
  }

  // modified members
  map< double, vector<feature*> >::iterator M_FEA = PEP->get_modified_members_list_start();
  while( M_FEA != PEP->get_modified_members_list_end() ){
    FEA = (*M_FEA).second.begin();
    while( FEA != (*M_FEA).second.end() ){
      update_feature_identity( *FEA );
      FEA++;
    }
    M_FEA++;
  }
  
  PEP = NULL;
}


////////////////////////////////////////////////////////////////
// update an extracted features with additional MS2 information:
void feature_updater::update_feature_identity( feature* IN){

  vector<ms2_info>::iterator START = ALL_MS2_INFO_LIST.begin();
  while( START != ALL_MS2_INFO_LIST.end() ){
    
    if( compare_MS2_with_MS1_feature( IN, &(*START) ) ){
      START = ALL_MS2_INFO_LIST.erase( START );      
      // for statistics
      if( (*START).get_PEP_PROB() >= feature::PEPTIDE_PROBABILITY_THRESHOLD){
        GOOD_HITS++;
      }
      else{
        LOW_HITS++; 
      }
    }
    else{
      START++;
    }
  }
  
  
}


////////////////////////////////////////////////////////////////////////////////////////////
// update the ms2 matching statistics:
void feature_updater::update_MS2_matching_statistics( ms2_info* IN ){
  
  if( IN->get_PEP_PROB() >= PEP_PROB_CUT_OFF ){
    GOOD_HITS++;
  }
  else{
    LOW_HITS++;
  }
  
  ALL_HITS++;

}

////////////////////////////////////////////////////////////////////////////////////////////
// print summary:
void feature_updater::print_update_summary(){
  
  if( tot_input_MS2_scans > 0 ){
    
    // # assigned ms2 and percentage
    double input_scans = (double)tot_input_MS2_scans;
    
    // compute hit statistics:
    ALL_HITS /= input_scans;
    ALL_HITS *= 100.00;    
    GOOD_HITS /= input_scans;
    GOOD_HITS *= 100.00;
    LOW_HITS /= input_scans;
    LOW_HITS *= 100.00;
    double NON_HITS = 100.00 - ALL_HITS;

    printf("\n\n\t\t ===============================\n" );
    printf("\t\t Annotation Summary:\n\n");
    printf("\t\t Total input MS2 spectra: %0.0f MS2 scans\n",   input_scans );
    printf("\t\t Percentage annotated:\n");
    printf("\t\t Total assigned: %0.2f MS2 scans (%d)\n", ALL_HITS, int(ALL_HITS*input_scans*0.01) );
    printf("\t\t High quality assigned: %0.2f MS2 scans (%d)\n", GOOD_HITS, int(GOOD_HITS*input_scans*0.01) );
    printf("\t\t Low quality assigned: %0.2f MS2 scans (%d)\n", LOW_HITS, int(LOW_HITS*input_scans*0.01) );
    printf("\t\t Total not assigned: %0.2f MS2 scans (%d)\n", NON_HITS, int(NON_HITS*input_scans*0.01) );
    printf("\n\n");
    
  }
  else{
    printf("\t\t- Summary: no MS2 scans extracted \n" );
  }
}



////////////////////////////////////////////////////////////////////////////////////////////
// compares a MS1 with MS2, if ok add its to the MS1 features
bool feature_updater::compare_MS2_with_MS1_feature( feature* ms1, ms2_info* ms2 ){


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
    if( ( ms1->get_scan_start() - SCAN_TOL >  ms2->get_SCAN_START() ) 
        || ( ms1->get_scan_end() + SCAN_TOL < ms2->get_SCAN_START() ) ){
      return false;
    }
  }
  
  
  ///////////////////////////////////////////////////////////////
  // C: check with M/Z:
  // compare now the m/z
  double ms2MZ = 0;
  // check if theoretical mass
  // or precursor mass used for matching
  if( ms2_info::THEO_MATCH_MODUS ){
    ms2MZ = ms2->get_THEO_MZ();
  }
  else{
    ms2MZ = ms2->get_MONO_MZ();
  }
  
  // compare 2 m/z values and decide whether they fall into the m/z tolerance
  // range or not:
  if( ! MS2_MS1_matcher::checkMZTolerance( ms2MZ , ms1->get_MZ() ) ){
    return false;
  }

  /*
  double delta_MZ = fabs(ms2MZ - ms1->get_MZ());
  if( delta_MZ > ms2_info::MS2_MZ_TOL){
    return false;
  }
   */  
  
  // ok, everything ckeck and ok, so return true: 
  return true;
}
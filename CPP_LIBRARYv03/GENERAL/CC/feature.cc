///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS feature:
// 
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_MS2_INFO
#define USE_FSTREAM
#define USE_STDIO
#define USE_STDLIB
#define USE_STRING
#define USE_MATH
#define USE_VECTOR
#define USE_MAP
#define USE_READ_PARAM
#define USE_NAMESPACE
#define USE_FEATURE

#include "GENERAL_USE.h"

///////////////////////////////////////////
// atomic mass to compute theo mass from 
// AA sequence:
double feature::_MONO_H = 1.00794;
double feature::_MONO_O = 15.99943;

// tolerance in m/z and TR:
double feature::PPM_MZ_TOL;
//double feature::MZ_TOL = 0.01;
double feature::TR_TOL = 0.5;
double feature::PEPTIDE_PROBABILITY_THRESHOLD = 0.9;

// option to store all low probability scans in the feature
bool feature::STORE_ALL_LOW_PROBABILITY_MS2_SCANS = false;
bool feature::PRINT_ALL_ACs = true;

////////////////////////////////////////////////
// constructor for the object feature:
feature::feature(){
  
  MONO_MZ = -1;
  MONO_MZ_ORIGINAL = -1;
  TR_APEX = -1;
  feature_ID = -1;
  spectrum_ID = -1;
  MASTER_ID = -1;
  scan_apex = -1;
  scan_end = -1;
  scan_start = -1;
  charge_state = -1;
  total_peak_area = -1;
  apex_peak_intensity = -1;
  TR = -1;
  PEAK_SCORE = -1;
  alignment_error_up = 0;
  alignment_error_down = 0;  
  SCORE_HOLDER = 0;
  TR_START = -1;
  TR_END = -1;
  MONO_MZ_START = -1;
  MONO_MZ_END = -1;
  feature_match_status = false;
  LCprofile = NULL;
  MS2TraceFeature = NULL;
  PI = 0;
}

////////////////////////////////////////////////
// constructor for the object feature:
feature::feature(double IN_MONO_MZ, double IN_TR, int IN_scan_apex, int IN_scan_start, int IN_scan_end,int IN_charge_state, float IN_total_peak_area, float IN_apex_peak_intensity, float IN_PEAK_SCORE){

  MONO_MZ = IN_MONO_MZ;
  MONO_MZ_ORIGINAL = MONO_MZ;
  TR_APEX = IN_TR;
  feature_ID = -1;
  spectrum_ID = -1;
  MASTER_ID = -1;
  scan_apex = IN_scan_apex;
  scan_end = IN_scan_end;
  scan_start = IN_scan_start;
  charge_state = IN_charge_state;
  total_peak_area = IN_total_peak_area;
  apex_peak_intensity = IN_apex_peak_intensity;
  TR = IN_TR;
  PEAK_SCORE = IN_PEAK_SCORE;
  alignment_error_up = 0;
  alignment_error_down = 0;  
  SCORE_HOLDER = 0;
  TR_START = TR;
  TR_END = TR;
  MONO_MZ_START = MONO_MZ;
  MONO_MZ_END = MONO_MZ;
  feature_match_status = false;
  PI = 0;
 
  LCprofile = NULL;
  MS2TraceFeature = NULL;
  
}


////////////////////////////////////////////////
// constructor for the object feature:
feature::feature( MS2_feature* ms2){
  
  MONO_MZ = ms2->getPrecursorMZ();
  MONO_MZ_ORIGINAL = MONO_MZ;
  TR_APEX = ms2->getStartTR();
  feature_ID = -1;
  spectrum_ID = -1;
  MASTER_ID = -1;
  scan_apex = ms2->getApexScan();
  scan_end = ms2->getEndScan();
  scan_start = ms2->getStartScan();
  charge_state = ms2->getPrecursorChrg();
  total_peak_area = -1;
  apex_peak_intensity = -1;
  TR = ms2->getTR();
  PEAK_SCORE = -1;
  alignment_error_up = 0;
  alignment_error_down = 0;  
  SCORE_HOLDER = 0;
  SignalToNoise = 100;
  TR_START = ms2->getStartTR();
  TR_END = ms2->getEndTR();
  MONO_MZ_START = MONO_MZ;
  MONO_MZ_END = MONO_MZ;
  feature_match_status = false;
  PI = 0;
  
  LCprofile = NULL;
  MS2TraceFeature = NULL;
  
  addMS2Feature( ms2 );
  
}


feature::feature(float IN_pmass, int IN_scan, int IN_charge_state){
  
  // get real charged peptide m/z
  MONO_MZ = IN_pmass;
  MONO_MZ_ORIGINAL = MONO_MZ;
  feature_ID = -1;
  spectrum_ID = -1;
  MASTER_ID = -1;
  scan_apex = IN_scan;
  scan_end = -1;
  scan_start = -1;
  charge_state = IN_charge_state;
  total_peak_area = 0;
  apex_peak_intensity = 0;
  TR = -1;
  PEAK_SCORE = 0;
  alignment_error_up = 0;
  alignment_error_down = 0;
  SCORE_HOLDER = 0;
  TR_START = TR;
  TR_END = TR;
  TR_APEX = TR;
  MONO_MZ_START = MONO_MZ;
  MONO_MZ_END = MONO_MZ;
  feature_match_status = false;
  PI = 0;
  
  LCprofile = NULL;
  MS2TraceFeature = NULL;
}


////////////////////////////////////////////////
// copy constructor:
feature::feature(const feature& tmp){

  MONO_MZ = tmp.MONO_MZ;
  MONO_MZ_ORIGINAL = tmp.MONO_MZ_ORIGINAL;
  TR_APEX = tmp.TR_APEX;
  feature_ID = tmp.feature_ID;
  spectrum_ID = tmp.spectrum_ID;
  MASTER_ID = tmp.MASTER_ID;
  MS2_SCANS = tmp.MS2_SCANS;
  scan_apex = tmp.scan_apex;
  scan_end = tmp.scan_end;
  scan_start = tmp.scan_start;
  charge_state = tmp.charge_state;
  total_peak_area = tmp.total_peak_area;
  apex_peak_intensity = tmp.apex_peak_intensity;
  TR = tmp.TR;
  PEAK_SCORE = tmp.PEAK_SCORE;
  alignment_error_up = tmp.alignment_error_up;
  alignment_error_down = tmp.alignment_error_down;
  SCORE_HOLDER = tmp.SCORE_HOLDER;
  matched_feature_list = tmp.matched_feature_list;
  TR_START = tmp.TR_START;
  TR_END = tmp.TR_END;
  MONO_MZ_START = tmp.MONO_MZ_START;
  MONO_MZ_END = tmp.MONO_MZ_END;
  feature_match_status = tmp.feature_match_status;
  SignalToNoise = tmp.SignalToNoise;
  BackgroundNoise = tmp.BackgroundNoise;
  PI = tmp.PI;
  childMSMSScans = tmp.childMSMSScans;
  featureExtraInformation = tmp.featureExtraInformation;

  MS2TraceFeature = NULL;
  if( tmp.MS2TraceFeature != NULL ){
    MS2TraceFeature = new MS2_feature( tmp.MS2TraceFeature );
  }
  
  LCprofile = NULL;
  if( tmp.LCprofile != NULL ){
    LCprofile = new featureLCprofile( tmp.LCprofile );
  }

}


////////////////////////////////////////////////
// constructor for the object feature:
feature::feature(const feature* tmp){
  MONO_MZ = tmp->MONO_MZ;
  MONO_MZ_ORIGINAL = tmp->MONO_MZ_ORIGINAL;
  TR_APEX = tmp->TR_APEX;
  feature_ID = tmp->feature_ID;
  spectrum_ID = tmp->spectrum_ID;
  MASTER_ID = tmp->MASTER_ID;
  MS2_SCANS = tmp->MS2_SCANS;
  scan_apex = tmp->scan_apex;
  scan_end = tmp->scan_end;
  scan_start = tmp->scan_start;
  charge_state = tmp->charge_state;
  total_peak_area = tmp->total_peak_area;
  apex_peak_intensity = tmp->apex_peak_intensity;
  TR = tmp->TR;
  PEAK_SCORE = tmp->PEAK_SCORE;
  alignment_error_up = tmp->alignment_error_up;
  alignment_error_down = tmp->alignment_error_down;
  SCORE_HOLDER = tmp->SCORE_HOLDER;
  matched_feature_list = tmp->matched_feature_list;
  TR_START = tmp->TR_START;
  TR_END = tmp->TR_END;
  MONO_MZ_START = tmp->MONO_MZ_START;
  MONO_MZ_END = tmp->MONO_MZ_END;
  feature_match_status = tmp->feature_match_status;
  SignalToNoise = tmp->SignalToNoise;
  BackgroundNoise = tmp->BackgroundNoise;
  PI = tmp->PI;
  childMSMSScans = tmp->childMSMSScans;
  featureExtraInformation = tmp->featureExtraInformation;

  MS2TraceFeature = NULL;
  if( tmp->MS2TraceFeature != NULL ){
    MS2TraceFeature = new MS2_feature( tmp->MS2TraceFeature );
  }
  
  /////////////////////////////////
  // copy the elution profile:
  LCprofile = NULL;
  if( tmp->LCprofile != NULL ){
    LCprofile = new featureLCprofile( tmp->LCprofile );
  }
  
}


//////////////////////////////////////////////////
// class desctructor

feature::~feature(){
  
  if(!matched_feature_list.empty())
    matched_feature_list.clear();
  
  if( !MS2_SCANS.empty()){
    MS2_SCANS.clear();
  }
  
  if( MS2TraceFeature != NULL){
    delete MS2TraceFeature;
    MS2TraceFeature = NULL;
  }
  
  if( LCprofile != NULL){
    delete LCprofile;
    LCprofile = NULL;
  }
  
  childMSMSScans.clear();
}


//////////////////////////////////////////////////
// copy operator:
feature& feature::operator=(const feature& tmp){
  MONO_MZ = tmp.MONO_MZ;
  MONO_MZ_ORIGINAL = tmp.MONO_MZ_ORIGINAL;
  TR_APEX = tmp.TR_APEX;
  feature_ID = tmp.feature_ID;
  spectrum_ID = tmp.spectrum_ID;
  MASTER_ID = tmp.MASTER_ID;
  MS2_SCANS = tmp.MS2_SCANS;
  scan_apex = tmp.scan_apex;
  scan_end = tmp.scan_end;
  scan_start = tmp.scan_start;
  charge_state = tmp.charge_state;
  total_peak_area = tmp.total_peak_area;
  apex_peak_intensity = tmp.apex_peak_intensity;
  TR = tmp.TR;
  PEAK_SCORE = tmp.PEAK_SCORE;
  alignment_error_up = tmp.alignment_error_up;
  alignment_error_down = tmp.alignment_error_down;
  SCORE_HOLDER = tmp.SCORE_HOLDER;
  SignalToNoise = tmp.SignalToNoise;
  BackgroundNoise = tmp.BackgroundNoise;
  matched_feature_list = tmp.matched_feature_list;
  TR_START = tmp.TR_START;
  TR_END = tmp.TR_END;
  MONO_MZ_START = tmp.MONO_MZ_START;
  MONO_MZ_END = tmp.MONO_MZ_END;  
  feature_match_status = tmp.feature_match_status;
  PI = tmp.PI;
  childMSMSScans = tmp.childMSMSScans;
  featureExtraInformation = tmp.featureExtraInformation;
  
  if( MS2TraceFeature != NULL ){
    delete MS2TraceFeature; 
    MS2TraceFeature = NULL;
  }
  if( tmp.MS2TraceFeature != NULL ){
    MS2TraceFeature = new MS2_feature( tmp.MS2TraceFeature );
  }
  
  if( LCprofile != NULL ){
    delete LCprofile;
    LCprofile = NULL;  
  }
  
  if( tmp.LCprofile != NULL ){
    LCprofile = new featureLCprofile( tmp.LCprofile );
  }
  
  return *this;  
}


/////////////////////////////////////////////////
// equal operator:
bool feature::operator==(const feature& tmp){
  
  
  if( tmp.feature_ID == this->feature_ID ){
    return true;
  }
  
  // compare the charge states:
  if(tmp.charge_state == charge_state){ 
    
    // compare the parent masses:
    if( feature::compareFeatureMassValuesAtPPMLevel( tmp.MONO_MZ, get_MZ() ) ){
      
      // copmare the TR:
      if(fabs(tmp.TR - TR) <= TR_TOL){
        return true;
      }
      else{
        return false;
      }
    }
    else{
      return false;
    }
  }
  else{
    return false;
  }
}

/*
//////////////////////////////////////////////////
// smaller operator:
bool feature::operator<(const feature& tmp){
  
  float delta = tmp.MONO_MZ - MONO_MZ;
  if( delta > 2 * MZ_TOL ){
    return true;
  }
  else{
    return false;
  }
}

//////////////////////////////////////////////////
// bigger operator:
bool feature::operator>(const feature& tmp){
  
  float delta = MONO_MZ - tmp.MONO_MZ;
  if( delta > 2 * MZ_TOL ){
    return true;
  }
  else{
    return false;
  }
}

//////////////////////////////////////////////////
// bigger or equal operator:
bool feature::operator>=(const feature& tmp){
  
  float delta = MONO_MZ - tmp.MONO_MZ;
  if(delta >= 2 * MZ_TOL){
    return true;
  }
  else{
    return false;
  }
}



//////////////////////////////////////////////////
// smaller or equal operator:
bool feature::operator<=(const feature& tmp){
  
  float delta = tmp.MONO_MZ - MONO_MZ;
  if( delta >= 2 * MZ_TOL ){
    return true;
  }
  else{
    return false;
  }
}
*/


//////////////////////////////////////////////////
// show the content of the spectra:
void feature::show_info(){
  
  string SEP = "";
  // print MS-peak info:
  printf("\tMS1 Feature#:%d,%s", get_feature_ID(),SEP.c_str()); 
  printf("m/z:%0.5f%s",get_MZ(),SEP.c_str()); 
  printf("[+%d],%s",get_charge_state(),SEP.c_str()); 
  printf("Area:%0.2f%s",get_peak_area(),SEP.c_str()); 
  printf(",apex:%0.2f[%0.2f:%0.2f][%d:%d:%d],s/n:%0.2f,%0.2f%s",get_retention_time(),get_retention_time_START(),get_retention_time_END(),get_scan_start(),get_scan_number(),get_scan_end(), getSignalToNoise(), get_peak_score(),SEP.c_str()); 
  printf(",matches:%d%s",get_replicate_match_nb(),SEP.c_str()); 
  printf(",LCMS-ID: %d",get_spectrum_ID()); 
  //printf(",M.ID:%d",get_MASTER_ID()); 
  printf("\n");
  ms2_info* tmp = get_best_MS2_SCAN( 0.0 ); 
  if( tmp != NULL ){
    tmp->show_info();
  }
  
  map<int,feature>::iterator P = matched_feature_list.begin();
  while( P != matched_feature_list.end() ){
    cout<<"\t\t-";
    (*P).second.show_info();
    P++;
  }
  
  //////////////////////////////////////////////////
  // show MS/MS spectra info:
  showMS2consensSpectraInfo( );
    
  
}


//////////////////////////////////////////////////
// show MS/MS spectra info:
void feature::showMS2consensSpectraInfo( ){
  if( getMS2Feature() != NULL ){ 
    getMS2Feature()->show_info();
  }
}


//////////////////////////////////////////////////
// add MS/MS info to the feature:
void feature::add_MS2_info( ms2_info* IN ){

  if( IN != NULL ){
    
    //////////////////////////////////////
    // check if alrady a high quality MS2 available:
    // if StoreAllLowProbMS2Scans is true, then store all
    // low quality scans unless a high quality is available
    // otherwise use only the one with the highest peptide probaility
    ms2_info* thisMS2 = NULL;
    if( STORE_ALL_LOW_PROBABILITY_MS2_SCANS ){
      thisMS2 = get_best_MS2_SCAN( );
    }
    else{
      thisMS2 = get_best_MS2_SCAN( -3.0 );
    }
    
    
    /////////////////////////////////////////
    // compare with already available MS2 scans:
    if( thisMS2 != NULL ){
  
      // only save the new scan if its of better quality:
      // if so, clear all previous ids to save space:
      if( thisMS2->get_PEP_PROB() < IN->get_PEP_PROB() ){
        MS2_SCANS.clear();
      }
      else{
        // is of lower quality, so through it away:
        IN = NULL;
      }
    }
    
    
    /////////////////////////////////////////
    // insert the MS2 scan if its not null:
    if( IN != NULL ){
      
      map< double, vector< ms2_info > >::iterator F = MS2_SCANS.find( IN->get_PEP_PROB() );
      if( F == MS2_SCANS.end() ){
        vector< ms2_info > TMP;
        TMP.push_back( *IN );
        MS2_SCANS.insert( make_pair( IN->get_PEP_PROB(), TMP ) );
      }
      else{
        (*F).second.push_back( *IN );
      }
      
    }
    
    IN = NULL;
  }
}


//////////////////////////////////////////////////
// add MS/MS info to the feature:
void feature::add_MS2_info(map<double, vector<ms2_info> >* IN){
  
  MS2_SCANS.clear();
  map<double, vector<ms2_info> >::iterator P = IN->begin();
  while ( P != IN->end() ) {
    vector<ms2_info>::iterator M = (*P).second.begin();
    while( M != (*P).second.end() ){
      add_MS2_info( &(*M) );
      M++;
    }
    P++; 
  }
  IN = NULL;
}


//////////////////////////////////////////////////
// writes out the important information:
void feature::print_content(ofstream* writer, bool TOP){

  char buffer[256];
  
  string SEP = "\t";
    
  // print MS1_info:
  // print matched features:
  if(TOP){
    sprintf(buffer,"%0.2f%s",get_MZ(),SEP.c_str()); 
    writer->write(buffer,strlen(buffer));
    sprintf(buffer,"%0.2f%s",get_retention_time(),SEP.c_str()); 
    writer->write(buffer,strlen(buffer));
    sprintf(buffer,"+%d%s",get_charge_state(),SEP.c_str());
    writer->write(buffer,strlen(buffer));
  }
  
  // print info aout id and peak area:
  sprintf(buffer,"%0.2f%s",get_peak_area(),SEP.c_str());
  writer->write(buffer,strlen(buffer));
  sprintf(buffer,"%d%s",get_spectrum_ID(),SEP.c_str()); 
  writer->write(buffer,strlen(buffer));
  
  sprintf(buffer,"%s%s%s",SEP.c_str(),SEP.c_str(),SEP.c_str()); 
  writer->write(buffer,strlen(buffer));
  
  // print matched features:
  if(TOP){
       
    sprintf(buffer,"%d%s",matched_feature_list.size(),SEP.c_str()); 
    writer->write(buffer,strlen(buffer));
		
    map<int,feature>::iterator P = matched_feature_list.begin();
    while( P != matched_feature_list.end() ){
      //sprintf(buffer,"%s",SEP.c_str()); 
      //writer->write(buffer,strlen(buffer));
      (*P).second.print_content(writer,false);
      P++;
    }
  
  }
}



//////////////////////////////////////////////////
// check how many matches
int feature::get_nb_common_match(){
  return matched_feature_list.size() + 1;
}



//////////////////////////////////////////////////
// functions to set/access machted features:
void feature::add_matched_feature(feature* IN){
  
  
  //////////////////////////////////////////////////////
  // check charge states, in cases where a feature was
  // created based on a MS2 trace, charge state is unknown ( = -1 )
  // -> derivce the charge state from the matched feature (if this is 
  // also not -1 
  deriveChargeStates( IN );
  
  // store the matches of this input features in
  // the new feature:
  map<int,feature>::iterator P = IN->get_match_list_start();
  while( P != IN->get_match_list_end()){
    add_matched_feature( &((*P).second ) );
    P++;
  }
  
  // add ms2 information:
  map<double, vector<ms2_info> >::iterator MS2 = IN->get_MS2_SCANS_START();
  while(  MS2 != IN->get_MS2_SCANS_END() ){
    vector<ms2_info>::iterator M = (*MS2).second.begin();
    while( M != (*MS2).second.end() ){
      add_MS2_info( &(*M) );
      M++;
    }
    MS2++;
  }

  // ok, now make a new feature and  
  // remove its aligned features, since the are now
  // stored in this new feature:
  // feature* TMP = new feature( IN );
  IN->erase_match_list();  
  // remove also the MS2 information:
  IN->removeAllMS2Information();
  
  // check now if this insert ID already exists
  // for example in meta alignments!!!
  // in this case add it to the size of the map
  // new ID = map_size + old_ID:
  int insert_ID = IN->get_spectrum_ID();
  map<int, feature>::iterator F = matched_feature_list.find( insert_ID );
  if( F != matched_feature_list.end() ){
    insert_ID += matched_feature_list.size();
  }
  matched_feature_list.insert( pair<int,feature>( insert_ID, *IN ) );
  IN = NULL;
  
}

//////////////////////////////////////////////////
// return the sum of all intensities over replicates:
double feature::get_replicate_intensity_sum(){
  double SUM = get_peak_area();
  map<int,feature>::iterator P = matched_feature_list.begin();
  while( P != matched_feature_list.end() ){
    SUM += (*P).second.get_peak_area();
    P++;
  }
  return SUM;
}



//////////////////////////////////////////////////
// get the molecular mass of the corrsponding peptide!
double feature::get_Molecular_Mass(){
  
  double MR = get_MZ() * double(get_charge_state());
  
  // remove the H+'s
  MR -= ( get_MONO_H() * double(get_charge_state()) );
  
  return MR;
}



//////////////////////////////////////////////////
// get the best ms2 scan with highest peptide probability:
ms2_info* feature::get_best_MS2_SCAN(){

  ms2_info* OUT = NULL;
  if( get_MS2_info() ) {
    
    map<double, vector<ms2_info> >::reverse_iterator R = MS2_SCANS.rbegin();
    OUT = &( *((*R).second.begin()) );
    
    map<int,feature>::iterator P = matched_feature_list.begin();
    while( P != matched_feature_list.end() ){
      ms2_info* TMP = (*P).second.get_best_MS2_SCAN();
      if( TMP != NULL ){
        if( TMP->get_PEP_PROB() > OUT->get_PEP_PROB() ){
          OUT = TMP;
        }
      }
      P++;
    }
  }
  return OUT;
}

//////////////////////////////////////////////////
// get the best ms2 scan with highest peptide probability:
ms2_info* feature::get_best_MS2_SCAN(double PP_T){
  
  ms2_info* OUT = NULL;
  if( get_MS2_info( PP_T ) ) {
    
    map<double, vector< ms2_info> >::reverse_iterator R = MS2_SCANS.rbegin();
    OUT = &(*((*R).second.begin()) );
    
    map<int,feature>::iterator P = matched_feature_list.begin();
    while( P != matched_feature_list.end() ){
      ms2_info* TMP = (*P).second.get_best_MS2_SCAN();
      if( TMP != NULL ){
        if( TMP->get_PEP_PROB() > OUT->get_PEP_PROB() ){
          OUT = TMP;
        }
      }
      P++;
    }
  }
  return OUT;
}




//////////////////////////////////////////////////
// check if a feature is matched with this id:
bool feature::check_match_by_id(int ID){
  // check if its teh own:
  if( spectrum_ID == ID ){
    return true;
  }
  else{
   // check the matched features:  
    if ( matched_feature_list.find( ID) != matched_feature_list.end() ){
      return true;
    }
    else{
      return false;
    }
  }
  
  return false;
}

//////////////////////////////////////////////////
// get the total peak are over all matched features:
double feature::get_MATCHED_peak_area(){
  double AREA = get_peak_area();
  map<int,feature>::iterator P = matched_feature_list.begin();
  while( P != matched_feature_list.end() ){
    AREA += (*P).second.get_peak_area();
    P++;
  }
  return AREA;
}

//////////////////////////////////////////////////
// get peak area at a certain LC/MS:
double feature::get_peak_area( int point){

  // check if its teh own:
  if( spectrum_ID == point ){
    return get_peak_area();
  }
  
  map<int,feature>::iterator P = matched_feature_list.find( point );
  if( P != matched_feature_list.end() ){
    return (*P).second.get_peak_area();
  }

  return -1.0;
  
}

//////////////////////////////////////////////////
// get feature at a certain LC-MS by LC_MS id
feature* feature::get_feature( int id){
  
  
  
  // check if its teh own:
  if( spectrum_ID == id ){
    return this;
  }
  
  map<int,feature>::iterator P = matched_feature_list.find( id );
  if( P != matched_feature_list.end() ){
    return &((*P).second);
  }
  
  return NULL;
  
}

//////////////////////////////////////////////////
// get the profile over all matched features:
map<int, double> feature::get_feature_profile(){

  double TOT_AREA = get_MATCHED_peak_area();
  map<int, double> PROFILE;
  PROFILE.insert( make_pair( get_spectrum_ID(), get_peak_area()/TOT_AREA ));
  
  map<int,feature>::iterator P = matched_feature_list.begin();
  while( P != matched_feature_list.end() ){
    PROFILE.insert( make_pair( (*P).second.get_spectrum_ID(), (*P).second.get_peak_area()/TOT_AREA ) );
    P++;
  }
  return PROFILE;
}


//////////////////////////////////////////////////
// Molecular Mass computed over matched features:
double feature::get_profile_Molecular_Mass(){

  double mz = get_MZ();
  map<int,feature>::iterator P = get_match_list_start();
  while( P != get_match_list_end() ){
    mz += (*P).second.get_MZ();
    P++;
  }
  mz /= double( get_replicate_match_nb() );
  // compute MR:
  double MR = mz * double(get_charge_state());
  // remove the H+'s
  MR -= ( get_MONO_H() * double(get_charge_state()) );
  
  return MR;
}

//////////////////////////////////////////////////
// retention time computed over matched features:
double feature::get_profile_retention_time(){
  double all_tr = TR;
  map<int,feature>::iterator P = get_match_list_start();
  while( P != get_match_list_end() ){
    all_tr += (*P).second.get_retention_time();
    P++;
  }
  all_tr /= double( get_replicate_match_nb() );
  return all_tr;
}



//////////////////////////////////////////////////
// check if MS2 info is available:
bool feature::get_MS2_info(){

  // check if list is empty:
  if( MS2_SCANS.empty() ){
    return false;
  }
  else{
    
    // check now if any ms2 info above PEP.PROP. THRESHOLD:
    map<double, vector<ms2_info> >::iterator F = MS2_SCANS.lower_bound( PEPTIDE_PROBABILITY_THRESHOLD );
    if( F == MS2_SCANS.end() ){
      return false;
    }
    else{
      return true;
    }
  }
}


//////////////////////////////////////////////////
// check if MS2 info is available:
bool feature::get_MS2_info(double PP_THRES){
  
  // check if list is empty:
  if( MS2_SCANS.empty() ){
    return false;
  }
  else{
    
    // check now if any ms2 info above PEP.PROP. THRESHOLD:
    map<double, vector<ms2_info> >::iterator F = MS2_SCANS.lower_bound( PP_THRES );
    if( F == MS2_SCANS.end() ){
      return false;
    }
    else{
      return true;
    }
  }
}


//////////////////////////////////////////////////
// writes out teh feature to a file:
void feature::print_2_file(ofstream* WRITER){
  
  double Pep_Proph_THRESHOLD = -3;
  char buffer[256];
  string SEP = "\t";
  feature* IN = this;
  
  /////////////////////////////////////////
  // print the general MS1_info:
  // print matched features:
  sprintf(buffer,"%0.4f%s",IN->get_MZ(),SEP.c_str()); 
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"%0.2f%s",IN->get_retention_time(),SEP.c_str()); 
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"+%d%s",IN->get_charge_state(),SEP.c_str());
  WRITER->write(buffer,strlen(buffer));
  
  /////////////////////////////////////////
  // write MS2 info:
  if( IN->get_MS2_info( Pep_Proph_THRESHOLD )){
    
    
    /////////////////////////////////////////////
    // print here all ACs out:
    if( PRINT_ALL_ACs ){
      
      ms2_info* info = IN->get_best_MS2_SCAN( Pep_Proph_THRESHOLD );
      string ACs;
      map<string, double>::iterator P = info->get_ALL_AC_START();
      while( P != info->get_ALL_AC_END() ){
        ACs += P->first + ",";
        P++;
      }
      // remove last comma:
      ACs.erase( ACs.size() - 1, 1 );
      sprintf(buffer,"%s%s",ACs.c_str(),SEP.c_str()); 
      WRITER->write(buffer,strlen(buffer));
    }
    else{
      sprintf(buffer,"%s%s",IN->get_AC( Pep_Proph_THRESHOLD ).c_str(),SEP.c_str()); 
      WRITER->write(buffer,strlen(buffer));
    }
    
    
    
    
    sprintf(buffer,"%s%s",IN->get_TOTAL_SQ( Pep_Proph_THRESHOLD ).c_str(), SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"%0.2f%s",IN->get_pep_prob( Pep_Proph_THRESHOLD ),SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
  }
  else{
    sprintf(buffer,"%s%s%s",SEP.c_str(),SEP.c_str(),SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
  }
  
  // print matched features list size::
  sprintf(buffer,"%d%s",IN->get_replicate_match_nb(),SEP.c_str()); 
  WRITER->write(buffer,strlen(buffer));
  
  /*
  //////////////////////////////////////////////////////
  // now print all peak areas for every id from 0-x
  // get max id:
  int max_ID = MASTER->get_nb_raw_specs()+1;
  for( int ID=0; ID < max_ID;ID++){
    
    if( IN->get_spectrum_ID() == ID){
      sprintf(buffer,"%0.2f%s", IN->get_peak_area(),SEP.c_str());
      WRITER->write(buffer,strlen(buffer));
    }
    else{ 
      map<int,feature>::iterator F = IN->find_match_by_id( ID);
      if( F != IN->get_match_list_end()){
        sprintf(buffer,"%0.2f%s", (*F).second.get_peak_area(),SEP.c_str());
        WRITER->write(buffer,strlen(buffer));
      }
      else{
        sprintf(buffer,"%s",SEP.c_str());
        WRITER->write(buffer,strlen(buffer));
      }
    }
  }
   */
  


}



//////////////////////////////////////////////////////
// check charge states, in cases where a feature was
// created based on a MS2 trace, charge state is unknown ( = -1 )
// -> derivce the charge state from the matched feature (if this is 
// also not -1 
void feature::deriveChargeStates( feature* IN ){
  
  feature* deriveFeature = NULL;
  feature* adjustFeature = NULL;
  
  // find the feature to derive:
  if( IN->get_charge_state() == -1 ){
    adjustFeature = IN;
  }
  else if( this->get_charge_state() == -1 ){
    adjustFeature = this;
  }
  
  // find the feature to derive:
  if( IN->get_charge_state() > 0 ){
    deriveFeature = IN;
  }
  else if( this->get_charge_state() > 0 ){
    deriveFeature = this;
  }
  
  
  // make a derived charge state:
  if( ( deriveFeature != NULL ) && ( adjustFeature != NULL ) ){
    
    adjustFeature->set_charge_state( deriveFeature->get_charge_state() ); 
    
    // adjust the mapped ones:
    map<int,feature>::iterator P = adjustFeature->get_match_list_start();
    while( P != adjustFeature->get_match_list_end()){
      P->second.set_charge_state( deriveFeature->get_charge_state() );
      P++;
    }
  }
  
  deriveFeature = NULL;
  adjustFeature = NULL;

  
}

//////////////////////////////////////////////////////
// compare to masses at the PPM value and decided
// if they fall into the m/z tolerance window
bool feature::compareFeatureMassValuesAtPPMLevel( double mzA, double mzB ){  
  return simple_math::compareMassValuesAtPPMLevel(mzA, mzB, feature::PPM_MZ_TOL );
}

//////////////////////////////////////////////////////
// get the masse error at the PPM value 
double feature::getFeatureMassErrorAtPPMLevel( double mz ){
  return simple_math::getMassErrorAtPPMLevel(mz, feature::PPM_MZ_TOL);
}  




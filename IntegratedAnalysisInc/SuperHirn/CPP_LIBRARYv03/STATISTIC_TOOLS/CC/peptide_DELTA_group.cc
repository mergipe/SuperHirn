///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS peptide_DELTA_group:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_PEPTIDE_DELTA_GROUP
#include "STATISTIC_TOOLS_USE.h"


int peptide_DELTA_group::NB_PROFILE_DATA_POINTS;
double peptide_DELTA_group::TR_TOL;
double peptide_DELTA_group::MOD_TR_TOL;
double peptide_DELTA_group::PEPTIDE_PROBABILITY_THRESHOLD;
bool peptide_DELTA_group::MOLECULAR_MASS_COMPARISON = false;

map<int, vector<double> > peptide_DELTA_group::LC_MS_Modification_Masses;

////////////////////////////////////////////////
// constructor for the object peptide_DELTA_group:
peptide_DELTA_group::peptide_DELTA_group(){
  TR = 0;
  pI = 0;
  MR = 0;
  MZ = 0;
  Z = -1;
  MASTER_ID = -1;
  PROFILE_PROBABILITY = 0;
  PROFILE_SCORE = 0;
  PROFILE_INTERNAL_SCORE = 0;
  PEPTIDE_GROUP_ID = -1;
}


////////////////////////////////////////////////
// constructor for the object peptide_DELTA_group:
peptide_DELTA_group::peptide_DELTA_group(feature* IN){
  TR = IN->get_profile_retention_time();
  MR = IN->get_profile_Molecular_Mass();
  MZ = IN->get_MZ();
  Z = IN->get_charge_state();
  MASTER_ID = IN->get_MASTER_ID();
  PROFILE_PROBABILITY = 0;
  pI = 0;
  PROFILE_SCORE = 0;
  PROFILE_INTERNAL_SCORE = 0;
  PEPTIDE_GROUP_ID = -1;
  // add teh feature to the group:
  add_feature( IN );
  IN = NULL;
}

//////////////////////////////////////////////////
// class desctructor of peptide_DELTA_group
peptide_DELTA_group::~peptide_DELTA_group(){
  MEMBERS.clear();
  MODIFIED_MEMBERS.clear();
  PROTEIN_ACs.clear();
  ALL_MS2_INFO.clear();
  AVERAGE_PEPTIDE_PROFILE.clear();
  AVERAGE_PEPTIDE_PROFILE_STD.clear();
}

//////////////////////////////////////////////////
// class copy constructor of peptide_DELTA_group
peptide_DELTA_group::peptide_DELTA_group(const peptide_DELTA_group& tmp){
  MR = tmp.MR;
  TR = tmp.TR;
  pI = tmp.pI;
  MZ = tmp.MZ;
  Z = tmp.Z;
  MEMBERS = tmp.MEMBERS;
  MODIFIED_MEMBERS = tmp.MODIFIED_MEMBERS;
  PROTEIN_ACs = tmp.PROTEIN_ACs;
  ALL_MS2_INFO = tmp.ALL_MS2_INFO;
  MASTER_ID = tmp.MASTER_ID;
  PEPTIDE_GROUP_ID = tmp.PEPTIDE_GROUP_ID;
  PROFILE_PROBABILITY = tmp.PROFILE_PROBABILITY;
  PROFILE_SCORE = tmp.PROFILE_SCORE;
  PROFILE_INTERNAL_SCORE = tmp.PROFILE_INTERNAL_SCORE;
  AVERAGE_PEPTIDE_PROFILE = tmp.AVERAGE_PEPTIDE_PROFILE;
  AVERAGE_PEPTIDE_PROFILE_STD = tmp.AVERAGE_PEPTIDE_PROFILE_STD;
}

//////////////////////////////////////////////////
// class copy constructor of peptide_DELTA_group
peptide_DELTA_group::peptide_DELTA_group(const peptide_DELTA_group* tmp){
  MR = tmp->MR;
  TR = tmp->TR;
  pI = tmp->pI;
  MZ = tmp->MZ;
  Z = tmp->Z;
  MEMBERS = tmp->MEMBERS;
  MODIFIED_MEMBERS = tmp->MODIFIED_MEMBERS;
  PROTEIN_ACs = tmp->PROTEIN_ACs;
  ALL_MS2_INFO = tmp->ALL_MS2_INFO;
  PEPTIDE_GROUP_ID = tmp->PEPTIDE_GROUP_ID;
  MASTER_ID = tmp->MASTER_ID;
  PROFILE_PROBABILITY = tmp->PROFILE_PROBABILITY;
  PROFILE_SCORE = tmp->PROFILE_SCORE;
  PROFILE_INTERNAL_SCORE = tmp->PROFILE_INTERNAL_SCORE;
  AVERAGE_PEPTIDE_PROFILE = tmp->AVERAGE_PEPTIDE_PROFILE;
  AVERAGE_PEPTIDE_PROFILE_STD = tmp->AVERAGE_PEPTIDE_PROFILE_STD;
}

//////////////////////////////////////////////////
// copy constructor:
peptide_DELTA_group& peptide_DELTA_group::operator=(const peptide_DELTA_group& tmp){
  MR = tmp.MR;
  TR = tmp.TR;
  pI = tmp.pI;
  MZ = tmp.MZ;
  Z = tmp.Z;
  MEMBERS = tmp.MEMBERS;
  MODIFIED_MEMBERS = tmp.MODIFIED_MEMBERS;
  PROTEIN_ACs = tmp.PROTEIN_ACs;
  ALL_MS2_INFO = tmp.ALL_MS2_INFO;
  MASTER_ID = tmp.MASTER_ID;
  PEPTIDE_GROUP_ID = tmp.PEPTIDE_GROUP_ID;
  PROFILE_PROBABILITY = tmp.PROFILE_PROBABILITY;
  PROFILE_SCORE = tmp.PROFILE_SCORE;
  PROFILE_INTERNAL_SCORE = tmp.PROFILE_INTERNAL_SCORE;
  AVERAGE_PEPTIDE_PROFILE = tmp.AVERAGE_PEPTIDE_PROFILE;
  AVERAGE_PEPTIDE_PROFILE_STD = tmp.AVERAGE_PEPTIDE_PROFILE_STD;
  return *this;
}

//////////////////////////////////////////////////
// equal operator:
bool peptide_DELTA_group::operator==( peptide_DELTA_group& tmp){
  
  if( feature::compareFeatureMassValuesAtPPMLevel( get_MR(), tmp.get_MR() )){
    double DELTA_TR = fabs( get_TR() - tmp.get_TR() );    
    if( DELTA_TR <= TR_TOL ){
      return true;
    }
  }
  else{
    return false;
  }
  
  return false;

}

//////////////////////////////////////////////////
// operators:
bool peptide_DELTA_group::operator<(const peptide_DELTA_group& tmp){
  
  if( tmp.MR > get_MR()){
    return true;
  }
  else{
    return false; 
  }
}

//////////////////////////////////////////////////
// check if this feature belongs to the group:
bool peptide_DELTA_group::check_belonging(feature* IN){
  
  if( IN == NULL ){
    return false;
  }
  
  ////////////////////////////////////////////////////////////////
  // check first on ms2 level:
  // only if high probability:
  ms2_info* this_TMP = get_BEST_MS2_SCAN();
  if( ( IN->get_MS2_info() ) && ( this_TMP != NULL) ){
    ms2_info* TMP = IN->get_best_MS2_SCAN();
    if( TMP->get_SQ() != this_TMP->get_SQ() ){
      return false;
    }
    else if( TMP->get_SQ() == this_TMP->get_SQ() ){
      return true;
    }
  }
  
  
  ////////////////////////////////////////////////////////////////
  // now check possible MR + Modification values:
  vector<double>* Masses = get_LC_MS_Modification_Masses( IN->get_spectrum_ID() );
  //map<int, vector<double> >::iterator F = LC_MS_Modification_Masses.find( IN->get_spectrum_ID() );
  if( Masses == NULL ){
    
    if( MOLECULAR_MASS_COMPARISON ){
      if( compare_MR( IN->get_profile_Molecular_Mass() , IN->get_retention_time(), 0.0 ) ){
        return true;
      }
    }
    else{
      if( compare_MZ( IN->get_MZ() , IN->get_retention_time(), 0.0 , IN->get_charge_state()) ){
        return true; 
      }
    }
  }
  else{
    vector<double>::iterator M = Masses->begin();
    while( M != Masses->end() ){ 
      if( MOLECULAR_MASS_COMPARISON ){
        if( compare_MR( IN->get_profile_Molecular_Mass() , IN->get_retention_time(), *M ) ){
          return true;        
        }
      }
      else{
        if( compare_MZ( IN->get_MZ() , IN->get_retention_time(), *M , IN->get_charge_state()) ){
          return true;
        }
      }
      
      M++;
    }
  }
  
  return false;
}


///////////////////////////////////////////////////////
// build average peptide cluster profile:
void peptide_DELTA_group::build_peptide_average_consens(){

  AVERAGE_PEPTIDE_PROFILE.clear();
  consens_profile_builder* BUILD = new consens_profile_builder( NB_PROFILE_DATA_POINTS );
  
  ////////////////////////////////
  // merge all members
  vector<feature*> ALL = MEMBERS;
  
  ///////////////////////////////
  // merge all modifed members
  map<double, vector<feature*> >::iterator M = get_modified_members_list_start();
  while( M != get_modified_members_list_end() ){
    vector<feature*> TMP = ALL;
    ALL.clear();
    merge( TMP.begin(), TMP.end(), (*M).second.begin(), (*M).second.end(), back_inserter( ALL ) );
    TMP.clear();
    M++;
  }

  //////////////////////////////
  // build average profile:
  map< int, pair<double, double> > TMP = BUILD->build_AVERAGE_CONSENS( &ALL );
  
  //////////////////////////////
  // transform the profile:
  AVERAGE_PEPTIDE_PROFILE.clear();
  AVERAGE_PEPTIDE_PROFILE_STD.clear();
  map< int, pair<double, double> >::iterator P = TMP.begin();
  while( P != TMP.end() ){
    AVERAGE_PEPTIDE_PROFILE.insert( make_pair( (*P).first, (*P).second.first) );   
    AVERAGE_PEPTIDE_PROFILE_STD.insert( make_pair( (*P).first, (*P).second.second) );   
    P++;
  }
  
  delete BUILD;
  BUILD = NULL;
  ALL.clear();
}


///////////////////////////////////////////////////////
// get the averaged TIC over all LC/MS point
double peptide_DELTA_group::get_peptide_TOTAL_TIC( ){
  
  vector<feature*> ALL = MEMBERS;
  map<double, vector<feature*> >::iterator M = get_modified_members_list_start();
  while( M != get_modified_members_list_end() ){
    vector<feature*> TMP = ALL;
    ALL.clear();
    merge( TMP.begin(), TMP.end(), (*M).second.begin(), (*M).second.end(), back_inserter( ALL ) );
    TMP.clear();
    M++;
  }
  
  vector<feature*>::iterator A = ALL.begin();
  double TOTAL = 0;
  while( A != ALL.end() ){
    TOTAL += (*A)->get_replicate_intensity_sum();
    A++;
  }
  ALL.clear();
  
  return TOTAL;
}

///////////////////////////////////////////////////////
// get the averaged TIC at a LC/MS point
pair<double,double> peptide_DELTA_group::get_peptide_TOTAL_TIC( int ID){
  pair<double,double> OUT;
  
  vector<feature*> ALL = MEMBERS;
  map<double, vector<feature*> >::iterator M = get_modified_members_list_start();
  while( M != get_modified_members_list_end() ){
    vector<feature*> TMP = ALL;
    ALL.clear();
    merge( TMP.begin(), TMP.end(), (*M).second.begin(), (*M).second.end(), back_inserter( ALL ) );
    TMP.clear();
    M++;
  }
  
  vector<feature*>::iterator A = ALL.begin();
  vector<double> AREAS;
  double TOTAL = 0;
  while( A != ALL.end() ){
    
    feature* MY_F = (*A);
    if( MY_F->check_match_by_id( ID ) ){
      AREAS.push_back( MY_F->get_peak_area( ID ) );
      TOTAL +=  MY_F->get_peak_area( ID );
    }
    A++; 
  }
  ALL.clear();
  
  if( AREAS.empty() ){
    OUT.first = 0.0;
    OUT.second = 0.0;
  }
  else{
    simple_math MATH;
    OUT = MATH.AVERAGE_and_STDEV( &AREAS );
  }
  OUT.first = TOTAL;
  OUT.second = 0.0;
  return OUT;
}

//////////////////////////////////////////////////
// check if this feature belongs to the group:
bool peptide_DELTA_group::check_belonging(peptide_DELTA_group* IN){
  
  if( IN == NULL ){
    return false;
  }
  
  // check first on ms2 level:
  // only if high probability:
  ms2_info* TMP = IN->get_BEST_MS2_SCAN();
  ms2_info* this_TMP = get_BEST_MS2_SCAN();
  if( ( TMP != NULL) && ( this_TMP != NULL) ){
    if( ( TMP->get_PEP_PROB() >= PEPTIDE_PROBABILITY_THRESHOLD) && ( TMP->get_SQ() != this_TMP->get_SQ() ) ){
      return false;
    }
  }
  bool HIT_CHECK = false;
  
  ////////////////////////////////////////////////////////////////
  // now check possible MR + Modification values:
  vector<double>* Masses = get_LC_MS_Modification_Masses( IN->get_MASTER_ID() );
  if( Masses == NULL ){
    if( compare_MR( IN->get_MR() , IN->get_TR(), 0.0 ) ){
      HIT_CHECK = true;
    }
  }
  else{
    vector<double>::iterator M = Masses->begin();
    while( M != Masses->end() ){ 
      if( compare_MR( IN->get_MR() , IN->get_TR(), *M ) ){
        HIT_CHECK = true;        
      }
      M++;
    }
  }
  
  return HIT_CHECK;
}  


////////////////////////////////////////////////////////////////
// get the delta mass list for a specific ID
vector<double>* peptide_DELTA_group::get_LC_MS_Modification_Masses( int LCMSid ){
  
  if( !LC_MS_Modification_Masses.empty() ){
  
    // search first specific mass list:
    map<int, vector<double> >::iterator F = LC_MS_Modification_Masses.find( LCMSid );
    if( F != LC_MS_Modification_Masses.end() ){
      return &( (*F).second );
    }
    // assuming that one mass list counts for all the runs:
    else if( LC_MS_Modification_Masses.size() == 1 ){
      F = LC_MS_Modification_Masses.find( 0 );
      if( F != LC_MS_Modification_Masses.end() ){
        return &( (*F).second );
      }
    }
  }
  
  return NULL;
}



///////////////////////////////////////////////////////////////////////
// get the average peptide ratio between 2 points:
vector<double> peptide_DELTA_group::get_peptide_ratio(int ID_A, int ID_B){
  
  double RATIO = 0;
  vector<double> ALL_RATIOS;
  vector<feature*>::iterator P = MEMBERS.begin();
  while( P != MEMBERS.end() ){
    
    double AREA_A = 0;
    double AREA_B = 0;
    feature* FEA = (*P);
    
    // find A:
    if( FEA->get_spectrum_ID() == ID_A){
      AREA_A = FEA->get_peak_area();
    }
    else{
      map<int, feature>::iterator F = FEA->find_match_by_id( ID_A );
      if( F != FEA->get_match_list_end() ){
        AREA_A = (*F).second.get_peak_area(); 
      }
    }
    // find B:
    if( FEA->get_spectrum_ID() == ID_B){
      AREA_B = FEA->get_peak_area(); 
    }
    else{
      map<int, feature>::iterator F = FEA->find_match_by_id( ID_B );
      if( F != FEA->get_match_list_end() ){
        AREA_B = (*F).second.get_peak_area(); 
      }
    }
    
    if( ( AREA_A > 0) && ( AREA_B > 0) ){
      RATIO  = ( AREA_A / AREA_B );
      ALL_RATIOS.push_back( RATIO );
    }
    
    FEA = NULL;
    P++;
  }

  return ALL_RATIOS;
  
}


///////////////////////////////////////////////////////////////////////
// get the peptide ratio for labelled peptides:
vector<double> peptide_DELTA_group::get_peptide_ratio( double DELTA_SHIFT){
  
  double RATIO = 0;
  vector<double> ALL_RATIOS;
  vector<feature*>::iterator P = MEMBERS.begin();
  while( P != MEMBERS.end() ){
    
    feature* FEA = (*P);
    double AREA_A = FEA->get_peak_area();
    double AREA_B = 0.0;
    
    vector<feature*>::iterator N = P;
    N++;
    while( N != MEMBERS.end() ){
      
      // see if a features with this shift present:
      double this_shift = fabs( FEA->get_MZ() - (*N)->get_MZ() );
      if( feature::compareFeatureMassValuesAtPPMLevel(this_shift, DELTA_SHIFT )){
        // cout<<FEA->get_MZ()<<" "<<(*N)->get_MZ()<<endl;
        // FEA->show_info();
        
        AREA_B = (*N)->get_peak_area();
        break;
      }
      N++;
    }
    
    if( ( AREA_A > 0) && ( AREA_B > 0) ){
      RATIO  = ( AREA_A / AREA_B );
      ALL_RATIOS.push_back( RATIO );
    }
    
    FEA = NULL;
    P++;
  }
  
  return ALL_RATIOS;
  
}


///////////////////////////////////////////////////////////////////////
// set modifications of peptide members:
void peptide_DELTA_group::set_PEPTIDE_MODIFICATIONS(){
  
  ////////////////////////////////////////
  // check list of available shifts:
  // if shifts are bigger than one (1), smaller(-1) or no shifts allowed(0)
  int MOD_SHIFT_FORM = 0;  
  map<int, vector<double> >::iterator F = LC_MS_Modification_Masses.find( 0 );
  if( F != LC_MS_Modification_Masses.end() ){
    vector<double>::iterator P = (*F).second.begin();
    while( P != (*F).second.end() ){
      if( (*P) > 0){
        MOD_SHIFT_FORM = 1;
      }
      if( (*P) < 0){
        MOD_SHIFT_FORM = -1;
      }
      P++;
    }
  }
  
  if( MOD_SHIFT_FORM != 0 ){
  
    ////////////////////////////////////////
    // find first the natural smallest(1) / biggest(-1) Molecular Mass, depending on MOD_SHIFT_FORM:
    double NAT_MOL_MR = 0;
    double MOD_CHECK = false;
    vector<feature*>::iterator X = MEMBERS.begin(); 
    while(X != MEMBERS.end()){
      bool MOD_FEA = false;
      feature* tmp = (*X);
      // check if there is a shift:
      if( feature::compareFeatureMassValuesAtPPMLevel( MR, tmp->get_profile_Molecular_Mass()) ){

        // check what shift form is allowed:
        switch( MOD_SHIFT_FORM ){
          case 1:
            if( MR > (tmp->get_profile_Molecular_Mass() + feature::getFeatureMassErrorAtPPMLevel( tmp->get_profile_Molecular_Mass() ) ) ){
              NAT_MOL_MR = tmp->get_profile_Molecular_Mass();
              MOD_CHECK = true;
            }     
            else{
              MOD_FEA = true;
            }
            break;
            
          case -1:
            if( MR < (tmp->get_profile_Molecular_Mass() - feature::getFeatureMassErrorAtPPMLevel( tmp->get_profile_Molecular_Mass() ) ) ){
              NAT_MOL_MR = tmp->get_profile_Molecular_Mass();
              MOD_CHECK = true;
            }    
            else{
              MOD_FEA = true;
            }
            break;
        }
      }    
      
      if( MOD_FEA ){        
        // insert into the modifications list:
        add_modified_feature( tmp );
        
        // erase from old list:
        X = MEMBERS.erase( X );
      }
      else{
        X++; 
      }    
    }
    
  
    if( MOD_CHECK ){
      
      // reset the Molecular Mass of the peptide group:
      MR = NAT_MOL_MR;
      
      ////////////////////////////////////////
      // no reorder the feature into mod/non mod peptides:
      X = MEMBERS.begin(); 
      while(X != MEMBERS.end()){

        feature* tmp = (*X);
        // check if there is a shift:
        if( feature::compareFeatureMassValuesAtPPMLevel( MR, tmp->get_profile_Molecular_Mass() ) ){
          
          // insert into the modifications list:
          add_modified_feature( tmp );
          
          // erase from old list:
          X = MEMBERS.erase( X );
        }
        else{
          X++;
        }
      }
      
    }
  }  
}


///////////////////////////////////////////////////////////////////////
// display function:
void peptide_DELTA_group::show_members(){
  
  printf("-----------------------------\n");
  printf("Peptide Group: Mr = %0.2f, Tr = %0.2f, Nr. members = %d, Modification = %d\n", MR, TR, MEMBERS.size(), get_modification_state() );
  printf("MEMBERS::\n");
  vector<feature*>::iterator X = MEMBERS.begin(); 
  while(X != MEMBERS.end()){
    (*X)->show_info();
    X++; 
  }
  printf("-----------------------------\n");
  
}


///////////////////////////////////////////////////////////////////////
// show info
void peptide_DELTA_group::show_info(){
  printf("\tMr=%0.2f, TR=%0.2f", MR, TR );
  if( get_BEST_MS2_SCAN(-3.0) ){
    printf(", SQ=%s(p=%0.2f)", get_MOD_SQ(-3.0).c_str(), get_PEP_PROB(-3.0) );
  }
  printf("\n");
  
}


//////////////////////////////////////////////////
// add features of a group into this one::
void peptide_DELTA_group::merge_peptide_DELTA_group(peptide_DELTA_group* IN){
  
  vector<feature*>::iterator P = IN->get_members_list_start();
  while( P != IN->get_members_list_end() ){
    add_feature( (*P) );
    P++;
  }
  
  IN = NULL;
}


///////////////////////////////////////////////////////////////////////
// add feature:
void peptide_DELTA_group::add_feature(feature* IN){
    
  // check MS2 info:
  if( IN->get_MS2_info() ){
    
    ms2_info* INFO = IN->get_best_MS2_SCAN();
    ALL_MS2_INFO.push_back( INFO );
    map< string, double >::iterator A = INFO->get_ALL_AC_START();
    while( A != INFO->get_ALL_AC_END()){
    
      // see if this ACs has been observed already:
      map<string, vector<ms2_info*> >::iterator F = PROTEIN_ACs.find( A->first );
      if( F == PROTEIN_ACs.end() ){
        vector<ms2_info*> tmp;
        tmp.push_back( INFO );
        PROTEIN_ACs.insert( make_pair( A->first, tmp ) );
      }
      else{
        (*F).second.push_back( INFO );
      }
      A++;
    }
    
    INFO = NULL;
  }
  
  MEMBERS.push_back(IN);
  IN = NULL;
  
}


///////////////////////////////////////////////////////////////////////
// add modified feature:
void peptide_DELTA_group::add_modified_feature(feature* IN){
  
  double shift = IN->get_profile_Molecular_Mass() - MR;
  if(!MODIFIED_MEMBERS.empty() ){
  
    map<double, vector<feature*> >::iterator P = MODIFIED_MEMBERS.begin();
    while( P != MODIFIED_MEMBERS.end() ){
    
      if( feature::compareFeatureMassValuesAtPPMLevel( shift, (*P).first )){
        (*P).second.push_back( IN );
        break;
      }
      P++;
    }
    
    if( P == MODIFIED_MEMBERS.end() ){
      vector<feature*> tmp;
      tmp.push_back( IN );
      MODIFIED_MEMBERS.insert( make_pair( shift, tmp) );
    }
  }
  else{
    vector<feature*> tmp;
    tmp.push_back( IN );
    MODIFIED_MEMBERS.insert( make_pair( shift, tmp) );
  }
  
  IN = NULL;
  
}


//////////////////////////////////////////////////////////
// write into XML formated text into file:
void peptide_DELTA_group::write_content_2_XML( LC_MS_XML_writer* IN ){
  
  LC_MS_XML_writer* W_OUT = IN;
  ofstream* WRITER = W_OUT->get_WRITER();
  
  char buffer[500];
  sprintf(buffer,"<PEPTIDE_GROUP ");  
  
  //////////////////////
  // main info
  sprintf(buffer, "%s Mr=\"%f\" Tr=\"%f\" pI=\"%f\" Nr_members=\"%d\" profile_score=\"%f\" profile_probability=\"%f\" modification_state=\"%d\" peptide_group_ID=\"%d\" proteotypic=\"%d\">\n",buffer, MR, TR, pI, MEMBERS.size(), PROFILE_SCORE, PROFILE_PROBABILITY, get_modification_state(), get_PEPTIDE_GROUP_ID(), get_PROTEO_TYPE() );
  WRITER->write(buffer,strlen(buffer));
    
  //////////////////////
  // write all the peptide assignments:
  sprintf(buffer,"<PEPTIDE_MS2_ASSIGNMENTS>\n");  
  WRITER->write(buffer,strlen(buffer));
  vector<ms2_info*>::iterator I = ALL_MS2_INFO.begin();
  while( I != ALL_MS2_INFO.end() ){
    W_OUT->write_ms2_info_2_XML_tag( (*I) );
    I++;
  }
  sprintf(buffer,"</PEPTIDE_MS2_ASSIGNMENTS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  //////////////////////
  // now write all the features:
  sprintf(buffer,"<FEATURE_MEMBERS>\n");  
  WRITER->write(buffer,strlen(buffer));
  vector<feature*>::iterator X = MEMBERS.begin(); 
  while(X != MEMBERS.end()){
    W_OUT->write_feature_2_XML_tag( (*X) );
    X++; 
  }
  sprintf(buffer,"</FEATURE_MEMBERS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  //////////////////////
  // now write all the modified features:    
  sprintf(buffer,"<MODIFIED_FEATURE_MEMBERS>\n");  
  WRITER->write(buffer,strlen(buffer));
  map<double, vector<feature*> >::iterator M = get_modified_members_list_start(); 
  while(M != get_modified_members_list_end()){
    
    // write modification group:
    sprintf(buffer,"<PEPTIDE_MODIFICATION_GROUP delta=\"%f\" nb_members=\"%d\">\n", (*M).first, (*M).second.size());  
    WRITER->write(buffer,strlen(buffer));    
    
    X = (*M).second.begin();
    while( X != (*M).second.end() ){ 
      W_OUT->write_feature_2_XML_tag( (*X) );
      X++; 
    }
    sprintf(buffer,"</PEPTIDE_MODIFICATION_GROUP>\n");  
    WRITER->write(buffer,strlen(buffer));
    
    M++;
  }
  sprintf(buffer,"</MODIFIED_FEATURE_MEMBERS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  //////////////////////
  // write the consens profile:
  sprintf(buffer,"<CONSENS_PROFILE>\n");  
  WRITER->write(buffer,strlen(buffer));
  // check if consens profiles are already build:
  if( AVERAGE_PEPTIDE_PROFILE.empty() ){
    build_peptide_average_consens();
  }    
  map<int, double>::iterator P = AVERAGE_PEPTIDE_PROFILE.begin();
  map<int, double>::iterator P_STDEV = AVERAGE_PEPTIDE_PROFILE_STD.begin();
  while( P != AVERAGE_PEPTIDE_PROFILE.end() ){      
    sprintf(buffer,"<DATA_POINT MASTER_ID=\"%d\" x=\"%d\" y=\"%f\" y_stdev=\"%f\"></DATA_POINT>\n", MASTER_ID, (*P).first, (*P).second, (*P_STDEV).second);  
    WRITER->write(buffer,strlen(buffer));
    P++;
    P_STDEV++;
  }
  sprintf(buffer,"</CONSENS_PROFILE>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  // end tag
  sprintf(buffer,"</PEPTIDE_GROUP>\n");
  WRITER->write(buffer,strlen(buffer));
  WRITER->flush();

  
}

//////////////////////////////////////////////////////////
// write into APML formated text into file:
void peptide_DELTA_group::write_content_2_APML( LC_MS_APML_writer* IN ){
  
  LC_MS_APML_writer* W_OUT = IN;
  ofstream* WRITER = W_OUT->get_WRITER();
  
  char buffer[500];
  sprintf(buffer,"<PEPTIDE_GROUP ");  
  
  //////////////////////
  // main info
  sprintf(buffer, "%s Mr=\"%f\" Tr=\"%f\" pI=\"%f\" Nr_members=\"%d\" profile_score=\"%f\" profile_probability=\"%f\" modification_state=\"%d\" peptide_group_ID=\"%d\" proteotypic=\"%d\">\n",buffer, MR, TR, pI, MEMBERS.size(), PROFILE_SCORE, PROFILE_PROBABILITY, get_modification_state(), get_PEPTIDE_GROUP_ID(), get_PROTEO_TYPE() );
  WRITER->write(buffer,strlen(buffer));
  
  //////////////////////
  // write all the peptide assignments:
  sprintf(buffer,"<PEPTIDE_MS2_ASSIGNMENTS>\n");  
  WRITER->write(buffer,strlen(buffer));
  W_OUT->write_ms2_info_2_APML_tag( &ALL_MS2_INFO );
  sprintf(buffer,"</PEPTIDE_MS2_ASSIGNMENTS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  //////////////////////
  // now write all the features:
  sprintf(buffer,"<FEATURE_MEMBERS>\n");  
  WRITER->write(buffer,strlen(buffer));
  W_OUT->write_aligned_feature_2_APML_tag( &MEMBERS );
  sprintf(buffer,"</FEATURE_MEMBERS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  //////////////////////
  // now write all the modified features:    
  sprintf(buffer,"<MODIFIED_FEATURE_MEMBERS>\n");  
  WRITER->write(buffer,strlen(buffer));
  map<double, vector<feature*> >::iterator M = get_modified_members_list_start(); 
  while(M != get_modified_members_list_end()){
    
    // write modification group:
    sprintf(buffer,"<PEPTIDE_MODIFICATION_GROUP delta=\"%f\" nb_members=\"%d\">\n", (*M).first, (*M).second.size());  
    WRITER->write(buffer,strlen(buffer));    
    W_OUT->write_aligned_feature_2_APML_tag( &( (*M).second ) );    
    sprintf(buffer,"</PEPTIDE_MODIFICATION_GROUP>\n");  
    WRITER->write(buffer,strlen(buffer));
    
    M++;
  }
  sprintf(buffer,"</MODIFIED_FEATURE_MEMBERS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  //////////////////////
  // write the consens profile:
  sprintf(buffer,"<CONSENS_PROFILE>\n");  
  WRITER->write(buffer,strlen(buffer));
  // check if consens profiles are already build:
  if( AVERAGE_PEPTIDE_PROFILE.empty() ){
    build_peptide_average_consens();
  }    
  map<int, double>::iterator P = AVERAGE_PEPTIDE_PROFILE.begin();
  map<int, double>::iterator P_STDEV = AVERAGE_PEPTIDE_PROFILE_STD.begin();
  while( P != AVERAGE_PEPTIDE_PROFILE.end() ){      
    sprintf(buffer,"<DATA_POINT MASTER_ID=\"%d\" x=\"%d\" y=\"%f\" y_stdev=\"%f\"></DATA_POINT>\n", MASTER_ID, (*P).first, (*P).second, (*P_STDEV).second);  
    WRITER->write(buffer,strlen(buffer));
    P++;
    P_STDEV++;
  }
  sprintf(buffer,"</CONSENS_PROFILE>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  // end tag
  sprintf(buffer,"</PEPTIDE_GROUP>\n");
  WRITER->write(buffer,strlen(buffer));
  WRITER->flush();
  
  
}

//////////////////////////////////////////////////////////
// write into XML formated text into file:
void peptide_DELTA_group::write_content_2_TXT( ofstream* WRITER ){
  
  char buffer[500];
  string SEP = "\t";
  
  
  
  //////////////////////
  // main peptide info 
  sprintf(buffer, "%0.3f%s%0.2f%s%d%s", MR, SEP.c_str(), TR, SEP.c_str(), MEMBERS.size(),SEP.c_str());
  WRITER->write(buffer,strlen(buffer));
  
  //////////////////////
  // write the charge state m/z of features:
  int max_z = 3;
  int z = 1;
  while( z <= max_z ){
    
    feature* MY = NULL;
    // find a feature which has this charge state:
    vector<feature*>::iterator X = MEMBERS.begin(); 
    while(X != MEMBERS.end()){
      if( (*X)->get_charge_state() == z ){
        MY = (*X);
        break; 
      }
      X++; 
    }
        
    map<double, vector<feature*> >::iterator M = MODIFIED_MEMBERS.begin();
    while(  ( M != MODIFIED_MEMBERS.end() ) && ( MY == NULL ) ){

      X = (*M).second.begin();
      while(X != MEMBERS.end()){
        if( (*X)->get_charge_state() == z ){
          MY = (*X);
          break; 
        }
        X++;
      }      
      M++;
    }
    
    if( MY != NULL ){
      sprintf(buffer, "%0.3f%s", MY->get_MZ() ,SEP.c_str());
      WRITER->write(buffer,strlen(buffer));
    }
    else{
      sprintf(buffer, "%s", SEP.c_str());
      WRITER->write(buffer,strlen(buffer));
    }
    
    z++;
  }
  
  
  // print the profile:
  map<int, double>::iterator P = AVERAGE_PEPTIDE_PROFILE.begin();
  while( P != AVERAGE_PEPTIDE_PROFILE.end() ){      
    sprintf(buffer,"%0.3f%s", (*P).second, SEP.c_str());  
    WRITER->write(buffer,strlen(buffer));
    P++;
  }

  
  sprintf(buffer,"\n");  
  WRITER->write(buffer,strlen(buffer));
  WRITER->flush();
}


//////////////////////////////////////////////////////////
// write into a formated text for each feature:
void peptide_DELTA_group::write_Feature_content_2_TXT( ofstream* WRITER ){
  
  char buffer[500];
  string SEP = "\t";
  double Pep_Proph_THRESHOLD = -3.0;
  
  //////////////////////
  // write the non modified members:
  vector<feature*>::iterator X = MEMBERS.begin(); 
  while(X != MEMBERS.end()){

    feature* IN = (*X);

    /////////////////////////////////////////
    // print the general MS1_info:
    sprintf(buffer,"%0.4f%s",IN->get_MZ(),SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"%0.2f%s",IN->get_retention_time(),SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"+%d%s",IN->get_charge_state(),SEP.c_str());
    WRITER->write(buffer,strlen(buffer));
    
    /////////////////////////////////////////
    // write MS2 info:
    if( IN->get_MS2_info( Pep_Proph_THRESHOLD )){
      sprintf(buffer,"%s%s",IN->get_AC( Pep_Proph_THRESHOLD ).c_str(),SEP.c_str()); 
      WRITER->write(buffer,strlen(buffer));
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
    
    ///
    // scoring:
    sprintf(buffer,"%0.2f%s",PROFILE_PROBABILITY,SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"%0.2f%s",PROFILE_SCORE,SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    
    sprintf(buffer, "\n");
    WRITER->write(buffer,strlen(buffer));
    
    X++; 
  }
  
  WRITER->flush();
}


//////////////////////////////////////////////////////////////////////////////
// check with members if 2 features from with same LC/MS ,
// if so take the one which is closest to 
void peptide_DELTA_group::filter_MEMBERS(){}


//////////////////////////////////////////////////////////////////////////////
// check if a protein by AC is in the cluster:
bool peptide_DELTA_group::Protein_CHECK(string AC){
  
  map<string, vector<ms2_info*> > ::iterator M = PROTEIN_ACs.find( AC );
  if( M != PROTEIN_ACs.end() ){   
    return true;
  }
    
  return false;
}


///////////////////////////////////////////////////////////////////////
// write out to inclusion list
void peptide_DELTA_group::write_2_inclusion_list(ofstream* WRITER){
  char buffer[255];
  
  vector<feature*>::iterator X = MEMBERS.begin(); 
  while(X != MEMBERS.end()){
    feature* tmp = (*X);
    sprintf(buffer,"%f\t%f\t%d\t%d\t%f\n", tmp->get_MZ(), tmp->get_retention_time(), tmp->get_MASTER_ID(), tmp->get_charge_state(), tmp->get_MATCHED_peak_area());
    WRITER->write(buffer, strlen(buffer));
    X++; 
  }
  
}

//////////////////////////////////////////////////////////////////////////
// get the best observed MS2 scan ( highest probability )
ms2_info* peptide_DELTA_group::get_BEST_MS2_SCAN(){
  
  ms2_info* BEST_SCAN = NULL;
  
  map<string, vector<ms2_info*> >::iterator L = PROTEIN_ACs.begin();
  while( L != PROTEIN_ACs.end()){
    
    vector<ms2_info*>::iterator P = (*L).second.begin();
    while( P != (*L).second.end() ){
      
      if( BEST_SCAN == NULL ){
        BEST_SCAN = (*P);
      }
      else{
      
        // filter only above the pep.prob thershold
        if( BEST_SCAN->get_PEP_PROB() > (*P)->get_PEP_PROB() ){
          BEST_SCAN = (*P);
        }
      }
      
      P++;
    }
    L++;
  }
  
  return BEST_SCAN;
  
}

//////////////////////////////////////////////////////////////////////////
// get the best observed MS2 scan above input threshold( highest probability )
ms2_info* peptide_DELTA_group::get_BEST_MS2_SCAN(double THRESHOLD){
  
  ms2_info* BEST_SCAN = NULL;
  
  vector<feature*>::iterator P = MEMBERS.begin();
 while( P != MEMBERS.end()){
    
   ms2_info* TMP = (*P)->get_best_MS2_SCAN( THRESHOLD );
   
   if( TMP != NULL ){
     if( BEST_SCAN == NULL ){
       BEST_SCAN = TMP;
     }
     else{
       // filter only above the pep.prob thershold
       if( BEST_SCAN->get_PEP_PROB() > TMP->get_PEP_PROB( ) ){
         BEST_SCAN = TMP;
       }
     }
   }
   
   P++;
 }
 
 return BEST_SCAN;
 
}


//////////////////////////////////////////////////////////////////////////
// get all observed MS2 scan above input threshold( highest probability )
vector<ms2_info*> peptide_DELTA_group::get_ALL_MS2_SCANS(double THRESHOLD){
  
  vector<ms2_info*> ALL_SCANS;
  
  vector<feature*>::iterator P = MEMBERS.begin();
  while( P != MEMBERS.end()){
        
    map<double, vector<ms2_info> >::iterator MS2 = (*P)->get_MS2_SCANS_START();
    while( MS2 != (*P)->get_MS2_SCANS_END() ){
      if( (*MS2).first >=THRESHOLD ){
        vector<ms2_info>::iterator M = (*MS2).second.begin();
        while( M != (*MS2).second.end() ){
          ALL_SCANS.push_back( &(*M) );
          M++;
        }
      }
      
      MS2++;
    }
    
    P++;
  }
  
  return ALL_SCANS;
}



//////////////////////////////////////////////////
// check a mz matches using the mz of this peptide_group
bool peptide_DELTA_group::compare_MZ(double IN_mz, double IN_TR, double MOD_SHIFT, int IN_z){
  
  if( IN_z == Z ){
    
    ////////////////////////////////////////////////////////////////
    // now check non modified Mr 
    if( MOD_SHIFT == 0.0 ){
    
      double delta_Tr = fabs( TR - IN_TR);            
      if( feature::compareFeatureMassValuesAtPPMLevel( MZ, IN_mz ) ){
        if( ( TR_TOL >= delta_Tr) ){
          return true;
        }
      }
      
      return false;
    }
    
    ////////////////////////////////////////////////////////////////
    // now check possible MR + Modification values:
    else{
      
      // include teh chartge state:
      MOD_SHIFT = MOD_SHIFT / (double)Z;
      
      double delta_Tr = fabs( TR - IN_TR);  
      if( MOD_TR_TOL >= delta_Tr ){
        
        double MOD_MZ_UP = IN_mz + MOD_SHIFT;
        double MOD_MZ_DOWN = IN_mz - MOD_SHIFT;
        
        if( feature::compareFeatureMassValuesAtPPMLevel( MOD_MZ_UP, MZ) ){
          return true;
        }
        else{
          if( feature::compareFeatureMassValuesAtPPMLevel( MOD_MZ_DOWN, MZ) ){
            return true;
          }
        }
      }
    }
  }
  return false;
}


//////////////////////////////////////////////////
// check a MR matches the MR of this peptide_group
bool peptide_DELTA_group::compare_MR(double IN_MR, double IN_TR, double MOD_SHIFT){
  
  
  ////////////////////////////////////////////////////////////////
  // now check non modified Mr 
  if( MOD_SHIFT == 0.0 ){
    
    double delta_Tr = fabs( TR - IN_TR);  
    if( feature::compareFeatureMassValuesAtPPMLevel( MR, IN_MR)){
      if( ( TR_TOL >= delta_Tr) ){
        return true;
      }
    }
    
    return false;
  }
  
  ////////////////////////////////////////////////////////////////
  // now check possible MR + Modification values:
  else{
    
    double delta_Tr = fabs( TR - IN_TR);  
    if( MOD_TR_TOL >= delta_Tr ){
  
      double MOD_MR_UP = IN_MR + MOD_SHIFT;
      double MOD_MR_DOWN = IN_MR - MOD_SHIFT;
      
      if( feature::compareFeatureMassValuesAtPPMLevel( MOD_MR_UP, MR) ){
        return true;
      }
      else{
        if( feature::compareFeatureMassValuesAtPPMLevel( MOD_MR_DOWN, MR) ){
          return true;
        }
      }
    }
  }
  
  return false;
}



////////////////////////////////////////////////////////////////////////
// check if its proteotypic:
bool peptide_DELTA_group::get_PROTEO_TYPE(){
  
  ms2_info* INFO = get_BEST_MS2_SCAN();
  if( INFO != NULL ){
    return INFO->get_PROTEO_TYPE();
  }
  else{
    return true;
  }
}


////////////////////////////////////////////////////////////////////////
// get all members ( modifed also) from the group
vector<feature*> peptide_DELTA_group::get_ALL_FEATURE_MEMBERS(){
  
  ////////////////////////////////
  // merge all members
  vector<feature*> ALL = MEMBERS;
  
  ///////////////////////////////
  // merge all modifed members
  map<double, vector<feature*> >::iterator M = get_modified_members_list_start();
  while( M != get_modified_members_list_end() ){
    vector<feature*> TMP = ALL;
    ALL.clear();
    merge( TMP.begin(), TMP.end(), (*M).second.begin(), (*M).second.end(), back_inserter( ALL ) );
    TMP.clear();
    M++;
  }
  
  return ALL;
}


///////////////////////////////////////////////////////////////////////
// get the smallest charge state of all feature members:
int peptide_DELTA_group::get_min_charge_state(){
  
  int MIN = 1000;
  vector<feature*>::iterator X = MEMBERS.begin(); 
  while(X != MEMBERS.end()){
    if( (*X)->get_charge_state() < MIN ){
      MIN = (*X)->get_charge_state();
    }
    X++; 
  }
  map<double, vector<feature*> >::iterator M = get_modified_members_list_start(); 
  while(M != get_modified_members_list_end()){    
    X = (*M).second.begin();
    while( X != (*M).second.end() ){ 
      if( (*X)->get_charge_state() < MIN ){
        MIN = (*X)->get_charge_state();
      }
      X++; 
    }
    M++;
  }
  return MIN;
}





////////////////////////////////////////////////////////////////////////
// build full ion current over peptide modification vairants
void peptide_DELTA_group::build_modification_resolved_feature_areas(){
    
  ///////////////////////////////
  // merge the areas of modifed members into its non modified feature
  map<double, vector<feature*> >::iterator M = get_modified_members_list_start();
  while( M != get_modified_members_list_end() ){
    
    vector<feature*>::iterator F = (*M).second.begin();
    while( F != (*M).second.end() ){
      
      // check if non modified form exists:
      bool mod_delete = false;
      vector<feature*>::iterator N = get_members_list_start();
      while( N != get_members_list_end() ){
      
        // compare the charge states:
        if( (*N)->get_charge_state() == (*F)->get_charge_state() ){
          // ok, the are the same charge state,
          // sum the peak areas up 
          merge_feature_areas( (*N), (*F) );
          mod_delete = true;
          break;
        }
        N++;
      }
      
      // now erase the modified matched ones:
      if( mod_delete ){
        F = (*M).second.erase( F );
      }
      else{
        F++;
      }
    }
        
    if( (*M).second.empty() ){
      // MODIFIED_MEMBERS.erase( M );
    }
    M++;
  }
  
}

////////////////////////////////////////////////////////////////////////
// merge the fetaure areas over the replicates
void peptide_DELTA_group::merge_feature_areas( feature* TARGET, feature* SOURCE ){
  
  double add_area = SOURCE->get_peak_area( TARGET->get_spectrum_ID() );
  if( add_area >= 0 ){
    add_area += TARGET->get_peak_area( );
    TARGET->set_peak_area( add_area );
  }
  
  // now for the matched features:
  map<int,feature>::iterator M = TARGET->get_match_list_start();
  while( M != TARGET->get_match_list_start() ){

    add_area = SOURCE->get_peak_area( (*M).first );
    if( add_area >= 0 ){
      add_area += (*M).second.get_peak_area( ); 
      (*M).second.set_peak_area( add_area );
    }
    
    M++;
  }
  
  TARGET = NULL;
  SOURCE = NULL;
}

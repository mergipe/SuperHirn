///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS Spec LC_MS:
// LC-MS spectra
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_IO
#define USE_Mono_H
#define USE_STDIO
#define USE_STDLIB
#define USE_VECTOR
#define USE_TIME
#define USE_MAP
#define USE_FEATURE
#define USE_ALGORITHM
#define USE_FUNCTIONAL
#define USE_NAMESPACE
#define USE_LC_MS

#include "GENERAL_USE.h"
double LC_MS::MINIMAL_PEP_PROPHET_THERSHOLD = -3.0;
double LC_MS::PEP_PROPHET_THERSHOLD;


////////////////////////////////////////////////
// constructor 
LC_MS::LC_MS(){
  spectrum_id = -1;
  MASTER_ID = -1;
}


////////////////////////////////////////////////
// constructor for the object LC_MS:
LC_MS::LC_MS(string IN_name){
  spec_name = IN_name;
  spectrum_id = -1;
  MASTER_ID = -1;
}

//////////////////////////////////////////////////
// copy constructor:
LC_MS::LC_MS(const LC_MS* tmp){
  spec_name = tmp->spec_name;
  spectrum_id = tmp->spectrum_id;
  raw_spec_names = tmp->raw_spec_names;
  MASTER_ID = tmp->MASTER_ID;
  ALIGNMENT_ERROR = tmp->ALIGNMENT_ERROR;
  feature_list = tmp->feature_list;
}

//////////////////////////////////////////////////
// copy constructor:
LC_MS::LC_MS(const LC_MS& tmp){
  spec_name = tmp.spec_name;
  spectrum_id = tmp.spectrum_id;
  raw_spec_names = tmp.raw_spec_names;
  MASTER_ID = tmp.MASTER_ID;
  ALIGNMENT_ERROR = tmp.ALIGNMENT_ERROR;
  feature_list = tmp.feature_list;
}

//////////////////////////////////////////////////
// copy constructor:
LC_MS& LC_MS::operator=(const LC_MS& tmp){
  
  spec_name = tmp.spec_name;
  spectrum_id = tmp.spectrum_id;
  raw_spec_names = tmp.raw_spec_names;
  MASTER_ID = tmp.MASTER_ID;
  ALIGNMENT_ERROR = tmp.ALIGNMENT_ERROR;
  feature_list = tmp.feature_list;
  
  return *this;
}

//////////////////////////////////////////////////
// class desctructor
LC_MS::~LC_MS(){
  
  //  destroy the feature list:
  if(!feature_list.empty())
    feature_list.clear();
  
  if(!raw_spec_names.empty())
    raw_spec_names.clear();
 
  ALIGNMENT_ERROR.clear();
  
}


//////////////////////////////////////////////////
// order the features according to parent mass:
void LC_MS::order_by_mass(){
  sort(feature_list.begin(),feature_list.end(),OPERATOR_MZ());
}


//////////////////////////////////////////////////
// show the content of the spectra:
void LC_MS::show_info(){
  
  if( get_spec_name().size() > 0)
    printf("\t\t\tLC-MS name: %s ", get_spec_name().c_str() );
  else
    printf("\t\t\tLC-MS ID: %d,", get_spectrum_ID());
  
  if( get_nb_raw_specs() != 0){
    printf("[MASTER MAP ID=%d] ", get_MASTER_ID()); 
  }
  else{
    printf("[LC-MS ID=%d] ", get_spectrum_ID()); 
  }
  
  printf(" #features: %d, #MS/MS ids: %d (no Thresholding: %d)\n", get_nb_features(), get_nb_identified_features(), get_nb_identified_features( MINIMAL_PEP_PROPHET_THERSHOLD ));
  
  // show the child LC/MS runs:
  map<int, string>::iterator C = get_raw_spec_name_start();
  while( C != get_raw_spec_name_end() ){
    printf("\t\t\t\t - Child LC-MS: %s [ID=%d]\n", (*C).second.c_str(), (*C).first);    
    C++;
  }
  

  vector<LC_MS_FEATURE>::iterator p = feature_list.begin();
  while(p != feature_list.end()){
    // if((*p).get_MS2_info()){
    //(*p).show_info();
    //}
    p++;
  }	
}



//////////////////////////////////////////////////
// search the list of feature for the one with input ID:
feature* LC_MS::find_feature_by_ID(int ID){
  
  vector<LC_MS_FEATURE>::iterator p = feature_list.begin();
  while( p != feature_list.end() ){
    
    if( ID == (*p).get_feature_ID() ){
      return &(*p);
    }
    
    p++;
  }
  
  return NULL;
}


 

//////////////////////////////////////////////////
// count the number of common peaks of a given number of LC-MS:
// ONLY count the one which appear count-times
int LC_MS::get_nb_common_peaks(int count){
  
 // search for the others:
 int common_count = 0;

  feature* PEAK = NULL;
  vector<LC_MS_FEATURE>::iterator p = feature_list.begin();
  while( p != feature_list.end()){
    
    // get the peak at a charge state:
    PEAK = &(*p);
    
   if(PEAK != NULL){
     if(PEAK->get_nb_common_match() == count){
       common_count++;
     }
   }
   // next feature
   p++;
  }
  
  return common_count;
}
 

//////////////////////////////////////////////////
// print the content of the LC/MS run to a file:
void LC_MS::print_2_file(string OUT){

  string file_name = OUT;
  file_name += get_spec_name() + ".txt";
  
  // open a file, etc:
  ofstream writer;
  writer.open(file_name.c_str(), ofstream::out | ofstream::trunc); 
  
  if(writer.good()){
    
    vector<LC_MS_FEATURE>::iterator p = get_feature_list_begin();
    while( p != get_feature_list_end()){
      (*p).print_2_file(&writer);
      p++;
    }
    
    // printf("\n-> LC_MS content written to file %s\n",file_name.c_str());
    // close now the file:
    writer.close();
       
  }
  else{
    printf("\nERROR: MASTER LC/MS run could not write out to '%s', LC_MS::553\n",file_name.c_str());
  }

}


//////////////////////////////////////////////////////////////////
// remove a feature from teh LC/MS run:
void LC_MS::remove_feature(feature* IN){
  vector<LC_MS_FEATURE>::iterator P = find(feature_list.begin(),feature_list.end(), IN );
  if(P != feature_list.end()){
    P->show_info();
    P = feature_list.erase(P);
  }
  else{
    // IN->show_info();
    // printf("\nERROR: FEATURE could not be found in LC/MS run::LC_MS::483\n");
  }
}


//////////////////////////////////////////////////////////////////
// remove a feature from the LC/MS run by ID:
void LC_MS::remove_feature_by_ID(feature* IN){
  remove_feature_by_ID( IN->get_feature_ID() );
}

//////////////////////////////////////////////////////////////////
// remove a feature from the LC/MS run by ID:
void LC_MS::remove_feature_by_ID(int ID){
  vector<LC_MS_FEATURE>::iterator P = feature_list.begin();
  while(P != feature_list.end()){
    if( P->get_feature_ID() == ID ){
      feature_list.erase(P);
      break;
    }
    
    P++;
    
  }
}
  



  

///////////////////////////////////////////////////////////
// get alignment error at specific TR:
void LC_MS::get_alignment_error(double IN, double* UP, double* DOWN){

  if(!ALIGNMENT_ERROR.empty()){
    
    map<double, pair<double,double> >::iterator P = ALIGNMENT_ERROR.lower_bound(IN);

    // exact match:
    if((*P).first == IN){
      *UP = (*P).second.first;
      *DOWN = (*P).second.second;      
    }
    
    // is bigger than any element:
    else if(P == ALIGNMENT_ERROR.end()){
      P--;
      *UP = (*P).second.first;
      *DOWN = (*P).second.second;      
    }

    // is smaller than any element:
    else if(P == ALIGNMENT_ERROR.begin()){
      *UP = (*P).second.first;
      *DOWN = (*P).second.second;      
    }

    else{      
      // is within the range of the list but has not found an exact match
      // -> extrapolate between 2 data points:
      double up_E_u = (*P).second.first;
      double down_E_u = (*P).second.second;
      double TR_u = (*P).first;
      
      P--;
      
      double up_E_d = (*P).second.first;
      double down_E_d = (*P).second.second;
      double TR_d = (*P).first;
      
      double w_u = (IN - TR_d)/(TR_u - TR_d);
      double w_d = (TR_u - IN)/(TR_u - TR_d);
      
      *UP = w_u * up_E_u + w_d * up_E_d;
      *DOWN = w_u * down_E_u + w_d * down_E_d;
    }
    
  }
}





///////////////////////////////////////////////////////////////
// check if this LC/MS ID is present in the raw LC/MS runs 
bool LC_MS::find_LC_MS_by_ID( int ID){
  
  map<int, string>::iterator F = raw_spec_names.find( ID );
  if( F != raw_spec_names.end() ){
    return true;
  }
  else{
    return false;
  }
}



///////////////////////////////////////////////////////////////
// compare the LC/MS runs names 
bool LC_MS::check_LCMS_name( string IN){
  
  if( spec_name.find( IN ) != string::npos ){
    return true;
  }
  
  map<int, string>::iterator F = raw_spec_names.begin(  );
  while( F != raw_spec_names.end() ){
    if( (*F).second.find( IN ) != string::npos ){
      return true;
    }
    F++;
  }
  
  return false;
}

//////////////////////////////////////////////////
// set the id of all features
void LC_MS::setFeatureLCMSID(){
  
  vector<LC_MS_FEATURE>::iterator p = feature_list.begin();
  while(p != feature_list.end()){
    (*p).set_spectrum_ID( get_spectrum_ID() );
    p++;
  }	
}



void LC_MS::set_spectrum_ID(int iID)
{
  this->spectrum_id = iID;
  
  vector<LC_MS_FEATURE>::iterator F = this->get_feature_list_begin();
  while( F != this->get_feature_list_end() )
    {
      F->set_spectrum_ID( this->get_spectrum_ID() );
    F++;
  }
  
}

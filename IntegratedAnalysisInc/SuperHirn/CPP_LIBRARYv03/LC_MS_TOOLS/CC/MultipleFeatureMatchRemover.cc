///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS MultipleFeatureMatchRemover:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_MULTIPLE_FEATURE_MATCH_REMOVER
#include "LC_MS_TOOLS_USE.h"


int MultipleFeatureMatchRemover::max_nb_stripes;

////////////////////////////////////////////////
// constructor for the object MultipleFeatureMatchRemover:
MultipleFeatureMatchRemover::MultipleFeatureMatchRemover(vector<LCMS_alignment_feature_match>* iData){
  DATA = iData;
}

//////////////////////////////////////////////////
// class desctructor of MultipleFeatureMatchRemover
MultipleFeatureMatchRemover::~MultipleFeatureMatchRemover(){
  DATA = NULL;
}


//////////////////////////////////////////////////
// copy constructor:
MultipleFeatureMatchRemover& MultipleFeatureMatchRemover::operator=(const MultipleFeatureMatchRemover& tmp){
  return *this;
}

////////////////////////////////////////////////////////////
// sorts the saved LCMS_alignment_feature_match according to retention time B
void MultipleFeatureMatchRemover::sort_LCMS_alignment_feature_match_by_Retention_To_Be_Aligned(){
  sort(DATA->begin(),DATA->end(), OPERATOR_Retention_To_Be_Aligned());
}


///////////////////////////////////////////////////////
// starts the smoothing process:
// first smooth by TB, then by TA
void MultipleFeatureMatchRemover::start_smoothing(){
  
  // smoothing by TB
  filter_by_Retention_To_Be_Aligned();  
}




///////////////////////////////////////////////////////
// remove the high abundance in spectrum B:
// - sort according to retention time TB (=x)
// - filter out those which have too many LCMS_alignment_feature_match at a TB
void MultipleFeatureMatchRemover::filter_by_Retention_To_Be_Aligned(){

  // order first according TB:
  sort_LCMS_alignment_feature_match_by_Retention_To_Be_Aligned();
  
  double start_TR = 0;
  double tmp_TR = 0;
  int same_tr_count = 0;
  
  vector<LCMS_alignment_feature_match>::iterator it = DATA->begin();
  vector<LCMS_alignment_feature_match>::iterator start = DATA->begin();
  
  if(start != DATA->end()){
    start_TR = (*start).get_TR_ToAlign();
    it++;
  }
  
  while(it != DATA->end()){
    
    tmp_TR = (*it).get_TR_ToAlign();
    
    // check if a new retention time is found
    if(start_TR != tmp_TR){
			
      // check if number of same retention time found is bigger
      // than max_nb_stripes
      if( same_tr_count > max_nb_stripes ){
        // now cycle through the list and remove retention time LCMS_alignment_feature_match
        // which have too many LCMS_alignment_feature_match:
        it = smooth_high_abundance_by_index( start, it-1 );
      }
  
      start = it;
      start_TR = (*start).get_TR_ToAlign();
      same_tr_count = 0;
      
    }
    else{
      same_tr_count++;
    }
    it++;
  }
  
  if(same_tr_count > max_nb_stripes){
    // now cycle through the list and remove retention time LCMS_alignment_feature_match
    // which have too many LCMS_alignment_feature_match:
    // it = smooth_high_abundance_by_index(start, DATA.end());
  }
  
    
}



///////////////////////////////////////////////////////
// smooths the high abundance peptides out
// gets the start and end of the stripes
vector<LCMS_alignment_feature_match>::iterator MultipleFeatureMatchRemover::smooth_high_abundance_by_index(vector<LCMS_alignment_feature_match>::iterator start, vector<LCMS_alignment_feature_match>::iterator end){

  // new array which stores the "potential high abundance LCMS_alignment_feature_match" 
  vector<LCMS_alignment_feature_match> high_data;
  
  // copy the elements from the orginal DATA:
  high_data.insert( high_data.begin(), start, end);
  
  // order now the sub-vector according to index_B
  sort( high_data.begin(), high_data.end(), OPERATOR_index_B());
  
  // find the apex match in this subvector
  get_apex_in_sublist(start, &high_data);
  
  // remove the old vector:
  high_data.clear();
  
  // remove the original data:
  if( end != DATA->end() ) {
    end++;
  }
  
  //  start++;
  start = DATA->erase( start, end);
  
  return start;
  
}

///////////////////////////////////////////////////////
// calculated the weighted peak of this
// long eluting peptide
void MultipleFeatureMatchRemover::get_apex_in_sublist(vector<LCMS_alignment_feature_match>::iterator TARGET_pos, vector<LCMS_alignment_feature_match>* subMatches){
  
  vector<LCMS_alignment_feature_match>::iterator start = subMatches->begin();
  vector<LCMS_alignment_feature_match>::iterator end = subMatches->end();
  vector<LCMS_alignment_feature_match>::iterator p = start;

  // calculate weighted centered TR
  double tot_area = 0; 
  double sum = 0;
  double max_area = 0;
  while(p != end){
    tot_area += (*p).get_peak_area_Reference();
    if( tot_area > max_area ){
      max_area = tot_area;
    }
    sum += (*p).get_peak_area_ToAlign() * (*p).get_TR_ToAlign();
    p++;
  }
  
  double new_TR_A = sum / tot_area;  
  
  // set the new means parameters:
  (*TARGET_pos).set_TR_ToAlign(new_TR_A);
  (*TARGET_pos).set_peak_area_ToAlign( max_area );
  
  subMatches = NULL;
}


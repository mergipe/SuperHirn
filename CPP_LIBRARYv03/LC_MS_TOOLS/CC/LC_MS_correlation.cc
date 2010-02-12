///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS Spec Correlations:
// compare of the input LC-MS spectra
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//
#define USE_LC_MS_CORRELATION
#include "LC_MS_TOOLS_USE.h"

double LC_MS_correlation::tr_tol;
double LC_MS_correlation::mz_tol;
bool LC_MS_correlation::VIEW;
double LC_MS_correlation::min_align_error;
double LC_MS_correlation::max_align_error;
double LC_MS_correlation::min_LC_MS_score;
int LC_MS_correlation::intensity_bin_size;
double LC_MS_correlation::intensity_bin_tolerance;

////////////////////////////////////////////////
// constructor for the object ana_summarizer:
LC_MS_correlation::LC_MS_correlation( SM_cell* IN ){
	
  align_object = NULL;
  P_get_alignment_error_to_extern = NULL;  
  SCORE = IN;	
}

//////////////////////////////////////////////////
// class desctructor
LC_MS_correlation::~LC_MS_correlation(){

  clear_variables();
  align_object = NULL;
  P_get_alignment_error_to_extern = NULL;
  SCORE = NULL;
}

////////////////////////////////////////////////
// set the access to the retention time error:
void LC_MS_correlation::set_alignment_error_access(void *pinstance, void(*pfct_get_alignment_error_to_extern)(void *pInstance, double IN, double* UP, double* DOWN)){
  // instance of spec_manager:
  align_object = pinstance;
  // instance of the function of spec_manager:
  P_get_alignment_error_to_extern = pfct_get_alignment_error_to_extern;		
  
}


//////////////////////////////////////////////////
// empty the list of common peaks
void LC_MS_correlation::clear_variables(){
  
  LCMS_A = NULL;
  LCMS_B = NULL;
  nb_feature_A = 0;
  nb_feature_B = 0;

  UP_error = 0;
  DOWN_error = 0;
  
  if(!peaks_A.empty())
    peaks_A.clear();
  
  if(!peaks_B.empty())
    peaks_B.clear();
  
  if(!intensity_rank_A.empty())
    intensity_rank_A.clear();
  
  if(!intensity_rank_B.empty())
    intensity_rank_B.clear();
  
  if(!rank_A.empty())
    rank_A.clear();
  
  if(!rank_B.empty())
    rank_B.clear();
  
  mean_A = 0;
  mean_B = 0;
  
  /////////////////////////////////////////////
  // different scores:
  total_score = 0;
  intensity_score = 0;
  alignment_error_score = 0;
  normalize_score = 0;
  OVERLAP_SCORE = 0;
  
  if(!align_up_error.empty())
    align_up_error.clear();
  
  if(!align_down_error.empty())
    align_down_error.clear();
  
  
}


////////////////////////////////////////////////
// compares for common lc_peak in two
// LC/MS A and B
void LC_MS_correlation::correlate_two_spectra(LC_MS* IN_A, LC_MS* IN_B){
  
  /////////////////////////////////////////////
  // clear the common peak list:
  clear_variables();
  
  LCMS_A = IN_A;
  nb_feature_A = LCMS_A->get_nb_features();
  LCMS_B = IN_B;
  nb_feature_B = LCMS_B->get_nb_features();
  
  
  ////////////////////////////////////////////
  // extract the overlapping features:
  MERGER = new LC_MS_merger();  
  // set the runs to be merged:
  MERGER->set_input_spectra(IN_A, IN_B);  
  // start merging
  MERGER->start_extracting_LCMS_overlap();
  // get the merged LC-MS now:
  LC_MS* OVERLAP_LCMS = MERGER->get_merged_spectrum();
  // add the overlaping features to the list:
  add_common_feature( OVERLAP_LCMS );
  // delete the merger
  delete MERGER;
  MERGER = NULL;
  
  
  ///////////////////////////////////////////////
  // create vectors of all intensities
  process_intensity_ranking();
  
  /////////////////////////////////////////////
  //compute_spearman correlation
  compute_spearman_correlation();
  
  /////////////////////////////////////////////
  // combine now the scores:
  combine_scores();

  ////////////////////////////////////////////
  // normalizes the total_score so it is more clear to 
  // o discriminate between bad / good run
  normalize_scores();

  ////////////////////////////////////////////
  // print detailed results:
  if(VIEW){
    print_detailed_analysis();
  }
  
  // delete the overlap/merged run:
  if( OVERLAP_LCMS ){
    delete OVERLAP_LCMS;
    OVERLAP_LCMS = NULL;
  }
  
  LCMS_A = NULL;
  LCMS_B = NULL;
  IN_A = NULL;
  IN_B = NULL;

}

/////////////////////////////////////////////////////////////
// build up the common feature list:
void LC_MS_correlation::add_common_feature( LC_MS* IN){
  
  vector<LC_MS_FEATURE>::iterator FEA = IN->get_feature_list_begin();
  while( FEA != IN->get_feature_list_end() ){
    if( (*FEA).get_match_list_start() != (*FEA).get_match_list_end() ){
      map<int, feature>::iterator B = (*FEA).get_match_list_start();
      add_common_feature( &( *FEA ), &( (*B).second ) ); 
    }
    FEA++;
  }
}

/////////////////////////////////////////////////////////////
// combine the different score into the total_score
void LC_MS_correlation::combine_scores(){  
  
  // only the overlap score: 
  // total_score = fabs(1200 - get_nb_common_peaks());
  // total_score /= 1200;
    
  // only intensity score:
  // total_score = get_intensity_score();
  
  // only alignment score:
  // total_score = alignment_error_score;
  
  // compute overlap:
  double common = get_nb_common_peaks();
  double all = double(nb_feature_A + nb_feature_B );
  OVERLAP_SCORE = 2.0 * common / all; 
  
  total_score = OVERLAP_SCORE * get_intensity_score( );
  SCORE->set_TOTAL_SCORE( total_score );
  SCORE->set_INT_SCORE( get_intensity_score() );
  SCORE->set_ALIGN_SCORE( OVERLAP_SCORE );
  SCORE->set_MATCH_SCORE( common );
  
}	

/////////////////////////////////////////////////////////////
// normalizes the total_score so it is more clear to 
// o discriminate between bad / good runs
void LC_MS_correlation::normalize_scores(){

  /*
  if( total_score <= minimal_LC_MS_score){
    normalize_score = 0.00001;
  }
  else{
    normalize_score  = (total_score - minimal_LC_MS_score) / ( 1.0 - minimal_LC_MS_score );
  }	
   */
  SCORE->set_NORM_SCORE( SCORE->get_TOTAL_SCORE( ) );
  
}


//////////////////////////////////////////////////////////////////////////
// create vectors of all intensities
void LC_MS_correlation::process_intensity_ranking(){
    
  if( !intensity_rank_A.empty() ){
    
    mean_A /= intensity_rank_A.size();
    sort( intensity_rank_A.begin(), intensity_rank_A.end());
    create_intensity_rank_list( &intensity_rank_A, &rank_A ); 
    
  }
  
  if( !intensity_rank_B.empty() ){
    
    mean_B /= intensity_rank_B.size();    
    sort( intensity_rank_B.begin(), intensity_rank_B.end());
    create_intensity_rank_list( &intensity_rank_B, &rank_B ); 
  }
    
}


///////////////////////////////////////////////////////////////////////////////////
// bin the list of intensities and create ranking accorindgly:
void LC_MS_correlation::create_intensity_rank_list( vector<double>* INTENS, vector<double>* RANKS){
  
  vector<double>::iterator P1 = INTENS->begin();
  vector<double>::iterator P2 = P1;
  
  bool DUB_CHECK = false;
  double Rank = 1;
  
  while(P1 != INTENS->end()){
    P2 = P1;
    P2++;
    
    int dub_count = 1;
    double rank_mean = Rank;
    
    // check if the following ones are in the same intensity:
    while( P2 != INTENS->end() ){
      if(*P1 != *P2){
        break;
      }
      Rank++;
      rank_mean += Rank;
      dub_count++;
      P2 = INTENS->erase(P2);
      DUB_CHECK = true;
    }
    
    if( DUB_CHECK ){
      DUB_CHECK = false;
      rank_mean = rank_mean / dub_count;
      RANKS->push_back(rank_mean);	  
      P1 = P2;
      Rank++;
    }
    else{             	
      RANKS->push_back( Rank );                    
      Rank++;
      P1++; 
    }
  }
  
  INTENS = NULL;
  RANKS = NULL;
}



///////////////////////////////////////////////////////////////////////////////////
// compute spearman correlation
// r = A / B, where
// A = sum[ ( Ri - Rmean)*(Si - Smean)]
// B = sqrt[sum( Ri - Rmean)^2] * sqrt[sum( Si - Smean)^2]
// Ri = rank of peak i in spectra A
// Si = rank of peak i in spectra B
// Rmean = rank of the mean
// Smean = rank of the mean
//
void LC_MS_correlation::compute_spearman_correlation(){
  
  // go through all the matched peaks:
  vector<feature*>::iterator pA = peaks_A.begin();
  vector<feature*>::iterator pB = peaks_B.begin();
  
  double RC = 0;
  double DIV_A = 0;
  double DIV_B = 0;
  double NUM = 0;
  double rank_A = 0;
  double rank_B = 0;
  double rank_mean_A = get_intensity_rank_A(mean_A);
  double rank_mean_B = get_intensity_rank_B(mean_B);
  
  
  while(pA != peaks_A.end()){
    
    // get the rank of a matched lc-peak pair:
    rank_A = get_intensity_rank_A((*pA)->get_peak_area());
    rank_B = get_intensity_rank_B((*pB)->get_peak_area());
    
    // print the alignent error:
    // cout<<" "<<(*pB).get_alignment_SD()<<endl;
    
    NUM += ( (rank_A - rank_mean_A) * ( rank_B - rank_mean_B) );
    DIV_A += (rank_A - rank_mean_A)*(rank_A - rank_mean_A);
    DIV_B += (rank_B - rank_mean_B)*(rank_B - rank_mean_B);
    
    pA++;
    pB++;
  }
  
  // take teh square root
  DIV_A = sqrt(DIV_A);
  DIV_B = sqrt(DIV_B);
  
  if(DIV_A*DIV_B > 0)
    // combine the factors:
    RC = NUM / ( DIV_A * DIV_B);
  
  intensity_score = RC;
}

///////////////////////////////////////////////////////////////////////////////////
// get intensity rank by peak area A
double LC_MS_correlation::get_intensity_rank_A(double IN){
  
  //print_ranking_A();
  
  vector<double>::iterator p = lower_bound(intensity_rank_A.begin(),intensity_rank_A.end(),IN);
  
  if(p != intensity_rank_A.end()){
    int pos = p - intensity_rank_A.begin();
    p = rank_A.begin() + pos;
    return *p;
  }
  else{
    return intensity_rank_A.size();
  }
  
}

///////////////////////////////////////////////////////////////////////////////////
// get intensity rank by peak area B
double LC_MS_correlation::get_intensity_rank_B(double IN){
  
  vector<double>::iterator p = lower_bound(intensity_rank_B.begin(),intensity_rank_B.end(),IN);
  
  if(p != intensity_rank_B.end()){
    int pos =   p - intensity_rank_B.begin();
    p = rank_B.begin() + pos;
    // cout<<IN<<": "<<*p<<endl;
    return *p;
  }
  else{
    return intensity_rank_B.size();
  }
}



/////////////////////////////////////////////////////////////////////////////////////
// print ranking A
void LC_MS_correlation::print_ranking_A(){
  vector<double>::iterator p = intensity_rank_A.begin();
  vector<double>::iterator b = rank_A.begin();
  
  while(p != intensity_rank_A.end()){
    cout<<*p<<" - "<<*b<<endl;
    p++;
    b++;
  }

}

/////////////////////////////////////////////////////////////////////////////////////
// print ranking B
void LC_MS_correlation::print_ranking_B(){
  vector<double>::iterator p = intensity_rank_B.begin();
  vector<double>::iterator b = rank_B.begin();
  
  while(p != intensity_rank_B.end()){
    cout<<*p<<" - "<<*b<<endl;
    p++;
    b++;
  }
  
}

/////////////////////////////////////////////////////////////////////////////////////
// print the detailed output of the similarity scoring
void LC_MS_correlation::print_detailed_analysis(){
	
  printf("\n\tLC-MS Similarity of:\n\t\t %s[%d, %d] vs. %s[%d, %d]:\n", SCORE->get_X_name().c_str(), SCORE->get_X(), nb_feature_A, SCORE->get_Y_name().c_str(), SCORE->get_Y(), nb_feature_B);
  printf("\t\t- nb. common peaks: %0.3f\n", SCORE->get_MATCH_SCORE());
  printf("\t\t- feature overlap: %0.3f\n", SCORE->get_ALIGN_SCORE());
  printf("\t\t- intensity correlation: %0.3f\n", SCORE->get_INT_SCORE());
  printf("\t\t- MAIN LC/MS SIMILARITY SCORE: %0.3f\n", SCORE->get_TOTAL_SCORE());


	
}

/////////////////////////////////////////////////////////////////////////////////////
// get the upper / lower error for a given retenetion time:
void LC_MS_correlation::get_alignment_error(double IN){
	
  // check if there has been an object assign to get the errors
  if(align_object == NULL){
    UP_error = tr_tol;
    DOWN_error = tr_tol;
  }
  else{
    // get the errors from the aligner object:
    P_get_alignment_error_to_extern(align_object, IN, &UP_error, &DOWN_error);
  }
}


/////////////////////////////////////////////////////////////////////////////////////
// add an alignment error:
void LC_MS_correlation::add_alignment_error(){
  align_up_error.push_back(UP_error);
  align_down_error.push_back(DOWN_error);
}


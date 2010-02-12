///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS ALIGNER:
// ALIGNMENT of the LC-MS spectra pair:
//
// according to Silva et al, 2005, "Quantitative Protemice Analysis by
// Accurate Mass Retention Time Pairs"
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#define USE_AMRTP_LC_MS_ALIGNER

#include "LC_MS_TOOLS_USE.h"




////////////////////////////////////////////////
// constructor for the
AmrtpLCMSAligner::AmrtpLCMSAligner(): AbstractLCMSAligner(){
  PLOT = NULL;
}

////////////////////////////////////////////////
// constructor for the
AmrtpLCMSAligner::AmrtpLCMSAligner(bool IN_plot):AbstractLCMSAligner(IN_plot){  
  PLOT = NULL;
}

//////////////////////////////////////////////////
// class desctructor
AmrtpLCMSAligner::~AmrtpLCMSAligner(){  
  clear_up();
}


//////////////////////////////////////////////////
// starts the preprocessing of the LCMS maps:
void AmrtpLCMSAligner::preprocessLCMSMaps(){
  
  // oder by mass:
  get_REFERENCE_LCMSMap( )->order_by_mass();
  get_ALIGNED_LCMSMap()->order_by_mass();
  
  // extract common features:
  extract_common_features();
  
  // remove the multiple matches:
  remove_multiple_LCMS_alignment_feature_match();
  
  // add MS1 and MS2 matches:
  // combine_MS1_and_MS2_LCMS_alignment_feature_match();
  
  ////////////////////////////////////////////////
  // save the alignment error in the LC/MS run B:
  // save_alignment_error_in_B();
  
}



//////////////////////////////////////////////////
// starts the modeling of the retention time drift:
void AmrtpLCMSAligner::modelRetentionTimeDrift(){
  
  ////////////////////////////////////////////////
  // A. smooth now the scatter plot with lowess:
  ITERATION_COUNT = 1;
  while( !CHECK_ALIGNMENT_CONVERGENCE() ){
    ITERATIVE_LOWESS_SMOOTH();
    ITERATION_COUNT++;
  }
  
}



////////////////////////////////////////////////
// starts an alignemnt iteration of the LC-MS A to B:
// 1. smoothes the Tr vs delta TR scatterplot with lowess procedure:
// 2. adjusts the TR in spectrum B with the obtained delta_TrS
void AmrtpLCMSAligner::ITERATIVE_LOWESS_SMOOTH(){

  match_list = raw_match_list;

  //////////////////////////////////////////////////////////////////
  // print and plot functions of the 
  // raw matches:
  if( print_out ){
    // print out the plot delta TR vs TR:
    PRINT_RAW_MATCHES();
  }
  
  //////////////////////////////////////////////////////////////////
  // B. smoothes the Tr vs delta TR scatterplot with lowess procedure:
  if( !match_list.empty() ){
    // do now a lowess filter
    // do not sort again since they should
    // already be sorted
    FeatureMatchRetentionTimeRegressor* analyst = new FeatureMatchRetentionTimeRegressor( &match_list );
    // normal lowess without bootstrap:
    analyst->start_lowess_fit();
    
    delete analyst;
    analyst = NULL;     
  }
  else{
    printf("\t*** Empty Tr vs deltaTr data to model alignment shift pair(%d,%d) ****\n", get_REFERENCE_LCMSMap( )->get_spectrum_ID(), get_ALIGNED_LCMSMap( )->get_spectrum_ID() );
  }
  
  //////////////////////////////////////////////////////////////////
  // print out the raw matches:
  if( print_out ){
    PRINT_ALIGNMENT();
  }
  
}



//////////////////////////////////////////////////
// A. extract common features between LC/MS runs
void AmrtpLCMSAligner::extract_common_features(){
  
  // set the iterator which controls matching of feature in B to A
  // to the start of the feature list in LC/MS run A
  MERGE_ITERATOR_REFERENCE_LCMS = get_REFERENCE_LCMSMap( )->get_feature_list_begin();

  vector<LC_MS_FEATURE>::iterator B = get_ALIGNED_LCMSMap()->get_feature_list_begin();  
  feature* current_B = NULL;
  
  // go now through all the features of spectrum B:
  while(B != get_ALIGNED_LCMSMap( )->get_feature_list_end()){
    
    // get the feature to compare:
    current_B = &(*B);
    
    //////////////////////////
    // debugging:
    if( LC_MS_merger::DEBUG_LCMS_MERGER ){
      if(( current_B->get_MZ() <= LC_MS_merger::FEATURE_MZ_MAX) && ( current_B->get_MZ() >= LC_MS_merger::FEATURE_MZ_MIN) ){
        //current_B->show_info();
      }
    }
    
    // find the feature in the other LC_MS run A
    match_feature_in_LCMS_A( current_B );
    
    B++;
  }
 }


/////////////////////////////////////////////////////////////////
// filter here the match list accoring to high abundance peptides
// using the object "plot_smoother", pass only the match_list
// create he retention time vs delta_retention time plot:
void AmrtpLCMSAligner::remove_multiple_LCMS_alignment_feature_match(){
  
  MultipleFeatureMatchRemover* corrector = new MultipleFeatureMatchRemover( &raw_match_list );
  // pass the raw data:
  // corrector->set_raw_data( raw_match_list );
  // start now smoothing:
  corrector->start_smoothing();
  
  // get the smoothe match list back:
  // cout<<raw_match_list.size()<<endl;
  // raw_match_list = corrector->get_smoothed_data();
  
  delete corrector;
  corrector = NULL;
  
 }




///////////////////////////////////////////////////////////////////////////
// find the corresponding MS1 feature in LC/MS run A:
void AmrtpLCMSAligner::match_feature_in_LCMS_A( feature* toAlign ){
  
  // define the constraints:
  int CHRG = toAlign->get_charge_state();
  double TR = toAlign->get_retention_time();
  double MZ = toAlign->get_MZ();
  double MZ_MAX = MZ + simple_math::getMassErrorAtPPMLevel(MZ, 2.0 * Mz_window);
  double MZ_MIN = MZ - simple_math::getMassErrorAtPPMLevel(MZ, 2.0 * Mz_window);
  bool feature_match = false;
  
  /*
  if( IN->get_MS2_info(0.0) ){
    if( IN->get_SQ(0.0) == "SIGTENTEENR"){
      IN->show_info();
    }
  }
  */
  /*
  if( ( IN->get_MZ() >= 490.95) && ( IN->get_MZ() <= 490.97) ){
    IN->show_info();
  }
  */
  
  
  /////////////////////////////
  // search lower m/z features:
  vector<LC_MS_FEATURE>::iterator FEA_A = MERGE_ITERATOR_REFERENCE_LCMS;
  while(  FEA_A != get_REFERENCE_LCMSMap( )->get_feature_list_end() ){
    
    feature* fReference = &(*FEA_A); 
    // check mass range, if it cant match anymore:
    if( MZ_MIN > fReference->get_MZ() ){
      break;
    }     
    
    
    //////////////////////////////////////////////
    // check the charge state:
    // for feature which originate from MS2 traces
    // the charge state can not be determined (is -1)
    // in these case ignore the charge state for matching:
    bool ChargeCheck = false;
    
    // in case on feature has z == -1:
    if( (fReference->get_charge_state() < 0) || (CHRG < 0) ){
      ChargeCheck = true;
    }
    else{
      // otherwise compare the values:
      if( fReference->get_charge_state()  == CHRG ){
        ChargeCheck = true;
      }
    }    
    if( ChargeCheck ){
      
      // now check if the are the same +/- tolerance:
      if(simple_math::compareMassValuesAtPPMLevel( MZ, fReference->get_MZ(), Mz_window )){
        
        // check if within the retention time window:
        // get the tolerance parameters:
        double MY_TR_WINDOW_UP = 0; 
        double MY_TR_WINDOW_DOWN = 0; 
        // check now the retention time difference:
        double delta_TR = fReference->get_retention_time() - TR;
        get_TR_WINDOW( TR , &MY_TR_WINDOW_UP , &MY_TR_WINDOW_DOWN);
        if( ( delta_TR <= MY_TR_WINDOW_UP ) && ( delta_TR >= MY_TR_WINDOW_DOWN ) ){ 
          // finds the fitted delta TR at a given delta TR
          save_MS1_feature_pair( toAlign, fReference);
          feature_match = true;
        }
      }
    }
    
    if(  FEA_A == get_REFERENCE_LCMSMap( )->get_feature_list_begin() ){
      break;
    }
    FEA_A--;
  } 
  
  /////////////////////////////
  // search higher m/z features:
  FEA_A = MERGE_ITERATOR_REFERENCE_LCMS;
  FEA_A++;
  while(  FEA_A != get_REFERENCE_LCMSMap( )->get_feature_list_end() ){
    
    feature* fReference = &(*FEA_A); 
    // check mass range, if it cant match anymore:
    if( MZ_MAX < fReference->get_MZ() ){
      break;
    }     
    // check only the same charge states:
    if( fReference->get_charge_state() == CHRG ){
      // now check if the are the same +/- tolerance:
      if(simple_math::compareMassValuesAtPPMLevel( MZ, fReference->get_MZ(), Mz_window )){
        
        // check if within the retention time window:
        // get the tolerance parameters:
        double MY_TR_WINDOW_UP = 0; 
        double MY_TR_WINDOW_DOWN = 0; 
        // check now the retention time difference:
        double delta_TR = fReference->get_retention_time() - TR;
        get_TR_WINDOW( TR , &MY_TR_WINDOW_UP , &MY_TR_WINDOW_DOWN);
        if( ( delta_TR <= MY_TR_WINDOW_UP ) && ( delta_TR >= MY_TR_WINDOW_DOWN ) ){ 
          
          // finds the fitted delta TR at a given delta TR
          save_MS1_feature_pair( toAlign, fReference);
          feature_match = true;
        }
      }
    }
    
    FEA_A++;
  } 
  
  
  // set the start for the next feature match search to the end of this search, 
  // so next search does not need to start all over from the beginning of the
  // list but directly from here:
  if( FEA_A != get_REFERENCE_LCMSMap( )->get_feature_list_end() ){
    MERGE_ITERATOR_REFERENCE_LCMS = FEA_A;
  }
  
  
  if( !feature_match ){
    // save the not matched peaks from spec. B
    // set pseudo delta as 0
    save_non_LCMS_alignment_feature_match(  TR );
  }  
  
  
}


////////////////////////////////////////////////////////////
// finds the fitted delta TR at a given delta TR
void AmrtpLCMSAligner::save_MS1_feature_pair( feature* iToAlign, feature* iReference){
  
  
  // use here if this option is on also
  // MS2 information for the alignment:
  bool SQ_CHECK = true;
  if( ID_ALIGNMENT_WEIGHT ){

    // check, if MS2 info is available for the 2 features:
    if( (iReference->get_MS2_info()) && (iToAlign->get_MS2_info()) ){ 
      if( iReference->get_SQ() == iToAlign->get_SQ() ){
        SQ_CHECK = true;
      }
      else{
        SQ_CHECK = false;
      }
    }
  }

  
  
  if( ( iToAlign->getMS2Feature() != NULL) && ( iReference->getMS2Feature() != NULL) ){
    match_region checker;
    double score = checker.getMS2SpectraCorrelation( iToAlign, iReference );
    if( score < 5 ){
      SQ_CHECK = false;
    }
  }
  
  
  /*
  
  ////////////////////////////
  // HPIN alignment landmarks:
  // do if two same HPINs found:
  
  if( ( SEARCH->getFeatureExtraInformation( ).find("HPin") != string::npos ) 
     && ( TARGET->getFeatureExtraInformation( ).find("HPin") != string::npos ) &&
     (SEARCH->getSignalToNoise( ) > 2.0) && (TARGET->getSignalToNoise( ) > 2.0)
    ){
    TARGET->show_info();
    SEARCH->show_info();
    cout<<endl;
    // add this match as landmark "ID_ALIGNMENT_WEIGHT" times:
    double min = -5.0;
    double step = 0.01;
    double max = 5.0;
    while(min<=max){
      save_MS2_match(SEARCH->get_retention_time() + min, TARGET->get_retention_time() + min, SEARCH->get_peak_area(), TARGET->get_peak_area(), TARGET->get_charge_state(), TARGET->get_feature_ID(), SEARCH->get_feature_ID());
      min += step;
    }
    SQ_CHECK = false;
  }
  
  // do if NOT the same HPINs found:
  else if( ( SEARCH->getFeatureExtraInformation( ).find("HPin") != string::npos ) 
     || ( TARGET->getFeatureExtraInformation( ).find("HPin") != string::npos ) ){
    SQ_CHECK = false;
  }
   
*/
  
  
  if( SQ_CHECK ){    
    // peak has been found in current mz/tr window:
    // save now the retention time of A and B
    //save_match(SEARCH->get_retention_time(), TARGET->get_retention_time(), SEARCH->get_peak_area(), TARGET->get_peak_area(), TARGET->get_charge_state(), TARGET->get_feature_ID(), SEARCH->get_feature_ID());
    // LCMS_alignment_feature_match* tmp = new LCMS_alignment_feature_match( iReference->get_retention_time(), iToAlign->get_retention_time(), iReference->get_peak_area(), iToAlign->get_peak_area() );
    LCMS_alignment_feature_match* tmp = new LCMS_alignment_feature_match( iReference, iToAlign );
    tmp->set_match_indexes( iToAlign->get_feature_ID(), iReference->get_feature_ID() );
    
    // store a match in vector:
    raw_match_list.push_back(*tmp);
    tmp = NULL;
  }
  
}


/*
////////////////////////////////////////////////////////////
// saves a matched peak pair: A = reference, B to be aligned
void AmrtpLCMSAligner::save_MS2_match(double TA, double TB, double peak_area_A, double peak_area_B, int charge_state, int index_B, int index_A){
  
  matches* tmp = new matches(TA, TB, peak_area_A, peak_area_B);
  
  // set match_indexes
  tmp->set_match_indexes(index_A,index_B);
  
  // store a match in vector:
  MS2_match_list.push_back(*tmp);
  tmp = NULL;
  
}
 */


////////////////////////////////////////////////////////////
// combines MS1 and MS2 information:
void AmrtpLCMSAligner::combine_MS1_and_MS2_LCMS_alignment_feature_match(){
  if( !MS2_match_list.empty() ){
    raw_match_list.insert(raw_match_list.end(),MS2_match_list.begin(),MS2_match_list.end());
  }
}

/*
////////////////////////////////////////////////////////////
// saves a matched peak pair: A = reference, B to be aligned
void AmrtpLCMSAligner::save_match_by_shift(double TA, double TB, double shift, double SD_up, double SD_down){
	
  matches* tmp = new matches(TA,TB);
	
  // set the error:
  tmp->set_alignment_error_up(SD_up);
  tmp->set_alignment_error_down(SD_down);
  
  // set the shift:
  tmp->set_Delta(shift);
  
  // store now in vector:
  match_list.push_back(*tmp);
  
  tmp = NULL;
}
*/
 
 
////////////////////////////////////////////////////////////
// saves not matched peak pair: A = reference, B to be aligned
void AmrtpLCMSAligner::save_non_LCMS_alignment_feature_match(double TB){
  LCMS_alignment_feature_match* tmp = new LCMS_alignment_feature_match(TB);
  // store now in vector:
  non_match_list.push_back(*tmp);
  tmp = NULL;
}

    
/*
////////////////////////////////////////////////////////////
// removes doubles in a vector of matches
void AmrtpLCMSAligner::remove_non_match_doubles(){

  vector<LCMS_alignment_feature_match>::iterator A = non_match_list.begin();
  vector<LCMS_alignment_feature_match>::iterator B;
  while(A != non_match_list.end()){
    B = A;
    B++;
    while( B != non_match_list.end() ){
      // if they are the same (same delta and TR)
      // remove it from the list:
      if(*A == *B){
        B = non_match_list.erase(B);
      }
      else{
        B++;
      }
      
      // if its bigger jump out:
      if(*A >= *B){
        break;
      }
    }
    A++;
  }
}
 


void AmrtpLCMSAligner::remove_doubles(){

  vector<LCMS_alignment_feature_match>::iterator A = match_list.begin();
  vector<LCMS_alignment_feature_match>::iterator B;
  
  while(A != match_list.end()){
    
    B = A;
    B++;
    
    while( B != match_list.end() ){
      
      // if they are the same (same delta and TR)
      // remove it from the list:
      if(*A == *B){
        B = match_list.erase(B);
      }
      else{
        B++;
      }
      
      // if its bigger jump out:
      if(*A >= *B){
        break;
      }
    }
    A++;
  }
}

 */


/*
////////////////////////////////////////////////////////////
// adjust the non matched delta TR:
void AmrtpLCMSAligner::adjust_non_matched_delta(){

  if( !match_list.empty() && !non_match_list.empty() ){
    
    vector<matches>::iterator M = match_list.begin(); 
    double tmp_TR = 0;
    double search_TR = 0;
    double TR_B_start = (*M).get_TB();
    double shift_start = (*M).get_Delta();
    double SD_start_up = (*M).get_alignment_error_up();
    double SD_start_down = (*M).get_alignment_error_down();
    M = match_list.end();
    M--;
    double TR_B_end = (*M).get_TB();
    double shift_end = (*M).get_Delta();
    double SD_end_up = (*M).get_alignment_error_up();
    double SD_end_down = (*M).get_alignment_error_down();
    double tmp_TR_B = 0;
    double tmp_SD_up = 0;
    double tmp_SD_down = 0;
    double tmp_shift=0;
    
    vector<matches>::iterator NM = non_match_list.begin(); 
    while( NM != non_match_list.end() ){

      // get Tr
      search_TR = (*NM).get_TB();
      
      // cycle thought the list and find the
      // correpsonding delta mass for this TR
      
      //////////////////////////////////////////////////////////
      // check if the non matched Tr is in the range of matched Tr:
      if( TR_B_end < search_TR){
        save_match_by_shift(TR_B_end + shift_end, search_TR, shift_end, SD_end_up, SD_end_down);
      }
      else if(TR_B_start > search_TR){
        save_match_by_shift( TR_B_start + shift_start, search_TR, shift_start, SD_start_up, SD_start_down);
      }
      
      else{

        vector<matches>::iterator p = match_list.begin(); 
        while( p != match_list.end()){
          
          tmp_TR = (*p).get_TB();
          
          // if the same retention time found,
          // then kick this match out, since we have already
          // a retention time shift for this TR
          if(search_TR == tmp_TR){
            break;
          }
          
          // this case the TR's is not found,
          // so average between 2 TR's:
          // ( kind of linear regression)
          if(tmp_TR > search_TR){
            // integrate the weights from the
            // lowess fit:
            tmp_TR_B = (*p).get_TB();
            tmp_SD_up = (*p).get_alignment_error_up();
            tmp_SD_down = (*p).get_alignment_error_down();
            tmp_shift = (*p).get_Delta();

            if(p != match_list.begin() ){ 
              p--;
              tmp_TR_B += (*p).get_TB();
              tmp_TR_B /= 2.0;
              tmp_SD_up += (*p).get_alignment_error_up();
              tmp_SD_up /= 2.0;
              tmp_SD_down += (*p).get_alignment_error_down();
              tmp_SD_down /= 2.0;
              tmp_shift += (*p).get_Delta();
              tmp_shift /= 2.0;
            }
            // add this peak to the match list:
            save_match_by_shift(tmp_TR_B + tmp_shift, search_TR, tmp_shift, tmp_SD_up, tmp_SD_down);
            break;
          }
          
          p++;
        }
      }
      NM++;
    }
    sort_matches();
  }
}

*/

/*
////////////////////////////////////////////////////////////
// sorts the saved matches according to retention time B
void AmrtpLCMSAligner::sort_LCMS_alignment_feature_match(){
  sort(raw_match_list.begin(),raw_match_list.end(),OPERATOR());
}
void AmrtpLCMSAligner::sort_non_LCMS_alignment_feature_match(){
  sort(non_match_list.begin(),non_match_list.end(),OPERATOR());
}
*/


///////////////////////////////
// normalize the retention time scale in the run
void AmrtpLCMSAligner::normalizeRetentionTimeScales(){
  
  
  
  double ERROR_UP = 0;
  double ERROR_DOWN = 0;
  
  // adjust all features in spectrum B
  feature* current_B = NULL;
  double this_TR = 0;  
  vector<LC_MS_FEATURE>::iterator F = get_ALIGNED_LCMSMap( )->get_feature_list_begin();
  while( F != get_ALIGNED_LCMSMap( )->get_feature_list_end() ){
    
    // get the feature to correct:
    current_B = &(*F);
    
    // get the current TR:
    this_TR = current_B->get_retention_time();
      
    // correct TR accoring to the regression analysis:
    // also get bakc the SD at this TR
    correct_TR(&this_TR, &ERROR_UP, &ERROR_DOWN);
    
    // save now the corrected TR:
    current_B->set_retention_time(this_TR);
          
    // save the calculated SD:
    current_B->set_alignment_error_up(ERROR_UP);
    current_B->set_alignment_error_down(ERROR_DOWN);
    F++;
  }
}



////////////////////////////////////////////////////////////
// corrects a TR using the analysis results stored in
// match_list
void AmrtpLCMSAligner::correct_TR(double* IN, double* ERROR_UP, double* ERROR_DOWN){
  
  if( !match_list.empty() ){
  
    // try to find the best match by Tr:
    vector<LCMS_alignment_feature_match>::iterator p = lower_bound( match_list.begin(), match_list.end(), *IN, OPERATOR_by_RT_TO_BE_ALIGNED() );
    
      
    // case where TR is bigger than any element in the list:
    if( p == match_list.end()){
      vector<LCMS_alignment_feature_match>::reverse_iterator r_p = match_list.rbegin();
      *IN = *IN + (*r_p).get_Delta();
      *ERROR_UP = (*r_p).get_alignment_error_up();
      *ERROR_DOWN = (*r_p).get_alignment_error_down();
    }
    
    
    // check for exact match of the TR's
    else if( (*p).get_TR_ToAlign() == *IN ){
      *IN = *IN +(*p).get_Delta();
      *ERROR_UP = (*p).get_alignment_error_up();
      *ERROR_DOWN = (*p).get_alignment_error_down();
    }
    
    
    
    // case where TR is smaller than any element in the list:
    else if( p == match_list.begin() ){
      *IN = *IN + (*p).get_Delta();
      *ERROR_UP = (*p).get_alignment_error_up();
      *ERROR_DOWN = (*p).get_alignment_error_down();
    }
      
    // case that there was just no extact match:
    else{
      
      double TR_HIGH = (*p).get_TR_ToAlign();
      double delta_HIGH = (*p).get_Delta();
      double ERROR_UP_HIGH = (*p).get_alignment_error_up();
      double ERROR_DOWN_HIGH = (*p).get_alignment_error_down();
      p--;
      double TR_LOW = (*p).get_TR_ToAlign();
      double delta_LOW = (*p).get_Delta();
      double ERROR_UP_LOW = (*p).get_alignment_error_up();
      double ERROR_DOWN_LOW = (*p).get_alignment_error_down();
      
      // define the weights:
      double factorL = *IN - TR_LOW;
      double factorH = TR_HIGH - *IN;
      double factorToT = factorL + factorH;
      
      // weighted delta and errors:
      double wDelta = (factorL*delta_HIGH  + delta_LOW * factorH ) / factorToT;
      double wUpError = ( factorL * ERROR_UP_HIGH + factorH * ERROR_UP_LOW) / factorToT;
      double wDownError = ( factorL * ERROR_DOWN_HIGH + factorH * ERROR_DOWN_LOW )  / factorToT;
      
      *IN = *IN + wDelta;
      *ERROR_UP = wUpError;
      *ERROR_DOWN = wDownError;
  
    }
    
  }
}


////////////////////////////////////////////////
// save the alignment error in the LC/MS run B:
void AmrtpLCMSAligner::save_alignment_error_in_B(){
  
  vector<LCMS_alignment_feature_match>::iterator p = match_list.begin();
  LC_MS* THIS = get_ALIGNED_LCMSMap( );
  
  while(p != match_list.end()){
    
    double TR = (*p).get_TR_ToAlign();
    double ERROR_UP = (*p).get_alignment_error_up();
    double ERROR_DOWN = (*p).get_alignment_error_down();
    
    // save the error:
    THIS->add_alignment_error(TR, ERROR_UP, ERROR_DOWN);
    
    // increase iterator
    p++;
     
  }
  
}


///////////////////////////////////////////////////////////////
// clear up all previous analysis resulst / LC-MS runs:
void AmrtpLCMSAligner::clear_up(){
  
  
  if(!MS2_match_list.empty()){
    MS2_match_list.clear();
  }

  if(!match_list.empty()){
    match_list.clear();
  }
  
  if(!non_match_list.empty()){
    non_match_list.clear();
  }
  
  if(!raw_match_list.empty()){
    raw_match_list.clear();
  }
    
  if( PLOT != NULL ){
    delete PLOT;
    PLOT = NULL;
  }
  
}

////////////////////////////////////////////////////////////
// check the correlation of 2 LC_MS runs:
void AmrtpLCMSAligner::check_LC_MS_correlation(SM_cell* IN){

  // construct the instance:
  LC_MS_correlation* comparer = new LC_MS_correlation( IN );
  
  // set the access pathe to the match list:
  if(!match_list.empty()){
    comparer->set_alignment_error_access(this, &AmrtpLCMSAligner::get_alignment_error_to_extern);
  }
  
  comparer->correlate_two_spectra( get_REFERENCE_LCMSMap( ), get_ALIGNED_LCMSMap( ));
  
  delete comparer;
  comparer = NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// here come all the print out functions for the statistics:
// print the raw matches = delta TR vs TR
void AmrtpLCMSAligner::PRINT_RAW_MATCHES(){
  
  if( !raw_match_list.empty() ){
  
    if( PLOT != NULL ){
      delete PLOT;
      PLOT = NULL;
    }
    
    char buffer[255];
    sprintf( buffer, "ALIGNPLOT_%d_%d_%d", get_REFERENCE_LCMS_ID(), get_ALIGNED_LCMS_ID(), ITERATION_COUNT);
    string tmp = buffer;
    PLOT = new data_plotter(tmp, false);
    PLOT->set_POINT_SIZE( 10 );
    if( PLOT != NULL ){
      map<double, double> DATA;
      vector< LCMS_alignment_feature_match>::iterator P = raw_match_list.begin();
      while( P != raw_match_list.end()){
        double TA = (*P).get_TR_ToAlign(); 
        double TB = (*P).get_Delta();
        DATA.insert( make_pair( TA, TB ) );
      P++;
      }
      sprintf( buffer, "raw(%d)", raw_match_list.size());
      PLOT->add_plot_data_dots( &DATA, buffer);  
    }
  }
}



///////////////////////////////////////////////////////////
// print the fitted matches = delta TR vs TR and the error
void AmrtpLCMSAligner::PRINT_ALIGNMENT(){

  if( ( !match_list.empty() ) && ( PLOT != NULL ) ){
    
    char buffer[64];
    map<double, double> DATA1;
    map<double, double> DATA2;
    map<double, double> DATA3;
    
    vector< LCMS_alignment_feature_match>::iterator P = match_list.begin();
    while( P != match_list.end()){
      double TA = (*P).get_TR_ToAlign();    
      double TB = (*P).get_Delta();
      DATA1.insert( make_pair( TA, TB ) );
      P++;
    }
    
    if( PLOT != NULL ){
      sprintf( buffer, "deltaTR");
      PLOT->add_plot_data( &DATA1, buffer );  
    }
    
    P = match_list.begin();
    while( P != match_list.end()){
      double TB = (*P).get_Delta() + (*P).get_alignment_error_up();
      double TA = (*P).get_TR_ToAlign(); 
      DATA2.insert( make_pair( TA, TB ) );
      P++;
    }
    
    if( PLOT != NULL ){
      sprintf( buffer, "upper error");
      PLOT->add_plot_data( &DATA2, buffer );  
    }
    
    P = match_list.begin();
    while( P != match_list.end()){
      double TA = (*P).get_TR_ToAlign(); 
      double TB = (*P).get_Delta() - (*P).get_alignment_error_down();
      DATA3.insert( make_pair( TA, TB ) );
      P++;
    }
    
    if( PLOT != NULL ){
      sprintf( buffer, "lower error");
      PLOT->add_plot_data( &DATA3, buffer );  
      
      // and now do the plotting:
      PLOT->plot_TWOD_data();
      
      delete PLOT;
      PLOT = NULL;
    }
    
    DATA1.clear();
    DATA2.clear();
    DATA3.clear();
  }
}



///////////////////////////////////////////////////////////////////
// function which returns the retention time correction error
// for a given input Tr B
void AmrtpLCMSAligner::get_alignment_error(double IN, double* UP, double* DOWN){
  
  if( !match_list.empty() ){
  
    vector<LCMS_alignment_feature_match>::iterator p = match_list.begin();
    bool match = false;
    
    // check for exact match:
    while( p != match_list.end() ){
      
      if( IN == (*p).get_TR_ToAlign() ){
        *UP = (*p).get_alignment_error_up();
        *DOWN = (*p).get_alignment_error_down();
        match = true;
        break;
      }
      
      if( IN >= (*p).get_TR_ToAlign() ){
        break;
      }
      
      p++;
    }
    
    if(!match){
      
      // TR B is smaller than any element in the list
      if(p == match_list.begin() ){
        *UP = (*p).get_alignment_error_up();
        *DOWN = (*p).get_alignment_error_down();
      }
      
      // TR B is bigger than any element in the list
      else if( p == match_list.end() ){
        p--;
        *UP = (*p).get_alignment_error_up();
        *DOWN = (*p).get_alignment_error_down();
      }
      else{
        // is within the range of the list but has not found an exact match
        // -> extrapolate:
        double tmp_u = (*p).get_alignment_error_up();
        double tmp_d = (*p).get_alignment_error_down();
        p--;
        tmp_u += (*p).get_alignment_error_up();
        tmp_d += (*p).get_alignment_error_down();
        tmp_u /= 2.0;
        tmp_d /= 2.0;
        
        *UP = tmp_u;
        *DOWN = tmp_d;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////
// function which returns the search with at a given TR
void AmrtpLCMSAligner::get_TR_WINDOW(double IN, double* UP, double* DOWN){
  
  //if( previous_match_list.empty() ){
    *UP = Tr_window;
    *DOWN = -1.0 * Tr_window;
  
  /*
  }
  else{
    
    vector<matches>::iterator p = previous_match_list.begin();
    bool match = false;
    while( p != previous_match_list.end() ){
      
      if( IN == (*p).get_TB() ){
        *UP = (*p).get_Delta() + (*p).get_alignment_error_up();
        *DOWN = (*p).get_Delta() - (*p).get_alignment_error_down();
        match = true;
        break;
      }
      
      if( IN >= (*p).get_TB() ) {
        break;
      }
      
      p++;
    }
    
    if(!match){
      // TR B is smaller than any element in the list
      if(p == previous_match_list.begin() ){
        *UP = (*p).get_Delta() + (*p).get_alignment_error_up();
        *DOWN = (*p).get_Delta() - (*p).get_alignment_error_down();
      }
      
      // TR B is bigger than any element in the list
      else if( p == previous_match_list.end() ){
        p--;
        *UP = (*p).get_Delta() + (*p).get_alignment_error_up();
        *DOWN = (*p).get_Delta() - (*p).get_alignment_error_down();
      }
      else{
        // is within the range of the list but has not found an exact match
        // -> extrapolate:
        double tmp_u = (*p).get_alignment_error_up();
        double tmp_d = (*p).get_alignment_error_down();
        double tmp_delta = (*p).get_Delta();
        p--;
        tmp_u += (*p).get_alignment_error_up();
        tmp_d += (*p).get_alignment_error_down();
        tmp_delta += (*p).get_Delta();
        tmp_u /= 2.0;
        tmp_d /= 2.0;
        tmp_delta /=2.0;
        
        *UP = tmp_delta + tmp_u;
        *DOWN = tmp_delta + tmp_d;
      }
    }
  }
   */
}

///////////////////////////////////////////////////////////////////
// function which returns the retention time correction error
// for a given input Tr B and which is called from another object class:
void AmrtpLCMSAligner::get_alignment_error_to_extern(void *pInstance, double IN, double* UP, double* DOWN){

  // instanciate the AmrtpLCMSAligner-class:
  AmrtpLCMSAligner *pThis = (AmrtpLCMSAligner *)pInstance; 
  pThis->get_alignment_error( IN, UP, DOWN );
  
}


///////////////////////////////////////////////////////////////////
// check alignment stopp, is ok if less than x% of the raw points
// lay outside the alignemmt error bars:
bool AmrtpLCMSAligner::CHECK_ALIGNMENT_CONVERGENCE(){
  
  // stopp if too many iterations reached
  if( ITERATION_COUNT > MAX_ITERATION ){
    return true;
  }
  
  // stopp if percentile is equal to one
  // i.e. all points can layout outside
  // of teh alignment error border
  // -> no alignment will be performed
  if( 1.0 == ERROR_DELTA_POINTS ){
    return true;
  }
  
  // if the match list is empty, then 
  // no alignment has been performed yet,
  // so at least one iteration more:
  if( match_list.empty() ){
    return false;
  }

  double outside_count = 0;
  double org_size = (double) raw_match_list.size();
  vector<LCMS_alignment_feature_match>::iterator P = raw_match_list.begin(); 
  while( P != raw_match_list.end() ) {
    
    double OBS_TR = (*P).get_TR_ToAlign();
    double MOD_TR = OBS_TR;
    double OBS_delta = (*P).get_Delta();
    double UP_E = 0;
    double DOWN_E = 0;
    correct_TR(&MOD_TR, &UP_E, &DOWN_E);      
    double MOD_delta = MOD_TR - OBS_TR;
    UP_E += MOD_delta;
    DOWN_E = MOD_delta - DOWN_E;
    
    //double OBS_delta = (*P).get_Delta_ORIGINAL();
    //double MOD_delta = (*P).get_Delta();
    //double UP_E = MOD_delta + (*P).get_alignment_error_up();
    //double DOWN_E = MOD_delta - (*P).get_alignment_error_down();

    // check if the delta is outside the error borders:
    if( ( OBS_delta > UP_E ) || ( OBS_delta < DOWN_E ) ){
      outside_count++;
      P = raw_match_list.erase( P );
    }
    else{
      P++;
    }
  }
  
  outside_count /= org_size;
  if( outside_count <= ERROR_DELTA_POINTS ){
    return true;
  }
  else{
    return false;
  }
}

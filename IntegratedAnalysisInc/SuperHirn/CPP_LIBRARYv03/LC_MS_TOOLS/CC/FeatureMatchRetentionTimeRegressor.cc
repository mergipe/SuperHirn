///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS REGRESSOR:
// regression of Tr vs delta Tr
//
// methods implemented:
//
// - by median filter
// - smoothing by lowess procedure
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#define USE_FEATURE_MATCH_RETENTION_TIME_REGRESSOR
#include "LC_MS_TOOLS_USE.h"


// minimal alignment error = TR tolerance / 2:
double FeatureMatchRetentionTimeRegressor::min_error;
// maximal alignment error:
double FeatureMatchRetentionTimeRegressor::max_error;

// used to copmute the alignment error, use a tr window to
// calculate the standard deviations of raw data to predicted
// delta shift
double FeatureMatchRetentionTimeRegressor::tr_error_smooth;


////////////////////////////////////////////////
// constructor for the
FeatureMatchRetentionTimeRegressor::FeatureMatchRetentionTimeRegressor(vector<LCMS_alignment_feature_match>* IN){

  // save the match list:
  data = IN;  
}


//////////////////////////////////////////////////
// class desctructor
FeatureMatchRetentionTimeRegressor::~FeatureMatchRetentionTimeRegressor(){
  retention_time_list.clear();
}

//////////////////////////////////////////////////
// create the fit data; only consists of
// single TR's, no delta's,
void FeatureMatchRetentionTimeRegressor::create_retention_time_list(){

  retention_time_list.clear();
  
  // find all observed retention times:
  vector<LCMS_alignment_feature_match>::iterator p = data->begin();
  double tr = -1.0;
  while(p != data->end()){
    // check if there is a new retention time found
    if(tr != (*p).get_TR_ToAlign()){
      tr = (*p).get_TR_ToAlign();
      retention_time_list.push_back( tr );
    }
    p++;
  }
  
  // now create the a vector for every observed retention time = tr_bins:
  //tr_shift_bins = new vector<double>[get_nb_retention_times()];
}

//////////////////////////////////////////////////
// find the index of a given TR in the list
int FeatureMatchRetentionTimeRegressor::get_retention_time_index(double TR){

  int i = 0;
  // find the retention times:
  while( i < get_nb_retention_times() ){ 
    
    if( TR > get_retention_time(i) ){
      break;
    }
    
    // check if the retention time is found
    if(TR == get_retention_time(i)){
      return i;
    }
    
    i++;
  }
  
  // search TR is smaller than any element in list
  if( i == 0){
    return 0;
  }
  else{
    // TR is in the range of the list but has not been found!!! 
    return ( i - 1 );
  }
}


//////////////////////////////////////////////////
// build up and down alignemnt errors:
void FeatureMatchRetentionTimeRegressor::create_LOWESS_ERRORS(){
  
  // create the mean and SD for every TR:
  double search_width = tr_error_smooth;
  vector<LCMS_alignment_feature_match>::iterator D = data->begin();
  while( D != data->end() ){
    
    ////////////////////////////////////////////////////
    // create the alignment error:
    // median deviation of the data from the computed delta shifts
    vector<double> up_error;
    vector<double> down_error;
    double this_TR = (*D).get_TR_ToAlign();
    double this_DELTA = (*D).get_Delta();
    
    // compute first the error for lower TR values:
    vector<LCMS_alignment_feature_match>::iterator P = D;
    if( P != data->begin() ){
      while( P != data->end() ){
        
        if( (*P).get_TR_ToAlign() < (this_TR - search_width) ){
          break;
        }
        double tmp = (*P).get_Delta_ORIGINAL() - this_DELTA;
        if(tmp >= 0){ 
          up_error.push_back( tmp );
        }
        else{
          down_error.push_back( fabs(tmp) );
        }
        if( P == data->begin() ){
          break;
        }
        P--;
      }
    }
    
    // compute first the error for higher TR values:
    P = D;
    P++;
    while( P != data->end() ){
      
      if( (*P).get_TR_ToAlign() > (this_TR + search_width) ){
        break;
      }
      double tmp = (*P).get_Delta_ORIGINAL() - this_DELTA;
      if(tmp >= 0){
        up_error.push_back( tmp );
      }
      else{
        down_error.push_back( fabs(tmp) );
      }
      
      P++;
    }
    
    // compute now the median from the deviation distribution:
    simple_math MY_MATH;
    double mean_down = 0;
    double mean_up = 0;
    
    // use average:
    /*
    pair<double, double> AV_TMP = MY_MATH.AVERAGE_and_STDEV( &down_error );
    mean_down = AV_TMP.first;
    AV_TMP = MY_MATH.AVERAGE_and_STDEV( &up_error );
    mean_up = AV_TMP.first;
    */
    
    // use average + standard error:
    pair<double, double> AV_TMP = MY_MATH.AVERAGE_and_STDERROR( &down_error );
    mean_down = AV_TMP.first + AV_TMP.second;
    AV_TMP = MY_MATH.AVERAGE_and_STDERROR( &up_error );
    mean_up = AV_TMP.first + AV_TMP.second;
                                                    
                          
    // mean_down = MY_MATH.MEDIAN( &down_error );
    // mean_up = MY_MATH.MEDIAN( &up_error );
    
    if( up_error.empty() && !down_error.empty()){
      mean_up = mean_down;    
    }
    if( !up_error.empty() && down_error.empty()){
      mean_down = mean_up;    
    }
    
    up_error.clear();
    down_error.clear();
    
    // check if errors are not smaller than minimal error:
    if( mean_up < min_error){
      mean_up =  min_error;
    }
    if( mean_down < min_error){
      mean_down =  min_error;
    }
    // check if errors are not bigger than maximal error:
    if( mean_up > max_error){
      mean_up =  max_error;
    }
    if( mean_down > max_error){
      mean_down =  max_error;
    }
    
    //set the errors:
    (*D).set_alignment_error_up( mean_up );
    (*D).set_alignment_error_down( mean_down );

    D++;
  }  
}




//////////////////////////////////////////////////
// fit the data with lowess fit, see lowess_manager
void FeatureMatchRetentionTimeRegressor::start_lowess_fit(){
  
  double* X;
  double* Delta;

  int nb_data = data->size();
  X = new double[ nb_data ];
  Delta = new double[ nb_data ];
  
  //////////////////////////////////////
  // create an array for the X (=TR) and 
  // its Y's(=deltaTR):
  vector<LCMS_alignment_feature_match>::iterator p = data->begin();
  int i = 0;
  while(p != data->end()){

    X[i] = (*p).get_TR_ToAlign();
    Delta[i] = (*p).get_Delta();
    
    // store the original delta
    (*p).set_Delta_ORIGINAL( (*p).get_Delta() );
    
    i++;
    p++;
  }
  
  //////////////////////////////////////
  // give them to the lowess fitter to create a fit:
  LowessRegressionManager* lower = new LowessRegressionManager( X, Delta, nb_data);
  // start the fitting:
  lower->start_fit();
  
  
  /////////////////////////////////////
  // now get the smoothed values back:
  // copy 
  i = 0;
  p = data->begin();
  while( p != data->end() ){
    // get X = TR:
    (*p). set_TR_ToAlign(lower->get_X(i));
    // store the smoothed Delta
    (*p).set_Delta(lower->get_smoothed_Y(i));
    //store the weight from teh lowess
    (*p).set_weight(lower->get_weight(i));
    // data[i].set_TB( lower->get_X(i) + lower->get_smoothed_Y(i) );
    // data[i].set_TB_ORIGINAL( lower->get_X(i) );
    i++;
    p++;
  }
		
  ///////////////////////////////////////
  // double residuals = lower->get_sum_residuals();
  // build the errors:
  create_LOWESS_ERRORS();
  
  delete lower;
  lower = NULL;
  
  
  delete X;
  delete Delta;
  
}




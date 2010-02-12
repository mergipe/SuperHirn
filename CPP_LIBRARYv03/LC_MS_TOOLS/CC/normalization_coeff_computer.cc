///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS normalization_coeff_computer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_NORMALIZATION_COEFF_COMPUTER
#include "LC_MS_TOOLS_USE.h"


vector<int> normalization_coeff_computer::LC_MS_SELECTION_A;
vector<int> normalization_coeff_computer::LC_MS_SELECTION_B;
double normalization_coeff_computer::FEATURE_ALIGN_PERCENTAGE;
double normalization_coeff_computer::TR_SLIDE_WINDOW;
double normalization_coeff_computer::TR_STEP;


////////////////////////////////////////////////
// constructor for the object normalization_coeff_computer:
normalization_coeff_computer::normalization_coeff_computer(LC_MS* IN, int nb_matches){
  
  MASTER = IN;
  NB_MATCHES = nb_matches;
}

////////////////////////////////////////////////
// constructor for the object normalization_coeff_computer:
normalization_coeff_computer::normalization_coeff_computer(LC_MS* IN){
  
  MASTER = IN;
  NB_MATCHES = MASTER->get_nb_raw_specs();
  
}

//////////////////////////////////////////////////
// class desctructor of normalization_coeff_computer
normalization_coeff_computer::~normalization_coeff_computer(){
  MASTER = NULL;
  GLOBAL_LC_MS_COEFF.clear();
  ALL_LOCAL_LC_MS_COEFF.clear();
  ABSOLUTE_LC_MS_COEFF.clear();
  GLOBAL_ABSOLUTE_LC_MS_COEFF.clear();

}

//////////////////////////////////////////////////
// class copy constructor of normalization_coeff_computer
normalization_coeff_computer::normalization_coeff_computer(const normalization_coeff_computer& tmp){
}


//////////////////////////////////////////////////
// copy constructor:
normalization_coeff_computer& normalization_coeff_computer::operator=(const normalization_coeff_computer& tmp){
  return *this;
}
    

//////////////////////////////////////////////////
// computes the coefficient at a given TR:
map<int,double> normalization_coeff_computer::compute_global_segment_coefficient(double TR){

  //*********************************
  // compute an arry of local coeffcient for this TR segment
  compute_local_coefficient( TR );

  //*********************************
  // compute a intensity coefficient based only on the overlap of a selection 
  // of LC/MS runs ( is based on the original alignmnet topology, which is not know in this class)
  compute_LC_MS_selected_local_coefficient( TR );
    
  //*********************************
  // comupte the global normalization coefficient:
  compute_average_coefficients();
  // compute_median_coefficients();
  
  //*********************************
  // compute the absolute coefficients:
  compute_absolute_average_coefficients();
  
  return GLOBAL_LC_MS_COEFF;
}


//////////////////////////////////////////////////
// computes the coefficient at a given TR:
void normalization_coeff_computer::compute_local_coefficient(double TR){
  
  // clear data
  ALL_LOCAL_LC_MS_COEFF.clear();
  ABSOLUTE_LC_MS_COEFF.clear();
  
  // define TR borders:
  double start = TR - TR_SLIDE_WINDOW;
  double end = TR + TR_SLIDE_WINDOW;
  
  
  ////////////
  // create a map for the computation of the absolute 
  // intesity variations:
  for( int ID = 0; ID < NB_MATCHES; ID++){
    ABSOLUTE_LC_MS_COEFF.insert( make_pair( ID, 0.0 ) );
  }
  
  
  vector<LC_MS_FEATURE>::iterator FEA = MASTER->get_feature_list_begin();
  while( FEA != MASTER->get_feature_list_end() ){
    
    // check whether has number of rep. match:
    if( (*FEA).get_replicate_match_nb() == NB_MATCHES ){

      // check whether in the retention time range:
      TR = (*FEA).get_retention_time();
      if( (TR <= end) && ( TR >= start) ) {

        ////////////////////////////////////////////////////////////
        // start local coefficient computing:
        // sum up mean intensities of all features within this region: 
        double this_mean_intens = ( (*FEA).get_replicate_intensity_sum() / double( NB_MATCHES ) );
        
        // get variation in form of coefficient for every intensity:
        map<int, double> THIS_COEFF;   
        THIS_COEFF.insert(pair<int, double>((*FEA).get_spectrum_ID(), ( (*FEA).get_peak_area() / this_mean_intens)));
      
        // for absolute varations coefficient:
        map<int, double>::iterator F = ABSOLUTE_LC_MS_COEFF.find( (*FEA).get_spectrum_ID()  );
        (*F).second += (*FEA).get_peak_area();

        map<int,feature>::iterator P = (*FEA).get_match_list_start();
        while( P != (*FEA).get_match_list_end()){
          THIS_COEFF.insert(pair<int, double>( (*P).first, ( (*P).second.get_peak_area() / this_mean_intens)));
          
          // for absolute varations coefficient:
          F = ABSOLUTE_LC_MS_COEFF.find( (*P).first  );
          (*F).second += (*P).second.get_peak_area();
                  
          P++;
        }
        
        // add the local variation to the global segment variation:
        add_local_coefficent( &THIS_COEFF );
        THIS_COEFF.clear();

        // end of local computing:
        /////////////////////////////////////////////////////////////////
      }
    }
    FEA++;
  
  }    
}


///////////////////////////////////////
// add the local variation to the global segment variation:
void normalization_coeff_computer::add_local_coefficent( map<int, double>* IN ){

  map<int, double>::iterator P = IN->begin(); 
  while( P != IN->end() ){

    map<int, vector<double> >::iterator F = ALL_LOCAL_LC_MS_COEFF.find( (*P).first );
    if( F != ALL_LOCAL_LC_MS_COEFF.end() ){
      (*F).second.push_back( (*P).second );
    }
    else{
      vector<double> tmp;
      tmp.push_back( (*P).second );
      ALL_LOCAL_LC_MS_COEFF.insert( make_pair( (*P).first, tmp ) );
    }
    P++;
  }
}


///////////////////////////////////////////////////////////////////
// compute the global normalization coefficient:
void normalization_coeff_computer::compute_average_coefficients(){
  
  GLOBAL_LC_MS_COEFF.clear();  
  
  map<int, vector<double> >::iterator A = ALL_LOCAL_LC_MS_COEFF.begin();
  while( A != ALL_LOCAL_LC_MS_COEFF.end() ){
    
    double av = 0;
    vector<double>::iterator T = (*A).second.begin();
    while( T != (*A).second.end() ){
      av += (*T);
      T++;
    }
    
    av /= double( (*A).second.size() ); 
    GLOBAL_LC_MS_COEFF.insert( make_pair( (*A).first, av )); 
    
    A++;
  }
}


///////////////////////////////////////////////////////////////////
// compute the global normalization coefficient:
void normalization_coeff_computer::compute_median_coefficients(){
  
  GLOBAL_LC_MS_COEFF.clear();  
  
  map<int, vector<double> >::iterator A = ALL_LOCAL_LC_MS_COEFF.begin();
  while( A != ALL_LOCAL_LC_MS_COEFF.end() ){
    
    simple_math M;
    double median = M.MEDIAN( &(*A).second );    
    GLOBAL_LC_MS_COEFF.insert( make_pair( (*A).first, median )); 
    
    A++;
  }
}



///////////////////////////////////////////////////////////////////
// compute the absolute coefficients:
void normalization_coeff_computer::compute_absolute_average_coefficients(){

  GLOBAL_ABSOLUTE_LC_MS_COEFF.clear();

  // compute the average TIC:
  double TOT = 0.0;
  map<int, double>::iterator P = ABSOLUTE_LC_MS_COEFF.begin();
  while( P != ABSOLUTE_LC_MS_COEFF.end() ){
    TOT += (*P).second;
    P++;
  }
  TOT /= double( ABSOLUTE_LC_MS_COEFF.size() );
  
  // compute the coefficient:
  P = ABSOLUTE_LC_MS_COEFF.begin();
  while( P != ABSOLUTE_LC_MS_COEFF.end() ){
    (*P).second /= TOT;
    if( (*P).second > 0 ){ 
      GLOBAL_ABSOLUTE_LC_MS_COEFF.insert( *P );
    }
    P++;
  }
  
}
  



////////////////////////////////////////////////////////////////////////////////
// compute a intensity coefficient based only on the overlap of a selection 
// of LC/MS runs ( is based on the original alignmnet topology, which is not know in this class)
void normalization_coeff_computer::compute_LC_MS_selected_local_coefficient( double TR ){
  
  // clear data
  ALL_LOCAL_LC_MS_COEFF.clear();
  ABSOLUTE_LC_MS_COEFF.clear();
  
  // define TR borders:
  double start = TR - TR_SLIDE_WINDOW;
  double end = TR + TR_SLIDE_WINDOW;
  
  ////////////
  // create a map for the computation of the absolute 
  // intesity variations:
  for( int ID = 0; ID < NB_MATCHES; ID++){
    ABSOLUTE_LC_MS_COEFF.insert( make_pair( ID, 0.0 ) );
  }
  
  vector<LC_MS_FEATURE>::iterator FEA = MASTER->get_feature_list_begin();
  while( FEA != MASTER->get_feature_list_end() ){
    
    // check if it is presenent in the current LC/MS selection
    if( check_FEATURE_LC_MS_selection( &(*FEA) ) ){
      
      // check whether in the retention time range:
      TR = (*FEA).get_retention_time();
      if( (TR <= end) && ( TR >= start) ) {
        
        ////////////////////////////////////////////////////////////
        // start local coefficient computing:
        simple_math MY_MATH;
        // get selected areas from this features according to the current LC/MS selection
        pair< vector<double>, vector<double> > FEATURE_AREAS = get_selected_feature_areas( &(*FEA) );
        
        // sum up mean intensities of all features within this region: 
        pair<double,double> MEAN_INTENS_A = MY_MATH.AVERAGE_and_STDEV( &(FEATURE_AREAS.first) );
        pair<double,double> MEAN_INTENS_B = MY_MATH.AVERAGE_and_STDEV( &(FEATURE_AREAS.second) );
        
        double MEAN_INTENS_TOT = MEAN_INTENS_A.first*double(FEATURE_AREAS.first.size());
        MEAN_INTENS_TOT += MEAN_INTENS_B.first*double(FEATURE_AREAS.second.size());
        MEAN_INTENS_TOT /= double(FEATURE_AREAS.first.size() + FEATURE_AREAS.second.size());
        
        // get variation in form of coefficient for every LC/MS run:
        map<int, double>::iterator F;
        map<int, double> THIS_COEFF; 
        
        
        // compute the run coefficients:
        vector<int>::iterator S = LC_MS_SELECTION_A.begin();
        while( S != LC_MS_SELECTION_A.end() ){
          
          // for RELATIVE varations coefficient:
          THIS_COEFF.insert( make_pair( (*S), MEAN_INTENS_A.first / MEAN_INTENS_TOT ) ) ;
          
          // for ABSOLUTE varations coefficient:
          map<int, double>::iterator F = ABSOLUTE_LC_MS_COEFF.find( (*S) );
          (*F).second += MEAN_INTENS_A.first;
          
          S++;
        }
        
        S = LC_MS_SELECTION_B.begin();
        while( S != LC_MS_SELECTION_B.end() ){
  
          // for RELATIVE varations coefficient:
          THIS_COEFF.insert( make_pair( (*S), MEAN_INTENS_B.first / MEAN_INTENS_TOT ) ) ;
                             
          // for ABSOLUTE varations coefficient:
          map<int, double>::iterator F = ABSOLUTE_LC_MS_COEFF.find( (*S) );
          (*F).second += MEAN_INTENS_B.first;
          
          S++;
        }

      
        // add the local variation to the global segment variation:
        add_local_coefficent( &THIS_COEFF );
        THIS_COEFF.clear();
        
      }
    }
    
    // next feature:
    FEA++;
    
  }    
}


/////////////////////////////////////////////////////////////////////////////////
// check if this features is detected in the current LC/MS selection
bool normalization_coeff_computer::check_FEATURE_LC_MS_selection( feature* IN ){
  
  // check for selection A:
  double A_counter = 0;
  vector<int>::iterator S = LC_MS_SELECTION_A.begin();
  while( S != LC_MS_SELECTION_A.end() ){
    if( IN->check_match_by_id( *S ) ){
      A_counter++;
    }
    S++;
  }
  A_counter /= double( LC_MS_SELECTION_A.size() );
  
  // check for selection B:
  double B_counter = 0;
  S = LC_MS_SELECTION_B.begin();
  while( S != LC_MS_SELECTION_B.end() ){
    if( IN->check_match_by_id( *S ) ){
      B_counter++;
    }
    S++;
  }
  B_counter /= double( LC_MS_SELECTION_B.size() );
  
  if( ( B_counter >= FEATURE_ALIGN_PERCENTAGE) && (A_counter >= FEATURE_ALIGN_PERCENTAGE )){
    return true;
  }
  else{
    return false;
  }
}


/////////////////////////////////////////////////////////////////////////////////
// get selected areas from this features according to the current LC/MS selection
pair< vector<double>, vector<double> > normalization_coeff_computer::get_selected_feature_areas( feature* IN ){
  
  vector<double> TMP_A;  
  vector<int>::iterator S = LC_MS_SELECTION_A.begin();
  while( S != LC_MS_SELECTION_A.end() ){
    if( IN->check_match_by_id( *S ) ){
      TMP_A.push_back( IN->get_peak_area( *S ) );
    }
    S++;
  }
  vector<double> TMP_B;  
  S = LC_MS_SELECTION_B.begin();
  while( S != LC_MS_SELECTION_B.end() ){
    if( IN->check_match_by_id( *S ) ){
      TMP_B.push_back( IN->get_peak_area( *S ) );
    }
    S++;
  }
  
  return make_pair( TMP_A, TMP_B );
}
///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS peptide_ratio_analyzer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_PEPTIDE_RATIO_ANALYZER
#include "RATIOLIZER_USE.h"


int peptide_ratio_analyzer::RATIO_TARGET_LC_MS;
int peptide_ratio_analyzer::REPLICATE_NUMBER;
// select if only feature observed all replicates used
bool peptide_ratio_analyzer::REPLICATE_CHECK;
int peptide_ratio_analyzer::TARGET_ID;
map<int, vector<double> > peptide_ratio_analyzer::LC_MS_Modification_Masses;

////////////////////////////////////////////////
// constructor for the object peptide_ratio_analyzer:
peptide_ratio_analyzer::peptide_ratio_analyzer(){
}

//////////////////////////////////////////////////
// class desctructor of peptide_ratio_analyzer
peptide_ratio_analyzer::~peptide_ratio_analyzer(){
}

//////////////////////////////////////////////////
// class copy constructor of peptide_ratio_analyzer
peptide_ratio_analyzer::peptide_ratio_analyzer(const peptide_ratio_analyzer& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of peptide_ratio_analyzer
peptide_ratio_analyzer::peptide_ratio_analyzer(const peptide_ratio_analyzer* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
peptide_ratio_analyzer& peptide_ratio_analyzer::operator=(const peptide_ratio_analyzer& tmp){
  return *this;
}


//////////////////////////////////////////////////
// analyze this peptide group:
PEPTIDE_RATIO peptide_ratio_analyzer::analyze_peptide( peptide_DELTA_group* IN ){
  
  PEPTIDE_RATIO MY_TMP;
  MY_TMP.TARGET = IN;
  
  // check whether the ratio sould be extracted over different runs ( Target ratio LC/MS > -1 )
  // or if its a differentially lablled run ( Target ratio LC/MS = -1 )
  
  //if( MY_TMP.TARGET->get_nb_members() > 1 ){
  
  if( RATIO_TARGET_LC_MS != -1 ){
    // analyze the peptide group: 
    analyze_peptide_DELTA_group( &MY_TMP );
  }
  else if( RATIO_TARGET_LC_MS == -1 ){
    // analyze now the isotopes, are all in teh same LC/MS run:
    analyze_ISOTOPE_peptide_DELTA_group( &MY_TMP );
  }
  IN = NULL;
  return MY_TMP;
}
    
/////////////////////////////////////////////////////////////////////
// compute the total peptide ratio from the features ratios:
void peptide_ratio_analyzer::analyze_peptide_DELTA_group( PEPTIDE_RATIO* IN ){
  
  /////////////////////////////////
  // build first feature ratios
  // taking int account the replicates:
  vector< feature* > THIS_MEMBERS = IN->TARGET->get_ALL_FEATURE_MEMBERS();
  vector< feature* >::iterator T =  THIS_MEMBERS.begin();
  while( T !=  THIS_MEMBERS.end() ){
  
    feature* FEA = (*T);
 
    // check ( if required ) if in all replicated observed:
    bool CHECK = true;
    if( REPLICATE_CHECK ){ 
      if( FEA->get_replicate_match_nb() >= REPLICATE_NUMBER * 2 ){
        CHECK = true;
      }
      else{
        CHECK = false;
      }
    }
 
    if( CHECK ){
      // compute the ratio of a features:
      pair<double, double> RATIO = get_FEATURE_RATIO( FEA );
      
      if( RATIO.first > 0 ){
        // insert the ratio:
        IN->ALL_FEATURE_RATIOS.push_back( RATIO );
      }
    }
    
    T++;
  }
  // build peptide ratio
  compute_PEPTIDE_ratio( IN );
}



/////////////////////////////////////////////////////////////////////
// compute the total peptide ratio from the features ratios:
void peptide_ratio_analyzer::compute_PEPTIDE_ratio( PEPTIDE_RATIO* IN ){
  
  simple_math MY_MATH;
  // intialize:
  IN->AVERAGE_PEPTIDE_RATIO = -1.0;
  IN->AVERAGE_PEPTIDE_RATIO_STD = -1.0;
  IN->NORMALIZED_AVERAGE_PEPTIDE_RATIO = -1.0;
  IN->P_RATIO_VALUE = -1.0;
  
  /////////////////////////////////
  // now build a peptide ratio
  if(  !IN->ALL_FEATURE_RATIOS.empty() ){
    
    ///////////////////////////
    // outlier testing:
    MY_MATH.OUTLIER_DETECTION_BY_DIXON( &(IN->ALL_FEATURE_RATIOS) );
    
    //////////////////////////
    // OLD VERSION:
    // build the average:
    pair<double, double> TMP = MY_MATH.AVERAGE_and_STDEV( &(IN->ALL_FEATURE_RATIOS) );
    IN->AVERAGE_PEPTIDE_RATIO = TMP.first;
    // build the standard deviation for this peptide
    // combine the individual stdev from the features
    IN->AVERAGE_PEPTIDE_RATIO_STD = TMP.second;
    
    //////////////////////////
    // NEW VERSION:
    // build the average:
    IN->AVERAGE_PEPTIDE_RATIO = computeAverageStdevWeightedRatio( &(IN->ALL_FEATURE_RATIOS) );
    // build the stdev:
    IN->AVERAGE_PEPTIDE_RATIO_STD = computeAverageRatioStdev( &(IN->ALL_FEATURE_RATIOS) );
    
    IN->validAnalysis = true;
  }
  else{
    IN->validAnalysis = false;
  }

}



/////////////////////////////////////////////////////////////////////
// compute the ratio of a features:
pair<double, double> peptide_ratio_analyzer::get_FEATURE_RATIO( feature* IN ){
  
  vector< double> AREAS_A;
  vector< double> AREAS_B;
  pair< double, double> RATIO;
  RATIO.first = -1;
  RATIO.second = -1;
  simple_math MY_MATH;
  
  ///////////////////////////////////////////
  // get the area from target = nominatore
  int id = RATIO_TARGET_LC_MS;
  while( id < ( RATIO_TARGET_LC_MS + REPLICATE_NUMBER ) ){
    
    // find B:
    if( IN->get_spectrum_ID() == id){
      AREAS_A.push_back( IN->get_peak_area() ); 
    }
    else{
      map<int, feature>::iterator F = IN->find_match_by_id( id );
      if( F != IN->get_match_list_end() ){
        AREAS_A.push_back( (*F).second.get_peak_area() ); 
      }
    }
    id++;
  }
  
  
  ///////////////////////////////////////////
  // get the area from other = denominator
  if( RATIO_TARGET_LC_MS != 0 ){
    id = 0;
  }
  else{
    id = REPLICATE_NUMBER;
  }
  int start = id;
  while( id < ( start + REPLICATE_NUMBER ) ){
    
    // find B:
    if( IN->get_spectrum_ID() == id){
      AREAS_B.push_back( IN->get_peak_area() ); 
    }
    else{
      map<int, feature>::iterator F = IN->find_match_by_id( id );
      if( F != IN->get_match_list_end() ){
        AREAS_B.push_back( (*F).second.get_peak_area() ); 
      }
    }
    id++;
  }
  
  if( !AREAS_A.empty() && !AREAS_B.empty() ){

    // average over the replicates: 
    pair<double, double> AREA_A = MY_MATH.AVERAGE_and_STDEV( &AREAS_A );
    // average over the replicates: 
    pair<double, double> AREA_B = MY_MATH.AVERAGE_and_STDEV( &AREAS_B );
    
    
    //////////////////////////////////
    // build finally the feature ratio:
    RATIO.first = AREA_A.first / AREA_B.first;
    
    // extract uncertainty for the average,
    // do by error propagatino from the measurments
    RATIO.second = (AREA_A.second /  AREA_A.first)*(AREA_A.second / AREA_A.first) 
      + (AREA_B.second / AREA_B.first)*(AREA_B.second/AREA_B.first);
    RATIO.second = sqrt(RATIO.second);
    RATIO.second = RATIO.first * RATIO.second;
    
  }
   
  return RATIO;
}



///////////////////////////////////////////////////////////////////
// analyze now the isotopes, are all in teh same LC/MS run:
void peptide_ratio_analyzer::analyze_ISOTOPE_peptide_DELTA_group( PEPTIDE_RATIO* IN ){
  
  // get the features:
  vector< feature* > THIS_MEMBERS = IN->TARGET->get_ALL_FEATURE_MEMBERS();
  // order the feature vector by m/z:
  map<double, feature*> ORDERED_FEATURES;
  vector< feature* >::iterator T =  THIS_MEMBERS.begin();
  while( T !=  THIS_MEMBERS.end() ){
    ORDERED_FEATURES.insert( make_pair( (*T)->get_MZ(), (*T) ) );
    T++;
  }
    
  map< double, feature* >::iterator F =  ORDERED_FEATURES.begin();
  while( F !=  ORDERED_FEATURES.end() ){
    feature* FEA = (*F).second;

    // search now for isotopes, dont build ratios to 
    // different charge states!
    map< double, feature* >::iterator COMP = F;
    COMP++;
    
    while( COMP != ORDERED_FEATURES.end() ){
    
      feature* COMP_FEA = (*COMP).second;
      // check if same charge state:
      if( ( COMP_FEA->get_charge_state() == FEA->get_charge_state() ) && ( compare_Mass( FEA->get_Molecular_Mass(), COMP_FEA->get_Molecular_Mass() ) ) ){
        
        // build a ratio:
        double R = -1.0;
        R = COMP_FEA->get_peak_area() / FEA->get_peak_area();        
        
        // compute the ratio of a features:
        pair<double, double> RATIO = make_pair( R, 0.0 );
        // insert the ratio:
        IN->ALL_FEATURE_RATIOS.push_back( RATIO );
      
      }
      COMP++;
     }
    
    F++;
  }
  
  // now build a peptide ratio
  compute_PEPTIDE_ratio( IN );

}



//////////////////////////////////////////////////
// compare if masses A/B match ( check for modification shift)
bool peptide_ratio_analyzer::compare_Mass( double A, double B ){
  
  map<int, vector<double> >::iterator T = LC_MS_Modification_Masses.find( 0 );
  vector<double>::iterator MOD = (*T).second.begin();
  while( MOD != (*T).second.end() ){ 
    
    double MOD_SHIFT = (*MOD);
    if( feature::compareFeatureMassValuesAtPPMLevel( fabs( A - B ), MOD_SHIFT) ){
      return true;
    }
   
    // next shift
    MOD++;
  }
  return false;
}


///////////////////////////////////////////////////////////////
// computes the stdev incorporates peptide average ratio
double peptide_ratio_analyzer::computeAverageStdevWeightedRatio( vector< pair<double,double> >* fRatios ){
  
  double avRatio = 0;
  
		// extract first for each ratio the ratio and stdev
		double stdevSum = 0;
		double nonZeroStdev = 0;
  vector< pair<double, double> >::iterator I = fRatios->begin();
		while( I != fRatios->end() ) {
    pair<double, double> fe = (*I);
    
    if (fe.second > 0) {
      nonZeroStdev++;
      stdevSum += fe.second;
    }
    
    avRatio += fe.first;
    I++;
		}
  
		// in the case non of the feature ratios
		// contained a stdev, make easy average:
		if( nonZeroStdev == 0 ){
    avRatio /= fRatios->size();
		}
		// other wise use stdev as weights for the
		// average:
		else if (nonZeroStdev > 0) {
    
    // compute now for every feature which does not
    // contain a stdev a fake one:
    double fake_stdev = stdevSum / nonZeroStdev;
    
    // now compute a ratio using stdevs as weights:
    double nom = 0;
    double denom = 0;
    
    I = fRatios->begin();
    while( I != fRatios->end() ) {
      pair<double, double> fe = (*I);
      double fAv = fe.first;
      double fSt = fe.second;
      
      // replace zero stdev with the fake stdev
      // -> cheap way of avoiding zero values:
      if (fSt == 0) {
        fSt = fake_stdev;
      }
      
      // n here could be the replicate number
      // like a weight from how many times the
      // ratios and stdev was computed
      //
      // accoriding to the formula:
      //
      // nom = sum[ fAv_i * n_i * 1 / sqrt(fSt_i ) ]
      // denom = sum[ n_i * 1 / sqrt(fSt_i ) ]
      // r_av = nom/denom:
      double n = 1;
      
      nom += fAv * n * (1 / sqrt(fSt));
      denom += n * (1 / sqrt(fSt));
      I++;
    }
    
    // combine into division:
    avRatio = nom / denom;
		}
  
  
  
  return avRatio;
}


/////////////////////////////////////////////////////////
// computes the stdev incorporates peptide average ratio
double peptide_ratio_analyzer::computeAverageRatioStdev( vector< pair<double,double> >* fRatios ) {
  
		double ratioStdev = 0;
  
		// extract first for ech feature ratio the ratio and stdev
		vector<double> allRatios;
		double stdevSum = 0;
		double nonZeroStdev = 0;
	 vector< pair<double, double> >::iterator I = fRatios->begin();
		while( I != fRatios->end() ) {
    pair<double, double> fe = (*I);
    if (fe.second > 0) {
      nonZeroStdev++;
      stdevSum += fe.second;
    }
    allRatios.push_back( fe.first);
    I++;
		}
  
		// in the case non of the feature ratios
		// contained a stdev, make easy average:
		if( nonZeroStdev == 0 ){
    simple_math ma;
    ratioStdev = ma.AVERAGE_and_STDEV(&allRatios).second;
		}
		// other wise use stdev as weights for the
		// average:
		else if (nonZeroStdev > 0) {
    
    // compute now for every feature which does not
    // contain a stdev a fake one:
    double fake_stdev = stdevSum / nonZeroStdev;
    
    // now compute a ratio using stdevs as weights:
    double combinedStdev = 0;
    
    vector< pair<double, double> >::iterator I = fRatios->begin();
    while( I != fRatios->end() ) {
      pair<double, double> fe = (*I);
      double fSt = fe.second;
      // replace zero stdev with the fake stdev
      // -> cheap way of avoiding zero values:
      if (fSt == 0) {
        fSt = fake_stdev;
      }
      combinedStdev += fSt*fSt;
      I++;
      
    }
    
    // combine into division:
    ratioStdev = sqrt( combinedStdev ) / allRatios.size();
		}
  
  return ratioStdev;
}
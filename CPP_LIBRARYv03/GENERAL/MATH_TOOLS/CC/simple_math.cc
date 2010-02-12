  
///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS simple_math:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_SIMPLE_MATH
#define USE_ALL_STANDART_CLASSES
#include "MATH_TOOLS_USE.h"



// significance values:
const double simple_math::T_TEST_001[12]={.999,.964,.895,.822,.763,.716,.675,.647,.544,.491,.455,.430};
const double simple_math::T_TEST_002[12]={.998,.949,869,.792,.731,.682,.644,.614,.515,.464,.430,.407};
const double simple_math::T_TEST_01[12]={.886,.679,.559,.484,.433,.398,.370,.349,.284,.251,.230,.216};
const double simple_math::T_TEST_02[12]={.782,.561,.452,.387,.344,.314,.291,.274,.220,.193,.176,.165};
const double simple_math::T_TEST_05[12]={.782,.561,.452,.387,.344,.314,.291,.274,.220,.193,.176,.165};
string simple_math::ALPHA_VALUE="0.05";

////////////////////////////////////////////////
// constructor for the object simple_math:
simple_math::simple_math(){
  LOW_CHECK = true;
  HIGH_CHECK = true;
}

//////////////////////////////////////////////////
// class desctructor of simple_math
simple_math::~simple_math(){
}

//////////////////////////////////////////////////
// class copy constructor of simple_math
simple_math::simple_math(const simple_math& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of simple_math
simple_math::simple_math(const simple_math* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
simple_math& simple_math::operator=(const simple_math& tmp){
  return *this;
}

//////////////////////////////////////////////////
// round at certain comma position
double simple_math::ROUND_NUMBER(double IN, int comma_pos){
  
  double tmp = IN * pow( 10.0, comma_pos);
  tmp += .5;
  tmp = floor(tmp);
  tmp /= pow( 10.0, comma_pos);
  return tmp;
  
}


//////////////////////////////////////////////////
// copmute sum over a vector:
double simple_math::SUM(vector<double>* IN){
  double SUM = 0;
  vector<double>::iterator P = IN->begin();
  while( P != IN->end() ){
    SUM += (*P);
    P++;
  }
  return SUM;
}

//////////////////////////////////////////////////
// compute the average only:
double simple_math::AVERAGE(vector<double>* IN){
  
  double AVERAGE = 0;
  
  if( IN->empty() ){
    return AVERAGE;  
  }
  
  if( IN->size() > 1 ){
    vector<double>::iterator START = IN->begin();  
    while( START != IN->end() ){
      AVERAGE += (*START);
      START++;
    }
    AVERAGE /= double( IN->size() );    
    return AVERAGE;  
  }
  else{
    return *(IN->begin());
  }
}
//////////////////////////////////////////////////
// compute the average only:
double simple_math::AVERAGE(map<int, double>* IN){
  
  double AVERAGE = 0;
  
  if( IN->empty() ){
    return AVERAGE;  
  }
  
  if( IN->size() > 1 ){
    map<int, double>::iterator START = IN->begin();  
    while( START != IN->end() ){
      AVERAGE += (*START).second;
      START++;
    }
    AVERAGE /= double( IN->size() );    
    return AVERAGE;  
  }
  else{
    return (*(IN->begin())).second;
  }
}

  
//////////////////////////////////////////////////
// compute the median:
double simple_math::MEDIAN(vector<double>* IN){
  
  double median = 0;
  if( !IN->empty() ){
    
    // sort the array:
    sort( IN->begin(), IN->end() );
    vector<double>::iterator START = IN->begin();  
    
    int size = IN->size();
    // find median:
    if( fmod( double(size), 2.0 ) == 0 ){
      int up_mid = size / 2 ;
      START += up_mid;
      median = (*START);
      START--;
      median += (*START);
      median /= 2.0;
    }
    else{
      int mid = int( floor( double(size)/2.0 ) ); 
      START += mid;
      median = (*START);
    }
  }
  return median;
}

//////////////////////////////////////////////////
// compute the median:
double simple_math::MEDIAN(vector< pair<double, double> >* IN){
  
  double median = 0;
  if( !IN->empty() ){
    
    // sort the array:
    sort( IN->begin(), IN->end() );
    vector< pair<double,double> >::iterator START = IN->begin();  
    
    int size = IN->size();
    // find median:
    if( fmod( double(size), 2.0 ) == 0 ){
      int up_mid = size / 2 ;
      START += up_mid;
      median = (*START).first;
      START--;
      median += (*START).first;
      median /= 2.0;
    }
    else{
      int mid = int( floor( double(size)/2.0 ) ); 
      START += mid;
      median = (*START).first;
    }
  }
  return median;
}


//////////////////////////////////////////////////
// compute the median:
double simple_math::MEDIAN(map< double, double >* IN){
  
  double median = 0;
  if( !IN->empty() ){
    
    map< double,double >::iterator START = IN->begin();  
    
    int size = IN->size();
    // find median:
    if( fmod( double(size), 2.0 ) == 0 ){
      int up_mid = size / 2 ;
      int c = 0;
      while( c != up_mid ){
        START++;
        c++;
      }      
      median = (*START).first;
      START--;
      median += (*START).first;
      median /= 2.0;
    }
    else{
      int mid = int( floor( double(size)/2.0 ) ); 
      int c = 0;
      while( c != mid ){
        START++;
        c++;
      }            
      median = (*START).first;
    }
  }
  return median;
}


//////////////////////////////////////////////////
// get a sub vector from this one from the percentile on
vector<double> simple_math::PercentilSubVector(vector<double>* IN, double PER){
  
  vector<double> OUT;
  if( !IN->empty() ){
    
    // sort the array:
    sort( IN->begin(), IN->end() );
    double size = double(IN->size());
    double pos = PER * size;
    // round:
    pos = ROUND_NUMBER( pos, 0);
    vector<double>::iterator START = IN->begin();
    if( pos > 0 ){
      START = START + (int)pos;
      START--;
    }
    OUT.insert( OUT.begin(), START, IN->end() );
    
  }
  return OUT;
}


//////////////////////////////////////////////////
// compute the median:
double simple_math::MEDIAN(map<int, double>* IN){
  
  vector<double> tmp;
  map<int, double>::iterator P = IN->begin();
  while( P != IN->end() ){
    tmp.push_back( (*P).second );
    P++;
  }
  
  return MEDIAN( &tmp );
}




//////////////////////////////////////////////////
// compute the average and stdev:
pair<double, double> simple_math::AVERAGE_and_STDEV(vector<double>* IN){

  double AVERAGE = 0;
  double STDEV = 0;
  
  if( IN->empty() ){
    return make_pair(AVERAGE, STDEV);  
  }
  
  if( IN->size() > 1 ){
    vector<double>::iterator START = IN->begin();  
    while( START != IN->end() ){
      AVERAGE += (*START);
      START++;
    }
    AVERAGE /= double( IN->size() );
    
    START = IN->begin();  
    while( START != IN->end() ){
      STDEV += ( ( AVERAGE - (*START) )*( AVERAGE - (*START) ) );
      START++;
    }
    STDEV /= double( IN->size() );
    STDEV = sqrt( STDEV );
    return make_pair(AVERAGE, STDEV);  
  }
  else{
    return make_pair( (*IN->begin()), 0.0);
  }
}

//////////////////////////////////////////////////
// compute the average and stdev:
pair<double, double> simple_math::AVERAGE_and_STDEV(map<int,double>* IN){
  
  double AVERAGE = 0;
  double STDEV = 0;
  
  if( IN->empty() ){
    return make_pair(AVERAGE, STDEV);  
  }
  
  if( IN->size() > 1 ){
    map<int, double>::iterator START = IN->begin();  
    while( START != IN->end() ){
      AVERAGE += (*START).second;
      START++;
    }
    AVERAGE /= double( IN->size() );
    
    START = IN->begin();  
    while( START != IN->end() ){
      STDEV += ( ( AVERAGE - (*START).second )*( AVERAGE - (*START).second ) );
      START++;
    }
    STDEV /= double( IN->size() );
    STDEV = sqrt( STDEV );
    return make_pair(AVERAGE, STDEV);  
  }
  else{
    return make_pair( ((*(IN->begin())).second), 0.0);
  }
}

//////////////////////////////////////////////////
// compute the average and stdev:
pair<double, double> simple_math::AVERAGE_and_STDEV(vector< pair<double,double> >* IN){
  
  double AVERAGE = 0;
  double STDEV = 0;
  
  if( IN->size() > 1 ){
    vector< pair<double,double> >::iterator START = IN->begin();  
    while( START != IN->end() ){
      AVERAGE += (*START).first;
      START++;
    }
    AVERAGE /= double( IN->size() );
    
    START = IN->begin();  
    while( START != IN->end() ){
      STDEV += ( ( AVERAGE - (*START).first )*( AVERAGE - (*START).first ) );
      START++;
    }
    STDEV /= double( IN->size() );
    STDEV = sqrt( STDEV );
    return make_pair(AVERAGE, STDEV);  
  }
  else{
    return make_pair( (*IN->begin()).first, 0.0);
  }
}


//////////////////////////////////////////////////
// compute the average and stdev:
pair<double, double> simple_math::AVERAGE_and_STDEV(map< double, void* >* IN){
  
  double AVERAGE = 0;
  double STDEV = 0;
  
  if( IN->size() > 1 ){
    map< double, void* >::iterator START = IN->begin();  
    while( START != IN->end() ){
      AVERAGE += (*START).first;
      START++;
    }
    AVERAGE /= double( IN->size() );
    
    START = IN->begin();  
    while( START != IN->end() ){
      STDEV += ( ( AVERAGE - (*START).first )*( AVERAGE - (*START).first ) );
      START++;
    }
    STDEV /= double( IN->size() );
    STDEV = sqrt( STDEV );    
    IN = NULL;
    return make_pair(AVERAGE, STDEV);  
  }
  else{    
    IN = NULL;
    return make_pair( (*IN->begin()).first, 0.0 );
  }
}

//////////////////////////////////////////////////
// compute the average and stdev:
pair<double, double> simple_math::AVERAGE_and_STDEV(map< double, double >* IN){
  
  double AVERAGE = 0;
  double STDEV = 0;
  
  if( IN->size() > 1 ){
    map< double, double >::iterator START = IN->begin();  
    while( START != IN->end() ){
      AVERAGE += (*START).first;
      START++;
    }
    AVERAGE /= double( IN->size() );
    
    START = IN->begin();  
    while( START != IN->end() ){
      STDEV += ( ( AVERAGE - (*START).first )*( AVERAGE - (*START).first ) );
      START++;
    }
    STDEV /= double( IN->size() );
    STDEV = sqrt( STDEV );
    IN = NULL;
    return make_pair(AVERAGE, STDEV);  
  }
  else{
    IN = NULL;
    return make_pair( (*IN->begin()).first, 0.0 );
  }
}

//////////////////////////////////////////////////
// compute the average and stdev:
pair<double, double> simple_math::AVERAGE_and_STDEV(vector< pair<double, void*> >* IN){
  
  double AVERAGE = 0;
  double STDEV = 0;
  
  if( IN->size() > 1 ){
    vector< pair<double, void*> >::iterator START = IN->begin();  
    while( START != IN->end() ){
      AVERAGE += (*START).first;
      START++;
    }
    AVERAGE /= double( IN->size() );
    
    START = IN->begin();  
    while( START != IN->end() ){
      STDEV += ( ( AVERAGE - (*START).first )*( AVERAGE - (*START).first ) );
      START++;
    }
    STDEV /= double( IN->size() );
    STDEV = sqrt( STDEV );
    IN = NULL;
    return make_pair(AVERAGE, STDEV);  
  }
  else if( IN->size() == 1 ){
    return make_pair( (*IN->begin()).first, 0.0 );
  }
  else{
    return make_pair( 0.0 , 0.0 );
  }
}

//////////////////////////////////////////////////
// compute the average and stdev:
pair<double, double> simple_math::WEIGHTED_AVERAGE_and_STDEV(vector< pair<double, double> >* IN){
  
  double AVERAGE = 0;
  double STDEV = 0;
  double TOT_WEIGHT = 0;  
  
  if( IN->size() > 1 ){
    
    vector< pair<double, double> >::iterator START = IN->begin();  
    while( START != IN->end() ){
      TOT_WEIGHT += (*START).second;
      START++;
    }
    // normalize the weights:
    START = IN->begin();  
    while( START != IN->end() ){
      (*START).second /= TOT_WEIGHT;
      START++;
    }
    
    // build average:
    START = IN->begin();
    while( START != IN->end() ){
      AVERAGE += (*START).first * (*START).second;
      START++;
    }
    // AVERAGE /= double( IN->size() );    
    
    START = IN->begin();  
    while( START != IN->end() ){
      STDEV += ( ( AVERAGE - (*START).first )*( AVERAGE - (*START).first ) );
      START++;
    }
    STDEV /= double( IN->size() );
    STDEV = sqrt( STDEV );
    IN = NULL;
    return make_pair(AVERAGE, STDEV);  
  }
  else{
    IN = NULL;
    return make_pair( (*IN->begin()).first, 0.0 );
  }
}


//////////////////////////////////////////////////
// compute the weighted average:
double simple_math::WEIGHTED_AVERAGE(map< double, double >* IN){
  
  double AVERAGE = 0;
  double TOT_WEIGHT = 0;  
  
  if( IN->size() > 1 ){
    
    map< double, double >::iterator START = IN->begin();  
    while( START != IN->end() ){
      TOT_WEIGHT += (*START).second;
      AVERAGE += ( (*START).first * (*START).second );
      START++;
    }
    
    return AVERAGE / TOT_WEIGHT;  
  }
  else{
    return (*IN->begin()).first;
    IN = NULL;
  }
}

//////////////////////////////////////////////////
// compute the weighted median:
double simple_math::WEIGHTED_MEDIAN(map< double, double >* IN){
  
  if( IN->size() > 1 ){
    
    double nbMEDIAN = 0;
    double MEDIAN = 0;
    double TOT_WEIGHT = 0;  
        
    map< double, double >::iterator START = IN->begin();
    double count = 0;
    while( START != IN->end() ){
      TOT_WEIGHT += (*START).second;
      nbMEDIAN += ( count * (*START).second );
      count++;
      START++;
    }
    nbMEDIAN = MEDIAN / TOT_WEIGHT;
    
    START = IN->begin();  
    int size = IN->size();
    // find median:
    if( fmod( double(size), 2.0 ) == 0 ){
    
      int up_mid = size / 2 ;
      int c = 0;
      while( c != up_mid ){
        START++;
        c++;
      }   
      MEDIAN = (*START).first;
      START--;
      MEDIAN += (*START).first;
      MEDIAN /= 2.0;
    }
    else{
      int mid = int( floor( double(size)/2.0 ) ); 
      int c = 0;
      while( c != mid ){
        START++;
        c++;
      }   
      MEDIAN = (*START).first;
    }
    return MEDIAN;  
  }
  else{
    IN = NULL;
    return (*IN->begin()).first;
  }
}





//////////////////////////////////////////////////
// compute the average and std error:
pair<double, double> simple_math::AVERAGE_and_STDERROR(vector<double>* IN){
  
  pair<double, double> MY = AVERAGE_and_STDEV( IN );
  MY.second /= sqrt( double( IN->size() ) );
  return MY;
  
}

//////////////////////////////////////////////////
// compute the average and std error:
pair<double, double> simple_math::WEIGHTED_AVERAGE_and_STDERROR(vector< pair<double,double> >* IN){
  
  pair<double, double> MY = WEIGHTED_AVERAGE_and_STDEV( IN );
  MY.second /= sqrt( double( IN->size() ) );
  return MY;
  
}




//////////////////////////////////////////////////
// iterative detection of outliers by the DIXON's test (Gibbons, 1994):
void simple_math::ITERATIVE_OUTLIER_DETECTION_BY_DIXON(vector<double>* IN){

  vector < pair<double, double> > TMP;
  vector<double>::iterator P = IN->begin();
  while( P != IN->end() ){
    TMP.push_back( make_pair( *P, 0) );
    P++;
  }
  IN->clear();
  

  ////////////////
  // start here teh outlier detection iteratively:
  unsigned int size;
  do{
    size = TMP.size();
    // make a outlier detection:
    OUTLIER_DETECTION_BY_DIXON( &TMP );
  }
  while( size != TMP.size() );
  
  
  vector< pair<double, double> >::iterator T = TMP.begin();
  while( T != TMP.end() ){
    IN->push_back( (*T).first );
    T++;
  }   

}

//////////////////////////////////////////////////
// iterative detection of outliers by the DIXON's test (Gibbons, 1994):
void simple_math::ITERATIVE_OUTLIER_DETECTION_BY_DIXON(vector< pair<double,double> >* IN){
  
  ////////////////
  // start here teh outlier detection iteratively:
  unsigned int size;
  do{
    size = IN->size();
    // make a outlier detection:
    OUTLIER_DETECTION_BY_DIXON( IN );
  }
  while( size != IN->size() );
  
}


//////////////////////////////////////////////////
// iterative detection of outliers by the DIXON's test (Gibbons, 1994):
void simple_math::ITERATIVE_OUTLIER_DETECTION_BY_DIXON(vector< pair<double, void*> >* IN){
  
  ////////////////
  // start here teh outlier detection iteratively:
  unsigned int size;
  do{
    size = IN->size();
    // make a outlier detection:
    OUTLIER_DETECTION_BY_DIXON( IN );
  }
  while( size != IN->size() );
  
}


//////////////////////////////////////////////////
// detection of outliers by the DIXON's test (Gibbons, 1994):
void simple_math::OUTLIER_DETECTION_BY_DIXON(vector<double>* IN){
  
  vector < pair<double, double> > TMP;
  vector<double>::iterator P = IN->begin();
  while( P != IN->end() ){
    TMP.push_back( make_pair( *P, 0) );
    P++;
  }
  IN->clear();
  
  // to the actual outlier detection:
  OUTLIER_DETECTION_BY_DIXON( &TMP );
  
  vector< pair<double, double> >::iterator T = TMP.begin();
  while( T != TMP.end() ){
    IN->push_back( (*T).first );
    T++;
  }  
}


//////////////////////////////////////////////////
// detection of outliers by the DIXON's test (Gibbons, 1994):
void simple_math::OUTLIER_DETECTION_BY_DIXON(vector< pair<double, double> >* IN){
  
  int SAMPLE_SIZE = IN->size();
  
  if( (SAMPLE_SIZE > 2) && (SAMPLE_SIZE < 2000) ){
    
    vector< pair<double, double> >::iterator START = IN->begin();
    vector< pair<double, double> >::iterator END = IN->end();
    END--;
    
    double TAU_HIGH;
    double TAU_LOW;
    
    if( SAMPLE_SIZE < 8 ){
      
      double XN = (*END).first;
      END--;
      double XN_1 = (*END).first;
      double X1 = (*START).first;
      START++;
      double X2 = (*START).first;
      
      TAU_HIGH = ( XN - XN_1) / ( XN - X1 );
      TAU_LOW = ( X2 - X1) / ( XN - X1 );      
    }
    else if( SAMPLE_SIZE < 11 ){
      
      double XN = (*END).first;
      END--;
      double XN_1 = (*END).first;
      double X1 = (*START).first;
      START++;
      double X2 = (*START).first;
      
      TAU_HIGH = ( XN - XN_1) / ( XN - X2 ); 
      TAU_LOW = ( X2 - X1) / ( XN_1 - X1 ); 
      
    }
    else if( SAMPLE_SIZE < 14 ){
      
      double XN = (*END).first;
      END--;
      double XN_1 = (*END).first;
      END--;
      double XN_2 = (*END).first;
      double X1 = (*START).first;
      START++;
      double X2 = (*START).first;
      START++;
      double X3 = (*START).first;
      
      TAU_HIGH = ( XN - XN_2) / ( XN - X2 ); 
      TAU_LOW = ( X3 - X1) / ( XN_1 - X1 ); 
      
    }
    else{
      
      double XN = (*END).first;
      END--;
      END--;
      double XN_2 = (*END).first;
      double X1 = (*START).first;
      START++;
      START++;
      double X3 = (*START).first;
      
      TAU_HIGH = ( XN - XN_2) / ( XN - X3 ); 
      TAU_LOW = ( X3 - X1) / ( XN_2 - X1 ); 
      
    }
    
    
    // delete here outliers if detected:
    if( check_T_TEST( TAU_HIGH, SAMPLE_SIZE ) && HIGH_CHECK ){
      END = IN->end();
      END--;
      IN->erase( END );
    }
    
    if( check_T_TEST( TAU_LOW, SAMPLE_SIZE )  && LOW_CHECK ){
      START = IN->begin();
      IN->erase( START );
    }
    
  }
}



///////////////////////////////////////////////
// check T-TEST:
bool simple_math::check_T_TEST( double IN , int SAMPLE_NB){
    
  // do only if more than 3 values;
  if( SAMPLE_NB < 3 ){
    return false;
  }
  
  const double* SIGN = NULL;
  if( ALPHA_VALUE == "0.001"){ 
    SIGN = T_TEST_001;
  }
  else if( ALPHA_VALUE == "0.002"){
    SIGN = T_TEST_002;
  }
  else if( ALPHA_VALUE == "0.01"){
    SIGN = T_TEST_01;
  }
  else if( ALPHA_VALUE == "0.02"){
    SIGN = T_TEST_02;
  }
  else if( ALPHA_VALUE == "0.05"){
    SIGN = T_TEST_05;
  }
  else{
    SIGN = T_TEST_01;
  }
  
  if( SAMPLE_NB < 11 ){
    if( SIGN[SAMPLE_NB - 3] <= IN ){
      return true;
    }
  }
  else if( SAMPLE_NB < 16 ){
    if( SIGN[8] <= IN ){
      return true;
    }   
  }
  else if( SAMPLE_NB < 21 ){
    if( SIGN[9] <= IN ){
      return true;
    }    
  }      
  else if( SAMPLE_NB < 25 ){
    if( SIGN[10] <= IN ){
      return true;
    }    
  }
  else if( SAMPLE_NB < 31 ){
    if( SIGN[10] <= IN ){
      return true;
    }    
  }
  else{
    if( SIGN[10] <= IN ){
      return true;
    }    
  }
  
  return false;
}


//////////////////////////////////////////////////
// detection of outliers by the DIXON's test (Gibbons, 1994):
void simple_math::OUTLIER_DETECTION_BY_DIXON(vector< pair<double, void*> >* IN){
  
  // sort the vector:
  sort( IN->begin(), IN->end(), VECTOR_OPERATOR() );
  
  int SAMPLE_SIZE = IN->size();
  
  if( (SAMPLE_SIZE > 2) && (SAMPLE_SIZE < 2000) ){
    
    vector< pair<double, void*> >::iterator START = IN->begin();
    vector< pair<double, void*> >::iterator END = IN->end();
    END--;
    
    double TAU_HIGH;
    double TAU_LOW;
    
    if( SAMPLE_SIZE < 8 ){
      
      double XN = (*END).first;
      END--;
      double XN_1 = (*END).first;
      double X1 = (*START).first;
      START++;
      double X2 = (*START).first;
      
      TAU_HIGH = ( XN - XN_1) / ( XN - X1 );
      TAU_LOW = ( X2 - X1) / ( XN - X1 );      
    }
    else if( SAMPLE_SIZE < 11 ){
      
      double XN = (*END).first;
      END--;
      double XN_1 = (*END).first;
      double X1 = (*START).first;
      START++;
      double X2 = (*START).first;
      
      TAU_HIGH = ( XN - XN_1) / ( XN - X2 ); 
      TAU_LOW = ( X2 - X1) / ( XN_1 - X1 ); 
      
    }
    else if( SAMPLE_SIZE < 14 ){
      
      double XN = (*END).first;
      END--;
      double XN_1 = (*END).first;
      END--;
      double XN_2 = (*END).first;
      double X1 = (*START).first;
      START++;
      double X2 = (*START).first;
      START++;
      double X3 = (*START).first;
      
      TAU_HIGH = ( XN - XN_2) / ( XN - X2 ); 
      TAU_LOW = ( X3 - X1) / ( XN_1 - X1 ); 
      
    }
    else{
      
      double XN = (*END).first;
      END--;
      END--;
      double XN_2 = (*END).first;
      double X1 = (*START).first;
      START++;
      START++;
      double X3 = (*START).first;
      
      TAU_HIGH = ( XN - XN_2) / ( XN - X3 ); 
      TAU_LOW = ( X3 - X1) / ( XN_2 - X1 ); 
      
    }
    
    
    // delete here outliers if detected:
    if( check_T_TEST( TAU_HIGH, SAMPLE_SIZE ) ){
      END = IN->end();
      END--;
      IN->erase( END );
    }
    if( check_T_TEST( TAU_LOW, SAMPLE_SIZE ) ){
      START = IN->begin();
      IN->erase( START );
    }
    
  }
}


////////////////////////////////////////////////////
// tranform intensty of a feature:
double simple_math::log_tranform_F_int(double IN ){
  if( IN != 0 ){
    return log10( IN );
  }
  else{
    return 0.0;
  }
  if( IN < 0 ){
    printf("\t\t*** ERROR: log10 of negative value! \n");
    return log10( IN );
  }
  
}


//////////////////////////////////////////////////////
// compare to masses at the PPM value and decided
// if they fall into the m/z tolerance window
bool simple_math::compareMassValuesAtPPMLevel( double mzA, double mzB, double PPM_TOLERANCE ){
  
  // take the average mass:
  double avMass = (mzA + mzB) / 2.0;
  
  // define the parts per million:
  double ppmValue = avMass / 1000000.00;
  double ppmDeltaTol = ppmValue * PPM_TOLERANCE;
  
  double deltaMass = fabs( mzA - mzB);
  if( deltaMass > ppmDeltaTol ){
    return false;
  }
  
  return true;
}


//////////////////////////////////////////////////////
// get the masse error at the PPM value 
double simple_math::getMassErrorAtPPMLevel( double mz, double PPM_TOLERANCE ){
  double ppmValue = mz / 1000000.00;
  return ppmValue * PPM_TOLERANCE;
}

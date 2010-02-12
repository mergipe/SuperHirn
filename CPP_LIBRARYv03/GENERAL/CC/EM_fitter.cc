///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS EM_fitter:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_EM_FITTER
#include "GENERAL_USE.h"



////////////////////////////////////////////////
// constructor for the object EM_fitter:
EM_fitter::EM_fitter(){

  DATA_MEAN = 0.0;
  DATA_VAR = 0.0;
  PI = 0;
  CONVERGENCE_CHECK = false;
  MODELING_CHECK = false;
  TEXT_OUT = false;
  DATA = NULL;
  srand( time(NULL));
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();  
  // print out
  def->search_tag("gnuplot plot generator", &PLOT);
  // ceonvergence of profile prophet 
  def->search_tag("profile prophet convergence", &CONVERGENCE);
  // ceonvergence of profile prophet 
  def->search_tag("profile prophet maximal iterations", &MAX_IT);
  // minimal number of histogram data points for EM fit:
  def->search_tag("Min. number histo data points", &MIN_NB_HISTOGRAM_DATA_POINTS);
  // number of histogram bins for the EM plot:
  def->search_tag("Number of histogram plot bins", &NB_HISTOGRAM_BINS);
  // number of histogram bins for the EM plot:
  def->search_tag("gauss data percentile", &Data_percentile);
  
  delete def;
  def = NULL;
  
}


////////////////////////////////////////////////
// constructor for the object EM_fitter:
EM_fitter::EM_fitter(string IN){
  CONVERGENCE_CHECK = false;
  MODELING_CHECK = false;
  ANALYSIS_NAME = IN;
  DATA_MEAN = 0.0;
  DATA_VAR = 0.0;
  PI = 0;
  DATA = NULL;
  TEXT_OUT = false;
  srand( time(NULL));

  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();  
  // print out
  def->search_tag("gnuplot plot generator", &PLOT);
  // ceonvergence of profile prophet 
  def->search_tag("profile prophet convergence", &CONVERGENCE);
  // ceonvergence of profile prophet 
  def->search_tag("profile prophet maximal iterations", &MAX_IT);
  // minimal number of histogram data points for EM fit:
  def->search_tag("Min. number histo data points", &MIN_NB_HISTOGRAM_DATA_POINTS);
  // minimal number of histogram data points for EM fit:
  def->search_tag("Number of histogram plot bins", &NB_HISTOGRAM_BINS);
  // number of histogram bins for the EM plot:
  def->search_tag("gauss data percentile", &Data_percentile);

  delete def;
  def = NULL;
  
}

//////////////////////////////////////////////////
// class desctructor of EM_fitter
EM_fitter::~EM_fitter(){
  DATA = NULL;
  CONVERGENCE_ROUND.clear();
}

//////////////////////////////////////////////////
// class copy constructor of EM_fitter
EM_fitter::EM_fitter(const EM_fitter& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of EM_fitter
EM_fitter::EM_fitter(const EM_fitter* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
EM_fitter& EM_fitter::operator=(const EM_fitter& tmp){
  return *this;
}

//////////////////////////////////////////////////
// initialze:
void EM_fitter::INIT_2COMP(){

  CONVERGENCE_ROUND.clear();
  NB_IT = 0;
  CONVERGENCE_CHECK = false;
  
  // init the means of the gauss.dist.
  // as (perc) th and (1-perc) th percentile:
  double perc = 0.001;
  double size = double(DATA->size());
  map<double, double>::iterator P = DATA->begin();
  double nb = 0.1;
  while( nb < perc * size){ 
    P++;
    nb++;
  }
  GAUSS_1.mean = (*P).first;
  while( nb < ( 1.0 - perc) * size){ 
    P++;
    nb++;
  }
  GAUSS_2.mean = (*P).first;
  
  // initialize the variance:
  // use the variance of the dataset:
  analyze_input_data( 1.0 );
  
  // take a tenth of the data variance:
  GAUSS_1.variance = DATA_VAR / 50;
  GAUSS_2.variance = DATA_VAR;

  PI = 0.5;
  
  // redo the initialization in another way:
  // gauss_1 is all the way left = smallest score
  simple_math MATHER;
  pair<double, double> STAT = MATHER.AVERAGE_and_STDEV( DATA );
  P = DATA->begin();
  GAUSS_1.mean = (*P).first;
  GAUSS_1.variance = STAT.second / 500;

  map<double, double>::reverse_iterator P_REV = DATA->rbegin();
  GAUSS_2.mean = (*P_REV).first;
  GAUSS_2.variance = STAT.second;
   
}


/////////////////////////////////////////////////////
// randomized access of the data:
double EM_fitter::get_random_data(){

  if( !DATA->empty() ){
    int max = DATA->size() + 1;
    int pos = rand()%max;
    map<double, double>::iterator p = DATA->begin();
    for(int i=0; i < pos; i++ ){
      p++;
    }
    return (*p).first;
  }
  else{
    return 0.0;
  }
}

/////////////////////////////////////////////////////
// analize input data ( mean / stdev):
void EM_fitter::analyze_input_data( double PERCENTILE){
  
  if( !DATA->empty() ){
    
    // process data for percentile:
    map<double, double> ANA_D;
    double size = DATA->size();
    PERCENTILE = 1.0 - PERCENTILE;
    PERCENTILE /= 2.0;
    int low_border = int( PERCENTILE * size );
    int up_border = int( size ) - low_border;
    map<double, double>::iterator P = DATA->begin();
    int pos = 0;
    while( P != DATA->end() ){      
      if( (pos >= low_border) && (pos <= up_border) ){
        ANA_D.insert( make_pair( (*P).first, 0.0) );
      }
      P++;
      pos++;
    }
    
    
    simple_math MY_MATH;
    pair<double, double> tmp = MY_MATH.AVERAGE_and_STDEV( &ANA_D );
    DATA_MEAN = tmp.first;
    DATA_VAR = tmp.second*tmp.second;
    ANA_D.clear();
    
  }
  else{
    DATA_MEAN = 0.0;
    DATA_VAR = 0.0;
  }
}


/////////////////////////////////////////////////////
// set the data:
void EM_fitter::set_DATA( map<double, double>* IN){
  // check if enough data for EM procedure:
  if( IN->size() >= MIN_NB_HISTOGRAM_DATA_POINTS ){
    DATA = IN;
    MODELING_CHECK = true;
  }
  else{
    MODELING_CHECK = false;
  }
}


/////////////////////////////////////////////////////
// get the value of a point in a gauss distribution:
double EM_fitter::get_GAUSS_VALUE( double mean, double var, double point){
  GAUSS_DIST TMP;
  TMP.mean = mean;
  TMP.variance = var;
  return get_GAUSS_VALUE( &TMP, point );
}


/////////////////////////////////////////////////////
// get the value of a point in a gauss distribution:
double EM_fitter::get_GAUSS_VALUE( GAUSS_DIST* IN, double point){
  
  double OUT = 0.0;
  
  // exponent
  double EXP = ( point - IN->mean );
  EXP *= EXP;
  double DIV = ( IN->variance * 2.0);
  EXP /= DIV;
  /*
  if( EXP > 50 ){
    //return 0.0;
  }
   */
 
  // factor  
  double FACT = sqrt( 2.0 * 3.1415926 * IN->variance );
  FACT = 1.0 / FACT;
  
  OUT = FACT * exp( (-1.0) * EXP ); 
  return OUT;
}


/////////////////////////////////////////////////////
// start to fit gauss model
void EM_fitter::start_EM_1COMP_analysis(){
  start_EM_1COMP_analysis( Data_percentile);
}  


/////////////////////////////////////////////////////
// start to fit gauss model
void EM_fitter::start_EM_1COMP_analysis(double PERC){
  
  if( MODELING_CHECK ){

    // set what percentile used for fit:
    analyze_input_data( PERC );
    
    GAUSS_1.mean = DATA_MEAN;
    GAUSS_1.variance = DATA_VAR;
    PI = 0;
    GAUSS_2.mean = -1;
    GAUSS_2.variance = -1;
    
    if( PLOT ){
      //plot_DATA_histogram();
      plot_DATA_hist_with_model_1COMP();
      // plot_gauss_distributions();
      // plot_score_vs_respons();
      // plot_PI();
    }
  }
}


/////////////////////////////////////////////////////
// start to fit 2-comp-mixture model
void EM_fitter::start_EM_2COMP_analysis(){
  
  if( MODELING_CHECK ){
    
    // initialze parameters:
    INIT_2COMP();  
    
    while( (!CONVERGENCE_CHECK) && (!DATA->empty()) && (NB_IT <= MAX_IT) ){
      
      // copmute the responsibilities,
      // i.e. to which distributione
      // a point belongs to:
      compute_responsabilities();
      
      // update the distributions:
      update_GAUSS_DIST();
      
      // check the convergence:
      CONVERGENCE_CHECK = check_CONVERGENCE();
      
      NB_IT++;
    }
    
    ///////////////////////
    // post process the probabilities:
    // allways want the probability for the gauss closest to zero:
    set_profile_probabilies();
    
    if(  PLOT  ) {
      plot_DATA_hist_with_model_2COMP();
      // plot_score_vs_respons();
      // plot_PI();
    }
  }
}



/////////////////////////////////////////////////////
// copmute the responsibilities,
// i.e. to which distributione
// a point belongs to:
void EM_fitter::compute_responsabilities(){
    
  pair< double, double> POS = make_pair(0.0, 0.0);
  pair< double, double> NEG = make_pair(0.0, 0.0);
  // check if GAUSS_1 or _2 is the smaller = true distribtion one (true)
  bool POS_CHECK = true;
  double FAC_POS = 0.0;
  double FAC_NEG = 0.0;
  
   if( GAUSS_1.mean < GAUSS_2.mean ){
    POS.first = GAUSS_1.mean - FAC_POS * GAUSS_1.variance; 
    POS.second = 0.99999; 
    NEG.first = GAUSS_2.mean + FAC_NEG *GAUSS_2.variance; 
    NEG.second = 0; 
  }
  else{
    POS.first = GAUSS_2.mean - FAC_POS * GAUSS_2.variance; 
    POS.second = 0.99999; 
    NEG.first = GAUSS_1.mean + FAC_NEG * GAUSS_1.variance; 
    NEG.second = 0.0; 
    POS_CHECK = false;
  }
  
  map<double, double>::iterator p = DATA->begin();
  while( p != DATA->end() ){

    double G_val_1 = 0;
    double G_val_2 = 0;
  
    G_val_1 = (1.0 - PI) * get_GAUSS_VALUE( &GAUSS_1, (*p).first );
    G_val_2 = PI * get_GAUSS_VALUE( &GAUSS_2, (*p).first );
    
    // ****************************************
    // this condition is used to avoid bitomies, i.e.
    // that the pobability goes up again with very low scores
    // for cases, where the negative distribution has a long
    // left tailing:
    // true for case with condition:
    // 1. check if a score is smaller than median + sdev of pos. Gauss.
    // 2. if the pobabililty of the the true Gauss is zero
  
    if( (*p).first <= POS.first )  {
      if( ( POS_CHECK ) && ( G_val_1 > G_val_2 ) ){
        G_val_2 = 1.0 - POS.second;
      }
      if( ( !POS_CHECK ) && ( G_val_2 < G_val_1 ) ){
        G_val_1 = 1.0 - POS.second;
      }
    }
    
    // ****************************************
    // another condition to avoid that
    // the probability goes wierdly up for 
    // high scores:
    // 1. if gauss 2 is zero
    // 2 if score is bigger than mean + sd of gauss 2
    if( (*p).first > NEG.first )  {
      if( ( POS_CHECK ) && ( G_val_2 == 0 ) ){
        G_val_2 = 0.0;
      }
      if( ( !POS_CHECK ) && ( G_val_1 == 0 ) ){
        G_val_1 = 0.0;
      }
    }
    double response = G_val_2 / ( G_val_1 + G_val_2 );
    (*p).second = response;
    
    p++;
  }
}

///////////////////////////////////////////////////////
// update the distributions:
void EM_fitter::update_GAUSS_DIST(){

  GAUSS_1.mean = 0.0;  
  GAUSS_2.mean = 0.0;
  GAUSS_1.variance = 0.0;  
  GAUSS_2.variance = 0.0;  

  // update means:
  double RESP_TOT_1 = 0.0;
  double RESP_TOT_2 = 0.0;
  map<double, double>::iterator D = DATA->begin();
  while( D != DATA->end() ){
    
    double point = (*D).first;
    double resp = (*D).second;
    
    GAUSS_1.mean += ( 1.0 - resp) * point; 
    RESP_TOT_1 += ( 1.0 - resp);
    GAUSS_2.mean += resp * point; 
    RESP_TOT_2 += resp;
    
    D++;
  }
  GAUSS_1.mean /= RESP_TOT_1;
  GAUSS_2.mean /= RESP_TOT_2;
  
  // update variance and compute new PI:
  D = DATA->begin();
  PI = 0.0;
  while( D != DATA->end() ){
  
    double point = (*D).first;
    double resp = (*D).second;
    PI += resp;
      
    GAUSS_1.variance += ( ( 1.0 - resp) * (GAUSS_1.mean - point) *(GAUSS_1.mean - point) ); 
    GAUSS_2.variance += ( resp * (GAUSS_2.mean - point) *(GAUSS_2.mean - point) ); 
    
    D++;
  }
  
  GAUSS_1.variance /= RESP_TOT_1;
  GAUSS_2.variance /= RESP_TOT_2;
    
  
  // check convergence:
  PI /= double( DATA->size() );
  
}

///////////////////////////////////////////////////////
// check if fitting has converged:
bool EM_fitter::check_CONVERGENCE(){
  
  // update means:
  double TOT = 0.0;
  map<double, double>::iterator D = DATA->begin();
  while( D != DATA->end() ){

    double point = (*D).first;
    double resp = (*D).second;
    
    double G_val_1 = get_GAUSS_VALUE( &GAUSS_1, point );
    G_val_1 = log( G_val_1 );
    G_val_1 *= (1.0 - resp);
    
    double G_val_2 = get_GAUSS_VALUE( &GAUSS_2, point );
    G_val_2 = log(  G_val_2 );
    G_val_2 *= resp;
    
    TOT += ( G_val_2 + G_val_1 );
    D++;
  }
  
  // check convergence:
  if( !CONVERGENCE_ROUND.empty() ){
    vector<double>::reverse_iterator R = CONVERGENCE_ROUND.rbegin();
    double delta = fabs( TOT - (*R) );
    CONVERGENCE_ROUND.push_back( TOT );
    if( delta <= CONVERGENCE ){
      CONVERGENCE_CHECK = true;
      return true;
    }
    else{
      CONVERGENCE_CHECK = false;
      return false;
    }
  }
  else{
    CONVERGENCE_CHECK = false;
    CONVERGENCE_ROUND.push_back( TOT );
    return false;
  }
  
}


///////////////////////////////////////////////////////////////
// plots the input data and the model
void EM_fitter::plot_DATA_hist_with_model_1COMP(){

  
  char buffer[255];
  sprintf( buffer, "EM_model_and_data_%s", ANALYSIS_NAME.c_str());
  string tmp = buffer;
  data_plotter* PLOT = new data_plotter(tmp); 
  map<double, double> PLOT_DATA;
 
  //////////////
  // make the historgram of the input data:
  map<double,double>::iterator P = DATA->begin();
  double START = (*P).first;
  P = DATA->end();
  P--;
  double END = (*P).first;
 
  double BIN = fabs( END - START ) / NB_HISTOGRAM_BINS;
  END += BIN;
  START -= BIN;
  
  double His_AREA = 0;
  double X = START;
  while( X <= END ){
    PLOT_DATA.insert( make_pair(X, 0) );
    X += BIN;
  }
  
  P = DATA->begin();
  while( P != DATA->end() ){  
    // find the correct bin:
    map<double, double>::iterator F = PLOT_DATA.lower_bound( (*P).first );
    if( F == PLOT_DATA.end() ){
      F--;
    }
    (*F).second += 1.0;
    P++;
  }
  
  sprintf( buffer, "Input data histogram (%d)", DATA->size());
  PLOT->add_plot_data( &PLOT_DATA, buffer );
  P = PLOT_DATA.begin();
  while( P != PLOT_DATA.end() ){  
    // find the correct bin:
    His_AREA += (*P).second;
    P++;
  }
  PLOT_DATA.clear();
  
  //////////////
  // print the model:
  double M_AREA = 0;
  X = START;
  map<double, double> G_DATA;
  while( X <= (END + 2*BIN) ){
    double g = get_GAUSS_VALUE( &GAUSS_1, X );
    G_DATA.insert( make_pair( X,  g) );
    M_AREA += g;
    X += BIN;
  }
  
  double f = His_AREA / M_AREA;
  map<double, double>::iterator M = G_DATA.begin();
  while( M != G_DATA.end() ){
    (*M).second *= f;
    M++;
  }
  
  sprintf( buffer, "Model(mean=%f,sd=%f)", GAUSS_1.mean, sqrt(GAUSS_1.variance));
  PLOT->add_plot_data( &G_DATA, buffer);
  G_DATA.clear();
  
  PLOT->plot_TWOD_data();
  PLOT->print_TWOD_data_to_TXT();
  
   
  delete PLOT;
  PLOT = NULL;
  
}

///////////////////////////////////////////////////////////////
// plots the input data and the model
void EM_fitter::plot_DATA_hist_with_model_2COMP(){
  
  char buffer[255];
  sprintf( buffer, "EM_model_and_data_%s", ANALYSIS_NAME.c_str());
  string tmp = buffer;
  data_plotter* PLOT = new data_plotter(tmp);  
  
  map<double, double> PLOT_DATA;
  
  //////////////
  // make the historgram of the input data:
  map<double,double>::iterator P = DATA->begin();
  double BIN = 0.005;
  P = DATA->end();
  P--;
  double END = (*P).first;
  // check that there is no stopp of the scores before 0.5:
  if( END < 2 * GAUSS_2.mean ){
    END = 2 * GAUSS_2.mean;
  }
  
  double His_AREA = 0;
  double X = 0;
  while( X <= END ){
    PLOT_DATA.insert( make_pair(X, 0) );
    X += BIN;
  }
  
  
  
  P = DATA->begin();
  while( P != DATA->end() ){  
    // find the correct bin:
    map<double, double>::iterator F = PLOT_DATA.lower_bound( (*P).first );
    if( F == PLOT_DATA.end() ){
      F--;
    }
    (*F).second++;
    P++;
  }
  
  
  
  
  sprintf( buffer, "Input data histogram (%d)", DATA->size());
  PLOT->add_plot_histogram_data( &PLOT_DATA, buffer );
  P = PLOT_DATA.begin();
  while( P != PLOT_DATA.end() ){  
    His_AREA += (*P).second;
    P++;
  }
  PLOT_DATA.clear();
  
  //////////////
  // print the model:
  double M_AREA = 0;
  X = 0;
  map<double, double> G_DATA;
  while( X <= (END + 2.0 * BIN) ){
    double g = (1.0 - PI) * get_GAUSS_VALUE( &GAUSS_1, X ) + (PI) * get_GAUSS_VALUE( &GAUSS_2, X );
    G_DATA.insert( make_pair( X,  g) );
    M_AREA += g;
    X += BIN;
  }
  
  double f = His_AREA / M_AREA;
  map<double, double>::iterator M = G_DATA.begin();
  map<double, double> G1_DATA;
  map<double, double> G2_DATA;
  while( M != G_DATA.end() ){
    (*M).second *= f;

    double g = (1.0 - PI)*get_GAUSS_VALUE( &GAUSS_1, (*M).first );
    if( g > 0){
      G1_DATA.insert( make_pair( (*M).first,  g*f ));
    }
    g = (PI)*get_GAUSS_VALUE( &GAUSS_2, (*M).first );
    if( g > 0){
      G2_DATA.insert( make_pair( (*M).first,  g*f ));
    }
    M++;
  }
  
  sprintf( buffer, "Model 1(mean=%f,sd=%f,pi=%f)", GAUSS_1.mean, sqrt(GAUSS_1.variance), 1.0 - PI);
  PLOT->add_plot_data( &G1_DATA, buffer);
  sprintf( buffer, "Model 2(mean=%f,sd=%f,pi=%f)", GAUSS_2.mean, sqrt(GAUSS_2.variance), PI);
  PLOT->add_plot_data( &G2_DATA, buffer);
  
  G1_DATA.clear();
  G2_DATA.clear();
  G_DATA.clear();
  
  PLOT->plot_TWOD_data();
  if( TEXT_OUT ){
    // PLOT->print_TWOD_data_to_TXT();
    //PLOT->print_MATRIX_data_to_TXT();

  }
  delete PLOT;
  PLOT = NULL;
  
}

///////////////////////////////////////////////////////////////
// plot score vs response
void EM_fitter::plot_score_vs_respons(){
  
  char buffer[255];
  sprintf( buffer, "em_score_vs_respons_%s", ANALYSIS_NAME.c_str());
  string tmp = buffer;
  data_plotter* PLOT = new data_plotter(tmp);
  
  PLOT->add_plot_data( DATA );
  PLOT->plot_TWOD_data();
  
  delete PLOT;
  PLOT = NULL;
}

///////////////////////////////////////////////////////////////
// plot PI
void EM_fitter::plot_PI(){
  
  char buffer[255];
  sprintf( buffer, "em_PI_convergence_%s", ANALYSIS_NAME.c_str());
  string tmp = buffer;
  data_plotter* PLOT = new data_plotter(tmp);
  
  map<double, double> PLOT_DATA;
  vector<double>::iterator D = CONVERGENCE_ROUND.begin();
  double count = 1;
  while( D != CONVERGENCE_ROUND.end() ){
    PLOT_DATA.insert( make_pair( count, (*D) ) );
    D++;
    count++;
  }
  PLOT->add_plot_data( &PLOT_DATA );
  PLOT->plot_TWOD_data();
  
  delete PLOT;
  PLOT = NULL;
  PLOT_DATA.clear();
}



///////////////////////////////////////////////////////////////
// show an analysis summary:
void EM_fitter::show_EM_summary(){
  
  printf("\t\t\tEM 2 component mixture model analysis of %s:\n", ANALYSIS_NAME.c_str());
  if( ( get_convergence_check() ) && ( MODELING_CHECK ) ){
    printf("\t\t\tconvergence (delta = %E) at %d iterations, %d data points\n", CONVERGENCE, NB_IT, DATA->size());
    printf("\t\t\tGAUSS DISTRIBUTIONS:\n");
    printf("\t\t\t\t1. Component [%f, %f]\n", GAUSS_1.mean, sqrt(GAUSS_1.variance));
    printf("\t\t\t\t2. Component [%f, %f]\n", GAUSS_2.mean, sqrt(GAUSS_2.variance));
  }
  else if( MODELING_CHECK ){
    printf("\t\t\t\tABORTED:: convergence (delta = %E) NOT reached (max. %d iteration), %d data points\n", CONVERGENCE, MAX_IT, DATA->size());
  }
  else{
    printf("\t\t\t\tnot enough data points (min=%d) for EM modeling!\n", MIN_NB_HISTOGRAM_DATA_POINTS);
  }
  
}


///////////////////////////////////////////////////////////////
// get the probability for a score:
double EM_fitter::get_profile_probabiliy_per_score(double IN){
  if( DATA == NULL){
    return 0.0;
  }
  map<double, double>::iterator F = DATA->lower_bound( IN );
  if( F == DATA->end() ){
    F--;
    if( (*F).first > IN ){
      F = DATA->begin();
    }
  }
  return (*F).second;
}


///////////////////////////////////////////////////////////////
// set the probabilities for all the scores:
void EM_fitter::set_profile_probabilies(){

  if( GAUSS_1.mean < GAUSS_2.mean ){
    map<double, double>::iterator P = DATA->begin();
    while( P != DATA->end() ){
      (*P).second = 1.0 - (*P).second; 
      P++;
    }
  }
}


///////////////////////////////////////////////////////////////
// prints the input data and the model to a text file
void EM_fitter::print_EM_distributions(){
  
  read_param* def = new read_param();  
  // print out
  string p_name = def->search_tag("MY PROJECT NAME");
  if( p_name[ p_name.size() - 1 ] != '/' ){
    p_name += "/";
  }
  delete def;
  def = NULL;
  
  char buffer[255];
  sprintf( buffer, "ANALYSIS_%sEM_model_and_data.txt", p_name.c_str());
  string file = buffer;
  ofstream* WRITER = new ofstream();
  WRITER->open( file.c_str(), ofstream::out); 
  
  if(WRITER->good()){
    
    map< double, double> HIST;
    map< double, double> G_DATA;
    
    ///////////////////////////////////////////////
    // make the historgram of the input data:
    map<double,double>::iterator P = DATA->begin();
    double BIN = 0.005;
    P = DATA->end();
    P--;
    double END = (*P).first;
    // check that there is no stopp of the scores before 0.5:
    if( END < 2 * GAUSS_2.mean ){
      END = 2 * GAUSS_2.mean;
    }

    double His_AREA = 0;
    double X = 0;
    while( X <= END ){
      HIST.insert( make_pair( X, 0.0 ) );
      X += BIN;
    }
    P = DATA->begin();
    while( P != DATA->end() ){  
      // find the correct bin:
      map<double, double>::iterator F = HIST.lower_bound( (*P).first );
      if( F == HIST.end() ){
        F--;
      }
      (*F).second++;
      P++;
    }
    
    P = HIST.begin();
    while( P != HIST.end() ){  
      His_AREA += (*P).second;
      P++;
    }
   
    ///////////////////////////////////
    // compute the models:
    double M_AREA = 0;
    X = 0;
    while( X <= (END + 2.0 * BIN) ){
      double g = (1.0 - PI) * get_GAUSS_VALUE( &GAUSS_1, X ) + (PI) * get_GAUSS_VALUE( &GAUSS_2, X );
      G_DATA.insert( make_pair( X,  g) );
      M_AREA += g;
      X += BIN;
    }
    
    double f = His_AREA / M_AREA;
    map<double, double>::iterator M = G_DATA.begin();
    map<double, double> G1_DATA;
    map<double, double> G2_DATA;
    while( M != G_DATA.end() ){
      (*M).second *= f;
      
      double g = (1.0 - PI)*get_GAUSS_VALUE( &GAUSS_1, (*M).first );
      if( g > 0){
        G1_DATA.insert( make_pair( (*M).first,  g*f ));
      }
      g = (PI)*get_GAUSS_VALUE( &GAUSS_2, (*M).first );
      if( g > 0){
        G2_DATA.insert( make_pair( (*M).first,  g*f ));
      }
      M++;
    }
    
    
    string SEP = "\t";
    // now save all in the text file:    
    map< double, double >::iterator H = HIST.begin();
    map< double, double >::iterator F;
    while( H != HIST.end() ){
    
      // print hist data:
      sprintf(buffer,"%0.3f%s%0.3f%s", (*H).first, SEP.c_str(), (*H).second, SEP.c_str());  
      WRITER->write(buffer,strlen(buffer));
      
      // EM1 data:
      F = G1_DATA.find( (*H).first );
      if( F != G1_DATA.end() ){
        sprintf(buffer,"%0.3f%s", (*F).second, SEP.c_str());  
        WRITER->write(buffer,strlen(buffer));
      }
      else{
        sprintf(buffer,"0.0%s" , SEP.c_str());  
        WRITER->write(buffer,strlen(buffer));
      }
          
      // EM1 data:
      F = G2_DATA.find( (*H).first );
      if( F != G2_DATA.end() ){
        sprintf(buffer,"%0.3f\n", (*F).second);  
        WRITER->write(buffer,strlen(buffer));
      }
      else{
        sprintf(buffer,"0.0\n");  
        WRITER->write(buffer,strlen(buffer));
      }
    
      H++;
    }
    
    
    printf("\t\t- EM distributions were saved to '%s'\n", file.c_str());
  }
  else{
    printf("\nERROR: opening file '%s', EM-fiter::715\n", file.c_str());
  }
  
  delete WRITER;
  WRITER = NULL;
  
  
}

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


#ifndef EM_FITTER_H
#define EM_FITTER_H


typedef struct{
  
  double mean;
  double variance;
  
} 
GAUSS_DIST;

class EM_fitter{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  string ANALYSIS_NAME;
  bool TEXT_OUT;
  
  // the 2 distributions:
  GAUSS_DIST GAUSS_1;
  GAUSS_DIST GAUSS_2;
  
  // the log likelihood:
  double PI;
  vector<double> CONVERGENCE_ROUND;
  
  // data:
  map<double, double>* DATA;
  double DATA_MEAN;
  double DATA_VAR;
  double CONVERGENCE;
  
  int MAX_IT;
  int NB_IT;
  bool PLOT;
  bool MODELING_CHECK;
  bool CONVERGENCE_CHECK;
  unsigned int MIN_NB_HISTOGRAM_DATA_POINTS;
  unsigned int NB_HISTOGRAM_BINS;
  double Data_percentile;
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~EM_fitter();
  
  // class constructor
  EM_fitter();
  // class constructor
  EM_fitter(string);
  // class copy constructor
  EM_fitter(const EM_fitter&);
  // class copy constructor
  EM_fitter(const EM_fitter*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  EM_fitter& operator=(const EM_fitter&);
  bool operator==(const EM_fitter&);
  EM_fitter& operator<=(const EM_fitter&);
  EM_fitter& operator>=(const EM_fitter&);
  EM_fitter& operator<(const EM_fitter&);
  EM_fitter& operator>(const EM_fitter&);
  
  // randomized access of the data:
  double get_random_data();
  // initialze:
  void INIT_2COMP();
  // analize input data ( mean / stdev):
  void analyze_input_data(double);

  // set the data:
  void set_DATA( map<double, double>*);
  // get the value of a point in a gauss distribution:
  double get_GAUSS_VALUE( GAUSS_DIST* , double );
  double get_GAUSS_VALUE( double, double, double);

  
  //////////////////////////////////////////////////
  // start to fit 2-comp-mixture model
  void start_EM_2COMP_analysis();
  // copmute the responsibilities,
  // i.e. to which distributione
  // a point belongs to:
  void compute_responsabilities();
  // update the distributions:
  void update_GAUSS_DIST();    
  // check if fitting has converged:
  bool check_CONVERGENCE();
  // get if converegnce has been reached or not:
  bool get_convergence_check(){ return CONVERGENCE_CHECK;};
  // set the probability for a score:
  void set_profile_probabilies();

  
  //////////////////////////////////////
  // plots the input data and the model
  void plot_DATA_hist_with_model_1COMP();
  void plot_DATA_hist_with_model_2COMP();
  // plot score vs response
  void plot_score_vs_respons();
  // plot PI
  void plot_PI();
  // show an analysis summary:
  void show_EM_summary();
  // prints the input data and the model to a text file
  void print_EM_distributions();


  
  
  /////////////////////////////////////////////////////
  // start to fit gauss model
  void start_EM_1COMP_analysis( double);
  void start_EM_1COMP_analysis();

  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

  // get the probability for a score:
  double get_profile_probabiliy_per_score(double);
  
  // access teh gauss parameters:
  double get_G1_mean(){ return GAUSS_1.mean;};
  double get_G1_variance(){ return GAUSS_1.variance;};
  double get_G2_mean(){ return GAUSS_2.mean;};
  double get_G2_variance(){ return GAUSS_2.variance;};
  void set_TEXT_OUT(bool IN){ TEXT_OUT = IN; };
  void set_PLOT_OFF(){PLOT = false;};
  
  // set the histogram from outside:
  void setHistData( map<double, double>* IN ){ DATA = IN;};
};

#endif

    

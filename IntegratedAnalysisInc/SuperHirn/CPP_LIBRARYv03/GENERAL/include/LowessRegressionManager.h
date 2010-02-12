///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS LOWESS_MANAGER:
// fits a scatter plot with loowess function imported from R
// -> uses teh C function "lowess.h/c"
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#ifndef LOWESS_REGRESSION_MANAGER_H
#define LOWESS_REGRESSION_MANAGER_H

class LowessRegressionManager{


    ////////////////////////////////////////////////
    // declaration of the private members:

private:


  double* X;
  double* Y;
  double sum_residuals;
  
  // number of data points:
  int nb_data;
  
  ////////////////////////////////////////////////
  // parameters for the lowess fit:
  // for a desccription see function define_params:
  double delta;
  int nb_steps;
  double F;
  
  
  ////////////////////////////////////////////////
  // result of lowess fit function
  // YS(i) at X(i)
  double* lowess_Y;
  
  // weight given to the point (X(i)|Y(i))
  double* robust_weights;
  
  // = Y(i)-YS(i)
  double* residuals;
  
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~LowessRegressionManager();
  
  // class constructor
  LowessRegressionManager(double*,double*,int);
  LowessRegressionManager();
  
  // define all teh parameters for teh lowess fit:
  void define_params();
  
  // define the input parameter if not done by the
  // constructor
  void define_input(double*,double*,int);
  
  // clear up input parameter
  void clean_input();
    
  
  // call the C function lowess and start the fit:
  void start_fit();
  
  // print the fitted data:
  void print_lowess_fit(){
    for(int i=0;i<nb_data;i++){
      cout<<X[i]<<'\t'<<lowess_Y[i]<<endl;
    }
  };
  
  // sorts the input data according TR = X
  // switches also Y
  void sort_input_data();
  
  
  // analyzes the performed fit:
  void analyse_fit();
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

  double get_X(int i){return X[i];};
  double get_Y(int i){return Y[i];};
  
  double get_weight(int i){return robust_weights[i];};

  double get_smoothed_Y(int i){return lowess_Y[i];};
  double get_sum_residuals(){return sum_residuals;};
  
    
};

#endif


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
// CALL LOWESS(double *X, double *Y, int N,  double *F, int *NSTEPS, double *DELTA,double *YS, double *RW, double *RES)
//
//        Purpose
//
//        LOWESS computes the smooth of a scatterplot of Y  against  X
//        using  robust  locally  weighted regression.  Fitted values,
//        YS, are computed at each of the  values  of  the  horizontal
//        axis in X.
//
//        Argument description
//
//              X = Input; abscissas of the points on the
//                  scatterplot; the values in X must be ordered
//                  from smallest to largest.
//              Y = Input; ordinates of the points on the
//                  scatterplot.
//              N = Input; dimension of X,Y,YS,RW, and RES.
//              F = Input; specifies the amount of smoothing; F is
//                  the fraction of points used to compute each
//                  fitted value; as F increases the smoothed values
//                  become smoother; choosing F in the range .2 to
//                  idea which value to use, try F = .5.
//         NSTEPS = Input; the number of iterations in the robust
//                  fit; if NSTEPS = 0, the nonrobust fit is
//                  returned; setting NSTEPS equal to 2 should serve
//                  most purposes.
//          DELTA = input; nonnegative parameter which may be used
//                  to save computations; if N is less than 100, set
//                  DELTA equal to 0.0; if N is greater than 100 you
//                  should find out how DELTA works by reading the
//                  additional instructions section.
//             YS = Output; fitted values; YS(I) is the fitted value
//                  at X(I); to summarize the scatterplot, YS(I)
//                  should be plotted against X(I).
//             RW = Output; robustness weights; RW(I) is the weight
//                  given to the point (X(I),Y(I)); if NSTEPS = 0,
//                  RW is not used.
//            RES = Output; residuals; RES(I) = Y(I)-YS(I).
//


#define USE_LOWESS_REGRESSION_MANAGER
#include "GENERAL_USE.h"


////////////////////////////////////////////////
// constructor for the
LowessRegressionManager::LowessRegressionManager(double* X_in,double* Y_in, int Nb_data_in){

    // number of data points:
    nb_data = Nb_data_in;

    X = X_in;
    Y = Y_in;

    robust_weights = NULL;
    lowess_Y = NULL;
    residuals = NULL;
    
    sort_input_data();

    sum_residuals = 0;

    define_params();
 }


////////////////////////////////////////////////
// constructor for the
LowessRegressionManager::LowessRegressionManager(){

    // number of data points:
    nb_data = 0;

    X = NULL;
    Y = NULL;
    
    robust_weights = NULL;
    lowess_Y = NULL;
    residuals = NULL;
    
    sum_residuals = 0;

}


//////////////////////////////////////////////////
// class desctructor
LowessRegressionManager::~LowessRegressionManager(){

    X = NULL;
    Y = NULL;

    if(lowess_Y != NULL){
        delete lowess_Y;
        lowess_Y = NULL;
    }

    if(residuals != NULL){
        delete residuals;
        residuals = NULL;
    }

    if(robust_weights != NULL){
        delete robust_weights;
        robust_weights = NULL;
    }

}

//////////////////////////////////////////////////
// cleans input parameters
void LowessRegressionManager::clean_input(){

    X = NULL;
    Y = NULL;

    if(lowess_Y != NULL){
        delete lowess_Y;
        lowess_Y = NULL;
    }
    if(robust_weights != NULL){
        delete robust_weights;
        robust_weights = NULL;
    }
    if(residuals != NULL){
        delete residuals;
        residuals = NULL;
    }
    

}


////////////////////////////////////////////////
// define the input parameter if not done by the
// constructor
void LowessRegressionManager::define_input(double* X_in,double* Y_in, int Nb_data_in){

    // number of data points:
    nb_data = Nb_data_in;

    X = X_in;
    Y = Y_in;

    // sort_input_data();

    sum_residuals = 0;

    define_params();
}



//////////////////////////////////////////////////
// sorts the input data according TR = X
// switches also Y
void LowessRegressionManager::sort_input_data(){
  
  bool v;  
  do{
    v=false;
    for(int k=0;k<(nb_data-1);k++){
      
      if(X[k]>X[k+1]){
        
        ///////////////////////////
        // switch here the values
        // for X:
        double a = X[k];
        double b = X[k+1];
        X[k+1] = a;
        X[k] = b;
        
        // for Y:
        a = Y[k];
        b = Y[k+1];
        Y[k+1] = a;
        Y[k] = b;
        ///////////////////////////
        
        v=true;
      }
    }
    
  }
  while(v);
}


//////////////////////////////////////////////////
// define all teh parameters for teh lowess fit:
void LowessRegressionManager::define_params(){

  // NSTEPS = Input; the number of iterations in the robust
  nb_steps = 2;
  
  // DELTA
  //        DELTA can be used to save computations.   Very  roughly  the
  //        algorithm  is  this:   on the initial fit and on each of the
  //        NSTEPS iterations locally weighted regression fitted  values
  //        are computed at points in X which are spaced, roughly, DELTA
  //        apart; then the fitted values at the  remaining  points  are
  //        computed  using  linear  interpolation.   The  first locally
  //        weighted regression (l.w.r.) computation is carried  out  at
  //        X(1)  and  the  last  is  carried  out at X(N).  Suppose the
  //        l.w.r. computation is carried out at  X(I).   If  X(I+1)  is
  //        greater  than  or  equal  to  X(I)+DELTA,  the  next  l.w.r.
  //        computation is carried out at X(I+1).   If  X(I+1)  is  less
  //        than X(I)+DELTA, the next l.w.r.  computation is carried out
  //        at the largest X(J) which is greater than or equal  to  X(I)
  //        but  is not greater than X(I)+DELTA.  Then the fitted values
  //        for X(K) between X(I)  and  X(J),  if  there  are  any,  are
  //        computed  by  linear  interpolation  of the fitted values at
  //        X(I) and X(J).  If N is less than 100 then DELTA can be set
  //        to  0.0  since  the  computation time will not be too great.
  //        For larger N it is typically not necessary to carry out  the
  //        l.w.r.  computation for all points, so that much computation
  //        time can be saved by taking DELTA to be  greater  than  0.0.
  //        If  DELTA =  Range  (X)/k  then,  if  the  values  in X were
  //        uniformly  scattered  over  the  range,  the   full   l.w.r.
  //        computation  would be carried out at approximately k points.
  //        Taking k to be 50 often works well.
    int k = 50;
  
    if( nb_data < 100 ){
        delta = 0.0;
    }
    else{
        delta = (X[nb_data-1] - X[0]) / k;
    }

  //    delta = 0.1;
  //              F = Input; specifies the amount of smoothing; F is
  //                  the fraction of points used to compute each
  //                  fitted value; as F increases the smoothed values
  //                  become smoother; choosing F in the range .2 to
  //                  idea which value to use, try F = .5.
  F = 0.1;
  

  // now allocate memory to the output arrays:
  lowess_Y = new double[nb_data];
  robust_weights = new double[nb_data];
  residuals = new double[nb_data];
  

}



//////////////////////////////////////////////////
// call the C function lowess and start the fit:
void LowessRegressionManager::start_fit(){
  lowess(X,Y,&nb_data,&F,&nb_steps,&delta,lowess_Y,robust_weights,residuals);
  // analyse_fit();
}


//////////////////////////////////////////////////////
// analyzes the performed fit:
void LowessRegressionManager::analyse_fit(){

  // sum the residuals:
  for(int i=0;i<nb_data;i++){
    sum_residuals += fabs(residuals[i]);
  }
}

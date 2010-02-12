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


#ifndef SIMPLE_MATH_H
#define SIMPLE_MATH_H



class simple_math{
  
    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  bool HIGH_CHECK;
  bool LOW_CHECK;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  static const double T_TEST_001[12];
  static const double T_TEST_002[12];
  static const double T_TEST_01[12];
  static const double T_TEST_02[12];
  static const double T_TEST_05[12];
  static string ALPHA_VALUE;


  
  // class destructor
  ~simple_math();
  
  // class constructor
  simple_math();
  // class copy constructor
  simple_math(const simple_math&);
  // class copy constructor
  simple_math(const simple_math*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  simple_math& operator=(const simple_math&);
  bool operator==(const simple_math&);
  simple_math& operator<=(const simple_math&);
  simple_math& operator>=(const simple_math&);
  simple_math& operator<(const simple_math&);
  simple_math& operator>(const simple_math&);
  
  
  
  //////////////////////////////////////////////////
  // simple functions:
  // copmute sum over a vector:
  double SUM(vector<double>* IN);

  // compute the median:
  double MEDIAN(vector<double>* );
  double MEDIAN(map<int, double>*);
  double MEDIAN(vector< pair<double, double> >*);
  double MEDIAN(map< double, double >*);
  // compute the weighted median:
  double WEIGHTED_MEDIAN(map< double, double >* );

  
  
  // compute the average only:
  double AVERAGE(vector<double>* );
  double AVERAGE(map<int, double>* IN);

  
  // compute the average and std error:
  pair<double, double> AVERAGE_and_STDERROR(vector<double>* );
  pair<double, double> WEIGHTED_AVERAGE_and_STDERROR(vector< pair<double,double> >* );
  // compute the weighted average:
  double WEIGHTED_AVERAGE(map< double, double >* );
  
  // compute the average and stdev:
  pair<double, double> AVERAGE_and_STDEV( vector<double>* );
  pair<double, double> AVERAGE_and_STDEV( vector< pair<double,double> >* );
  pair<double, double> WEIGHTED_AVERAGE_and_STDEV( vector< pair<double,double> >* );
  pair<double, double> AVERAGE_and_STDEV( map< double, void* >*);
  pair<double, double> AVERAGE_and_STDEV( map< double, double >*);
  pair<double, double> AVERAGE_and_STDEV( vector< pair< double, void* > >*);
  pair<double, double> AVERAGE_and_STDEV(map<int,double>*);


  // get a sub vector from this one from the percentile on
  vector<double> PercentilSubVector(vector<double>* , double );

  
  //////////////////////////////////////////////////
  // detection of outliers by the DIXON's test (Gibbons, 1994):
  void OUTLIER_DETECTION_BY_DIXON(vector<double>*);
  void OUTLIER_DETECTION_BY_DIXON(vector< pair<double, double> >* );
  void OUTLIER_DETECTION_BY_DIXON(vector< pair<double, void*> >* );
  
  // iterative detection of outliers by the DIXON's test (Gibbons, 1994):
  void ITERATIVE_OUTLIER_DETECTION_BY_DIXON(vector<double>*);
  void ITERATIVE_OUTLIER_DETECTION_BY_DIXON(vector< pair<double,double> >*);
  void ITERATIVE_OUTLIER_DETECTION_BY_DIXON(vector< pair<double, void*> >*);

  
  // tranform intensty of a feature:
  double log_tranform_F_int(double);

  // this structure provides the function to compare
  // in the sorting algorithm:
  struct VECTOR_OPERATOR{
    // provide the compare function for sort:
    bool operator()(const pair<double,  void*> A,const pair<double,  void*> B) const{
      // check if they have same mass
      if(A.first == B.first){
        return false;
      }
      else{
        return A.first > B.first;
      }
    }
  };
  
  

  // check T-TEST:
  bool check_T_TEST( double, int );


  // round at certain comma position
  static double ROUND_NUMBER(double , int );

  // compare to masses at the PPM value and decided
  // if they fall into the m/z tolerance window
  static bool compareMassValuesAtPPMLevel( double, double , double );
  // get the masse error at the PPM value     
  static double getMassErrorAtPPMLevel( double , double );

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

  void set_HIGH_CHECK( bool IN){ HIGH_CHECK = IN;};
  void set_LOW_CHECK( bool IN){ LOW_CHECK = IN;};
};

#endif

    

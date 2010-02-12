///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS param_initializer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef USE_PARAM_INITIALIZER_H
#define USE_PARAM_INITIALIZER_H



class param_initializer{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~param_initializer();
  
  // class constructor
  param_initializer();
  // class copy constructor
  param_initializer(const param_initializer&);
  // class copy constructor
  param_initializer(const param_initializer*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  param_initializer& operator=(const param_initializer&);
  bool operator==(const param_initializer&);
  param_initializer& operator<=(const param_initializer&);
  param_initializer& operator>=(const param_initializer&);
  param_initializer& operator<(const param_initializer&);
  param_initializer& operator>(const param_initializer&);
  
  // initialze all object static parameters:
  static void INIT_ALL();
  // checks if root parameter file is available:
  static bool check_ROOT_PARAM( );
  // set the path to the parameter file:
  static void DEFINE_COSTUM_PARAM(string);

  
  /////////////////////////////////////////////////
  // ms2 fragment tracing:
  static void MS2_FRAGMENT_TRACE_INITIALIZATION();  
  
  //////////////////////////////////////////////////
  // initialze parameters for cpp library environment:
  static void CPP_LIBRARY_ENVIRONMENT_PARAMETERS();
  
  //////////////////////////////////////////////////
  // initialze parameters peak detection classes;
  static void PEAK_DETECTION_CLASS_INITIALIZATION();    
    
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

};

#endif

    


#ifndef USE_PARAM_INITIALIZER_H
#define USE_PARAM_INITIALIZER_H


/**
 *  @brief Class to initialize parameters of the SuperHirn program. Uses read_param class to extract by string a parameter
 *  from the param.def or ROOT_PARAM.def file.
 *  @author Lukas N. Mueller (Lukas.Mueller@imsb.biol.ethz.ch), Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
 *
 */

class param_initializer{

    
private:
  
public:
  
  ~param_initializer();
  param_initializer();
  param_initializer(const param_initializer&);
  param_initializer(const param_initializer*);
  param_initializer& operator=(const param_initializer&);
  bool operator==(const param_initializer&);
  param_initializer& operator<=(const param_initializer&);
  param_initializer& operator>=(const param_initializer&);
  param_initializer& operator<(const param_initializer&);
  param_initializer& operator>(const param_initializer&);
  
  
  /**
   * @brief initialze all object static parameters:
   */
  static void INIT_ALL();
  
  /**
   * @brief  checks if root parameter file is available:
   */
  static bool check_ROOT_PARAM( );
  
  /**
   * @brief set the path to the parameter file:
   */
  static void DEFINE_COSTUM_PARAM(string);

  
  /**
   * @brief ms2 fragment tracing
   */
  static void MS2_FRAGMENT_TRACE_INITIALIZATION();  
  
  /**
   * @brief initialze parameters for cpp library environment
   */
  static void CPP_LIBRARY_ENVIRONMENT_PARAMETERS();
  
  /**
   * @brief Initialization of peak extraction parameter . 
   */
  static void PEAK_DETECTION_CLASS_INITIALIZATION();    
  
  /**
   * @brief Set global parameters that were passed to the program from the consol. 
   * These parameters replace existing parameters from the param.def / ROOT_PARAM.def.
   * @param string, contains a list of parameters and corresponding values.
   */
  static void EXTRACT_ARGUMENT_PARAMETERS(string);    
  
};

#endif

    

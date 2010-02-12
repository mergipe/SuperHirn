///////////////////////////////////////////////////////////////////////////
//
//  PEAK DETECTION OF FOURIER TRANSFORME MS INSTRUMENT DATA
//
//  written by Markus Mueller, markus.mueller@imsb.biol.ethz.ch
//  and Lukas Mueller, Lukas.Mueller@imsb.biol.ethz.ch
//  October 2005
//
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//


#ifndef FT_PEAK_DETEC_INITIALIZER_H
#define FT_PEAK_DETEC_INITIALIZER_H

class FT_PEAK_DETECT_initializer{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~FT_PEAK_DETECT_initializer();
  
  // class constructor
  FT_PEAK_DETECT_initializer();
  // class copy constructor
  FT_PEAK_DETECT_initializer(const FT_PEAK_DETECT_initializer&);
  
  // initialize all objects!
  static void init_all();
  
  
  //////////////////////////////////////////////////
  // overload operators:
  FT_PEAK_DETECT_initializer& operator=(const FT_PEAK_DETECT_initializer&);
  FT_PEAK_DETECT_initializer& operator<=(const FT_PEAK_DETECT_initializer&);
  FT_PEAK_DETECT_initializer& operator>=(const FT_PEAK_DETECT_initializer&);
  FT_PEAK_DETECT_initializer& operator<(const FT_PEAK_DETECT_initializer&);
  FT_PEAK_DETECT_initializer& operator>(const FT_PEAK_DETECT_initializer&);
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
};

#endif

    

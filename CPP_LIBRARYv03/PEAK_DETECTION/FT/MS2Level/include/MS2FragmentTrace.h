///////////////////////////////////////////////////////////////////////////
//
//  PEAK DETECTION OF FOURIER TRANSFORME MS INSTRUMENT DATA
//
//  written by Markus Mueller, markus.mueller@imsb.biol.ethz.ch
//  ( and Lukas Mueller, Lukas.Mueller@imsb.biol.ethz.ch)
//  October 2005
//
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//


#ifndef USE_MS2_FRAGMENT_TRACE_H
#define USE_MS2_FRAGMENT_TRACE_H



class MS2FragmentTrace : public LC_elution_peak{
  
  using LC_elution_peak::operator=;


    
    ////////////////////////////////////////////////
    // declaration of the private members:

private:

  ////////////////////////////////////////////////
  // declaration of the public members:
  
  double averagePrecursorMZ;
  int averagePrecursorCHRG;
  
public:
  
  static double APEX_PERCENTILE;
  
    // class destructor
    ~MS2FragmentTrace();

    // class constructor
    MS2FragmentTrace();
    MS2FragmentTrace(MZ_series_ITERATOR , double);
    // class copy constructor
    MS2FragmentTrace(const MS2FragmentTrace&);
    // class copy constructor
    MS2FragmentTrace(const MS2FragmentTrace*);
    
    
    ////////////////////////////////////
    // analysze the fragment trace
    void analyzeMS2FragmentTrace(){
      
      if( get_nb_ms_peaks() > 1 ){
        
        // determine the intensity background baseline based on the apex percentile:
        setApexIntensityThreshold();
        
        // Compute a variety of parameters for the LC elution peak
        computeLCElutionPeakParameters( );
        setStartEndElution();
      }
      else{
        defineLCElutionPeakParametersFromMSPeak();
      }
      
      computeAveragePrecursorValues();
    }
    
  // determine start / end of the elution peak
  void setStartEndElution();
  // compute values of the precuror:
  void computeAveragePrecursorValues();
  // show info of the MS2 fragment trace:
  void show_info();

  // determine the intensity background baseline based on Apex 
  // value:
  void setApexIntensityThreshold();
    

    //////////////////////////////////////////////////
    // overload operators:
    MS2FragmentTrace& operator=(const MS2FragmentTrace&);
    bool operator==(const MS2FragmentTrace&);
    MS2FragmentTrace& operator<=(const MS2FragmentTrace&);
    MS2FragmentTrace& operator>=(const MS2FragmentTrace&);
    MS2FragmentTrace& operator<(const MS2FragmentTrace&);
    MS2FragmentTrace& operator>(const MS2FragmentTrace&);
    

    ///////////////////////////////
    // start here all the get / set
    // function to access the
    // variables of the class
    
    
    // get hte averaged precurso mass:
    double getPrecursorMZ(){ return averagePrecursorMZ;};
    int getPrecursorCHRG(){ return averagePrecursorCHRG;};
};

#endif

    

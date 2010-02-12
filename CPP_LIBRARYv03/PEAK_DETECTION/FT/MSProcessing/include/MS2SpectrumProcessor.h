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


#ifndef MS2SpectrumProcessor_H
#define MS2SpectrumProcessor_H



class MS2SpectrumProcessor{

    
    ////////////////////////////////////////////////
    // declaration of the private members:

private:

    ////////////////////////////////////////////////
    // declaration of the public members:
  vector<ms_peak> ms2PeakList;
  map<double, ms_peak> ms2PeakListMap;
  
  
  double averagePrecursorMZ;
  double averagePrecursorCHRG;
  
  /// 
  // parameters for teh processing of ms2 peaks
  double maxIntensity;
  double fragmentIntensThreshold;
  
public:

    
    static double apexPercentile;
  static int highestOrder;
    
  // class destructor
  ~MS2SpectrumProcessor();
  
  // class constructor
  MS2SpectrumProcessor();
  // class copy constructor
  MS2SpectrumProcessor(const MS2SpectrumProcessor&);
  // class copy constructor
  MS2SpectrumProcessor(const MS2SpectrumProcessor*);
  

  //////////////////////////////////////////////////
  // overload operators:
  MS2SpectrumProcessor& operator=(const MS2SpectrumProcessor&);
  bool operator==(const MS2SpectrumProcessor&);
  MS2SpectrumProcessor& operator<=(const MS2SpectrumProcessor&);
  MS2SpectrumProcessor& operator>=(const MS2SpectrumProcessor&);
  MS2SpectrumProcessor& operator<(const MS2SpectrumProcessor&);
  MS2SpectrumProcessor& operator>(const MS2SpectrumProcessor&);
  
  // add ms2 peaks:
  void addMS2Peak( ms_peak* );
  // get a pointer to the peak list:
  vector<ms_peak>* getMS2PeakList(){return &ms2PeakList; };
  
  //////////////////////////////////////////////////
  // process a ms2 spectrum:
  void processMS2Spectrum( vector<ms_peak>* );
  void processMS2Spectrum( );

  //////////////////////////////////////////////////
  // full processing of a ms2 spectrum:
  // - remove H20 loss peak
  // - apex percentile thresholding
  void processMS2Spectrum( MS2_feature*);  
    
  // remove H2O loss region of the MS2 spectra
  void removeWaterLossRegion( feature*);
  void removeWaterLossRegion( MS2_feature* );
  void removeWaterLossRegion( double, int );



  // compute the apex intensity threshold for a ms2 spectrum:
  void computeMS2SpectrumApexThreshold( MS2_feature* );
  void computeMS2SpectrumApexThreshold( MS2ConsensusSpectrum*);

  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  double getFragmentIntensThreshold(){return fragmentIntensThreshold;};
  
  
};

#endif

    

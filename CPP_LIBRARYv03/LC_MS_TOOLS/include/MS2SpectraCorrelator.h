///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS MS2SpectraCorrelator:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef MS2_SPECTRUM_CORRELATOR_H
#define MS2_SPECTRUM_CORRELATOR_H

struct MS2FragmentPair{
  
  double targetMZ;
  double checkerMZ;
  
  double targetIntens;
  double checkerIntens;
  
  double targetRank;
  double checkerRank;

};


struct MS2CorrelationAnalysis{
  
  double overlapCorrelationScore;
  double intensityCorrelationScore;
  double mainCorrelationScore;
  
  double peakOverlap;
  double nbTargetPeaks;
  double nbCheckerPeaks;
  
};



class MS2SpectraCorrelator{
  
  
  

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
  // MS2ConsensusSpectrum* target;
  multimap< double, MS2Fragment* > targetFragments;
  
  vector<double> targetIntensityMap;
  vector<double> checkerIntensityMap;
  
  // MS2ConsensusSpectrum* checker;
  multimap< double,  MS2Fragment* > checkerFragments;
  
  vector<MS2FragmentPair> extractedMS2FragmentPairs;
  
  MS2CorrelationAnalysis mainAnalysis;
  
  /*
  double overlapCorrelationScore;
  double intensityCorrelationScore;
  double mainCorrelationScore;
*/
  
  double meanIntensTarget;
  double meanIntensChecker;
    
public:
    
    static bool DEBUG_MS2_CORRELATOR;
  static double MIN_NB_OVERLAP;
  
  // class destructor
  ~MS2SpectraCorrelator();
  // constructor for the object MS2SpectraCorrelator:
  MS2SpectraCorrelator(MS2_feature* );

  
  // class constructor
  MS2SpectraCorrelator();
  // class copy constructor
  MS2SpectraCorrelator(const MS2SpectraCorrelator&);
  // class copy constructor
  MS2SpectraCorrelator(const MS2SpectraCorrelator*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  MS2SpectraCorrelator& operator=(const MS2SpectraCorrelator&);
  bool operator==(const MS2SpectraCorrelator&);
  MS2SpectraCorrelator& operator<=(const MS2SpectraCorrelator&);
  MS2SpectraCorrelator& operator>=(const MS2SpectraCorrelator&);
  MS2SpectraCorrelator& operator<(const MS2SpectraCorrelator&);
  MS2SpectraCorrelator& operator>(const MS2SpectraCorrelator&);
  
  // set teh input checker spectra
  void setMS2CheckSpectra(MS2ConsensusSpectrum* );
  // set teh target spectrum
  void setTargetSpectra(MS2_feature* );

  // get the intensity rank in an intensity map
  int getIntensityRank(vector<double>* , double );

  // add pair to spectra correlation:
  void addSpectralCorrelationFragment(MS2FragmentPair);
  
  // find a corresponding MS2 fragment in the target spectrum
  MS2Fragment* findMS2FragmentInTarget( MS2Fragment* );
  // compare 2 fragments to decided if they should be matched
  bool compareM2Fragments(MS2Fragment* , MS2Fragment* );
  // find a corresponding MS2 fragment in a searchMap
  MS2Fragment* findMS2FragmentInMap( MS2Fragment* , multimap<double,MS2Fragment*>*);


    
  //////////////////////////////////////////////////////////////////
  // comparison of an input spectrum to a set target spectrum
  void computeMS2SpectrumCorrelation(MS2_feature*);
  // comparison of an input spectrum to a set target spectrum
  void computeMS2SpectrumCorrelation();
  // extract fragment pairs from the scans:
  void extractMS2FragmentPairs();

  ///////////////////////////////////////////////////////////////////////////////////
  // compute spearman correlation
  // r = A / B, where
  // A = sum[ ( Ri - Rmean)*(Si - Smean)]
  // B = sqrt[sum( Ri - Rmean)^2] * sqrt[sum( Si - Smean)^2]
  // Ri = rank of peak i in spectra A
  // Si = rank of peak i in spectra B
  // Rmean = rank of the mean
  // Smean = rank of the mean
  //
  void computeSpearmanCorrelationIntensityScore();
  // compute kendall tau correlation
  //
  void computeKendallCorrelationIntensityScore();
    
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  MS2CorrelationAnalysis* getMainAnalysis( ){ return &mainAnalysis;};


  double getMainCorrelation(){return mainAnalysis.mainCorrelationScore;};
  /*
  double getOverlapCorrelation(){return mainAnalysis.overlapCorrelationScore;};
  double getIntensityCorrelation(){return mainAnalysis.intensityCorrelationScore;};
  double getPeakOverlap(){return mainAnalysis.peakOverlap;};
  */
  
};

#endif



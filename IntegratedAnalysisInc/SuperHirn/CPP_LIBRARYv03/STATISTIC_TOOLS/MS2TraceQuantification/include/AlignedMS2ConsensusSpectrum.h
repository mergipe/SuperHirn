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


#ifndef _ALIGNED_MS2_CONSENSUS_SPECTRUM_H
#define _ALIGNED_MS2_CONSENSUS_SPECTRUM_H




typedef map<int, MS2Fragment* > profileFragmentTrace;


class AlignedMS2ConsensusSpectrum : public MS2ConsensusSpectrum{

    
    ////////////////////////////////////////////////
    // declaration of the private members:

private:

    ////////////////////////////////////////////////
    // declaration of the public members:
  
  // stores the aligned MS2 fragments:
  map<double,  profileFragmentTrace> AlignedMS2FragmentPeaks;
  
  // stores the individual consens spectra:
  map<int, MS2ConsensusSpectrum* > ConsensSpectra;
  
public:
    
    
    // minmal detection occurence in order to accept as aligned fragment:
    static unsigned int MINMINAL_ALIGNED_FRAGMENT_DETECTION_TIME;
  
  // class destructor
  ~AlignedMS2ConsensusSpectrum();
  
  // class constructor
  AlignedMS2ConsensusSpectrum();
  AlignedMS2ConsensusSpectrum( int, MS2ConsensusSpectrum* );
  
  // class copy constructor
  AlignedMS2ConsensusSpectrum(const AlignedMS2ConsensusSpectrum&);
  // class copy constructor
  AlignedMS2ConsensusSpectrum(const AlignedMS2ConsensusSpectrum*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  AlignedMS2ConsensusSpectrum& operator=(const AlignedMS2ConsensusSpectrum&);
  bool operator==(const AlignedMS2ConsensusSpectrum&);
  AlignedMS2ConsensusSpectrum& operator<=(const AlignedMS2ConsensusSpectrum&);
  AlignedMS2ConsensusSpectrum& operator>=(const AlignedMS2ConsensusSpectrum&);
  AlignedMS2ConsensusSpectrum& operator<(const AlignedMS2ConsensusSpectrum&);
  AlignedMS2ConsensusSpectrum& operator>(const AlignedMS2ConsensusSpectrum&);
  
  
  
  
  
  
  
  //////////////////////////////////////////////////
  // trace the fragments across LC-MS runs:
  void constructAlignedConsenusSpectraFragments();
  // extracts fragments from a consens and inserts
  // them into the trace:
  void extractFragmentsFromConsensSpectra( int , MS2ConsensusSpectrum* );
  // process extracted aligned fragment traces:
  void processAlignedFragmentTraces();
  
  
  
  // add a MS2 Consensus Spectrum:
  void addMS2ConsensusSpectrum( int, MS2ConsensusSpectrum* );
    
  // plot all the consensus MS2 spectrum in one plot:
  void plotCombinedSpectra( );

  
  // remove outlier fragments based on their:
  // MS2Fragment::OutlierAttribute = ...
  // 1: retention time
  // 2: precursor mass
  // etc.
  void removeOutlierFragments();
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
 
  
  map<int, MS2ConsensusSpectrum* >::iterator getConsensSpectraStart(){return ConsensSpectra.begin();};
  map<int, MS2ConsensusSpectrum* >::iterator getConsensSpectraEnd(){return ConsensSpectra.end();};
  
  
  map<double,  profileFragmentTrace>::iterator getAlignedMS2FragmentPeakStart(){return AlignedMS2FragmentPeaks.begin();};
  map<double,  profileFragmentTrace>::iterator getAlignedMS2FragmentPeakEnd(){return AlignedMS2FragmentPeaks.end();};
   
};

#endif

    

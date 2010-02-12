///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS MS2quantificationController:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef _MS2_QUANTIFICATION_CONTROLLER_H
#define _MS2_QUANTIFICATION_CONTROLLER_H



class MS2quantificationController{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  // master map:
  LC_MS* MasterMap;
  
  bool gnuplot;

  // stores teh aligned consensus spectra
  vector< pair<AlignedMS2ConsensusSpectrum , feature*> > alignedConsensMS2;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
  
public:
  
  // class destructor
  ~MS2quantificationController();
  
  // class constructor
  MS2quantificationController( LC_MS*);
  // class copy constructor
  MS2quantificationController(const MS2quantificationController&);
  // class copy constructor
  MS2quantificationController(const MS2quantificationController*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  MS2quantificationController& operator=(const MS2quantificationController&);
  bool operator==(const MS2quantificationController&);
  MS2quantificationController& operator<=(const MS2quantificationController&);
  MS2quantificationController& operator>=(const MS2quantificationController&);
  MS2quantificationController& operator<(const MS2quantificationController&);
  MS2quantificationController& operator>(const MS2quantificationController&);
  
  
  //////////////////////////////////////////////////
  // start the quantification on the MS2 tarce level:
  void START_MS2_TRACE_QUANTIFICATION();
  // processes an aligned MS2 feature:
  void processAlignedFeature( feature*);
  // process fragment profiles:
  void processLCMSMS2FragmentTraces();

  
  
  //////////////////////////////////////////////////
  // constructs a MS2 fragment trace across LC-MS runs
  // -> for each fragment trace it across aligned features
  // and extract a intensity profile:
  void constructLCMSMS2FragmentTraces();
  // store the aligned consensus spectrum together with its feature
  void storeAlignedMS2ConsensSpectrum( feature* , AlignedMS2ConsensusSpectrum*  );

  
  // writes the header:
  void printMS2TraceHeader( ofstream* );
  // write out to a file:
  void writeFragmentTracetextFile( );
  // writes out to a file:
  void print_2_file(ofstream*, AlignedMS2ConsensusSpectrum* );

  // plot the MS2 spectrum of the aligned fetaure
  void plotMS2spectra( AlignedMS2ConsensusSpectrum*  );

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

};

#endif

    

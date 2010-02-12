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
// **********************************************************************//
// CLASS ms_peak:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_MS_PEAK
#include "MS_PROCESSING_USE.h"


////////////////////////////////////////////////
// constructor for the object ms_peak:
ms_peak::ms_peak(){
  MZ = 0;
  INTENSITY = 0;
  SCAN = 0;
  TR = 0;
  CHRG = 0;
  NRISOTOPES = 0;
  SCORE = 0; 
  precursorMZ = 0;
  SignalToNoise = 1;
  precursorMass = false;
  childScan = -1;

}


////////////////////////////////////////////////
// constructor for the object ms_peak:
ms_peak::ms_peak(int IN_scan, double IN_mass, float IN_intens){
  MZ = IN_mass;
  INTENSITY = IN_intens;
  SCAN = IN_scan;
  TR = 0;
  NRISOTOPES = 0;
  SCORE = 0; 
  CHRG = 0;
  precursorMZ = 0;
  SignalToNoise = 1;
  precursorMass = false;
  childScan = -1;

}

////////////////////////////////////////////////
// constructor for the object ms_peak:
ms_peak::ms_peak(int IN_scan, double IN_mass, float IN_intens, unsigned int IN_CHRG, unsigned int IN_NRISOTOPES, float IN_SCORE,vector<CentroidPeak> IN_ISOPEAKS){
  MZ = IN_mass;
  INTENSITY = IN_intens;
  SCAN = IN_scan;
  TR = 0;
  precursorMZ = 0;
  SignalToNoise = 1;
  
  CHRG = IN_CHRG;
  NRISOTOPES = IN_NRISOTOPES;
  SCORE = IN_SCORE;
  ISOPEAKS = IN_ISOPEAKS;
  precursorMass = false;
  childScan = -1;

}


//////////////////////////////////////////////////
// class desctructor of ms_peak
ms_peak::~ms_peak(){
  MZ = 0;
  INTENSITY = 0;
  SCAN = 0;
  TR = 0;
  CHRG = 0;
  NRISOTOPES = 0;
  SCORE = 0; 
  precursorMZ = 0;
  SignalToNoise = 0;
  precursorMass = false;
  childScan = -1;


}

//////////////////////////////////////////////////
// copy constructor:
ms_peak::ms_peak(const ms_peak& tmp){
  MZ = tmp.MZ;
  SignalToNoise = tmp.SignalToNoise;
  INTENSITY = tmp.INTENSITY;
  SCAN = tmp.SCAN;
  TR = tmp.TR;
  CHRG = tmp.CHRG;
  SCORE = tmp.SCORE;
  NRISOTOPES = tmp.NRISOTOPES;
  ISOPEAKS = tmp.ISOPEAKS;
  precursorMZ = tmp.precursorMZ;
  precursorMass = tmp.precursorMass;
  childScan = tmp.childScan;
  extraMSPeakInfo = tmp.extraMSPeakInfo;
  childScanRT = tmp.childScanRT;


}

//////////////////////////////////////////////////
// copy constructor:
ms_peak::ms_peak(const ms_peak* tmp){
  MZ = tmp->MZ;
  SignalToNoise = tmp->SignalToNoise;
  INTENSITY = tmp->INTENSITY;
  SCAN = tmp->SCAN;
  TR = tmp->TR;
  CHRG = tmp->CHRG;
  SCORE = tmp->SCORE;
  NRISOTOPES = tmp->NRISOTOPES;
  ISOPEAKS = tmp->ISOPEAKS;
  precursorMZ = tmp->precursorMZ;
  precursorMass = tmp->precursorMass;
  childScan = tmp->childScan;
  extraMSPeakInfo = tmp->extraMSPeakInfo;
  childScanRT = tmp->childScanRT;


}


//////////////////////////////////////////////////
// copy constructor:
ms_peak& ms_peak::operator=(const ms_peak& tmp){
  MZ = tmp.MZ;
  SignalToNoise = tmp.SignalToNoise;
  INTENSITY = tmp.INTENSITY;
  SCAN = tmp.SCAN;
  TR = tmp.TR;
  CHRG = tmp.CHRG;
  SCORE = tmp.SCORE;
  NRISOTOPES = tmp.NRISOTOPES;
  ISOPEAKS = tmp.ISOPEAKS;
  precursorMZ = tmp.precursorMZ;
  precursorMass = tmp.precursorMass;
  childScan = tmp.childScan;
  extraMSPeakInfo = tmp.extraMSPeakInfo;
  childScanRT = tmp.childScanRT;

  return *this;
}


//////////////////////////////////////////////////
// show info:
void ms_peak::show_info(){
  printf( "mz=%0.4f,int=%0.1f,scan=%d,tr=%0.2f,+%d,bg=%0.1f", MZ, INTENSITY, SCAN, TR, CHRG, SignalToNoise );
  
  if( precursorMZ > 1.0){
    printf( ", preMZ=%0.4f\n", precursorMZ );
  }
  
  if( getPrecursorActivation( ) ){
    printf( ",MS2:%0.2f/%d\n", childScanRT, childScan );
  }
  
  if( !getExtraPeakInfo().empty() ){
    cout<<getExtraPeakInfo()<<endl;
  }
  else{
    printf( "\n");
  }
  
  
  // print the isotope pattern:

  if( ! ISOPEAKS.empty()){ 
    printf("\t");
    vector<CentroidPeak>::iterator I = ISOPEAKS.begin();
    while( I != ISOPEAKS.end() ) {
      printf("%0.4f(%0.0f[%0.0f]) ", (*I).getMass(), (*I).getFittedIntensity(), (*I).getOrgIntensity());
      I++;
    }
    printf("\n");
  }
  
}



//////////////////////////////////////////////////
// check if the input mass matches one of the isotopic masses
bool ms_peak::checkIsotopeBelongingAndAdjustMass(double mass, double mzTolerance){
  
  // check if the mass is really smaller then the mono isotope,
  // then it cant bellong to this peak:
  double deltaSmall = get_MZ() - mass;
  deltaSmall -= simple_math::getMassErrorAtPPMLevel( mass, mzTolerance);
  if( deltaSmall > 0){
    return false;
  }
  
  // ok if not, then check the isotopes:
  // highest isotope to consider:
  int max = 2;
  // check now also the isotopes
  if( ! ISOPEAKS.empty()){ 
    vector<CentroidPeak>::iterator I = ISOPEAKS.begin();
    int i = 1;
    while( I != ISOPEAKS.end() ) {
      if( simple_math::compareMassValuesAtPPMLevel(mass, (*I).getMass(), mzTolerance)){
        return true;
      }
      i++;
      I++;
      if( i > max ){
        break;
      }
    }
  }
  else{
    if( simple_math::compareMassValuesAtPPMLevel(mass, MZ, mzTolerance)){
      return true;
    }
  }

  return false;
}


//////////////////////////////////////////////////
// store the MS/MS scan number and activate this peak as precursor peak:
void ms_peak::activateAsPrecursorPeak( int in ){
  precursorMass = true;
  childScan = in;    
}

//////////////////////////////////////////////////
// store the MS/MS scan number and activate this peak as precursor peak:
void ms_peak::activateAsPrecursorPeak( int in , double iRT){
  
  precursorMass = true;
  childScan = in;
  childScanRT = iRT;
}






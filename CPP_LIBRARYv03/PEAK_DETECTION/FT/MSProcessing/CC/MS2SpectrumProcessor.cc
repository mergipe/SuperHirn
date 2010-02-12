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

#define  USE_ALL_MS_PROCESSING_CLASSES
#include "MS_PROCESSING_USE.h"


double MS2SpectrumProcessor::apexPercentile;
int MS2SpectrumProcessor::highestOrder = 2;

////////////////////////////////////////////////
// constructor for the object MS2SpectrumProcessor:
MS2SpectrumProcessor::MS2SpectrumProcessor(){
  maxIntensity = -1;
}

//////////////////////////////////////////////////
// class desctructor of MS2SpectrumProcessor
MS2SpectrumProcessor::~MS2SpectrumProcessor(){
  ms2PeakList.clear();
}

//////////////////////////////////////////////////
// class copy constructor of MS2SpectrumProcessor
MS2SpectrumProcessor::MS2SpectrumProcessor(const MS2SpectrumProcessor& tmp){
  ms2PeakList = tmp.ms2PeakList;
}

//////////////////////////////////////////////////
// class copy constructor of MS2SpectrumProcessor
MS2SpectrumProcessor::MS2SpectrumProcessor(const MS2SpectrumProcessor* tmp){
  ms2PeakList = tmp->ms2PeakList;
}


//////////////////////////////////////////////////
// copy constructor:
MS2SpectrumProcessor& MS2SpectrumProcessor::operator=(const MS2SpectrumProcessor& tmp){
  ms2PeakList = tmp.ms2PeakList;

  return *this;
}
    
//////////////////////////////////////////////////
// add ms2 peaks:
void MS2SpectrumProcessor::addMS2Peak( ms_peak* in){
  
  if( ms2PeakListMap.empty() ){
    averagePrecursorMZ = 0;
    averagePrecursorCHRG = 0;
  }
 
  averagePrecursorMZ += in->getPrecursorMZ();
  averagePrecursorCHRG += in->getPrecursorCHRG();
  
  ms2PeakListMap.insert( make_pair(in->get_intensity(), *in) );
};


//////////////////////////////////////////////////
// process a ms2 spectrum:
void MS2SpectrumProcessor::processMS2Spectrum( vector<ms_peak>* ms2peaks ){}


//////////////////////////////////////////////////
// process a ms2 spectrum:
void MS2SpectrumProcessor::processMS2Spectrum(){
  
  // make hs2 loss removal:
  averagePrecursorMZ /= (double)ms2PeakListMap.size();
  averagePrecursorCHRG /= (double)ms2PeakListMap.size();

  removeWaterLossRegion( averagePrecursorMZ, averagePrecursorCHRG );

  
  // compute the intensity threshold:
  map<double, ms_peak>::reverse_iterator p = ms2PeakListMap.rbegin();
  
  // get the n-highest intensity value:
  int n = 1;
  double maxIntens = -1;
  while( ( n <= highestOrder) && (p != ms2PeakListMap.rend()) ){
    maxIntens = p->second.get_intensity();
    p++;
    n++;
  }
  
  fragmentIntensThreshold = apexPercentile * maxIntens;
  
  ms2PeakList.clear();
  map<double, ms_peak>::iterator I = ms2PeakListMap.begin();
  while( I != ms2PeakListMap.end() ) {
    // filter the low intensities out:
    if( I->second.get_intensity() >= fragmentIntensThreshold ){
      ms2PeakList.push_back( I->second );
    }
    I++;
  }
  
  ms2PeakListMap.clear();
}


//////////////////////////////////////////////////
// full processing of a ms2 spectrum:
// - remove H20 loss peak
// - apex percentile thresholding
void MS2SpectrumProcessor::processMS2Spectrum( MS2_feature* in){  
  
  MS2ConsensusSpectrum* ms2 = (MS2ConsensusSpectrum*) in;
  
  // h20 loss:
  removeWaterLossRegion( in );
  
  // compute the apex of the MS2 scans:
  computeMS2SpectrumApexThreshold( ms2 );
  
  // apply the threshold:
  map<double, MS2Fragment>::iterator I = ms2->getMS2FragmentMap()->begin();  
  while( I != ms2->getMS2FragmentMap()->end() ) {
    
    if( I->second.getFragmentPeakArea() < fragmentIntensThreshold ){
      ms2->getMS2FragmentMap()->erase( I++ );
    }
    else{
      I++;
    }      
  }  
  
  
}


//////////////////////////////////////////////////
// compute the apex intensity threshold for a ms2 spectrum:
void MS2SpectrumProcessor::computeMS2SpectrumApexThreshold( MS2_feature* in){  
  computeMS2SpectrumApexThreshold( (MS2ConsensusSpectrum*)in );
}




//////////////////////////////////////////////////
// compute the apex intensity threshold for a ms2 spectrum:
void MS2SpectrumProcessor::computeMS2SpectrumApexThreshold( MS2ConsensusSpectrum* in){
  
  
  vector<double> intensMap;
  map<double, MS2Fragment>::iterator I = in->getMS2FragmentMap()->begin();  
  while( I != in->getMS2FragmentMap()->end()  ) {
    intensMap.push_back( I->second.getFragmentPeakArea());
    I++;
  }
  
  if( !intensMap.empty() ){
    
    // sort it:
    sort( intensMap.begin(), intensMap.end() );
    
    // get the n-highest intensity value:
    int n = intensMap.size() - highestOrder;
    if( n < 0 ){
      n = 0;
    }
    double maxIntens = intensMap.at( n );
    fragmentIntensThreshold = apexPercentile * maxIntens;
  }
  else{
    fragmentIntensThreshold = 0;
  }
  
  in = NULL;
}


//////////////////////////////////////////////////
// remove H2O loss region of the MS2 spectra
void MS2SpectrumProcessor::removeWaterLossRegion( feature* in){
   
  MS2_feature* ms2F = in->getMS2Feature();
  removeWaterLossRegion( ms2F );
  
  // check if peaks are remaining:
  if( ms2F->getMS2FragmentMap()->size( ) == 0 ){
    in->removeMS2Feature();
  }
}

//////////////////////////////////////////////////
// remove H2O loss region of the MS2 spectra
void MS2SpectrumProcessor::removeWaterLossRegion( MS2_feature* ms2F){
  
  if( ms2F != NULL ){
    
    double precursorMZ = ms2F->getPrecursorMZ();
    double precursorCHRG = ms2F->getPrecursorChrg();
    if( precursorCHRG < 1 ){
      precursorCHRG = 1.0;
    }
    
    // define water loss region:
    // 3 times H2O / by charge state:
    // max = precursor mass:
    double minLossMZRegion = precursorMZ - ( 2.0 * 18.00 ) / precursorCHRG;
    double maxLossMZRegion = precursorMZ;
    
    map<double, MS2Fragment>::iterator I = ms2F->getMS2FragmentMap()->begin();  
    while( I != ms2F->getMS2FragmentMap()->end() ) {
      
      if( ( I->second.getFragmentMz() > minLossMZRegion ) && ( I->second.getFragmentMz() < maxLossMZRegion ) ){
        ms2F->getMS2FragmentMap()->erase( I++ );
      }
      else{
        I++;
      }      
    }  

  }
}



//////////////////////////////////////////////////
// remove H2O loss region in the map of MS peaks
void MS2SpectrumProcessor::removeWaterLossRegion( double precursorMZ, int precursorCHRG){
  
  if( !ms2PeakListMap.empty() ){
  
    if( precursorCHRG < 1 ){
      precursorCHRG = 1.0;
    }
    
    // define water loss region:
    // 3 times H2O / by charge state:
    // max = precursor mass:
    double minLossMZRegion = precursorMZ - ( 2.0 * 18.00 ) / precursorCHRG;
    double maxLossMZRegion = precursorMZ;
    
    map<double, ms_peak>::iterator I = ms2PeakListMap.begin();
    while( I != ms2PeakListMap.end() ) {
      
      if( ( I->second.get_MZ() > minLossMZRegion ) && ( I->second.get_MZ() < maxLossMZRegion ) ){
        ms2PeakListMap.erase( I++ );
      }
      else{
        I++;
      }      
    }  
    
  }
}
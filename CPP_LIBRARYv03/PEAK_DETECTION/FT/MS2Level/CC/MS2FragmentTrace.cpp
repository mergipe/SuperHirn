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

#define USE_MS2_FRAGMENT_TRACE
#include "MS2_FT_PEAK_DETECTION_USE.h"



double MS2FragmentTrace::APEX_PERCENTILE;

////////////////////////////////////////////////
// constructor for the object MS2FragmentTrace:
MS2FragmentTrace::MS2FragmentTrace(){
}

////////////////////////////////////////////////
// constructor for the object LC_elution_peak:
MS2FragmentTrace::MS2FragmentTrace(MZ_series_ITERATOR data, double MZ): LC_elution_peak(data, MZ){
  averagePrecursorCHRG = 0;
  averagePrecursorMZ = 0;
  
  MS2FragmentTrace::APEX_PERCENTILE = 0.1;
  
  // analyze the peak:
  // analyzeLCElutionPeak();
  // analyze();
}  


//////////////////////////////////////////////////////////////////
// determine the intensity background baseline based on Apex 
// value:
void MS2FragmentTrace::setApexIntensityThreshold(){

  double maxIntens = 0;
  SIGNAL_iterator P = get_signal_list_start();
  while(P != get_signal_list_end()){
    ms_peak* peak = &(P->second);
    if( maxIntens < peak->get_intensity() ) {
      maxIntens = peak->get_intensity();
    }
    P++;
  }
  
  fSignalToNoise = 0;
  fSNIntensityThreshold = maxIntens * MS2FragmentTrace::APEX_PERCENTILE;

}


//////////////////////////////////////////////////////////////////
// determine start / end of the elution peak
void MS2FragmentTrace::setStartEndElution(){

  // search first peak above apex threshold:
  SIGNAL_iterator P = get_signal_list_start();
  while(P != get_signal_list_end()){
    ms_peak* peak = &(P->second);
    if( fSNIntensityThreshold <= peak->get_intensity() ) {
      fScanNumberStart = peak->get_scan_number();
      fStartTR = peak->get_retention_time();
      break;
    }
    P++;
  }

  // search last above:
  while(P != get_signal_list_end()){
    ms_peak* peak = &(P->second);
    fScanNumberEnd = peak->get_scan_number();
    fEndTR = peak->get_retention_time();
    if( fSNIntensityThreshold > peak->get_intensity() ) {
      break;
    }
    P++;
  }
  
  
}


//////////////////////////////////////////////////
// class desctructor of MS2FragmentTrace
MS2FragmentTrace::~MS2FragmentTrace(){
}

//////////////////////////////////////////////////
// class copy constructor of MS2FragmentTrace
MS2FragmentTrace::MS2FragmentTrace(const MS2FragmentTrace& tmp):LC_elution_peak(tmp){
  averagePrecursorMZ = tmp.averagePrecursorMZ;
  averagePrecursorCHRG = tmp.averagePrecursorCHRG;
}

//////////////////////////////////////////////////
// class copy constructor of MS2FragmentTrace
MS2FragmentTrace::MS2FragmentTrace(const MS2FragmentTrace* tmp):LC_elution_peak(tmp){
  averagePrecursorMZ = tmp->averagePrecursorMZ;
  averagePrecursorCHRG = tmp->averagePrecursorCHRG;
}


//////////////////////////////////////////////////
// copy constructor:
MS2FragmentTrace& MS2FragmentTrace::operator=(const MS2FragmentTrace& tmp){
  averagePrecursorMZ = tmp.averagePrecursorMZ;
  averagePrecursorCHRG = tmp.averagePrecursorCHRG;
  return *this;
}
    
/////////////////////////////////////////////
// compute average precuror Mz:
void MS2FragmentTrace::computeAveragePrecursorValues(){
  
  if( get_nb_ms_peaks() > 1 ){
    
    averagePrecursorMZ = 0;
    averagePrecursorCHRG = 0;
    
    SIGNAL_iterator P = get_signal_list_start();
    while(P != get_signal_list_end()){
      ms_peak* peak = &(P->second); 
      averagePrecursorMZ += peak->getPrecursorMZ();
      averagePrecursorCHRG += peak->getPrecursorCHRG();
     P++;
    }
    averagePrecursorMZ /= (double) intens_signals.size();
    averagePrecursorCHRG /= (double) intens_signals.size();
  }
  else{
    SIGNAL_iterator P = get_signal_list_start();
    ms_peak* peak = &(P->second); 
    averagePrecursorMZ = peak->getPrecursorMZ();
    averagePrecursorCHRG = peak->getPrecursorCHRG();
  }
}

/////////////////////////////////////////////
// show info of the MS2 fragment trace:
void MS2FragmentTrace::show_info(){

  // print AMRT tag info:
  printf("m/z=%0.4f(+%d) | precursor=%0.4f :: ", get_apex_MZ(), get_charge_state(), averagePrecursorMZ );
  
  // scan/tr range:
  printf("scan:[%d,%d,%d], Tr:[%0.2f,%0.2f,%0.2f],",fScanNumberStart,fScanNumberApex,fScanNumberEnd,fStartTR,fRT,fEndTR);

  // area/intensity:
  printf("A=%0.1f[%0.1f]",get_total_peak_area(),get_apex_intensity());
  printf("\n");

}

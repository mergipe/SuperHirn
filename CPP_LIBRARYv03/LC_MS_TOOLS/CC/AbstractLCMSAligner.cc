///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS ALIGNER:
// ALIGNMENT of the LC-MS spectra pair:
//
// according to Silva et al, 2005, "Quantitative Protemice Analysis by
// Accurate Mass Retention Time Pairs"
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#define USE_ABSTRACT_LC_MS_ALIGNER
#include "LC_MS_TOOLS_USE.h"



// retention time tolerance, usual 5min:
double AbstractLCMSAligner::Tr_window;
// mass tolerance, usual 1.0 Da:
double AbstractLCMSAligner::Mz_window;
// percentage of outside delta error points
double AbstractLCMSAligner::ERROR_DELTA_POINTS;
// Tr tolerance used to compare
// spec A to B after smoothing and
// also used in the merging
double AbstractLCMSAligner::Tr_tolerance;
// M/z tolerance used to compare
// spec A to B after smoothing and
// also used in the merging
double AbstractLCMSAligner::Mz_tolerance;

// defines if also identifications should be checked
// in finding common lc/ms peaks between runs:
bool AbstractLCMSAligner::ID_ALIGNMENT_WEIGHT;

bool AbstractLCMSAligner::print_out = false;
// maximal number of iterations for a LC/MS alignment
int AbstractLCMSAligner::MAX_ITERATION = 10;


////////////////////////////////////////////////
// constructor for the
AbstractLCMSAligner::AbstractLCMSAligner(){
  referenceLCMSMap = NULL;
  toAlignLCMSMap = NULL;  
}

////////////////////////////////////////////////
// constructor for the
AbstractLCMSAligner::AbstractLCMSAligner(bool IN_plot){  
  print_out = IN_plot;
  referenceLCMSMap = NULL;
  toAlignLCMSMap = NULL;
}


//////////////////////////////////////////////////
// class desctructor
AbstractLCMSAligner::~AbstractLCMSAligner(){  
  referenceLCMSMap = NULL;
  toAlignLCMSMap = NULL;
}


///////////////////////////////
// starts alignemnt of the 2 spec
void AbstractLCMSAligner::START_LC_MS_Alignment(){
  
  // prepocess the LC-MS maps for the alignemnt
  preprocessLCMSMaps();
  
  // determine the retention time model
  modelRetentionTimeDrift();
  
  // prepocess the LC-MS maps for the alignemnt
  normalizeRetentionTimeScales();
  
  
}

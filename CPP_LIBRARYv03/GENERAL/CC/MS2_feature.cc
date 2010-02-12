///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS MS2_feature:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_MS2_FEATURE
#include "GENERAL_USE.h"


////////////////////////////////////////////////
// constructor for the object MS2_feature:
MS2_feature::MS2_feature(MS2Fragment* in):ClusteredMS2ConsensusSpectrum(in){
  ID = -1;
}

////////////////////////////////////////////////
// constructor for the object MS2ConsensusSpectrum:
MS2_feature::MS2_feature(double iPrecursorMZ, double iTR, int iChrg, int iApexScan) : ClusteredMS2ConsensusSpectrum(iPrecursorMZ, iTR, iChrg, iApexScan ){
  ID = -1;
}

//////////////////////////////////////////////////
// class desctructor of MS2_feature
MS2_feature::~MS2_feature(){
}

//////////////////////////////////////////////////
// class copy constructor of MS2_feature
MS2_feature::MS2_feature(const MS2_feature& tmp):ClusteredMS2ConsensusSpectrum(tmp){
  ID = tmp.ID;
}

//////////////////////////////////////////////////
// class copy constructor of MS2_feature
MS2_feature::MS2_feature(const MS2_feature* tmp):ClusteredMS2ConsensusSpectrum(tmp){
  ID = tmp->ID;
}

//////////////////////////////////////////////////
// copy constructor:
MS2_feature& MS2_feature::operator=(const MS2_feature& tmp){
  ClusteredMS2ConsensusSpectrum::operator=(tmp);
  ID = tmp.ID;
  return *this;
}


/////////////////////////////////////////////
// show info 
void MS2_feature::show_info(){
  
  // print AMRT tag info:
  printf("\t\tMS2 Cons.Scan:#%d,m/z=%0.4f,Tr=%0.2f[%0.2f-%0.2f],z=%d,apex.scan=%d,#MS2 Fragm=%d", 
         getID(), getPrecursorMZ(), getTR(), getStartTR(), getEndTR(), getPrecursorChrg(), getApexScan(), getNbMS2Fragments());
  printf("\n");
  
  //((ClusteredMS2ConsensusSpectrum)this).show_info();
}

    
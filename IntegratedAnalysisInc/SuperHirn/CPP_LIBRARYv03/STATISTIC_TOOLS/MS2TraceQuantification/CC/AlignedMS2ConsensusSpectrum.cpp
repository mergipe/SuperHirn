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

#define USE_ALIGNED_MS2_CONSENSUS_SPECTRUM
#include "MS2_TRACE_QUANTIFICATION_USE.h"



// minmal detection occurence in order to accept as aligned fragment:
unsigned int AlignedMS2ConsensusSpectrum::MINMINAL_ALIGNED_FRAGMENT_DETECTION_TIME = 2;


////////////////////////////////////////////////
// constructor for the object AlignedMS2ConsensusSpectrum:
AlignedMS2ConsensusSpectrum::AlignedMS2ConsensusSpectrum( int id, MS2ConsensusSpectrum* in){
  
  precursorMZ = in->getPrecursorMZ();
  TR = in->getTR();
  z = in->getPrecursorChrg();
  apexScan = in->getApexScan();
  
  this->addMS2ConsensusSpectrum( id, in );
}


//////////////////////////////////////////////////
// class desctructor of AlignedMS2ConsensusSpectrum
AlignedMS2ConsensusSpectrum::~AlignedMS2ConsensusSpectrum(){
  AlignedMS2FragmentPeaks.clear();
  ConsensSpectra.clear();
}

//////////////////////////////////////////////////
// class copy constructor of AlignedMS2ConsensusSpectrum
AlignedMS2ConsensusSpectrum::AlignedMS2ConsensusSpectrum(const AlignedMS2ConsensusSpectrum& tmp) : MS2ConsensusSpectrum(tmp) {
  AlignedMS2FragmentPeaks = tmp.AlignedMS2FragmentPeaks;
  ConsensSpectra = tmp.ConsensSpectra;
}

//////////////////////////////////////////////////
// class copy constructor of AlignedMS2ConsensusSpectrum
AlignedMS2ConsensusSpectrum::AlignedMS2ConsensusSpectrum(const AlignedMS2ConsensusSpectrum* tmp){
  AlignedMS2FragmentPeaks = tmp->AlignedMS2FragmentPeaks;
  ConsensSpectra = tmp->ConsensSpectra;
}


//////////////////////////////////////////////////
// copy constructor:
AlignedMS2ConsensusSpectrum& AlignedMS2ConsensusSpectrum::operator=(const AlignedMS2ConsensusSpectrum& tmp){
  AlignedMS2FragmentPeaks = tmp.AlignedMS2FragmentPeaks;
  ConsensSpectra = tmp.ConsensSpectra;
  return *this;
}

//////////////////////////////////////////////////
// remove outlier fragments based on their:
// MS2Fragment::OutlierAttribute = ...
// 1: retention time
// 2: precursor mass
// etc.
void AlignedMS2ConsensusSpectrum::removeOutlierFragments(){

  // store in this vector the fragments:
  vector< pair<double, void*> > ValueVector;
  
  /*
  // store fragments by the desired attribute as outlier detection value:
  map<double, MS2Fragment>::iterator P = MS2FragmentPeaks.begin();
  while( P != MS2FragmentPeaks.end() ){
    
    // get the attribute:
    double value = (*P).second.getOutlierDetectionAttribute();
    
    // store in the vector
    ValueVector.push_back( pair<double, void*>( value, &(*P).second ) );     
   
    P++;
  }
  
  // start the iterative outlier removal by the set attribut of a ms2 fragment:
  simple_math myMath;
  myMath.ITERATIVE_OUTLIER_DETECTION_BY_DIXON( &ValueVector );

  // convert back after the oulier detected vector:
  map<double, MS2Fragment> newFragments;
  vector< pair<double, void*> >::iterator I = ValueVector.begin();
  while( I != ValueVector.end() ){
    pair<double, void*> p = (*I);
    MS2Fragment* frag = (MS2Fragment*) p.second;
    newFragments.insert( make_pair( frag->getFragmentPeakArea(), *frag ) );
    I++;
  }
  
  // copy back:
  MS2FragmentPeaks.clear();
  MS2FragmentPeaks = newFragments;
  newFragments.clear();
   */
  
}


//////////////////////////////////////////////////
// trace the fragments across LC-MS runs:
void AlignedMS2ConsensusSpectrum::constructAlignedConsenusSpectraFragments(){
  
  // stores the individual consens spectra:
  map<int, MS2ConsensusSpectrum* >::iterator C = ConsensSpectra.begin();
  while( C != ConsensSpectra.end() ){
    extractFragmentsFromConsensSpectra( (*C).first, (*C).second );
    C++; 
  }
  
  // process extracted aligned fragment traces:
  processAlignedFragmentTraces();
  
}



//////////////////////////////////////////////////
// extracts fragments from a consens and inserts
// them into the trace:
void AlignedMS2ConsensusSpectrum::extractFragmentsFromConsensSpectra( int id, MS2ConsensusSpectrum* in  ){
  
  map<double, MS2Fragment>::iterator P = in->getMS2FragmentPeakStart();
  while( P != in->getMS2FragmentPeakEnd() ){
    
    double searchMZ = P->second.getFragmentMz();
    
    // try to map the fragment to an existing trace:
    map<double,  profileFragmentTrace>::iterator F = AlignedMS2FragmentPeaks.lower_bound( searchMZ );
  
    // everything smaller:
    if( F == AlignedMS2FragmentPeaks.end() ){
      
      // check end mass:
      if( !AlignedMS2FragmentPeaks.empty() ){
        F--;
        double delta = fabs( F->first - searchMZ );
        if( delta > MS2_feature::MS2_MZ_TOLERANCE){
          F = AlignedMS2FragmentPeaks.end();
        }
      }
    }
    
    // everything bigger ?:
    else if( F == AlignedMS2FragmentPeaks.begin() ){
      
      // check end mass:
      double delta = fabs( F->first - searchMZ );
      if( delta > MS2_feature::MS2_MZ_TOLERANCE){
        F = AlignedMS2FragmentPeaks.end();
      }
    }
    
    // exact match:
    else if( (*F).first == (*P).first ){
      // do nothing
    }
    
    // somewhere within the list, but not exact match:
    else { 
      
      // check current one:
      double deltaUP = fabs( F->first - searchMZ );
      F--;
      double deltaDOWN = fabs( F->first - searchMZ );      
      // determine the smaller delta:
      double mainDelta = deltaDOWN;
      if ( mainDelta > deltaUP ){
        mainDelta = deltaUP;
        F++;
      }
      
      if( mainDelta > MS2_feature::MS2_MZ_TOLERANCE){
        F = AlignedMS2FragmentPeaks.end();
      }
    
    }
    
    if( F != AlignedMS2FragmentPeaks.end() ){
      (*F).second.insert( make_pair (id,  &(P->second)) );
    }
    else{
      profileFragmentTrace tmp;
      tmp.insert ( make_pair (id,  &(P->second)) );
      AlignedMS2FragmentPeaks.insert( make_pair(searchMZ, tmp ) );
    }
    
    P++;
  }
}



//////////////////////////////////////////////////
// process extracted aligned fragment traces:
void AlignedMS2ConsensusSpectrum::processAlignedFragmentTraces(){
  
  // try to map the fragment to an existing trace:
  map<double,  profileFragmentTrace>::iterator P = AlignedMS2FragmentPeaks.begin();
  while( P != AlignedMS2FragmentPeaks.end() ){
    
    
    // check the number of alignments
    if( P->second.size() < AlignedMS2ConsensusSpectrum::MINMINAL_ALIGNED_FRAGMENT_DETECTION_TIME ){
      AlignedMS2FragmentPeaks.erase( P++ );
    }
    
    
    
    else{
      P++;
    }
    
  }
  
}



//////////////////////////////////////////////////
// add a MS2 fragment:
void AlignedMS2ConsensusSpectrum::addMS2ConsensusSpectrum( int id, MS2ConsensusSpectrum* in ){
  
  ConsensSpectra.insert( make_pair(id, in) );
  
}



//////////////////////////////////////////////////////
// plot all the consensus MS2 spectrum in one plot:
void AlignedMS2ConsensusSpectrum::plotCombinedSpectra( ){
  
  char buffer[255];
  sprintf( buffer, "CombinedMS2ConsSpec%0.2f", precursorMZ);
  string tmp = buffer;
  data_plotter* PLOT = new data_plotter( tmp ); 
  
  map<int, MS2ConsensusSpectrum* >::iterator C = ConsensSpectra.begin();
  while( C != ConsensSpectra.end() ){

    sprintf( buffer, "LC-MS %d", C->first);
    
    map<double, double> data;
    map<double, MS2Fragment>::iterator I = C->second->getMS2FragmentPeakStart();
    while( I != C->second->getMS2FragmentPeakEnd() ){
      data.insert( make_pair( I->second.getFragmentMz(), I->second.getFragmentPeakArea()) );
      I++;
    }
    
    PLOT->add_plot_data_impulses( &data, buffer);
  
    data.clear();
    C++;
  }
  
  
  PLOT->plot_TWOD_data();
  
  delete PLOT;
  PLOT = NULL;
  
  
}


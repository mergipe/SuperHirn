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


#define USE_MS2_PROCESS_DATA
#include "MS2_FT_PEAK_DETECTION_USE.h"

// minimal signal intensity
double MS2_Process_Data::MS2_INTENSITY_THRESHOLD;
// MS1 (= precurosr) mass tolerance
double MS2_Process_Data::PRECURSOR_MZ_TOLERANCE;


// how many times a fragment had to be detected at minima:
int MS2_Process_Data::MS2_MINIMAL_NB_FRAGMENT_DETECTION;
// minimal number of MS2 consenus fragments in the MS2 spectrum:
int MS2_Process_Data::MINIMAL_NB_MS2_FRAGMENT_IN_SPECTRUM;

double MS2_Process_Data::MS2FragTraceOperatorTargetMZ;
//double MS2_Process_Data::MS2_intensity_apex_percentil_cutoff;
double MS2_Process_Data::MS2_TR_RESOLUTION;
double MS2_Process_Data::MS2_TR_LC_CORRELATION_TOLERANCE_APEX;
double MS2_Process_Data::MS2_TR_LC_CORRELATION_TOLERANCE_BORDER;
double MS2_Process_Data::MS2_FRAGMENT_MIN_INTENSITY_AREA_THRESHOLD;


// if data are in centroid form or not:
bool MS2_Process_Data::CENTROID_DATA_MODUS = true;
bool MS2_Process_Data::POST_MSMS_SPECTRUM_CLUSTERING;


// to track detected monoistopic mass for debugging:
bool MS2_Process_Data::MonoIsoDebugging = false;
double MS2_Process_Data::DebugMonoIsoMassMin =  248.8; 
double MS2_Process_Data::DebugMonoIsoMassMax = 249.3; 

////////////////////////////////////////////////
// constructor for the object MS2_Process_Data:
MS2_Process_Data::MS2_Process_Data() : Process_Data(){
  
  
  //////////////////
  // parameter to visualize
  // the data & peak detection:
  // read parameters from "param.def"
  read_param* def = new read_param();
  
  // minimal intensity of a ms peak::
  //def->search_tag("FT peak detect MS2 intensity min threshold", &INTENSITY_THRESHOLD);
  MS2_INTENSITY_THRESHOLD = INTENSITY_THRESHOLD;
  
  // MS2 m/z tolerance value:
  def->search_tag("FT peak detect MS2 m/z tolerance", &MZ_TOLERANCE);
  MS2_feature::MS2_MZ_TOLERANCE = MZ_TOLERANCE;

  // option if inter scan distance measured in time or scans::
  // def->search_tag("MS/MS inter-scan distance measure type", &TIME_CLUSTERING_BY_RETENTION_TIME);
  
  // interscan distance in minutes:
  def->search_tag("MS/MS inter-scan retentiontime distance", &max_inter_scan_retention_time_distance);
    
  // MS1 m/z tolerance value:
  def->search_tag("FT peak detect MS1 m/z tolerance", &PRECURSOR_MZ_TOLERANCE);
  
  // minimal number of cluster members
  def->search_tag("FT peak detect MS2 min nb peak members", &min_nb_cluster_members);
  MS2_MINIMAL_NB_FRAGMENT_DETECTION = min_nb_cluster_members;
  
  // minimal number of cluster members
  def->search_tag("Min nb MS2 fragments per consenus spectrum", &MINIMAL_NB_MS2_FRAGMENT_IN_SPECTRUM);

  // plotting
  def->search_tag("gnuplot plot generator", &gnuplot);
  
  delete def;
  def = NULL;

}

//////////////////////////////////////////////////
// class desctructor of MS2_Process_Data
MS2_Process_Data::~MS2_Process_Data(){
  allMS2ConsenusSpectra.clear();
}

//////////////////////////////////////////////////
// class copy constructor of MS2_Process_Data
MS2_Process_Data::MS2_Process_Data(const MS2_Process_Data& tmp){
}


//////////////////////////////////////////////////
// copy constructor:
MS2_Process_Data& MS2_Process_Data::operator=(const MS2_Process_Data& tmp){
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
// inputs raw data into the object:
void MS2_Process_Data::add_scan_MS2_raw_data(int SCAN, double precursorMZ, float TR, int z, float* peaksValues){
  
  
  vector<ms_peak> PEAK_LIST;
  int i = 0;
  while ( peaksValues[i] >= 0) { // isotopic patterns are withing the same peak group
    double mz = peaksValues[i];
    i++;
    double intens = peaksValues[i];
    i++;
    
    // convert to peak:
    ms_peak* peak = new ms_peak( SCAN, mz, intens );
    peak->set_retention_time( TR );
    peak->setPrecursorMZ( precursorMZ );
    peak->setPrecursorCHRG( z );
    PEAK_LIST.push_back( *peak);
    delete peak;
    peak = NULL;
    
   }
    
  // add the ms peaks to the data structure: 
  add_scan_MS2_raw_data( precursorMZ, TR, z, &PEAK_LIST);
  // delete the peaks:
  PEAK_LIST.clear();


}

///////////////////////////////////////////////////////////////////////////////
// inputs raw data into the object:
void MS2_Process_Data::add_scan_MS2_raw_data(Generic_MS_Scan* MS2Scan, CentroidData& pCentroidData){
  
  
  //////////////////////////////////
  // process the MS2 peaks:
  MS2SpectrumProcessor* processor = new MS2SpectrumProcessor();
  
  
  list<CentroidPeak> centroidPeaks;
  list<CentroidPeak>::iterator start, end, pi;
  pCentroidData.get(centroidPeaks);  
  pCentroidData.resetPeakGroupIter();
  while (pCentroidData.getNextPeakGroup(start,end)) { // isotopic patterns are withing the same peak group
    
    while( start != end ){
    
      if (start->getIntensity() >= MS2_INTENSITY_THRESHOLD){
          
        // convert to peak:
        ms_peak* peak = new ms_peak( MS2Scan->getMSScanNumber(), start->getMass(), start->getIntensity() );
        peak->set_retention_time( MS2Scan->getMSScanRetentionTime() );
        peak->setPrecursorMZ( MS2Scan->getMZPrecursor() );
        peak->setPrecursorCHRG( MS2Scan->getZPrecursor() );
        peak->set_Chrg( -1 );
        
        // add the peak to the processor class:
        processor->addMS2Peak( peak );
        
        /*
        if( MonoIsoDebugging ){
          if( ( DebugMonoIsoMassMin <= peak->get_MZ()) && ( DebugMonoIsoMassMax >= peak->get_MZ()) ){
            cout<<"before:"<<endl;
            peak->show_info();
          }
        }
         */
        
        
        // delete it:
        delete peak;
        peak = NULL;
      }
      
      start++;
    }
  }
  
  // process a ms2 spectrum:
  processor->processMS2Spectrum();
      
  // add the ms peaks to the data structure: 
  add_scan_MS2_raw_data( MS2Scan->getMZPrecursor(), MS2Scan->getMSScanRetentionTime(), MS2Scan->getZPrecursor(), processor->getMS2PeakList());
  
  // delete:
  delete processor;
  processor = NULL;
  
  
  }



///////////////////////////////////////////////////////////////////////////////
// inputs raw data into the object:
void MS2_Process_Data::add_scan_MS2_raw_data( double precursorMZ, float TR, int z, vector<ms_peak>* PEAK_LIST){


  //////////////////////////////////
  // iterate through the vector:
  vector<ms_peak>::iterator P = PEAK_LIST->begin();
  while(P != PEAK_LIST->end()){
    
    ms_peak* PEAK = &(*P);
    
    // check if its above the min. intensity:
    if( PEAK->get_intensity() > MS2_INTENSITY_THRESHOLD ){
      
      // set the retention time of the ms peak:
      PEAK->set_retention_time(TR);
      
      if( MonoIsoDebugging ){
        if( ( DebugMonoIsoMassMin <= PEAK->get_MZ()) && ( DebugMonoIsoMassMax >= PEAK->get_MZ()) ){
          PEAK->show_info();
        }
      }
      
      // check if this MZ has already been observed:
      MAIN_ITERATOR LCP = this->check_MZ_occurence( PEAK , true);
      if(  LCP != get_MZ_LIST_end() ){
        insert_observed_mz(LCP, PEAK);
      }
      else{
        insert_new_observed_mz( PEAK );
      }
      
      
    }
    PEAK = NULL;
    P++;
  }  
}



///////////////////////////////////////////////////////////////////////////////
// check extracted MS2 level traces:
// - discard those with less than minimal occurence (parameter 
bool MS2_Process_Data::check_extracted_MS2_trace(elution_peak LCelution){
  
  // filter be times detected:
  if( LCelution.size() < (unsigned int)MS2_Process_Data::MS2_MINIMAL_NB_FRAGMENT_DETECTION ){
    return false;
  }
  
  return true;
}




///////////////////////////////////////////////////////////////////////////////
// construction of MS2 Consensus Spectra:
// - converts an extracted MS2 level traces into a MS2 fragment trace object
// - assembles these then into MS2 consensus spectra
void MS2_Process_Data::constructMS2ConsensusSpectra( ){
  
  // set important processing parameters such as apex cutoff for noise removal
  // and the tr delta steps for peak area integration
  // LC_elution_peak::intensity_apex_percentil_cutoff = MS2_Process_Data::MS2_intensity_apex_percentil_cutoff;
  LC_elution_peak::TR_RESOLUTION = MS2_Process_Data::MS2_TR_RESOLUTION;
  
  
  //////////////////////////////////
  // run through all constructe fragment traces and assemble them into
  // ms2 conesnus spectra:
  MAIN_ITERATOR P_MZ = get_MZ_LIST_start();
  while(P_MZ != get_MZ_LIST_end()){
    
    // fragment m/z
    double fragmentMZ = (*P_MZ).first;
    MZ_series_ITERATOR Q_SER = (*P_MZ).second.begin();
    while( Q_SER != (*P_MZ).second.end() ){

      if( check_extracted_MS2_trace( *Q_SER) ){
 
        /*
        if( fragmentMZ > 353){
          cout<<fragmentMZ<<endl;
        }
         */
        
        // convert to a MS2 fragment trace object:
        MS2FragmentTrace* tmp = new MS2FragmentTrace(Q_SER, fragmentMZ);
        // analysze the fragment trace
        tmp->analyzeMS2FragmentTrace();
          
        //tmp->show_info();
        
        // find the corrsponding MS2 consensus spectra
        assembleMS2FragmentTraceIntoMS2Scan( tmp );
              
        delete tmp;
        tmp = NULL;
      }
      
      Q_SER++;
    }
    
    P_MZ++;
  }
  
  
  ////////////////////////
  // post process the extracted MS2 consenus spectra:
  MS2SpectrumProcessor processor;
  multimap< double, MS2_feature >::iterator P = MS2ConsenusSpectraMap.begin();  
  while( P != MS2ConsenusSpectraMap.end()){
    MS2_feature* fea = &(P->second);    
    
    /* 
    if( (722 < fea->getPrecursorMZ() ) && (722.9 > fea->getPrecursorMZ() ) ){
      fea->show_info();
    }
     */ 
    
    // process the spectrum:
    //fea->processConsenusSpectraFragments();
    
    // remove H2O loss stuff:
    // fea->removeWaterLossRegion( );
    // processor.processMS2Spectrum( fea );

    // check if this is a valid ms2 consensus spectrum
    if( checkMS2ConsensusSpectrum( fea ) ){
      
      
      // plot the spectra
      if( gnuplot ){
        //(*P).plotSpectrum();
      }
      
      // store it in the out vector:
      allMS2ConsenusSpectra.push_back( *fea );
    }
    
    P++;
  }
  
  // sort the MS2_features:
  sortMS2Features();  
  
  /*
  vector<MS2_feature>::iterator z = allMS2ConsenusSpectra.begin();
  while(z != allMS2ConsenusSpectra.end() )  {
    z->show_info();
    z++;
  }
   */
  
  // clear the map:
  MS2ConsenusSpectraMap.clear();
  
}


/////////////////////////////////////////////////////////////////////////////////
// sort the MS2_features:
void MS2_Process_Data::sortMS2Features(){
  sort(allMS2ConsenusSpectra.begin(),allMS2ConsenusSpectra.end(),OPERATOR_MS2_FEATURE());
}



/////////////////////////////////////////////////////////////////////////////////
// check if this is a valid ms2 consensus spectrum
bool MS2_Process_Data::checkMS2ConsensusSpectrum( MS2_feature* spectrum ){
  
  // check how many consenus fragments are in the spectrum:
  if( spectrum->getNbMS2Fragments() < MS2_MINIMAL_NB_FRAGMENT_DETECTION ){
    return false;
  }
  
  return true;
}




/////////////////////////////////////////////////////////////////////////////////
// construct conensus MS2 spectra by assembling the traces by their precursor MZ
void MS2_Process_Data::assembleMS2FragmentTraceIntoMS2Scan( MS2FragmentTrace* in ){
  
  /////////////////
  // convert first to a simple MS2_fragment object:
  MS2Fragment* ms2Frag = new MS2Fragment(
                                         in->getPrecursorMZ(), in->getPrecursorCHRG(), in->get_apex_retention_time(), in->get_scan_apex(), in->get_charge_state(), 
                                         in->get_apex_MZ(), in->get_total_peak_area(),
                                         in->get_start_scan(), in->get_end_scan(), 
                                         in->get_start_retention_time(), in->get_end_retention_time());
  
  
  
  // ms2Frag->show_info();
  
  /////////////////////////
  // filter the MS2 Fragment:
  if( checkMS2FragmentValid( ms2Frag ) ){
    
    if( MonoIsoDebugging ){
      if( ( DebugMonoIsoMassMin <= ms2Frag->getFragmentMz()) && ( DebugMonoIsoMassMax >= ms2Frag->getFragmentMz()) ){
        ms2Frag->show_info();
      }
    }
    
    /*
    if ( 1016 == ms2Frag->getApexScan()){
      ms2Frag->show_info();
    }
    */
    
   
    multimap< double, MS2_feature >::iterator F = findMS2ConsensSpectra( ms2Frag );
    if( F == MS2ConsenusSpectraMap.end() ){
      
      // insert new MS2 consenus spectra:
      MS2_feature* tmp = new MS2_feature( ms2Frag );
      tmp->setID( MS2ConsenusSpectraMap.size() );
      
      // add to the map:
      MS2ConsenusSpectraMap.insert( make_pair( tmp->getPrecursorMZ(), *tmp) );
      
      if( MonoIsoDebugging ){
        if( ( DebugMonoIsoMassMin <= ms2Frag->getFragmentMz()) && ( DebugMonoIsoMassMax >= ms2Frag->getFragmentMz()) ){
          cout<<"NewSpec: ";
          tmp->show_info();
        }
      }
      delete tmp;
      tmp = NULL;
      
    }
    else{
      F->second.addMS2Fragment( ms2Frag );
      
      if( MonoIsoDebugging ){
        if( ( DebugMonoIsoMassMin <= ms2Frag->getFragmentMz()) && ( DebugMonoIsoMassMax >= ms2Frag->getFragmentMz()) ){
          cout<<"ExistingSpec: ";
          F->second.show_info();
        }
      }
      
    }
    
  }
  
  delete ms2Frag;
  ms2Frag = NULL;
  in = NULL;
  
}


/////////////////////////////////////////////////////////////////////////////////
// check if a MS2 fragment is valid, i.e. kept for the following analysis steps
bool MS2_Process_Data::checkMS2FragmentValid( MS2Fragment* frag ){
  
  
  
  // filter by peak area, remove those with only very small peak area:
  if( frag->getFragmentPeakArea() <= MS2_FRAGMENT_MIN_INTENSITY_AREA_THRESHOLD){
    return false;
  }
 
  return true;
  
}


/////////////////////////////////////////////////////////////////////////////////
// find the correct MS2Consensus spectrum by precursor mz, z and tr:
multimap< double, MS2_feature >::iterator MS2_Process_Data::findMS2ConsensSpectra( MS2Fragment* frag ){
  
  // precursor mass of the fragment:
  double TargetPrecursorMZ = frag->getPrecursorMZ(); 
  
  // if its empty:
  if( MS2ConsenusSpectraMap.empty() ){
    return MS2ConsenusSpectraMap.end( );
  }
  
  // otherwise search closest iterator:
  multimap< double, MS2_feature >::iterator F = MS2ConsenusSpectraMap.lower_bound( frag->getPrecursorMZ() );
  
  ///////////////////////
  // collect a list of iterators with potential candidates:
  multimap< double, map< double, MS2_feature >::iterator > candidates;
  
  // scan lower mass tolerance:
  multimap< double, MS2_feature >::iterator I = F;
  if( I != MS2ConsenusSpectraMap.begin() ){
    I--;
  }
  
  double deltaPrecursor = fabs( I->second.getPrecursorMZ() - TargetPrecursorMZ );
  while( deltaPrecursor <= MS2_Process_Data::PRECURSOR_MZ_TOLERANCE ){
    
    if( MonoIsoDebugging ){
      
      if( ( DebugMonoIsoMassMin <= frag->getFragmentMz()) && ( DebugMonoIsoMassMax >= frag->getFragmentMz()) ){
        if( (MS2_Process_Data::DebugMonoIsoMassMin <  frag->getFragmentMz() ) && (MS2_Process_Data::DebugMonoIsoMassMax >  frag->getFragmentMz() ) ){
          I->second.show_info();
        }
      }
    }
    
    if( checkMS2FragmentToMS2FeatureBelonging( frag, &(I->second) ) ){
      
      double rank = (&(I->second))->getLCElutionPeakSimilarity( frag );
      if( MonoIsoDebugging ){
        if( ( DebugMonoIsoMassMin <= frag->getFragmentMz()) && ( DebugMonoIsoMassMax >= frag->getFragmentMz()) ){
          cout<<"yes: ";
          I->second.show_info();
          cout<<"::"<<rank<<endl;
        }
      }
      candidates.insert( make_pair( rank, I) );
    }
    
    if( I == MS2ConsenusSpectraMap.begin() ){
      break;
    }
    I--;
    deltaPrecursor = fabs( I->second.getPrecursorMZ() - TargetPrecursorMZ );
  }
  
  
  // scan upper mass tolerance:
  I = F;
  if( I != MS2ConsenusSpectraMap.end() ){
    double deltaPrecursor = fabs( I->second.getPrecursorMZ() - TargetPrecursorMZ );

    while( deltaPrecursor <= MS2_Process_Data::PRECURSOR_MZ_TOLERANCE ){
    
      if( MonoIsoDebugging ){
        if( ( DebugMonoIsoMassMin <= frag->getFragmentMz()) && ( DebugMonoIsoMassMax >= frag->getFragmentMz()) ){
          if( (MS2_Process_Data::DebugMonoIsoMassMin <  frag->getFragmentMz() ) && (MS2_Process_Data::DebugMonoIsoMassMax >  frag->getFragmentMz() ) ){
            I->second.show_info();
          }
        }
      }
      
      if( checkMS2FragmentToMS2FeatureBelonging( frag, &(I->second) ) ){
        
        double rank = (&(I->second))->getLCElutionPeakSimilarity( frag );
  
        if( MonoIsoDebugging ){
          if( ( DebugMonoIsoMassMin <= frag->getFragmentMz()) && ( DebugMonoIsoMassMax >= frag->getFragmentMz()) ){
            cout<<"yes: ";
            I->second.show_info();
            cout<<"::"<<rank<<endl;
          }
        }
        candidates.insert( make_pair( rank, I) );
      }
      
      I++;
      if( I == MS2ConsenusSpectraMap.end() ){
        break;
      }
      deltaPrecursor = fabs( I->second.getPrecursorMZ() - TargetPrecursorMZ );
    }
  }
  
  
  /////////////////////////////////////////////////////
  // find now the one with the best match:
  // i.e. take the one with the smallest Mz difference:
  if( !candidates.empty() ){
    return (candidates.begin())->second;
  }
   return MS2ConsenusSpectraMap.end();
 
 }

///////////////////////////////////////////////////////////////////////////////
// checks if MS2 fragment can be grouped into a existing MS2 feature,
bool MS2_Process_Data::checkMS2FragmentToMS2FeatureBelonging(MS2Fragment* in, MS2_feature* feature){
  
  // check first the charge state:
  if( in->getPrecursorCHRG() != feature->getPrecursorChrg() ){
    return false;
  }
      
  // check precursor mass:
  if( !simple_math::compareMassValuesAtPPMLevel(feature->getPrecursorMZ(),in->getPrecursorMZ(), MS2_Process_Data::PRECURSOR_MZ_TOLERANCE) ){
    return false;
  }
  
  // check if the apexes of the elution peak correspond:
  double delta = fabs( feature->getTR() - in->getTR() );
  if( delta > MS2_TR_LC_CORRELATION_TOLERANCE_APEX){
    return false;
  }
  
  // check now the elution borders if they correspond:
  // give a bit more tolerance:
  delta = fabs(feature->getStartTR() - in->getStartTR());
  if( delta > MS2_TR_LC_CORRELATION_TOLERANCE_BORDER){
    return false;
  }
  
  delta = fabs(in->getEndTR() - feature->getEndTR());
  if( delta > MS2_TR_LC_CORRELATION_TOLERANCE_BORDER){
    return false;
  }
  
  return true;

}
  

///////////////////////////////////////////////////////////////////////////////
// checks if a mz value has already been seen,
// also look for very close ones and cluster them
MAIN_ITERATOR MS2_Process_Data::check_MZ_occurence(ms_peak* PEAK, bool chargeStateCheck){
  /*
  if( MonoIsoDebugging ){
    if( ( DebugMonoIsoMassMin <= PEAK->get_MZ()) && ( DebugMonoIsoMassMax >= PEAK->get_MZ()) ){
      PEAK->show_info();
    }
  }
  */
  
  ////////////////////////////////
  // check first for the possible candidates within the MZ tolerance range:
  double targetMZ = PEAK->get_MZ();
  
  MAIN_ITERATOR  P = get_MZ_lower_bound(targetMZ); 
  vector<MAIN_ITERATOR> CandidateList;
  
  // go decreasing order
  MAIN_ITERATOR  down = P;
  if( P != get_MZ_LIST_start() ){
    do{ 
      down--;
      int check = compareIteratorToPeak( PEAK, down );
        if( check == 1 ){
          CandidateList.push_back( down );
        }
      else if( check == -1){
        break;
      }
      
      
    }
    while( down != get_MZ_LIST_start() );
  }
  
  
  // go increasing order
  MAIN_ITERATOR up = P;
  while( up != get_MZ_LIST_end() ){
    
    int check = compareIteratorToPeak( PEAK, up );
    if( check == 1 ){
      CandidateList.push_back( up );
    }
    else if( check == -1){
      break;
    }
    
    
    up++;
  }
  
  // here the list of possible candidates for 
  // matching this mass is complete
  // now -> find the best one according o
  // a: closest in m/z
  
  if( CandidateList.empty() ){
    // if its not found at all:
    // return the end:
    P = get_MZ_LIST_end();
  }
  else if( CandidateList.size() == 1 ){
    P = *(CandidateList.begin());
  }
  else{
    
    ///////////
    // check for those with smallest mz:
    // (within the tolerance range:)
    P = get_MZ_LIST_end();
    double smallMZDiff = 1000000;
    // int smallScanDiff = 1000000;
    vector<MAIN_ITERATOR>::iterator Z = CandidateList.begin();
    while( Z != CandidateList.end() ){
      
      // get the inter scan distance:
      double MZDiff = fabs(targetMZ - (*Z)->first); 
      if( MZDiff < smallMZDiff ) {
        P = *Z;
        smallMZDiff = MZDiff;
      }
      
      /*
       // get the inter scan distance:
       MZ_series_ITERATOR x = (*Z)->second.end();
       x--;
       int ScanDiff = getElutionPeakDistance( x, targetScan );
       if( ScanDiff < smallScanDiff ) {
         P = *Z;
         smallScanDiff = ScanDiff;
       }
       */
      
      Z++;
      
    }
    
  }
  
  return P;
}




//////////////////////////////////////////////////////////////////
// function which check if a data structure iterator is similar
// to a peak and should be considered 
// returns 1 if ok
// returns 0 if not
// returns -1 if scan range exceeded
int MS2_Process_Data::compareIteratorToPeak(ms_peak* peak, MAIN_ITERATOR check){
  
  // get the difference in the scan numbers:
  MZ_series_ITERATOR x = check->second.end();
  x--;
  // int ScanDiff = getElutionPeakDistance( x, peak->get_Scan() );
  
  // get the last ms peak in the elution peak:
  ms_peak* lastPeak = &(x->rbegin()->second);
  // get precursor mass difference:
  if( !simple_math::compareMassValuesAtPPMLevel(peak->getPrecursorMZ(), lastPeak->getPrecursorMZ(), MS2_Process_Data::PRECURSOR_MZ_TOLERANCE) ){
    return 0;
  }
  
  // charge state:
  if( peak->get_Chrg() != lastPeak->get_Chrg() ){
    return 0;
  }
  
  // fragment mass difference:
  if( !simple_math::compareMassValuesAtPPMLevel(peak->get_MZ(), check->first, MZ_TOLERANCE) ){
    return -1;
  }

  /*
  // scan difference:
  if( ScanDiff > getMaxScanDistance() ){ 
    return 0;
  }
  */
  
  return 1;
}
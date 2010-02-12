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

#define USE_MS2_QUANTIFICATION_CONTROLLER
#define USE_ALIGNED_MS2_CONSENSUS_SPECTRUM
#include "MS2_TRACE_QUANTIFICATION_USE.h"




////////////////////////////////////////////////
// constructor for the object MS2quantificationController:
MS2quantificationController::MS2quantificationController(LC_MS* in){
  MasterMap = in;
  //////////////////
  // parameter to visualize
  // the data & peak detection:
  // read parameters from "param.def"
  read_param* def = new read_param();
  
  // plotting
  def->search_tag("gnuplot plot generator", &gnuplot);
  
  delete def;
  def = NULL;
}

//////////////////////////////////////////////////
// class desctructor of MS2quantificationController
MS2quantificationController::~MS2quantificationController(){
  MasterMap = NULL;
  alignedConsensMS2.clear();
}

//////////////////////////////////////////////////
// class copy constructor of MS2quantificationController
MS2quantificationController::MS2quantificationController(const MS2quantificationController& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of MS2quantificationController
MS2quantificationController::MS2quantificationController(const MS2quantificationController* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
MS2quantificationController& MS2quantificationController::operator=(const MS2quantificationController& tmp){
  return *this;
}


//////////////////////////////////////////////////
// start the quantification on the MS2 tarce level:
void MS2quantificationController::START_MS2_TRACE_QUANTIFICATION(){
  
  // construct fragment profiles:
  this->constructLCMSMS2FragmentTraces();

  // process fragment profiles:
  this->processLCMSMS2FragmentTraces();

}



//////////////////////////////////////////////////
// constructs a MS2 fragment trace across LC-MS runs
// -> for each fragment trace it across aligned features
// and extract a intensity profile:
void MS2quantificationController::constructLCMSMS2FragmentTraces(){
  
  // go through the aligned features and extract traces:
  vector< LC_MS_FEATURE >::iterator P = MasterMap->get_feature_list_begin();
  while( P != MasterMap->get_feature_list_end() ){
    this->processAlignedFeature( &(*P) );
    P++;
  }

}


////////////////////////////////////////////////////
// process fragment profiles:
void MS2quantificationController::processLCMSMS2FragmentTraces(){
  // stores teh aligned consensus spectra
  vector< pair<AlignedMS2ConsensusSpectrum , feature*> >::iterator P = alignedConsensMS2.begin();
  while( P != alignedConsensMS2.end() ) {
   
    P->first.constructAlignedConsenusSpectraFragments();
    //P->first.processAlignedFragmentTraces();
    P++;
  }
}


////////////////////////////////////////////////////
// processes an aligned MS2 feature:
void MS2quantificationController::processAlignedFeature(feature* IN){

  // AlignedMS2ConsensusSpectrum* consens = NULL;
  
  /*
  MS2_feature* ms2 = IN->getMS2Feature();
  if( ms2 != NULL ){
    consens = new AlignedMS2ConsensusSpectrum( IN->get_spectrum_ID(), ms2 );
  }
  
  map<int,feature>::iterator F = IN->get_match_list_start();
  while(F != IN->get_match_list_end()){

    MS2_feature* ms2 = (*F).second.getMS2Feature();
    if( ms2 != NULL ){
      if( consens == NULL ){
        consens = new AlignedMS2ConsensusSpectrum( (*F).second.get_spectrum_ID(), ms2 );
      }
      else{
        consens->addMS2ConsensusSpectrum( (*F).second.get_spectrum_ID(), ms2 );
      }
    }
  
    F++;
  }
  
  
  if( consens != NULL ){
    // set the charge state of the precursor MZ:
    consens->setPrecursorChrg( IN->get_charge_state() );
    
    storeAlignedMS2ConsensSpectrum( IN, consens );
    
    ////////////
    // plot the MS2 spectrum of the aligned fetaure
    if( gnuplot ){
      plotMS2spectra( consens );
      //consens->plotCombinedSpectra();
    }      
    
    consens = NULL;
  }
   */
  
}


///////////////////////////////////////////////////////////////
// store the aligned consensus spectrum together with its feature
void MS2quantificationController::storeAlignedMS2ConsensSpectrum( feature* IN, AlignedMS2ConsensusSpectrum* consens ){
  alignedConsensMS2.push_back( make_pair( *consens, IN ) );
}




    
////////////////////////////////////////////////////
// write out to a file:
void MS2quantificationController::writeFragmentTracetextFile( ){

  
  // get the anaylsis directory:
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  string SOURCE_DIR = "ANALYSIS_" + def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;
  //////////////////////////////////
  
  if( SOURCE_DIR[SOURCE_DIR.size() -1 ] != '/' ){
    SOURCE_DIR += "/";
  }
  string file_name = SOURCE_DIR;
  file_name += MasterMap->get_spec_name() + "_MS2ConsenusTraces.txt";
  
  // open a file, etc:
  ofstream writer;
  writer.open(file_name.c_str(), ofstream::out | ofstream::trunc); 
  
  if(writer.good()){
   
    // print the header:
    printMS2TraceHeader( &writer );
    
    
    // write out the aligned ms2 traces of features:
    vector< pair<AlignedMS2ConsensusSpectrum , feature*> >::iterator P = alignedConsensMS2.begin();
    while( P != alignedConsensMS2.end() ) {
      print_2_file(&writer, &((*P).first));
      P++;
    }
    
    
  }
  else{
    printf("\nERROR: MS2 trace file could not write out to '%s', MS2QuantificationConrtroller::183\n",file_name.c_str());
  }
  
}



//////////////////////////////////////////////////
// writes the header:
void MS2quantificationController::printMS2TraceHeader( ofstream* WRITER ){
  
  char buffer[512];
  string SEP = "\t";
  
  
  // LC-MS overview;
  sprintf(buffer, "\t MastreMap contained LC-MS runs:\n");     
  WRITER->write(buffer,strlen(buffer));  
  
  map<int, string>::iterator M = MasterMap->get_raw_spec_name_start();
  while( M != MasterMap->get_raw_spec_name_end() ){
    sprintf(buffer, "\t\t\t- %s [id=%d]\n", M->second.c_str(), M->first);     
    WRITER->write(buffer,strlen(buffer));  
    M++;
  }
  sprintf(buffer, "\n\n");     
  WRITER->write(buffer,strlen(buffer));  
  
  
  // tabel header
  sprintf(buffer, "\nMS1mz%sTr%sz%sMS2mz%s",SEP.c_str(),SEP.c_str(),SEP.c_str(),SEP.c_str());   
  WRITER->write(buffer,strlen(buffer));  

  // lc-ms runs:
  M = MasterMap->get_raw_spec_name_start();
  while( M != MasterMap->get_raw_spec_name_end() ){
    sprintf(buffer, "%d%s", M->first, SEP.c_str());     
    WRITER->write(buffer,strlen(buffer));  
    M++;
  }
  
  sprintf(buffer, "\n");     
  WRITER->write(buffer,strlen(buffer));  
  
  
  WRITER = NULL;
}


//////////////////////////////////////////////////
// writes out to a file:
void MS2quantificationController::print_2_file(ofstream* WRITER, AlignedMS2ConsensusSpectrum* IN){
  
  char AMRT_buffer[256];
  string SEP = "\t";
  
  /////////////////////////////////////////
  // construct the general AMRT info:
  sprintf(AMRT_buffer,"%0.4f%s",IN->getPrecursorMZ() ,SEP.c_str()); 
  sprintf(AMRT_buffer,"%s%0.2f%s", AMRT_buffer, IN->getTR(),SEP.c_str()); 
  sprintf(AMRT_buffer,"%s+%d%s", AMRT_buffer, IN->getPrecursorChrg() ,SEP.c_str());
  
  
  // try to map the fragment to an existing trace:
  map<double,  profileFragmentTrace>::iterator P = IN->getAlignedMS2FragmentPeakStart();
  while( P != IN->getAlignedMS2FragmentPeakEnd() ){
    
    char buffer[512];
    sprintf(buffer,"%s%0.4f%s", AMRT_buffer, P->first , SEP.c_str()); 
    
    map<int, string>::iterator M = MasterMap->get_raw_spec_name_start();
    while( M != MasterMap->get_raw_spec_name_end() ){
      
      map<int, MS2Fragment*>::iterator F = P->second.find( M->first );
      if( F != P->second.end() ) {
        sprintf(buffer,"%s%0.2f%s", buffer, F->second->getFragmentPeakArea() , SEP.c_str());         
      }
      else{
        sprintf(buffer,"%s%s", buffer, SEP.c_str());         
      }
      
      
      M++;
    }
    
    // make new line:
    sprintf(buffer,"%s\n", buffer);         
    // write to file now:
    WRITER->write(buffer,strlen(buffer));
    
    
    P++; 
  }
}




/////////////////////////////////////////////////////////////////////////////
// plot the MS2 spectrum of the aligned fetaure
void MS2quantificationController::plotMS2spectra( AlignedMS2ConsensusSpectrum* IN ){
  
  map<int, MS2ConsensusSpectrum* >::iterator P = IN->getConsensSpectraStart();
  while( P != IN->getConsensSpectraEnd()){
  
    char buffer[255];
    sprintf( buffer, "%0.2f_%s_MS2spec", P->second->getPrecursorMZ(), MasterMap->get_raw_spec_name( P->first).c_str() );
    P->second->plotSpectrum( buffer );
    
    P++;
  }
}

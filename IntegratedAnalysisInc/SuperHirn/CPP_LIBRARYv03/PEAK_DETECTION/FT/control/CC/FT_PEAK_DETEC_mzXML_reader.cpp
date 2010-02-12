///////////////////////////////////////////////////////////////////////////
//
//  PEAK DETECTION OF FOURIER TRANSFORME MS INSTRUMENT DATA
//
//  by Lukas Mueller, Lukas.Mueller@imsb.biol.ethz.ch
//  October 2005
//
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//

//
// **********************************************************************//
// CLASS file reader:
// provides function to open /reade / modify and close text files
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#define USE_PROCESS_DATA
#define USE_RAW_DATA
#define USE_CENTROID_DATA
#define USE_DEISOTOPE_DATA
#define USE_FT_PEAK_DETEC_MZXML_READER


#include "FT_PEAK_DETECTION_USE.h"


vector<double> FT_PEAK_DETEC_mzXML_reader::FRAGMENT_MASS_SCAN_LEVELS;
vector<double> FT_PEAK_DETEC_mzXML_reader::PEAK_EXTRACTION_SCAN_LEVELS;
bool FT_PEAK_DETEC_mzXML_reader::MS2_PEAK_PROCESSING = false;
int FT_PEAK_DETEC_mzXML_reader::MS1_base_inter_scan_distance;
int FT_PEAK_DETEC_mzXML_reader::MS2_base_inter_scan_distance;
double FT_PEAK_DETEC_mzXML_reader::TR_MIN;
double FT_PEAK_DETEC_mzXML_reader::TR_MAX;

////////////////////////////////////////////////
// constructor for the object ana_summarizer:
FT_PEAK_DETEC_mzXML_reader::FT_PEAK_DETEC_mzXML_reader(){
  
  MS1_LC_MS_DATA_PROCESSOR = NULL;
  MS2_LC_MS_DATA_PROCESSOR = NULL;
  
  // initialize the variables
  index_offset = 0;
  scan_index = NULL;
  total_scan = 0;
  nbMS2Scans = 0;

}

//////////////////////////////////////////////////
// class desctructor
FT_PEAK_DETEC_mzXML_reader::~FT_PEAK_DETEC_mzXML_reader(){

  if(ramp_file_Struct->fileHandle != NULL){
    close_mzxml_file();
    ramp_file_Struct->fileHandle = NULL;
  }
  
  if(MS1_LC_MS_DATA_PROCESSOR != NULL ){
    delete MS1_LC_MS_DATA_PROCESSOR;
    MS1_LC_MS_DATA_PROCESSOR = NULL;
  }
  if(MS2_LC_MS_DATA_PROCESSOR != NULL ){
    delete MS2_LC_MS_DATA_PROCESSOR;
    MS2_LC_MS_DATA_PROCESSOR = NULL;
  }
  
}

//////////////////////////////////////////////////
// set the current mzXML file;
void FT_PEAK_DETEC_mzXML_reader::open_mzxml_file(){
  
  if(!get_current_file().empty()){
    
    //file_handler = (MZXML_FILE) fopen( get_current_file().c_str(),"r");
    ramp_file_Struct = rampOpenFile( get_current_file().c_str() );
    if(ramp_file_Struct->fileHandle != NULL){
      
      // set also the ramp file handler:
      // ramp_file_Struct.fileHandle = file_handler;
      // ramp_file_Struct.bIsMzData = 0;
      
      set_current_indexes();
      
      if( MS1_LC_MS_DATA_PROCESSOR != NULL){
        delete MS1_LC_MS_DATA_PROCESSOR;
        MS1_LC_MS_DATA_PROCESSOR = NULL;
      }
      
      if( MS2_LC_MS_DATA_PROCESSOR != NULL){
        delete MS2_LC_MS_DATA_PROCESSOR;
        MS2_LC_MS_DATA_PROCESSOR = NULL;
      }
      
      // MS1 level processor:
      MS1_LC_MS_DATA_PROCESSOR = new Process_Data( );
      // MS1 level processor:
      MS2_LC_MS_DATA_PROCESSOR = new MS2_Process_Data( );
    }
    else{
      printf("\nERROR:: mzXML file %s could not be opened,mzXML_manager::open_mzxml_file()\n",get_current_file().c_str());
    }
  }
}

//////////////////////////////////////////////////
// close the current mzXML file;
void FT_PEAK_DETEC_mzXML_reader::close_mzxml_file(){
  
  if(ramp_file_Struct->fileHandle != NULL){
    fclose( (FILE*) ramp_file_Struct->fileHandle);    
  }
  
}

//////////////////////////////////////////////////
// set indexes of the current mzXML file;
void FT_PEAK_DETEC_mzXML_reader::set_current_indexes(){
  
  if(ramp_file_Struct->fileHandle != NULL){
        
    // get index offset::
    index_offset = getIndexOffset(  get_Ramp_file_handler() );
    
    // index vector::
    scan_index = readIndex( get_Ramp_file_handler() , get_index_off_set(), &total_scan);
    
    // get min / max retention time in mzXML:
    // read the mzXML header at this scan
    struct ScanHeaderStruct scan_header;
    // start:
    readHeader( get_Ramp_file_handler(), get_scan( 0 ), &scan_header);
    minRT = scan_header.retentionTime / 60.00;
    readHeader( get_Ramp_file_handler(), get_scan( total_scan ), &scan_header);
    maxRT = scan_header.retentionTime / 60.00;
    ExternalIsotopicDistribution::initRetentionTimeSegments( minRT, maxRT );

  }
}





//////////////////////////////////////////////////
// reads the ms data from a mzXML file opened by teh handler
void FT_PEAK_DETEC_mzXML_reader::read_mzXML_DATA(){
  
  // check all the scans of the current region:
  off_t scan = 0;
  previousMSnScans.clear();
  off_t MAX_SCAN = total_scan;

  progress_bar bar(total_scan,"read");
  
  while(scan <= MAX_SCAN){
    // extract ms info within a mass range of a scan
    get_MS_scan(scan);
    scan++;
    bar.update_progress();
  }
}



//////////////////////////////////////////////////
// get a MS scan at a given scan number within
// a mass range
void FT_PEAK_DETEC_mzXML_reader::get_MS_scan(off_t currentMSScan){

  if(currentMSScan < get_total_scan()){

    struct ScanHeaderStruct scan_header;
    
    // read the mzXML header at this scan
    readHeader( get_Ramp_file_handler(), get_scan( currentMSScan ), &scan_header);
    
    ///////////////////////
    // get retention time:
    double TR = scan_header.retentionTime / 60.00;

    /////////////////////////////////////
    // check if this scan (MSx) is in the TR range
    // defned by max / min TR:
    // should have some peaks!
    if( (TR >= TR_MIN) && ( TR <= TR_MAX ) &&( scan_header.peaksCount > 1 ) ){
      
      // object to store the MS scan information:
      Generic_MS_Scan* thisMSScan = new Generic_MS_Scan( currentMSScan, TR, scan_header.precursorMZ, scan_header.precursorCharge, scan_header.msLevel);
            
      //////////////////////////////////////////////////////
      // check here the MS Precursor Mass Spectrum Level 
      if( checkMSPrecursorMassScan( scan_header.msLevel ) ){
        
        // process the data:
        processMS1InputData( thisMSScan );
      }
      
      //////////////////////////////////////////////////////
      // check here the Fragment Spectrum Level 
      if( checkMSFragmentMassScan( scan_header.msLevel ) ){
        
        // find the corresponding MS1 scan and retention time:
        map<int, pair<int, double> >::iterator F = previousMSnScans.find( 1 );
        if( F != previousMSnScans.end() ){
          thisMSScan->setScanPrecursor( F->second.first );          
          thisMSScan->setRTPrecursor( F->second.second );          
        }
        
        // process the data:
        processMS2InputData( thisMSScan );
        
      }
      
      // insert into the scan map the current scan:
      previousMSnScans.erase( scan_header.msLevel );
      previousMSnScans.insert( make_pair( scan_header.msLevel, make_pair( currentMSScan, TR) ) );
      
    
    }
  }
}


///////////////////////////////////////////////////////////////////////////////////////
// process the MS1 level input data:
// - construct a RawData object with input peaks
// - centoid them
// - add them to Process_Data Structure:
void FT_PEAK_DETEC_mzXML_reader::processMS1InputData( Generic_MS_Scan* MS1Scan ){

  
  //////////////////////////////////
  // read the mass/intensity signals
  MY_RAMP_PEAKS* ms_peaks = (MY_RAMP_PEAKS*) readPeaks( get_Ramp_file_handler(), get_scan( MS1Scan->getMSScanNumber() ) );
  MS1Scan->setRampMSPeakList( ms_peaks );
  ms_peaks = NULL;
  
  //////////////////////////////////
  // bool debug = (FT_PEAK_DETEC_mzXML_reader::sfReportMonoPeaks ==1 && SCAN== FT_PEAK_DETEC_mzXML_reader::sfReportScanNumber);
  IsotopicDist::setDebug(false);
  
  //////////////////////////////////
  // construct a raw_data
  RawData rawData( MS1Scan->getRampMSPeakList( ), CentroidData::sfIntensityFloor);
  
  //////////////////////////////////
  // centroid it:
  // CentroidData cd(CentroidPeak::sfCentroidWindowWidth, data, Process_Data::CENTROID_DATA_MODUS);
  CentroidData centroidData(CentroidPeak::sfCentroidWindowWidth, rawData, MS1Scan->getMSScanRetentionTime(), Process_Data::CENTROID_DATA_MODUS);
  
  //////////////////////////////////
  //  store it:
  MS1_LC_MS_DATA_PROCESSOR->add_scan_raw_data( MS1Scan, &centroidData);
  
  delete MS1Scan;
  MS1Scan = NULL;      
  
 }


///////////////////////////////////////////////////////////////////////////////////////
// process the MS2 level input data:
// - construct a RawData object with input peaks
// - centoid them
// - add them to Process_Data Structure:
void FT_PEAK_DETEC_mzXML_reader::processMS2InputData( Generic_MS_Scan* MS2Scan){
                            
  
  //////////////////////////
  // go back to the MS1 level and
  // find the correct precursor mass by mz and z:
  // precursorScan = getPreviousMSXScan( SCAN, 1);
  findMS1PrecursorData( MS2Scan );
  
  // only extract Fragment Mass spectra if this option is activated:
  if( MS2_PEAK_PROCESSING ){
    
    //////////////////////////////////
    // read the mass/intensity signals
    MY_RAMP_PEAKS* ms_peaks = (MY_RAMP_PEAKS*) readPeaks( get_Ramp_file_handler(), get_scan( MS2Scan->getMSScanNumber() ) );
    MS2Scan->setRampMSPeakList( ms_peaks );
    ms_peaks = NULL;
    
    IsotopicDist::setDebug(false);
    
    // construct a raw_data
    RawData rawData(MS2Scan->getRampMSPeakList( ), MS2_Process_Data::MS2_INTENSITY_THRESHOLD);
    
    // centroid it:
    CentroidData centroidData( 4 , rawData, MS2_Process_Data::CENTROID_DATA_MODUS);
    
    // add the MS2 scan fragments:
    MS2_LC_MS_DATA_PROCESSOR->add_scan_MS2_raw_data( MS2Scan, centroidData);
    
  }
  
  if( MS2Scan->getPrecursorFoundState() ){
    delete MS2Scan;
    MS2Scan = NULL;      
  }
  
  
  // MS2 scan counter:
  nbMS2Scans++;
}


///////////////////////////////////////////////////////////////////////////////////////
 // converts DeconvPeak list to ms_peak vector
void FT_PEAK_DETEC_mzXML_reader::convert_ms_peaks(int SCAN,double TR, list<DeconvPeak>& DECONVPEAK,vector<ms_peak>& MSPEAK){
  
  list<DeconvPeak>::iterator mpi;
  for (mpi=DECONVPEAK.begin();mpi!=DECONVPEAK.end();++mpi) {
    ms_peak peak(SCAN,mpi->getMass(),mpi->getIntensity(),mpi->getCharge(),mpi->getNrIsotopes(),mpi->getScore(),mpi->getIsotopicPeaks());
    peak.set_retention_time( TR );
    MSPEAK.push_back(peak);
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// delete existing debug files before writing in append mode
void FT_PEAK_DETEC_mzXML_reader::delete_existing_debug_file()
{
  /*
	if (PeakDetectGlobals::sfReportMonoPeaks == 1) {
		string str = get_current_file();
		str.erase(0,str.find_last_of('/')+1);
		str.replace(str.find(".mzXML"),6,"_mono_peaks.txt");
		str = PeakDetectGlobals::sfDebugDirectory + str;
		ofstream outd(str.c_str(),ios::trunc);
	}
   */
}


/////////////////////////////////////////////////////////////////////////
// set the maximal inter-monoisotopic distance
// for the same LC-elution peak
int FT_PEAK_DETEC_mzXML_reader::setInterMonoIsotopicLCDistance( int my_scan, int level, int max_inter_scan_distance){
  
  if( my_scan == 0 ){
    return 0;
  }
  
  // check how many ms scan of not this level have been 
  // performed since the last MS scan on the input level:
  struct ScanHeaderStruct scan_header;
  int tmp_scan = my_scan - 1;
  int nonMSLevelCount = 0;
  int MSLevelCount = 0;
  readHeader( get_Ramp_file_handler(), get_scan( tmp_scan ), &scan_header);
  while( MSLevelCount < max_inter_scan_distance){
    
    // count the MS scans of this level:
    nonMSLevelCount++;

    // count the MS scans of this level:
    if( scan_header.msLevel == level){
      MSLevelCount++;
    }     
    tmp_scan--;
    
    // dont go below 1
    if( tmp_scan < 1){
      break;
    }
    
    readHeader( get_Ramp_file_handler(), get_scan(tmp_scan), &scan_header);
  }
  
  return nonMSLevelCount;
}


/////////////////////////////////////////////////////////////////////////
// get a previous scan at the MS level from the current scan
int FT_PEAK_DETEC_mzXML_reader::getPreviousMSXScan( int CurrentScan, int level){
  
  if( CurrentScan == 0 ){
    return 0;
  }
  
  // check how many ms scan of not this level have been 
  // performed since the last MS scan on the input level:
  struct ScanHeaderStruct scan_header;
  CurrentScan--;
  while( CurrentScan > 0){

    readHeader( get_Ramp_file_handler(), get_scan( CurrentScan ), &scan_header);
    // count the MS scans of this level:
    if( scan_header.msLevel == level){
      return CurrentScan;
    }     
    CurrentScan--;    
  }
  
  return 0;
}



/////////////////////////////////////////////////////////////////////////
// set the maximal inter-monoisotopic distance
// for the same LC-elution peak
int FT_PEAK_DETEC_mzXML_reader::setInterMonoIsotopicLCDistance( int my_scan, int level, int max_inter_scan_distance, double precourserIon){
  
  if( my_scan == 0 ){
    return 0;
  }
  
  // check how many ms scan of not this level have been 
  // performed since the last MS scan on the input level:
  struct ScanHeaderStruct scan_header;
  int tmp_scan = my_scan - 1;
  int nonMSLevelCount = 0;
  int MSLevelCount = 0;
  readHeader( get_Ramp_file_handler(), get_scan( tmp_scan ), &scan_header);
  while( MSLevelCount < max_inter_scan_distance){
    
    // count the MS scans of this level:
    nonMSLevelCount++;
    
    // precursor mass difference:
    double precursorDelta = fabs(precourserIon - scan_header.precursorMZ); 
    
    // count the MS scans of this level:
    // only if the same precursor:
    if(( scan_header.msLevel == level) && (precursorDelta <= MS2_Process_Data::PRECURSOR_MZ_TOLERANCE) ){
      MSLevelCount++;
    }     
    tmp_scan--;
    
    // dont go below 1
    if( tmp_scan < 1){
      break;
    }
    
    readHeader( get_Ramp_file_handler(), get_scan(tmp_scan), &scan_header);
  }
  
  return nonMSLevelCount;
}



///////////////////////////////////////////////////////////////////////////////
// print the scan header:
void FT_PEAK_DETEC_mzXML_reader::print_scan_header(ScanHeaderStruct* scan_header ){
  
  printf("\tlevel=%d, peakCount=%d, tr=%0.2f\n", scan_header->msLevel, scan_header->peaksCount, scan_header->retentionTime/60.0);
  
}



///////////////////////////////////////////////////////////////////////////////
// go back to the MS1 level and
// find the correct precursor mass by mz and z:
void FT_PEAK_DETEC_mzXML_reader::findMS1PrecursorData( Generic_MS_Scan* MS2Scan){
  MS1_LC_MS_DATA_PROCESSOR->adjustCorrectToMS1Precursor( MS2Scan );
}


///////////////////////////////////////////////////////////////////////////////
// check if the scan number should be processed by MS Precursor Mass extraction
bool FT_PEAK_DETEC_mzXML_reader::checkMSPrecursorMassScan( int scanLevel ){
  
  vector<double>::iterator F = find(PEAK_EXTRACTION_SCAN_LEVELS.begin(), PEAK_EXTRACTION_SCAN_LEVELS.end() , (double)scanLevel);
  if( F == PEAK_EXTRACTION_SCAN_LEVELS.end() ){
    return false;
  }
  
  if( *F != scanLevel ){
    return false;
  }
  
  return true;
  
}


///////////////////////////////////////////////////////////////////////////////
// check if the scan number should be processed as MSn FragmentMass spectrum
bool FT_PEAK_DETEC_mzXML_reader::checkMSFragmentMassScan( int scanLevel ){
   
  vector<double>::iterator F = find(FRAGMENT_MASS_SCAN_LEVELS.begin(), FRAGMENT_MASS_SCAN_LEVELS.end() , (double)scanLevel);
  if( F == FRAGMENT_MASS_SCAN_LEVELS.end() ){
    return false;
  }
  
  if( *F != scanLevel ){
    return false;
  }
  
  return true;
}




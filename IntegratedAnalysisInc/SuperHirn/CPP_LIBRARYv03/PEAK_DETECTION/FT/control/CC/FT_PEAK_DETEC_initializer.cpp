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



#define USE_FT_PEAK_DETEC_MZXML_READER
#define USE_FT_PEAK_DETEC_PLOTTER
#define USE_PROCESS_DATA
#define USE_LCMSCDATA
#define USE_LC_ELUTION_PEAK
#define USE_CENTROID_DATA
#define USE_DEISOTOPE_DATA
#define USE_MS_PEAK
#define USE_PROCESS_DATA
#define USE_RAW_DATA
#define USE_FT_PEAK_DETEC_INITIALIZER

#include "FT_PEAK_DETECTION_USE.h"




////////////////////////////////////////////////
// constructor for the object initializer:
FT_PEAK_DETECT_initializer::FT_PEAK_DETECT_initializer(){
}

//////////////////////////////////////////////////
// class desctructor of initializer
FT_PEAK_DETECT_initializer::~FT_PEAK_DETECT_initializer(){
}

//////////////////////////////////////////////////
// class copy constructor of initializer
FT_PEAK_DETECT_initializer::FT_PEAK_DETECT_initializer(const FT_PEAK_DETECT_initializer& tmp){
}


//////////////////////////////////////////////////
// copy constructor:
FT_PEAK_DETECT_initializer& FT_PEAK_DETECT_initializer::operator=(const FT_PEAK_DETECT_initializer& tmp){

    return *this;
}

//////////////////////////////////////////////////
// initialize all objects!
void FT_PEAK_DETECT_initializer::init_all(){

  double DB;
  int INT;
  bool TMP_B;
  vector<double> vInt;
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  
  
  
  /////////////////////////////////////////
  // Lukas parameters for the 
  // detection of MS1 features:
  
  // start/end of the retention time window
  def->search_tag("RT start elution window", &DB);
  FT_PEAK_DETEC_mzXML_reader::TR_MIN = DB;
  def->search_tag("RT end elution window", &DB);
  FT_PEAK_DETEC_mzXML_reader::TR_MAX = DB;
  
  // check which MS levels to use for peak extraction: 
  def->search_tag("Precursor detection scan levels", &vInt);
  FT_PEAK_DETEC_mzXML_reader::PEAK_EXTRACTION_SCAN_LEVELS = vInt;
  
  // if mzXML in centroid mode on MS1 level
  // -> turns off the centroiding 
  def->search_tag("MS1 data centroid data", &TMP_B);
  Process_Data::CENTROID_DATA_MODUS = TMP_B; 
  
  // if mono isotopic peaks which do not fullfill the time cluster criteria (i.e. how many times detected)
  // but have been selected for MS/MS are kept or discarded
  def->search_tag("Save MS/MS sequenced MS1 monoisotopic peaks", &TMP_B);
  Process_Data::KEEP_MS2_SELECTED_PRECURSOR_SIGNALS = TMP_B;  
  
  // maximal scan distance between scan of the same MS level
  def->search_tag("MS1 max inter scan distance", &INT);
  FT_PEAK_DETEC_mzXML_reader::MS1_base_inter_scan_distance = INT;  
  // time resolution to integrate peak areas 
  def->search_tag("MS1 LC retention time resolution", &DB);
  Process_Data::MS1_TR_RESOLUTION = DB;
  
  // external isotopic distribution xml file
  ExternalIsotopicDistribution::XMLInputFile = def->search_tag("MS1 external isotopic distribution file");
  ExternalIsotopicDistribution::parseExternalIsotopicProfiles( );
  // PPM tolerance to match precursor with external isotopic distribution:
  def->search_tag("External isotope distribution mono isotope PPM tolerance", &DB);
  ExternalIsotopicDistribution::EXTERNAL_DISTRIBUTION_MONO_ISOTOPE_PPM_TOLERANCE = DB;
  
  ////////////////////////////////////////////////////
  // markus centroiding and deisotoping parameters:
  def->search_tag("Centroid window width",&INT);
  CentroidPeak::sfCentroidWindowWidth = INT;
  def->search_tag("Absolute isotope mass precision",&DB);
  CentroidData::sfMassTolDa = DB;
  def->search_tag("Relative isotope mass precision",&DB);
  CentroidData::sfMassTolPpm = DB;
  def->search_tag("Minimal peak height",&DB);
  CentroidData::sfMinIntensity = DB;
  def->search_tag("Min. RAW MS Signal Intensity",&DB);
  CentroidData::sfIntensityFloor = DB;
  
  def->search_tag("MS1 feature CHRG range min", &INT);
  Deisotoper::sfMinCharge = INT;
  def->search_tag("MS1 feature CHRG range max", &INT);
  Deisotoper::sfMaxCharge = INT;
  def->search_tag("Detectable isotope factor",&DB);
  IsotopicDist::sfDetectableIsoFact = DB;
  def->search_tag("IntensityCV",&DB);
  IsotopicDist::sfIntensityCV = DB;

  
  ////////////////////////////////////////////////////
  // parameters for the MS2 peak picking:
  // check activated at all:
  def->search_tag("MS2 fragment mass tracing", &TMP_B);
  FT_PEAK_DETEC_mzXML_reader::MS2_PEAK_PROCESSING = TMP_B;

  def->search_tag("Post MS/MS spectra clustering", &TMP_B);
  MS2_Process_Data::POST_MSMS_SPECTRUM_CLUSTERING = TMP_B;
  
  // check which MS levels to use for peak extraction: 
  def->search_tag("Fragment Mass Scan levels", &vInt);
  FT_PEAK_DETEC_mzXML_reader::FRAGMENT_MASS_SCAN_LEVELS = vInt;  
  
  // ms2 tolerance:
  def->search_tag("FT peak detect MS2 m/z tolerance", &DB);
  MS2_feature::MS2_MZ_TOLERANCE=DB;
  
  
  // if mzXML in centroid mode on MS2 level
  // -> turns off the centroiding 
  def->search_tag("FT MS2 data centroid data", &TMP_B);
  MS2_Process_Data::CENTROID_DATA_MODUS = TMP_B;  
  def->search_tag("MS/MS max inter scan trace distance", &INT);
  FT_PEAK_DETEC_mzXML_reader::MS2_base_inter_scan_distance = INT;  
  def->search_tag("N-th highest fragment in MS2 spectrum", &INT);
  MS2SpectrumProcessor::highestOrder = INT;  
  def->search_tag("MS2 intensity apex percentil cutoff", &DB);
  MS2FragmentTrace::APEX_PERCENTILE = DB;
  def->search_tag("MS2 LC retention time resolution", &DB);
  MS2_Process_Data::MS2_TR_RESOLUTION = DB;
  def->search_tag("MS2 LC APEX correlation retention time tolerance", &DB);
  MS2_Process_Data::MS2_TR_LC_CORRELATION_TOLERANCE_APEX = DB;
  def->search_tag("MS2 LC BORDER correlation retention time tolerance", &DB);
  MS2_Process_Data::MS2_TR_LC_CORRELATION_TOLERANCE_BORDER = DB;
  def->search_tag("Min MS2 Fragment Intensity Area threshold", &DB);
  MS2_Process_Data::MS2_FRAGMENT_MIN_INTENSITY_AREA_THRESHOLD = DB;
  

  delete def;
  def = NULL;
  
  IsotopicDist::init();
  

}

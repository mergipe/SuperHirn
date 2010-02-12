///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS param_initializer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_PARAM_INITIALIZER
#include "use.h"




param_initializer::param_initializer(){
}

param_initializer::~param_initializer(){
}
param_initializer::param_initializer(const param_initializer& tmp){
}

param_initializer::param_initializer(const param_initializer* tmp){
}

param_initializer& param_initializer::operator=(const param_initializer& tmp){
  return *this;
}

bool param_initializer::check_ROOT_PARAM( ){
  // read parameters from "param.def"
  read_param* def = new read_param();
  string ROOT_PARAM = def->search_tag("ROOT PARAMETER FILE" );
  delete def;
  def = NULL;
 
  // check if it is a a file:
  file_sys TEST;
  
  if( !TEST.check_file_exists_in_Dir( ROOT_PARAM , ".def") ){
    printf("\n *** \n ERROR:: root parameter file not found at '%s' \n *** \n",  ROOT_PARAM.c_str() ); 
    return false;
  }
  return true;
  
}


void param_initializer::INIT_ALL(){
  
  // read parameters from "param.def"
  read_param* def = new read_param();
  double TMP = 0;
  bool TMP_B;
  int TMP_I;
  
    // TR min:
  def->search_tag("RT start elution window", &TMP);
  FT_PEAK_DETEC_mzXML_reader::TR_MIN=TMP;
  // TR max:
  def->search_tag("RT end elution window", &TMP);
  FT_PEAK_DETEC_mzXML_reader::TR_MAX=TMP;
  
  // feature parameters:
  def->search_tag("MS1 retention time tolerance", &TMP);
  feature::TR_TOL = TMP;
  def->search_tag("MS1 m/z tolerance", &TMP);
  feature::PPM_MZ_TOL = TMP;
  consensIsotopePattern::FT_MZ_TOLERANCE = feature::PPM_MZ_TOL;
  
  // MS2_M2_matcher parameters:
  def->search_tag("MS2 mass matching modus", &TMP_B);
  ms2_info::THEO_MATCH_MODUS = TMP_B;
  def->search_tag("Peptide Prophet Threshold", &TMP);
  MS2_MS1_matcher::PEP_PROB_CUT_OFF = TMP;
  def->search_tag("MS2 SCAN tolerance", &TMP_I);
  MS2_MS1_matcher::SCAN_TOL = TMP_I;
  def->search_tag("INCLUSIONS LIST MS2 SCAN tolerance", &TMP_I);
  MS2_MS1_matcher::POST_SCAN_TOL = TMP_I;
  
  // MS2 matching PPM parameters:
  def->search_tag("MS2 PPM m/z tolerance", &TMP);
  ms2_info::MS2_MZ_PPM_TOLERANCE = TMP;
  
  // MS2 retention time tolerance:
  def->search_tag("MS2 retention time tolerance", &TMP);
  if( TMP > 0 ){
    ms2_info::MS2_TR_TOL = TMP; 
  }
  else{
    ms2_info::MS2_TR_TOL = feature::TR_TOL;
  }
  
  // peptide_DELTA_group:
  def->search_tag("MS1 retention time tolerance", &TMP);
  peptide_DELTA_group::TR_TOL = TMP;
  def->search_tag("Delta pair TR tolerance", &TMP);  
  peptide_DELTA_group::MOD_TR_TOL = TMP;
  // if smaller than -1, then use the general TR tolerance:
  if( peptide_DELTA_group::MOD_TR_TOL < peptide_DELTA_group::TR_TOL ){
    peptide_DELTA_group::MOD_TR_TOL = peptide_DELTA_group::TR_TOL;
  }
  def->search_tag("Delta pair TR tolerance", &TMP);  
  
  
  // protein group:
  def->search_tag("Peptide Proteotype Mode", &TMP_B);
  protein_group::PEPTIDE_PROTEOTYPE_MODE = TMP_B;
    
  
  def->search_tag("Peptide Prophet Threshold", &TMP);
  peptide_DELTA_group::PEPTIDE_PROBABILITY_THRESHOLD = TMP;
  feature::PEPTIDE_PROBABILITY_THRESHOLD = TMP;
  interact_parser::PEPTIDE_PROBABILITY_THRESHOLD = TMP;
  LC_MS::PEP_PROPHET_THERSHOLD = TMP;
  
  // slide window over TR
  map<int, vector<double> > TMP2;
  def->search_tag("Delta M/z list", &TMP2);
  peptide_DELTA_group::LC_MS_Modification_Masses = TMP2; 
  MS1_feature_ratiolizer::LC_MS_Modification_Masses = TMP2;
  peptide_ratio_analyzer::LC_MS_Modification_Masses = TMP2;

  // DELTA Pair MATCHING
  def->search_tag("MS2 Delta clustering filter", &TMP_B);
  DELTA_grouper::MS_2_SELECTION = TMP_B;
  
  
  // STATIC MODIFICATIONS:
  def->search_tag("static glyco modifictation", &TMP_B);
  interact_parser::STATIC_GLYCOSYLATION_MOD = TMP_B;
  def->search_tag("static C-term. modification", &TMP);
  interact_parser::STATIC_C_TERM_MODIFICATION = TMP;
  map<double,double> M_TMP;
  def->search_tag("INTERACT AA MOD transform table", &M_TMP);
  interact_parser::MOD_MASS_TRANSFORM_TABLE = M_TMP;
  def->search_tag("MS2 mass type check", &TMP_B);
  interact_parser::MS2_MASS_COMPARE = TMP_B;
  
  //////////////////
  // log modus or not:
  bool LOG_MODE_INTENSITY = false;
  consens_profile_builder::LOG_INTENSITY_TRANSFORMATION = LOG_MODE_INTENSITY;
  profile_scorer::LOG_INTENSITY_TRANSFORMATION = LOG_MODE_INTENSITY;

  
  /////////////////////////
  // LC-MS correlation:
  
  def->search_tag("pairwise correlation analysis", &TMP_B);
  LC_MS_correlation::VIEW = TMP_B;
  
  // Tr tolerance used to compare
  // spec A to B after smoothing and
  // also used in the merging
  def->search_tag("MS1 retention time tolerance", &TMP);
  LC_MS_correlation::tr_tol = TMP;
  
  // M/z tolerance used to compare
  // spec A to B after smoothing and
  // also used in the merging
  def->search_tag("MS1 m/z tolerance", &TMP);
  LC_MS_correlation::mz_tol = TMP;
  
  // numbers of bins to divide the peak areas
  // used afterwards to compare peak areas of 2 peaks
  def->search_tag("intensity bin size", &TMP_I);
  LC_MS_correlation::intensity_bin_size = TMP_I;
  
  // minimal alignment error = TR tolerance / 2:
  def->search_tag("MS1 retention time tolerance", &TMP);
  TMP /= 2.0;
  LC_MS_correlation::min_align_error = TMP;
  
  // maximal alignment error:
  def->search_tag("maximal smoothing error", &TMP);
  LC_MS_correlation::max_align_error = TMP;
  
  // tolerance of bins, how far 2 bins can be apart and still
  // be a match
  def->search_tag("intensity bin tolerance", &TMP);
  LC_MS_correlation::intensity_bin_tolerance = TMP;
  
  // represents the worst score possible, this one will be used to 
  // normaize the observed scores between 0(bad) and 1(good) [ 0 ... 1]
  def->search_tag("minimal LC/MS score", &TMP);
  LC_MS_correlation::min_LC_MS_score = TMP;
  
  
  
  //////////////////////////////////////////////////////////
  // LC_MS alignmnt parameters:

  // retention time tolerance:
  def->search_tag("retention time window", &TMP);
  AmrtpLCMSAligner::Tr_window = TMP;
  
  // mass tolerance:
  def->search_tag("mass / charge window", &TMP);
  AmrtpLCMSAligner::Mz_window = TMP;  
  
  // percentage of outside delta error points
  def->search_tag("perc. outside error delta points", &TMP);
  AmrtpLCMSAligner::ERROR_DELTA_POINTS = TMP;

  // Tr tolerance used to compare
  // spec A to B after smoothing and
  // also used in the merging
  def->search_tag("MS1 retention time tolerance", &TMP);
  AmrtpLCMSAligner::Tr_tolerance = TMP;

  // M/z tolerance used to compare
  // spec A to B after smoothing and
  // also used in the merging
  def->search_tag("MS1 m/z tolerance", &TMP);
  AmrtpLCMSAligner::Mz_tolerance = TMP;
  
  // defines if also identifications should be checked
  // in finding common lc/ms peaks between runs:
  def->search_tag("MS2 info alignment weight", &TMP_B);
  AmrtpLCMSAligner::ID_ALIGNMENT_WEIGHT = TMP_B;
  
  // plotting:
  def->search_tag("gnuplot plot generator", &TMP_B);
  if( TMP_B){
    def->search_tag("pairwise alignment plotting", &TMP_B);
    AmrtpLCMSAligner::print_out = TMP_B;
  }
  
  /////////////////////////////////////////////////
  // these are the important parameters for the
  // for the lowess fitter druing the LC/MS alignment:
    
  // minimal alignment error = TR tolerance / 2:
  def->search_tag("MS1 retention time tolerance", &TMP);
  FeatureMatchRetentionTimeRegressor::min_error = TMP;
  
  // number of boostrape cycles:
  def->search_tag("maximal smoothing error", &TMP);
  FeatureMatchRetentionTimeRegressor::max_error = TMP;
  
  // max stripes for the multi feature match remover:
  def->search_tag("max. nb. stripes", &TMP_I);
  MultipleFeatureMatchRemover::max_nb_stripes = TMP_I;  
  
  // used to copmute the alignment error, use a tr window to
  // calculate the standard deviations of raw data to predicted
  // delta shift
  def->search_tag("smoothing error TR window", &TMP);
  FeatureMatchRetentionTimeRegressor::tr_error_smooth = TMP;
  
    
  /////////////////////////////////////////////////
  // parameter if the LC elution profile will be stored 
  // in the XML:
  def->search_tag("Create monoisotopic LC profile", &TMP_B);
  LCMSDataImporter::CREATE_FEATURE_ELUTION_PROFILES = TMP_B;
  FT_PeakDetectController::CREATE_FEATURE_ELUTION_PROFILES = TMP_B;
  LC_MS_XML_writer::STORE_FEATURE_ELUTION_PROFILES = TMP_B;

  /////////////////////////////////////////////////
  // what and how XML data is stored in the mastermap:
  // ms2 information of a feature:
  // only the best ms2 info / feature stored:
  def->search_tag("store only best MS2 per feature", &TMP_B);
  LC_MS_XML_writer::STORE_BEST_MS2_SCAN_PER_FEATURE = TMP_B;
  // only the best ms2 info / aligned feature stored:
  def->search_tag("store only best MS2 per ALIGNED feature", &TMP_B);
  LC_MS_XML_writer::STORE_BEST_MS2_SCAN_PER_FEATURE = TMP_B;
  // how many alternative protein names to store:
  def->search_tag("nb. max. alternative protein names", &TMP_I);
  LC_MS_XML_writer::MAXIMAL_NB_ALTERNATIVE_PROTEIN_NAMES = TMP_I;
  // if to store ms2 traces
  def->search_tag("MS2 fragment mass tracing", &TMP_B);
  LC_MS_XML_writer::STORE_MS2_FRAGMENT_TRACE_DATA = TMP_B;
  
  
  
  /////////////////////////////////////////////////
  // Parameters for the peak merging:
  def->search_tag("Activation of MS1 feature merging post processing", &TMP_B);
  MS1_feature_merger::MS1_FEATURE_CLUSTERING = TMP_B;
  def->search_tag("MS1 LC retention time resolution", &TMP);
  MS1_feature_merger::MS1_PEAK_AREA_TR_RESOLUTION = TMP;

  def->search_tag("Initial Apex Tr tolerance", &TMP);
  MS1_feature_merger::INITIAL_TR_TOLERANCE = TMP;
  def->search_tag("MS1 feature Tr merging tolerance", &TMP);
  MS1_feature_merger::MS1_FEATURE_MERGING_TR_TOLERANCE = TMP;
  def->search_tag("Percentage of intensity variation between LC border peaks", &TMP);
  MS1_feature_merger::PERCENTAGE_INTENSITY_ELUTION_BORDER_VARIATION = TMP;
  def->search_tag("PPM value for the m/z clustering of merging candidates", &TMP);
  MS1_feature_merger::PPM_TOLERANCE_FOR_MZ_CLUSTERING = TMP;
  
  
  
  /////////////////////////////////////////////////
  // what information is extracted from the LC/MS or mastermap:
  // TR min:
  def->search_tag("RT start elution window", &TMP);
  LC_MS_XML_reader::TR_MIN = TMP;
  interact_parser::TR_MIN = TMP;
  
  // TR max:
  def->search_tag("RT end elution window", &TMP);
  LC_MS_XML_reader::TR_MAX = TMP;
  interact_parser::TR_MAX = TMP;
  
  // mz min.
  def->search_tag("MS1 feature mz range min", &TMP);
  LC_MS_XML_reader::FEATURE_MZ_MIN =TMP;
  interact_parser::FEATURE_MZ_MIN = TMP;

  // mz max.
  def->search_tag("MS1 feature mz range max", &TMP );
  LC_MS_XML_reader::FEATURE_MZ_MAX = TMP;
  interact_parser::FEATURE_MZ_MAX = TMP;

  // signal to noise min.
  def->search_tag("MS1 feature signal to noise threshold", &TMP );
  LC_MS_XML_reader::SIGNAL_TO_NOISE_THERSHOLD = TMP;
  // intensity min.
  def->search_tag("MS1 feature intensity cutoff", &TMP );
  LC_MS_XML_reader::PEAK_INTENSITY_THRESHOLD = TMP;
  // charge state min.
  def->search_tag("MS1 feature CHRG range min", &TMP_I );
  LC_MS_XML_reader::FEATURE_CHRG_MIN = TMP_I;
  interact_parser::FEATURE_CHRG_MIN = TMP_I;

  // charge state max.
  def->search_tag("MS1 feature CHRG range max", &TMP_I );
  LC_MS_XML_reader::FEATURE_CHRG_MAX = TMP_I;
  interact_parser::FEATURE_CHRG_MAX = TMP_I;
  
  
  //  Create monoisotopic LC profile:	to create and store the original profile of the detected
  //					monosiotopic pecursors in the XML (!!! increases the
  //					XML file size!!! (on[1]/off[0])
  def->search_tag("Create monoisotopic LC profile", &TMP_B);
  LC_MS_XML_reader::EXTRACT_MONO_ISOTOPE_PROFILE = TMP_B;
  
  
  /////////////////////////////////////////////////
  // what and how data is stored during superhirn processing:
  // ms2 information of a feature:
  // only the best ms2 info / feature stored:
  def->search_tag("progress all low probability ms2 info in MS1 feature", &TMP_B);
  feature::STORE_ALL_LOW_PROBABILITY_MS2_SCANS = TMP_B;
  // XML Data format to use during SuperHirn processing:
  LC_MS_XML_reader::DATA_STORAGE_XML_FORMAT_TYPE = def->search_tag("SuperHirn Data Storage XML Output Format");

  
  delete def;
  def = NULL;
  
  
  /////////////////////////////////////////////////
  // ms2 fragment tracing:
  MS2_FRAGMENT_TRACE_INITIALIZATION();
  
  //////////////////////////////////////////////////
  // initialze parameters for cpp library environment:
  CPP_LIBRARY_ENVIRONMENT_PARAMETERS();
    
  //////////////////////////////////////////////////
  // initialze parameters peak detection classes:
  // PEAK_DETECTION_CLASS_INITIALIZATION();
}



void param_initializer::MS2_FRAGMENT_TRACE_INITIALIZATION(){
  
  // read parameters from "param.def"
  read_param* def = new read_param();
  double TMP = 0;
  int INT = 0;
  // ms2 tolerance:
  def->search_tag("FT peak detect MS2 m/z tolerance", &TMP);
  MS2_feature::MS2_MZ_TOLERANCE=TMP;
  
  def->search_tag("MS2 intensity apex percentil cutoff", &TMP);
  MS2SpectrumProcessor::apexPercentile = TMP;  

  def->search_tag("N-th highest fragment in MS2 spectrum", &INT);
  MS2SpectrumProcessor::highestOrder = INT;  

  delete def;
  def = NULL;
}


void param_initializer::PEAK_DETECTION_CLASS_INITIALIZATION(){
  FT_PEAK_DETECT_initializer::init_all();
}

void param_initializer::CPP_LIBRARY_ENVIRONMENT_PARAMETERS(){
  
  // read parameters from "param.def"
  read_param* def = new read_param();
  
  LC_MS_similarity_matrix::PerlPathName = def->search_tag("LC-MS sim. matrix perl plotting path");
    
  delete def;
  def = NULL;
}



void param_initializer::DEFINE_COSTUM_PARAM(string in){
  read_param::COSTUM_PARAM_FILE = in;
}



void param_initializer::EXTRACT_ARGUMENT_PARAMETERS(string in)
{
  // argument consists of key/value pairs separated by a space:
  // example: key1(value1)key2(value2)etc.
  // parse these here and set in read_param class as global variables: 
  string sepStartValue("(");
  string sepEndValue(")");
  
  while( in.size() > 0 )
    {
      // key:
      string key = in.substr( 0, in.find( sepStartValue ) ); 
      in.erase( 0, in.find( sepStartValue ) + sepStartValue.size() ); 
      string value = in.substr( 0, in.find( sepEndValue ) ); 
      in.erase( 0, in.find( sepEndValue ) + sepEndValue.size() ); 
      read_param::addGlobalParameter( key, value );
    }

}


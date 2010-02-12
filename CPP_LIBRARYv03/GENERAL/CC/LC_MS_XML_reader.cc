///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS LC_MS_XML_reader:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_LC_MS_XML_READER
#define USE_MS2_INFO

#include "GENERAL_USE.h"


double LC_MS_XML_reader::TR_MIN = 0;
double LC_MS_XML_reader::TR_MAX = 180;
double LC_MS_XML_reader::FEATURE_MZ_MIN = 0;
double LC_MS_XML_reader::FEATURE_MZ_MAX = 1800;
int LC_MS_XML_reader::FEATURE_CHRG_MIN = 1;
int LC_MS_XML_reader::FEATURE_CHRG_MAX = 5;
//double LC_MS_XML_reader::PEAK_SCORE_THERSHOLD = -1000;
double LC_MS_XML_reader::SIGNAL_TO_NOISE_THERSHOLD = 0;
double LC_MS_XML_reader::PEAK_INTENSITY_THRESHOLD = 1000;
bool LC_MS_XML_reader::EXTRACT_MONO_ISOTOPE_PROFILE = true;

string LC_MS_XML_reader::DATA_STORAGE_XML_FORMAT_TYPE;


////////////////////////////////////////////////
// constructor for the object LC_MS_XML_reader:
LC_MS_XML_reader::LC_MS_XML_reader( ){
  THIS_LC_MS = NULL;
  PARSER = NULL;
  INIT_PARSER();
}


//////////////////////////////////////////////////
// class desctructor of LC_MS_XML_reader
LC_MS_XML_reader::~LC_MS_XML_reader(){
  if( PARSER != NULL ){
    delete PARSER;
    PARSER = NULL;
  }
  THIS_LC_MS = NULL;
  PARSED_LC_MS_RUNS.clear();
  
}

//////////////////////////////////////////////////
// class copy constructor of LC_MS_XML_reader
LC_MS_XML_reader::LC_MS_XML_reader(const LC_MS_XML_reader& tmp){
  PARSER = tmp.PARSER;
}

//////////////////////////////////////////////////
// class copy constructor of LC_MS_XML_reader
LC_MS_XML_reader::LC_MS_XML_reader(const LC_MS_XML_reader* tmp){
  PARSER = tmp->PARSER;
}

//////////////////////////////////////////////////
// copy constructor:
LC_MS_XML_reader& LC_MS_XML_reader::operator=(const LC_MS_XML_reader& tmp){
  PARSER = tmp.PARSER;
  return *this;
}

//////////////////////////////////////////////////////////
// get initialzatio parameters
void LC_MS_XML_reader::INIT_PARSER(){
  THIS_LC_MS = NULL;
  PARSED_LC_MS_RUNS.clear();
}

//////////////////////////////////////////////////////////
// starts parsing of the LC/MS run
void LC_MS_XML_reader::start_LC_MS_parsing( string IN ){

  INIT_PARSER();
  
  // check if infile is a directory or a file:
  file_sys accessor;
  if( accessor.check_directory( IN ) ){
    start_LC_MS_parsing_of_DIR( IN );
  }
  else{
    start_LC_MS_parsing_of_FILE( IN );
  }
  
  
}
  
  
//////////////////////////////////////////////////////////
// starts parsing of LC/MS runs in a directory
void LC_MS_XML_reader::start_LC_MS_parsing_of_DIR( string IN_DIR ){
  
  // get the list of target files in the dir
  // with the according extension:
  vector<string> endings;
  endings.push_back( LC_MS_XML_reader::getXMLFileEnding() );

  //endings.push_back( ".xml" );
  //endings.push_back( ".apml" );
  
  // read the files:
  file_sys accessor;
  accessor.read_dir(IN_DIR, endings);
  endings.clear();
  char* file  = NULL;
  file = accessor.get_next_file();
  
  while(file != NULL){
    
    // check if its a LC/MS XML file:
    if( check_LC_MS_XML_file( file ) ){
    
      printf("\t\t-- importing '%s'\n",file);
      
      // jump into read loop which only stopps
      // when end of file is reached
      start_LC_MS_parsing_of_FILE(file);
    }
    
    // next file:
    file = accessor.get_next_file();
  }
}



//////////////////////////////////////////////////////////
// starts parsing of the LC/MS run of one file
void LC_MS_XML_reader::start_LC_MS_parsing_of_FILE( string XML_IN_FILE ){

  MAIN_XML_FILE = XML_IN_FILE;

  // check if its a tab delimited file:
  if( XML_IN_FILE.find( ".tsv" ) != string::npos ){    
    start_LC_MS_parsing_MASTER_TAB_DEL_FORMAT( XML_IN_FILE );
  }
  else{
    
    PARSER = new TiXmlDocument( XML_IN_FILE.c_str() );
    
    if( PARSER->LoadFile() ){
      printf("\t\t\t--- parsing XML file '%s' ...\n", XML_IN_FILE.c_str() );    
    }
    else{    
      if( PARSER != NULL ){
        delete PARSER;
        PARSER = NULL;
      }
      
      printf("\n\t\t**** Could not open XML input-file %s, LC_MS_XML_reader::166 \n", XML_IN_FILE.c_str() );
    }
    
    
    if( PARSER != NULL ){
      
      //////////////////////////////////
      // check first if its APML schema or
      // MASTER run:
      bool falseFile = true;
      if( PARSER->FirstChild( "MASTER_RUN_SUMMARY" ) != NULL ){
        if( PARSER->FirstChild( "MASTER_RUN_SUMMARY" )->FirstChild( "LC_MS_RUN" ) != NULL ){
          falseFile = false;
          start_LC_MS_parsing_MASTER_XML_FORMAT();
        }
      }
      else if( PARSER->FirstChild( "apml" ) ){
        falseFile = false;
        start_LC_MS_parsing_APML_XML_FORMAT();
      }
      if( falseFile ){
        printf("\t\t**** Could not open XML input-file %s, LC_MS_XML_reader::166 \n", XML_IN_FILE.c_str() );      
      }
      
      if( PARSER != NULL ){
        delete PARSER;
        PARSER = NULL;
      }
      
    }
  }
}


//////////////////////////////////////////////////////////
// store the LC/MS run in the list:
void LC_MS_XML_reader::store_LC_MS_RUN( LC_MS* IN ){
  
  // check if the run contains any features:
  if( IN != NULL ){
    
    if( IN->check_feature_list_empty() ){
      
      printf( " ********************************************************************************************************************\n");
      printf( "\t Empty LC-MS run '%s':: TR-range [%f:%f], Signal/Noise threshold: %f \n", IN->get_spec_name().c_str(), TR_MIN, TR_MAX, SIGNAL_TO_NOISE_THERSHOLD); 
      printf( "\t -> this run was removed from the analysis!\n");
      printf( " ********************************************************************************************************************\n");
        
    }
    else{
      // process the input run:
      // process_validated_LCMS( IN );
      // contains features, so add it:
      PARSED_LC_MS_RUNS.push_back( *IN );      
    }
    
  }
    
  IN = NULL;

}


//////////////////////////////////////////////////////////
// store a featuer in the current LC/MS run
void LC_MS_XML_reader::store_FEATURE_in_THIS_LC_MS( feature* IN ){
  
  if( check_FEATURE_VALIDITY( IN ) ){
    THIS_LC_MS->add_feature( IN );
  }

  // clean up:
  delete IN;
  IN = NULL;
}

//////////////////////////////////////////////////////////
// check if feature accepted
bool LC_MS_XML_reader::check_FEATURE_VALIDITY( feature* IN ){
  
  //////////////////////////
  // check the retention time range
  double TR = IN->get_retention_time();
  if( ( TR < TR_MIN) || ( TR > TR_MAX) ){
    return false;
  }
  

  //////////////////////////
  // check signal to noise value:
  if( IN->getSignalToNoise() < SIGNAL_TO_NOISE_THERSHOLD ){
    return false;
  }
    
  /*
  //////////////////////////
  // check if has good enough score:
  if( ( IN->get_peak_score() < PEAK_SCORE_THERSHOLD ) && ( IN->get_peak_score() > -1.0 ) ){
    return false;
  }
  */
  
  //////////////////////////
  // check if passes intensity threshold:
  if( ( IN->get_peak_area() < PEAK_INTENSITY_THRESHOLD ) && ( IN->get_peak_area() < -1.0 ) ){
    return false;
  }
  
  //////////////////////////
  // charge state selection range:
  if( ( IN->get_charge_state() < FEATURE_CHRG_MIN ) || ( IN->get_charge_state() > FEATURE_CHRG_MAX ) ){
    return false;
  }

  //////////////////////////
  // m/z selection range:
  if( ( IN->get_MZ() < FEATURE_MZ_MIN ) && ( !feature::compareFeatureMassValuesAtPPMLevel(IN->get_MZ(), FEATURE_MZ_MIN) ) ){
    return false;
  }
  if( ( IN->get_MZ() > FEATURE_MZ_MAX ) && ( !feature::compareFeatureMassValuesAtPPMLevel(IN->get_MZ(), FEATURE_MZ_MAX) ) ){
    return false;
  }
  
  
  
  
  return true;
  
}




//################################################################################################
//
// FUNCTIONS TO PARSE MASTER XML ENCODED DATA:

//////////////////////////////////////////////////////////
// starts parsing of the LC/MS run in MASTER XML format
void LC_MS_XML_reader::start_LC_MS_parsing_MASTER_XML_FORMAT(){
  
  // get the element of the ms search summary and go through each of them:
  TiXmlNode* MSMS_RUN_SUMMARY = PARSER->FirstChild( "MASTER_RUN_SUMMARY" ); 
  
  // extract child LC/MS:
  extract_LC_MS_childs( MSMS_RUN_SUMMARY );
  
  // extract LC/MS features:
  extract_LC_MS_features( MSMS_RUN_SUMMARY->FirstChild("LC_MS_RUN")->FirstChild("LC_MS_FEATURES") );
  
  // store the run:
  store_LC_MS_RUN( THIS_LC_MS );
  
}  


//////////////////////////////////////////////////////////
// extracts the LC/MS childs:
void LC_MS_XML_reader::extract_LC_MS_childs( TiXmlNode* MSMS_RUN_SUMMARY ){
  
  if( MSMS_RUN_SUMMARY != NULL ){
  
    TiXmlNode* LCMS_RUN = MSMS_RUN_SUMMARY->FirstChild("LC_MS_RUN");
    if( LCMS_RUN != NULL ){
      
      TiXmlElement* THIS_RUN = LCMS_RUN->ToElement();
      string name = THIS_RUN->Attribute("name");
      THIS_LC_MS = new LC_MS(name);
    
      //int ID = THIS_RUN->Attribute("ID");
      //THIS_LC_MS->set_spectrum_ID( ID );
      
      TiXmlNode* LCMS_CHILD;
      for( LCMS_CHILD = LCMS_RUN->FirstChild("CHILD_LC_MS_RUNS")->FirstChild("LC_MS_CHILD"); LCMS_CHILD; LCMS_CHILD = LCMS_CHILD->NextSibling() ){
        
        if( LCMS_CHILD != NULL ){
          TiXmlElement* THIS_CHILD = LCMS_CHILD->ToElement();
          string name = THIS_CHILD->Attribute("name");
          int ID = atoi( THIS_CHILD->Attribute("ID") );
          THIS_LC_MS->add_raw_spec_name( ID, name );
        }
      }
    }
  }
}


//////////////////////////////////////////////////////////
// extract LC/MS features:
void LC_MS_XML_reader::extract_LC_MS_features( TiXmlNode* LC_MS_FEATURES ){

  if( LC_MS_FEATURES != NULL ){
    
    TiXmlNode* FEATURE;
    for( FEATURE = LC_MS_FEATURES->FirstChild("MS1_FEATURE"); FEATURE; FEATURE = FEATURE->NextSibling() ){
      extract_FEATURE( FEATURE );
    }
  }
}

//////////////////////////////////////////////////////////
// extract LC/MS features:
void LC_MS_XML_reader::extract_FEATURE( TiXmlNode* FEATURE ){

  if( FEATURE != NULL ) {
    
    feature* fea = NULL;
    fea = extract_and_return_FEATURE( FEATURE, fea );
    
      if( fea != NULL ){
      // add it to the current LC/MS
      store_FEATURE_in_THIS_LC_MS( fea );    
    }
  }
}



//////////////////////////////////////////////////////////
// extract and returns a MS1 features:
feature* LC_MS_XML_reader::extract_and_return_FEATURE( TiXmlNode* FEATURE , feature* FEA){
  
  if( FEATURE != NULL ) {
    
    TiXmlElement* THIS_FEATURE = FEATURE->ToElement();
    
    // parameters to parse out per MS2 identification
    double MONO_MZ = atof( THIS_FEATURE->Attribute("m_z") );
    double MONO_MZ_START = MONO_MZ;
    double MONO_MZ_END = MONO_MZ;
    double RAW_MZ = MONO_MZ;
    if( THIS_FEATURE->Attribute("m_z_Start") ){
      MONO_MZ_START = atof( THIS_FEATURE->Attribute("m_z_Start") );
    }
    if( THIS_FEATURE->Attribute("m_z_End") ){
      MONO_MZ_END = atof( THIS_FEATURE->Attribute("m_z_End") );
    }
    if( THIS_FEATURE->Attribute("m_z_Original") ){
      RAW_MZ = atof( THIS_FEATURE->Attribute("m_z_Original") );
    }
    
    int CHRG = atoi( THIS_FEATURE->Attribute("charge_state") );
    int LC_MS_ID = atoi( THIS_FEATURE->Attribute("LC_MS_ID") );
    
    double TR = atof( THIS_FEATURE->Attribute("Tr") );
    double TR_START = TR;
    if( THIS_FEATURE->Attribute("Tr_Start") ){
      TR_START = atof( THIS_FEATURE->Attribute("Tr_Start") );
    }
    double TR_END = TR;
    if( THIS_FEATURE->Attribute("Tr_End") ){
      TR_END = atof( THIS_FEATURE->Attribute("Tr_End") );
    }
    double TR_RAW_APEX = TR;
    if( THIS_FEATURE->Attribute("Tr_Apex") ){
      TR_RAW_APEX = atof( THIS_FEATURE->Attribute("Tr_Apex") );
    }
    
    string specialTag = "";
    if( THIS_FEATURE->Attribute("StandardPeptide") ){
      specialTag = "HPin";
    }
    
    
    double snRatio = 100;
    if( THIS_FEATURE->Attribute("snRatio") ){
      snRatio = atof( THIS_FEATURE->Attribute("snRatio") );
    }
    
    double NoiseBackground = 100;
    if( THIS_FEATURE->Attribute("BGnoise") ){
      NoiseBackground = atof( THIS_FEATURE->Attribute("BGnoise") );
    }
    
    
    //float PEAK_SCORE = atof( THIS_FEATURE->Attribute("score") );    
    int FEA_ID = atoi( THIS_FEATURE->Attribute("Feature_ID") );
    
    // LC information:
    TiXmlElement* LC_INFO = FEATURE->FirstChild("LC_INFO")->ToElement();
    int SCAN_APEX = atoi( LC_INFO->Attribute("apex") );
    int SCAN_START = atoi( LC_INFO->Attribute("start") 
                           );
    int SCAN_END = atoi( LC_INFO->Attribute("end") );
    float PEAK_AREA = atof( LC_INFO->Attribute("AREA") );
    
    float APEX_INTENSITY = -1;
    if( LC_INFO->Attribute("apex_intensity") != NULL ){
      APEX_INTENSITY = atof( LC_INFO->Attribute("apex_intensity") );
    }
    
    // here all the info is available, construct the feature:
    FEA = new feature(MONO_MZ, TR, SCAN_APEX, SCAN_START, SCAN_END, CHRG, PEAK_AREA, APEX_INTENSITY, 0);
    FEA->set_feature_ID(FEA_ID);  
    FEA->set_spectrum_ID(LC_MS_ID);  
    FEA->set_MZ_START( MONO_MZ_START );
    FEA->set_MZ_END( MONO_MZ_END );
    FEA->set_raw_MZ( RAW_MZ ); 
    FEA->set_retention_time_START( TR_START );
    FEA->set_retention_time_END( TR_END );
    FEA->set_raw_retention_time_apex( TR_RAW_APEX );
    FEA->setFeatureExtraInformation( specialTag );
    FEA->setSignalToNoise( snRatio );
    FEA->setBackgroundNoiseLevel( NoiseBackground );
    
    
    // check for MS2 scan info:
    if( THIS_FEATURE->Attribute("ms2Scans") ){
      extractPerformedMS2ScanInfo( THIS_FEATURE->Attribute("ms2Scans"), FEA );
    }
      
    // add the MS2 info:
    TiXmlNode* MS_INFOs = FEATURE->FirstChild("MS2_INFO");
    if( MS_INFOs != NULL ){
      TiXmlNode* MS_INFO;
      for( MS_INFO = MS_INFOs->FirstChild("MS2_SCAN"); MS_INFO; MS_INFO = MS_INFO->NextSibling() ){
        extract_MS_info( MS_INFO , FEA);
      }
    }
    
    // check for lc profile:
    TiXmlNode* Profile = FEATURE->FirstChild("LC_ELUTION_PROFILE");
    if( ( Profile != NULL ) && (EXTRACT_MONO_ISOTOPE_PROFILE) ){
        extractLCProfile( Profile , FEA);
    }
    
    // extract now matched feature info:
    extract_matched_features( FEATURE->FirstChild("MATCHED_FEATURES"), FEA);

    // check for ms2 consenus spectrum:
    TiXmlNode* MSMSSPECTRA = FEATURE->FirstChild("MS2_SPECTRA");
    if( ( MSMSSPECTRA != NULL ) && (LC_MS_XML_writer::STORE_MS2_FRAGMENT_TRACE_DATA)  ){
      TiXmlNode* MS2cons;
      for( MS2cons = MSMSSPECTRA->FirstChild("MS2_SPECTRUM"); MS2cons; MS2cons = MS2cons->NextSibling() ){
        extractMS2ConsenusSpectrum( MS2cons , FEA);
        // if it was only a ms2 consensus fake feature with no more
        // ms2 consensus spectrum, then remove it:
        if( (FEA->getMS2Feature() == NULL) && (FEA->get_charge_state() == -1) ){
          delete FEA;
          FEA = NULL;
        }
      }
    }
    // return the parsed feature:
    return FEA;
  }
  else{
    return NULL;
  }
}


//////////////////////////////////////////////////////////
// extract MS2 features info:
void LC_MS_XML_reader::extract_MS_info( TiXmlNode* MS2_INFO , feature* IN){
  
  if( MS2_INFO != NULL ) {
    
    TiXmlElement* THIS_MS_INFO = MS2_INFO->ToElement();
   
    int SCAN_MS2 = atoi( THIS_MS_INFO->Attribute("MS2_scan") );

    string mainAC;
    if( THIS_MS_INFO->Attribute("AC") ){
      mainAC = THIS_MS_INFO->Attribute("AC");
    }
    
    string SQ = THIS_MS_INFO->Attribute("SQ");
    float PEP_PROB = atof( THIS_MS_INFO->Attribute("PeptideProbability") );
    ms2_info* TMP = NULL;
    
    if( mainAC.empty() ){
      TMP = new ms2_info(SQ, PEP_PROB, IN->get_charge_state(), SCAN_MS2 );
    }
    else{
      TMP = new ms2_info(mainAC, SQ, PEP_PROB, IN->get_charge_state(), SCAN_MS2 );
    }
    
    if( THIS_MS_INFO->Attribute("precursor_m_z") ){
      double MONO_MZ = atof( THIS_MS_INFO->Attribute("precursor_m_z") );
      TMP->set_MONO_MZ( MONO_MZ );
    }
    if(  THIS_MS_INFO->Attribute("charge_state") ){
      int CHRG = atoi( THIS_MS_INFO->Attribute("charge_state") );
      TMP->set_CHRG( CHRG );
    }
    
    if(  THIS_MS_INFO->Attribute("retention_time") ){
      double tr = atof( THIS_MS_INFO->Attribute("retention_time") );
      TMP->setRetentionTime( tr );
    }
    
    if(  THIS_MS_INFO->Attribute("interact_file") ){
      TMP->set_ORIGINAL_INTERACT_FILE( THIS_MS_INFO->Attribute("interact_file") );
    }
    else{
      TMP->set_ORIGINAL_INTERACT_FILE( "NA" );
    }
    
    if(  THIS_MS_INFO->Attribute("ms2_type_tag") ){
      TMP->set_MS2_TYPE_TAG( THIS_MS_INFO->Attribute("ms2_type_tag") );
    }
    else{
      TMP->set_MS2_TYPE_TAG( "NA" );
    }
   
    if(  THIS_MS_INFO->Attribute("prev_AA") ){
      TMP->set_PREV_AA( THIS_MS_INFO->Attribute("prev_AA") );
    }
    else{
      TMP->set_PREV_AA( "NA" );
    }
    
    // sorcerer search results:
    if(  THIS_MS_INFO->Attribute("Delta_CN") ){
      double Delta_CN = atof( THIS_MS_INFO->Attribute("Delta_CN") );
      TMP->set_DELTA_CN( Delta_CN );
    }
    if(  THIS_MS_INFO->Attribute("XCorr") ){
      double XCORR = atof( THIS_MS_INFO->Attribute("XCorr") );
      TMP->set_XCORR( XCORR );
    }
    
    // check for alternative proteins ACs:
    TiXmlNode* ALT_PROT;    
    for( ALT_PROT = MS2_INFO->FirstChild( "ALTERNATIVE_PROTEIN" ); ALT_PROT; ALT_PROT = ALT_PROT->NextSibling()){
      TiXmlElement* THIS_ALT_PROT = ALT_PROT->ToElement();
      string name = ALT_PROT->Value(); 
      if( name == "ALTERNATIVE_PROTEIN" ){
        mainAC = THIS_ALT_PROT->Attribute("AC");
        if( THIS_ALT_PROT->Attribute("ProteinProb") ){
          TMP->set_AC( mainAC, atof(THIS_ALT_PROT->Attribute("ProteinProb")) );
        }
        else{
          TMP->set_AC(mainAC);
        }
      }
      else{
        break;
      }
    }
    
    // extract AA modifications
    // add the MS2 info:
    TiXmlNode* AAs = MS2_INFO->FirstChild("MODIFICATIONS");
    TiXmlNode* AA;
    for( AA = AAs->FirstChild("AA_MOD"); AA; AA = AA->NextSibling() ){
      extract_AA_modifications( AA , TMP);
    }
    
    IN->add_MS2_info( TMP );
    TMP = NULL;
  }
}



/////////////////////////////////////////////////////////
// extract ms2 scans information:
void LC_MS_XML_reader::extractPerformedMS2ScanInfo( string ms2scans , feature* IN){
  
  while (ms2scans.find(",") != string::npos) {
    
    string tmp = ms2scans.substr(0, ms2scans.find(","));
    IN->addChildMSMSScans( atof(tmp.c_str()) );
    ms2scans = ms2scans.substr(ms2scans.find(",") + 1, ms2scans.size());
  }
  
  IN->addChildMSMSScans( atof(ms2scans.c_str()) );
  
}



/////////////////////////////////////////////////////////
// extract MS1 features LC profile:
void LC_MS_XML_reader::extractLCProfile( TiXmlNode* profile , feature* IN){
  
  if( profile != NULL ) {
    
    TiXmlElement* THISp = profile->ToElement();
    string pString = THISp->Attribute("mono_peak");
    
    // create the profile object:
    featureLCprofile* pro = new featureLCprofile(
                                                 IN->get_MZ(), IN->get_retention_time()
                                                 , IN->get_apex_peak_intensity(), IN->get_scan_number(), 
                                                 IN->get_charge_state(), IN->get_peak_area()
                                                 );
    
    
    int scan_counter = IN->get_scan_start();
    while (pString.find(",") != string::npos) {
      
      string tmp = pString.substr(0, pString.find(","));
      double TR = atof(tmp.c_str());
      pString = pString.substr(pString.find(",") + 1,
                                        pString.length());
     
      if (pString.find(",") == string::npos) {
        double intens = atof(pString.c_str());			
        pro->addMS1elutionSignal(  IN->get_MZ(), intens, scan_counter, IN->get_charge_state(), TR);
        break;
      } else {
        tmp = pString.substr(0, pString.find(","));
        double intens = atof(tmp.c_str());
        pro->addMS1elutionSignal(  IN->get_MZ(), intens, scan_counter, IN->get_charge_state(), TR);
        pString = pString.substr(pString.find(",") + 1, pString.size());
      }
      
      scan_counter++;
    }
    
    // add now to feature:
    IN->setLCelutionProfile( pro );
    pro = NULL;
    IN = NULL;
  }
}


  
/////////////////////////////////////////////////////////
// extract the ms2 conesnus spectrum of a feature
void LC_MS_XML_reader::extractMS2ConsenusSpectrum( TiXmlNode* MS2Conensus , feature* IN){
  
  if( MS2Conensus != NULL ) {
    
    double preMZ = atof( MS2Conensus->ToElement()->Attribute("precursorMZ") );
    double preTR = atof( MS2Conensus->ToElement()->Attribute("precursorTR") );
    int preZ = atoi( MS2Conensus->ToElement()->Attribute("precursorChrg") );
    int preScan = atoi( MS2Conensus->ToElement()->Attribute("apexScan") );
    MS2_feature* ms2Fea = new MS2_feature( preMZ, preTR, preZ, preScan); 
      
    // parse the fragments:
    string fString = MS2Conensus->ToElement()->Attribute("fragments");
    
    while (fString.find(",") != string::npos) {
      
      string tmp = fString.substr(0, fString.find(","));
      double mz = atof(tmp.c_str());
      fString = fString.substr(fString.find(",") + 1,
                               fString.length());
      
      if (fString.find(",") == string::npos) {
        double intens = atof(fString.c_str());			

        // constructor for the object MS2Fragment:
        MS2Fragment* myF = new MS2Fragment( preMZ, preZ, preTR, preScan,  preZ, mz, intens);
        ms2Fea->addMS2Fragment( myF );
        delete myF;
        myF = NULL;
        
        break;
      } 
      else {
        tmp = fString.substr(0, fString.find(","));
        double intens = atof(tmp.c_str());
        
        // constructor for the object MS2Fragment:
        MS2Fragment* myF = new MS2Fragment( preMZ, preZ, preTR, preScan,  preZ, mz, intens);
        ms2Fea->addMS2Fragment( myF );
        delete myF;
        myF = NULL;
        
        fString = fString.substr(fString.find(",") + 1, fString.size());
      }
      
    }
    
    /////////////////////////////
    // remove H2O loss stuff:
    // ms2Fea->removeWaterLossRegion( );
  
    // check if should be processed on:
    if( ms2Fea->getNbMS2Fragments() > 0 ){
      IN->addMS2Feature( ms2Fea );
    }
    else{
      delete ms2Fea;
    }
    
    ms2Fea = NULL;
    IN = NULL;
  }
}




//////////////////////////////////////////////////////////
// extracts the modifcation 
void LC_MS_XML_reader::extract_AA_modifications(TiXmlNode* AA, ms2_info* IN){

  if( AA != NULL ){
    
    TiXmlElement* THIS_AA = AA->ToElement();
    int pos = atoi(THIS_AA->Attribute("pos"));
    double value = atof(THIS_AA->Attribute("value"));
    IN->add_modification(pos, value);
  }
}


//////////////////////////////////////////////////////////
// extracts a profile 
map<int, double> LC_MS_XML_reader::extract_profile(TiXmlNode* PROFILE){
  
  map<int, double> OUT;
  
  if( PROFILE != NULL ){
    
    TiXmlNode* X;
    for( X = PROFILE->FirstChild("DATA_POINT"); X; X = X->NextSibling() ){
      if( X != NULL ){
        TiXmlElement* THIS_X = X->ToElement();
        int pos = atoi(THIS_X->Attribute("x"));
        double value = atof(THIS_X->Attribute("y"));
        OUT.insert( make_pair(pos, value) );
      }
    }
  }
  
  return OUT;
}


//////////////////////////////////////////////////////////
// extract now matched feature info:
void LC_MS_XML_reader::extract_matched_features( TiXmlNode* M_FEATURES, feature* IN){
    
  if( M_FEATURES != NULL ){

    TiXmlNode* F;
    for( F = M_FEATURES->FirstChild("MS1_FEATURE"); F; F = F->NextSibling() ){
  
      feature* fea = NULL;
      fea = extract_and_return_FEATURE( F, fea );

      if( check_FEATURE_VALIDITY( fea ) ){
        IN->add_matched_feature( fea );
      }
      else{
        delete fea;
      }
      
      fea = NULL;
    }
  }
  IN = NULL;
}







//################################################################################################
//
// FUNCTIONS TO PARSE APML XML ENCODED DATA:

//////////////////////////////////////////////////////////
// starts parsing of the LC/MS run in APML XML format
void LC_MS_XML_reader::start_LC_MS_parsing_APML_XML_FORMAT(){
  
  // get the element of the ms search summary and go through each of them:
  TiXmlNode* MSMS_RUN_SUMMARY = PARSER->FirstChild( "apml" ); 
  
  // extract child LC/MS:
  TiXmlNode* DATA = MSMS_RUN_SUMMARY->FirstChild( "data" );
  if( DATA != NULL ){

    // extract child LC/MS:
    TiXmlNode* ALIGNMENT = DATA->FirstChild( "alignment" );
    
    if( ALIGNMENT ){
      
      // extract the LC/MS source list:
      extract_LC_MS_source( ALIGNMENT );
      // check here if its an aligned LCMS run or not:
      extract_aligned_feature_list( ALIGNMENT );
    
    }
    else{
      // check here if its an aligned LCMS run or not:
      extract_feature_list( DATA );
    }
    
  }  
}


//////////////////////////////////////////////////////////
// extracts the source LC/MS runs of a MASTERMAP
void LC_MS_XML_reader::extract_LC_MS_source( TiXmlNode* ALIGNMENT ){
  
  if( ALIGNMENT != NULL ){
    
    TiXmlNode* LCMS_SOURCE = ALIGNMENT->FirstChild("feature_source_list");
    if( LCMS_SOURCE != NULL ){
      
      if( THIS_LC_MS != NULL ){
        delete THIS_LC_MS;
        THIS_LC_MS = NULL;
      }
      
      THIS_LC_MS = new LC_MS( "MASTER_MAP" );
      
      
      TiXmlNode* SOURCE;
      for( SOURCE = LCMS_SOURCE->FirstChild("source"); SOURCE; SOURCE = SOURCE->NextSibling() ){
        
        if( SOURCE != NULL ){
          TiXmlElement* THIS_SOURCE = SOURCE->ToElement();
          string name = THIS_SOURCE->Attribute("location");
          int ID = atoi( THIS_SOURCE->Attribute("id") );
          THIS_LC_MS->add_raw_spec_name( ID, name );
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////
// extract feature list:
void LC_MS_XML_reader::extract_feature_list( TiXmlNode* DATA ){
  
  if( DATA != NULL ){
    
    TiXmlNode* LISTS = DATA->FirstChild("peak_lists");
    TiXmlNode* LIST;
    // get id:
    int ID = 0;
    for( LIST = LISTS->FirstChild("peak_list"); LIST; LIST = LIST->NextSibling() ){

      // get the run name:
      TiXmlElement* THIS_LIST_NAME = LIST->ToElement();
      string name;
      if( THIS_LIST_NAME->Attribute("source") ){
        name = THIS_LIST_NAME->Attribute("source");
      }
      
      // create a LC/MS run:
      if( THIS_LC_MS != NULL ){
        delete THIS_LC_MS;
        THIS_LC_MS = NULL;
      }
      THIS_LC_MS = new LC_MS(name);
      THIS_LC_MS->set_spectrum_ID( ID );
      
      TiXmlNode* FEATURES = LIST->FirstChild("features");
      if( FEATURES != NULL ){
        
        TiXmlNode* FEATURE;
        for( FEATURE = FEATURES->FirstChild("feature"); FEATURE; FEATURE = FEATURE->NextSibling() ){
          extract_FEATURE_APML( FEATURE );
        }
      }
      
      // store this RUN:
      store_LC_MS_RUN( THIS_LC_MS );
          
      // next list:
      ID++;
    }
  }
}



//////////////////////////////////////////////////////////
// extract aligned feature list:
void LC_MS_XML_reader::extract_aligned_feature_list( TiXmlNode* DATA ){
  
  if( DATA != NULL ){
    
    TiXmlNode* ALIGNED_FEATURES = DATA->FirstChild("aligned_features");
    TiXmlNode* A_FEATURE;
    // get id:
    for( A_FEATURE = ALIGNED_FEATURES->FirstChild("aligned_feature"); A_FEATURE; A_FEATURE = A_FEATURE->NextSibling() ){
            
      if( A_FEATURE != NULL ){        
        TiXmlElement* THIS_ALL_FEATURE = A_FEATURE->ToElement();
        int ALL_FEA_ID;
        if( THIS_ALL_FEATURE->Attribute("id") ){
          ALL_FEA_ID = atoi( THIS_ALL_FEATURE->Attribute("id") );
        }
        
        TiXmlNode* MATCHED_FEATURES = A_FEATURE->FirstChild("features");

        TiXmlNode* FEATURE = MATCHED_FEATURES->FirstChild("feature");
        if( FEATURE ){
          // extract the first feature:
          feature* MAIN = new feature();
          extract_FEATURE_APML( FEATURE, MAIN );
          
          // extract the matched ones:
          FEATURE = FEATURE->NextSibling();
          while( FEATURE && MAIN ){
            feature* SUB_MAIN = new feature();
            extract_FEATURE_APML( FEATURE , SUB_MAIN);
            if( SUB_MAIN ){
              MAIN->add_matched_feature( SUB_MAIN );
              SUB_MAIN = NULL;
            }
            FEATURE = FEATURE->NextSibling();
          }
          
          // add to the MASTER MAP:
          store_FEATURE_in_THIS_LC_MS( MAIN );
          MAIN = NULL;
        }    
      }
    }
  }
  if( THIS_LC_MS ){
    // store the run:
    store_LC_MS_RUN( THIS_LC_MS );
  }  
}


//////////////////////////////////////////////////////////
// extract features APML:
void LC_MS_XML_reader::extract_FEATURE_APML( TiXmlNode* FEATURE ){
  
  feature* FEA = new feature();
  extract_FEATURE_APML( FEATURE , FEA);
  if( FEA ){
    // add it to the current LC/MS
    store_FEATURE_in_THIS_LC_MS( FEA );
    FEA = NULL;
  }  
}

//////////////////////////////////////////////////////////
// extract features APML:
void LC_MS_XML_reader::extract_FEATURE_APML( TiXmlNode* FEATURE , feature* IN){
  feature* FEA;
  if( FEATURE != NULL ) {
    
    TiXmlElement* THIS_FEATURE = FEATURE->ToElement();
    int F_ID = -1;
    if( THIS_FEATURE->Attribute("id") ){
      F_ID = atoi( THIS_FEATURE->Attribute("id") );
    }
    
    // determine the source
    int LC_MS_ID = -1;
    if( THIS_FEATURE->Attribute("source") ){
     LC_MS_ID = get_LCM_ID_by_name( THIS_FEATURE->Attribute("source") );
    }
    
    string specialTag = "";
    if( THIS_FEATURE->Attribute("StandardPeptide") ){
      specialTag = "HPin";
    }
    
    // coordinates
    TiXmlNode* CORD = FEATURE->FirstChild("coordinate");
    if( CORD ){
      TiXmlElement* THIS_CORD = CORD->ToElement();
      
      
      // parameters to parse out per MS2 identification
      double MONO_MZ = atof( THIS_CORD->Attribute("mz") );
      int CHRG = atoi( THIS_CORD->Attribute("charge") );
      double TR = atof( THIS_CORD->Attribute("rt") );
      float PEAK_AREA = atof( THIS_CORD->Attribute("intensity") );
      float APEX_INTENSITY = -1;
      double PEAK_SCORE = -1;
      int SCAN_START = 0;
      int SCAN_END = 0;
      int SCAN_APEX = -1;
      
      // scan information:
      // coordinates
      TiXmlNode* SCAN = CORD->FirstChild("scan_range");
      if( SCAN ){
        TiXmlElement* THIS_SCAN = SCAN->ToElement();
        SCAN_START = atoi( THIS_SCAN->Attribute("min") );
        SCAN_END = atoi( THIS_SCAN->Attribute("max") );
      }
      
      // here all the info is available, construct the feature:
      FEA = new feature(MONO_MZ, TR, SCAN_APEX, SCAN_START, SCAN_END, CHRG, PEAK_AREA, APEX_INTENSITY, PEAK_SCORE);
      // add a feature ID:
      FEA->set_feature_ID(F_ID);  
      // add a LC/MS ID:
      FEA->set_spectrum_ID( LC_MS_ID );  
      FEA->setFeatureExtraInformation( specialTag );
      // time information:
      // coordinates
      TiXmlNode* TIME = CORD->FirstChild("time_range");
      if( TIME && FEA ){
        
        TiXmlElement* THIS_TIME = TIME->ToElement();
        double TR_START = atof( THIS_TIME->Attribute("min") );
        double TR_END = atof( THIS_TIME->Attribute("max") );
        FEA->set_retention_time_START( TR_START );  
        FEA->set_retention_time_END( TR_END );  
        
      }
      // M/z information:
      // coordinates
      TiXmlNode* MZ = CORD->FirstChild("mz_range");
      if( MZ && FEA ){
        
        TiXmlElement* THIS_MZ = MZ->ToElement();
        double MZ_START = atof( THIS_MZ->Attribute("min") );
        double MZ_END = atof( THIS_MZ->Attribute("max") );
        FEA->set_MZ_START( MZ_START );  
        FEA->set_MZ_END( MZ_END );  
        
      }
      
   
      // add the MS2 info:
      TiXmlNode* MS_INFOs = FEATURE->FirstChild("ppids");
      if( MS_INFOs ){
        TiXmlNode* MS_INFO;
        for( MS_INFO = MS_INFOs->FirstChild("ppid"); MS_INFO; MS_INFO = MS_INFO->NextSibling() ){
          extract_MS2_info_from_APML( MS_INFO, FEA);
        }
      }
      
      if( FEA ){
        *IN = *FEA;
      }
    }
  }
  else{
    //delete IN;
    //IN = NULL;
  }
}

//////////////////////////////////////////////////////////
// extract MS2 features info from APML
void LC_MS_XML_reader::extract_MS2_info_from_APML( TiXmlNode* MS2_INFO , feature* IN){
  
  if( MS2_INFO != NULL ) {
    
    TiXmlElement* THIS_MS_INFO = MS2_INFO->ToElement();
    double MONO_MZ = 0;
    int CHRG = 0;
    string dta;
    string SQ;
    string MS2_TYPE_TAG;
    vector<string> AC;
    float PEP_PROB = 0;
      
    int SCAN_MS2 = atoi( THIS_MS_INFO->Attribute("ms2_scan_num") );
    if( THIS_MS_INFO->Attribute("precursor_mz") ){
      MONO_MZ = atof( THIS_MS_INFO->Attribute("precursor_mz") );
    }
    if(  THIS_MS_INFO->Attribute("charge") ){
      CHRG = atoi( THIS_MS_INFO->Attribute("charge") );
    }
    TiXmlNode* THIS_DTA = MS2_INFO->FirstChild("dta");
    if( THIS_DTA ){
      TiXmlElement* THIS_E_DTA = THIS_DTA->ToElement();  
      if(  THIS_E_DTA->Attribute("ms2_type_tag") ){
        MS2_TYPE_TAG = THIS_E_DTA->Attribute("ms2_type_tag");
      }
      else{
        MS2_TYPE_TAG = "NA";
      }
      dta = THIS_DTA->ToElement()->GetText();
    }
    else{
      dta = "NA";
    }
    
    
    // peptide sequence:
    TiXmlNode* THIS_PEP = MS2_INFO->FirstChild("peptide_sequence");
    if( THIS_PEP ){
      SQ = THIS_PEP->ToElement()->GetText();
    }
    
    // protein:
    // check for alternative proteins ACs:
    TiXmlNode* ALL_PROT = MS2_INFO->FirstChild("proteins");   
    if( ALL_PROT ){
      TiXmlNode* PROT;
      for( PROT = ALL_PROT->FirstChild( "protein" ); PROT; PROT = PROT->NextSibling()){
        TiXmlElement* THIS_PROT = PROT->ToElement();
        AC.push_back( THIS_PROT->Attribute("accession_num") ); 
      }
    }

    TiXmlNode* SCORE = MS2_INFO->FirstChild("search_score");   
    if( SCORE ){
      TiXmlElement* THIS_SCORE = SCORE->ToElement();
      PEP_PROB = atof( THIS_SCORE->Attribute("value") );
    }
    
    ms2_info* TMP = new ms2_info( *(AC.begin()), SQ, PEP_PROB, IN->get_charge_state(), SCAN_MS2 );
    TMP->set_MONO_MZ( MONO_MZ );
    TMP->set_CHRG( CHRG );
    TMP->set_ORIGINAL_INTERACT_FILE( dta );
    TMP->set_MS2_TYPE_TAG( MS2_TYPE_TAG );
   
    
    // set the other ACs:
    vector<string>::iterator A = AC.begin();
    A++;
    while( A != AC.end() ){
      TMP->set_AC( *A );
      A++;
    }
    
   
    // extract AA modifications
    // add the MS2 info:
    TiXmlNode* AAs = MS2_INFO->FirstChild("modifications");
    if( AAs != NULL ){
      TiXmlNode* AA;
      for( AA = AAs->FirstChild("modification"); AA; AA = AA->NextSibling() ){
        extract_APML_AA_modifications( AA , TMP);
      }
    }
    
    IN->add_MS2_info( TMP );
    TMP = NULL;
  }
}

//////////////////////////////////////////////////////////
// extracts the modifcation 
void LC_MS_XML_reader::extract_APML_AA_modifications(TiXmlNode* AA, ms2_info* IN){
  
  if( AA != NULL ){
    
    TiXmlElement* THIS_AA = AA->ToElement();
    int pos = atoi(THIS_AA->Attribute("position"));
    double value = atof(THIS_AA->Attribute("value"));
    IN->add_modification(pos, value);
  }
}


/////////////////////////////////////////////////////////////////////////
// check if its a LC/MS XML file:
bool LC_MS_XML_reader::check_LC_MS_XML_file( string IN ){
  
  // check if its .apml format
  if( IN.find( ".apml" ) != string::npos ){
    return true;
  }
  else if( IN.find( ".tsv" ) != string::npos ){
    return true;
  }
  else{
    // if xml, check if its LC/MS data:
    /*
    TiXmlDocument* THIS_PARSER = new TiXmlDocument( IN.c_str() );
    if( THIS_PARSER->LoadFile() ){
      if( THIS_PARSER->FirstChild( "MASTER_RUN_SUMMARY" ) != NULL ){
        if( THIS_PARSER->FirstChild( "MASTER_RUN_SUMMARY" )->FirstChild( "LC_MS_RUN" ) != NULL ){
          return true; 
        }
      }
    }
    delete THIS_PARSER;
    THIS_PARSER = NULL;
     */
    return true;
  }
  
  return false;
}





//////////////////////////////////////////////////////////////////////////
// parsing of tab delim. format
void LC_MS_XML_reader::start_LC_MS_parsing_MASTER_TAB_DEL_FORMAT( string IN ){
  
  printf("\n\t\tParsing fake Master Tab-input-file '%s' \n", IN.c_str() );
  
  // open the file:
  file_reader READ;
  READ.set_current_file( IN );
  if( READ.open_file(  ) ){
    
    THIS_LC_MS = new LC_MS(IN);
    THIS_LC_MS->set_spectrum_ID(0);
    string file;
    
    READ.read_file_line();
    file = READ.get_line();
    // skip the header:
    READ.read_file_line(); 
    while( READ.get_line() ){
      
      // extract the feature:
      read_tab_del_feature( READ.get_line() );
      READ.read_file_line();  
      
    }        
    
    PARSED_LC_MS_RUNS.push_back( THIS_LC_MS );
  }
  else{
    printf("\n\t\t**** Could not open Tab-input-file %s, LC_MS_XML_writer::690 \n", MAIN_XML_FILE.c_str() );
  }
  
  
}


//////////////////////////////////////////////////////////////////////////
// extract the feature:
void LC_MS_XML_reader::read_tab_del_feature( string LINE ){
  
  feature* FEA = NULL;
  ms2_info* ms2 = NULL;
  double m_z = 2 * THIS_LC_MS->get_nb_features();
  double rt = 1 * THIS_LC_MS->get_nb_features();
  double SN = 10;
  int z = 2;
  int ID = 0;
  int FID = THIS_LC_MS->get_nb_features();
  string name;
  
  while( LINE.find( "\t") != string::npos ){
    
    string tmp = LINE;
    if( tmp.find( "\t") != string::npos ){
      tmp.erase( LINE.find( "\t"), tmp.size() - LINE.find( "\t") ); 
    }
    
    if( !tmp.empty() ){
    
      if( ms2 == NULL ){
        name = tmp;
        ms2 = new ms2_info(name, "STTTTTTTTTT", 0.9);
        
      }
      else if(( FEA == NULL ) && ( ms2 != NULL )){
        FEA = new feature(m_z, rt, 0, 0, 0, z, atof( tmp.c_str() ), 0.0, 0.0);
        FEA->add_MS2_info( ms2 );
        FEA->set_spectrum_ID( ID );
        FEA->set_feature_ID( FID );
        FEA->setSignalToNoise(SN);
        ID++;
        if( THIS_LC_MS->get_nb_features() == 0 ){
          THIS_LC_MS->add_raw_spec_name(ID, ID +" Run");
        }
        
      }
      else if( FEA != NULL){
        feature* f = new feature(m_z, rt, 0, 0, 0, z, atof( tmp.c_str() ), 0.0, 0.0);
        f->set_spectrum_ID( ID );
        f->set_feature_ID( FID );
        f->setSignalToNoise(SN);
        ID++;
        FEA->add_matched_feature(f);
        
        if( THIS_LC_MS->get_nb_features() == 0 ){
        THIS_LC_MS->add_raw_spec_name(ID, ID +" Run");
        }
        delete f;
      }
    }
    
    LINE.erase( 0, LINE.find( "\t") + 1 );
  }
  
  if( FEA != NULL ){

    feature* f = new feature(m_z, rt, 0, 0, 0, z, atof( LINE.c_str() ), 0.0, 0.0);
    f->set_spectrum_ID( ID );
    f->setSignalToNoise(SN);
    ID++;
    FEA->add_matched_feature(f);
    
    if( THIS_LC_MS->get_nb_features() == 0 ){
      THIS_LC_MS->add_raw_spec_name(ID, ID +" Run");
    }
    delete f;
    
    FEA->show_info();
    THIS_LC_MS->add_feature( FEA );
    delete FEA;
    FEA = NULL;
  }
  
}




//////////////////////////////////////////////////////////////////////
// get the LCMS id from the MasterMap or LCMS run by the LCMS run name
int LC_MS_XML_reader::get_LCM_ID_by_name( string name ){
  
  if( name.find( THIS_LC_MS->get_spec_name() ) != string::npos ){
    return THIS_LC_MS->get_spectrum_ID();
  }
  
  
  map<int, string>::iterator LCMS = THIS_LC_MS->get_raw_spec_name_start();
  while( LCMS != THIS_LC_MS->get_raw_spec_name_end() ){    
    if( name.find( (*LCMS).second ) != string::npos ){
      return (*LCMS).first;
    }
    
    LCMS++;
  }
  
  return -1;
}



//////////////////////////////////////////////////////////////////////
// process the input run:
void LC_MS_XML_reader::process_validated_LCMS( LC_MS* IN ){
  
  // set the correct LCMS id if teh run is not a mastermap:
  if( IN->check_raw_spec_name_empty() ){
    IN->setFeatureLCMSID();
  }
  
}

//////////////////////////////////////////////////////////
// get the XML file ending 
string LC_MS_XML_reader::getXMLFileEnding(){
  
  // apml format by Brusniak, M et al, submitted
  if( LC_MS_XML_reader::DATA_STORAGE_XML_FORMAT_TYPE == "APML" ){
      return ".apml";
  }      
  else if( LC_MS_XML_reader::DATA_STORAGE_XML_FORMAT_TYPE == "MuellerLN-XML" ){
    return ".xml";
  }      
  
  return ".xml";

}



///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//

#define USE_INTERACT_PARSER
#include "GENERAL_USE.h"


// **********************************************************************//
//  INTERACT_parser:
//  extracts all the search results from the different output
//  files in the sub directory
//  
//
// **********************************************************************//


double interact_parser::TR_MIN = 0;
double interact_parser::TR_MAX = 180;
double interact_parser::FEATURE_MZ_MIN = 200;
double interact_parser::FEATURE_MZ_MAX = 1800;
int interact_parser::FEATURE_CHRG_MIN = 1;
int interact_parser::FEATURE_CHRG_MAX = 5;

bool interact_parser::STATIC_GLYCOSYLATION_MOD;
double interact_parser::STATIC_C_TERM_MODIFICATION;
map<double,double> interact_parser::MOD_MASS_TRANSFORM_TABLE;
bool interact_parser::MS2_MASS_COMPARE;
double interact_parser::PEPTIDE_PROBABILITY_THRESHOLD;

////////////////////////////////////////////////
// constructor for the object INTERACT_MS_interact_parser:
interact_parser::interact_parser( ){

  // read parameters from "param.def"
  read_param* def = new read_param();
  // FT feature mono-isotopic toggle correction:
  def->search_tag("FT monoisotopic toggle correction", &FT_MONO_TOGGLE_CORRECTION);
  // MS2 mass tolerance:
  def->search_tag("MS1 m/z tolerance", &MZ_TOL);
  
  delete def;
  def = NULL;
  
}



//////////////////////////////////////////////////
// class desctructor
interact_parser::~interact_parser(){}


//////////////////////////////////////////////////
// starts the parsing process of an interact file:
void interact_parser::parseMSMSInfo(TiXmlDocument* HeadNode){
    
  // get the element of the ms search summary and go through each of them:
  TiXmlNode* ALL_MSMS_RUN_SUMMARY = HeadNode->FirstChild( "msms_pipeline_analysis" );
  TiXmlNode* MSMS_RUN_SUMMARY;
  if( ALL_MSMS_RUN_SUMMARY != NULL ){
    for( MSMS_RUN_SUMMARY = ALL_MSMS_RUN_SUMMARY->FirstChild( "msms_run_summary" ); MSMS_RUN_SUMMARY; MSMS_RUN_SUMMARY = MSMS_RUN_SUMMARY->NextSibling() ){
      
      // extract LC/MS summary info:
      extract_LC_MS_summary( MSMS_RUN_SUMMARY );
      
      TiXmlNode* SPECTRUM_QUERY;
      for( SPECTRUM_QUERY = MSMS_RUN_SUMMARY->FirstChild("spectrum_query"); SPECTRUM_QUERY; SPECTRUM_QUERY = SPECTRUM_QUERY->NextSibling() ){
        parse_SPECTRUM_QUERY( SPECTRUM_QUERY );
      }        
    }
    show_summary();
  }
  else{
    printf("\t\t**** No MS2 data available, skip file... \n" );
  }
}

//////////////////////////////////////////////////////////////////
// show parsing summary:
void interact_parser::show_summary(  ){
  
  double HIGH = 0, LOW = 0;
  vector<ms2_info>::iterator M = this->getMSMSList_START();
  while( M != this->getMSMSList_END() ){
    
    if( (*M).get_PEP_PROB() >= PEPTIDE_PROBABILITY_THRESHOLD){
      HIGH++;
    }
    else{
      LOW++; 
    }
    M++;
  }
  
  LOW /= double( this->getMSMSListSize() );
  HIGH /= double( this->getMSMSListSize() );
  printf("\t\t\t\t-> %d MS2 ids extracted (%0.2f percent high probabality scans)\n", this->getMSMSListSize(), HIGH*100.0 );

}

//////////////////////////////////////////////////////////////////
// extract LC/MS summary info:
void interact_parser::extract_LC_MS_summary( TiXmlNode* MSMS_RUN_SUMMARY ){
  
  TiXmlElement* THIS_E = MSMS_RUN_SUMMARY->ToElement();
  
  LC_MS_NAME = THIS_E->Attribute("base_name");
  
  // print out the runs:
  printf("\t\t\t---- contains MS/MS anaylsis from LC-MS run '%s'...\n", LC_MS_NAME.c_str() );
  
  // get the base name:
  int pos = LC_MS_NAME.rfind("/");
  LC_MS_NAME.erase(0,pos+1);
  
}


//////////////////////////////////////////////////////////////////
// parse SPECRUM_QUERY node:
void interact_parser::parse_SPECTRUM_QUERY( TiXmlNode* SPECTRUM_QUERY ){

  TiXmlElement* THIS_SEARCH = SPECTRUM_QUERY->ToElement();
  
  // parameters to parse out per MS2 identification:
  float PEP_PROB = 0;
  int CHRG = 0;
  double PRECURSOR_MZ = 0;
  int SCAN_START = 0;
  int SCAN_END = 0;
  string AC;
  string SQ;
  string PREV_AA;
  double TR = -1.0;
  
  
  bool VALID_SCAN = true;
  
  // make a new MS2_info objects:
  ms2_info* this_MS2 = NULL;
  this_MS2 = new ms2_info( this->getMSMSListSize() );
  
  //////////////////////////////////////////////////
  // get start / end scan, charge state, precursor mass,
  // charge state, name
  string MS2_name = THIS_SEARCH->Attribute("spectrum");
  
  
  // get the scan information:
  SCAN_START = atoi(THIS_SEARCH->Attribute("start_scan"));
  this_MS2->set_SCAN_START(SCAN_START);
  SCAN_END = atoi(THIS_SEARCH->Attribute("end_scan"));
  this_MS2->set_SCAN_END(SCAN_END);
  
  // check for special scan tag in mascot pepXML
  // contained in the spectrum attribute:
  string MascotTag = MS2_name;
  
  // check if the scans are both zero:
  if( ( SCAN_START == 0 )&&(SCAN_END == 0)){
    
    
    string tag = "FinneganScanNumber:";
    if( MascotTag.find( tag ) != string::npos){
      string mascotScan = MascotTag.substr( MascotTag.find( tag ) + tag.size() + 1, MascotTag.size());
      // set the scan:
      this_MS2->set_SCAN_START( atoi( mascotScan.c_str() ) );
      this_MS2->set_SCAN_END( atoi( mascotScan.c_str() ) );
    }
    else if ( MascotTag.find( "Scan" ) ){

      // get teh scan:
      string scan = "Scan";
      string tmp = MascotTag.substr( MascotTag.find( scan ) + scan.size(), MascotTag.size());
      string mascotScan = tmp.substr( 0, tmp.find( "(" ) );
      SCAN_START = SCAN_END = atoi( mascotScan.c_str() );
      
      // get the retention time:      
      string rt = "rt=";
      string Srt = tmp.substr( tmp.find( rt ) + rt.size(), tmp.size());
      Srt = Srt.substr( 0, Srt.find( ")" ) );
      TR = atof( Srt.c_str() );
      
    }
    else{
      
      string mascotScan = MascotTag.substr( MascotTag.find( "." ) + 1, MascotTag.size());
      mascotScan.erase(mascotScan.find( "." ), mascotScan.size());
      // set the scan:
      this_MS2->set_SCAN_START( atoi( mascotScan.c_str() ) );
      this_MS2->set_SCAN_END( atoi( mascotScan.c_str() ) );
      
    }
  }
  
  // process the scan name, remove scan number and charge state
  MS2_name = MS2_name.substr( 0, MS2_name.find(".") );
  
  this_MS2->set_ORIGINAL_INTERACT_FILE( MS2_name );   
  
  
  CHRG = atoi(THIS_SEARCH->Attribute("assumed_charge"));
  this_MS2->set_CHRG(CHRG);
  PRECURSOR_MZ = atof(THIS_SEARCH->Attribute("precursor_neutral_mass"));
  this_MS2->set_NEUTRAL_MR(PRECURSOR_MZ);
  
  /////////////////
  // mascot files dont guve start/end scan, there scan is maybe the index???
  // so store the index:
  if( ( SCAN_START == 0 ) && ( SCAN_END == 0 ) ){
    if( THIS_SEARCH->Attribute("index") ){
      SCAN_START = atoi(THIS_SEARCH->Attribute("index"));
      SCAN_END = SCAN_START;
    }
  }
  
  //////////////////////////////////////////////////
  // retention time information:
  if( THIS_SEARCH->Attribute("retention_time_sec")){
    TR = atof( THIS_SEARCH->Attribute("retention_time_sec") );
    // convert seconds to minutes:
    TR /= 60.0;
  }
  this_MS2->setRetentionTime(TR);

  TiXmlNode* SEARCH_HIT = NULL;
  if( SPECTRUM_QUERY->FirstChild( "search_result" ) != NULL ){
    
    // get now AC, SQ,
    SEARCH_HIT = SPECTRUM_QUERY->FirstChild( "search_result" )->FirstChild( "search_hit" );    
    if( SEARCH_HIT != NULL ){
      
      TiXmlElement* THIS_SEARCH_HIT_ELEMENT = SEARCH_HIT->ToElement();
      
      AC = THIS_SEARCH_HIT_ELEMENT->Attribute("protein");
      this_MS2->set_AC(  processAC(AC) );

      SQ = THIS_SEARCH_HIT_ELEMENT->Attribute("peptide");
      this_MS2->set_SQ( SQ );
      
      PREV_AA = THIS_SEARCH_HIT_ELEMENT->Attribute("peptide_prev_aa");
      this_MS2->set_PREV_AA( PREV_AA ); 
      
      // check for alternative proteins:
      TiXmlNode* ALT_PROT;    
      for( ALT_PROT = SEARCH_HIT->FirstChild( "alternative_protein" ); ALT_PROT; ALT_PROT = ALT_PROT->NextSibling()){
        TiXmlElement* THIS_ALT_PROT = ALT_PROT->ToElement();
        string name = ALT_PROT->Value(); 
        if( name == "alternative_protein" ){
          AC = THIS_ALT_PROT->Attribute("protein");
          this_MS2->set_AC( processAC(AC) );
        }
        else{
          break;
        }
      }
      
    }
    else{
      VALID_SCAN = false; 
    }
  }
  else{
    VALID_SCAN = false; 
  }
  
  /////////////////////////////////////////////////
  // search for modifications
  if( SEARCH_HIT != NULL ){
    TiXmlNode* MODIFICATION_INFO = SEARCH_HIT->FirstChild( "modification_info" );    
    
    if( MODIFICATION_INFO != NULL ){
      
      // itearat through all modifications:
      TiXmlNode* MODIFICATION;
      for( MODIFICATION = MODIFICATION_INFO->FirstChild("mod_aminoacid_mass"); MODIFICATION; MODIFICATION = MODIFICATION->NextSibling() ){
        
        
        TiXmlElement* MODIFICATION_ELEMENT = MODIFICATION->ToElement();
        int POS = atoi(MODIFICATION_ELEMENT->Attribute("position")) - 1;
        double MASS = atof(MODIFICATION_ELEMENT->Attribute("mass"));
        
        // transform the mass:
        MASS = transform_MODIFICATION( MASS );
        this_MS2->add_modification(POS, MASS);
        
      }
    }
  }
  else{
    VALID_SCAN = false; 
  }
  
  /////////////////////////////////////////////////
  // search for MS2 identification results
  if( SEARCH_HIT != NULL ){
    
    TiXmlNode* SEARCH_SCORE;
    for( SEARCH_SCORE = SEARCH_HIT->FirstChild( "search_score" ); SEARCH_SCORE; SEARCH_SCORE = SEARCH_SCORE->NextSibling() ){
      TiXmlElement* SEARCH_SCORE_ELEMENT = SEARCH_SCORE->ToElement();
      if(  SEARCH_SCORE_ELEMENT->Attribute("name") ){
        string NAME = SEARCH_SCORE_ELEMENT->Attribute("name");
        // sorcerer search results:
        if( NAME == "deltacn" ){
          double deltaCN = atof( SEARCH_SCORE_ELEMENT->Attribute("value") );
          this_MS2->set_DELTA_CN(deltaCN);
        }
        if( NAME == "xcorr" ){
          double xcorr = atof( SEARCH_SCORE_ELEMENT->Attribute("value") );
          this_MS2->set_XCORR(xcorr);
        }
        
        
        ////////////////////////////////////////////////////
        // parse here the mascot expectation score out:
        // since the smaller the prob, the more likely
        // but we are working with peptide probability which should be close to 1.0
        // make a super simple transformation: PepProb = 1.0 - MascotProb
        if( NAME == "expect" ){
          double pseudoProb = atof( SEARCH_SCORE_ELEMENT->Attribute("value") );
          pseudoProb = 1.0 - pseudoProb; 
          this_MS2->set_PEP_PROB( pseudoProb );
        }
        
      }
    }
  }
  else{
    VALID_SCAN = false; 
  }
  
  
  /////////////////////////////////////////////////
  // search for peptide prophet
  if( SEARCH_HIT != NULL ){
    TiXmlNode* ANALYSIS_RESULTS = SEARCH_HIT->FirstChild( "analysis_result" );        
    if( ANALYSIS_RESULTS != NULL ){      

      TiXmlElement* ANALYSIS_RESULTS_ELEMENT = ANALYSIS_RESULTS->ToElement();
      string NAME = ANALYSIS_RESULTS_ELEMENT->Attribute("analysis");
      if( NAME == "peptideprophet" ){
        // itearat through all modifications:
        TiXmlNode* PROPH = ANALYSIS_RESULTS->FirstChild("peptideprophet_result");
        TiXmlElement* PROPH_ELEMENT = PROPH->ToElement();
        PEP_PROB = atof(PROPH_ELEMENT->Attribute("probability"));
        this_MS2->set_PEP_PROB(PEP_PROB);
      }
    }
    else{
      VALID_SCAN = true; 
    }
  }
  else{
    VALID_SCAN = false; 
  }
  
  if( VALID_SCAN ){
    
    // process ms2_info object:
    process_MS2_info( this_MS2 );
    
    // add it now to the MS2-list:
    add_MS2_info(this_MS2);
    
    this_MS2 = NULL;
  }
  else{
    delete this_MS2;
    this_MS2 = NULL;    
  }
}


////////////////////////////////////////////////////
// process AC string:
string interact_parser::processAC(string AC){
  
  unsigned int maxCharacters = 50;
  
  // remove from |
  if( AC.size() > maxCharacters){
    AC.erase( maxCharacters, AC.size() );
  }
  
  return AC;
}


////////////////////////////////////////////////////
// process ms2_info object:
void interact_parser::process_MS2_info( ms2_info* IN ){
  
  // set the ms2 tag:
  IN->set_MS2_TYPE_TAG( "MS2" );
  
  // static glycosylations:
  if( STATIC_GLYCOSYLATION_MOD ){
    add_STATIC_glycosylation_modifications( IN );
  }

  add_STATIC_C_TERM_MODIFICATION( IN );
  
}


//////////////////////////////////////////////////////////
// add static C-terminal methylation modifcations
void interact_parser::add_STATIC_C_TERM_MODIFICATION( ms2_info* IN ){

  if( STATIC_C_TERM_MODIFICATION != 0.0 ){
    string SQ = IN->get_SQ();
    int pos = SQ.size() - 1;
    int nb = (int)SQ[pos] - (int)'A';
    double MOD = ms2_info::mono_mass[nb] + STATIC_C_TERM_MODIFICATION;
    IN->add_modification(pos, MOD );
  }
}


//////////////////////////////////////////////////////////
// add glyco site modifcations
void interact_parser::add_STATIC_glycosylation_modifications(ms2_info* IN){
  
  int nb=(int)'N'-(int)'A';
  double MOD = ms2_info::mono_mass[nb] + DEAMIDATION_SHIFT;
  
  // check for NX(S/T) motif:
  string SQ = IN->get_SQ();
  int MAX =  SQ.size();
  
  int i = 0; 
  while( i < MAX){
    
    if( i == MAX - 2 ){
      break;
    }
    // search for N:
    if( SQ[i] == 'N'){
      
      if( ( SQ[i+2] == 'S') || ( SQ[i+2] == 'T') ){
        IN->add_modification(i, MOD );
      }
    }
    
    i++;
  }
  
}

///////////////////////////////////////////////////////////
// show parsed ms2_info object:
void interact_parser::show_all_MS2_info( ){
  vector<ms2_info>::iterator P = this->getMSMSList_START(); 
  while(P != this->getMSMSList_END()){
    (*P).show_info();
    P++;
  }
}



//////////////////////////////////////////////////////////////
// check if Modification mass needs to be transformed:
double interact_parser::transform_MODIFICATION( double IN ){
  
  simple_math::ROUND_NUMBER( IN, 4 );
  
  map<double,double>::iterator F = MOD_MASS_TRANSFORM_TABLE.find( IN );
  if( F != MOD_MASS_TRANSFORM_TABLE.end()){
    IN = (*F).second;
  }
  return IN;
}


//////////////////////////////////////////////////////////////
// add features to the current spectra:
void interact_parser::add_MS2_info(ms2_info* IN){

  double M_DIFF = fabs( IN->get_THEO_MZ() - IN->get_MONO_MZ());
  bool ADD_CHECK = false;

  /////////////////////////////////////////////////////
  // check if difference between theo and precursor m/z
  if( ( M_DIFF > MZ_TOL ) && MS2_MASS_COMPARE ){
  
    // check if isotopic correction required:
    if( FT_MONO_TOGGLE_CORRECTION ){
      
      double I_DELTA = 1.0 / double(IN->get_CHRG());
      double i = 1.0; 
      while( i < IN->get_CHRG()){
        double D = fabs( M_DIFF - i*I_DELTA );
        
        if( D <= MZ_TOL){
          // correct the precursor mass:
          double NEW_P_MZ = IN->get_MONO_MZ() - i * I_DELTA;
          IN->set_MONO_MZ( NEW_P_MZ );
          ADD_CHECK = true;
          break;
        }         
        i++;
      }
    }  
    else{
      ADD_CHECK = false;
    }
  
  }
  else{
    ADD_CHECK = true;
  }
  
  
  /////////////////////////////////////////////////////
  // check for other onstraint:
  if( ADD_CHECK ){
    if( check_MS2_SCAN_VALIDITY ( IN ) ){
      this->addMSMSInfo( IN );
    }
    
    }
  else{
    /*
    if( IN->get_PEP_PROB() > 0.9 ){
      cout<<"\tbadMS2::";
      IN->show_info();
    }
     */
  }
  delete IN;
  IN = NULL;
}


//////////////////////////////////////////////////////////
// check if feature accepted
bool interact_parser::check_MS2_SCAN_VALIDITY( ms2_info* IN ){
  
  //////////////////////////
  // check the retention time range
  /*
  double TR = IN->getRetentionTime();
  if( ( ( TR < TR_MIN) && ( TR != -1.0) ) || ( ( TR > interact_parser::TR_MAX) && ( TR != -1.0) )  ){
    return false;
  }
   */
  
    
  //////////////////////////
  // charge state selection range:
  if( ( IN->get_CHRG() < FEATURE_CHRG_MIN ) || ( IN->get_CHRG() > FEATURE_CHRG_MAX ) ){
    return false;
  }
  
  //////////////////////////
  // m/z selection range:
  if( ( IN->get_MONO_MZ() < FEATURE_MZ_MIN ) && ( !feature::compareFeatureMassValuesAtPPMLevel(IN->get_MONO_MZ(), FEATURE_MZ_MIN) ) ){
    return false;
  }
  if( ( IN->get_MONO_MZ() > FEATURE_MZ_MAX ) && ( !feature::compareFeatureMassValuesAtPPMLevel(IN->get_MONO_MZ(), FEATURE_MZ_MAX) ) ){
    return false;
  }

  return true;
}




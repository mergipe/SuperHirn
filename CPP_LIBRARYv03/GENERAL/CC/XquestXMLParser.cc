///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//

#define USE_XQUEST_XML_PARSER
#include "GENERAL_USE.h"


// **********************************************************************//
//  INTERACT_parser:
//  extracts all the search results from the different output
//  files in the sub directory
//  
//
// **********************************************************************//



////////////////////////////////////////////////
// constructor for the object INTERACT_MS_XquestXMLParser:
XquestXMLParser::XquestXMLParser( ) {

  // read parameters from "param.def"
  read_param* def = new read_param();
  delete def;
  def = NULL;
  
}



//////////////////////////////////////////////////
// class desctructor
XquestXMLParser::~XquestXMLParser(){}


//////////////////////////////////////////////////
// starts the parsing process of an XQuest file:
void XquestXMLParser::parseMSMSInfo(TiXmlDocument* Root){
  
  // get the element of the ms search summary and go through each of them:
  TiXmlNode* XQUEST_RESULTS = Root->FirstChild( "xquest_results" );
  
  if( XQUEST_RESULTS != NULL ){
    
    // extract LC/MS summary info:
    extractXQuestSummary( XQUEST_RESULTS );
    
    TiXmlNode* MSMS_SCAN;
    for( MSMS_SCAN = XQUEST_RESULTS->FirstChild( "spectrum_search" ); MSMS_SCAN; MSMS_SCAN = MSMS_SCAN->NextSibling() ){
      parseXQuestSearchHit( MSMS_SCAN );
    }
    show_summary();
    //showParserMSMSInfo();
  }
  else{
    printf("\t\t**** No MS2 data available, skip file... \n" );
  }
}

//////////////////////////////////////////////////////////////////
// show parsing summary:
void XquestXMLParser::show_summary(  ){
  
  double HIGH = 0, LOW = 0;
  vector<ms2_info>::iterator M = this->getMSMSList_START();
  while( M != this->getMSMSList_END() ){
    
    if( (*M).get_PEP_PROB() >= interact_parser::PEPTIDE_PROBABILITY_THRESHOLD){
      HIGH++;
    }
    else{
      LOW++; 
    }
    M++;
  }
  
  LOW /= double( getMSMSListSize() );
  HIGH /= double( getMSMSListSize() );
  printf("\t\t\t\t-> %d MS2 ids extracted (%0.2f percent high probabality scans)\n", getMSMSListSize(), HIGH*100.0 );

}

//////////////////////////////////////////////////////////////////
// extract Xquest summary:
void XquestXMLParser::extractXQuestSummary( TiXmlNode* XQuestSummary ){
  
  TiXmlElement* THIS_E = XQuestSummary->ToElement();  
  string LC_MS_NAME = THIS_E->Attribute("outputpath");
  // print out the runs:
  printf("\t\t\t---- contains MS/MS anaylsis from xQuest file '%s'...\n", LC_MS_NAME.c_str() );
  
  
  // parse the xQuest linker mass:
  
  string xMassString = THIS_E->Attribute("monolinkmw");
  if( xMassString.find(",") != string::npos ){
    string sub = xMassString.substr( 0, xMassString.find(",") );
    xLinkerMass = atof( sub.c_str());
  }
}


//////////////////////////////////////////////////////////////////
// parse SPECRUM_QUERY node:
void XquestXMLParser::parseXQuestSearchHit( TiXmlNode* spectrumSearch ){
  
  // take only the first search hit from xquest:
  TiXmlNode* searchHitNode = spectrumSearch->FirstChild("search_hit");  
  TiXmlElement* searchHit = NULL;
  if( searchHitNode != NULL){
    searchHit = searchHitNode->ToElement();
  }
  else{
    // if not search hit, jump out of function
    return;
  }
  
  TiXmlElement* spectrumElement = spectrumSearch->ToElement();
  
  // parameters to parse out per MS2 identification:
  double PEP_PROB = 0;
  int CHRG = 0;
  double PRECURSOR_MZ = 0;
  int SCAN = 0;
  string AC;
  string SQ;
  string PREV_AA;
  double TR = -1.0;
  
  // make a new MS2_info objects:
  ms2_info* this_MS2 = NULL;
  this_MS2 = new ms2_info( this->getMSMSListSize() );
  
  //////////////////////////////////////////////////
  // get start / end scan, charge state, precursor mass,
  // charge state, name
  string MS2_name = spectrumElement->Attribute("spectrum");
  // get the start / end scan:
  string tmp = MS2_name.substr( MS2_name.find(".") + 1, MS2_name.size() );
  tmp = tmp.substr( 0, tmp.find(".") );
  SCAN = atoi(tmp.c_str());
  this_MS2->set_SCAN_END(SCAN);
  this_MS2->set_SCAN_START(SCAN);
  
  // process spec name:
  MS2_name = MS2_name.substr( 0, MS2_name.find("."));  
  this_MS2->set_ORIGINAL_INTERACT_FILE( MS2_name );   
  
  CHRG = atoi(spectrumElement->Attribute("charge_precursor"));
  this_MS2->set_CHRG(CHRG);
  PRECURSOR_MZ = atof(spectrumElement->Attribute("mz_precursor"));
  this_MS2->set_MONO_MZ(PRECURSOR_MZ);
  
  // retention time information not yet available in XQuest XML:
  /*
  if( THIS_SEARCH->Attribute("retention_time_sec")){
    TR = atof( THIS_SEARCH->Attribute("retention_time_sec") );
    // convert seconds to minutes:
    TR /= 60.0;
  }
   */
  this_MS2->setRetentionTime( TR );
  
  
  // now follows the search hit information:
  AC = searchHit->Attribute("prot1");
  AC = AC + "-" + searchHit->Attribute("prot2");
  this_MS2->set_AC(  AC );
  
  // assemble the x-linked peptides:
  SQ = searchHit->Attribute("seq1");
  SQ = SQ + searchHit->Attribute("seq2");
  this_MS2->set_SQ( SQ );

  // also this info not available:
  this_MS2->set_PREV_AA( "" ); 
  
  // add the XQuest linker modification info:
  tmp = searchHit->Attribute("seq1");
  int xPOS = tmp.size() - 1;
  //double xMASS = atof(searchHit->Attribute("xlinkermass")) + 2 * ms2_info::_MONO_H + ms2_info::_MONO_O;  
  this_MS2->add_modification(xPOS, xLinkerMass + ms2_info::getMolecularAminoAcidMass( tmp[xPOS]));
  
  ///////////////////////////////////////
  // MS/MS XQuest scoring information:
  PEP_PROB = atof(searchHit->Attribute("score"));
  this_MS2->set_PEP_PROB(PEP_PROB);  
  
  // process ms2_info object:
  process_MS2_info( this_MS2 );
  // add it now to the MS2-list:
  add_MS2_info(this_MS2);
  this_MS2 = NULL;
}



////////////////////////////////////////////////////
// process ms2_info object:
void XquestXMLParser::process_MS2_info( ms2_info* IN ){
  
  // set the ms2 tag:
  IN->set_MS2_TYPE_TAG( "xQuest" );
  
}




//////////////////////////////////////////////////////////////
// add features to the current spectra:
void XquestXMLParser::add_MS2_info(ms2_info* IN){

  
  if( check_MS2_SCAN_VALIDITY ( IN ) ){
    addMSMSInfo(IN);
  }
  delete IN;
  IN = NULL;
}


//////////////////////////////////////////////////////////
// check if feature accepted
bool XquestXMLParser::check_MS2_SCAN_VALIDITY( ms2_info* IN ){
  
  //////////////////////////
  // check the retention time range
  double TR = IN->getRetentionTime();
  if( ( ( TR < interact_parser::TR_MIN) && ( TR != -1.0) ) || ( ( TR > interact_parser::TR_MAX) && ( TR != -1.0) )  ){
    return false;
  }
  
  //////////////////////////
  // charge state selection range:
  if( ( IN->get_CHRG() < interact_parser::FEATURE_CHRG_MIN ) || ( IN->get_CHRG() > interact_parser::FEATURE_CHRG_MAX ) ){
    return false;
  }
  
  //////////////////////////
  // m/z selection range:
  if( ( IN->get_MONO_MZ() < interact_parser::FEATURE_MZ_MIN ) && ( !feature::compareFeatureMassValuesAtPPMLevel(IN->get_MONO_MZ(), interact_parser::FEATURE_MZ_MIN) ) ){
    return false;
  }
  if( ( IN->get_MONO_MZ() > interact_parser::FEATURE_MZ_MAX ) && ( !feature::compareFeatureMassValuesAtPPMLevel(IN->get_MONO_MZ(), interact_parser::FEATURE_MZ_MAX) ) ){
    return false;
  }

  return true;
}




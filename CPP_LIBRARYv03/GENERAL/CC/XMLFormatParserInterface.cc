///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//

#define USE_XML_FORMAT_PARSER_INTERFACE
#include "GENERAL_USE.h"


// **********************************************************************//
//  INTERACT_parser:
//  extracts all the search results from the different output
//  files in the sub directory
//  
//
// **********************************************************************//



////////////////////////////////////////////////
// constructor for the object INTERACT_MS_XMLFormatParserInterface:
XMLFormatParserInterface::XMLFormatParserInterface( ) {}



//////////////////////////////////////////////////
// class desctructor
XMLFormatParserInterface::~XMLFormatParserInterface(){
  MSMSInfoList.clear();
}



//////////////////////////////////////////////////
// add MS/MS information to the list
void XMLFormatParserInterface::addMSMSInfo(ms2_info* info){
  MSMSInfoList.push_back( info );
  info = NULL;
}




 ///////////////////////////////////////////////////////////
 // show parsed ms2_info object:
 void XMLFormatParserInterface::showParserMSMSInfo( ){
   vector<ms2_info>::iterator P = getMSMSList_START(); 
   while(P != getMSMSList_END()){
     (*P).show_info();
     P++;
   }
 }





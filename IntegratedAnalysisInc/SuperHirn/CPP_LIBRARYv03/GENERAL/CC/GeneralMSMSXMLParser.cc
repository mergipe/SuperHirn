///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//

#define USE_GENERAL_MSMS_XML_PARSER
#include "GENERAL_USE.h"


// **********************************************************************//
//  INTERACT_parser:
//  extracts all the search results from the different output
//  files in the sub directory
//  
//
// **********************************************************************//


////////////////////////////////////////////////
// constructor for the object INTERACT_MS_GeneralMSMSXMLParser:
GeneralMSMSXMLParser::GeneralMSMSXMLParser()
{
   XMLInt = NULL;
}

////////////////////////////////////////////////
// constructor for the object INTERACT_MS_GeneralMSMSXMLParser:
GeneralMSMSXMLParser::GeneralMSMSXMLParser(string file): GeneralXMLParser(file ){
  XMLInt = NULL;
}



//////////////////////////////////////////////////
// class desctructor
GeneralMSMSXMLParser::~GeneralMSMSXMLParser(){  
  if( XMLInt != NULL ){
    delete XMLInt;
    XMLInt = NULL;
  }  
}


bool GeneralMSMSXMLParser::startMSMSXMLFileParsing(string file)
{
  this->setXMLParsingFile( file );
  return this->startMSMSXMLFileParsing();
}



bool GeneralMSMSXMLParser::startMSMSXMLFileParsing(){

  if( XMLInt != NULL ){
    delete XMLInt;
    XMLInt = NULL;
    return true;
  }  
  
  // process the file:
  completeXMLFile();
  
  // open the XML file:
  if( this->openXMLFile() ){
    

    // check which type of file it is:
    TiXmlNode* HEAD = NULL;
    
    
    ///////////////////////////////////
    // check for PEPXML format:
    HEAD = getFirstChild( "msms_pipeline_analysis" );
    TiXmlNode* TEST = this->getXMLDocument( )->FirstChild( "msms_pipeline_analysis" );
    if( TEST != NULL ){
      XMLInt = new interact_parser( );
    }
    ///////////////////////////////////
    // check for XQuest format:
    HEAD = getFirstChild( "xquest_results" );
    TEST = this->getXMLDocument( )->FirstChild( "xquest_results" );
    if( TEST != NULL ){
      XMLInt = new XquestXMLParser( );
    }
    
    // if correct XML, then start parsing:
    if( XMLInt != NULL ){
      XMLInt->parseMSMSInfo( this->getXMLDocument( ) );
      
      vector<ms2_info>::iterator Start = XMLInt->getMSMSList_START();
      while( Start != XMLInt->getMSMSList_END() )
        {
          this->MSMSInfoList.push_back( *Start );
          Start++;
        }
      return true;
    }
  }
  return false;
}



////////////////////////////////////////////////////////////////
// check for the correct file name, either "_c.xml" or "_p.xml"
void GeneralMSMSXMLParser::completeXMLFile(){
  
  file_sys CHECK;
  string TMP = getXMLParsingFile( );
  
  // remove the last _p
  string suffix = "_p";
  if( TMP.rfind( suffix ) != string::npos ){
    TMP.erase( TMP.rfind( suffix ), TMP.size( ) - TMP.rfind( suffix ) );
  }
  
  // remove the last _c
  suffix = "_c";
  if( TMP.rfind( suffix ) != string::npos ){
    TMP.erase( TMP.rfind( suffix ), TMP.size( ) - TMP.rfind( suffix ) );
  }
  
  if( CHECK.check_file_exists_in_Dir( TMP , ".xml" ) ){
    setXMLParsingFile( CHECK.get_file_name_in_Dir( TMP , ".xml" ) );
  }
  
}

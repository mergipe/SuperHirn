///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//

#define USE_GENERAL_XML_PARSER
#include "GENERAL_USE.h"


// **********************************************************************//
//  INTERACT_parser:
//  extracts all the search results from the different output
//  files in the sub directory
//  
//
// **********************************************************************//


////////////////////////////////////////////////
// constructor for the object INTERACT_MS_GeneralXMLParser:
GeneralXMLParser::GeneralXMLParser( ){
  
  // copy the target file string:
  XML_PARSER = NULL;
  
}

////////////////////////////////////////////////
// constructor for the object INTERACT_MS_GeneralXMLParser:
GeneralXMLParser::GeneralXMLParser(string IN){

  // copy the target file string:
  PARSING_FILE = IN; 
  XML_PARSER = NULL;
  
}



//////////////////////////////////////////////////
// class desctructor
GeneralXMLParser::~GeneralXMLParser(){  
  if( XML_PARSER != NULL ){
    delete XML_PARSER;
    XML_PARSER = NULL;
  }
}


//////////////////////////////////////////////////
// starts the parsing process of the input file:
bool GeneralXMLParser::openXMLFile(){
  
  
  if( XML_PARSER != NULL ){
    delete XML_PARSER;
    XML_PARSER = NULL;
  }
  
  XML_PARSER = new TiXmlDocument( PARSING_FILE.c_str() );
  if( XML_PARSER->LoadFile() ){
    
    // start the parsing:
    printf("\t\t\t---- parsing file '%s' ...\n", PARSING_FILE.c_str() );
    
    return true;
  }
  else{
    printf("\t\t**** Could not open XML file %s, GeneralXMLParser::61 \n", PARSING_FILE.c_str() );
    return false;
  }
  
}


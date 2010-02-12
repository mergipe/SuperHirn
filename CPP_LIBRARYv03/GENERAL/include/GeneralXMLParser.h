///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//


#ifndef _GENERAL_XML_PARSER_H
#define _GENERAL_XML_PARSER_H

// **********************************************************************//
// CLASS INTERACT_PARSER:
// extracts all the search results from the different output
// files in the sub directory
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

class GeneralXMLParser{


    ////////////////////////////////////////////////
    // declaration of the private members:

private:
  
  // target path in file system
  string PARSING_FILE;
  TiXmlDocument* XML_PARSER;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
 public:
    
  // class destructor
  ~GeneralXMLParser();
  // class constructor
  GeneralXMLParser();
  // class constructor
  GeneralXMLParser(string);
  
  // starts the parsing process of the input file:
  bool openXMLFile();
  
  // get node from first child:
  TiXmlNode* getFirstChild( string key ){ return XML_PARSER->FirstChild( );};
  // get node from first child:
  TiXmlDocument* getXMLDocument( ){ return XML_PARSER;};
  
  // set / get teh name of the parsed file:
  string getXMLParsingFile( ){return PARSING_FILE;};
  void setXMLParsingFile( string in ){PARSING_FILE = in;};
  
};

#endif


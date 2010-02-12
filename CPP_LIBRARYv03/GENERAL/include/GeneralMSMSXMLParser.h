///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//


#ifndef _GENERAL_MSMS_XML_PARSER_H
#define _GENERAL_MSMS_XML_PARSER_H

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

class GeneralMSMSXMLParser : public GeneralXMLParser{


  
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the private members:
  
  // check for the correct file name, either "_c.xml" or "_p.xml"
  void completeXMLFile();
  
  XMLFormatParserInterface* XMLInt;

  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
 public:
    
  // class destructor
  ~GeneralMSMSXMLParser();
  // class constructor
  GeneralMSMSXMLParser();
  // class constructor
  GeneralMSMSXMLParser(string);
  
  // starts the parsing process of the input file:
  bool startMSMSXMLFileParsing();  
  
  // access the parsed MS/MS information:
  vector<ms2_info>::iterator getMSMSListStart(){return XMLInt->getMSMSList_START();};
  vector<ms2_info>::iterator getMSMSListEnd(){return XMLInt->getMSMSList_END();};

  
};

#endif


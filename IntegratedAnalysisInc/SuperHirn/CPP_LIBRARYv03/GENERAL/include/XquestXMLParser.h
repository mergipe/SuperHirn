///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//


#ifndef _XQUEST_XML_PARSER_H
#define _XQUEST_XML_PARSER_H

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

class XquestXMLParser: public XMLFormatParserInterface{


  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  double xLinkerMass;
  
  // extract Xquest summary summary info:
  void extractXQuestSummary( TiXmlNode* );
  
  // parse a search hit of xquest:
  void parseXQuestSearchHit( TiXmlNode*);

  //add features to the current spectra:
  void add_MS2_info(ms2_info*);
  
  // process ms2_info object:
  void process_MS2_info( ms2_info* );
  
  // show parsing summary:
  void show_summary(  );
  
  // check if feature accepted
  bool check_MS2_SCAN_VALIDITY( ms2_info* );
  
public:

  ////////////////////////////////////////////////
  // declaration of the public members:

  // class destructor
  ~XquestXMLParser();
  // class constructor
  XquestXMLParser( );
  
  // starts the parsing process of an XQuest file:
  void parseMSMSInfo(TiXmlDocument* );
  
  
};

#endif


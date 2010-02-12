///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//


#ifndef _XML_FORMAT_PARSER_INTERFACE_H
#define _XML_FORMAT_PARSER_INTERFACE_H

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

class XMLFormatParserInterface{


    ////////////////////////////////////////////////
    // declaration of the private members:

protected:
  
  vector<ms2_info> MSMSInfoList;
    
 public:

  ////////////////////////////////////////////////
  // declaration of the public members:

  // class destructor
  virtual ~XMLFormatParserInterface();
  // class constructor
  XMLFormatParserInterface( );
  
  // starts the parsing process of an XML file:
  virtual void parseMSMSInfo(TiXmlDocument* ) = 0;
  // add MS/MS information to the list
  void addMSMSInfo(ms2_info*);
  // show parsed ms2_info object:
  void showParserMSMSInfo( );
    

  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // access the list:
  int getMSMSListSize(){return MSMSInfoList.size();};
  vector<ms2_info> getMSMSList(){return MSMSInfoList;};
  vector<ms2_info>::iterator getMSMSList_START(){return MSMSInfoList.begin();};
  vector<ms2_info>::iterator getMSMSList_END(){return MSMSInfoList.end();};

};

#endif


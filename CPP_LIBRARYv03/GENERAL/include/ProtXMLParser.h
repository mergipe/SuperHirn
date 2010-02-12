///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//


#ifndef PROT_XML_PARSER_H
#define PROT_XML_PARSER_H

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

class ProtXMLParser : public GeneralXMLParser{


    ////////////////////////////////////////////////
    // declaration of the private members:

private:
  
  
  map<string, MS2ProteinGroup> proteinGroups;
  
  MS2ProteinGroup* currentProteinGroup;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
 public:

  // class destructor
  ~ProtXMLParser();
  // class constructor
  ProtXMLParser(string);
  ProtXMLParser( );
  

  // starts the parsing process of an protxml file:
  void parseProtXMLFile();
  
  // starts the parsing process of a directory with protxml files:
  void parseProtXMLFileDirectory( string );

  // add a protein to the map:
  void addProteinGroup( MS2ProteinGroup*);

  // extract protein summary info:
  void extractProteinGroupInfo( TiXmlNode* );
  
  // parse a peptide node:
  void parsePeptide( TiXmlNode*);

  // find the most likely peptide for a ms2 scan
  void checkProteinBeloning( ms2_info* );
  
  // check for the correct file name, either "_c.xml" or "_p.xml"
  void completeProtXMLFile();

  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // get size of the list:
  int getNBProteinGroups(){return proteinGroups.size();};
  
  map<string, MS2ProteinGroup> getMS2ProteinGroupList(){return proteinGroups;};
  map<string,MS2ProteinGroup>::iterator getMS2ProteinGroupStart(){return proteinGroups.begin();};
  map<string,MS2ProteinGroup>::iterator getMS2ProteinGroupEND(){return proteinGroups.end();};
  
};

#endif


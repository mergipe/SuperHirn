///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//


#ifndef INTERACT_PARSER_H
#define INTERACT_PARSER_H

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

class interact_parser : public XMLFormatParserInterface{


    ////////////////////////////////////////////////
    // declaration of the private members:

private:
  
  bool FT_MONO_TOGGLE_CORRECTION;
  double MZ_TOL;
  
  // LC/MS parameters:
  string LC_MS_NAME;  
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
 public:
    
    static double STATIC_C_TERM_MODIFICATION;
  static bool STATIC_GLYCOSYLATION_MOD;
  static map<double,double> MOD_MASS_TRANSFORM_TABLE;
  static bool MS2_MASS_COMPARE;
  static double PEPTIDE_PROBABILITY_THRESHOLD;
  
  static double TR_MIN;
  static double TR_MAX;
  static double FEATURE_MZ_MIN;
  static double FEATURE_MZ_MAX;
  static int FEATURE_CHRG_MIN;
  static int FEATURE_CHRG_MAX;
  

  // class destructor
  ~interact_parser();
  // class constructor
  interact_parser( );
  

  // extract LC/MS summary info:
  void extract_LC_MS_summary( TiXmlNode* );
  
  // starts the parsing process of an interact file:
  void parseMSMSInfo(TiXmlDocument* );
  
  // parse SPECRUM_QUERY node:
  void parse_SPECTRUM_QUERY( TiXmlNode*);
  
  
  //add features to the current spectra:
  void add_MS2_info(ms2_info*);
  
  
  // check if Modification mass needs to be transformed:
  double transform_MODIFICATION( double );
  
  // process ms2_info object:
  void process_MS2_info( ms2_info* );
  // process AC string:
  string processAC(string AC);
    
  // add glyco site modifcations
  void add_STATIC_glycosylation_modifications(ms2_info*);
  // add static C-terminal methylation modifcations
  void add_STATIC_C_TERM_MODIFICATION( ms2_info* );
  // show parsed ms2_info object:
  void show_all_MS2_info();
  
  // show parsing summary:
  void show_summary(  );

  // check if feature accepted
  bool check_MS2_SCAN_VALIDITY( ms2_info* );
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  string get_LC_MS_NAME(){return LC_MS_NAME;};

  
};

#endif


///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS IMPORTER:
// management of the input LC-MS spectra
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef IMPORTER_H
#define IMPORTER_H


class importer{

  
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:

  
  // stores all the parsed lc-ms runs at one timepoint
  // will be processed in the pre analysis;
  // -> for example merge double features
  // -> order by mass etc.
  vector <LC_MS> lc_ms_spectra;

  string target;
  string SOURCE_DIR;
  
  // file reader object:
  file_reader* reader;
    
  // elution window start /end:
  double elution_window_start;
  double elution_window_end;
  double MZ2_tolerance;
  
  // counter which sets the spectrum ids
  unsigned int id_counter;
  int feature_count;
  
  LC_MS* LC_MS_UPDATE;
  LC_MS* MASTER;
  ////////////////////////////////////////////////
  // declaration of the public members:
  
 public:
  
  // class destructor
  ~importer();
  
  // class constructor
  importer();
  
  // show all spectra in the vector:
  void show_spectra();
  
  // show info of a lc-ms spectrum:
  void show_lc_ms_info(int i){lc_ms_spectra[i].show_info();};
  
  //////////////////////////////////////////////////////////
  // these functions below to reade the processed lc_ms:
  //
  // read the lc-ms output files from LS-MS-XML
  void read_XML_files(string);

  //////////////////////////////////////////////////////////
  
  //############################################################################################
  //
  // FUNCTIONS TO read the MASTER LC/MS run from a text file!!!!
  //
  // reads a MASTER LC/MS from a file:
  void read_MASTER_from_file(string);
  // process the input run:
  void process_validated_LCMS( LC_MS* );

  
  //############################################################################################
  //
  // FUNCTIONS TO PARSE XML INTERACT DATA
  //
  // A. function which opens teh xml file "read_current_xml_file(char* file)"
  // B. function wihch searches the start tag of a feature and extracts
  //    the information from this tag, "get_data_from_xml_tag()"
  //
  // reads from the current XML - file:
  void read_current_xml_file();
    
  // add MS2 info to the LC/MS run:
  void add_MS2_info_to_run(ms2_info*);
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  
  // access methods to the spectra:
  LC_MS* get_spectra(int i){return &lc_ms_spectra[i];};
  
  // get number of spectra added:
  int get_nb_spectra(){return lc_ms_spectra.size();};
  

  //////////////////////////////////////////////////////////
  // get the whole feature list:
  vector<feature> get_feature_list(int i){return lc_ms_spectra[i].get_feature_list();};
  
  //////////////////////////////////////////////////////////
  // get the whole LC_MS list:
  vector<LC_MS> get_lc_ms_list(){return lc_ms_spectra;};
  int get_nb_features(int i){return lc_ms_spectra[i].get_nb_features();};
  
  // get name of a spectrum:
  string get_spec_name(int i){return lc_ms_spectra[i].get_spec_name();};

  void set_LC_MS_UPDATE(LC_MS* IN){LC_MS_UPDATE = IN;};
  LC_MS* get_MASTER(){return MASTER;};
  
};

#endif


///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS LCMSDataImporter:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef LCMS_DATA_IMPORTER_H
#define LCMS_DATA_IMPORTER_H


class GeneralMSMSXMLParser;
class LCMSDataImporter{

  
public:

    static bool CREATE_FEATURE_ELUTION_PROFILES;
 
  // class destructor
  ~LCMSDataImporter();
  
  // class constructor
  LCMSDataImporter( );
  // class copy constructor
  LCMSDataImporter(const LCMSDataImporter&);
  
   
  // extract mzXML runs from raw data, i.e. mzXML files:
  void start_peak_extraction_from_mzxml_data();
  
  // parses LC/MS from XML fiels, i.e. .xml files:
  void parse_from_XML_data();
  
  // write teh parseed LC/MS into directory:
  void write_out_parsed_LC_MS();
    
  
 
  //////////////////////////////////////////////////
  // overload operators:
  LCMSDataImporter& operator=(const LCMSDataImporter&);
  LCMSDataImporter& operator<=(const LCMSDataImporter&);
  LCMSDataImporter& operator>=(const LCMSDataImporter&);
  LCMSDataImporter& operator<(const LCMSDataImporter&);
  LCMSDataImporter& operator>(const LCMSDataImporter&);
  
  
  
  //////////////////////////////////////////////////
  // NOTE by Vagisha: added this method to do all the post processing on the LC_MS objects. 
  void doPostProcessing();
  // NOTE by Vagisha: added this function
  void order_by_mass();

  /////////////////////////////////////////////////////
  // NOTE by Vagisha: moved this out of the parse_fromXML_data method
  // Should have been common to both parse_form_raw_data() and parse_from_XML_data() methods. ASK LUKAS!!!
  void add_interact_info();
    
  // assign LC-MS ids to feature
  void assigenLCMSIdToFeatures();

    
  // process extracted LC_peaks:
  void process_extracted_LC_peaks();
  // process input LC/MS runs:
  void process_input_LC_MS_runs();
  // parses LC-MS runs from XML files:
  void parseLCMSDataFromXML(string);
  // reads a MASTER LC-MS :
  LC_MS* read_MASTER_MAP( );
    
  
  /////////////////////////////////////////////////////
  // parses MASTER MAPS from XML files, i.e. .xml files:
  void parse_from_MASTER_MAPS();
  // post processing on the Master Maps 
  void doMasterMapPostProcessing();
  //  renumerate the child LC-MS ids:
  void renummerateChildIDs();
  // renummerated the ids of aligned features in the corresponding master map
  void renummeratedAlignedFeaturesInMasterMap( LC_MS*, map<int, int>* ) ;



    

  // filter LC peaks in all LC/MS runs by
  // their LC scores:
  void filter_data_by_LC_score();
    
  //////////////////////////
  // write in XML fomrat:
  //
  // writes a LC/MS run into XML formatted file:
  void write_out_LC_MS_run(LC_MS*, string);
  
  
  // set the path from extarenal for the:
  // INTERACT PEPXML INPUT DATA:
  void setExternalInteractPepXMLPath( string in){ ORIGINAL_INTERACT_DIR = in;};
  // MZXML RAW INPUT DATA:
  void setExternalMzXMLRawPath( string in){ MZXMLINPUT_DIR = in;};

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // get the vector of LC/MS runs:
  vector<LC_MS> get_parsed_DATA(){return LC_MS_RUNS;};
  vector<LC_MS>::iterator get_parsed_DATA_START(){return LC_MS_RUNS.begin();};
  vector<LC_MS>::iterator get_parsed_DATA_END(){return LC_MS_RUNS.end();};
  
  
private:

  string MZXMLINPUT_DIR;
  string LC_MS_RUN_DIR;
  string ORIGINAL_INTERACT_DIR;
  string PROT_XML_DIR;
  
  vector<LC_MS> LC_MS_RUNS;
  
  GeneralMSMSXMLParser* pepXMLParser;
  
  /// \brief Method to extraction pepXML information from the specified folder/file (ORIGINAL_INTERACT_DIR)
  void extractPepXMLInformation();

  /// \brief Method to assign extracted MS/MS information to the input LC-MS run
  /// \param iRun LC_MS* LC-MS run to add pepXML information
  void addPepXMLInfoToLCMS(LC_MS* iRun);
  
  /// \brief get the output directory to which processed results are stored
  /// \return string
  string getOutputDirectory();

  /// \brief Write a LC-MS run to XML
  /// \param iRun LC_MS* pointer to the LC-MS instance
  void writeLCMSToXML(LC_MS* iRun);
  
  
  LC_MS* findLCMSByMS2Name( string MS2_Name );



  
};

#endif

    

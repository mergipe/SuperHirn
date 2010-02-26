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

//#define USE_IMPORTER
#define USE_INPUT
#define USE_LCMS_DATA_IMPORTER

#include "use.h"

bool LCMSDataImporter::CREATE_FEATURE_ELUTION_PROFILES = false;

////////////////////////////////////////////////
// constructor for the object LCMSDataImporter:
LCMSDataImporter::LCMSDataImporter( ){
  

  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  
  // read mzXML / .xml paths:
  MZXMLINPUT_DIR = def->search_tag( "MZXML DIRECTORY" );
  LC_MS_RUN_DIR = def->search_tag( "MY PROJECT NAME" );

  if( LC_MS_RUN_DIR[ LC_MS_RUN_DIR.size() - 1] != '/'){
    LC_MS_RUN_DIR += "/";
  }
    
  // If the pepxml_dir was not specified on the command-line, look for it in the parameters file.
  if (ORIGINAL_INTERACT_DIR.empty())
    ORIGINAL_INTERACT_DIR = def->search_tag("PEPXML DIRECTORY");
  if( ORIGINAL_INTERACT_DIR[ORIGINAL_INTERACT_DIR.size() - 1] != '/' && !ORIGINAL_INTERACT_DIR.empty()){
    ORIGINAL_INTERACT_DIR += '/';
  }  
  
  if (PROT_XML_DIR.empty())
    PROT_XML_DIR = def->search_tag("PROTXML DIRECTORY");
  if( PROT_XML_DIR[PROT_XML_DIR.size() - 1] != '/' && !PROT_XML_DIR.empty()){
    PROT_XML_DIR += '/';
  }  
  
    
  delete def;
  def = NULL;
  
  
}

//////////////////////////////////////////////////
// class desctructor of LCMSDataImporter
LCMSDataImporter::~LCMSDataImporter(){
  LC_MS_RUNS.clear();
}

//////////////////////////////////////////////////
// class copy constructor of LCMSDataImporter
LCMSDataImporter::LCMSDataImporter(const LCMSDataImporter& tmp){}


//////////////////////////////////////////////////
// copy constructor:
LCMSDataImporter& LCMSDataImporter::operator=(const LCMSDataImporter& tmp){
  return *this;
}
    


//////////////////////////////////////////////////
// process input LC/MS runs:
void LCMSDataImporter::process_input_LC_MS_runs(){

  vector<LC_MS>::iterator P = LC_MS_RUNS.begin();
  while( P != LC_MS_RUNS.end() ){
    
    // set the correct LCMS id if teh run is not a mastermap:
    if( (*P).check_raw_spec_name_empty() ){
      (*P).setFeatureLCMSID();
      
      // mass calibration
      LCMSmassCalibrator massCali;
      massCali.START_LCMS_MASS_CALIBRATION( &(*P) );
      //massCali.plotMassShifts();
  
      if( MS1_feature_merger::MS1_FEATURE_CLUSTERING ){
        MS1_feature_merger* merg = new MS1_feature_merger( &(*P) );
        merg->startFeatureMerging();
        delete merg;
        merg = NULL;
      }
      
    }
    P++;
  }
  
  //MS2_MS1_matcher::showAmbiguouities();

}


//////////////////////////////////////////////////
// NOTE by Vagisha: added this method to do all the post processing on the LC_MS objects. 
void LCMSDataImporter::doPostProcessing() {

  // order features by mass:
  order_by_mass();
  
  // NO need to do filtering; data read from xml files is filtered by the xml reader
  // data parsed from mzxml files is filtered in the do_peak_extraction method
  //filter_data_by_LC_score();
  process_input_LC_MS_runs();  
  
  // assign LC-MS ids to feature
  assigenLCMSIdToFeatures();
  
  // add interact data:
  add_interact_info();
  
 }

//////////////////////////////////////////////////
// post processing on the Master Maps 
void LCMSDataImporter::doMasterMapPostProcessing() {
  
  // order features by mass:
  order_by_mass();  

  // set the MasterMap ids:
  int counter = 0;
  vector<LC_MS>::iterator P = LC_MS_RUNS.begin();
  while( P != LC_MS_RUNS.end() ){
    P->set_MASTER_ID( counter );
    P->set_spectrum_ID( counter );
    counter++;
    P++;
  }
  
  // renumerate the child LC-MS ids:
  renummerateChildIDs( );
  
}


//////////////////////////////////////////////////
// renumerate the child LC-MS ids:
void LCMSDataImporter::renummerateChildIDs( ) {
    
  // set the MasterMap ids:
  int childID = 0;
  vector<LC_MS>::iterator P = LC_MS_RUNS.begin();
  while( P != LC_MS_RUNS.end() ){
  
    // create first the mapping of the ids:
    map<int, int> idMap;
    
    map<int, string>::iterator C = P->get_raw_spec_name_start();
    while( C != P->get_raw_spec_name_end() ){
      idMap.insert( make_pair( C->first, childID ) );
      childID++;
      C++;
    }

    // remap the child LC-MS:
    map<int, string> oldChildNameMap = P->get_raw_spec_name_map();
    map<int, int>::iterator M = idMap.begin();
    while( M != idMap.end() ){
  
      map<int, string>::iterator name = oldChildNameMap.find( M->first );
      P->remove_raw_spec_name( M->first );
      P->add_raw_spec_name( M->second, name->second );
      M++;
      
    }
    
    // renummerated the ids of aligned features in the corresponding master map
    renummeratedAlignedFeaturesInMasterMap( &(*P), &idMap );
    P++;
  }
  
}




//////////////////////////////////////////////////
// NOTE by Vagisha: added this function
void LCMSDataImporter::order_by_mass() {
  
  printf("\n\t\t-- Postprocessing of extracted LC-MS runs: ... \n");
  vector<LC_MS>::iterator P = LC_MS_RUNS.begin();
  while( P != LC_MS_RUNS.end() ){
    printf("\t\t --- sorting features in LC-MS %s\n", P->get_spec_name().c_str());
    P->order_by_mass();
    P++;
  }
  
}


//////////////////////////////////////////////////
// assign LC-MS ids to feature
void LCMSDataImporter::assigenLCMSIdToFeatures() {
  
  vector<LC_MS>::iterator P = LC_MS_RUNS.begin();
  while( P != LC_MS_RUNS.end() ){

    LC_MS* run = &(*P);
    
    vector<LC_MS_FEATURE>::iterator F = run->get_feature_list_begin();
    while( F != run->get_feature_list_end() ){
      
      feature* fea = &(*F);
      fea->set_spectrum_ID( run->get_spectrum_ID() );
      
      F++;
    }
    
    P++;
  }
  
}



/////////////////////////////////////////////////////////
// parses LC/MS from XML files, i.e. .xml files:
void LCMSDataImporter::parse_from_XML_data(){
 
  
  // parse the MS1 LC/MS runs:  
  string targetDir = "ANALYSIS_";
  targetDir += LC_MS_RUN_DIR + "LC_MS_RUNS/";
  
  parseLCMSDataFromXML( targetDir );
  doPostProcessing();
  
}  


/////////////////////////////////////////////////////////
// parses LC-MS runs from XML files:
void LCMSDataImporter::parseLCMSDataFromXML(string targetDir){
  
   // create a new LC_MS reader:
  LC_MS_XML_reader* READ = new LC_MS_XML_reader();
  
  // now read all features:
  READ->start_LC_MS_parsing( targetDir );
  
  // get the parsed run:
  vector<LC_MS>::iterator P = READ->get_parsed_LCMS_list_start();
  while( P != READ->get_parsed_LCMS_list_end() ){
    
    LC_MS* TMP = &(*P);
    
    if( TMP != NULL ){      
      
      TMP->set_spectrum_ID(LC_MS_RUNS.size());
      
      // set the correct LCMS ID if the run is not a mastermap:
      if( TMP->check_raw_spec_name_empty() ){
        TMP->setFeatureLCMSID();
      }
      
      // add the LC/MS run to the vector
      LC_MS_RUNS.push_back( *TMP );
      TMP = NULL;
    }
    P++;
  }
  
  
  delete READ;
  READ = NULL;
  
  
}  


//////////////////////////////////////////////////
// reads a MASTER LC-MS :
LC_MS* LCMSDataImporter::read_MASTER_MAP( ){
  
  ////////////////////////////////
  // get the input name from the input source directory:
  read_param* def = new read_param();
  string projName = def->search_tag("MY PROJECT NAME");
  // read input directory:
  string projDir = "ANALYSIS_" + projName;
  delete def;
  def = NULL;  
  // add backslash at end:
  if(projDir[projDir.size()-1] != '/'){
    projDir += "/";  
  }
  
  
  ////////////////////////////////
  // check which kind of master map name:
  file_sys fchecker;
  string MMname = "";

  // check PROCESSED_MASTER.xml:
  if( fchecker.check_file_exists( projDir + "PROCESSED_MASTER.xml" ) ){
    MMname = projDir + "PROCESSED_MASTER.xml";
  }
  // check <PROJECT NAME>.xml:
  else if( fchecker.check_file_exists( projDir + projName + ".xml" ) ){
    MMname = projDir + projName + ".xml";
  }
  
  if( MMname.size() != 0 ){

    printf("\n\t\t- importing MASTER run from '%s'\n", MMname.c_str());
    
    
    /////////////////////////////////
    // create a new LC_MS reader:
    LC_MS_XML_reader* READ = new LC_MS_XML_reader( );
    
    // now read all features:
    READ->start_LC_MS_parsing( MMname );
    return &(*READ->get_parsed_LCMS_list_start());
  }
  
  return NULL;
}
 

/////////////////////////////////////////////////////////
// parses MASTER MAPS from XML files, i.e. .xml files:
void LCMSDataImporter::parse_from_MASTER_MAPS(){

  // parse the MS1 LC/MS runs:  
  string targetDir = "ANALYSIS_";
  targetDir += LC_MS_RUN_DIR + "MASTER_MAPS/";
  
  parseLCMSDataFromXML( targetDir );
  
  doMasterMapPostProcessing();
  
}  





/////////////////////////////////////////////////////
// NOTE by Vagisha: moved this out of the parse_fromXML_data method
// Should have been common to both parse_form_raw_data() and parse_from_XML_data() methods. ASK LUKAS!!!
void LCMSDataImporter::add_interact_info() {
  
  /////////////////////////////////////////////////////
  // add here the interact information:
  
  if( !ORIGINAL_INTERACT_DIR.empty() ){
    
    if( ORIGINAL_INTERACT_DIR[ORIGINAL_INTERACT_DIR.size() - 1] != '/'){
      ORIGINAL_INTERACT_DIR += '/';
    }
    
    bool POST_ANNOTATION = true;
    
    //////////////////////////////////////////////////////////////
    // check if protXML information is available:
    // if so parse this first:
    ProtXMLParser* PPAR = NULL;
    if( !PROT_XML_DIR.empty() ){
      printf("\n\t\t\t--- parsing protXML information from '%s'...\n", ORIGINAL_INTERACT_DIR.c_str());
      PPAR = new ProtXMLParser( );
      PPAR->parseProtXMLFileDirectory( PROT_XML_DIR );
    }
    
    printf("\n\t\t\t--- parsing pepXML information from '%s'...\n", ORIGINAL_INTERACT_DIR.c_str());
    
    //////////////////////////////////////////////////////////////
    // check if post annotate data is required:
    // in equal one interact file for all the runs:
    // do only if no run-specific interact file has been found before:
    if( POST_ANNOTATION ){

      file_sys accessor;
      accessor.read_dir( ORIGINAL_INTERACT_DIR, ".xml");
      char* file = accessor.get_next_file();
      while(file != NULL){        
        
        // printf("\n\t\tPost-Annotation of LC-MS runs by '%s'\n",file);        
        GeneralMSMSXMLParser* PAR = NULL;
        PAR = new GeneralMSMSXMLParser( file );
        if( PAR->startMSMSXMLFileParsing() ){
        
        
          // try to add now a scan depending on its spectrum name
          // to the corresponing LC/MS run in the batch:
          // now get the ms2_info and transform to features:
          MS2_MS1_matcher* MATCHER = new MS2_MS1_matcher();
          vector<ms2_info>::iterator X = PAR->getMSMSListStart();
          while( X != PAR->getMSMSListEnd()){
            
            // adjust the ACs:
            if( PPAR != NULL){
              PPAR->checkProteinBeloning( &(*X) );
            }
            
            // get the spectrum name:
            string MS2_name = (*X).get_ORIGINAL_INTERACT_FILE();
            
            // remove the last _p
            string suffix = "_p";
            if( MS2_name.rfind( suffix ) != string::npos ){
              MS2_name.erase( MS2_name.rfind( suffix ), MS2_name.size( ) - MS2_name.rfind( suffix ) );
            }
            // remove the last _c
            suffix = "_c";
            if( MS2_name.rfind( suffix ) != string::npos ){
              MS2_name.erase( MS2_name.rfind( suffix ), MS2_name.size( ) - MS2_name.rfind( suffix ) );
            }
            
            // find the corresponing LC/MS run:
            LC_MS* THIS_RUN = NULL;
            vector<LC_MS>::iterator LCMS = get_parsed_DATA_START();
            while( LCMS != get_parsed_DATA_END() ){
              if( (*LCMS).get_spec_name().find( MS2_name ) != string::npos  ){
                THIS_RUN = &(*LCMS);
                break;
              }
              LCMS++;
            }
            
            if( THIS_RUN ){
              // matches a MS2-info to a feature:
              MATCHER->combine_LC_MS_specific_MS2_to_MS1_data(THIS_RUN, &(*X));
            }
            else{
              // printf( "\t\t runs %s not found!\n", MS2_name.c_str() );
            }
            X++; 
          }
      
          MATCHER->show_annotation_overview();
          delete MATCHER;
          MATCHER = NULL;
        }
        
        // next mzXML file:
        file = accessor.get_next_file();
        
        delete PAR;
        PAR = NULL;
      }
      
    }
    
    if( PPAR != NULL ){
      delete PPAR;
      PPAR = NULL;
    }
    
  }
  
}

/////////////////////////////////////////////////////////
// extract mzXML runs from raw data, i.e. mzXML files:
void LCMSDataImporter::start_peak_extraction_from_mzxml_data(){
    
  // map containig all runs in alphabetical order:
  map<string, LC_MS> LC_MS_NAME_MAP;

  //////////////////////////////////////////////////
  // start the peak extraction of mzXML data:
  FT_PeakDetectController* FT_controller = new FT_PeakDetectController( MZXMLINPUT_DIR );
  FT_controller->parseMzXMLData();

  // preprocess now the extracted runs if not empty
  if( !FT_controller->getParsedDataEmpty() ){
    
    printf("\t\t- postprocessing of extracted LC-MS runs ...\n");
    progress_bar bar(LC_MS_RUNS.size());
    
    vector<LC_MS>::iterator p = FT_controller->get_parsed_DATA_START();
    while(p != FT_controller->get_parsed_DATA_END()){
      
      // get the run:
      LC_MS* THIS_RUN = &(*p);
      
      // order by parent mass:
      THIS_RUN->order_by_mass();
      
      // copy into the name map
      LC_MS_NAME_MAP.insert( make_pair( THIS_RUN->get_spec_name(), *THIS_RUN ) );
        
      bar.update_progress();
      p++;
    }
  }  
  delete FT_controller;
  FT_controller = NULL;
  

  /////////////////////////////////////////////////////
  // get the remaining already exiting XML formatted peak picked runs and combine with the extracted ones:
  // first read all the .xml files if the LCMSRUNS_DIR exists
		file_sys accessor;
  vector<LC_MS> parsed_xml_runs;
  string LCMSRUNS_DIR = "ANALYSIS_";
  LCMSRUNS_DIR += LC_MS_RUN_DIR + "LC_MS_RUNS/";
		if (accessor.check_directory(LCMSRUNS_DIR)) {
    printf("\n\t--- Importing LC-MS runs from %s:\n\n", LCMSRUNS_DIR.c_str());
    parse_from_XML_data();
		}
  
  
  ////////////////////////////////////////////////////
  // if we did not find any .xml files to read it means that parsed_xml_runs vector is empty
		// If it is not, them we add its LC_MS objects to LC_MS_RUNS.
		if (!LC_MS_NAME_MAP.empty()) {

    // reindex the LCMS_runs that we got from the importer class and add them to the LC_MS_RUNS vector
    map<string,LC_MS>::iterator P = LC_MS_NAME_MAP.begin();
    int index = 0;
    while( P != LC_MS_NAME_MAP.end() ){
  				// set the LC-MS id:
      P->second.set_spectrum_ID(index);
      LC_MS_RUNS.push_back( (P->second) );
  				index++;
      P++;
    }
    LC_MS_NAME_MAP.clear();
  }
  
  // assign LC-MS ids to feature
  assigenLCMSIdToFeatures();
		
}





/////////////////////////////////////////////////////////
// write teh parseed LC/MS into directory:
void LCMSDataImporter::write_out_parsed_LC_MS(){
    
  string out_dir;
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  out_dir = def->search_tag("MY PROJECT NAME");
  
  if( out_dir[ out_dir.size() - 1] != '/'){
    out_dir += "/";
  }
  
  string tmp = "ANALYSIS_";
  tmp += out_dir + "LC_MS_RUNS/";
  out_dir = tmp;
  mkdir(out_dir.c_str(),0777);
  
  vector<LC_MS>::iterator p = LC_MS_RUNS.begin();
  while(p != LC_MS_RUNS.end()){
    // write the LC/MS run out to a file:
    write_out_LC_MS_run( &(*p) , out_dir);

    p++;
  }
  
  printf("\n\t\tInput LC-MS runs have been written to '%s'... \n", out_dir.c_str());
}


/////////////////////////////////////////////////////////
// filter LC peaks in all LC/MS runs by
// their LC scores:
void LCMSDataImporter::filter_data_by_LC_score(){
  
  vector<LC_MS>::iterator P = LC_MS_RUNS.begin();
  while( P != LC_MS_RUNS.end() ){
    
    /////////////////////////////
    // check if a run does not contain any fetaures:
    if( (*P).get_nb_features() == 0){
      
      printf("\t********************************************************************************************************************\n");
      printf("\t LC-MS run '%s' contains no features... check feature filter criteria\n",(*P).get_spec_name().c_str());
      printf("\t -> this run is removed from the analysis!\n");
      printf("\t********************************************************************************************************************\n");
      P = LC_MS_RUNS.erase(P);
    }
    else{
      P++;
    }
  }
}





/////////////////////////////////////////////////////////////
// writes a LC/MS run into XML formatted file:
void LCMSDataImporter::write_out_LC_MS_run(LC_MS* IN, string OUT){
  
  string file_name = OUT;
  file_name += IN->get_spec_name() + ".xml";
  
  // write LC/MS stuff:
  // write first the LC/MS MASTER run:
  LC_MS_XML_writer* LC_W = new LC_MS_XML_writer( file_name ) ;
  LC_W->write_XML_MAIN_HEADER();    
  LC_W->write_LC_MS_run_2_XML_tag(IN);
  
  // now write the features:
  vector<LC_MS_FEATURE>::iterator p = IN->get_feature_list_begin();
  while( p != IN->get_feature_list_end()){
    LC_W->write_feature_2_XML_tag( &(*p), true );
    p++;
  }
  
  LC_W->write_LC_MS_run_close_tag();
  LC_W->write_XML_MAIN_HEADER_CLOSE();

  delete LC_W;
  LC_W = NULL;
}




/////////////////////////////////////////////////////////////
// renummerated the ids of aligned features in the corresponding master map
void LCMSDataImporter::renummeratedAlignedFeaturesInMasterMap( LC_MS* mastermap, map<int, int>* idMap ){
  
  vector<LC_MS_FEATURE>::iterator P = mastermap->get_feature_list_begin();
  while( P != mastermap->get_feature_list_end() ){
    
    feature* fea = &(*P);
    
    // set mastermap id:
    fea->set_MASTER_ID( mastermap->get_MASTER_ID() );
    // set LC-MS id:
    map<int, int>::iterator newID = idMap->find( fea->get_spectrum_ID() );
    fea->set_spectrum_ID( newID->second );
    
    
    map<int,feature>* refMatchList = fea->get_match_list_REFERENCE();

    map<int,feature> oldMatchList = fea->get_match_list();
    map<int,feature>::iterator old = oldMatchList.begin();
    while( old != oldMatchList.end() ){
      
      // remove the old feature:
      refMatchList->erase( old->first );
      // get the new LC-MS id:
      map<int, int>::iterator newID = idMap->find( old->first );
      
      // set LC-MS id:
      newID = idMap->find( old->second.get_spectrum_ID() );
      old->second.set_spectrum_ID( newID->second );
      // set the parameters:
      old->second.set_MASTER_ID( mastermap->get_MASTER_ID() );
      // reinsert the adjusted feature:
      refMatchList->insert( make_pair( old->second.get_spectrum_ID(), old->second ) ); 
      
      old++; 
    }
    
    oldMatchList.clear();
    refMatchList = NULL;
    
    P++;
  }
}


///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS Spec Manager:
// management of the input LC-MS spectra
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#define USE_TREE_BUILDER
#define USE_TREE_READER
#define USE_PEPTIDE_DELTA_CLUSTER
#define USE_STATISTIC_BOX
#define USE_SUPERHIRN_MANAGER

#include "use.h"



////////////////////////////////////////////////
// constructor for the object ana_summarizer:
SuperHirn_manager::SuperHirn_manager(){
  
  //////////////////////////////////
  // initialize all runing parameters:
  if( !param_initializer::check_ROOT_PARAM( )){
    exit( 0 );
  }
  param_initializer::INIT_ALL();
  //////////////////////////////////
  
 
  STAT = NULL;
  SIM_MATRIX = NULL;
  MASTER = NULL;
  ALIGNMENT_MODE = 0;
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  SOURCE_DIR = def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;
  //////////////////////////////////
  
}

//////////////////////////////////////////////////
// class desctructor
SuperHirn_manager::~SuperHirn_manager(){
  
  if(!ALL_LC_MS_RUNS.empty())
    ALL_LC_MS_RUNS.clear();
  
   if( SIM_MATRIX == NULL){
    delete SIM_MATRIX;
    SIM_MATRIX = NULL;
  }
  
  if( STAT != NULL ){
    delete STAT;
    STAT = NULL;
  }

  MASTER = NULL;
  ALL_LC_MS_MASTER_MAP.clear();
}


//////////////////////////////////////////////////
// show spectra:
void SuperHirn_manager::show_spectra(){
  
  printf("\n\t\t- Imported LC-MS runs:\n");
  LC_MS_ITERATOR P = get_LC_MS_LIST_start();
  while(  P != get_LC_MS_LIST_end() ){
    (*P).show_info();
    P++;
  }
  printf("\n");

}

//////////////////////////////////////////////////
// show imported MasterMaps
void SuperHirn_manager::show_MASTER_MAPS(){
  
  printf("\n\t\t- Imported MASTER MAPS:\n");
  LC_MS_ITERATOR P = ALL_LC_MS_MASTER_MAP.begin();
  while(  P != ALL_LC_MS_MASTER_MAP.end() ){
    (*P).show_info();
    P++;
  }
  printf("\n");
  
}



////////////////////////////////////////////////
// PEAK EXTRACTION ON MZXML DATA:
// however, here a specific path is given which is not defined in the
// param.def file:
void SuperHirn_manager::start_peak_extraction( string in ) {
  mzXMLDataPath = in;
		start_peak_extraction( );
}



////////////////////////////////////////////////
// PEAK EXTRACTION ON MZXML DATA:
void SuperHirn_manager::start_peak_extraction( ) {
  
  
  ////////////////////////////////////////////////////////
  // initialization of peak detection parameters:
  param_initializer::PEAK_DETECTION_CLASS_INITIALIZATION();

  LCMSDataImporter* IMP = new LCMSDataImporter( );
  
  // check if external data required:
  if( !mzXMLDataPath.empty() ){
    IMP->setExternalMzXMLRawPath( mzXMLDataPath );
  }
  
  IMP->start_peak_extraction_from_mzxml_data();
  ALL_LC_MS_RUNS = IMP->get_parsed_DATA();

  delete IMP;
  IMP = NULL;
  
}



////////////////////////////////////////////////
// import data either from RAW or XML
void SuperHirn_manager::parse_INPUT_DATA( ){
  
  // only parse if the vector of runs is empty:
  if( ALL_LC_MS_RUNS.empty() ){

    LCMSDataImporter* IMP = new LCMSDataImporter( );  
    printf("\t\t\t- importing LC-MS runs for project '%s'...\n", SOURCE_DIR.c_str());
    
    // check if external data required:
    if( !interactPepXMLPath.empty() ){
      IMP->setExternalInteractPepXMLPath( interactPepXMLPath );
    }
    
    // if false, parse data from xml file:
    IMP->parse_from_XML_data();
    
    // IMP->process_input_LC_MS_runs();
    ALL_LC_MS_RUNS = IMP->get_parsed_DATA();
    
    delete IMP;
    IMP = NULL;
  }
  
  // print parsed LC/MS runs:
  show_spectra(); 
}


////////////////////////////////////////////////
// parse already existing MasterMaps
void SuperHirn_manager::parse_MASTER_MAPS( ){
  
  // only parse if the vector of runs is empty:
  if( ALL_LC_MS_MASTER_MAP.empty() ){
    
    LCMSDataImporter* IMP = new LCMSDataImporter( );  
    printf("\t\t- importing MASTER MAPs for Project '%s'...\n", SOURCE_DIR.c_str());
    
    // if false, parse data from xml file:
    IMP->parse_from_MASTER_MAPS();
    
    // IMP->process_input_LC_MS_runs();
    ALL_LC_MS_MASTER_MAP = IMP->get_parsed_DATA();
    
    delete IMP;
    IMP = NULL;
  }
  
  // print parsed MASTER MAPS runs:
  show_MASTER_MAPS(); 
}


  
//////////////////////////////////////////////////
// main function to start the pairwise LC/MS alignment 
void SuperHirn_manager::start_PAIRWISE_ALIGNMENT(){
  
  // parse perprocessed runs:
  parse_INPUT_DATA( );
  
  if( !ALL_LC_MS_RUNS.empty() ){
  
    progress_bar bar( ( get_nb_LCMS_runs()*(get_nb_LCMS_runs()+1) ) / 2, "analyzed");
    
    // initialize the similiarity matrix:
    SIM_MATRIX = new LC_MS_similarity_matrix( SOURCE_DIR );
    
    // loop now through all the spectra and compare to each other:
    LC_MS_ITERATOR A = get_LC_MS_LIST_start();
    while( A != get_LC_MS_LIST_end()){
      
      LC_MS_ITERATOR B = A;
      B++;
      
      // now make pairwise comparison to all others
      while( B != get_LC_MS_LIST_end()){
        
        // aligne replicates:
        alignLCMSpair(&(*A), &(*B));
        
        // progress bar:
        bar.update_progress();
        
        B++;
      }
      
      A++;
    }  
    
    SIM_MATRIX->write_Matrix_to_XML();
    // plot the similarity matrix using perl:
    SIM_MATRIX->plot_matrix_by_perl();

  }
  else{
    printf("\n\t *** NO LC-MS run in batch to compute LC-MS similarity correlations ***\n");
  }
}


//////////////////////////////////////////////////
// alignment of 2 spectra as defined by the input
void SuperHirn_manager::alignLCMSpair(LC_MS* A, LC_MS* B){
 
  // create here a copy of the run to be
  // corrected, otherwise the Tr values will
  // be definitely altered:
  LC_MS* fakeB = new LC_MS( B );
  
  // align now the spectra:
  AmrtpLCMSAligner* ALL = new AmrtpLCMSAligner();
  
  // set the spectra pairs:
  ALL->set_REFERENCE_LCMSMap( &(*A) );
  ALL->set_CORRECTION_LCMSMap( fakeB );
  
  // get the correlation before the alignment:
  SM_cell* SIM_SCORE = new SM_cell( A, B);
  
  // do the alignment:
  ALL->START_LC_MS_Alignment();
  
  // set the correletion of this pair after alignment
  ALL->check_LC_MS_correlation( SIM_SCORE );
  
  // build up the matrix elements:
  if(SIM_MATRIX != NULL){
    SIM_MATRIX->insert_ELEMENT( SIM_SCORE );
  }
  
  if( SIM_SCORE != NULL ){
    delete SIM_SCORE;
    SIM_SCORE = NULL;
  }
  
  delete ALL;
  ALL = NULL; 
    
  delete fakeB;
  fakeB = NULL;
}


//////////////////////////////////////////////////////////////////////////////
// build guided tree: takes the distance matrix (= matrix of overlap numbers)
// and builds a tree using the object tree_builder:
void SuperHirn_manager::start_build_guide_tree(){
  
  if( !ALL_LC_MS_RUNS.empty() ){

    // create a new object tree_builder:
    tree_builder* tourist_guide = new tree_builder();
    
    if( get_nb_LCMS_runs() == 1 ){ 
      // save the guide tree structure to a file:
      tourist_guide->save_guide_tree( &(*ALL_LC_MS_RUNS.begin() ) );      
    }
    else{
    
      // add the LC/MS sim. matrix:
      tourist_guide->set_LC_MS_similarity_matrix(SIM_MATRIX);
      
      // start buidling up the alignment tree:
      tourist_guide->start_building();
      
      // save the guide tree structure to a file:
      tourist_guide->save_guide_tree();
      
    }
    
    delete tourist_guide;
    tourist_guide = NULL;
  }
  else{
    printf("\n\t *** NO LC-MS run in batch to compute LC-MS guide tree ***\n");
  }
}

////////////////////////////////////////////////////////////////////////
// start here to create MASTER runs for different LC_MS groups
void SuperHirn_manager::start_creating_MASTER_MAP(string in){
  
  interactPepXMLPath = in;
  start_creating_MASTER_MAP();
  
}  
  
////////////////////////////////////////////////////////////////////////
// start here to create MASTER MAP for different LC_MS groups
void SuperHirn_manager::start_creating_MASTER_MAP(){

  // read the input data from XML
  parse_INPUT_DATA( );    
  
  // build the master run from the current alignment tree:
  START_MULTIPLE_ALIGNMENT_OF_LC_MS_MAPS( );
  
  if( MASTER != NULL ){
    // print the master to XML
    print_MASTER_to_XML_file();
    
    // print out the normalized master run to excel
    convert_Master_2_excel();
  }
}


////////////////////////////////////////////////////////////////////////
// start here to create the SuperMap from the different MasterMaps
void SuperHirn_manager::start_creating_SUPER_MAP(){
  
  // parse the master maps:
  parse_MASTER_MAPS( );
  ALIGNMENT_MODE = 1;
  
  // start the multiple alignment of the mastermaps
  START_MULTIPLE_ALIGNMENT_OF_LC_MS_MAPS();
  
  if( MASTER != NULL ){
    convert_Master_2_excel( );
    print_MASTER_to_XML_file( );
  }
  
}


////////////////////////////////////////////////////////////////////////
// start the multiple LC_MS merging process 
void SuperHirn_manager::START_MULTIPLE_ALIGNMENT_OF_LC_MS_MAPS(){
  
  if( get_ALIGNMENT_MAP_VECTOR() != NULL ){
    
    if( get_NB_ALIGNMENT_MAPS() > 1 ){ 
      
      // create first the multi_merge object:
      // pass as arguments the guide tree and reference to this object
      // to access the LC-MS runs!
      multi_merger* MUL_ALL = new multi_merger(this, &SuperHirn_manager::get_LC_MS_by_ID, &SuperHirn_manager::delete_LC_MS_by_ID);
      
      // start the multi merging procedure
      MUL_ALL->start_merging_process();
      
      LC_MS* out = MUL_ALL->get_MASTER_LC_MS();
      if( out != NULL ){
        // get the created master run back:
        MASTER = new LC_MS( out );
      }
      
      delete MUL_ALL;
      MUL_ALL = NULL;
      
    }
    else if( get_NB_ALIGNMENT_MAPS() == 1 ) {
      
      // only one LC/MS run, so its directly the MASTER run:
      LC_MS* FIRST = &( *(get_ALIGNMENT_MAP_VECTOR()->begin()) );
      MASTER = new LC_MS( FIRST);
      // add the original raw name:
      MASTER->add_raw_spec_name( FIRST->get_spectrum_ID(), FIRST->get_spec_name() );
      // termin it mastermap
      if( ALIGNMENT_MODE == 0){
        MASTER->set_spec_name( SOURCE_DIR );
      }
      else if( ALIGNMENT_MODE == 1 ){
        MASTER->set_spec_name("SUPER MAP");
      }
    }
    else {
      printf("\n\t\t No LC-MS data in the batch, stopp building MASTERMAP. \n");
    }
    
    // delete the single LC-MS runs, not needed anymore:
    get_ALIGNMENT_MAP_VECTOR()->clear();
  }
  
}


////////////////////////////////////////////////////////////////////////
// starts the statistical analysis of the MASTER
void SuperHirn_manager::start_PROFILING_MASTER_analysis(){
  
  if( STAT == NULL ){
    STAT = new statistic_box();
  }

  // import MASTER runs:
  if(MASTER == NULL){
    STAT->import_MASTER_run();
  }
  else{
    STAT->set_MASTER_RUN( MASTER );
  }  
  
  ///////////////////////////////////////////////
  // NORMALIZE INTENSITY VARIATION ACROSS SAMPLES:
  // compute variation profiles along TR for every run:
  // and apply normalization to features for each input MASTER run:
  //STAT->start_GLOBAL_INTENSITY_NORMALIZATION();
  
  /////////////////////////////////////////////////
  // clustering of peptide profiles:
  STAT->START_MS1_FEATURE_PROFILING();
    
}


////////////////////////////////////////////////////////////////////////
// do intensity normalization of the MasterMap
void SuperHirn_manager::start_INTENSITY_NORMALIZATION( ){
  
  if( STAT == NULL ){
    STAT = new statistic_box();
  }
  
  // import MASTER runs:
  if(MASTER == NULL){
    STAT->import_MASTER_run();
    MASTER = STAT->get_MASTER_RUN();
  }
  else{
    STAT->set_MASTER_RUN( MASTER );
  }  
  
  ///////////////////////////////////////////////
  // NORMALIZE INTENSITY VARIATION ACROSS SAMPLES:
  // compute variation profiles along TR for every run:
  // and apply normalization to features for each input MASTER run:
  STAT->start_GLOBAL_INTENSITY_NORMALIZATION();
  

  // adjust teh name to flag as normalized master run:
  string prefix = "NORMALIZED_";
  MASTER->set_spec_name( MASTER->get_spec_name() );
  print_MASTER_to_XML_file( prefix );
  
  // print MasterMap to tab file
  MASTER->set_spec_name( "normalized_MasterMap.txt");
  print_Master_LC_MS_to_excel( );
  
}

////////////////////////////////////////////////////////////////////////
// update the mastermap by inlcuions list ms2 scans
void SuperHirn_manager::start_MASTERMAP_INCLUSION_LIST_UPDATING( string in){
  inclusionPepXMLListPath = in;
  start_MASTERMAP_INCLUSION_LIST_UPDATING();
}


////////////////////////////////////////////////////////////////////////
// update the mastermap by inlcuions list ms2 scans
void SuperHirn_manager::start_MASTERMAP_INCLUSION_LIST_UPDATING( ){
  
  if( STAT == NULL ){
    STAT = new statistic_box();
  }
  
  // import MASTER runs:
  if(MASTER == NULL){
    STAT->import_MASTER_run();
    MASTER = STAT->get_MASTER_RUN();
  }
  else{
    STAT->set_MASTER_RUN( MASTER );
  }  
  
  feature_updater* UPDATER = new feature_updater();
  
  // check if external data required:
  if( !inclusionPepXMLListPath.empty() ){
    //IMP->setExternalMzXMLRawPath( mzXMLDataPath );
  }
  
  UPDATER->INIT_ALL();
  // update all extracted features with additional MS2 information:
  UPDATER->update_feature_identity( MASTER );
  UPDATER->print_update_summary();
  
  delete UPDATER;
  UPDATER = NULL;
  
  MASTER->show_info();
  
  // print MasterMap to tab file
  print_Master_LC_MS_to_excel( );
  
  // adjust teh name to flag as inlcusion list annotated run:
  string prefix = "ILA_";
  MASTER->set_spec_name( prefix + MASTER->get_spec_name());
  print_MASTER_to_XML_file( prefix );
  
}



////////////////////////////////////////////////////////////////////////
// analyze profiling data from superhirn k-means clustering
void SuperHirn_manager::start_PROFILING_EVALUATION(){  
  if( STAT == NULL ){
    STAT = new statistic_box();
  }
  STAT->ANALYZE_MS1_FEATURE();
}



////////////////////////////////////////////////////////////////////////
// starts delta search analysis of the MASTER
void SuperHirn_manager::start_DELTA_SEARCH_MASTER_analysis(){
  
  if( STAT == NULL ){
    STAT = new statistic_box();
  }
  // import MASTER runs:
  if(MASTER == NULL){
    STAT->import_MASTER_run();
  }
  else{
    STAT->set_MASTER_RUN(MASTER);
  }  
  
  /////////////////////////////////////////////////
  // search for delta peptide pairs:
  STAT->extract_DELTA_PEPTIDE_PAIRS();
}



////////////////////////////////////////////////////////////////////////
// starts delta search analysis of the MASTER
void SuperHirn_manager::start_MS2_TRACE_FRAGMENT_analysis(){
  
  // import MASTER runs:
  if(MASTER == NULL){
    STAT = new statistic_box();
    STAT->import_MASTER_run();
    MASTER = STAT->get_MASTER_RUN();
  }
  
  MS2quantificationController* controller = new MS2quantificationController( MASTER );
  // start tracing:
  controller->START_MS2_TRACE_QUANTIFICATION();
  // print to file:
  controller->writeFragmentTracetextFile();
  
  // clean up:
  delete controller;
  controller = NULL;
}




////////////////////////////////////////////////////////////////////////
// starts ratiolizer analysis
void SuperHirn_manager::start_RATIOLIZER_analysis(){
  
  ratiolizer_manager* MANAGER = new ratiolizer_manager();
  
  // import MASTER runs:
  if(MASTER == NULL){
    MANAGER->import_MASTER_run();
    MASTER = MANAGER->get_MASTER();
  }
  else{
    MANAGER->insert_MASTER( MASTER );
  }  
  
  if( MASTER ){

    // start analysis
    MANAGER->start_RATIOLIZER();
  }
  
  delete MANAGER;
  MANAGER = NULL;
  MASTER = NULL;

}


////////////////////////////////////////////////////////////////////////
// access the LC-MS by their id:
LC_MS* SuperHirn_manager::get_LC_MS_by_ID(void *pInstance,int ID){
 
  SuperHirn_manager *pThis = (SuperHirn_manager *)pInstance; 
  
  vector<LC_MS>* maps = pThis->get_ALIGNMENT_MAP_VECTOR();
  if( maps != NULL ){
    vector<LC_MS>::iterator p = maps->begin();
    while( p != maps->end() ){
      if( (*p).get_spectrum_ID() == ID){
        return &(*p); 
      }	
      p++;
    }
  }
  
  return NULL;
}

////////////////////////////////////////////////////////////////////////
// delete a LC-MS by their id:
void SuperHirn_manager::delete_LC_MS_by_ID(void *pInstance,int ID){
  
  SuperHirn_manager *pThis = (SuperHirn_manager *)pInstance; 
  
  vector<LC_MS>* maps = pThis->get_ALIGNMENT_MAP_VECTOR();
  if( maps != NULL ){
    vector<LC_MS>::iterator p = maps->begin();  
    while( p != maps->end() ){
      
      if( (*p).get_spectrum_ID() == ID){
        maps->erase( p );
        break;
      }	
      p++;
    }
  }
}



//////////////////////////////////////////////////////////
// prints the master run, so it does not have to be computed
// all the time into XML formatted file:
void SuperHirn_manager::print_MASTER_to_XML_file(){
  
  // write first the LC/MS MASTER run:
  LC_MS_writer* writter = new LC_MS_writer( MASTER );
  writter->storeMasterMapToXML(  );
  delete writter;
  writter = NULL;
  
}


//////////////////////////////////////////////////////////
// prints the master run, so it does not have to be computed
// all the time into XML formatted file:
void SuperHirn_manager::print_MASTER_to_XML_file( string Prefix ){
  
  // write first the LC/MS MASTER run:
  LC_MS_writer* writter = new LC_MS_writer( MASTER );
  writter->storeMasterMapToXML( Prefix );
  delete writter;
  writter = NULL;
  
}




////////////////////////////////////////////////////////////////////////
// converts a set of input XML formatted runs to APML
void SuperHirn_manager::convert_XML_to_AMPL( string TARGET ){
  
  if( TARGET[ TARGET.size() -1 ] != '/' ){
    TARGET += "/";
  }
  
  // create a new LC_MS reader:
  LC_MS_XML_reader* READ = new LC_MS_XML_reader( );
  LC_MS_APML_writer* LC_W = new LC_MS_APML_writer(TARGET );  
  
  // now read all features:
  READ->start_LC_MS_parsing( TARGET  );
    
  // get the parsed run:
  vector<LC_MS>::iterator P = READ->get_parsed_LCMS_list_start();
  while( P != READ->get_parsed_LCMS_list_end() ){
    LC_MS* TMP = &(*P);
    if( TMP != NULL ){
      TMP->show_info();      
      // check if its a mastermap:
      if( TMP->check_raw_spec_name_empty() ){
        LC_W->WRITE_COMPLETE_LC_MS_2_APML( TMP->get_spec_name(), TMP );
      }
      else{
        LC_W->WRITE_MASTER_LC_MS_2_APML( TMP->get_spec_name(), TMP );
      }
    }
    TMP = NULL;
    P++;
  }
  
  delete LC_W;
  delete READ;
  LC_W = NULL;  
  READ = NULL;
  
}


//////////////////////////////////////////
// convert the master run to excel file
void SuperHirn_manager::convert_Master_2_excel(){
  
  if( STAT == NULL ){
    STAT = new statistic_box();
  }
  
  if(MASTER == NULL){
    STAT->import_MASTER_run();
    MASTER = STAT->get_MASTER_RUN();
  }
  else{
    STAT->set_MASTER_RUN( MASTER ); 
  }
  // do normalization:
  // STAT->start_GLOBAL_INTENSITY_NORMALIZATION();
    
  // write out:
  print_Master_LC_MS_to_excel();
  
}


////////////////////////////////////////////////////////////////////////
// converts a set of input XML formatted runs to excel table
void SuperHirn_manager::convert_Master_2_excel( string TARGET ){
  
  if( TARGET[ TARGET.size() -1 ] != '/' ){
    TARGET += "/";
  }
  
  // create a new LC_MS reader:
  LC_MS_XML_reader* READ = new LC_MS_XML_reader();
  
  // now read all features:
  READ->start_LC_MS_parsing( TARGET  );
  
  // get the parsed run:
  vector<LC_MS>::iterator P = READ->get_parsed_LCMS_list_start();
  while( P != READ->get_parsed_LCMS_list_end() ){
    LC_MS* TMP = &(*P);
    if( TMP != NULL ){
      MASTER = TMP;
      print_Master_LC_MS_to_excel( TARGET );
    }
    TMP = NULL;
    P++;
  }
  
  delete READ;
  READ = NULL;
  
}

//////////////////////////////////////////
// print out the content of MASTER run
// into tab delim format:
void SuperHirn_manager::print_Master_LC_MS_to_excel(string in){
  
  if( MASTER != NULL ){
    LC_MS_XML_writer* LC_W = new LC_MS_XML_writer( in );  
    LC_W->write_MASTER_LC_MS_to_EXCEL( MASTER );
    delete LC_W;
    LC_W = NULL;
  }
  
}

//////////////////////////////////////////
// print out the content of MASTER run
// into tab delim format:
void SuperHirn_manager::print_Master_LC_MS_to_excel(){
  
  if( MASTER != NULL ){
    LC_MS_XML_writer* LC_W = new LC_MS_XML_writer();  
    LC_W->write_MASTER_LC_MS_to_EXCEL( MASTER );
    delete LC_W;
    LC_W = NULL;
  }
}


//////////////////////////////////////////
// get the number of maps to align
int SuperHirn_manager::get_NB_ALIGNMENT_MAPS(){
  
  switch( ALIGNMENT_MODE ){
      
      // normal alignment mode:
    case 0:
      return ALL_LC_MS_RUNS.size();
      break;
      
      // Mastermap alignment mode:
    case 1:
      return ALL_LC_MS_MASTER_MAP.size();
      break;
      
    default:
      return 0;
      break;
  }  
  
  return 0;
}


//////////////////////////////////////////
// get a reference to the vector of maps to be aligned
vector<LC_MS>* SuperHirn_manager::get_ALIGNMENT_MAP_VECTOR(){
  
  switch( ALIGNMENT_MODE ){
      
      // normal alignment mode:
    case 0:
      return &ALL_LC_MS_RUNS;
      break;
      
      // Mastermap alignment mode:
    case 1:
      return &ALL_LC_MS_MASTER_MAP;
      break;
  
    default:
      return NULL;
      break;
  }  
  return NULL;
}

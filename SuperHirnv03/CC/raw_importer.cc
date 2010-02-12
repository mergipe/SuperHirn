///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS raw_importer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_IMPORTER
#define USE_RAW_IMPORTER

#include "use.h"

bool raw_importer::CREATE_FEATURE_ELUTION_PROFILES = false;
bool raw_importer::LCelutionPeakDebugging = false;
double raw_importer::LCelutionPeakMassMin = 938.65;
double raw_importer::LCelutionPeakMassMax = 938.72;

////////////////////////////////////////////////
// constructor for the object raw_importer:
raw_importer::raw_importer(bool raw_data){

  // initialize the class parameters from the
  // FT PEAK DETECTION program:
  initializer INITER;
  INITER.init_all();
  
  raw_DATA = NULL;
  PROCESSED_DATA = NULL;
  
  THIS_LC_MS = NULL;
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  
  if( raw_data){
    SOURCE_DIR = def->search_tag("MZXML DIRECTORY");
  }
  else{
    SOURCE_DIR = def->search_tag("MY PROJECT NAME");
  }

  ORIGINAL_INTERACT_DIR = def->search_tag("PEPXML DIRECTORY");
  if( ORIGINAL_INTERACT_DIR[ORIGINAL_INTERACT_DIR.size() - 1] != '/'){
    ORIGINAL_INTERACT_DIR += '/';
  }  
  
  if( SOURCE_DIR[ SOURCE_DIR.size() - 1] != '/'){
    SOURCE_DIR += "/";
  }

  ////////////
  // define the retention time window:
  
  // start of the window
  def->search_tag("start elution window", &TR_start);
  
  // end of the window
  def->search_tag("end elution window", &TR_end);
  
  // define static parameters of objects classes:
  double tmp = 0;
  
  // mass accuracy:
  def->search_tag("retention time tolerance", &tmp);
  feature::MZ_TOL = tmp;
  // TR accuracy:
  def->search_tag("MS1 m/z tolerance", &tmp);
  feature::TR_TOL = tmp;
  
  // MS2 mass accuracy:
  def->search_tag("MS2 m/z tolerance", &MZ2_TOL);

  // MS2 mass accuracy:
  def->search_tag("MS2 SCAN tolerance", &SCAN_MS2_TOL);

  // MS2 mass accuracy:
  def->search_tag("Peptide Prophet Threshold", &PEP_PROPH);
  
  
  // LC peak cutoff:
  def->search_tag("LC peak score cutoff", &LC_PEAK_THRESHOLD);  
  
  // 
  def->search_tag("MS2 mass matching modus",&THEO_MS_MATCH);
  
  delete def;
  def = NULL;
  
  
}

//////////////////////////////////////////////////
// class desctructor of raw_importer
raw_importer::~raw_importer(){
  
  if(raw_DATA != NULL){
    delete raw_DATA;
    raw_DATA = NULL;
  }
  PROCESSED_DATA = NULL;
  THIS_LC_MS = NULL;
}

//////////////////////////////////////////////////
// class copy constructor of raw_importer
raw_importer::raw_importer(const raw_importer& tmp){
  LC_MS_RUNS.clear();
}


//////////////////////////////////////////////////
// copy constructor:
raw_importer& raw_importer::operator=(const raw_importer& tmp){

    return *this;
}
    


//////////////////////////////////////////////////
// read mzXML data:
void raw_importer::read_mzXML_data(int id){
  
  FT_PEAK_DETEC_mzXML_reader* READER = new FT_PEAK_DETEC_mzXML_reader();
  // set input file and open:
  READER->set_current_file( get_target_file() );
  READER->delete_existing_debug_file(); // old debug files are overwritten
  READER->open_mzxml_file();
  
  // get titel:
  string name = get_target_file();
  name.erase(0, name.rfind("/")+1);
  name.erase(name.rfind(".mzXML"), name.size() - name.rfind(".mzXML"));
  
  // create a new LC/MS:
  THIS_LC_MS = new LC_MS(name);
  THIS_LC_MS->set_spectrum_ID(id);
  
  // start the mzXML reading process:
  READER->read_mzXML_DATA();
    
  raw_DATA =  READER->get_processed_data_structure();
}


//////////////////////////////////////////////////
// process input LC/MS runs:
void raw_importer::process_input_LC_MS_runs(){

  vector<LC_MS>::iterator P = LC_MS_RUNS.begin();
  while( P != LC_MS_RUNS.end() ){
    
    // set the correct LCMS id if teh run is not a mastermap:
    if( (*P).check_raw_spec_name_empty() ){
      (*P).setFeatureLCMSID();
    }
    
    P++;
  }
  
  //MS2_MS1_matcher::showAmbiguouities();

}

//////////////////////////////////////////////////
// process extracted LC_peaks:
void raw_importer::process_extracted_LC_peaks(){
  
  int id_count = 0;
  vector<LC_elution_peak*> PEAKS = get_ALL_peaks();
  vector<LC_elution_peak*>::iterator P = PEAKS.begin();
  LC_elution_peak* tmp = NULL;

  while( P != PEAKS.end()){
    
    tmp = (*P);
    
    if( tmp != NULL ) {
      // print the peak:
      add_raw_peak_to_LC_MS_run(tmp, id_count);
    }
    
    id_count++;
    tmp = NULL;
    P++; 
  }
  
}

////////////////////////////////////////////////////
// adds an elution peak to the LC/MS run:
void raw_importer::add_raw_peak_to_LC_MS_run(LC_elution_peak* PEAK, int ID){
  

  
  ////////////////////////////////
  // get parameters of the peak:
  // apex:
  int apex_scan = PEAK->get_scan_apex();
  double apex_MZ = PEAK->get_apex_MZ();
  double apex_TR = PEAK->get_apex_retention_time();
  float apex_INTENSITY = PEAK->get_apex_intensity();
  
  // peak shape:
  float peak_area = PEAK->get_total_peak_area();
  int charge_state = PEAK->get_charge_state();
  int peak_start = PEAK->get_start_scan();
  int peak_end = PEAK->get_end_scan();
  
  // peak validation:
  float score = PEAK->get_LC_score();
  
  if( (apex_TR <= TR_end) && (apex_TR >= TR_start) ){
    // build a feature:
    feature* TMP = new feature(apex_MZ, apex_TR, apex_scan, peak_start,peak_end, charge_state, peak_area, apex_INTENSITY, score);
    TMP->set_spectrum_ID(THIS_LC_MS->get_spectrum_ID());
    TMP->set_feature_ID(ID);

    // function to add the elution profile to the feature:
    // (if turned on)
    if( CREATE_FEATURE_ELUTION_PROFILES ){
      addLCelutionProfile( TMP, PEAK );
    }

    // add it to the LC_MS run:
    THIS_LC_MS->add_feature(TMP);
    TMP = NULL;
  }
  
  
  ////////////////////////////////
  // incase of debugging mode:
  // print out the LC peak info:
  if( LCelutionPeakDebugging ){
    if( ( LCelutionPeakMassMin <= apex_MZ) && ( LCelutionPeakMassMax >= apex_MZ) ){
      PEAK->show_info();
    }
    
  }
  
  PEAK = NULL;
}



////////////////////////////////////////////////////////
// function to add the elution profile to the feature:
void raw_importer::addLCelutionProfile( feature* inF, LC_elution_peak* PEAK ){

  ////////////////////////////////
  // get parameters of the peak:
  // apex:
  int apex_scan = PEAK->get_scan_apex();
  double apex_MZ = PEAK->get_apex_MZ();
  double apex_TR = PEAK->get_apex_retention_time();
  float apex_Intensity = PEAK->get_apex_intensity();

  // peak shape:
  float peak_area = PEAK->get_total_peak_area();
  int charge_state = PEAK->get_charge_state();
  //int peak_start = PEAK->get_start_scan();
  //int peak_end = PEAK->get_end_scan();
  
  // create the class:
  featureLCprofile* myProfile = new featureLCprofile(apex_MZ, apex_TR, apex_Intensity, apex_scan, charge_state, peak_area);

  // add the individual MS1peaks:
  SIGNAL_iterator P = PEAK->get_signal_list_start();
  while( P != PEAK->get_signal_list_end() ){
    //int scan = (*P).first;
    ms_peak* MSpeak = &(*P).second;
    myProfile->addMS1elutionSignal( MSpeak->get_MZ(), MSpeak->get_intensity(), MSpeak->get_scan_number(), MSpeak->get_charge_state(), MSpeak->get_retention_time() );    
    P++;
  }

  inF->setLCelutionProfile( myProfile );
  myProfile = NULL;
}



/////////////////////////////////////////////////////////
// parses LC/MS from XML files, i.e. .xml files:
void raw_importer::parse_from_XML_data(){
 
  
  
  //////////////////////////////////////////////////
  // parse the MS1 LC/MS runs:
  
  string IN_DIR = SOURCE_DIR;  
  // remove backslash at end:
  if(IN_DIR[IN_DIR.size()-1] == '/')
    IN_DIR.erase(IN_DIR.size()-1,IN_DIR.size()-1);
  
  // process the input path:
  unsigned int i = IN_DIR.rfind("/");
  // no signe found:
  if( i != string::npos ){ 
    IN_DIR.erase(0,i+1);
  }
  string tmp = "ANALYSIS_";
  tmp += IN_DIR + "/LC_MS_RUNS/";
  IN_DIR = tmp;
  
  importer* IMP = new importer();
  IMP->read_XML_files(IN_DIR);
  LC_MS_RUNS = IMP->get_lc_ms_list();
  
  delete IMP;
  IMP = NULL;
  
  
  /////////////////////////////////////////////////////
  // add here the interact information:
  
  if( !ORIGINAL_INTERACT_DIR.empty() ){

    if( ORIGINAL_INTERACT_DIR[ORIGINAL_INTERACT_DIR.size() - 1] != '/'){
      ORIGINAL_INTERACT_DIR += '/';
    }
    
    bool POST_ANNOTATION = true;

    printf("\n\n\t\tParsing MS2 information from '%s'...\n", ORIGINAL_INTERACT_DIR.c_str());
    vector<LC_MS>::iterator p = LC_MS_RUNS.begin();
    while(p != LC_MS_RUNS.end()){
      
      LC_MS* THIS_RUN = &(*p);
      // order by parent mass:
      THIS_RUN->order_by_mass();
      
      ////////////////////////////////////////////////////////////////
      // get the MS2 info for this LC/MS;
      // construct the name to the xml file:
      string INTERACT_name = ORIGINAL_INTERACT_DIR + THIS_RUN->get_spec_name();
      
      // object to retrive MS2 infor in form of MS2_info objects:
      interact_parser* PAR = NULL;
      PAR = new interact_parser(INTERACT_name);
      PAR->parse_interact_file();
      
      if( PAR->check_MS2_info_list_content() ) {

        POST_ANNOTATION = false;
        MS2_MS1_matcher* MATCHER = new MS2_MS1_matcher();
        // now get the ms2_info and transform to features:
        vector<ms2_info>::iterator X = PAR->get_MS2_info_list_START();
        while( X != PAR->get_MS2_info_list_END()){
          // matches a MS2-info to a feature:
          MATCHER->combine_LC_MS_specific_MS2_to_MS1_data( THIS_RUN, &(*X));
          X++; 
        }
        MATCHER->show_annotation_overview();
        delete MATCHER;
        MATCHER = NULL;
      }
      
      delete PAR;
      PAR = NULL;
      p++;
    }
    
    
    //////////////////////////////////////////////////////////////
    // check if post annotate data is required:
    // in equal one interact file for all the runs:
    // do only if no run-specific interact file has been found before:
    if( POST_ANNOTATION ){

      file_sys accessor;
      accessor.read_dir(ORIGINAL_INTERACT_DIR,".xml");
      char* file = accessor.get_next_file();
      while(file != NULL){        
        
        printf("\n\t\tPost-Annotation of LC-MS runs by '%s'\n",file);        
        interact_parser* PAR = NULL;
        PAR = new interact_parser( file );
        PAR->parse_interact_file();

        
        // try to add now a scan depending on its spectrum name
        // to the corresponing LC/MS run in the batch:
        // now get the ms2_info and transform to features:
        MS2_MS1_matcher* MATCHER = new MS2_MS1_matcher();
        vector<ms2_info>::iterator X = PAR->get_MS2_info_list_START();
        while( X != PAR->get_MS2_info_list_END()){
  
          // get the spectrum name:
          string MS2_name = (*X).get_ORIGINAL_INTERACT_FILE();
          MS2_name.erase( MS2_name.find("_"), MS2_name.size( ) - MS2_name.find("_") );

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
        
        // next mzXML file:
        file = accessor.get_next_file();
        delete PAR;
        PAR = NULL;
      }
      
    }
  }
}

/////////////////////////////////////////////////////////
// parses LC/MS from raw data, i.e. mzXML files:
void raw_importer::parse_from_raw_data(){
  
  int IN_type;
  file_sys accessor;
  if(accessor.check_directory(SOURCE_DIR))
    IN_type = 0;
  else
    IN_type = 1;
  
  
  //////////////////////////////////////////////////
  // the target path is a directory:
  
  // get the list of target files in the dir
  // with the according extension:
  accessor.read_dir(SOURCE_DIR,".mzXML");
  char* file  = NULL;
  file = accessor.get_next_file();
  
  int id = 0;
  
  while(file != NULL){
    
    printf("\t-- importing '%s'\n",file);
    
    set_target_file(file);
    read_mzXML_data(id);
    
    // print parsed raw data:
    // IMP.print_RAW_DATA();
    // ectract LC elution peaks:
    extract_elution_peaks();
    
    // process now the peaks:
    process_extracted_LC_peaks();
    
    // add it to the spectrum list:
    LC_MS_RUNS.push_back(*THIS_LC_MS);
    THIS_LC_MS = NULL;
    
    id++;
    // next mzXML file:
    file = accessor.get_next_file();
  }
  
  
  //////////////////////////////////
  // end of input reading:
  // preprocess now the input spectra:
  
  // show program status:
  printf("\n\n\tPreprocessing of input LC-MS runs [ order MS1 features | check for MS/MS info ] ... \n");
  
  progress_bar bar(LC_MS_RUNS.size());
  
  vector<LC_MS>::iterator p = LC_MS_RUNS.begin();
  while(p != LC_MS_RUNS.end()){
    
    LC_MS* THIS_RUN = &(*p);
    
    // order by parent mass:
    THIS_RUN->order_by_mass();
    
    ////////////////////////////////////////////////////////////////
    // get the MS2 info for this LC/MS 
    // construct the name to the xml file:
    string INTERACT_name = ORIGINAL_INTERACT_DIR + THIS_RUN->get_spec_name() + ".xml";
    
    // object to retrive MS2 infor in form of MS2_info objects:
    interact_parser* PAR = NULL;
    PAR = new interact_parser(INTERACT_name);
    PAR->parse_interact_file();
    
    MS2_MS1_matcher* MATCHER = new MS2_MS1_matcher();
    // now get the ms2_info and transform to features:
    vector<ms2_info>::iterator X = PAR->get_MS2_info_list_START();
    while( X != PAR->get_MS2_info_list_END()){
      // matches a MS2-info to a feature:
      MATCHER->combine_LC_MS_specific_MS2_to_MS1_data(THIS_RUN, &(*X));
      X++; 
    }
    MATCHER->show_annotation_overview();
    delete MATCHER;
    MATCHER = NULL;
    delete PAR;
    PAR = NULL;
    
    bar.update_progress();
    p++;
  }
  
  // save the parsed LC_MS runs:
  write_out_parsed_LC_MS();
}


/////////////////////////////////////////////////////////
// write teh parseed LC/MS into directory:
void raw_importer::write_out_parsed_LC_MS(){
    
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
    //(*p).print_2_file(out_dir);
    p++;
  }
  
  printf("\n\t\tInput LC-MS runs have been written to '%s'... \n", out_dir.c_str());
}


/////////////////////////////////////////////////////////
// filter LC peaks in all LC/MS runs by
// their LC scores:
void raw_importer::filter_data_by_LC_score(){
  
  vector<LC_MS>::iterator P = LC_MS_RUNS.begin();
  while( P != LC_MS_RUNS.end() ){

    // go through all features:
    vector<feature>::iterator FEA = (*P).get_feature_list_begin();
    while( FEA != (*P).get_feature_list_end() ) {

      // check if bigger score than threshold:
      if( (*FEA).get_peak_score() >= LC_PEAK_THRESHOLD ){
        FEA++;
      }
      else{
        FEA = (*P).remove_feature_from_list(FEA);
      }
    }
    
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
void raw_importer::write_out_LC_MS_run(LC_MS* IN, string OUT){
  
  string file_name = OUT;
  file_name += IN->get_spec_name() + ".xml";
  
  // write LC/MS stuff:
  // write first the LC/MS MASTER run:
  LC_MS_XML_writer* LC_W = new LC_MS_XML_writer( file_name ) ;
  LC_W->write_XML_MAIN_HEADER();    
  LC_W->write_LC_MS_run_2_XML_tag(IN);
  
  // now write the features:
  vector<feature>::iterator p = IN->get_feature_list_begin();
  while( p != IN->get_feature_list_end()){
    LC_W->write_feature_2_XML_tag( &(*p) );
    p++;
  }
  
  LC_W->write_LC_MS_run_close_tag();
  LC_W->write_XML_MAIN_HEADER_CLOSE();

  delete LC_W;
  LC_W = NULL;
}

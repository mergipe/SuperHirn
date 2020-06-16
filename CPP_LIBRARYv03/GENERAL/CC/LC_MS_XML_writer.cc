///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS LC_MS_XML_writer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_LC_MS_XML_WRITER
#define USE_LC_MS_XML_READER
#define USE_MS2_INFO

#include "GENERAL_USE.h"

// if LC profiles should be stored:
bool LC_MS_XML_writer::STORE_FEATURE_ELUTION_PROFILES = true;
// if MS2 fragment traces are stored:
bool LC_MS_XML_writer::STORE_MS2_FRAGMENT_TRACE_DATA = false;

// how to store MS2 data:
bool LC_MS_XML_writer::STORE_BEST_MS2_SCAN_PER_FEATURE = true;
bool LC_MS_XML_writer::STORE_BEST_MS2_SCAN_PER_ALIGNED_FEATURE = true;
// how many alterantive proteins names:
int LC_MS_XML_writer::MAXIMAL_NB_ALTERNATIVE_PROTEIN_NAMES = 5;


////////////////////////////////////////////////
// constructor for the object LC_MS_XML_writer:
LC_MS_XML_writer::LC_MS_XML_writer(){
  WRITER = NULL;
  MASTER = NULL;
}

////////////////////////////////////////////////
// constructor for the object LC_MS_XML_writer:
LC_MS_XML_writer::LC_MS_XML_writer( string IN ){
  WRITER = NULL;
  MASTER = NULL;
  target_dir = IN;
}

//////////////////////////////////////////////////
// class desctructor of LC_MS_XML_writer
LC_MS_XML_writer::~LC_MS_XML_writer(){
  if( WRITER != NULL ){
    delete WRITER;
    WRITER = NULL;
  }
  MASTER = NULL;
}

//////////////////////////////////////////////////
// class copy constructor of LC_MS_XML_writer
LC_MS_XML_writer::LC_MS_XML_writer(const LC_MS_XML_writer& tmp){
  WRITER = tmp.WRITER;
}

//////////////////////////////////////////////////
// class copy constructor of LC_MS_XML_writer
LC_MS_XML_writer::LC_MS_XML_writer(const LC_MS_XML_writer* tmp){
  WRITER = tmp->WRITER;
}

//////////////////////////////////////////////////
// copy constructor:
LC_MS_XML_writer& LC_MS_XML_writer::operator=(const LC_MS_XML_writer& tmp){
  WRITER = tmp.WRITER;
  return *this;
}
    

/////////////////////////////////////////////////////////
// extracts the output directory:
void LC_MS_XML_writer::set_OUT_DIR(){
  
  if( target_dir.empty() ){
    
    target_dir.clear();
    target_dir = "ANALYSIS_";
    
    //////////////////////////////////
    // read parameters from "param.def"
    read_param* def = new read_param();  
    target_dir += def->search_tag("MY PROJECT NAME");  
    delete def;
    def = NULL;
    //////////////////////////////////
    
    
    if( target_dir[ target_dir.size() - 1 ] != '/'){
      target_dir += "/";
    }
  }
  
  out_file.insert( 0, target_dir );
}

//////////////////////////////////////////////////////////
// write out a complete LC/MS run (MASTER OR NOT)
void LC_MS_XML_writer::WRITE_COMPLETE_LC_MS_2_XML(string OUT, LC_MS* IN){
  
  out_file = OUT;
  MASTER = IN;
  
  if( open_WRITER() ){
    
    write_XML_MAIN_HEADER();
    
    // write the experiment analysis
    write_MASTER_run_experiment_overview(MASTER);
    
    ///////////////////////
    // write feature matching data:
    if( MASTER->get_nb_features() != 0 ){
      
      write_LC_MS_run_2_XML_tag(MASTER);
      
      // write the individual features to file:
      vector<LC_MS_FEATURE>::iterator FEA = MASTER->get_feature_list_begin();    
      while( FEA != MASTER->get_feature_list_end() ){
        write_feature_2_XML_tag( &(*FEA) );
        FEA++;
      }
      write_LC_MS_run_close_tag();
    }
    ///////////////////////
  
    
    write_XML_MAIN_HEADER_CLOSE();
  }
  
}


///////////////////////////////////////////////////////////
// write the experiment analysis
void LC_MS_XML_writer::write_MASTER_run_experiment_overview(LC_MS* MASTER){
  
  if(WRITER != NULL){
    
    char buffer[500];
    sprintf(buffer,"<LC_MS_EXPERIMENT_SUMMARY>\n");  
    WRITER->write(buffer,strlen(buffer));
    
    // similarity assessement:
    print_similarity_assessement();
    
    sprintf(buffer,"</LC_MS_EXPERIMENT_SUMMARY>\n");  
    WRITER->write(buffer,strlen(buffer));
  }
}


///////////////////////////////////////////////////////////
// opens a file for writing:
bool LC_MS_XML_writer::open_WRITER(){
  
  if( WRITER != NULL){
    delete WRITER;
    WRITER = NULL;
  }
  WRITER = new ofstream();
  WRITER->open(out_file.c_str(), ofstream::out); 
  
  if(WRITER->good()){
    printf("\t\t- LC-MS file saved to '%s'\n", out_file.c_str());
    return true;
  }
  else{
    printf("\nERROR: opening file '%s', LC_MS_XML_writer::165\n", out_file.c_str());
    return false;
  }
}


//////////////////////////////////////////////////////////
// write XML HEADER:
void LC_MS_XML_writer::write_XML_MAIN_HEADER(){
  
  if( WRITER == NULL ){
    set_OUT_DIR();
    open_WRITER();
  }
  
  
  if( !WRITER->good() ){
    open_WRITER();
  }
  
  if( WRITER->good() ){

    string H = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"; 
    // write to the file:
    WRITER->write(H.c_str(), H.size());
    
    // write dtd / css:
    string C = "<?xml-stylesheet type=\"text/xsl\" href=\"http://prottools.ethz.ch/muellelu/XML/XSLT/LC_MS_MASTER_display/display_Master_LCMS.xsl\"?>\n"; 
                                                               // write to the file:
    WRITER->write(C.c_str(), C.size());
    
    // version:
    string V = "<MASTER_RUN_SUMMARY version=\"master_minder::v0.1\" author=\"Lukas Mueller\" author_mail=\"Lukas.Mueller@imsb.biol.ethz.ch\""; 
    // get date and time:
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char buffer[500];
    strftime( buffer, 500, "%A, %d.%B.%Y, %H.%M.%S", timeinfo);
    V += " date=\"";
    V += buffer;
    V += "\">\n";
    // write to the file:
    WRITER->write(V.c_str(),V.size());
    WRITER->flush();
  }
}

//////////////////////////////////////////////////////////
// write unformatted XML HEADER:
void LC_MS_XML_writer::write_unformatted_XML_MAIN_HEADER(){
  
  set_OUT_DIR();
  
  if( open_WRITER() ){
    
    string H = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"; 
    // write to the file:
    WRITER->write(H.c_str(), H.size());
    // version:
    string V = "<MASTER_RUN_SUMMARY version=\"master_minder::v0.1\" author=\"Lukas Mueller\" author_mail=\"Lukas.Mueller@imsb.biol.ethz.ch\""; 
    // get date and time:
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char buffer[500];
    strftime( buffer, 500, "%A, %d.%B.%Y, %H.%M.%S", timeinfo);
    V += " date=\"";
    V += buffer;
    V += "\">\n";
    // write to the file:
    WRITER->write(V.c_str(),V.size());
    WRITER->flush();
  }
}



//////////////////////////////////////////////////////////
// write unformatted XML HEADER with input XST file:
void LC_MS_XML_writer::write_unformatted_XML_MAIN_HEADER( string XST_FILE){

  set_OUT_DIR();

  if( open_WRITER() ){
    
    char buffer[500];
    
    string H = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"; 
    WRITER->write(H.c_str(), H.size());
    
    // write dtd / css:
    sprintf(buffer, "<?xml-stylesheet type=\"text/xsl\" href=\"http://prottools.ethz.ch/muellelu/XML/XSLT/%s\"?>\n", XST_FILE.c_str()); 
    WRITER->write(buffer,strlen(buffer));  
    
    // version:
    string V = "<MASTER_RUN_SUMMARY version=\"master_minder::v0.1\" author=\"Lukas Mueller\" author_mail=\"Lukas.Mueller@imsb.biol.ethz.ch\""; 
    // get date and time:
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime( buffer, 500, "%A, %d.%B.%Y, %H.%M.%S", timeinfo);
    V += " date=\"";
    V += buffer;
    V += "\">\n";
    // write to the file:
    WRITER->write(V.c_str(),V.size());
    WRITER->flush();
  }
}



//////////////////////////////////////////////////////////
// write XML HEADER:
void LC_MS_XML_writer::write_XML_MAIN_HEADER_CLOSE(){
  
  if( WRITER != NULL ){
    // version:
    string V = "</MASTER_RUN_SUMMARY>"; 
    WRITER->write(V.c_str(), V.size());
    WRITER->flush();
    WRITER->close();
    WRITER = NULL;
  }
}  

//////////////////////////////////////////////////////////
// write comment:
void LC_MS_XML_writer::write_comment_into_XML(string COM){
  
  string OUT = "<!-- ";
  OUT += COM;
  OUT += " --!>";
  // write to the file:
  WRITER->write(OUT.c_str(),OUT.size());
  WRITER->flush();
}

//////////////////////////////////////////////////////////
// write a feature tag into XML formated text into file:
void LC_MS_XML_writer::write_feature_2_XML_tag(feature* IN, ofstream* in){
  WRITER = in;
  write_feature_2_XML_tag( IN );
  WRITER = NULL;
}


//////////////////////////////////////////////////////////
// write a feature tag into XML formated text into file:
void LC_MS_XML_writer::write_feature_2_XML_tag(feature* IN ){
  // set true so that for the first feature available MS2 will be stored in XML
  write_feature_2_XML_tag( IN, true );
}

//////////////////////////////////////////////////////////
// write a feature tag into XML formated text into file:
void LC_MS_XML_writer::write_feature_2_XML_tag(feature* IN, bool STORE_MS2_DATA){
  
  if(WRITER != NULL){
    
    char buffer[1056];
    
    // open a MS1 tag:
    sprintf(buffer,"<MS1_FEATURE ");  
    

    ///////////////////////////////////////////
    // MS1-peak info
    string specialTag = "";
    if( IN->getFeatureExtraInformation( ).find( "HPin") != string::npos ){
      specialTag = " StandardPeptide=\"1\""; 
    }
    
    //////////////////////////////////
    // write here a string containing the MS/MS scans:
    if( IN->getChildMSMSScanEND() != IN->getChildMSMSScanSTART()){
      
      // sort first:
      sort( IN->getChildMSMSScanSTART(), IN->getChildMSMSScanEND());
      
      
      string ms2ScanString;
      char ms2Buffer[1056];
      sprintf(ms2Buffer," ms2Scans=\"");
      ms2ScanString += ms2Buffer;
      vector<double>::iterator P = IN->getChildMSMSScanSTART( );
      while( P != IN->getChildMSMSScanEND( ) ){
        sprintf(ms2Buffer,"%0.2f,", (*P));  
        ms2ScanString += ms2Buffer;
        P++;
      }
      // remove the last comma:
      if( ms2ScanString.find("," ) != string::npos ){
        ms2ScanString.erase(ms2ScanString.size() - 1, 1);
      }
      sprintf(ms2Buffer,"\"");  
      ms2ScanString += ms2Buffer;
      
      // add to the special tag:
      specialTag += " " + ms2ScanString;
    }
    
    sprintf(buffer,"%sm_z=\"%0.4f\" m_z_Original=\"%0.4f\" charge_state=\"%d\" Tr=\"%0.2f\" Tr_Apex=\"%0.2f\" Tr_Start=\"%0.2f\" Tr_End=\"%0.2f\" Feature_ID=\"%d\" LC_MS_ID=\"%d\" snRatio=\"%0.1f\" BGnoise=\"%0.1f\"%s>\n", buffer,IN->get_MZ(),IN->get_raw_MZ(),IN->get_charge_state(),IN->get_retention_time(), IN->get_raw_retention_time_apex(),IN->get_retention_time_START(),IN->get_retention_time_END(),IN->get_feature_ID(), IN->get_spectrum_ID(), IN->getSignalToNoise(), IN->getBackgroundNoiseLevel(), specialTag.c_str());         
    WRITER->write(buffer,strlen(buffer));
    
    
    ///////////////////////////////////////////
    // open a LC_MS shape tag tag:
    sprintf(buffer,"<LC_INFO ");
    // MS1-peak info
    // old:
    //sprintf(buffer,"%sapex=\"%d\" start=\"%d\" end=\"%d\" AREA=\"%0.2f\" apex_intensity=\"%0.2f\"/>\n", buffer, IN->get_scan_number(), IN->get_scan_start(), IN->get_scan_end(), IN->get_peak_area(), IN->get_apex_peak_intensity());  
    // new version does not store the apex intensity since its coded in the feature profiles:
    sprintf(buffer,"%sapex=\"%d\" start=\"%d\" end=\"%d\" AREA=\"%0.2f\" apex_intensity=\"%0.2f\"/>\n", buffer, IN->get_scan_number(), IN->get_scan_start(), IN->get_scan_end(), IN->get_peak_area(), IN->get_apex_peak_intensity());  
    WRITER->write(buffer,strlen(buffer));    
    

    ///////////////////////////////////////////
    // only open tags if there is any MS2 information:
    // check if there is MS2 info:
    if( STORE_MS2_DATA ){
      write_ms2_info_2_XML_tag( IN );
    }
    
  
    ///////////////////////////////////////////
    // stores MS2 fragment trace information:
    // (if option is turned on in (root) parameter file )
    if( STORE_MS2_FRAGMENT_TRACE_DATA ){
      write_ms2_spectra_2_XML_tag( IN->getMS2Feature() );
    }
    
    
    //////////////////////////////////////////
    // if available: write here the LC elution profile
    // information out:
    if( ( STORE_FEATURE_ELUTION_PROFILES ) && ( IN->getLCelutionProfile( ) != NULL ) ){
      writeLCelutionProfile2XML( IN->getLCelutionProfile( ) );
    }
    
    
    ///////////////////////////////////////////
    // print the matched features:
    if( IN->get_replicate_match_nb() > 1){

      sprintf(buffer,"<MATCHED_FEATURES>\n");
      WRITER->write(buffer,strlen(buffer));
      
      map<int,feature>::iterator M_F = IN->get_match_list_start();
      while( M_F != IN->get_match_list_end()){
        feature* Match = &((*M_F).second); 
        write_feature_2_XML_tag(Match, LC_MS_XML_writer::STORE_BEST_MS2_SCAN_PER_ALIGNED_FEATURE);
        M_F++;
      }
    
      sprintf(buffer,"</MATCHED_FEATURES>\n");
      WRITER->write(buffer,strlen(buffer));
    
    }
    
    ///////////////////////////////////////////    
    // close MS1 tag:
    sprintf(buffer,"</MS1_FEATURE>\n");  
    WRITER->write(buffer,strlen(buffer));
    WRITER->flush();
    
  }
  else{
    printf("\n\t* COULD NOT WRITE XML INTO STREAM::LC_MS_XML_WRITER!*\n");
  }
  
  IN = NULL;
  
}

//////////////////////////////////////////////////////////
// write a the MS2-object info into XML formated text into file:
void LC_MS_XML_writer::write_ms2_info_2_XML_tag(feature* IN){
  
  if( IN->get_MS2_info(-3.0) ){
  
    char buffer[254];
    sprintf(buffer,"<MS2_INFO>\n");
    WRITER->write(buffer,strlen(buffer));
    
    // check if only best scan to store to XML
    if( LC_MS_XML_writer::STORE_BEST_MS2_SCAN_PER_FEATURE ){
      write_ms2_info_2_XML_tag( IN->get_best_MS2_SCAN( -3.0 ) );
    }
    // or all scans
    else{
      write_ms2_info_2_XML_tag( IN->get_MS2_SCAN_LIST_START(), IN->get_MS2_SCAN_LIST_END() );
    }
    
    // open a MS2-info tag tag:
    sprintf(buffer,"</MS2_INFO>\n");
    WRITER->write(buffer,strlen(buffer));
  }
  
  
}

//////////////////////////////////////////////////////////
// write a the MS2-object info into XML formated text into file:
void LC_MS_XML_writer::write_ms2_info_2_XML_tag(map<double, vector<ms2_info> >::iterator START, map<double, vector<ms2_info> >::iterator END){

  while( START != END){
    
    vector< ms2_info>::iterator P = (*START).second.begin();
    while( P != (*START).second.end() ){
      // MS2-peak info
      ms2_info* T_S = &(*P);    
      write_ms2_info_2_XML_tag( T_S );
      T_S = NULL;
      P++;
    }
    START++;
  }
  
  
}
//////////////////////////////////////////////////////////
// write a the MS2-object info into XML formated text into file:
void LC_MS_XML_writer::write_ms2_info_2_XML_tag(ms2_info* IN){
  
  char buffer[502];

  sprintf(buffer,"<MS2_SCAN SQ=\"%s\" PeptideProbability=\"%0.3f\" retention_time=\"%.2f\" Delta_CN=\"%0.3f\" XCorr=\"%0.3f\" MS2_scan=\"%d\" theo_m_z=\"%0.4f\" precursor_m_z=\"%f\" charge_state=\"%d\" interact_file=\"%s\" ms2_type_tag=\"%s\" prev_AA=\"%s\">\n", IN->get_SQ().c_str(), IN->get_PEP_PROB(), IN->getRetentionTime(), IN->get_DELTA_CN(), IN->get_XCORR(), IN->get_SCAN_START(), IN->get_THEO_MZ(), IN->get_MONO_MZ(), IN->get_CHRG(), IN->get_ORIGINAL_INTERACT_FILE().c_str(), IN->get_MS2_TYPE_TAG().c_str() , IN->get_PREV_AA().c_str());
  WRITER->write(buffer,strlen(buffer));
  
  map< string, double >::iterator A = IN->get_ALL_AC_START();
  int count = 1;
  while( ( A != IN->get_ALL_AC_END()) && (LC_MS_XML_writer::MAXIMAL_NB_ALTERNATIVE_PROTEIN_NAMES >= count) ){
    
    if( A->second > -1 ){
      sprintf(buffer,"<ALTERNATIVE_PROTEIN AC=\"%s\" ProteinProb=\"%0.2f\"/>\n", A->first.c_str(), A->second);  
    }
    else{
      sprintf(buffer,"<ALTERNATIVE_PROTEIN AC=\"%s\"/>\n", A->first.c_str());  
    }
    
    WRITER->write(buffer,strlen(buffer));
    count++;
    A++;
  }
  
  // write the modification info:
  sprintf(buffer,"<MODIFICATIONS>\n");  
  WRITER->write(buffer,strlen(buffer));
  map< int, double>::iterator P = IN->get_Modification_list_start();
  while( P != IN->get_Modification_list_end()){
    
    sprintf(buffer,"<AA_MOD pos=\"%d\" value=\"%0.4f\"/>\n", (*P).first, (*P).second);  
    WRITER->write(buffer,strlen(buffer));
      
    P++;
  }
  sprintf(buffer,"</MODIFICATIONS>\n");  
  WRITER->write(buffer,strlen(buffer));		
  sprintf(buffer,"</MS2_SCAN>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  IN = NULL;
}



//////////////////////////////////////////////////////////
// write a header for LC/MS run into XML formated text into file:
void LC_MS_XML_writer::write_LC_MS_run_2_XML_tag(LC_MS* IN){
  
  
  if(WRITER != NULL){
    
    if( IN != NULL ){
      
      char buffer[500];
    
      // open a MS1 tag:
      sprintf(buffer,"<LC_MS_RUN ");  
      // MS1-peak info
      sprintf(buffer,"%sname=\"%s\" number_of_features=\"%d\" tr_min=\"%f\" tr_max=\"%f\" m_z_min=\"%f\" m_z_max=\"%f\">\n", buffer,IN->get_spec_name().c_str(),IN->get_nb_features(), LC_MS_XML_reader::TR_MIN, LC_MS_XML_reader::TR_MAX, LC_MS_XML_reader::FEATURE_MZ_MIN, LC_MS_XML_reader::FEATURE_MZ_MAX);  
      // write to the file:
      WRITER->write(buffer,strlen(buffer));
      
      ////////////////////////////////////////
      // now print the CHILD LC/MS runs:
      
      // open a tag:
      sprintf(buffer,"<CHILD_LC_MS_RUNS>\n");
      // write:
      WRITER->write(buffer,strlen(buffer));
      
      map<int, string>::iterator IT = IN->get_raw_spec_name_start();
      while( IT != IN->get_raw_spec_name_end() ){
        
        // open a tag:
        sprintf(buffer,"<LC_MS_CHILD name=\"%s\" ID=\"%d\"/>\n", (*IT).second.c_str(), (*IT).first);
        // write:
        WRITER->write(buffer,strlen(buffer));
        
        IT++;
      }
      
      // close a tag:
      sprintf(buffer,"</CHILD_LC_MS_RUNS>\n");
      // write:
      WRITER->write(buffer,strlen(buffer));
      
      // open a tag for features:
      sprintf(buffer,"<LC_MS_FEATURES>\n");
      // write:
      WRITER->write(buffer,strlen(buffer));
      
      WRITER->flush();
    }
  }
  else{
    printf("\n\t* COULD NOT WRITE XML INTO STREAM::LC_MS_XML_WRITER!*\n");
  }
  
}

//////////////////////////////////////////////////////////
// write a close tag for LC/MS run:
void LC_MS_XML_writer::write_LC_MS_run_close_tag(){

  if(WRITER != NULL){
    char buffer[500];
    
    // close MS1 tag:
    sprintf(buffer,"</LC_MS_FEATURES>\n");  
    // write to the file:
    WRITER->write(buffer,strlen(buffer));
    
    // close MS1 tag:
    sprintf(buffer,"</LC_MS_RUN>\n");  
    // write to the file:
    WRITER->write(buffer,strlen(buffer));

    WRITER->flush();
  }
  else{
    printf("\n\t* COULD NOT WRITE XML INTO STREAM::LC_MS_XML_WRITER!*\n");
  }
}


/////////////////////////////////////////////
// print similarity assessment info:
void LC_MS_XML_writer::print_similarity_assessement(){
  
  char buffer[500];
  
  sprintf(buffer,"<LC_MS_SIMILARITY>\n");  
  WRITER->write(buffer,strlen(buffer));
  

  // print the colour matrix:
  // check if a correlation matrix image is available: 
  string sim_matrix_image = "image_sim_tree_build_matrix.jpg";
  file_sys* CHECK = new file_sys();
  bool IMAGE = CHECK->check_file_exists(sim_matrix_image);
  delete CHECK;
  CHECK = NULL;
  
  // the similarity matrix:
  sprintf(buffer,"<SIMILARITY_MATRIX>\n");  
  WRITER->write(buffer,strlen(buffer));

  if(IMAGE){
    // open a tag:
    sprintf(buffer,"<IMAGE href=\"%s\"></IMAGE>\n", sim_matrix_image.c_str());
    // write:
    WRITER->write(buffer,strlen(buffer));   
  }

  sprintf(buffer,"</SIMILARITY_MATRIX>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer,"</LC_MS_SIMILARITY>\n");  
  WRITER->write(buffer,strlen(buffer));

}

 
//////////////////////////////////////////
// print out the content of MASTER run
// into tab delim format:
void LC_MS_XML_writer::write_MASTER_LC_MS_to_EXCEL( LC_MS* IN){
  
  MASTER = IN;
  
  
  if( target_dir.empty() ){
  
    target_dir = "ANALYSIS_";
    
    // define the name first:
    // get the input name from the input source directory:
    read_param* def = new read_param();
    // read input directory:
    target_dir = target_dir + def->search_tag("MY PROJECT NAME");
    delete def;
    def = NULL;
    
  }
  
  // check backslash at end:
  if(target_dir[target_dir.size()-1] != '/'){
    target_dir += '/';
  }
  
  char file_name[255];
  sprintf( file_name, "%sMASTER_RUN.txt", target_dir.c_str());
  
  // open a file, etc:
  WRITER = new ofstream();
  WRITER->open(file_name, ofstream::out); 
  if(WRITER->good()){
    
    // print summary
    PRINT_MASTER_SUMMARY_to_EXCEL();
    // print the features table
    print_EXCEL_feature_table();    
    printf("\t\t- Text formatted MASTER LC/MS run saved to '%s'\n", file_name);
  }
  else{
    printf("\nERROR: MASTER LC/MS run could not be saved to '%s', LC_MS_XML_writer::549\n", file_name);
  }
  
  IN = NULL;
}


//////////////////////////////////////////////////
// print the summary of the MASTER run
void LC_MS_XML_writer::PRINT_MASTER_SUMMARY_to_EXCEL( ){
 
  char buffer[256]; 
  string SEP = "\t";
  
  // get date and time:
  time_t rawtime;
  struct tm* timeinfo;
  time(&rawtime);
  timeinfo = localtime( &rawtime);
  sprintf(buffer,"\n#############################################################\n");
  sprintf(buffer,"%s#####\n",buffer);
  sprintf(buffer,"%s#####  MASTER LC/MS OUTPUT by S U P E R  H I R N\n",buffer);
  sprintf(buffer,"%s#####  Lukas Mueller (Lukas.Mueller@imsb.biol.ethz.ch), %s\n\n",buffer,asctime(timeinfo));
  WRITER->write(buffer,strlen(buffer));
  
  // print the name: 
  if(MASTER->get_spec_name().size() > 0){
    sprintf(buffer, "LC-MS name: %s\n", MASTER->get_spec_name().c_str());
    WRITER->write(buffer,strlen(buffer));
  }
  else{
    sprintf(buffer,"LC-MS ID: %d\n", MASTER->get_spectrum_ID());
    WRITER->write(buffer,strlen(buffer));
  }
  
  // print the sub LC/MS runs:
  sprintf(buffer, "\t\t( ");
  WRITER->write(buffer,strlen(buffer));   
  map<int, string>::iterator SUB = MASTER->get_raw_spec_name_start();
  while(SUB != MASTER->get_raw_spec_name_end()){
    sprintf(buffer, "%s[%d] | ", (*SUB).second.c_str(), (*SUB).first);
    WRITER->write(buffer,strlen(buffer));   
    SUB++;
  }
  sprintf(buffer, " )\n");
  WRITER->write(buffer,strlen(buffer));   
  
  
   // print # of features and valid lc-peaks:
  sprintf(buffer,"# detected features: %d [%d with MS2 info]\n", MASTER->get_nb_features(), MASTER->get_nb_identified_features());
  WRITER->write(buffer,strlen(buffer));
  
  // print how # of common features in 0 - count LC_MS runs of this run:
  int count = MASTER->get_nb_raw_specs();
  sprintf(buffer,"common features matched in replicates:\n");
  WRITER->write(buffer,strlen(buffer));
  
  while( count > 0){
    int NB = MASTER->get_nb_common_peaks(count);
    sprintf(buffer,"common features in %d replicates: %d\n", count, NB);
    WRITER->write(buffer,strlen(buffer));
    count--;
  }
  
  sprintf(buffer,"\n\n--------------------------------------------------------------------------\n");
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"Detailed list of matched features within the replicates:\n\n");
  WRITER->write(buffer,strlen(buffer));
  
}



//////////////////////////////////////////////////
// print the content of the spectra (features)
void LC_MS_XML_writer::print_EXCEL_feature_table(){
  
  char buffer[512];
  
  string SEP = "\t";
  string EXCEL_OUT_STYLE = "Lukas";
  
  ////////////////////////////////////////////////////
  // this here is my header:
  // feature columns:
  // for all possible lc_peak matches:
  if( EXCEL_OUT_STYLE.find("Lukas") != string::npos ){ 
    sprintf(buffer,"M/z%s",SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"Tr%s",SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"CHRG%s",SEP.c_str());
    WRITER->write(buffer,strlen(buffer));
    // ms2 info:
    sprintf(buffer,"AC%s",SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"SQ%s",SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"Pep.Prob%s",SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"# Rep. Match%s",SEP.c_str());
    WRITER->write(buffer,strlen(buffer));
    
    // now print the areas
    for( int i=0; i < MASTER->get_nb_raw_specs(); i++){
      sprintf(buffer,"AREA_%d%s",i,SEP.c_str()); 
      WRITER->write(buffer,strlen(buffer));
    }
  }
  ////////////////////////////////////////////////////
  
  
  ////////////////////////////////////////////////////
  // this here is Olga's header:
  // feature columns:
  // for all possible lc_peak matches:
  else if( EXCEL_OUT_STYLE.find("Olga") != string::npos ){ 
    sprintf(buffer,"M/z Start%s",SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"M/z End%s",SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"Tr Start%s",SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"Tr End%s",SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"Mz%s",SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"Tr%s",SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"CHRG%s",SEP.c_str());
    WRITER->write(buffer,strlen(buffer));
    
    // now print the areas
    map<int, string>::iterator N = MASTER->get_raw_spec_name_start();
    while( N != MASTER->get_raw_spec_name_end()){
      sprintf(buffer,"%s[%d]%s", (*N).second.c_str(), (*N).first, SEP.c_str()); 
      WRITER->write(buffer,strlen(buffer));
      N++;
    }
  }
  ////////////////////////////////////////////////////
    
  sprintf(buffer,"\n");
  WRITER->write(buffer,strlen(buffer));
  WRITER->flush();

  vector<LC_MS_FEATURE>::iterator p = MASTER->get_feature_list_begin();
  while(p != MASTER->get_feature_list_end()){
    // print the content of the feature match list
    if( EXCEL_OUT_STYLE.find("Lukas") != string::npos ){ 
      print_feature_to_EXCEL_table( &(*p) );
    }
    else if( EXCEL_OUT_STYLE.find("Olga") != string::npos ){ 
      print_feature_to_EXCEL_table_APML_STYLE( &(*p) );
      // new line:
    }
    sprintf(buffer,"\n");
    WRITER->write(buffer,strlen(buffer));
    WRITER->flush();

    p++;
  }
  
  WRITER = NULL;
}


///////////////////////////////////////////////////////////////////////
// prints the matched feature list for a peptide:
void LC_MS_XML_writer::print_feature_to_EXCEL_table( feature* IN ){
  
  double Pep_Proph_THRESHOLD = -3;
  char buffer[512];
  string SEP = "\t";
  
  /////////////////////////////////////////
  // print the general MS1_info:
  // print matched features:
  sprintf(buffer,"%0.4f%s",IN->get_MZ(),SEP.c_str()); 
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"%0.2f%s",IN->get_retention_time(),SEP.c_str()); 
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"+%d%s",IN->get_charge_state(),SEP.c_str());
  WRITER->write(buffer,strlen(buffer));
  
  /////////////////////////////////////////
  // write MS2 info:
  if( IN->get_MS2_info( Pep_Proph_THRESHOLD )){
    
    // get the best scan:
    ms2_info* info = IN->get_best_MS2_SCAN( Pep_Proph_THRESHOLD );
    
    // in case no AC information:
    if( info->getACListRefr()->empty() ){
      sprintf(buffer,"na%s",SEP.c_str()); 
    }
    // print here all ACs out:
    else if( feature::PRINT_ALL_ACs ){
      string ACs;
      map<string, double>::iterator P = info->get_ALL_AC_START();
      int count = 0;
      while( (count <= LC_MS_XML_writer::MAXIMAL_NB_ALTERNATIVE_PROTEIN_NAMES) && ( P != info->get_ALL_AC_END() ) ){
        char acBuffer[521];
        sprintf( acBuffer, "%s%s(%0.2f),", ACs.c_str(), P->first.c_str(), P->second );
        ACs = acBuffer;
        //ACs += P->first + "("+ P->second + "),";
        P++;
        count++;
      }
      // remove last comma:
      ACs.erase( ACs.size() - 1, 1 );
      sprintf(buffer,"%s%s",ACs.c_str(),SEP.c_str()); 
    }
    // print only the best:
    else{
        sprintf(buffer,"%s%s",IN->get_AC( Pep_Proph_THRESHOLD ).c_str(),SEP.c_str()); 
    }
    WRITER->write(buffer,strlen(buffer));
    
    // print the rest of the MS/MS info:
    sprintf(buffer,"%s%s",IN->get_TOTAL_SQ( Pep_Proph_THRESHOLD ).c_str(), SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"%0.2f%s",IN->get_pep_prob( Pep_Proph_THRESHOLD ),SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
  }
  else{
    sprintf(buffer,"%s%s%s",SEP.c_str(),SEP.c_str(),SEP.c_str()); 
    WRITER->write(buffer,strlen(buffer));
  }
  
  // print matched features list size::
  sprintf(buffer,"%d%s",IN->get_replicate_match_nb(),SEP.c_str()); 
  WRITER->write(buffer,strlen(buffer));
  
  //////////////////////////////////////////////////////
  // now print all peak areas for every id from 0-x
  // get max id:
  int max_ID = MASTER->get_nb_raw_specs()+1;
  for( int ID=0; ID < max_ID;ID++){
    
    if( IN->get_spectrum_ID() == ID){
      sprintf(buffer,"%0.2f%s", IN->get_peak_area(),SEP.c_str());
      WRITER->write(buffer,strlen(buffer));
    }
    else{ 
      map<int,feature>::iterator F = IN->find_match_by_id( ID);
      if( F != IN->get_match_list_end()){
        sprintf(buffer,"%0.2f%s", (*F).second.get_peak_area(),SEP.c_str());
        WRITER->write(buffer,strlen(buffer));
      }
      else{
        sprintf(buffer,"%s",SEP.c_str());
        WRITER->write(buffer,strlen(buffer));
      }
    }
  }  
  
}

///////////////////////////////////////////////////////////////////////
// prints the matched feature list for a peptide:
void LC_MS_XML_writer::print_feature_to_EXCEL_table_APML_STYLE( feature* IN ){
  
  double Pep_Proph_THRESHOLD = -3;
  
  char buffer[256];
  string SEP = "\t";
  
  /////////////////////////////////////////
  // print the general MS1_info:
  // print matched features:
  sprintf(buffer,"%0.3f%s",IN->get_MZ_START(),SEP.c_str()); 
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"%0.3f%s",IN->get_MZ_END(),SEP.c_str()); 
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"%0.3f%s",IN->get_retention_time_START(),SEP.c_str()); 
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"%0.3f%s",IN->get_retention_time_END(),SEP.c_str()); 
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"%0.3f%s",IN->get_MZ(),SEP.c_str()); 
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"%0.3f%s",IN->get_retention_time(),SEP.c_str()); 
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"%d%s",IN->get_charge_state(),SEP.c_str());
  WRITER->write(buffer,strlen(buffer));
  
  //////////////////////////////////////////////////////
  // now print all peak areas for every id from 0-x
  // get max id:
  int max_ID = MASTER->get_nb_raw_specs();
  for( int ID=0; ID < max_ID;ID++){
    feature* MY = NULL;
    
    if( IN->get_spectrum_ID() == ID){
      MY = IN;
    }
    else{ 
      map<int,feature>::iterator F = IN->find_match_by_id( ID);
      if( F != IN->get_match_list_end()){
        MY = &( (*F).second );
      }
    }
    
    if( MY != NULL ){    
      sprintf(buffer,"("); 
      sprintf(buffer,"%s%0.3f,%0.3f,%0.3f,", buffer, MY->get_MZ(), MY->get_retention_time(), MY->get_peak_area());

      // check for MS2 info:
      if( MY->get_MS2_info( Pep_Proph_THRESHOLD )){
        sprintf(buffer,"%s%s,%s,%0.3f", buffer, MY->get_AC( Pep_Proph_THRESHOLD ).c_str(), MY->get_MOD_SQ( Pep_Proph_THRESHOLD ).c_str(), MY->get_pep_prob( Pep_Proph_THRESHOLD )); 
      }
      else{
        sprintf(buffer,"%sNA,NA,NA",buffer);
      }      
      sprintf(buffer,"%s)%s",buffer,SEP.c_str());
    }
    else{
      sprintf(buffer,"(NA)%s",SEP.c_str()); 
    }
    WRITER->write(buffer,strlen(buffer));
  }

}



///////////////////////////////////////////////////////////////////
// writes out a feature LC profile:
void LC_MS_XML_writer::writeLCelutionProfile2XML( featureLCprofile* IN ){
  
  char buffer[256];
  sprintf(buffer,"<LC_ELUTION_PROFILE ");
  WRITER->write(buffer,strlen(buffer));
  
  // write the monoisotopic peaks:
  sprintf(buffer," mono_peak=\"");
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"");  
  string tmp = "";
  map<int, MS1Signal>::iterator P = IN->getLCelutionSignalsStart();  
  while( P != IN->getLCelutionSignalsEnd() ){
    sprintf(buffer,"%0.2f,%0.2f,", (*P).second.TR, (*P).second.intensity);  
    tmp += buffer;
    P++;
  }
  // remove the last comma:
  if( tmp.find("," ) != string::npos ){
    tmp.erase(tmp.size() - 1, 1);
  }
  WRITER->write(tmp.c_str(),tmp.size());		
  sprintf(buffer,"\"/>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  IN = NULL;

}

////////////////////////////////////////////////////////////////////////////
// store all MS/MS spectra of a ms1 feature
void LC_MS_XML_writer::write_ms2_spectra_2_XML_tag( vector<MS2_feature>* IN ){
  
  if( !IN->empty() ){
    
    char buffer[256];
    sprintf(buffer,"<MS2_SPECTRA>\n");
    WRITER->write(buffer,strlen(buffer));
    
    vector<MS2_feature>::iterator P = IN->begin();
    while( P != IN->end() ){
      write_ms2_spectrum_2_XML_tag( &(*P) );
      P++;
    }
    
    sprintf(buffer,"</MS2_SPECTRA>\n");
    WRITER->write(buffer,strlen(buffer));
  }
}


////////////////////////////////////////////////////////////////////////////
// store all MS/MS spectra of a ms1 feature
void LC_MS_XML_writer::write_ms2_spectra_2_XML_tag( MS2_feature* IN ){
  
  if( IN != NULL ){
    
    char buffer[256];
    sprintf(buffer,"<MS2_SPECTRA>\n");
    WRITER->write(buffer,strlen(buffer));
    
    write_ms2_spectrum_2_XML_tag( IN );
    
    sprintf(buffer,"</MS2_SPECTRA>\n");
    WRITER->write(buffer,strlen(buffer));
  }
}


////////////////////////////////////////////////////////////////////////////
// stores MS2 fragment trace information:
// (if option is turned on in (root) parameter file )
void LC_MS_XML_writer::write_ms2_spectrum_2_XML_tag( MS2_feature* ms2 ){
  
  if( ms2 != NULL ){
    
    char buffer[256];
    
    // write the main coordinates:
    sprintf(buffer,"<MS2_SPECTRUM precursorMZ=\"%0.4f\" precursorTR=\"%0.2f\" precursorChrg=\"%d\" apexScan=\"%d\" ", 
            ms2->getPrecursorMZ(), ms2->getTR(), ms2->getPrecursorChrg(), ms2->getApexScan());
    WRITER->write(buffer,strlen(buffer));
    
    // write the underlaying MS/MS scans (only if more than one):
    if( ms2->getNumberOfSpectraScan() > 1 ){
      
      char str_scan[256];
      int Sstart = (*ms2->getSpectraScanNumberStart());
      vector<int>::iterator z = ms2->getSpectraScanNumberEnd();
      z--;
      int Send = (*z);
      sprintf(str_scan,"%d-%d", Sstart, Send);
      sprintf(buffer,"scans=\"%s\" ", str_scan);
      WRITER->write(buffer,strlen(buffer));
    }
    
    
    // write now the individual consensus fragments:    

    // remap thes fragments by molecular weight:
    map<double, MS2Fragment*> reMap;
    map<double, MS2Fragment>::iterator P = ms2->getMS2FragmentPeakStart();
    while( P != ms2->getMS2FragmentPeakEnd() ){
      reMap.insert( make_pair( P->second.getFragmentMz(), &(P->second) ));  
      P++;
    }
    
    sprintf(buffer,"fragments=\"");
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"");  
    string tmp = "";
    map<double, MS2Fragment*>::iterator PP = reMap.begin();
    while( PP != reMap.end() ){
      sprintf(buffer,"%0.3f,%0.0f,", PP->second->getFragmentMz(), PP->second->getFragmentPeakArea());  
      tmp += buffer;
      PP++;
    }
    
    // remove the last comma:
    tmp.erase(tmp.size() - 1, 1);
    WRITER->write(tmp.c_str(),tmp.size());		
    sprintf(buffer,"\"/>\n");  
    WRITER->write(buffer,strlen(buffer));
 
    
    ms2 = NULL;
  }
}

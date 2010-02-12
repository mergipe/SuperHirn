///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS LC_MS_APML_writer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_LC_MS_APML_WRITER

#include "GENERAL_USE.h"


////////////////////////////////////////////////
// constructor for the object LC_MS_APML_writer:
LC_MS_APML_writer::LC_MS_APML_writer(){
  WRITER = NULL;
  MASTER = NULL;
}

////////////////////////////////////////////////
// constructor for the object LC_MS_APML_writer:
LC_MS_APML_writer::LC_MS_APML_writer( string IN ){
  WRITER = NULL;
  MASTER = NULL;
  target_dir = IN;
}

//////////////////////////////////////////////////
// class desctructor of LC_MS_APML_writer
LC_MS_APML_writer::~LC_MS_APML_writer(){
  if( WRITER != NULL ){
    delete WRITER;
    WRITER = NULL;
  }
  MASTER = NULL;
}

//////////////////////////////////////////////////
// class copy constructor of LC_MS_APML_writer
LC_MS_APML_writer::LC_MS_APML_writer(const LC_MS_APML_writer& tmp){
  WRITER = tmp.WRITER;
}

//////////////////////////////////////////////////
// class copy constructor of LC_MS_APML_writer
LC_MS_APML_writer::LC_MS_APML_writer(const LC_MS_APML_writer* tmp){
  WRITER = tmp->WRITER;
}

//////////////////////////////////////////////////
// copy constructor:
LC_MS_APML_writer& LC_MS_APML_writer::operator=(const LC_MS_APML_writer& tmp){
  WRITER = tmp.WRITER;
  return *this;
}
    

/*
/////////////////////////////////////////////////////////
// extracts the output directory:
void LC_MS_APML_writer::set_OUT_DIR(){
  
  if( out_file.find( "ANALYSIS" ) != string::npos ){
    target_dir = out_file;
  }
  else if( target_dir.empty() ){
    
    //////////////////////////////////
    // read parameters from "param.def"
    read_param* def = new read_param();  
    target_dir += def->search_tag("MY PROJECT NAME");  
    delete def;
    def = NULL;
    //////////////////////////////////
    
    if( ! target_dir.empty() ){
      target_dir = "ANALYSIS_" + target_dir;
      
      if( target_dir[ target_dir.size() - 1 ] != '/'){
        target_dir += "/";
      }
      out_file.insert( 0, target_dir );    
    }
    
  }
  
  // check slash at end:
  if( target_dir[ target_dir.size() - 1 ] != '/'){
    target_dir += "/";
  }
  
  if( out_file.find( "ANALYSIS" ) == string::npos ){
    out_file.insert( 0, target_dir );    
  }
  
  // check that the extension is really .apml
  // otherwise replace it:
  if( out_file.find( ".apml") == string::npos ){
    // remove everything after the dot:
    if( out_file.find( ".") != string::npos ){
      out_file =  out_file.erase( out_file.find( "." ), out_file.size() - out_file.find( "." ) );
    }
      
    // add the .apml extension:
    out_file = out_file + ".apml";
  }
}
*/

/////////////////////////////////////////////////////////
// extracts the output directory:
void LC_MS_APML_writer::set_OUT_DIR(){
  
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

  // check that the extension is really .apml
  // otherwise replace it:
  if( out_file.find( ".apml") == string::npos ){
    // remove everything after the dot:
    if( out_file.find( ".") != string::npos ){
      out_file =  out_file.erase( out_file.find( "." ), out_file.size() - out_file.find( "." ) );
    }
    
    // add the .apml extension:
    out_file = out_file + ".apml";
  }
  

}



//////////////////////////////////////////////////////////
// write out a MASTERMAP LC/MS run (MASTER OR NOT)
void LC_MS_APML_writer::WRITE_MASTER_LC_MS_2_APML(string OUT, LC_MS* IN){
  
  out_file = OUT;
  MASTER = IN;
  
  if( open_WRITER() ){
    
    PROCESSING_TYPE = "alignment";
    
    write_APML_MAIN_HEADER();
    
    
    ///////////////////////
    // write feature matching data:
    if( MASTER->get_nb_features() != 0 ){
      
      // write_LC_MS_run_2_APML_tag(MASTER);
      char buffer[254];
      sprintf(buffer,"<alignment>\n");  
      WRITER->write(buffer,strlen(buffer));
      
      // write the experiment analysis
      write_MASTER_MAP_SOURCE(MASTER);

      sprintf(buffer,"<aligned_features count=\"%d\">\n", IN->get_nb_features() );  
      WRITER->write(buffer,strlen(buffer));
        
      // write the individual features to file:
      vector<LC_MS_FEATURE>::iterator FEA = MASTER->get_feature_list_begin();    
      while( FEA != MASTER->get_feature_list_end() ){
        write_aligned_feature_2_APML_tag( &(*FEA) );
        FEA++;
      }
      sprintf(buffer,"</aligned_features>\n");  
      WRITER->write(buffer,strlen(buffer));
      sprintf(buffer,"</alignment>\n");  
      WRITER->write(buffer,strlen(buffer));
      sprintf(buffer,"</data>\n");  
      WRITER->write(buffer,strlen(buffer));
      sprintf(buffer,"</apml>\n");  
      WRITER->write(buffer,strlen(buffer));
      WRITER->flush();
      
    }
    ///////////////////////
  }
  
}



//////////////////////////////////////////////////////////
// write out a list of LC/MS runs
void LC_MS_APML_writer::WRITE_COMPLETE_LC_MS_2_APML(string OUT, vector<LC_MS>* IN){
  
  out_file = OUT;
  
  if( open_WRITER() ){
    
    PROCESSING_TYPE = "peak_picking";
    
    write_APML_MAIN_HEADER();
    
    vector<LC_MS>::iterator START = IN->begin();
    vector<LC_MS>::iterator END = IN->end();

    char buffer[254];
    sprintf(buffer,"<peak_lists count=\"%d\">\n", IN->size());  
    WRITER->write(buffer,strlen(buffer));
    
    // print all runs:
    while( START != END ){
      WRITE_COMPLETE_LC_MS_2_APML( &(*START) );
      START++; 
    }
    
    sprintf(buffer,"</peak_lists>\n");  
    WRITER->write(buffer,strlen(buffer));

    write_APML_MAIN_HEADER_CLOSE();
  }
  
}


//////////////////////////////////////////////////////////
// write out a list of LC/MS runs
void LC_MS_APML_writer::WRITE_COMPLETE_LC_MS_2_APML(string OUT, LC_MS* IN){
  
  out_file = OUT;
  
  if( open_WRITER() ){
    
    PROCESSING_TYPE = "peak_picking";
    
    write_APML_MAIN_HEADER();
    
    char buffer[254];
    if( IN->get_nb_raw_specs() == 0 ){
      sprintf(buffer,"<peak_lists count=\"1\">\n");  
    }
    else{
      sprintf(buffer,"<peak_lists count=\"%d\">\n", IN->get_nb_raw_specs());  
    }
    WRITER->write(buffer,strlen(buffer));
    
    WRITE_COMPLETE_LC_MS_2_APML( IN );
    
    sprintf(buffer,"</peak_lists>\n");  
    WRITER->write(buffer,strlen(buffer));
    
    write_APML_MAIN_HEADER_CLOSE();
  }
  
}


//////////////////////////////////////////////////////////
// write out a LC/MS runs
void LC_MS_APML_writer::WRITE_COMPLETE_LC_MS_2_APML( LC_MS* IN){
  
  if( WRITER != NULL ){
    
    MASTER = IN;
    char buffer[254];
    sprintf(buffer,"<peak_list source=\"%s\">\n", IN->get_spec_name().c_str());  
    WRITER->write(buffer,strlen(buffer));
    
    sprintf(buffer,"<features count=\"%d\">\n", IN->get_nb_features() );
    WRITER->write(buffer,strlen(buffer));
    
    vector<LC_MS_FEATURE>::iterator F = IN->get_feature_list_begin();
    // print all runs:
    while( F != IN->get_feature_list_end() ){
      write_feature_2_APML_tag( &(*F) );
      F++; 
    }
    sprintf(buffer,"</features>\n");  
    WRITER->write(buffer,strlen(buffer));
    
    sprintf(buffer,"</peak_list>\n");  
    WRITER->write(buffer,strlen(buffer));
    
  }
  
  IN = NULL;
}


///////////////////////////////////////////////////////////
// write the source runs:
void LC_MS_APML_writer::write_MASTER_MAP_SOURCE(LC_MS* MASTER){
  
  if(WRITER != NULL){
    
    char buffer[500];
    sprintf(buffer,"<feature_source_list count=\"%d\">\n", MASTER->get_nb_raw_specs() );  
    WRITER->write(buffer,strlen(buffer));

    map<int, string>::iterator R = MASTER->get_raw_spec_name_start();
    while( R != MASTER->get_raw_spec_name_end() ){

      sprintf(buffer,"<source id=\"%d\" location=\"%s\"/>\n", (*R).first, (*R).second.c_str() );  
      WRITER->write(buffer,strlen(buffer));
    
      R++;
    }
    
    sprintf(buffer,"</feature_source_list>\n");  
    WRITER->write(buffer,strlen(buffer));
  }
}


///////////////////////////////////////////////////////////
// opens a file for writing:
bool LC_MS_APML_writer::open_WRITER(){
  
  // set the output directory:
  set_OUT_DIR();
  
  if( WRITER != NULL){
    delete WRITER;
    WRITER = NULL;
  }
  
  WRITER = new ofstream();
  WRITER->open(out_file.c_str(), ofstream::out); 
  
  if(WRITER->good()){
    printf("\t\t- LC/MS MASTER was saved to '%s'\n", out_file.c_str());
    return true;
  }
  else{
    printf("\nERROR: opening file '%s', LC_MS_APML_writer::236\n", out_file.c_str());
    return false;
  }
}


//////////////////////////////////////////////////////////
// write XML HEADER:
void LC_MS_APML_writer::write_APML_MAIN_HEADER(){
  
  if( WRITER == NULL ){
    open_WRITER();
  }

  if( WRITER != NULL ){
    string H = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"; 
    // write to the file:
    WRITER->write(H.c_str(), H.size());
    
    // write dtd / css:
    string C = "<apml xmlns=\"http://www.systemsbiology.org/apml\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">\n";
    WRITER->write(C.c_str(), C.size());
    
    // software information:
    C = "<dataProcessing>\n";
    WRITER->write(C.c_str(), C.size());
    C = "<software name=\"SuperHirn\" version=\"v0.1\" type=\"";
    C += PROCESSING_TYPE + "\"/>\n";
    WRITER->write(C.c_str(), C.size());
    C = "</dataProcessing>\n";
    WRITER->write(C.c_str(), C.size());
    
    C = "<data>\n";
    WRITER->write(C.c_str(), C.size());
  }
}


//////////////////////////////////////////////////////////
// write unformatted XML HEADER:
void LC_MS_APML_writer::write_unformatted_APML_MAIN_HEADER(){
  
  set_OUT_DIR();
  
  if( open_WRITER() ){
    
    string H = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"; 
    // write to the file:
    WRITER->write(H.c_str(), H.size());
    
    // write dtd / css:
    string C = "<apml xmlns=\"http://www.systemsbiology.org/apml\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">";
    WRITER->write(C.c_str(), C.size());
  }
}


//////////////////////////////////////////////////////////
// write unformatted XML HEADER with input XST file:
void LC_MS_APML_writer::write_unformatted_APML_MAIN_HEADER( string XST_FILE){

  set_OUT_DIR();

  if( open_WRITER() ){
    
    string H = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"; 
    // write to the file:
    WRITER->write(H.c_str(), H.size());
    
    // write dtd / css:
    string C = "<apml xmlns=\"isb_apml\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance";
    C = C + " U:\\development\\apml\\apml.xsd\">\n";
    WRITER->write(C.c_str(), C.size());
    
    C = "<data>\n";
    WRITER->write(C.c_str(), C.size());
  }
}



//////////////////////////////////////////////////////////
// write XML HEADER:
void LC_MS_APML_writer::write_APML_MAIN_HEADER_CLOSE(){
  // version:
  string C = "</data>\n";
  WRITER->write(C.c_str(), C.size());
  C = "</apml>\n";
  WRITER->write(C.c_str(), C.size());
  WRITER->flush();
  WRITER->close();
  WRITER = NULL;

}  

//////////////////////////////////////////////////////////
// write comment:
void LC_MS_APML_writer::write_comment_into_APML(string COM){
  
  string OUT = "<!-- ";
  OUT += COM;
  OUT += " --!>";
  // write to the file:
  WRITER->write(OUT.c_str(),OUT.size());
  WRITER->flush();
}

//////////////////////////////////////////////////////////
// write a feature tag into APML formated into file:
void LC_MS_APML_writer::write_aligned_feature_2_APML_tag(vector<feature*>* IN){

  vector<feature*>::iterator F = IN->begin();
  while( F != IN->end() ){
    write_aligned_feature_2_APML_tag( *F );
    F++;
  }
}

//////////////////////////////////////////////////////////
// write a feature tag into APML formated into file:
void LC_MS_APML_writer::write_aligned_feature_2_APML_tag(feature* IN){
  
  if(WRITER != NULL){
    
    char buffer[500];
    // open a MS1 tag:
    sprintf(buffer,"<aligned_feature id=\"%d\">\n", IN->get_feature_ID() );  
    WRITER->write(buffer,strlen(buffer));
    
    // write the aligned feature coordinate
    write_aligned_feature_coordinate(IN);
    
    // write all the matched features out:
    sprintf(buffer,"<features count=\"%d\">\n", IN->get_replicate_match_nb() );
    WRITER->write(buffer,strlen(buffer));
    write_feature_2_APML_tag( IN );
    
    map<int,feature>::iterator M_F = IN->get_match_list_start();
    while( M_F != IN->get_match_list_end()){
      feature* Match = &((*M_F).second); 
      write_feature_2_APML_tag(Match);
      M_F++;
    }
    sprintf(buffer,"</features>\n");
    WRITER->write(buffer,strlen(buffer));      
    
    
    //////////////////////////////
    // write the ms2 info out ( Ppids ):
    write_ms2_info_2_APML_tag( IN->get_MS2_SCAN_LIST_START(), IN->get_MS2_SCAN_LIST_END(), IN->get_MS2_SCANS_SIZE());
    
    sprintf(buffer,"</aligned_feature>\n");  
    WRITER->write(buffer,strlen(buffer));
    WRITER->flush();
  }
  else{
    printf("\n\t* COULD NOT WRITE XML INTO STREAM::LC_MS_XML_WRITER!*\n");
  }
  
  IN = NULL;
  
}


//////////////////////////////////////////////////////////
// write the aligned feature coordinate
void LC_MS_APML_writer::write_aligned_feature_coordinate(feature* IN){
  
  char buffer[500];
  
  // MS1-peak info
  sprintf(buffer,"<coordinate mz=\"%f\" rt=\"%f\" intensity=\"%f\" charge=\"%d\" mass=\"%f\">\n", IN->get_MZ(), IN->get_retention_time(), IN->get_peak_area(), IN->get_charge_state(), IN->get_Molecular_Mass() );  
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer,"<scan_range min=\"%d\" max=\"%d\"/>\n", IN->get_scan_start(),IN->get_scan_end() );  
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer,"<time_range min=\"%f\" max=\"%f\"/>\n", IN->get_retention_time_START(),IN->get_retention_time_END() );  
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer,"<mz_range min=\"%f\" max=\"%f\"/>\n", IN->get_MZ_START(), IN->get_MZ_END());  
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer,"</coordinate>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  IN = NULL;	
}


//////////////////////////////////////////////////////////
// write the individual feature coordinate
void LC_MS_APML_writer::write_feature_coordinate(feature* IN){
  
  char buffer[500];

  // MS1-peak info
  sprintf(buffer,"<coordinate mz=\"%f\" rt=\"%f\" intensity=\"%f\" charge=\"%d\" mass=\"%f\">\n", IN->get_MZ(), IN->get_raw_retention_time_apex(), IN->get_peak_area(), IN->get_charge_state(), IN->get_Molecular_Mass() );  
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer,"<scan_range min=\"%d\" max=\"%d\"/>\n", IN->get_scan_start(),IN->get_scan_end() );  
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer,"<time_range min=\"%f\" max=\"%f\"/>\n", IN->get_retention_time_START(),IN->get_retention_time_END() );  
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer,"<mz_range min=\"%f\" max=\"%f\"/>\n", IN->get_MZ_START(), IN->get_MZ_END());  
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer,"</coordinate>\n");  
  WRITER->write(buffer,strlen(buffer));
    
  IN = NULL;	
}


/////-------SHOULD RE-NAME the method name ----///////////////////////////////////////
// write a feature elution profile
void LC_MS_APML_writer::write_APML_feature_Elution_Profile(featureLCprofile* profile, feature* IN){
  
  if( profile != NULL ){
  
    char buffer[500];
    
    // base info
    sprintf(buffer,"<multi_scan_peak peak_offset=\"0\">\n");


//count=\"%d\">\n", profile->getNbLCelutionSignals());  
    WRITER->write(buffer,strlen(buffer));

    // write the profile points:
    map<int, MS1Signal>::iterator P = profile->getLCelutionSignalsStart();
    while( P != profile->getLCelutionSignalsEnd() ){
      
      sprintf(buffer,"<single_scan_peak rt=\"%0.3f\" intensity=\"%0.2f\" mz=\"%0.3f\" scan=\"%d\" />\n", 
              (*P).second.TR,(*P).second.intensity, (*P).second.mass ,(*P).second.scan);  
     WRITER->write(buffer,strlen(buffer));
      P++;
    }

    //write the coordinate   
    write_feature_coordinate(IN); 

    sprintf(buffer,"</multi_scan_peak>\n");  
    WRITER->write(buffer,strlen(buffer));
  
    profile = NULL;
  }
}

//////////////////////////////////////////////////////////
// write a feature tag into APML formated file:
void LC_MS_APML_writer::write_feature_2_APML_tag(feature* IN){
  
  if(WRITER != NULL){
    
    char buffer[500];
    string LCMS_source = "";
    if( MASTER ){
      LCMS_source = MASTER->get_raw_spec_name( IN->get_spectrum_ID() );
      if( LCMS_source == "" ){
        LCMS_source = MASTER->get_spec_name(); 
      }
    }
    
    
    /////////////////////////////////
    // check if there is a specifal tag for the feature:
    string tag = "";
    if( IN->getFeatureExtraInformation( ).find( "HPin") != string::npos ){
      tag = "StandardPeptide=\"1\""; 
    }
        
    // open a MS1 tag:
    bool MS2_OUTPUT = true;
    if( PROCESSING_TYPE == "alignment" ){
      sprintf(buffer,"<feature source=\"%s\" id=\"%d\" %s>\n", LCMS_source.c_str(), IN->get_feature_ID(), tag.c_str()); 
      MS2_OUTPUT = false;
    }
    else if(PROCESSING_TYPE == "peak_picking" ){
      sprintf(buffer,"<feature id=\"%d\" %s>\n", IN->get_feature_ID(), tag.c_str());        
    }
    else{
      sprintf(buffer,"<feature id=\"%d\" %s>\n", IN->get_feature_ID(), tag.c_str());        
    }
    
    WRITER->write(buffer,strlen(buffer));

    // write the feature coordinate
    write_feature_coordinate(IN);
    
    // open a MS2-info tag tag:
    if( ( !IN->check_MS2_empty() ) && (MS2_OUTPUT) ){
      // check if there is MS2 info:
      write_ms2_info_2_APML_tag( IN->get_MS2_SCAN_LIST_START(), IN->get_MS2_SCAN_LIST_END(), IN->get_MS2_SCANS_SIZE() );
    }    
    
    // write the feature elution profile:
    write_APML_feature_Elution_Profile( IN->getLCelutionProfile(), IN);
    
    // close MS1 tag:
    sprintf(buffer,"</feature>\n");  
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
void LC_MS_APML_writer::write_ms2_info_2_APML_tag( vector<ms2_info>* IN ){
  
  if( !IN->empty() ){
    
    char buffer[254];
    //sprintf(buffer,"<ppids>\n");  
    sprintf(buffer,"<ppids count=\"%d\">\n", IN->size());  
    WRITER->write(buffer,strlen(buffer));
    
    vector< ms2_info>::iterator P = IN->begin();
    while( P != IN->end() ){
      // MS2-peak info
      ms2_info* T_S = &(*P);    
      write_ms2_info_2_APML_tag( T_S );
      T_S = NULL;
      P++;
    }
    
    sprintf(buffer,"</ppids>\n");  
    WRITER->write(buffer,strlen(buffer));    
  }
}

//////////////////////////////////////////////////////////
// write a the MS2-object info into XML formated text into file:
void LC_MS_APML_writer::write_ms2_info_2_APML_tag( vector<ms2_info*>* IN ){
  
  if( !IN->empty() ){
    
    char buffer[254];
    //sprintf(buffer,"<ppids>\n");  
    sprintf(buffer,"<ppids count=\"%d\">\n", IN->size());  
    WRITER->write(buffer,strlen(buffer));
    
    vector< ms2_info*>::iterator P = IN->begin();
    while( P != IN->end() ){
    write_ms2_info_2_APML_tag( (*P) );
      P++;
    }
    sprintf(buffer,"</ppids>\n");  
    WRITER->write(buffer,strlen(buffer));

  }
}

//////////////////////////////////////////////////////////
// write a the MS2-object info into XML formated text into file:
void LC_MS_APML_writer::write_ms2_info_2_APML_tag(map<double, vector<ms2_info> >::iterator START, map<double, vector<ms2_info> >::iterator END, int n){
  
  if( START != END ){
    
    char buffer[254];
    //sprintf(buffer,"<ppids>\n");  
    sprintf(buffer,"<ppids count=\"%d\">\n", n);  
    WRITER->write(buffer,strlen(buffer));
    
    while( START != END){    
      vector< ms2_info>::iterator P = (*START).second.begin();
      while( P != (*START).second.end() ){
        // MS2-peak info
        ms2_info* T_S = &(*P);    
        write_ms2_info_2_APML_tag( T_S );
        T_S = NULL;
        P++;
      }
      START++;
    }  
    
    sprintf(buffer,"</ppids>\n");  
    WRITER->write(buffer,strlen(buffer));    
  }
}

//////////////////////////////////////////////////////////
// write a the MS2-object info into XML formated text into file:
void LC_MS_APML_writer::write_ms2_info_2_APML_tag(ms2_info* IN){
  
  char buffer[500];

  sprintf(buffer,"<ppid ms2_scan_num=\"%d\" precursor_mz=\"%f\" previous_aa=\"%s\" theoretical_mz=\"%f\" charge=\"%d\">\n", IN->get_SCAN_START(), IN->get_MONO_MZ(), IN->get_PREV_AA().c_str(), IN->get_THEO_MZ(), IN->get_CHRG() );
  WRITER->write(buffer,strlen(buffer));

  // original path of interact file:
  //sprintf(buffer,"<dta ms2_match_type=\"%s\">%s</dta>\n", IN->get_MS2_TYPE_TAG().c_str(), IN->get_ORIGINAL_INTERACT_FILE().c_str() );
  sprintf(buffer,"<dta>%s</dta>\n", IN->get_ORIGINAL_INTERACT_FILE().c_str() );
  WRITER->write(buffer,strlen(buffer));
  
  // print the score:
  sprintf(buffer,"<ms2_search_score name=\"\" value=\"%f\"/>\n", IN->get_PEP_PROB() );
  WRITER->write(buffer,strlen(buffer));
  
  // print the peptide sequence:
  sprintf(buffer,"<peptide_sequence>%s</peptide_sequence>\n", IN->get_SQ().c_str() );  
  WRITER->write(buffer,strlen(buffer));

  sprintf(buffer,"<proteins>\n");  
  WRITER->write(buffer,strlen(buffer));		

  // print the possible protein ACs:
  map< string, double >::iterator A = IN->get_ALL_AC_START();
  while( A != IN->get_ALL_AC_END()){    
    sprintf(buffer,"<protein accession_num=\"%s\"/>\n", A->first.c_str());  
    WRITER->write(buffer,strlen(buffer));
    A++;
  }
  sprintf(buffer,"</proteins>\n");  
  WRITER->write(buffer,strlen(buffer));		

  // write the modification info:
  if( IN->check_MODIFICATION() ){
    sprintf(buffer,"<modifications>\n");  
    WRITER->write(buffer,strlen(buffer));
    map< int, double>::iterator P = IN->get_Modification_list_start();
    while( P != IN->get_Modification_list_end()){
      sprintf(buffer,"<modification position=\"%d\" value=\"%f\"></modification>\n", (*P).first, (*P).second);  
      WRITER->write(buffer,strlen(buffer));
      P++;
    }
    sprintf(buffer,"</modifications>\n");  
    WRITER->write(buffer,strlen(buffer));		
    
  }
  
  sprintf(buffer,"</ppid>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  IN = NULL;
}


//////////////////////////////////////////////////////////
// write a header for LC/MS run into XML formated text into file:
void LC_MS_APML_writer::write_LC_MS_run_2_APML_tag(LC_MS* IN){
  
  
  if(WRITER != NULL){
    
    if( IN != NULL ){
      char buffer[500];
    
      // open a MS1 tag:
      sprintf(buffer,"<LC_MS_RUN ");  
      // MS1-peak info
      sprintf(buffer,"%sname=\"%s\" number_of_features=\"%d\">\n", buffer,IN->get_spec_name().c_str(),IN->get_nb_features());  
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
        sprintf(buffer,"<LC_MS_CHILD name=\"%s\" ID=\"%d\"></LC_MS_CHILD>\n", (*IT).second.c_str(), (*IT).first);
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
void LC_MS_APML_writer::write_LC_MS_run_close_tag(){

  if(WRITER != NULL){
    char buffer[500];
    
    // close MS1 tag:
    sprintf(buffer,"</features>\n");  
    WRITER->write(buffer,strlen(buffer));
    
    // close MS1 tag:
    sprintf(buffer,"</data>\n");  
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"</apml>\n");  
    WRITER->write(buffer,strlen(buffer));
    WRITER->flush();
  }
  else{
    printf("\n\t* COULD NOT WRITE APML INTO STREAM::LC_MS_APML_WRITER!*\n");
  }
}

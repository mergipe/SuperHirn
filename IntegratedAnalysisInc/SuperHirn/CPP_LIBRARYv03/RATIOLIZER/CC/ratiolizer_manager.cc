///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS ratiolizer_manager:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_RATIOLIZER_MANAGER
#include "RATIOLIZER_USE.h"




////////////////////////////////////////////////
// constructor for the object ratiolizer_manager:
ratiolizer_manager::ratiolizer_manager(){
  MY_MASTER = NULL;
}

//////////////////////////////////////////////////
// class desctructor of ratiolizer_manager
ratiolizer_manager::~ratiolizer_manager(){
  
  if( MY_MASTER != NULL ){
    delete MY_MASTER;
    MY_MASTER = NULL;
  }
  
  if( ms1RatioAnalyzer != NULL ){
    delete ms1RatioAnalyzer;
    ms1RatioAnalyzer = NULL;
  }
  
  if( proteinRatioAnalyzer != NULL ){
    delete proteinRatioAnalyzer;
    proteinRatioAnalyzer = NULL;
  }
}

//////////////////////////////////////////////////
// class copy constructor of ratiolizer_manager
ratiolizer_manager::ratiolizer_manager(const ratiolizer_manager& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of ratiolizer_manager
ratiolizer_manager::ratiolizer_manager(const ratiolizer_manager* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
ratiolizer_manager& ratiolizer_manager::operator=(const ratiolizer_manager& tmp){
  return *this;
}


///////////////////////////////////////////////////
// start the ratiolizer analysis:
void ratiolizer_manager::start_RATIOLIZER(){
  
  // normnalize the master run:
  NORMALIZE_MASTER_MAP();
  
  ///////////////////////////////////
  // do analysis on the peptide level
  printf("\n\tExtracting Feature/Peptide groups:\n");
  ms1RatioAnalyzer = new MS1_feature_ratiolizer( MY_MASTER );

  // start ratiolizer analysis:
  ms1RatioAnalyzer->start_RATIO_ANALYSIS();
  
  ///////////////////////////////////
  // do analysis on the protein level
  printf("\n\tExtracting Protein groups:\n");
  proteinRatioAnalyzer = new protein_ratiolizer(); 
  // get the computed peptide ratio 
  proteinRatioAnalyzer->set_PEPTIDE_ANALYSIS_RATIO_LIST( ms1RatioAnalyzer->get_PEPTIDE_ANALYSIS_LIST() );
    
  // start the analysis:  
  proteinRatioAnalyzer->start_RATIO_ANALYSIS();
 
  ///////////////////////////////////
  // write the output:
  // print all peptide ratios to simple text file
  ms1RatioAnalyzer->print_text_peptide_analysis();
  // protein level:
  proteinRatioAnalyzer->print_text_protein_analysis();
  // all anaylsis to XML:
  write_analysis_2_XML();
}
  
  
///////////////////////////////////////////////////
// imports the MASTER LC/MS run:
void ratiolizer_manager::import_MASTER_run(){

  printf("\n\tImporting the MasterMap:\n");
  
  if( MY_MASTER != NULL ){
    delete  MY_MASTER;
    MY_MASTER = NULL;
  }
  
  MY_MASTER = new LC_MS();

  // define the name first:
  // get the input name from the input source directory:
  read_param* def = new read_param();
  // read input directory:
  string file_name = "ANALYSIS_" + def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;
  
  // add backslash at end:
  if(file_name[file_name.size()-1] != '/')
    file_name += "/";  
  
  // create a new LC_MS reader:
  LC_MS_XML_reader READ;
  
  // now read all features:
  READ.start_LC_MS_parsing( file_name );
  
  // get the parsed run and store in MY_MASTER:
  if( READ.check_parsed_LCMS_list() ){
    *MY_MASTER = *(READ.get_parsed_LCMS_list_start()) ;
  }
  
  // print overview:
  MY_MASTER->show_info();
}


///////////////////////////////////////////////////
// imports the MASTER LC/MS run:
void ratiolizer_manager::insert_MASTER(LC_MS* IN){
  
  if( MY_MASTER != NULL ){
    delete  MY_MASTER;
    MY_MASTER = NULL;
  }
  MY_MASTER = new LC_MS();
  *MY_MASTER = *IN;
}



////////////////////////////////////////////////////////////////////////
// make intensity normalization of the MASTER map
void ratiolizer_manager::NORMALIZE_MASTER_MAP(){    
  printf("\n\tNormalization of MasterMap:\n");
  MASTER_LC_MS_intensity_normalizer* NORM = new MASTER_LC_MS_intensity_normalizer();

  // compute coefficient
  NORM->start_GLOBAL_INTENSITY_NORMALIZATION( MY_MASTER );
  
  delete NORM;
  NORM = NULL;
  
}



/////////////////////////////////////////////////////////////////////
// write the analysis to XML file:
void ratiolizer_manager::write_analysis_2_XML(){
  
  // define the name first:
  // get the input name from the input source directory:
  read_param* def = new read_param();
  // read input directory:
  string dirName = "ANALYSIS_" + def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;
  
  char file_name[255];
  sprintf( file_name, "%s/ratiolizer_analysis.xml", dirName.c_str());
  
  char buffer[1054];
  
  //////////////////////////////////
  // print the header:
  LC_MS_XML_writer* LC_W = new LC_MS_XML_writer( file_name );
  LC_W->set_OUT_DIR();
  
  //string XST_FILE = "FEATURE_RATIOLIZER/display_FEATURE_RATIOLIZER.xsl";
 // LC_W->write_unformatted_XML_MAIN_HEADER(XST_FILE);
  if( LC_W->open_WRITER() ){
    ofstream* WRITER = LC_W->get_WRITER();
    
    // write open header:
    sprintf(buffer,"<JRatioXML>\n");  
    WRITER->write(buffer,strlen(buffer));

    
    //////////////////////////////////////////////////
    // write out the LC-MS child runs within the mastermap:
    sprintf(buffer,"<MasterMap nbChilds=\"%d\">\n", MY_MASTER->get_nb_raw_specs());
    WRITER->write(buffer,strlen(buffer));

    // write the raw childs:
    sprintf(buffer,"<LCMSchilds>\n");
    WRITER->write(buffer,strlen(buffer));

    map<int, string>::iterator L = MY_MASTER->get_raw_spec_name_start();
    while( L != MY_MASTER->get_raw_spec_name_end() ){
      sprintf(buffer,"<LCMSchild id=\"%d\" name=\"%s\"/>\n", (*L).first, (*L).second.c_str());
      WRITER->write(buffer,strlen(buffer));
      L++;

    }
    
    sprintf(buffer,"</LCMSchilds>\n");
    WRITER->write(buffer,strlen(buffer));

    sprintf(buffer,"</MasterMap>\n");
    WRITER->write(buffer,strlen(buffer));
    
    //////////////////////////////////////////////////
    // print the anaylsis outline:
    sprintf(buffer,"<JRatioAnalysisOutline>\n");  
    WRITER->write(buffer,strlen(buffer));

    
    //////////////////////////////////////////////////
    // write the outline, which runs are selected as nominators etc.
    sprintf(buffer,"<LCMSoutline>\n");  
    WRITER->write(buffer,strlen(buffer));
    
    if(  peptide_ratio_analyzer::RATIO_TARGET_LC_MS != -1){
      
      
      ///////////////////////////////////////////
      // get the area from target = nominatore
      sprintf(buffer,"<NominatorIDs size=\"%d\">\n", peptide_ratio_analyzer::REPLICATE_NUMBER);  
      WRITER->write(buffer,strlen(buffer));
      
      int id = peptide_ratio_analyzer::RATIO_TARGET_LC_MS;
      while( id < ( peptide_ratio_analyzer::RATIO_TARGET_LC_MS + peptide_ratio_analyzer::REPLICATE_NUMBER ) ){
        
        sprintf(buffer,"<LCMSset id=\"%d\"/>\n", id, MY_MASTER->get_raw_spec_name( id ).c_str());  
        WRITER->write(buffer,strlen(buffer));
        id++;
      }
      sprintf(buffer,"</NominatorIDs>\n");  
      WRITER->write(buffer,strlen(buffer));
      
      
      ///////////////////////////////////////////
      // get the area from target = denominatore
      sprintf(buffer,"<DenominatorIDs size=\"%d\">\n", peptide_ratio_analyzer::REPLICATE_NUMBER);  
      WRITER->write(buffer,strlen(buffer));
      
      if( peptide_ratio_analyzer::RATIO_TARGET_LC_MS != 0 ){
        id = 0;
      }
      else{
        id = peptide_ratio_analyzer::REPLICATE_NUMBER;
      }
      int start = id;
      while( id < ( start + peptide_ratio_analyzer::REPLICATE_NUMBER ) ){
        
        sprintf(buffer,"<LCMSset id=\"%d\"/>\n", id, MY_MASTER->get_raw_spec_name( id ).c_str());  
        WRITER->write(buffer,strlen(buffer));
        id++;
      }
      sprintf(buffer,"</DenominatorIDs>\n");  
      WRITER->write(buffer,strlen(buffer));
      
      
      
    }
    sprintf(buffer,"</LCMSoutline>\n");  
    WRITER->write(buffer,strlen(buffer));
    
    
    
    // minimal nnb. replicate matches:
    sprintf(buffer,"<MinRepMatches nb=\"%d\"/>\n",
            peptide_ratio_analyzer::REPLICATE_NUMBER);
    WRITER->write(buffer,strlen(buffer));
    
    
    // print delta mass list
    sprintf(buffer,"<DeltaMassOutline>\n");
    WRITER->write(buffer,strlen(buffer));
    
    
    // write the raw childs:
    sprintf(buffer,"<LCMSrun id=\"0\" name=\"%s\"/>\n", MY_MASTER->get_raw_spec_name( 0 ).c_str());
    WRITER->write(buffer,strlen(buffer));
    
    
    map<int, vector<double> >::iterator F = peptide_DELTA_group::LC_MS_Modification_Masses.find( 0 );
    if(F != peptide_DELTA_group::LC_MS_Modification_Masses.end()){
      vector<double>::iterator P = (*F).second.begin();
      while ( P != (*F).second.end() ) {
        sprintf(buffer,"<deltaMass value=\"%.5f\"/>\n", (*P));
        WRITER->write(buffer,strlen(buffer));
        P++;
      }
    }
    
    sprintf(buffer,"</DeltaMassOutline>\n");
    WRITER->write(buffer,strlen(buffer));
    
    sprintf(buffer,"</JRatioAnalysisOutline>\n");  
    WRITER->write(buffer,strlen(buffer));
    
    
    // write the MS1 feature ratios:
    ms1RatioAnalyzer->write_peptide_group_analysis_2_XML( WRITER );
    // write the protein ratios:
    proteinRatioAnalyzer->write_protein_group_analysis_2_XML( WRITER );
    
    sprintf(buffer,"</JRatioXML>\n");  
    WRITER->write(buffer,strlen(buffer));
  }
  
  delete LC_W;
  LC_W = NULL;
  
}

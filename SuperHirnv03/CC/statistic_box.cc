///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS statistic_box:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_COEFF_COMPUTER
#define USE_STAT_CORRELATE
#define USE_PROFILE_CLUSTER_TOOL
#define USE_STATISTIC_BOX
#define USE_DELTA_GROUPER


#include "use.h"

bool statistic_box::ABSOLUTE_NORMALIZATION_MODE = false;


////////////////////////////////////////////////
// constructor for the object statistic_box:
statistic_box::statistic_box(){  
  MASTER = NULL;
  PRO_COMP = NULL;
}

//////////////////////////////////////////////////
// class desctructor of statistic_box
statistic_box::~statistic_box(){
  RELATIVE_VARIATIONS.clear();
  ABSOLUTE_VARIATIONS.clear();
  TARGET_PROFILE_GROUPS.clear();
  
  if( MASTER != NULL){
    delete MASTER;
    MASTER = NULL;
  }
  
  if( PRO_COMP != NULL ){
    delete PRO_COMP;
    PRO_COMP = NULL;    
  }

}

//////////////////////////////////////////////////
// class copy constructor of statistic_box
statistic_box::statistic_box(const statistic_box& tmp){
}

//////////////////////////////////////////////////
// copy constructor:
statistic_box& statistic_box::operator=(const statistic_box& tmp){
  return *this;
}
    

//**************************************************************
// LC/MS RUN NORMALIZATION FUNCTIONS

// NORMALIZE INTENSITY VARIATION ACROSS SAMPLES:
// compute variation profiles along TR for every run:
// and apply normalization to features for each input MASTER run:
void statistic_box::start_GLOBAL_INTENSITY_NORMALIZATION(){
  
  MASTER_LC_MS_intensity_normalizer* NORM = new MASTER_LC_MS_intensity_normalizer();
  
  // compute coefficient
  NORM->start_GLOBAL_INTENSITY_NORMALIZATION( MASTER );
  
  delete NORM;
  NORM = NULL;
}



//**************************************************************
// PROFILE CLUSTERING FUNCTIONS

//////////////////////////////////////////////////////////
// start the clustering process:
void statistic_box::START_MS1_FEATURE_PROFILING(){
  
  TARGET_PROFILE_GROUPS.clear();
  
  //**************************************************************
  // for paola fake import from text file:
/*
  LC_MS_XML_reader reader;
  reader.start_LC_MS_parsing_MASTER_TAB_DEL_FORMAT( "/Users/muellelu/Desktop/ToLukas_SD_Dataset.txt");
  //reader.start_LC_MS_parsing_MASTER_TAB_DEL_FORMAT( "/Users/muellelu/Desktop/ToLukas_YPD_Dataset.txt");
  MASTER = new LC_MS ( &(*reader.get_parsed_LCMS_list_start()) );
  MASTER->show_info();
 */
  //**************************************************************
  
  
  printf("\n\t\t MS1 FEATURE PROFILING [ k-means clustering ]:\n");  
  printf("\t\t\t- MasterMap:'%s'\n", MASTER->get_spec_name().c_str());

  // start here the profiling clustering process:
  profile_cluster_tool* STAT = new profile_cluster_tool(MASTER);

  // start the clustering process:
  STAT->start_profile_clustering();
  
  // get the profile groups from the clustering:
  TARGET_PROFILE_GROUPS = STAT->get_TARGET_PROFILE_GROUPS();
  
  // store the clustering in XML
  STAT->store_END_CLUSTERS_to_XML( );
  
  delete STAT;
  STAT = NULL;
 
}



////////////////////////////////////////////////////
// analyze feature profiles
// either by the created profile gropus
// or rank them according to profile similarity 
// to teh target profile:
void statistic_box::ANALYZE_MS1_FEATURE(){
  
  // check first if a K-Means clusetring analyis has been
  // performed -> search for the file:
  if( checkKmeanClustringFile() ){
    EXTRACT_PROFILE_GROUP_FROM_KMEANS();
  }
  else{
    EXTRACT_PROFILE_GROUP_FROM_MASTERMAP();
  }

  // analyze the profile groups:
  ANALYZE_MS1_FEATURE_PROFILING();

}


////////////////////////////////////////////////////
// analyze the extracted profile groups
// - build peptide groups
// - build protein groups
// - make a statistic for every profile groups:
void statistic_box::ANALYZE_MS1_FEATURE_PROFILING(){
  
  // initate the post annotation object:
  feature_updater* UPDATER = new feature_updater(); 
  UPDATER->INIT_ALL();
  
  ///////////////////////////
  // iterate through all the best fit clusters:
  vector< profile_group>::iterator P = PRO_COMP->get_PROFILE_GROUPS_START();
  while( P != PRO_COMP->get_PROFILE_GROUPS_END() ){
    
    profile_group* THIS_P = &(*P);
    printf("\n\t\t*************************\n");
    printf("\t\t ANALYSIS of profile_group to target profile %d:\n", THIS_P->get_TARGET_PROFILE_ID() );
    
    ////////////////////////////////////////////
    // start the feature MS2 updating process:
    printf("\t\t\t- MS1 feature post-annotation updating ...\n");
    UPDATER->update_feature_identity( THIS_P );
    // UPDATER->print_update_summary();
    // THIS_P->show_info();
    
    
    ////////////////
    // start the peptide cluster process:
    printf("\t\t- peptide grouping ...\n");
    DELTA_grouper* D_C = new DELTA_grouper();
    D_C->extract_PEPTIDE_GROUPS_from_PROFILE_GROUP( THIS_P );


    ///////////////
    // extracted protein groups:
    printf("\t\t- protein grouping ...\n");
    D_C->extract_PROTEIN_GROUPS_from_PROFILE_GROUP( THIS_P );    
    delete D_C;
    D_C = NULL;
  
    
    ///////////////
    // do here a peptide and then protein clustering for every input profile:
    // store the output clusters and analze them:
    // make a statistical analysis:
    // print the groups
    // PRO_COMP->print_PEPTIDE_profile_group( &((*C).second), (*P).first);
    // PRO_COMP->print_PROTEIN_profile_group( &((*C).second), (*P).first);
    // score by average protein profile:
    PRO_COMP->statistical_cluster_analysis( THIS_P );

    
    /////////////////
    // print the output to XML file:
    printf("\n\t\t- print individual cluster analysis to XML / TXT file ...\n");
    PRO_COMP->print_profile_group_to_XML( THIS_P );
    // PRO_COMP->print_profile_group_to_APML( THIS_P );
    PRO_COMP->print_profile_group_to_TXT( THIS_P );
    
  
    // next profile group:
    P++;
  }
  
  delete UPDATER;
  UPDATER = NULL;

  
  // process now the best protein hits:
  printf("\n\t\t- Processing extracted best proteins to target profiles\n");
  PRO_COMP->process_BEST_PROTEIN_HITS( );
  
  
  ////////////////////////////////////////////////////////////
  // EVALUATION OF TOTAL PROFILING PROCESS: 
  /*
  profiling_evaluator* PROF_EVAL = new profiling_evaluator( PRO_COMP );

  // start the evaluation process
  PROF_EVAL->START_PROFILING_EVALUATION( );

  delete PROF_EVAL;
  PROF_EVAL = NULL;
  
  delete PRO_COMP;
  PRO_COMP = NULL;
  */
}


////////////////////////////////////////////////////////////////
// extracts from the mastermap a profile group
// filters for features with min profile lengths
void statistic_box::EXTRACT_PROFILE_GROUP_FROM_MASTERMAP(){

  // import the master map:
  import_MASTER_run();
  printf("\t\t- extract feature profiles DIRECTLY from MasterMap '%s'...\n", MASTER->get_spec_name().c_str()); 
  
  PRO_COMP = new profile_comparer();
  PRO_COMP->extract_PROFILE_GROUP_from_MASTERMAP(MASTER);
  
}  


////////////////////////////////////////////////////////////////
// extracts profile groups directly from Kmeans groups:
void statistic_box::EXTRACT_PROFILE_GROUP_FROM_KMEANS(){
  
  PRO_COMP = new profile_comparer();
  // import the XML from the clustering process:
  PRO_COMP->import_profiling_data_from_XML();
  // select the best scoring clusters to profile:
  PRO_COMP->EXTRACT_BEST_TARGET_PROFILE_CLUSTERS();    
  
}  



//**************************************************************
// search for delta peptide pairs:
void statistic_box::extract_DELTA_PEPTIDE_PAIRS(){

  printf("\t\t- start feature delta pair matching of MASTER '%s'...\n", MASTER->get_spec_name().c_str()); 
  DELTA_grouper* D_C = new DELTA_grouper( MASTER );
  
  // init:
  DELTA_grouper::MIN_NB_PROFILE_POINTS = 1;
  
  // start delta search:
  D_C->start_MASTER_LC_MS_PAIR_MATCHING();
  
  delete D_C;
  D_C = NULL;
}


//**************************************************************
// start ratiolizer analysis:
void statistic_box::start_RATIOLIZER_ANALYSIS(){
  
  printf("\t\t- start ratiolizer analysis of MASTER '%s'...\n", MASTER->get_spec_name().c_str()); 

  MS1_feature_ratiolizer* RL = new MS1_feature_ratiolizer( MASTER );

  // start delta search:
  RL->start_RATIO_ANALYSIS();

  delete RL;
  RL = NULL;

}



//**************************************************************
// IMPORT / GET / SET FUNCTIONS


///////////////////////////////////////////////////
// imports the MASTER LC/MS run:
void statistic_box::import_MASTER_run(){
  
  if( MASTER != NULL ){
    MASTER = NULL;
    delete MASTER;
  }

  // start the importer:    
  LCMSDataImporter* IMP = new LCMSDataImporter();
  LC_MS* THIS_MASTER = IMP->read_MASTER_MAP( );
  MASTER = new LC_MS();
  *MASTER = *THIS_MASTER;
  
  // set master run ID:
  MASTER->set_MASTER_ID( 0 );
  // print overview of imported MASTER:
  MASTER->show_info();
    
  delete IMP;
  IMP = NULL;
  

}


//**************************************************************




//**************************************************
// help functions:

// check if the kmeans fiel is there:
bool statistic_box::checkKmeanClustringFile(){
  
  // get the input name from the input source directory:
  read_param* def = new read_param();
  // read input directory:
  string file_name = "ANALYSIS_" + def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;  
  // add backslash at end:
  if(file_name[file_name.size()-1] != '/'){
    file_name += "/";  
  }
  
  // create file path:
  string Kfile = file_name + "KMEANS_cluster_groups.xml";
  
  file_sys SYS;
  return SYS.check_file_exists( Kfile);
  
  
}


///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS profile_comparer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_PROFILE_COMPARER
#include "use.h"


////////////////////////////////////////////////
// constructor for the object profile_comparer:
profile_comparer::profile_comparer(){
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();  
  // print out
  def->search_tag("gnuplot plot generator", &PRINT_OUT);
  
  // get the input cluster profiles:
  def->search_tag("min. nb. of profile data points", &NB_PROFILE_POINTS);
  peptide_DELTA_group::NB_PROFILE_DATA_POINTS = NB_PROFILE_POINTS;
  
  // get the input cluster profiles:
  def->search_tag("clustering input profiles", &TARGET_CLUSTER_PROFILES);
  protein_group::NB_PROFILE_DATA_POINTS = (*TARGET_CLUSTER_PROFILES.begin()).second.size();
  peptide_DELTA_group::NB_PROFILE_DATA_POINTS = (*TARGET_CLUSTER_PROFILES.begin()).second.size();  

  // get the input cluster profiles:
  def->search_tag("Peptide Prophet Threshold", &PEPTIDE_PROBABILITY_THRESHOLD);

  // get the input cluster profiles:
  def->search_tag("Profile Prophet Threshold", &PROFILE_PROBABILITY_THRESHOLD);

  // whether to score agains target profile or best cluster profile:
  def->search_tag("Target Profile Scoring", &TRUE_TARGET_PROFILE);
    
  delete def;
  def = NULL;
  
}

//////////////////////////////////////////////////
// class desctructor of profile_comparer
profile_comparer::~profile_comparer(){
  PROFILE_PROTEIN_DATA.clear();
  ALL_PROTEINS.clear();
  ALL_MS1_FEATURES.clear();
  ALL_PROFILE_GROUPS.clear();
  BEST_PROTEINS.clear();
  TARGET_CLUSTER_PROFILES.clear();
}

//////////////////////////////////////////////////
// class copy constructor of profile_comparer
profile_comparer::profile_comparer(const profile_comparer& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of profile_comparer
profile_comparer::profile_comparer(const profile_comparer* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
profile_comparer& profile_comparer::operator=(const profile_comparer& tmp){
  return *this;
}
    

//////////////////////////////////////////////////
// add protein group from a profile:
void profile_comparer::add_protein_group_profile(int P_NB, map<int, vector< protein_group> >* IN){

  map<int, vector<protein_group> >::iterator F = PROFILE_PROTEIN_DATA.find( P_NB );

  if( F == PROFILE_PROTEIN_DATA.end() ){
    vector<protein_group> tmp;
    PROFILE_PROTEIN_DATA.insert( make_pair( P_NB, tmp));
    F = PROFILE_PROTEIN_DATA.find( P_NB );
  }
  
  map<int, vector< protein_group> >::iterator P = IN->begin();
  while( P != IN->end() ){

    // and store in the member list:
    vector< protein_group>::iterator X = (*P).second.begin();
    while( X != (*P).second.end() ){
      protein_group* PDC = &(*X);
      string AC = PDC->get_AC();
      
      (*F).second.push_back( *PDC );
      
      map<string, protein_group>::iterator F = ALL_PROTEINS.find( AC );      
      if( F != ALL_PROTEINS.end() ){
        (*F).second.merge_protein_group( PDC );
      }
      else{
        // not found, make a new one:
        ALL_PROTEINS.insert( make_pair( AC , *PDC ) );
      }
      
      PDC = NULL;
      
      X++;
    }
  
    P++;
  }
  
  IN = NULL;
}



////////////////////////////////////////////////////////////////
// update created protein groups with information
// from MASTER run
void profile_comparer::update_protein_groups_by_MASTER( map<int, LC_MS>* MASTER_LIST, profile_group* IN){
  
  // get the right MASTER run:
  LC_MS* MASTER = &( ( *MASTER_LIST->find( IN->get_MASTER_ID()) ).second);
  vector<protein_group>::iterator PROT = IN->get_PROTEIN_GROUPS_START();
  while( PROT != IN->get_PROTEIN_GROUPS_END() ){
  
    string AC = (*PROT).get_AC();
    
    // search in the MASTER run for features which belong to this protein:
    vector< LC_MS_FEATURE >::iterator FEA = MASTER->get_feature_list_begin();
    while( FEA != MASTER->get_feature_list_end() ){
      feature* THIS_F = &(*FEA);
      
      // check if enough profile points:
      // and MS2 info available
      if( ( THIS_F->get_replicate_match_nb() >= NB_PROFILE_POINTS ) && (THIS_F->get_MS2_info()) ){        
        // now check if its the right AC and if peptitde probability good enough:
        ms2_info* INFO = THIS_F->get_best_MS2_SCAN();
        if( INFO->get_AC() == AC ){
          (*PROT).update_protein_by_feature( THIS_F );
        }
      }
      
      THIS_F = NULL;
      FEA++;
    }
    
    PROT++;
  }
  
  MASTER_LIST = NULL;
  IN = NULL;
}


//////////////////////////////////////////////////
// score all elements in a profile group versus the
// target profile, then make statistics and assign p-value: 
void profile_comparer::statistical_cluster_analysis( profile_group* IN ){
  
  //*******************************************
  // define here against which profile to score:
  // 
  map<int, double> MY_PROFILE;
  
  if( TRUE_TARGET_PROFILE ){
    //
    // a.TARGET_PROFILE: orginal input target profile
    MY_PROFILE = IN->get_TARGET_PROFILE();
  }
  else{
    // b. CONSENS_PROFILE: profile of the selected 
    // best scoring cluster from k-means analysis
    MY_PROFILE = IN->get_CLUSTER_PROFILE();
  }
  //*******************************************
  
  // set the nb of profile points in protein 
  // and peptide group:
  peptide_DELTA_group::NB_PROFILE_DATA_POINTS = MY_PROFILE.size();
  profile_scorer::MAX_NB_PROILE_DATA_POINTS = MY_PROFILE.size();
  protein_group::NB_PROFILE_DATA_POINTS = MY_PROFILE.size();
  peptide_DELTA_group::NB_PROFILE_DATA_POINTS = MY_PROFILE.size();
  consens_profile_builder::MAX_NB_PROILE_DATA_POINTS = MY_PROFILE.size();
  
  
  ///////////////////////////////////////////////////////
  // TARGETET PROFILING:
  // target profile on peptide level 
  target_profiling_PEPTIDE_LEVEL( IN , MY_PROFILE );
  // target profile on protein level 
  target_profiling_PROTEIN_LEVEL( IN , MY_PROFILE );
  
  ///////////////////////////////////////////////////////
  // store the best candidates by this functions:
  store_BEST_HITS( IN );
  
  IN = NULL;
  
  
}

//////////////////////////////////////////////////
// score all peptide groups versus the input
// target profile, then make statistics and assign p-value: 
void profile_comparer::target_profiling_PEPTIDE_LEVEL( profile_group* IN , map<int, double> TARGET_PROFILE){

  
  map<double, double> ALL_PEPTIDE_SCORES;
  // scorer against target profile:  
  profile_scorer* PRO_S = new profile_scorer( TARGET_PROFILE ); 


  bool peptidePlot = false;
   
  char MY_buffer[255];
  data_plotter* PLOT = NULL;
  if( peptidePlot && PRINT_OUT ){
    sprintf( MY_buffer, "all_peptide_groups_%d", IN->get_CLUSTER_ID());
    PLOT = new data_plotter(MY_buffer, false);  
  }
  
  // extarct peptides from teh proteins:
  vector<protein_group>::iterator PROT = IN->get_PROTEIN_GROUPS_START();
  while( PROT != IN->get_PROTEIN_GROUPS_END() ){
    
    protein_group* PROT_G = &(*PROT);
    
    // non proteotypic:
    vector<peptide_DELTA_group>::iterator PROT_PEP = PROT_G->get_peptide_members_list_start();
    while( PROT_PEP != PROT_G->get_peptide_members_list_end() ){
    
      peptide_DELTA_group* PEP_G = &(*PROT_PEP);    
      
      PEP_G->build_peptide_average_consens();      
      map<int, double>* CONSENS = PEP_G->get_PROFILE_AVERAGE_CONSENS();
      double score = PRO_S->COMPARE_TO_TARGET_PROFILE( CONSENS );
      PEP_G->set_PROFILE_SCORE( score );      
      ALL_PEPTIDE_SCORES.insert( make_pair( score, 0.0 ) );

      if( peptidePlot ){
        PLOT->add_plot_data( CONSENS );
      }
      PROT_PEP++; 
    }
    
    // proteotypic:
    PROT_PEP = PROT_G->get_proteotypic_peptide_members_list_start();
    while( PROT_PEP != PROT_G->get_proteotypic_peptide_members_list_end() ){
      peptide_DELTA_group* PEP_G = &(*PROT_PEP);    
      
      // build average profile:
      PEP_G->build_peptide_average_consens();      
      map<int, double>* CONSENS = PEP_G->get_PROFILE_AVERAGE_CONSENS();
      double score = PRO_S->COMPARE_TO_TARGET_PROFILE( CONSENS );
      PEP_G->set_PROFILE_SCORE( score );      
      ALL_PEPTIDE_SCORES.insert( make_pair( score, 0.0 ) );
      
      if( peptidePlot && PRINT_OUT ){
        PLOT->add_plot_data( CONSENS );
      }
      
      PROT_PEP++; 
    }
      
    PROT++;
  }
  
  
  // go through the unassigned peptides:
  vector<peptide_DELTA_group>::iterator PEP = IN->get_PEPTIDE_GROUPS_START();
  while( PEP != IN->get_PEPTIDE_GROUPS_END() ){
    
    peptide_DELTA_group* PEP_G = &(*PEP);    
    PEP_G->build_peptide_average_consens();    
    map<int, double>* CONSENS = PEP_G->get_PROFILE_AVERAGE_CONSENS();
    double score = PRO_S->COMPARE_TO_TARGET_PROFILE( CONSENS );
    PEP_G->set_PROFILE_SCORE( score );
    ALL_PEPTIDE_SCORES.insert( make_pair( score, 0.0 ) );
    
    if( peptidePlot && PRINT_OUT ){
      PLOT->add_plot_data( CONSENS );
    }
    
    PEP++;
  }

  if( peptidePlot && PRINT_OUT ){
    PLOT->plot_TWOD_data();
    PLOT->print_MATRIX_data_to_TXT();
    delete PLOT;
    PLOT = NULL;
  }
  
  ////////////////////////////////////
  // EM analysis:
  // on PEPTIDE level
  char buffer[254];
  printf("\t\t- Peptide Group EM Analysis (%d profiles)\n", ALL_PEPTIDE_SCORES.size());
  sprintf(buffer, "peptide_profiles_%d", IN->get_TARGET_PROFILE_ID());
  EM_fitter* ANA = new EM_fitter(buffer);
  ANA->set_TEXT_OUT( true );
  ANA->set_DATA( &ALL_PEPTIDE_SCORES );
  ANA->start_EM_2COMP_analysis();
  ANA->show_EM_summary();

  /////////////////////////////////
  // set the probabilities:
  // go through the unassigned peptides:
  PEP = IN->get_PEPTIDE_GROUPS_START();
  while( PEP != IN->get_PEPTIDE_GROUPS_END() ){
    peptide_DELTA_group* PEP_G = &(*PEP);
    PEP_G->set_PROFILE_PROBABILITY( ANA->get_profile_probabiliy_per_score( PEP_G->get_PROFILE_SCORE() ) );
    PEP++;
  }
  // go through the peptides in the proteins:
  PROT = IN->get_PROTEIN_GROUPS_START();
  while( PROT != IN->get_PROTEIN_GROUPS_END() ){    
    
    // non proteotypic:
    vector<peptide_DELTA_group>::iterator PROT_PEP = (*PROT).get_peptide_members_list_start();
    while( PROT_PEP != (*PROT).get_peptide_members_list_end() ){
      peptide_DELTA_group* PEP_G = &(*PROT_PEP);
      PEP_G->set_PROFILE_PROBABILITY( ANA->get_profile_probabiliy_per_score( PEP_G->get_PROFILE_SCORE() ) );
      PROT_PEP++;
    }
    // proteotypic:
    PROT_PEP = (*PROT).get_proteotypic_peptide_members_list_start();
    while( PROT_PEP != (*PROT).get_proteotypic_peptide_members_list_end() ){
      peptide_DELTA_group* PEP_G = &(*PROT_PEP);
      PEP_G->set_PROFILE_PROBABILITY( ANA->get_profile_probabiliy_per_score( PEP_G->get_PROFILE_SCORE() ) );
      PROT_PEP++;
    }
    
    PROT++;
  }
  
  ALL_PEPTIDE_SCORES.clear();
  delete ANA;
  ANA = NULL;
  delete PRO_S;
  PRO_S = NULL;
  IN = NULL;
}

//////////////////////////////////////////////////
// score the protein groups versus the input
// target profile, then make statistics and assign p-value: 
void profile_comparer::target_profiling_PROTEIN_LEVEL( profile_group* IN , map<int, double> TARGET_PROFILE){
  
  
  map<double, double> ALL_PROTEIN_SCORES;
  bool PROTEIN_CONVERGENCE = false;
  // score against target profile:  
  profile_scorer* PRO_S = new profile_scorer( TARGET_PROFILE ); 
  
  // go through proteins:
  vector<protein_group>::iterator PROT = IN->get_PROTEIN_GROUPS_START();
  while( PROT != IN->get_PROTEIN_GROUPS_END() ){
    
    protein_group* PROT_G = &(*PROT);    
    // build average profile:
    PROT_G->build_protein_average_consens();
    
    // score now the median profile:
    double score = PRO_S->COMPARE_TO_TARGET_PROFILE( PROT_G->get_PROFILE_AVERAGE_CONSENS() );
    PROT_G->set_PROFILE_SCORE( score );
    ALL_PROTEIN_SCORES.insert( make_pair( score, 0.0 ) );    
    PROT++;
  }
  
  ////////////////////////////////////
  // EM analysis:
  // on protein level
  char buffer[255];
  EM_fitter* ANA = NULL;
  
  printf("\t\t- Protein Group EM Analysis (%d profiles)\n", ALL_PROTEIN_SCORES.size());
  sprintf(buffer, "protein_profiles_%d", IN->get_TARGET_PROFILE_ID());
  ANA = new EM_fitter(buffer);
  ANA->set_DATA( &ALL_PROTEIN_SCORES );
  ANA->start_EM_2COMP_analysis();
  PROTEIN_CONVERGENCE = ANA->get_convergence_check();
  ANA->show_EM_summary();
  
  ////////////////////////////////
  // go through proteins:
  protein_profile_evaluator EVAL; 
  PROT = IN->get_PROTEIN_GROUPS_START();
  while( PROT != IN->get_PROTEIN_GROUPS_END() ){    
    
    // detect outliers by profile score to protein consens profile:
    EVAL.START_PEPTIDE_OUTLIER_DETECTION( &(*PROT) );
    
    protein_group* PROT_G = &(*PROT);    
    // check if the EM worked, otherwise retrieve the 
    if(  ANA->get_convergence_check() ) {
      PROT_G->set_PROFILE_PROBABILITY( ANA->get_profile_probabiliy_per_score( PROT_G->get_PROFILE_SCORE() ) );      
    }
    // probability from the peptide groups:
    else{
      PROT_G->set_profile_probability_from_peptide_groups();
    }
    
    PROT++;
    }
  ALL_PROTEIN_SCORES.clear();
  delete ANA;
  ANA = NULL;
  delete PRO_S;
  PRO_S = NULL;
  IN = NULL;
  
}


//////////////////////////////////////////////////
// print peptide of a profile_cluster
void profile_comparer::print_PEPTIDE_profile_group( profile_group* IN, int NB){
  
  if( PRINT_OUT ){      
    
    char buffer[255];
    sprintf( buffer, "cluster_view_profile_%d_M_ID_%d", NB, IN->get_MASTER_ID());
    data_plotter* PLOT = new data_plotter(buffer);
    PLOT->set_Y_RANGE(0.0, 0.7);

    // go through peptides now:
    vector<peptide_DELTA_group>::iterator PEP = IN->get_PEPTIDE_GROUPS_START();
    while( PEP != IN->get_PEPTIDE_GROUPS_END() ){
      peptide_DELTA_group* PEP_G = &(*PEP);
      // build average profile:
      PEP_G->build_peptide_average_consens();
      sprintf( buffer, "");
      ms2_info* INFO = PEP_G->get_BEST_MS2_SCAN();
      if( INFO ){
        sprintf( buffer, "%s,", INFO->get_AC().c_str() );        
      }
      sprintf( buffer, "%s%0.2f(#%d)", buffer, PEP_G->get_MR(), PEP_G->get_nb_members());
      PLOT->add_plot_data( PEP_G->get_PROFILE_AVERAGE_CONSENS(), buffer);
      PEP++;
    }
    
    PLOT->plot_TWOD_data();
    delete PLOT;
    PLOT = NULL;
  }
}

/////////////////////////////////////////////////////////////////////////
// plot the best matching clusters to the target profile:
void profile_comparer::plot_best_matching_clusters(){
  
  char outfile[255];
  sprintf( outfile, "BEST_matching_clusters");
  string tmp = outfile;
  data_plotter* PLOT = new data_plotter(tmp, false);
  
  // first the target profiles:
  map< int, vector<double> >::iterator TAR =  TARGET_CLUSTER_PROFILES.begin();
  while( TAR != TARGET_CLUSTER_PROFILES.end() ){
    
    // normalize profile:
    consens_profile_builder CONS;
    map<int, double> my_p = CONS.normalize_profile( (*TAR).second ) ;
    char buffer[255];
    sprintf( buffer, "target profile #%d", (*TAR).first);    
    PLOT->add_plot_data_dotlines( &my_p , buffer );
    TAR++;
  }
  
  // now the cluster profiles:
  vector<profile_group>::iterator P = ALL_PROFILE_GROUPS.begin();
  while(P != ALL_PROFILE_GROUPS.end()){
    
    map< int, map<int, double> >::iterator S = (*P).get_SELECTED_CLUSTER_PROFILE_START( );
    while( S != (*P).get_SELECTED_CLUSTER_PROFILE_END( ) ){
      char buffer[255];
      sprintf( buffer, "%d. cluster", (*S).first);  
      map< int, double > tmp = (*S).second;
      PLOT->add_plot_data( &tmp, buffer );
      S++;
    }
      P++;
  }
  
  PLOT->define_MAX_Y();
  PLOT->set_POINT_SIZE( 2 );
  PLOT->print_TWOD_data_to_TXT();
  PLOT->plot_TWOD_data();
  delete PLOT;
  PLOT = NULL;
  
}


//////////////////////////////////////////////////
// print proteins of a profile_cluster
void profile_comparer::print_PROTEIN_profile_group( profile_group* IN, int NB){
  
  if( PRINT_OUT ){      
    
    char buffer[255];
    sprintf( buffer, "cluster_view_profile_%d_M_ID_%d", NB, IN->get_MASTER_ID());
    data_plotter* PLOT = new data_plotter(buffer);
    PLOT->set_Y_RANGE(0.0, 0.7);
        
    // go through proteins first:
    vector<protein_group>::iterator PROT = IN->get_PROTEIN_GROUPS_START();
    while( PROT != IN->get_PROTEIN_GROUPS_END() ){
      protein_group* PROT_G = &(*PROT);
      // build average profile:
      PROT_G->build_protein_average_consens();      
      // score now the median profile:
      sprintf( buffer, "%s", PROT_G->get_AC().c_str() );
      PLOT->add_plot_data(  PROT_G->get_PROFILE_AVERAGE_CONSENS(), buffer );
      PROT++;
    }
    
    // go through peptides now:
    vector<peptide_DELTA_group>::iterator PEP = IN->get_PEPTIDE_GROUPS_START();
    while( PEP != IN->get_PEPTIDE_GROUPS_END() ){
      peptide_DELTA_group* PEP_G = &(*PEP);
      // build average profile:
      PEP_G->build_peptide_average_consens();      
      // PLOT->add_plot_data( PEP_G->get_PROFILE_AVERAGE_CONSENS());
      PEP++;
    }
    
    PLOT->plot_TWOD_data();
    delete PLOT;
    PLOT = NULL;
  }
}

//////////////////////////////////////////////////
// write a profile_cluster to XML
void profile_comparer::print_profile_group_to_XML( profile_group* IN ){
  
  // define the name first:
  // get the input name from the input source directory:
  read_param* def = new read_param();
  // read input directory:
  string dirName = "ANALYSIS_" + def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;  
  
  
  if(dirName[ dirName.size() - 1 ] != '/'){
    dirName += "/";
  }
  
  char file_name[255];
  sprintf( file_name, "%sprofile_group_XML_%d.xml", dirName.c_str(), IN->get_TARGET_PROFILE_ID());
  
  
  //////////////////////////////////
  // print the header:
  LC_MS_XML_writer* LC_W = new LC_MS_XML_writer(file_name);
  LC_W->write_unformatted_XML_MAIN_HEADER();
  
  // print content of the profile group:
  IN->write_to_XML_file( LC_W ); 

  // put the close header:
  LC_W->write_XML_MAIN_HEADER_CLOSE();
  
  delete LC_W;
  LC_W = NULL;
  
}


//////////////////////////////////////////////////
// write a profile_cluster to APML
void profile_comparer::print_profile_group_to_APML( profile_group* IN ){
  
  char file_name[255];
  sprintf( file_name, "profile_group_APML_%d.apml", IN->get_TARGET_PROFILE_ID());
  
  //////////////////////////////////
  // print the header:
  LC_MS_APML_writer* LC_W = new LC_MS_APML_writer(file_name);
  LC_W->write_APML_MAIN_HEADER();
  
  // print content of the profile group:
  IN->write_to_APML_file( LC_W ); 
  
  delete LC_W;
  LC_W = NULL;
  
}


//////////////////////////////////////////////////
// write a profile_cluster to space delimited txt file
void profile_comparer::print_profile_group_to_TXT( profile_group* IN ){
  
  // read parameters from "param.def"
  read_param* def = new read_param();
  string DIR = def->search_tag("MY PROJECT NAME");
  if( DIR[ DIR.size() - 1] != '/'){
    DIR += "/";
  }
  string DEL = "\t";
  char buffer[254];
  char file_name[255];
  ofstream* WRITER;
  
  ////////////////////////////////////////////////////////////////////
  // print the extracted proteins out:
  sprintf( file_name, "ANALYSIS_%sPeptideCandidates_%d.txt", DIR.c_str(), IN->get_TARGET_PROFILE_ID());
  WRITER = new ofstream();
  WRITER->open(file_name, ofstream::out); 
  if( WRITER->good() ){
    
    // sort the proteins in the profile group by protein profile probability:
    IN->sort_proteins_by_profile_probability( );
    
    // print the header:
    sprintf(buffer,"m/z%sTr%sz%sProtein%sSequence%sPeptide.Prob%s#Points%sProfileProb.%sProfileScore\n", DEL.c_str(),DEL.c_str(), DEL.c_str(),  DEL.c_str(),DEL.c_str(), DEL.c_str(), DEL.c_str(), DEL.c_str());  
    // write to the file:
    WRITER->write(buffer,strlen(buffer));
    
    // go through all protein groups:
    vector<protein_group>::iterator PROT = IN->get_PROTEIN_GROUPS_START();
    while( PROT != IN->get_PROTEIN_GROUPS_END() ){
      
      // print from each protein the features:
      protein_group* prot = &(*PROT);
      vector<peptide_DELTA_group>::iterator PEP = prot->get_peptide_members_list_start();
      while( PEP != prot->get_peptide_members_list_end() ){
        PEP->write_Feature_content_2_TXT( WRITER );
        PEP++;
      }
      PEP = prot->get_proteotypic_peptide_members_list_start();
      while( PEP != prot->get_proteotypic_peptide_members_list_end() ){
        PEP->write_Feature_content_2_TXT( WRITER );
        PEP++;
      }
      
      PROT++;
    }
    
    printf( "\t\t- Peptides of Profile Group %d saved to '%s'...\n", IN->get_TARGET_PROFILE_ID(), file_name );
    
    
    delete WRITER;
    WRITER = NULL;
  }
  else{
    sprintf( "\t ERROR:: could not save profile group to file \'%s\'::profile_comparer, 408\n", file_name );
  }  
  
  
  ////////////////////////////////////////////////////////////////////
  // print the extracted proteins out:
  sprintf( file_name, "ANALYSIS_%sProteinCandidates_%d.txt", DIR.c_str(), IN->get_TARGET_PROFILE_ID());
  WRITER = new ofstream();
  WRITER->open(file_name, ofstream::out); 
  if( WRITER->good() ){
    
    // sort the proteins in the profile group by protein profile probability:
    IN->sort_proteins_by_profile_probability( );
    
    // print the header:
    sprintf(buffer,"IPI%sProfile.Prob%sProfileScore%s#MS1features%s#Proteo.Pep.\n", DEL.c_str(), DEL.c_str(), DEL.c_str(),DEL.c_str() );  
    // write to the file:
    WRITER->write(buffer,strlen(buffer));
    
    // print all protein groups:
    vector<protein_group>::iterator PROT = IN->get_PROTEIN_GROUPS_START();
    while( PROT != IN->get_PROTEIN_GROUPS_END() ){
      PROT->write_to_TXT_file( WRITER );
      PROT++;
    }
    printf( "\t\t- Proteins of Profile Group %d saved to '%s'...\n", IN->get_TARGET_PROFILE_ID(), file_name );
    
    
    delete WRITER;
    WRITER = NULL;
  }
  else{
    sprintf( "\t ERROR:: could not save profile group to file \'%s\'::profile_comparer, 408\n", file_name );
  }  
  
}


///////////////////////////////////////////////////////////////
// parser that extracts profile groups from a superhirn clusterin
// output
void profile_comparer::import_profiling_data_from_XML(){
  
  ALL_MS1_FEATURES.clear();
  ALL_PROFILE_GROUPS.clear();
  
  string file = "ANALYSIS_";
  read_param* def = new read_param();  
  file = file + def->search_tag("MY PROJECT NAME");
  if( file[ file.size() - 1 ] != '/' ){
    file += "/";
  }
  file += "KMEANS_cluster_groups.xml";
  
  TiXmlDocument* PARSER = new TiXmlDocument( file.c_str() );
  if( PARSER->LoadFile() ){
    
    printf("\tparsing XML-file '%s' ...\n", file.c_str() ); 
    // parse all profile groups:
    TiXmlNode* PROFILE_GROUPS = PARSER->FirstChild( "MASTER_RUN_SUMMARY" )->FirstChild( "CLUSTERING_OUTPUT" )->FirstChild( "PROFILE_GROUPS" ); 
    TiXmlNode* PROFILE_GROUP;
    for( PROFILE_GROUP = PROFILE_GROUPS->FirstChild("PROFILE_GROUP"); PROFILE_GROUP; PROFILE_GROUP = PROFILE_GROUP->NextSibling() ){
      if( PROFILE_GROUP != NULL ){
        // extract profile_group
        parse_profile_group( PROFILE_GROUP );
      }
    }
    
    // show overview:
    printf("\n\t\t\t-> %d profile groups imported.\n", ALL_PROFILE_GROUPS.size() );
    
  }
  else{    
    printf("\n\t**** Could not open cluster-XML-file '%s', profile_copmaraer::549 \n", file.c_str() );
  }
  
  if( PARSER != NULL ){
    delete PARSER;
    PARSER = NULL;
  }

}



/////////////////////////////////////////////////////////////////////
// parses a profile group:
void profile_comparer::parse_profile_group( TiXmlNode* PROFILE_GROUP ){

  LC_MS_XML_reader* READ = new LC_MS_XML_reader();  
  TiXmlElement* P_ELE = PROFILE_GROUP->ToElement();
  int P_ID = atoi( P_ELE->Attribute("ID") );
  vector< feature > FEATURES;
  
  // create a new profile group:
  profile_group* MY_PG = new profile_group( P_ID );
  MY_PG->set_TARGET_PROFILE_ID( P_ID );
  

  // extract first all the profiles
  // main cluster profile:
  TiXmlNode* MCP = PROFILE_GROUP->FirstChild( "MAIN_CLUSTER_PROFILE" ); 
  TiXmlElement* ELE = MCP->ToElement();
  int MCP_ID = atoi( ELE->Attribute("id") );
  map<int, double> MCP_P = READ->extract_profile( MCP );
  MY_PG->set_CLUSTER_PROFILE( MCP_P );
  MY_PG->set_CLUSTER_ID( MCP_ID);
  
  
  // target profile:
  TiXmlNode* TP = PROFILE_GROUP->FirstChild( "TARGET_PROFILE" ); 
  ELE = TP->ToElement();
  int TP_ID = atoi( ELE->Attribute("id") );
  map<int, double> TP_P = READ->extract_profile( TP );
  MY_PG->set_TARGET_PROFILE( TP_P );
  MY_PG->set_TARGET_PROFILE_ID( TP_ID);

  
  // selected cluster profiles
  TiXmlNode* CP_A = PROFILE_GROUP->FirstChild( "SELECTED_CLUSTER_PROFILES" ); 
  TiXmlNode* CP = NULL; 
  for( CP = CP_A->FirstChild( "CLUSTER_PROFILE" ); CP; CP = CP->NextSibling() ){
    ELE = CP->ToElement();
    int CP_ID = atoi( ELE->Attribute("id") );
    map<int, double> CP_P = READ->extract_profile( CP );
    MY_PG->add_SELECTED_CLUSTER_PROFILE( CP_ID, CP_P );
  }


  // extract all MS1 features:
  TiXmlNode* ALL_FEATURES = PROFILE_GROUP->FirstChild( "MS1_FEATURE_MEMBERS" ); 
  TiXmlNode* FEATURE = NULL;
  for( FEATURE = ALL_FEATURES->FirstChild("MS1_FEATURE"); FEATURE; FEATURE = FEATURE->NextSibling() ){
    if( FEATURE != NULL ){    
      feature* MY_FEA = NULL;
      MY_FEA = READ->extract_and_return_FEATURE( FEATURE, MY_FEA );
      if( MY_FEA != NULL ){
        FEATURES.push_back( *MY_FEA ); 
      }
    }
  }
  
  // add them to the all features holder:
  ALL_MS1_FEATURES.insert( make_pair( P_ID, FEATURES ) ); 
  // and add references to the profile group
  map<int, vector< feature> >::iterator A = ALL_MS1_FEATURES.find( P_ID );
  vector< feature>::iterator F = (*A).second.begin();
  while( F != (*A).second.end() ){
    MY_PG->add_cluster_member( &(*F) );
    F++;
  }
  
  
  // add the profile group now:
  ALL_PROFILE_GROUPS.push_back( *MY_PG );
  delete MY_PG;
  MY_PG = NULL;
  
}




///////////////////////////////////////////////////////
// store the best candidates by this functions:
void profile_comparer::store_BEST_HITS( profile_group* IN ){
  
  vector< protein_group > TMP;
  BEST_PROTEINS.insert( make_pair( IN->get_TARGET_PROFILE_ID(), TMP ) ); 
  map< int, vector<protein_group> >::iterator MY_PROT = BEST_PROTEINS.find( IN->get_TARGET_PROFILE_ID() );
  
  // filter out the best according to profile probability
  vector<protein_group>::iterator PROT = IN->get_PROTEIN_GROUPS_START();
  while( PROT != IN->get_PROTEIN_GROUPS_END() ){
    protein_group* PROT_G = &(*PROT);
    if( PROT_G->get_PROFILE_PROBABILITY() > PROFILE_PROBABILITY_THRESHOLD ){
      (*MY_PROT).second.push_back( *PROT_G ); 
    }  
    PROT++;
  }
}



/////////////////////////////////////////////////////////////////////
// process the best hits protein from the profiling
void profile_comparer::process_BEST_PROTEIN_HITS(){
  
  protein_profile_evaluator EVAL;
  EVAL.set_TARGET_PROFILES( TARGET_CLUSTER_PROFILES );

  map< int, vector<protein_group> >::iterator PROT = BEST_PROTEINS.begin();
  while( PROT != BEST_PROTEINS.end() ){
    char file[255];
    sprintf(file, "best_protein_profiles_%d", (*PROT).first );
    
    // plotting:
    if( PRINT_OUT ){
      // EVAL.plot_this_protein( &((*PROT).second), file );
      EVAL.plot_this_protein( &((*PROT).second), (*PROT).first, file );
    }
    
    // print their profile:
    // EVAL.print_this_protein_profile( &((*PROT).second) , file);
    
    PROT++;
  }
  
  // plotting:
  if( PRINT_OUT ){
    plot_BEST_PROTEIN_HITS();
  }
  
}



///////////////////////////////////////////////////////
// plot the best candidates by this functions:
void profile_comparer::plot_BEST_PROTEIN_HITS( ){
  
  
  char buffer[255];
  sprintf( buffer, "best_protein_profiles");
  data_plotter* PLOT = new data_plotter(buffer, false);  
  
  map< int, vector<protein_group> >::iterator PROT = BEST_PROTEINS.begin();
  while( PROT != BEST_PROTEINS.end() ){
    
    // first the target profiles:
    map< int, vector<double> >::iterator TAR =  TARGET_CLUSTER_PROFILES.find( (*PROT).first );
    if( TAR != TARGET_CLUSTER_PROFILES.end() ){
      // normalize profile:
      consens_profile_builder CONS;
      map<int, double> my_p = CONS.normalize_profile( (*TAR).second ) ;
      char buffer[255];
      sprintf( buffer, "target profile #%d", (*TAR).first);    
      PLOT->add_plot_data_dotlines( &my_p , buffer );
    }
    
    vector<protein_group>::iterator PR = (*PROT).second.begin();
    while( PR != (*PROT).second.end() ){
      
      protein_group* THIS_PROTEIN = &(*PR);
      map<int, double>* PROF = THIS_PROTEIN->get_PROFILE_AVERAGE_CONSENS();
      sprintf( buffer, "#%d: %s[MS1-Fea=%d, Pep-IDs=%d]", (*PROT).first, THIS_PROTEIN->get_AC().c_str(), THIS_PROTEIN->get_nb_features(), THIS_PROTEIN->get_nb_assigned_features());
      PLOT->add_plot_data( PROF, buffer);
      
      // get the standard deviation:
      map<int, double>* PROF_STDEV = THIS_PROTEIN->get_PROFILE_AVERAGE_CONSENS_STDEV();
      vector< double> TMP;
      map<int, double>::iterator P = PROF_STDEV->begin();
      while( P != PROF_STDEV->end() ){
        TMP.push_back( (*P).second );
        P++;
      }
      PLOT->add_plot_data( PROF, &TMP );
      PR++;
    }
    PROT++;
  }
  
  PLOT->define_MAX_Y();
  PLOT->plot_TWOD_data();  
  delete PLOT;
  PLOT = NULL;
  
}

///////////////////////////////////////////////////////
// plot peptides of the best candidates by this functions:
void profile_comparer::plot_peptides_of_BEST_PROTEIN_HITS( ){
  
  
  char buffer[255];
  sprintf( buffer, "best_protein_peptide_profiles");
  data_plotter* PLOT = new data_plotter(buffer, false);  
  
  
  map< int, vector<protein_group> >::iterator PROT = BEST_PROTEINS.begin();
  while( PROT != BEST_PROTEINS.end() ){
    
    vector<protein_group>::iterator THIS_PROT = (*PROT).second.begin();
    while( THIS_PROT != (*PROT).second.end() ){
      protein_group* MY_PROT = &(*THIS_PROT);
    
      vector<peptide_DELTA_group>::iterator PROT_PEP = MY_PROT->get_peptide_members_list_start();
      while( PROT_PEP != MY_PROT->get_peptide_members_list_end() ){
        
        peptide_DELTA_group* PEP_G = &(*PROT_PEP);    
        map<int, double>* CONSENS = PEP_G->get_PROFILE_AVERAGE_CONSENS();
        PLOT->add_plot_data( CONSENS );
        PROT_PEP++; 
      }
      // proteotypic:
      PROT_PEP = MY_PROT->get_proteotypic_peptide_members_list_start();
      while( PROT_PEP != MY_PROT->get_proteotypic_peptide_members_list_end() ){
        peptide_DELTA_group* PEP_G = &(*PROT_PEP);    
        map<int, double>* CONSENS = PEP_G->get_PROFILE_AVERAGE_CONSENS();
        PLOT->add_plot_data( CONSENS );
        PROT_PEP++; 
      }
      //////////////////////////////////
        
      THIS_PROT++;
    }
    PROT++;
  }
  
  PLOT->plot_TWOD_data();  
  delete PLOT;
  PLOT = NULL;
  
}




/////////////////////////////////////////////////////////////
// make a statistics for which input profiles the cluster
// match best:
void profile_comparer::EXTRACT_BEST_TARGET_PROFILE_CLUSTERS(){
  
  vector< profile_group > NEW_PROFILE_GROUPS;
  
  // check how many best ranking clusters to take:
  double NB_BEST_RANKING = 0;  
  read_param* def = new read_param();
  // print out
  def->search_tag("target profile score precentile", &NB_BEST_RANKING);
  delete def;
  def = NULL;
  
  //////////////////////
  // associate the clusters to the target profiles: 
  map< int, vector<double> >::iterator P = TARGET_CLUSTER_PROFILES.begin();
  while( P != TARGET_CLUSTER_PROFILES.end() ){
    
    // cluster vector:
    map<double, profile_group*> RANKED_CLUSTERS;
    vector<double> MY_TARGET_PROFILE = (*P).second;
    
    /////////////////////////////////////////////
    // score all clusters vs the target profile:
    profile_scorer* PRO_S = new profile_scorer( MY_TARGET_PROFILE ); 
    vector<profile_group>::iterator C = ALL_PROFILE_GROUPS.begin();
    while( C != ALL_PROFILE_GROUPS.end() ){
      
      // compute similarity to this target profile: 
      map<int, double> THIS_PROFILE = (*C).get_CLUSTER_PROFILE();
      double this_score = PRO_S->COMPARE_TO_TARGET_PROFILE( &THIS_PROFILE);      
      THIS_PROFILE.clear();
      // store the score of this cluster:
      (*C).set_CLUSTER_SCORE( this_score );      
      // insert
      RANKED_CLUSTERS.insert(pair<double, profile_group*>(this_score, &(*C)));          
      C++;
    }
    delete PRO_S;
    PRO_S = NULL;
    
    
    /////////////////////////////////////////////
    // now select the best profile group:
    // select those which are above the cutoff
    map<double, profile_group*>::iterator RANKED_PROFILE = RANKED_CLUSTERS.begin();
    // store the first best cluster:
    profile_group* NEW_PROFILE_GROUP = new profile_group( (*RANKED_PROFILE).second );
    // and the original cluster profile:
    NEW_PROFILE_GROUP->add_SELECTED_CLUSTER_PROFILE( NEW_PROFILE_GROUP->get_CLUSTER_ID(), (*RANKED_PROFILE).second->get_CLUSTER_PROFILE());    
    // add the target profile and id to the profile group:
    NEW_PROFILE_GROUP->set_TARGET_PROFILE_ID( (*P).first );
    NEW_PROFILE_GROUP->set_TARGET_PROFILE( MY_TARGET_PROFILE );
    
    
    ////////////////////////////////////////////
    // save the next n-1 best ranking clusters
    // ( where n = NB_BEST_RANKING )
    RANKED_PROFILE++;
    double bad_rank_count = 1.0;
    while( RANKED_PROFILE != RANKED_CLUSTERS.end() ){
      
      if( bad_rank_count >= NB_BEST_RANKING ){
        break; 
      }
      NEW_PROFILE_GROUP->merge_profile_group( (*RANKED_PROFILE).second );
      bad_rank_count++;
      RANKED_PROFILE++;
    }
    
    //////////////////////
    // insert:
    NEW_PROFILE_GROUPS.push_back( *NEW_PROFILE_GROUP );
    RANKED_CLUSTERS.clear();
    // do for next target profile:
    P++;
    
  }
  
  if( !TARGET_CLUSTER_PROFILES.empty() ){
    
    ALL_PROFILE_GROUPS.clear();
    ALL_PROFILE_GROUPS = NEW_PROFILE_GROUPS;
    NEW_PROFILE_GROUPS.clear();
        
  }
  ////////////////////////
  // in case there are no target profiles, just use every create cluster single 
  else if( TARGET_CLUSTER_PROFILES.empty() ){
    
    printf( "\t\t-> no target profiles found, process every cluster.\n" );
    
    // numerate the profile groups:
    vector< profile_group>::iterator PRO = get_PROFILE_GROUPS_START();
    while( PRO != get_PROFILE_GROUPS_END() ){
      
      profile_group* MY_P = &(*PRO);
      MY_P->set_TARGET_PROFILE_ID( MY_P->get_CLUSTER_ID() );
      MY_P->set_TARGET_PROFILE( MY_P->get_CLUSTER_PROFILE() );
      
      PRO++;
    }
    
  }
  
  if( PRINT_OUT ){
    plot_best_matching_clusters();
  }
}




////////////////////////////////////////////////////////////////
// extracts from the mastermap a profile group
// filters for features with min profile lengths
void profile_comparer::extract_PROFILE_GROUP_from_MASTERMAP( LC_MS* IN){
  
  // create a profile group
  // make an object and set the consensu profile:
  profile_group* MY_group = new profile_group( 0 );
  MY_group->set_PROFILE_GROUP_ID( 0 );
  MY_group->set_MASTER_ID( IN->get_MASTER_ID() );
  // tmp->set_CLUSTER_PROFILE( (*C_M).second );
  
  vector<LC_MS_FEATURE>::iterator START = IN->get_feature_list_begin();
  while( START != IN->get_feature_list_end() ){
    feature* F = &(*START);
    if( F->get_replicate_match_nb() >= NB_PROFILE_POINTS ){
      MY_group->add_cluster_member( F );
    }
    F = NULL;
    START++;
  }
  // set the target profile ( can only have one)!!!
  MY_group->set_TARGET_PROFILE_ID( 0 );
  MY_group->set_TARGET_PROFILE( (*TARGET_CLUSTER_PROFILES.begin()).second );
  ALL_PROFILE_GROUPS.push_back( MY_group ) ;
}  

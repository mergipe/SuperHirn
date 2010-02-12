///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS protein_group:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_PROTEIN_GROUP
#include "STATISTIC_TOOLS_USE.h"


int protein_group::NB_PROFILE_DATA_POINTS;
double protein_group::TR_TOL;
bool protein_group::PEPTIDE_PROTEOTYPE_MODE;
map<int, vector<double> > protein_group::LC_MS_Modification_Masses;

////////////////////////////////////////////////
// constructor for the object protein_group:
protein_group::protein_group(){
  MR = 0;
  PROFILE_SCORE = 0;
  CLUSTER_INTERNAL_SCORE = 0;
  PROFILE_PROBABILITY = 0;
  MASTER_ID = -1;
  PROTEIN_GROUP_ID = -1;

}

////////////////////////////////////////////////
// constructor for the object protein_group:
protein_group::protein_group(peptide_DELTA_group* IN){

  MASTER_ID = -1;
  MR = 0;
  // get the best AC:
  AC = IN->get_BEST_MS2_SCAN()->get_AC();
  // add this cluster:
  merge_peptide_DELTA_group(IN);
  IN = NULL;
  PROFILE_SCORE = 0;
  CLUSTER_INTERNAL_SCORE = 0;
  PROFILE_PROBABILITY = 0;
  PROTEIN_GROUP_ID = -1;

}

////////////////////////////////////////////////
// constructor for the object protein_group:
protein_group::protein_group(string IN){
  
  MASTER_ID = -1;
  MR = 0;
  AC = IN;
  PROFILE_SCORE = 0;
  CLUSTER_INTERNAL_SCORE = 0;
  PROFILE_PROBABILITY = 0;
  PROTEIN_GROUP_ID = -1;
  
}

//////////////////////////////////////////////////
// class desctructor of protein_group
protein_group::~protein_group(){
  PEPTIDE_MEMBERS.clear();
  PROTEO_TYPIC_PEPTIDE_MEMBERS.clear();
  OUTLIER_PEPTIDE_MEMBERS.clear();
  PEPTIDE_LIST.clear();
  PROFILE_AVERAGE_CONSENS.clear();
  PROFILE_AVERAGE_CONSENS_STD.clear();
  ALL_FEATURES.clear();
}

//////////////////////////////////////////////////
// class copy constructor of protein_group
protein_group::protein_group(const protein_group& tmp){
  AC = tmp.AC;
  MR = tmp.MR;
  ALL_FEATURES = tmp.ALL_FEATURES;
  PROFILE_PROBABILITY = tmp.PROFILE_PROBABILITY;
  CLUSTER_INTERNAL_SCORE = tmp.CLUSTER_INTERNAL_SCORE;
  PROFILE_SCORE = tmp.PROFILE_SCORE;
  PEPTIDE_LIST = tmp.PEPTIDE_LIST;
  PEPTIDE_MEMBERS = tmp.PEPTIDE_MEMBERS;
  PROTEO_TYPIC_PEPTIDE_MEMBERS = tmp.PROTEO_TYPIC_PEPTIDE_MEMBERS;
  OUTLIER_PEPTIDE_MEMBERS = tmp.OUTLIER_PEPTIDE_MEMBERS;
  MASTER_ID = tmp.MASTER_ID;
  PROTEIN_GROUP_ID = tmp.PROTEIN_GROUP_ID;
  PROFILE_AVERAGE_CONSENS = tmp.PROFILE_AVERAGE_CONSENS;
  PROFILE_AVERAGE_CONSENS_STD = tmp.PROFILE_AVERAGE_CONSENS_STD;
}

//////////////////////////////////////////////////
// class copy constructor of protein_group
protein_group::protein_group(const protein_group* tmp){
  AC = tmp->AC;
  MR = tmp->MR;
  ALL_FEATURES = tmp->ALL_FEATURES;
  PROFILE_PROBABILITY = tmp->PROFILE_PROBABILITY;
  CLUSTER_INTERNAL_SCORE = tmp->CLUSTER_INTERNAL_SCORE;
  PROFILE_SCORE = tmp->PROFILE_SCORE;
  PEPTIDE_LIST = tmp->PEPTIDE_LIST;
  PEPTIDE_MEMBERS = tmp->PEPTIDE_MEMBERS;
  PROTEO_TYPIC_PEPTIDE_MEMBERS = tmp->PROTEO_TYPIC_PEPTIDE_MEMBERS;
  OUTLIER_PEPTIDE_MEMBERS = tmp->OUTLIER_PEPTIDE_MEMBERS;
  MASTER_ID = tmp->MASTER_ID;
  PROTEIN_GROUP_ID = tmp->PROTEIN_GROUP_ID;
  PROFILE_AVERAGE_CONSENS = tmp->PROFILE_AVERAGE_CONSENS;
  PROFILE_AVERAGE_CONSENS_STD = tmp->PROFILE_AVERAGE_CONSENS_STD;
}

//////////////////////////////////////////////////
// copy constructor:
protein_group& protein_group::operator=(const protein_group& tmp){
  AC = tmp.AC;
  MR = tmp.MR;
  ALL_FEATURES = tmp.ALL_FEATURES;
  PROFILE_PROBABILITY = tmp.PROFILE_PROBABILITY;
  CLUSTER_INTERNAL_SCORE = tmp.CLUSTER_INTERNAL_SCORE;
  PROFILE_SCORE = tmp.PROFILE_SCORE;
  PEPTIDE_MEMBERS = tmp.PEPTIDE_MEMBERS;
  PROTEO_TYPIC_PEPTIDE_MEMBERS = tmp.PROTEO_TYPIC_PEPTIDE_MEMBERS;
  OUTLIER_PEPTIDE_MEMBERS = tmp.OUTLIER_PEPTIDE_MEMBERS;
  PEPTIDE_LIST = tmp.PEPTIDE_LIST;
  MASTER_ID = tmp.MASTER_ID;
  PROTEIN_GROUP_ID = tmp.PROTEIN_GROUP_ID;
  PROFILE_AVERAGE_CONSENS = tmp.PROFILE_AVERAGE_CONSENS;
  PROFILE_AVERAGE_CONSENS_STD = tmp.PROFILE_AVERAGE_CONSENS_STD;
  return *this;
}
    
//////////////////////////////////////////////////
// operators:
bool protein_group::operator<(const protein_group& tmp){
  
  if( tmp.MR > get_MR()){
    return true;
  }
  else{
    return false; 
  }
}

//////////////////////////////////////////////////
// check if this feature belongs to the group:
bool protein_group::check_belonging(feature* IN){
  
  // find a peptide group for the feature:
  vector<peptide_DELTA_group>::iterator X = PEPTIDE_MEMBERS.begin(); 
  while(X != PEPTIDE_MEMBERS.end()){
    if( (*X).check_belonging( IN ) ){
      return true;
    }
    X++; 
  }
  
  // find a proteo typic peptide group for the feature:
  X = PROTEO_TYPIC_PEPTIDE_MEMBERS.begin(); 
  while(X != PROTEO_TYPIC_PEPTIDE_MEMBERS.end()){
    if( (*X).check_belonging( IN ) ){
      return true;
    }
    X++;
  }

  return false;
}

//////////////////////////////////////////////////
// check if this feature belongs to the group:
bool protein_group::check_belonging(peptide_DELTA_group* IN){

  if( IN->Protein_CHECK( AC ) ){
    return true;
  }
  else{
    return false;
  }
}

///////////////////////////////////////////////////////////////////////
// display function:
void protein_group::show_members(){
  
  printf("\n-----------------------------\n");
  printf("\tPROTEIN: %s [ Mr = %0.2f, Unique peptides= %d, proteotypic=%d ]\n", AC.c_str(), MR, PEPTIDE_LIST.size(), get_PROTEOTYPE() );
  
  // show the unique peptides
  map<string, vector< MS2_HIT> >::iterator P = PEPTIDE_LIST.begin();
  while( P != PEPTIDE_LIST.end()){
    
    printf("\t\t- %s [ ", (*P).first.c_str());    
    vector< MS2_HIT>::iterator M = (*P).second.begin();
    while( M != (*P).second.end() ){
      printf("%0.2f,", (*M).first);    
      M++;
    }
    printf("]\n");    
    P++;
  }    
}

///////////////////////////////////////////////////////////////////////
// display function:
void protein_group::show_protein_cluster_simple_view(){
  printf("\tPROTEIN: %s (%f) [ Mr = %0.2f, Unique peptides= %d, MS1 members = %d ]\n", AC.c_str(), PROFILE_SCORE, MR, PEPTIDE_LIST.size(), get_nb_features() );
}


//////////////////////////////////////////////////
// add features of a group into this one::
void protein_group::merge_peptide_DELTA_group(peptide_DELTA_group* IN){
  
  // add the peptide delta group:
  if( IN->get_PROTEO_TYPE() ){
    PROTEO_TYPIC_PEPTIDE_MEMBERS.push_back( *IN );
  }
  else{
    PEPTIDE_MEMBERS.push_back( *IN );
  }
  
  vector<feature*>::iterator P = IN->get_members_list_start();
  while( P != IN->get_members_list_end() ){
    add_MS2_feature_INFO( (*P) );
    P++;
  }
  
  // add the modified features:
  map<double, vector<feature*> >::iterator M = IN->get_modified_members_list_start();
  while( M != IN->get_modified_members_list_end() ){
    
    P = (*M).second.begin();
    while( P != (*M).second.end() ){
      add_MS2_feature_INFO( (*P) );
      P++;
    }
  
    M++;
  }
  
  IN = NULL;
}

//////////////////////////////////////////////////
// add features of a group into this one::
void protein_group::merge_protein_group(protein_group* IN){

  // add the peptide delta group:
  vector<peptide_DELTA_group>::iterator P = IN->get_peptide_members_list_start();
  while( P != IN->get_peptide_members_list_end() ){
    merge_peptide_DELTA_group( &((*P)) );
    P++;
  }
  
}



///////////////////////////////////////////////////////////////////////
// add feature:
void protein_group::add_MS2_feature_INFO(feature* IN){
    
  // check MS2 info:
  if( IN->get_MS2_info()){

    ms2_info* INFO = IN->get_best_MS2_SCAN();
    
    // see if peptide already seen:
    map<string, vector<MS2_HIT> >::iterator H =  PEPTIDE_LIST.find( INFO->get_SQ() );
    if( H ==  PEPTIDE_LIST.end() ){
      vector< MS2_HIT > tmp;
      tmp.push_back( make_pair(INFO->get_PEP_PROB(), INFO->get_PROTEO_TYPE()) ); 
      PEPTIDE_LIST.insert( make_pair( INFO->get_SQ(), tmp ) );
      
      // add up the molecular weight of the protein:
      MR += INFO->get_MONO_MZ();
      
    }
    else{
      (*H).second.push_back( make_pair(INFO->get_PEP_PROB(), INFO->get_PROTEO_TYPE()) ); 
    }
  }
  ALL_FEATURES.push_back( IN );
  IN = NULL;  
}


///////////////////////////////////////////////////////////////////////
// update protein group by a feature:
void protein_group::update_protein_by_feature(feature* IN){
 
  bool check = false;
  
  // find a peptide group for the feature:
  vector<peptide_DELTA_group>::iterator X = PEPTIDE_MEMBERS.begin(); 
  while(X != PEPTIDE_MEMBERS.end()){
    
    if( (*X).check_belonging( IN ) ){
      (*X).add_feature( IN );
      check = true;
      break;
    }
    
    X++; 
  }
  
  // find a proteo typic peptide group for the feature:
  X = PROTEO_TYPIC_PEPTIDE_MEMBERS.begin(); 
  while(X != PROTEO_TYPIC_PEPTIDE_MEMBERS.end()){
    
    if( (*X).check_belonging( IN ) ){
      (*X).add_feature( IN );
      check = true;
      break;
    }
    
    X++; 
  }
  
  // in case that peptide was not matched into a 
  // peptide group, make a new one:
  if( !check ){
    peptide_DELTA_group* NEW = new peptide_DELTA_group( IN );
    merge_peptide_DELTA_group( NEW );
    NEW = NULL;
  }
  
  add_MS2_feature_INFO( IN );
  IN = NULL;
}


///////////////////////////////////////////////
// add a MASTER ID:
void protein_group::add_MASTER_ID(int IN){  
  MASTER_ID  = IN;
}



//////////////////////////////////////////////////////////
// build protein average profile consensus
void protein_group::build_protein_average_consens(){
  
  PROFILE_AVERAGE_CONSENS.clear();
  
  // merge the list of modified and non feature list:
  vector<feature*> ALL;

  // get features of proteotypic peptide groups:
  vector<peptide_DELTA_group>::iterator X = PROTEO_TYPIC_PEPTIDE_MEMBERS.begin(); 
  while(X != PROTEO_TYPIC_PEPTIDE_MEMBERS.end()){
    
    // non modified:
    ALL.insert(ALL.begin(), (*X).get_members_list_start(), (*X).get_members_list_end());
    // modified:
    map<double, vector<feature*> >::iterator M = (*X).get_modified_members_list_start();
    while( M != (*X).get_modified_members_list_end() ){
      ALL.insert( ALL.begin(), (*M).second.begin(), (*M).second.end() );
      M++;
    }
               
    X++; 
  }
  if( ( ALL.empty() ) || ( !PEPTIDE_PROTEOTYPE_MODE ) ){
    
    // get features of non proteotypic peptide groups:
    X = PEPTIDE_MEMBERS.begin(); 
    while(X != PEPTIDE_MEMBERS.end()){
      
      // non modified:
      ALL.insert(ALL.begin(), (*X).get_members_list_start(), (*X).get_members_list_end());
      // modified:
      map<double, vector<feature*> >::iterator M = (*X).get_modified_members_list_start();
      while( M != (*X).get_modified_members_list_end() ){
        ALL.insert( ALL.begin(), (*M).second.begin(), (*M).second.end() );
        M++;
      }
      
      X++; 
    }
  }
  
  
  
  // build avarage profile
  consens_profile_builder* BUILD = new consens_profile_builder( NB_PROFILE_DATA_POINTS );
  map< int, pair<double, double> > TMP = BUILD->build_AVERAGE_CONSENS( &ALL );
  
  
  // transform the profile:
  PROFILE_AVERAGE_CONSENS.clear();
  PROFILE_AVERAGE_CONSENS_STD.clear();
  map< int, pair<double, double> >::iterator P = TMP.begin();
  while( P != TMP.end() ){
    PROFILE_AVERAGE_CONSENS.insert( make_pair( (*P).first, (*P).second.first) );   
    PROFILE_AVERAGE_CONSENS_STD.insert( make_pair( (*P).first, (*P).second.second) );   
    P++;
  }
  TMP.clear();
  
  delete BUILD;
  BUILD = NULL;
  ALL.clear();
  
}

//////////////////////////////////////////////////////////
// set protein profile probability by averaging from peptides
void protein_group::set_profile_probability_from_peptide_groups( ){
  
  PROFILE_PROBABILITY = 0.0;
  vector<double> probs;
  
  vector<peptide_DELTA_group>::iterator X = PROTEO_TYPIC_PEPTIDE_MEMBERS.begin(); 
  while( X != PROTEO_TYPIC_PEPTIDE_MEMBERS.end() ){
    probs.push_back(  (*X).get_PROFILE_PROBABILITY() );
    X++; 
  }

  if( !PEPTIDE_PROTEOTYPE_MODE ){
    X = PEPTIDE_MEMBERS.begin(); 
    while( X != PEPTIDE_MEMBERS.end() ){
      probs.push_back(  (*X).get_PROFILE_PROBABILITY() );
      X++; 
    }
  }
  
  double P_tmp = 1.0;
  vector< double >::iterator P = probs.begin();
  while( P != probs.end() ){
    P_tmp *= ( 1.0 - (*P) );
    P++;
  }
  
  
  PROFILE_PROBABILITY = 1.0 - P_tmp ; 
  probs.clear();
  
}

//////////////////////////////////////////////////////////
// write into XML formated text into file:
void protein_group::write_to_XML_file(LC_MS_XML_writer* IN){
  
  ofstream * WRITER = IN->get_WRITER();

  char buffer[500];
  sprintf(buffer,"<PROTEIN_GROUP");  
  sprintf(buffer, "%s AC=\"%s\" nb_unique_peptide=\"%d\" nb_MS1_features=\"%d\" nb_assigned_MS1_features=\"%d\" profile_score=\"%f\" profile_probability=\"%f\" protein_group_ID=\"%d\" proteotypic=\"%d\">\n", buffer, AC.c_str(), get_nb_peptide_members(), get_nb_features() , get_nb_assigned_features(), PROFILE_SCORE, PROFILE_PROBABILITY, get_PROTEIN_GROUP_ID(), get_NB_PROTEOTYPIC_PEPTIDES() );
  WRITER->write(buffer,strlen(buffer));
  
  // print overview of peptides in this protein:
  sprintf(buffer,"<MS2_PEPTIDE_HITS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  map<string, vector<MS2_HIT> >::iterator PEP = get_PEPTIDE_LIST_START();
  while( PEP != get_PEPTIDE_LIST_END() ){
    sprintf(buffer,"<MS2_UNIQUE_PEPTIDE SQ=\"%s\" number_hits=\"%d\">\n", (*PEP).first.c_str(), (*PEP).second.size());  
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"</MS2_UNIQUE_PEPTIDE>\n");  
    WRITER->write(buffer,strlen(buffer));
    PEP++;
  }
  sprintf(buffer,"</MS2_PEPTIDE_HITS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  sprintf(buffer,"<PROTEOTYPIC_PEPTIDE_GROUP_MEMBERS nb_peptide_group_members=\"%d\">\n", PROTEO_TYPIC_PEPTIDE_MEMBERS.size());  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  vector<peptide_DELTA_group>::iterator X = PROTEO_TYPIC_PEPTIDE_MEMBERS.begin(); 
  while(X != PROTEO_TYPIC_PEPTIDE_MEMBERS.end()){
    (*X).write_content_2_XML( IN );
    X++; 
  }
  sprintf(buffer,"</PROTEOTYPIC_PEPTIDE_GROUP_MEMBERS>\n");  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));

  
  sprintf(buffer,"<NON_PROTEOTYPIC_PEPTIDE_GROUP_MEMBERS nb_peptide_group_members=\"%d\">\n", PEPTIDE_MEMBERS.size());  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  X = PEPTIDE_MEMBERS.begin(); 
  while(X != PEPTIDE_MEMBERS.end()){
    (*X).write_content_2_XML( IN );
    X++; 
  }
  sprintf(buffer,"</NON_PROTEOTYPIC_PEPTIDE_GROUP_MEMBERS>\n");  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
  
  sprintf(buffer,"<OUTLIER_PEPTIDE_GROUP_MEMBERS nb_peptide_group_members=\"%d\">\n", OUTLIER_PEPTIDE_MEMBERS.size());  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  X = OUTLIER_PEPTIDE_MEMBERS.begin(); 
  while(X != OUTLIER_PEPTIDE_MEMBERS.end()){
    (*X).write_content_2_XML( IN );
    X++; 
  }
  sprintf(buffer,"</OUTLIER_PEPTIDE_GROUP_MEMBERS>\n");  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer,"<CONSENS_PROFILE>\n");  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
  // check if consens profiles are already build:
  if( PROFILE_AVERAGE_CONSENS.empty() ){
    build_protein_average_consens();
  }    
  
  map<int, double>::iterator P = PROFILE_AVERAGE_CONSENS.begin();
  while( P != PROFILE_AVERAGE_CONSENS.end() ){
    map<int, double>::iterator P_STD = PROFILE_AVERAGE_CONSENS_STD.find( (*P).first );    
    double y_std = 0;
    if( P_STD != PROFILE_AVERAGE_CONSENS_STD.end() ){
      y_std = (*P_STD).second;
    }
    
    sprintf(buffer,"<DATA_POINT MASTER_ID=\"%d\" x=\"%d\" y=\"%f\" y_stdev=\"%f\"></DATA_POINT>\n", MASTER_ID, (*P).first, (*P).second, y_std );  
    // write to the file:
    WRITER->write(buffer,strlen(buffer));
    P++;
    P_STD++;
  }
  sprintf(buffer,"</CONSENS_PROFILE>\n");  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
    
  // open a tag for features:
  sprintf(buffer,"</PROTEIN_GROUP>\n");
  // write:
  WRITER->write(buffer,strlen(buffer));
  //
  WRITER->flush();

  IN = NULL;
  WRITER = NULL;
}

//////////////////////////////////////////////////////////
// write into APML formated text into file:
void protein_group::write_to_APML_file(LC_MS_APML_writer* IN){
  
  ofstream * WRITER = IN->get_WRITER();
  
  char buffer[500];
  sprintf(buffer,"<PROTEIN_GROUP");  
  sprintf(buffer, "%s AC=\"%s\" nb_unique_peptide=\"%d\" nb_MS1_features=\"%d\" nb_assigned_MS1_features=\"%d\" profile_score=\"%f\" profile_probability=\"%f\" protein_group_ID=\"%d\" proteotypic=\"%d\">\n", buffer, AC.c_str(), get_nb_peptide_members(), get_nb_features() , get_nb_assigned_features(), PROFILE_SCORE, PROFILE_PROBABILITY, get_PROTEIN_GROUP_ID(), get_NB_PROTEOTYPIC_PEPTIDES() );
  WRITER->write(buffer,strlen(buffer));
  
  // print overview of peptides in this protein:
  sprintf(buffer,"<MS2_PEPTIDE_HITS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  map<string, vector<MS2_HIT> >::iterator PEP = get_PEPTIDE_LIST_START();
  while( PEP != get_PEPTIDE_LIST_END() ){
    sprintf(buffer,"<MS2_UNIQUE_PEPTIDE SQ=\"%s\" number_hits=\"%d\">\n", (*PEP).first.c_str(), (*PEP).second.size());  
    WRITER->write(buffer,strlen(buffer));
    sprintf(buffer,"</MS2_UNIQUE_PEPTIDE>\n");  
    WRITER->write(buffer,strlen(buffer));
    PEP++;
  }
  sprintf(buffer,"</MS2_PEPTIDE_HITS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  sprintf(buffer,"<PROTEOTYPIC_PEPTIDE_GROUP_MEMBERS nb_peptide_group_members=\"%d\">\n", PROTEO_TYPIC_PEPTIDE_MEMBERS.size());  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  vector<peptide_DELTA_group>::iterator X = PROTEO_TYPIC_PEPTIDE_MEMBERS.begin(); 
  while(X != PROTEO_TYPIC_PEPTIDE_MEMBERS.end()){
    (*X).write_content_2_APML( IN );
    X++; 
  }
  sprintf(buffer,"</PROTEOTYPIC_PEPTIDE_GROUP_MEMBERS>\n");  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
  
  sprintf(buffer,"<NON_PROTEOTYPIC_PEPTIDE_GROUP_MEMBERS nb_peptide_group_members=\"%d\">\n", PEPTIDE_MEMBERS.size());  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  X = PEPTIDE_MEMBERS.begin(); 
  while(X != PEPTIDE_MEMBERS.end()){
    (*X).write_content_2_APML( IN );
    X++; 
  }
  sprintf(buffer,"</NON_PROTEOTYPIC_PEPTIDE_GROUP_MEMBERS>\n");  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
  
  sprintf(buffer,"<OUTLIER_PEPTIDE_GROUP_MEMBERS nb_peptide_group_members=\"%d\">\n", OUTLIER_PEPTIDE_MEMBERS.size());  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  X = OUTLIER_PEPTIDE_MEMBERS.begin(); 
  while(X != OUTLIER_PEPTIDE_MEMBERS.end()){
    (*X).write_content_2_APML( IN );
    X++; 
  }
  sprintf(buffer,"</OUTLIER_PEPTIDE_GROUP_MEMBERS>\n");  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer,"<CONSENS_PROFILE>\n");  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
  // check if consens profiles are already build:
  if( PROFILE_AVERAGE_CONSENS.empty() ){
    build_protein_average_consens();
  }    
  
  map<int, double>::iterator P = PROFILE_AVERAGE_CONSENS.begin();
  while( P != PROFILE_AVERAGE_CONSENS.end() ){
    map<int, double>::iterator P_STD = PROFILE_AVERAGE_CONSENS_STD.find( (*P).first );    
    double y_std = 0;
    if( P_STD != PROFILE_AVERAGE_CONSENS_STD.end() ){
      y_std = (*P_STD).second;
    }
    
    sprintf(buffer,"<DATA_POINT MASTER_ID=\"%d\" x=\"%d\" y=\"%f\" y_stdev=\"%f\"></DATA_POINT>\n", MASTER_ID, (*P).first, (*P).second, y_std );  
    // write to the file:
    WRITER->write(buffer,strlen(buffer));
    P++;
    P_STD++;
  }
  sprintf(buffer,"</CONSENS_PROFILE>\n");  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
  
  // open a tag for features:
  sprintf(buffer,"</PROTEIN_GROUP>\n");
  // write:
  WRITER->write(buffer,strlen(buffer));
  //
  WRITER->flush();
  
  IN = NULL;
  WRITER = NULL;
}


//////////////////////////////////////////////////////////
// write into delimited TXT file:
void protein_group::write_to_TXT_file(ofstream* WRITER){
  string DEL = "\t";
  
  char buffer[500];
  sprintf(buffer, "%s%s%f%s%f%s%d%s%d\n", AC.c_str(), DEL.c_str(), PROFILE_PROBABILITY, DEL.c_str(), PROFILE_SCORE, DEL.c_str(), get_nb_features(), DEL.c_str() , get_NB_PROTEOTYPIC_PEPTIDES() );
  WRITER->write(buffer,strlen(buffer));
  
  /*
  // write the probs:
  vector<peptide_DELTA_group>::iterator X = PEPTIDE_MEMBERS.begin(); 
  while(X != PEPTIDE_MEMBERS.end()){
    sprintf(buffer, "%s%f",DEL.c_str(), (*X).get_PROFILE_PROBABILITY() );
    WRITER->write(buffer,strlen(buffer));
    X++; 
  }
  
  X = PROTEO_TYPIC_PEPTIDE_MEMBERS.begin(); 
  while(X != PROTEO_TYPIC_PEPTIDE_MEMBERS.end()){
    sprintf(buffer, "%s%f",DEL.c_str(), (*X).get_PROFILE_PROBABILITY() );
    WRITER->write(buffer,strlen(buffer));
    X++; 
  }
  */
  
  WRITER = NULL;
}



///////////////////////////////////////////////////////////////////////
// info:
void protein_group::show_info(){  
  printf("\t- AC=\"%s\" nb_unique_peptide=\"%d\" Mr=\"%f\" Nr_members=\"%d\" profile_score=\"%f\" profile_probability=\"%f\" protein_group_ID=\"%d\">\n", AC.c_str(), PEPTIDE_LIST.size(), MR, get_nb_features(), PROFILE_SCORE, PROFILE_PROBABILITY, get_PROTEIN_GROUP_ID());
}  

///////////////////////////////////////////////////////////////////////
// check how many proteotypic peptides are in the protein
int protein_group::get_NB_PROTEOTYPIC_PEPTIDES(){  
  
  int count = 0;
  if( get_PROTEOTYPE() ){

    map<string, vector<MS2_HIT> >::iterator PEP = get_PEPTIDE_LIST_START();
    while( PEP != get_PEPTIDE_LIST_END() ){
     
      vector<MS2_HIT>::iterator P = (*PEP).second.begin();
      while( P != (*PEP).second.end() ){
        
        if( (*P).second  ){
          count++;
          break;
        }
        
        P++;
      }
      
      PEP++;
    }
  }
  
  
  return count;
}



//////////////////////////////////////////////////////////////////
// get nb of identified MS1 featues in the PROTEIN group
int protein_group::get_nb_assigned_features(){
  
  int NB = 0;
  
  vector< feature* >::iterator FEA = ALL_FEATURES.begin();
  while( FEA != ALL_FEATURES.end() ){
    if( (*FEA)->get_MS2_info() ){
      NB++;
    }
    FEA++;
  }
  return NB;
}


//////////////////////////////////////////////////////////////////
// print protein profile to file:
void protein_group::print_protein_profile_to_file(){
  
  // read parameters from "param.def"
  read_param* def = new read_param();
  string file = "ANALYSIS_" + def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;
  
  if( file[ file.size() - 1] != '/' ){
   file += "/"; 
  }
  string tmp = AC;
  while( tmp.find(" ") != string::npos ){
    tmp.erase( tmp.find(" "), 1 ); 
  }
  file += tmp;
  file += ".txt";
  
  ofstream* WRITER = new ofstream();
  WRITER->open(file.c_str(), ofstream::out); 
  if(WRITER->good()){
    
    char buffer[254];
    map<int, double>::iterator P_M = PROFILE_AVERAGE_CONSENS.begin();
    map<int, double>::iterator P_S = PROFILE_AVERAGE_CONSENS_STD.begin();
    while( P_M != PROFILE_AVERAGE_CONSENS.end() ){
      sprintf(buffer,"%0.4f\t%0.4f\n", (*P_M).second, (*P_S).second); 
      WRITER->write(buffer,strlen(buffer));
      P_M++;
      P_S++;
    }  
    WRITER->close();
  }
  
  delete WRITER;
  WRITER = NULL;
}
  



//////////////////////////////////////////////////////////////////////
// give the iterators to the protein, so it can mark bad ones:
void protein_group::remove_outlier_peptide_groups( vector< vector< peptide_DELTA_group >::iterator > IN ){

  // add the peptide to the outliers:
  vector< vector< peptide_DELTA_group >::iterator >::iterator OUT = IN.begin();
  while( OUT != IN.end() ){
    OUTLIER_PEPTIDE_MEMBERS.push_back( *(*OUT) );
    OUT++;
  }
  IN.clear();
  
  // now delete them from the peptide lists:
  vector< peptide_DELTA_group >::iterator MY = OUTLIER_PEPTIDE_MEMBERS.begin();
  while( MY != OUTLIER_PEPTIDE_MEMBERS.end() ){
    
    // search in proteotypic peptides:
    string SQ = (*MY).get_SQ();
    double MR = (*MY).get_MR();
    bool FOUND = false;
    vector<peptide_DELTA_group>::iterator F = PROTEO_TYPIC_PEPTIDE_MEMBERS.begin( );
    while( F != PROTEO_TYPIC_PEPTIDE_MEMBERS.end( ) ){
      
      if( ( SQ == (*F).get_SQ() ) && ( MR == (*F).get_MR()) ){
        F = PROTEO_TYPIC_PEPTIDE_MEMBERS.erase( F );
        FOUND = true;
        break;
      }
      else{
        F++;
      }
    }
      
    // search in non proteotypic peptides:
    F = PEPTIDE_MEMBERS.begin( );
    while( ( F != PEPTIDE_MEMBERS.end( ) ) && ( !FOUND ) ){
      
      if( ( SQ == (*F).get_SQ() ) && ( MR == (*F).get_MR()) ){
        F = PEPTIDE_MEMBERS.erase( F );
        FOUND = true;
        break;
      }
      else{
        F++;
      }
    }
    
    MY++;
  }
  
}



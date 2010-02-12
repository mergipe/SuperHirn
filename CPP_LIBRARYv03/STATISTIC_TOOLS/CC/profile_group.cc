///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS profile_group:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_PROFILE_GROUP
#include "STATISTIC_TOOLS_USE.h"


int profile_group::PROFILE_LENGTH;
bool profile_group::GLOBAL_SCORING;


////////////////////////////////////////////////
// constructor for the object profile_group:
profile_group::profile_group(int IN_ID){
  CLUSTER_ID = IN_ID;
  PROFILE_GROUP_ID = IN_ID;
  MASTER_ID = -1;
  TARGET_PROFILE_ID = -1;
  CLUSTER_SCORE = 0;
  CLUSTER_STDEV = 0;
}

//////////////////////////////////////////////////
// class desctructor of profile_group
profile_group::~profile_group(){
  UNIQUE_SQ.clear();
  UNIQUE_AC.clear();
  CLUSTER_PROFILE.clear();
  MEMBER_LIST.clear();
  PEPTIDE_GROUP_LIST.clear();
  PROTEIN_GROUPS.clear();
  SELECTED_CLUSTER_PROFILES.clear();
  TARGET_PROFILE.clear();
}

//////////////////////////////////////////////////
// class copy constructor of profile_group
profile_group::profile_group(const profile_group& tmp){
  UNIQUE_SQ = tmp.UNIQUE_SQ;
  UNIQUE_AC = tmp.UNIQUE_AC;
  CLUSTER_STDEV = tmp.CLUSTER_STDEV;
  CLUSTER_PROFILE = tmp.CLUSTER_PROFILE;
  MEMBER_LIST = tmp.MEMBER_LIST;
  CLUSTER_ID = tmp.CLUSTER_ID;
  PROFILE_GROUP_ID = tmp.PROFILE_GROUP_ID;
  MASTER_ID = tmp.MASTER_ID;
  CLUSTER_SCORE = tmp.CLUSTER_SCORE;
  SELECTED_CLUSTER_PROFILES = tmp.SELECTED_CLUSTER_PROFILES;
  PEPTIDE_GROUP_LIST = tmp.PEPTIDE_GROUP_LIST;
  PROTEIN_GROUPS = tmp.PROTEIN_GROUPS;
  TARGET_PROFILE = tmp.TARGET_PROFILE;
  TARGET_PROFILE_ID = tmp.TARGET_PROFILE_ID;
}

//////////////////////////////////////////////////
// class copy constructor of profile_group
profile_group::profile_group(const profile_group* tmp){
  UNIQUE_SQ = tmp->UNIQUE_SQ; 
  UNIQUE_AC = tmp->UNIQUE_AC;
  CLUSTER_STDEV = tmp->CLUSTER_STDEV;
  CLUSTER_PROFILE = tmp->CLUSTER_PROFILE;
  MEMBER_LIST = tmp->MEMBER_LIST;
  CLUSTER_ID = tmp->CLUSTER_ID;
  PROFILE_GROUP_ID = tmp->PROFILE_GROUP_ID;
  MASTER_ID = tmp->MASTER_ID;
  SELECTED_CLUSTER_PROFILES = tmp->SELECTED_CLUSTER_PROFILES;
  CLUSTER_SCORE = tmp->CLUSTER_SCORE;
  PEPTIDE_GROUP_LIST = tmp->PEPTIDE_GROUP_LIST;
  PROTEIN_GROUPS = tmp->PROTEIN_GROUPS;
  TARGET_PROFILE = tmp->TARGET_PROFILE;
  TARGET_PROFILE_ID = tmp->TARGET_PROFILE_ID;
}

//////////////////////////////////////////////////
// copy constructor:
profile_group& profile_group::operator=(const profile_group& tmp){
  UNIQUE_SQ = tmp.UNIQUE_SQ;
  UNIQUE_AC = tmp.UNIQUE_AC;
  CLUSTER_STDEV = tmp.CLUSTER_STDEV;
  CLUSTER_PROFILE = tmp.CLUSTER_PROFILE;
  MEMBER_LIST = tmp.MEMBER_LIST;
  CLUSTER_ID = tmp.CLUSTER_ID;
  PROFILE_GROUP_ID = tmp.PROFILE_GROUP_ID;
  MASTER_ID = tmp.MASTER_ID;
  SELECTED_CLUSTER_PROFILES = tmp.SELECTED_CLUSTER_PROFILES;
  CLUSTER_SCORE = tmp.CLUSTER_SCORE;
  PEPTIDE_GROUP_LIST = tmp.PEPTIDE_GROUP_LIST;
  PROTEIN_GROUPS = tmp.PROTEIN_GROUPS;
  TARGET_PROFILE = tmp.TARGET_PROFILE;
  TARGET_PROFILE_ID = tmp.TARGET_PROFILE_ID;
  return *this;
}
    

//////////////////////////////////////////////////
// add a peptide = feature to the cluster:
void profile_group::add_cluster_member(feature* IN){

  ///////////////////////////
  // insert into the ranking list:
  MEMBER_LIST.push_back( IN );

  if( IN->get_MS2_info()){
    
    string AC = IN->get_AC();
    string SQ = IN->get_SQ();
    PEPTIDE new_member = IN;
    
    //////////////////////////
    // insert according to SQ:
    map< string, PEPTIDE_LIST >::iterator P = UNIQUE_SQ.find( SQ );
    if( P == UNIQUE_SQ.end() ){
      PEPTIDE_LIST tmp;
      tmp.push_back(new_member);
      UNIQUE_SQ.insert(pair<string, PEPTIDE_LIST> (SQ, tmp));
    }
    else{
      // store the AC and score:
      (*P).second.push_back( new_member );
    }
    
    //////////////////////////
    // insert according to AC:
    P = UNIQUE_AC.find( AC );
    if( P == UNIQUE_AC.end() ){
      PEPTIDE_LIST tmp;
      tmp.push_back(new_member);
      UNIQUE_AC.insert(pair<string, PEPTIDE_LIST> (AC, tmp));
    }
    else{
      // store the new-memeber:
      (*P).second.push_back( new_member);
    }
  }
  
  IN = NULL;
}

//////////////////////////////////////////////////
// megre another profile cluster into this cluster:
void profile_group::merge_profile_group(profile_group* IN){
  
  vector<feature*>::iterator P = IN->get_MEMBER_LIST_start();
  while( P != IN->get_MEMBER_LIST_end() ){
    add_cluster_member( (*P) );
    P++;
  }
  
  // save the profile:
  add_SELECTED_CLUSTER_PROFILE( IN->get_CLUSTER_ID(), IN->get_CLUSTER_PROFILE());

  IN = NULL;
}


//////////////////////////////////////////////////
// show overview:
void profile_group::show_profile_group_overview(){
  printf("CLUSTER %d, CLUSTER_SCORE = %f [MASTER ID = %d](%d Proteins, %d Peptides, %d total members)\n", get_PROFILE_GROUP_ID(), CLUSTER_SCORE, MASTER_ID, UNIQUE_AC.size(), UNIQUE_SQ.size(), MEMBER_LIST.size());
}

//////////////////////////////////////////////////
// show content:
void profile_group::show_cluster_members_by_SQ(){
  
  show_profile_group_overview();
  
  int m_count = 1;
  map< string, PEPTIDE_LIST >::iterator P = UNIQUE_SQ.begin();
  while( P != UNIQUE_SQ.end() ){
    
    double av_score = 0;
    // get average score:
    vector<PEPTIDE>::iterator A = (*P).second.begin();
    while( A != (*P).second.end() ){
      // print info:
      // printf("%s,", (*A)->get_SQ().c_str());    
      // print the profile:
      show_peptide_profile( (*A) );
      A++;
    }
    
    av_score /= (*P).second.size();
  
    printf("\t %d\t%s[#%d, cluster association score = %f]: \n", m_count, (*P).first.c_str(),(*P).second.size(), av_score);
    
    P++;
    m_count++;
  }
}

//////////////////////////////////////////////////
// show content:
void profile_group::show_unique_protein_overview(){
  
  // show the general cluster overview:
  show_profile_group_overview();
  
  map< string, PEPTIDE_LIST >::iterator P = UNIQUE_AC.begin();
  while( P != UNIQUE_AC.end() ){
    
    // get average score/rank:
    double av_score = 0;
    
    /////////////////////////////////
    // print unique protein info:
    printf("\t * %s [#%d peptides]: \n", (*P).first.c_str(),(*P).second.size());
    
    
    vector<PEPTIDE>::iterator PEP = (*P).second.begin();
    while( PEP != (*P).second.end() ){
      
      //////////////////////////////////
      // show its peptides:
      printf("\t\t - rank = %d: %s {CAS = %f}: \n", get_rank(0.0), (*PEP)->get_SQ().c_str(), 0.0);
      
      // show_peptide_profile( (*A).second );
      // add up score:
      PEP++;
    }
      
    // end peptide list
    ///////////////////////////////////
    
    av_score /= double((*P).second.size());
    
    // next unique protein:
    P++;
  }
}


/////////////////////////////////////////////////////////////
// show a profile of a peptide:
void profile_group::show_peptide_profile(feature* IN){
  
  double TOT_SUM = 0;
  vector<double> DATA;
  
  for( int ID = 0; ID < PROFILE_LENGTH; ID++){

    double value = 0.0;
    
    if( ID == IN->get_spectrum_ID()){
      value = IN->get_peak_area();
    }
    else{
      // check first the matches:
      map<int,feature>::iterator X = IN->find_match_by_id( ID );
      if( X != IN->get_match_list_end()){
        value = (*X).second.get_peak_area();      
      }
    }
  
    // store the value:
    TOT_SUM += value;
    DATA.push_back(value);
  }
  
  vector<double>::iterator P = DATA.begin();
  while( P != DATA.end() ){
    printf("%f\t", (*P) / TOT_SUM );
    P++;
  }
  printf("\n");
}



////////////////////////////////////////////////////////////////
// print consens profile:
void profile_group::print_consens_profile(){
  
  map<int, double>::iterator P = CLUSTER_PROFILE.begin();
  while( P != CLUSTER_PROFILE.end() ){
    printf("%f\t", (*P).second);
    P++;
  }
  printf("\n");
  
}


//////////////////////////////////////////////////
// show content ordered by the cluster association score:
void profile_group::show_cluster_members_ranking(){  
  
  printf("- Ranking of members:\n");
  printf("\t RANK\t| AC\t | CAS\n");
  
  int pos = 1;
  if( !GLOBAL_SCORING ){
    vector<feature*>::iterator F = MEMBER_LIST.begin();
    while ( F != MEMBER_LIST.end()){
      if( (*F)->get_MS2_info() ){
        printf("\t %d\t| %s\t| %f\n", pos, (*F)->get_AC().c_str(), 0.0);
      }
      pos++;
      F++;
    }
  }
  else{
    vector<feature*>::reverse_iterator F = MEMBER_LIST.rbegin();
    while ( F != MEMBER_LIST.rend()){
      if( (*F)->get_MS2_info() ){
        printf("\t %d\t| %s\t| %f\n", pos, (*F)->get_AC().c_str(), 0.0);
      }
      pos++;
      F++;
    }
  }
}



//////////////////////////////////////////////////
// get the rank of a cluster association score:
int profile_group::get_rank(double SCORE){  
  
  int rank = 1;
  return rank;
}
  
//////////////////////////////////////////////////////////
// write into XML formated text into file:
void profile_group::write_to_XML_file(LC_MS_XML_writer* IN){
  
  ofstream* WRITER = IN->get_WRITER();
  char buffer[500];
  
  //////////////////////////////////
  // print the target profile
  sprintf(buffer,"<PROFILE_GROUP ID=\"%d\" nb_MS1_features=\"%d\" nb_peptide_groups=\"%d\" nb_proteins=\"%d\">\n", get_PROFILE_GROUP_ID(), get_NB_MEMBERS(), get_NB_PEPTIDE_GROUPS(), get_NB_PROTEIN_GROUPS());  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
  //////////////////////////////////
  // print the target profile
  sprintf(buffer,"<MAIN_CLUSTER_PROFILE id=\"%d\">\n", get_CLUSTER_ID());  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
  map<int, double >::iterator C = CLUSTER_PROFILE.begin();
  while( C != CLUSTER_PROFILE.end() ){
    sprintf(buffer,"<DATA_POINT x=\"%d\" y=\"%f\"></DATA_POINT>\n", (*C).first, (*C).second );  
    WRITER->write(buffer,strlen(buffer));
    C++;
  }
  sprintf(buffer,"</MAIN_CLUSTER_PROFILE>\n");  
  WRITER->write(buffer,strlen(buffer));

  //////////////////////
  // print the target profile:
  sprintf(buffer,"<TARGET_PROFILE id=\"%d\">\n", get_TARGET_PROFILE_ID( ));  
  WRITER->write(buffer,strlen(buffer));
  
  C = TARGET_PROFILE.begin();
  while( C != TARGET_PROFILE.end() ){
    sprintf(buffer,"<DATA_POINT x=\"%d\" y=\"%f\"></DATA_POINT>\n", (*C).first, (*C).second );  
    WRITER->write(buffer,strlen(buffer));
    C++;
  }    
  sprintf(buffer,"</TARGET_PROFILE>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  //////////////////////
  // print the profiles of selected clusters:
  sprintf(buffer,"<SELECTED_CLUSTER_PROFILES>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  map<int, map<int, double> >::iterator S = SELECTED_CLUSTER_PROFILES.begin();
  while( S != SELECTED_CLUSTER_PROFILES.end() ){
    
    sprintf(buffer,"<CLUSTER_PROFILE id=\"%d\">\n", (*S).first);  
    WRITER->write(buffer,strlen(buffer));
    
    map<int, double >::iterator C = (*S).second.begin();
    while( C != (*S).second.end() ){
      sprintf(buffer,"<DATA_POINT x=\"%d\" y=\"%f\"></DATA_POINT>\n", (*C).first, (*C).second );  
      WRITER->write(buffer,strlen(buffer));
      C++;
    }
    
    sprintf(buffer,"</CLUSTER_PROFILE>\n");  
    WRITER->write(buffer,strlen(buffer));
    
    S++;
  }
  sprintf(buffer,"</SELECTED_CLUSTER_PROFILES>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  
  
  //////////////////////
  // write proteins groups:
  sprintf(buffer,"<PROTEIN_GROUPS nb_protein_groups=\"%d\" nb_peptide_groups=\"%d\">\n", get_NB_PROTEIN_GROUPS(), get_NB_PEPTIDE_GROUPS());  
  WRITER->write(buffer,strlen(buffer));
  
  vector<protein_group>::iterator PROT = get_PROTEIN_GROUPS_START();
  while( PROT != get_PROTEIN_GROUPS_END() ){
    (*PROT).write_to_XML_file( IN );      
    PROT++;
  }    
  sprintf(buffer,"</PROTEIN_GROUPS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  
  //////////////////////
  // write peptides groups:
  sprintf(buffer,"<PEPTIDE_GROUPS nb_peptide_groups=\"%d\">\n", get_NB_PEPTIDE_GROUPS());  
  WRITER->write(buffer,strlen(buffer));
  vector<peptide_DELTA_group>::iterator PEP = get_PEPTIDE_GROUPS_START();
  while( PEP != get_PEPTIDE_GROUPS_END() ){
    (*PEP).write_content_2_XML( IN );          
    PEP++;
  }
  sprintf(buffer,"</PEPTIDE_GROUPS>\n");  
  WRITER->write(buffer,strlen(buffer));

  
  
  //////////////////////////
  // write ms1 features members:
  sprintf(buffer,"<MS1_FEATURE_MEMBERS nb_MS1_features=\"%d\">\n",  get_NB_MEMBERS());  
  WRITER->write(buffer,strlen(buffer));
  
  vector< feature* >::iterator FEA = get_MEMBER_LIST_start();
  while( FEA != get_MEMBER_LIST_end() ){
    IN->write_feature_2_XML_tag( (*FEA) );
    FEA++;
  }    
  sprintf(buffer,"</MS1_FEATURE_MEMBERS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  // end of profile group
  sprintf(buffer,"</PROFILE_GROUP>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  IN = NULL;
  WRITER = NULL;
}

//////////////////////////////////////////////////////////
// write into XML formated text into file:
void profile_group::write_to_APML_file(LC_MS_APML_writer* IN){
  
  ofstream* WRITER = IN->get_WRITER();
  char buffer[500];
  
  //////////////////////////////////
  // print the target profile
  sprintf(buffer,"<PROFILE_GROUP ID=\"%d\" nb_MS1_features=\"%d\" nb_peptide_groups=\"%d\" nb_proteins=\"%d\">\n", get_PROFILE_GROUP_ID(), get_NB_MEMBERS(), get_NB_PEPTIDE_GROUPS(), get_NB_PROTEIN_GROUPS());  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
  //////////////////////////////////
  // print the target profile
  sprintf(buffer,"<MAIN_CLUSTER_PROFILE id=\"%d\">\n", get_CLUSTER_ID());  
  // write to the file:
  WRITER->write(buffer,strlen(buffer));
  
  map<int, double >::iterator C = CLUSTER_PROFILE.begin();
  while( C != CLUSTER_PROFILE.end() ){
    sprintf(buffer,"<DATA_POINT x=\"%d\" y=\"%f\"></DATA_POINT>\n", (*C).first, (*C).second );  
    WRITER->write(buffer,strlen(buffer));
    C++;
  }
  sprintf(buffer,"</MAIN_CLUSTER_PROFILE>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  //////////////////////
  // print the target profile:
  sprintf(buffer,"<TARGET_PROFILE id=\"%d\">\n", get_TARGET_PROFILE_ID( ));  
  WRITER->write(buffer,strlen(buffer));
  
  C = TARGET_PROFILE.begin();
  while( C != TARGET_PROFILE.end() ){
    sprintf(buffer,"<DATA_POINT x=\"%d\" y=\"%f\"></DATA_POINT>\n", (*C).first, (*C).second );  
    WRITER->write(buffer,strlen(buffer));
    C++;
  }    
  sprintf(buffer,"</TARGET_PROFILE>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  //////////////////////
  // print the profiles of selected clusters:
  sprintf(buffer,"<SELECTED_CLUSTER_PROFILES>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  map<int, map<int, double> >::iterator S = SELECTED_CLUSTER_PROFILES.begin();
  while( S != SELECTED_CLUSTER_PROFILES.end() ){
    
    sprintf(buffer,"<CLUSTER_PROFILE id=\"%d\">\n", (*S).first);  
    WRITER->write(buffer,strlen(buffer));
    
    map<int, double >::iterator C = (*S).second.begin();
    while( C != (*S).second.end() ){
      sprintf(buffer,"<DATA_POINT x=\"%d\" y=\"%f\"></DATA_POINT>\n", (*C).first, (*C).second );  
      WRITER->write(buffer,strlen(buffer));
      C++;
    }
    
    sprintf(buffer,"</CLUSTER_PROFILE>\n");  
    WRITER->write(buffer,strlen(buffer));
    
    S++;
  }
  sprintf(buffer,"</SELECTED_CLUSTER_PROFILES>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  
  
  //////////////////////
  // write proteins groups:
  sprintf(buffer,"<PROTEIN_GROUPS nb_protein_groups=\"%d\" nb_peptide_groups=\"%d\">\n", get_NB_PROTEIN_GROUPS(), get_NB_PEPTIDE_GROUPS());  
  WRITER->write(buffer,strlen(buffer));
  
  vector<protein_group>::iterator PROT = get_PROTEIN_GROUPS_START();
  while( PROT != get_PROTEIN_GROUPS_END() ){
    (*PROT).write_to_APML_file( IN );      
    PROT++;
  }    
  sprintf(buffer,"</PROTEIN_GROUPS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  
  //////////////////////
  // write peptides groups:
  sprintf(buffer,"<PEPTIDE_GROUPS nb_peptide_groups=\"%d\">\n", get_NB_PEPTIDE_GROUPS());  
  WRITER->write(buffer,strlen(buffer));
  vector<peptide_DELTA_group>::iterator PEP = get_PEPTIDE_GROUPS_START();
  while( PEP != get_PEPTIDE_GROUPS_END() ){
    (*PEP).write_content_2_APML( IN );          
    PEP++;
  }
  sprintf(buffer,"</PEPTIDE_GROUPS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  
  if( !MEMBER_LIST.empty() ){
  
    //////////////////////////
    // write ms1 features members:
    sprintf(buffer,"<MS1_FEATURE_MEMBERS nb_MS1_features=\"%d\">\n",  get_NB_MEMBERS());  
    WRITER->write(buffer,strlen(buffer));
    
    vector< feature* >::iterator FEA = get_MEMBER_LIST_start();
    while( FEA != get_MEMBER_LIST_end() ){
      IN->write_aligned_feature_2_APML_tag( (*FEA) );
      FEA++;
    }    
    sprintf(buffer,"</MS1_FEATURE_MEMBERS>\n");  
    WRITER->write(buffer,strlen(buffer));
    
    
  }
  
  // end of profile group
  sprintf(buffer,"</PROFILE_GROUP>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  IN = NULL;
  WRITER = NULL;
}



////////////////////////////////////////////////////////////////////////////////////////
// show info:
void profile_group::show_info(){
  
  // get number of MS2-identifed MS1 features:
  int MS2_count = 0;
  vector< feature*>::iterator IT = get_MEMBER_LIST_start();
  while( IT != get_MEMBER_LIST_end() ){
    if( (*IT)->get_MS2_info( -3.0 ) ) MS2_count++;
    IT++;
  }
  
  printf("\t\tProfile Group %d (M_ID = %d):\n", get_PROFILE_GROUP_ID(), MASTER_ID);  
  printf("\t\t\t\t- %d MS1 feature members (%d)\n", MEMBER_LIST.size(), MS2_count );  
  printf("\t\t\t\t- %d peptide groups\n", PEPTIDE_GROUP_LIST.size() );  
  printf("\t\t\t\t- %d protein groups\n", PROTEIN_GROUPS.size() );  
  
  
}



////////////////////////////////////////////////////////////////////////////////////////
// set the target profile from a vector
void profile_group::set_TARGET_PROFILE( vector<double> IN){
  TARGET_PROFILE.clear();
  int my_id = 0;
  vector<double>::iterator P = IN.begin();
  while( P != IN.end() ){
    TARGET_PROFILE.insert( make_pair( my_id, (*P) ) );
    my_id++;
    P++;
  }
}
  
////////////////////////////////////////////////////////////////////////////////////////
// sort the porteins according to profile probablity 
void profile_group::sort_proteins_by_profile_probability( ){

  sort( PROTEIN_GROUPS.begin(), PROTEIN_GROUPS.end(), OPERATOR_PROT() ); 
  
}

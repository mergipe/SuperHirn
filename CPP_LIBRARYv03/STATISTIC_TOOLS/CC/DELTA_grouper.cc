///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS DELTA_grouper:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_DELTA_GROUPER
#define USE_PROTEIN_GROUP
#define USE_PROFILE_CLUSTER
#define USE_PEPTIDE_DELTA_GROUP

#include "STATISTIC_TOOLS_USE.h"

bool DELTA_grouper::MS_2_SELECTION = true;
int DELTA_grouper::MIN_NB_PROFILE_POINTS;


////////////////////////////////////////////////
// constructor for the object DELTA_grouper:
DELTA_grouper::DELTA_grouper(){
  
  MASTER = NULL;
  INIT_PARAM();
}

//////////////////////////////////////////////////
// class desctructor of DELTA_grouper
DELTA_grouper::DELTA_grouper(LC_MS* IN){
  MASTER = IN;
  INIT_PARAM();
}

//////////////////////////////////////////////////
// class desctructor of DELTA_grouper
DELTA_grouper::~DELTA_grouper(){
  MASTER = NULL;
  LC_MS_DELTA_masses.clear();
  TARGET_CLUSTER_PROFILES.clear();
  PEPTIDE_GROUPS.clear();
}

//////////////////////////////////////////////////
// class copy constructor of DELTA_grouper
DELTA_grouper::DELTA_grouper(const DELTA_grouper& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of DELTA_grouper
DELTA_grouper::DELTA_grouper(const DELTA_grouper* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
DELTA_grouper& DELTA_grouper::operator=(const DELTA_grouper& tmp){
  return *this;
}


///////////////////////////////////////////////////
// initialize the parameters:
void DELTA_grouper::INIT_PARAM(){
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  
  // MS2 filter:
  def->search_tag("MS2 Delta clustering filter", &MS_2_SELECTION);

  // min. number of detected:
  def->search_tag("min. nb. of profile data points", &MIN_NB_PROFILE_POINTS);
  
  // print out
  def->search_tag("gnuplot plot generator", &PRINT_CLUSTER);
  
  // list of possible modifications:
  def->search_tag("Delta M/z list", &LC_MS_DELTA_masses);
 
  delete def;
  def = NULL;
  
  // order MASTER LC/MS accroding to m/z 
  if(MASTER != NULL){
    MASTER->order_by_mass();
  }
  
  // TR_TOL 
  TR_TOL = feature::get_TR_TOL();
}

///////////////////////////////////////////////////
// start finding correpsonding features with
// m/z delta shifts:
void DELTA_grouper::extract_PEPTIDE_DELTA_GROUPS_from_MASTERMAP(){

  if( MASTER != NULL){
    
    progress_bar bar( MASTER->get_nb_features(), "processed");
    
    // go throug each feature and try to find its pair: 
    vector<LC_MS_FEATURE>::iterator FEA = MASTER->get_feature_list_begin();
    while( FEA != MASTER->get_feature_list_end() ){ 
      
      feature* THIS_F = &(*FEA);
      
      if( THIS_F->get_replicate_match_nb() >= MIN_NB_PROFILE_POINTS  ){
        
        // see if it matches to a existing protein_group:
        bool belong_check = check_peptide_cluster_belonging( THIS_F,  &PEPTIDE_GROUPS);
        
        // not association found, make new one:
        if( !belong_check ){
          
          peptide_DELTA_group* NEW = new peptide_DELTA_group( THIS_F);
          PEPTIDE_GROUPS.push_back( *NEW );        
          NEW = NULL;
        }
      }
      
      // progress bar:
      bar.update_progress();
      
      FEA++;
    }
  }
    
}
  
///////////////////////////////////////////////////
// start finding correpsonding features with
// m/z delta shifts:
void DELTA_grouper::start_MASTER_LC_MS_PAIR_MATCHING(){

  if( MASTER != NULL){
    
    progress_bar bar( MASTER->get_nb_features(), "processed");
    
    // go throug each feature and try to find its pair: 
    vector<LC_MS_FEATURE>::iterator FEA = MASTER->get_feature_list_begin();
    while( FEA != MASTER->get_feature_list_end() ){ 
  
      feature* THIS_F = &(*FEA);
      
      if( THIS_F->get_replicate_match_nb() >= MIN_NB_PROFILE_POINTS  ){
  
        // see if it matches to a existing protein_group:
        bool belong_check = check_peptide_cluster_belonging( THIS_F,  &PEPTIDE_GROUPS);
        
        // not association found, make new one:
        if( !belong_check ){
          
          peptide_DELTA_group* NEW = new peptide_DELTA_group( THIS_F);
          PEPTIDE_GROUPS.push_back( *NEW );        
          NEW = NULL;
        }
      }
      
      // progress bar:
      bar.update_progress();

      FEA++;
    }
    
    // process extracted delta pairs:
    process_extracted_peptide_DELTA_groups(  );
    
    
    // print matching results:
    printf("\t\t\t\t-> %d peptide groups extracted.\n", PEPTIDE_GROUPS.size() );
    
    // write into xml:
    write_peptide_groups_to_XML( );
    write_peptide_groups_to_TXT();
    
    PEPTIDE_GROUPS.clear();
  }
}

///////////////////////////////////////////////////
// find corresponding peptide groups for a feature
void DELTA_grouper::find_PEPTIDE_GROUPS_for_FEATURE(feature* IN){
  
  // see if it matches to a existing protein_group:
  bool belong_check = check_peptide_cluster_belonging( IN, &PEPTIDE_GROUPS );      
  
  // no association found, make new one:
  if( !belong_check ){
    peptide_DELTA_group* NEW = new peptide_DELTA_group( IN );
    PEPTIDE_GROUPS.push_back( *NEW );
    NEW = NULL;
  }
  
}

 
///////////////////////////////////////////////////
// groups feature in the clusters into peptide groups
// m/by MR:
void DELTA_grouper::extract_PEPTIDE_GROUPS_from_PROFILE_GROUP(profile_group* this_C){
  
  // get the MASTER run ID
  int MASTER_ID = this_C->get_MASTER_ID();
  vector<peptide_DELTA_group> THIS_PEPTIDE_GROUPS;
  int pep_group_ID = 0;
  
  vector<feature*>::iterator FEA = this_C->get_MEMBER_LIST_start();
  while( FEA != this_C->get_MEMBER_LIST_end() ){
    
    feature* THIS_F = (*FEA);
    THIS_F->set_MASTER_ID(MASTER_ID);
    
    // see if it matches to a existing protein_group:
    bool belong_check = check_peptide_cluster_belonging( THIS_F  );      
    
    // not association found, make new one:
    if( !belong_check ){
      
      peptide_DELTA_group* NEW = new peptide_DELTA_group( THIS_F );
      NEW->set_PEPTIDE_GROUP_ID( pep_group_ID );
      pep_group_ID++;
      THIS_PEPTIDE_GROUPS.push_back( *NEW );
      NEW = NULL;
      
    }
    // erase since its saved inform of peptide group 
    FEA = this_C->erase_MEMBER( FEA );
    
  }
  
  // save the generated list:
  this_C->add_PEPTIDE_GROUPS( THIS_PEPTIDE_GROUPS);
  THIS_PEPTIDE_GROUPS.clear();

  // check for modifications:
  extract_PEPTIDE_GROUPS_MODIFICATIONS( this_C );
    
}

//////////////////////////////////////////////////////////////////
// check for modifications in every peptide group in a profile group:
void DELTA_grouper::extract_PEPTIDE_GROUPS_MODIFICATIONS(profile_group* IN){
  
  // go through each peptide group and check for modifications:
  vector<peptide_DELTA_group>::iterator P = IN->get_PEPTIDE_GROUPS_START();
  while( P != IN->get_PEPTIDE_GROUPS_END() ){    
    (*P).set_PEPTIDE_MODIFICATIONS();
    P++;
  }
}




//////////////////////////////////////////////////////////////////
// process extracted delta pairs:
void DELTA_grouper::process_extracted_peptide_DELTA_groups(){
  
  // set the max length of teh profiles: 
  peptide_DELTA_group::NB_PROFILE_DATA_POINTS = MASTER->get_nb_raw_specs();
  // go through each peptide group:
  vector<peptide_DELTA_group>::iterator P = PEPTIDE_GROUPS.begin();
  while( P != PEPTIDE_GROUPS.end() ){    

    peptide_DELTA_group* PEP = &(*P);
    
    // build consens profile:
    PEP->build_peptide_average_consens();
    
    
    PEP = NULL;
    P++;
  }  
}


//////////////////////////////////////////////////////////////////
// group the existing peptide clusters into protein_group objects:
void DELTA_grouper::extract_PROTEIN_GROUPS_from_PROFILE_GROUP(profile_group* IN){
  
  // go through each peptide group in this cluster and create PROTEIN groups:
  vector<protein_group> PROTEIN_GROUPS;
  vector<peptide_DELTA_group>::iterator P = IN->get_PEPTIDE_GROUPS_START();
  int protein_ID = 0;
  while( P != IN->get_PEPTIDE_GROUPS_END() ){
  
    peptide_DELTA_group* this_pep = &(*P);
    
    // check if it contains MS2 info:
    if( this_pep->get_BEST_MS2_SCAN() != NULL){
      
      bool protein_test = check_protein_cluster_belonging( this_pep , &PROTEIN_GROUPS);
      
      // was not associated to any protein group;
      // make new one:
      if( !protein_test ){
        
        map<string, vector<ms2_info*> >::iterator Q = this_pep->get_MS2_INFO_START();
        while( Q != this_pep->get_MS2_INFO_END() ){
          
          protein_group* PROT_G = new protein_group( (*Q).first );
          PROT_G->merge_peptide_DELTA_group( this_pep );
          PROT_G->set_PROTEIN_GROUP_ID(protein_ID);
          protein_ID++;
          // insert into the vector of the main structure:
          PROTEIN_GROUPS.push_back( PROT_G );        
          PROT_G = NULL;
          Q++;
        }
        
        this_pep = NULL;
      }
      
      P = IN->erase_PEPTIDE_GROUP( P );
    }
    else{
      P++;
    }
  }
  
  // save the generated list:
  IN->add_PROTEIN_GROUPS( PROTEIN_GROUPS);  

}


//////////////////////////////////////////////////////////////////
// see if it matches to a existing protein_group:
bool DELTA_grouper::check_protein_cluster_belonging( peptide_DELTA_group* IN, vector<protein_group>* DATA){

  bool belong_check = false;

  vector<protein_group>::iterator PG = DATA->begin();      
  while( PG != DATA->end() ){
    
    if( (*PG).check_belonging( IN ) ){
      (*PG).merge_peptide_DELTA_group( IN );
      belong_check = true;
    }
    
    PG++;
  }
  
  DATA = NULL;
  return belong_check;
}
//////////////////////////////////////////////////////////////////
// see if it matches to a existing protein_group:
bool DELTA_grouper::check_peptide_cluster_belonging( peptide_DELTA_group* IN){
  
  bool belong_check = false;
  
  vector<peptide_DELTA_group>::iterator PG = PEPTIDE_GROUPS.begin();      
  while( PG != PEPTIDE_GROUPS.end() ){
    
    if( (*PG).check_belonging( IN ) ){
      (*PG).merge_peptide_DELTA_group( IN );
      belong_check = true;
    }
    
    PG++;
  }
  
  return belong_check;
}

//////////////////////////////////////////////////////////////////
// see if it matches to a existing protein_group:
bool DELTA_grouper::check_peptide_cluster_belonging( feature* IN, vector<peptide_DELTA_group>* DATA){
  
  bool belong_check = false;  
  vector<peptide_DELTA_group>::iterator PG = DATA->begin();      
  while( PG != DATA->end() ){
    
    if( (*PG).check_belonging( IN ) ){
      (*PG).add_feature( IN );
      belong_check = true;
    }
    
    PG++;
  }
  
  IN = NULL;
  return belong_check;
}

//////////////////////////////////////////////////////////////////
// see if it matches to a existing protein_group:
bool DELTA_grouper::check_peptide_cluster_belonging( feature* IN ){
  
  bool belong_check = false;
  
  vector<peptide_DELTA_group>::iterator PG = PEPTIDE_GROUPS.begin();      
  while( PG != PEPTIDE_GROUPS.end() ){
    
    if( (*PG).check_belonging( IN ) ){
      (*PG).add_feature( IN );
      belong_check = true;
    }
    
    PG++;
  }
  
  IN = NULL;
  return belong_check;
}



//////////////////////////////////////////////////////////////////
// see if it matches to a existing peptide_DELTA_group:
bool DELTA_grouper::check_peptide_cluster_belonging(feature* IN, map<double, peptide_DELTA_group>* PEPTIDE_GROUPS){

  bool belong_check = false;
  map<double,peptide_DELTA_group>::iterator PG = PEPTIDE_GROUPS->begin();      
  while( PG != PEPTIDE_GROUPS->end() ){ 
    if( (*PG).second.check_belonging( IN ) ){
      (*PG).second.add_feature( IN );
      belong_check = true;
    }
    PG++;
  }
  return belong_check;
}



//////////////////////////////////////////////////////////////////
// show protein groups:
void DELTA_grouper::show_peptide_groups(){}


//////////////////////////////////////////////////////////////////
// write the peptide groups to a file:
void DELTA_grouper::write_peptide_groups_to_XML( ){
  
  string file_name = "peptide_DELTA_grouping.xml";
  
  // write first the LC/MS MASTER run:
  LC_MS_XML_writer* LC_W = new LC_MS_XML_writer( file_name );
  LC_W->write_unformatted_XML_MAIN_HEADER();
  ofstream* WRITER = LC_W->get_WRITER();
  // write now the DELTA clustering tag:
  char buffer[500];  
  sprintf(buffer, "<PEPTIDE_GROUPS nb_peptide_clusters=\"%d\">\n", PEPTIDE_GROUPS.size()); 
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer, "<INPUT_DELTA_VALUES>\n"); 
  WRITER->write(buffer,strlen(buffer));
  // print input shifts for every LC/MS:
  map<int, string>::iterator IT = MASTER->get_raw_spec_name_start();
  while( IT != MASTER->get_raw_spec_name_end()){
    
    vector<double>::iterator v = get_LC_MS_DELTA_mass( (*IT).first )->begin();
    while( v != get_LC_MS_DELTA_mass( (*IT).first )->end() ){
      sprintf(buffer, "<DELTA_VALUE LC_MS_name=\"%s\" LC_MS_ID=\"%d\" delta=\"%f\"></DELTA_VALUE>\n", (*IT).second.c_str(), (*IT).first, (*v) ); 
      WRITER->write(buffer,strlen(buffer));
      v++;
    }
    IT++; 
  }
  
  sprintf(buffer, "</INPUT_DELTA_VALUES>\n"); 
  WRITER->write(buffer,strlen(buffer));
  
  // now the peptide groups:
  vector<peptide_DELTA_group>::iterator PG = PEPTIDE_GROUPS.begin();      
  while( PG != PEPTIDE_GROUPS.end() ){
    
    (*PG).write_content_2_XML( LC_W );
    PG++;
  }
  
  sprintf(buffer, "</PEPTIDE_GROUPS>\n"); 
  WRITER->write(buffer,strlen(buffer));
  
  LC_W->write_XML_MAIN_HEADER_CLOSE();
  
  delete LC_W;
  LC_W = NULL;
}


//////////////////////////////////////////////////////////////////
// write the peptide groups to a txt file:
void DELTA_grouper::write_peptide_groups_to_TXT(){

  string file_name = "peptide_DELTA_grouping.txt";
  LC_MS_XML_writer* LC_W = new LC_MS_XML_writer( file_name );

  LC_W->set_OUT_DIR();
  LC_W->open_WRITER();

  ofstream* WRITER = LC_W->get_WRITER();
  
  if( WRITER != NULL ){
    // write now the peptide groups:
    char buffer[500];  
    string SEP = "\t";
    
    sprintf(buffer, "\n\n\n"); 
    WRITER->write(buffer,strlen(buffer));
    
    // write the header:
    sprintf(buffer, "MR%sTR%s#%s+1%s+2%s+3%s", SEP.c_str(), SEP.c_str(), SEP.c_str(), SEP.c_str(), SEP.c_str(), SEP.c_str());
    WRITER->write(buffer,strlen(buffer));
    
    int max_points = MASTER->get_nb_raw_specs();
    int i = 0;
    while( i < max_points ){
      sprintf(buffer, "%d%s", i, SEP.c_str());
      WRITER->write(buffer,strlen(buffer));
      i++;
    }
    sprintf(buffer, "\n");
    WRITER->write(buffer,strlen(buffer));
    
    // now the peptide groups:
    vector<peptide_DELTA_group>::iterator PG = PEPTIDE_GROUPS.begin();      
    while( PG != PEPTIDE_GROUPS.end() ){
      
      if( MS_2_SELECTION ){
        if( (*PG).MS2_cluster_info() ){
          (*PG).write_content_2_TXT( WRITER );
        }
      }
      else{
        (*PG).write_content_2_TXT( WRITER );
      }  
      PG++;
    }
    // printf("\t\t- peptide group written to '%s'.\n", file_name.c_str()); 
  }
  else{
    printf("\t\t* file '%s' could not be written! *\n", file_name.c_str()); 
  }
  
  delete LC_W;
  LC_W = NULL;
}

////////////////////////////////////////////
// check if contains a ID from every LC/MS:
bool DELTA_grouper::check_LC_MS_IDS(peptide_DELTA_group* IN){
  
  if( MASTER != NULL){
  
    vector<feature*>* CHECK = IN->get_members_list();
    map<int, string>::iterator IT = MASTER->get_raw_spec_name_start();
    while( IT != MASTER->get_raw_spec_name_end()){
      
      int ID = (*IT).first;
      vector<feature*>::iterator p = CHECK->begin();  
      bool FOUND = false;  
      while( p != CHECK->end() ) {
        
        if( (*p)->get_spectrum_ID() == ID ){
          FOUND = true;
          break;
        }
        
        p++; 
      }
      
      if( ! FOUND ){
        return false;
      }
      
      IT++; 
    }
    return true;
  }
  else{
    return true;
  }
}

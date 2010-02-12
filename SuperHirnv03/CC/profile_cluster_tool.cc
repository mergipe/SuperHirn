///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS profile_cluster_tool:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_TREE_BUILDER
#define USE_MATRIX
#define USE_MAIN_CLUSTER
#define USE_PROFILE_CLUSTER_TOOL
#include "use.h"


bool profile_cluster_tool::TARGET_PROFILE_CLUSTER_INITIAZTION = false;


////////////////////////////////////////////////
// constructor for the object profile_cluster_tool:
profile_cluster_tool::profile_cluster_tool(LC_MS* IN){

  MASTER = IN;
  RANDOM_PROFILE_CLUSTERING = false;  
  RES = NULL;
  
  

  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  
  
  // check which score schem is used for k-means:
  bool global_schema = false;
  def->search_tag("global cluster modus", &global_schema);
  profile_group::GLOBAL_SCORING = global_schema;
  
  // print out
  def->search_tag("gnuplot plot generator", &PRINT_CLUSTER);
  
  // minimal nb of data points in a profile to be clustered
  def->search_tag("min. nb. of profile data points", &MIN_REPLICATE_NB);
  
  // get the input cluster profiles:
  def->search_tag("target profile clustering initiation", &TARGET_PROFILE_CLUSTER_INITIAZTION);
  
  // profile replicate set ids:
  def->search_tag("profile replicate set ids", &consens_profile_builder::NB_REPLICATE_IDS);
    
  // get the input cluster profiles:
  def->search_tag("clustering input profiles", &TARGET_CLUSTER_PROFILES);
   
  // set here the how many data points a porfile can have maximally = nb. of LC/MS runs
  MAX_PROFILE_NB = MASTER->get_nb_raw_specs();
  profile_group::PROFILE_LENGTH = MAX_PROFILE_NB;
  profile_scorer::MAX_NB_PROILE_DATA_POINTS = MAX_PROFILE_NB;
  protein_group::NB_PROFILE_DATA_POINTS = MAX_PROFILE_NB;
  peptide_DELTA_group::NB_PROFILE_DATA_POINTS = MAX_PROFILE_NB;
  consens_profile_builder::MAX_NB_PROILE_DATA_POINTS = MAX_PROFILE_NB;
  
  if( ! consens_profile_builder::NB_REPLICATE_IDS.empty() ){
    profile_scorer::MAX_NB_PROILE_DATA_POINTS = consens_profile_builder::NB_REPLICATE_IDS.size();
  }
  
  
    
  delete def;
  def = NULL;
}

//////////////////////////////////////////////////
// class desctructor of profile_cluster_tool
profile_cluster_tool::~profile_cluster_tool(){
  
  MASTER = NULL;
  
  if(RES != NULL){
    delete RES;
    RES = NULL;
  }
  ID_DATA.clear();
  TARGET_CLUSTER_PROFILES.clear();
  OUT_CLUSTERS.clear();
  TARGET_PROFILE_GROUPS.clear();

}

//////////////////////////////////////////////////
// class copy constructor of profile_cluster_tool
profile_cluster_tool::profile_cluster_tool(const profile_cluster_tool& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of profile_cluster_tool
profile_cluster_tool::profile_cluster_tool(const profile_cluster_tool* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
profile_cluster_tool& profile_cluster_tool::operator=(const profile_cluster_tool& tmp){
	return *this;
}
    
///////////////////////////////////////////////////
// start to cluster the input data:
void profile_cluster_tool::start_profile_clustering(){
  
  // start the clustering process with K means method:
  start_K_MEAN_clustering();
  
  // start a random profile clustering :
  // start_RANDOM_PROFILE_clustering();
  
  // start the X_Validation process
  // start_K_MEAN_X_VALIDATION();
} 
 

///////////////////////////////////////////////////////
// start the clustering process by the K meas method:
void profile_cluster_tool::start_K_MEAN_clustering(){
  
  // set a new id to all features in the matrix:
  renumerate_features_in_MASTER();
  
  RANDOM_PROFILE_CLUSTERING = false;
  printf("\t\t- K-MEANS clustering in process...\n");
  
  // initialize the cluster object:
  // give functions to access parameters:
  KMEAN_main_cluster* PRO = new KMEAN_main_cluster( this );
  
  // clear all cluster centers:
  CLUSTER_CENTERS.clear();
  
  // insert the input master cluster profiles:
  if( TARGET_PROFILE_CLUSTER_INITIAZTION ){
    insert_target_cluster_centers();
  }
  
  ////////////////////////////////////////////////////////
  // set all the required functions by the cluster object:
  // to insert cluster centers:
  PRO->set_insert_cluster_center_FCT(profile_cluster_tool::insert_cluster_center);
  // to delete cluster centers:
  PRO->set_delete_cluster_center_FCT(profile_cluster_tool::delete_cluster_center);
  // to compare a features to a centroid:
  PRO->set_compare_FCT( profile_cluster_tool::OUTSIDE_compare_profile );
  // to compare 2 features:
  PRO->set_feature_compare_FCT( profile_cluster_tool::OUTSIDE_feature_compare_profile );
  // to compare 2 clusters:
  PRO->set_cluster_compare_FCT( profile_cluster_tool::OUTSIDE_cluster_compare_profile );
  // to get an objects by ID:
  PRO->set_get_object_by_id_FCT(profile_cluster_tool::get_feature_by_ID);
  // to print the content of an objects by ID:
  PRO->set_print_object_by_id_FCT(profile_cluster_tool::print_feature_info);
  // to compute a new mean center of a cluster by ID:
  PRO->set_compute_new_center_FCT(profile_cluster_tool::compute_MEAN_center);
  ////////////////////////////////////////////////////////
  
  // set the data:
  PRO->set_DATA( ID_DATA );
  
  // start the clustering
  PRO->start_clustering();
  
  // sort the clusters
  // PRO->order_clusters();
  
  // create profile groups from every cluster:
  transform_CLUSTERS_to_PROFILE_GROUPS( PRO );
  
  delete PRO;
  PRO = NULL;
  
  //////////////////////////////////////
  // analyze the profile groups:
  analyze_profile_groups();
  
  ////////////////////////////////////////
  // check if plotting requested:
  if( PRINT_CLUSTER ){
    plot_all_end_cluster();
  }
}

///////////////////////////////////////////////////////
// start the X validation, determination of optimal
// number of start kmeans cluster
// -> see cluster_X_scorer class
void profile_cluster_tool::start_K_MEAN_X_VALIDATION(){
  
  printf("\t\t- K-MEANS clustering X validation...\n");
  cluster_X_scorer EVALUATER;
  int X_CROSS_REPLCIATE_NR = 10;

  vector<double> CLUSTER_NB;
  //CLUSTER_NB.push_back( 2 );
  //CLUSTER_NB.push_back( 4 );
  //CLUSTER_NB.push_back( 6 );
  //CLUSTER_NB.push_back( 8 );
  //CLUSTER_NB.push_back( 10 );
//  CLUSTER_NB.push_back( 12 );
  CLUSTER_NB.push_back( 14 );
  CLUSTER_NB.push_back( 16 );
  CLUSTER_NB.push_back( 18 );
  CLUSTER_NB.push_back( 20 );
  CLUSTER_NB.push_back( 22 );

  
  vector<double>::iterator CN = CLUSTER_NB.begin();
  while( CN != CLUSTER_NB.end() ){
      
    double NB_OF_START_CLUSTERS = (*CN);
    // set a new id to all features in the matrix:
    renumerate_features_in_MASTER();
    
    // initialize the cluster object:
    // clear all cluster centers:
    CLUSTER_CENTERS.clear();
    RANDOM_PROFILE_CLUSTERING = false;
    KMEAN_main_cluster* PRO = new KMEAN_main_cluster( this );
    PRO->set_NB_START_CLUSTERS( NB_OF_START_CLUSTERS );
    // insert the input master cluster profiles:
    // insert_target_cluster_centers();
    
    ////////////////////////////////////////////////////////
    // set all the required functions by the cluster object:
    // to insert cluster centers:
    PRO->set_insert_cluster_center_FCT(profile_cluster_tool::insert_cluster_center);
    // to delete cluster centers:
    PRO->set_delete_cluster_center_FCT(profile_cluster_tool::delete_cluster_center);
    // to compare a features to a centroid:
    PRO->set_compare_FCT( profile_cluster_tool::OUTSIDE_compare_profile );
    // to compare 2 features:
    PRO->set_feature_compare_FCT( profile_cluster_tool::OUTSIDE_feature_compare_profile );
    // to compare 2 clusters:
    PRO->set_cluster_compare_FCT( profile_cluster_tool::OUTSIDE_cluster_compare_profile );
    // to get an objects by ID:
    PRO->set_get_object_by_id_FCT(profile_cluster_tool::get_feature_by_ID);
    // to print the content of an objects by ID:
    PRO->set_print_object_by_id_FCT(profile_cluster_tool::print_feature_info);
    // to compute a new mean center of a cluster by ID:
    PRO->set_compute_new_center_FCT(profile_cluster_tool::compute_MEAN_center);
    
    // set the data:
    PRO->set_DATA( ID_DATA );
    // start the clustering
    PRO->start_clustering();
    // evaluate the clusters
    EVALUATER.evaluate_kmeans_clustering( PRO );
    
    
    ////////////////////////////////////////////////////////
    // build reference library:
    printf("\t\t\t- build reference library:\n");
    int i = 0;
    while( i < X_CROSS_REPLCIATE_NR){ 
      CLUSTER_CENTERS.clear();
      build_random_feature_profiles( );
      PRO->set_DATA( ID_DATA );
      EVALUATER.build_GAP_REFERENCE_DISTRIBUTION( );
      i++;
    }
    
    // build now the total score
    // EVALUATER.build_CH_clustering_SCORE( );
    EVALUATER.build_ITERATION_GAP_STATISTICS( );
    EVALUATER.show_evaluation( );

    delete PRO;
    PRO = NULL;
    CN++;
  }
  
  ////////////////////////////////////////
  // check if plotting requested:
  if( PRINT_CLUSTER ){
    EVALUATER.PRINT_ITERATION_SCORES( );
  }
}

///////////////////////////////////////////////////////
// start a kmeans cluster with random profiling data
void profile_cluster_tool::start_RANDOM_PROFILE_clustering(){
  
  printf("\t\t- K-MEANS random profile clustering...\n");
  // set a new id to all features in the matrix:
  renumerate_features_in_MASTER();
  CLUSTER_CENTERS.clear();

  // build a random profile library library:
  CLUSTER_CENTERS.clear();
  build_random_feature_profiles( );
  KMEAN_main_cluster* PRO = new KMEAN_main_cluster( this );
  PRO->set_DATA( ID_DATA );
  // insert the input master cluster profiles:
  // insert_target_cluster_centers();
    
  ////////////////////////////////////////////////////////
  // set all the required functions by the cluster object:
  // to insert cluster centers:
  PRO->set_insert_cluster_center_FCT(profile_cluster_tool::insert_cluster_center);
  // to delete cluster centers:
  PRO->set_delete_cluster_center_FCT(profile_cluster_tool::delete_cluster_center);
  // to compare a features to a centroid:
  PRO->set_compare_FCT( profile_cluster_tool::OUTSIDE_compare_profile );
  // to compare 2 features:
  PRO->set_feature_compare_FCT( profile_cluster_tool::OUTSIDE_feature_compare_profile );
  // to compare 2 clusters:
  PRO->set_cluster_compare_FCT( profile_cluster_tool::OUTSIDE_cluster_compare_profile );
    // to get an objects by ID:
  PRO->set_get_object_by_id_FCT(profile_cluster_tool::get_feature_by_ID);
  // to print the content of an objects by ID:
  PRO->set_print_object_by_id_FCT(profile_cluster_tool::print_feature_info);
  // to compute a new mean center of a cluster by ID:
  PRO->set_compute_new_center_FCT(profile_cluster_tool::compute_MEAN_center);

  // start the clustering
  PRO->start_clustering();
  transform_CLUSTERS_to_PROFILE_GROUPS( PRO );

  delete PRO;
  PRO = NULL;
    
  // check if plotting requested:
  if( PRINT_CLUSTER ){    
    plot_all_end_cluster();
  }
}


/////////////////////////////////////////////////////
// renumerate the features for the clustering:
void profile_cluster_tool::renumerate_features_in_MASTER(){
  
  int ID = 0;
  ID_DATA.clear();
  // get start /end of the feature list:
  vector< LC_MS_FEATURE >::iterator A = MASTER->get_feature_list_begin();
  
  // progress_bar bar( MASTER->get_nb_features());  
  while( A != MASTER->get_feature_list_end() ){
    
    (*A).set_feature_ID(ID);
    
    // remove those which have a small nb of
    // features matches:
    if( (*A).get_replicate_match_nb() >= MIN_REPLICATE_NB ){
      // store a vector of observed IDs
      ID_DATA.push_back(ID);
    }
    
    ID++;
    A++;
  }
  RANDOM_PROFILE_CLUSTERING = false;
}


///////////////////////////////////////////////////////
// method to print info of a feature by ID:
void profile_cluster_tool::print_feature_info(void *pInstance, ELEMENT IN, bool mode){
  
  profile_cluster_tool *pThis = (profile_cluster_tool *)pInstance; 
  int ID = IN.first;
  double SCORE = IN.second;
  
  // get start
  vector<LC_MS_FEATURE>::iterator P = pThis->MASTER->get_feature_list_begin();
  // go now to this id:
  P = P + ID;
  feature* tmp = &(*P);
  
  // show info:
  if(  tmp->get_MS2_info() ){
    if( tmp->get_AC( ) == "sp|P68082|MYG_HORSE" ) {
      cout<<tmp->get_SQ( )<<": "<<SCORE<<endl;
    }
  }  
}


///////////////////////////////////////////////////
// compare profiles of 2 features, score them and add to 
// result manager:
double profile_cluster_tool::OUTSIDE_compare_profile(void *pInstance, int IN_C, void* IN){
  
  double MAIN_SCORE = 0;
  // get the instance and feature:
  profile_cluster_tool *pThis = (profile_cluster_tool *)pInstance; 
  feature* FEA = (feature *)IN;
  map<int, double > MEAN_POINTS = (*(pThis->CLUSTER_CENTERS.find(IN_C))).second; 
  profile_scorer* PRO_S = new profile_scorer( MEAN_POINTS ); 
  MAIN_SCORE = PRO_S->COMPARE_TO_TARGET_PROFILE( FEA );
  delete PRO_S;
  PRO_S = NULL;
  return MAIN_SCORE;
}


///////////////////////////////////////////////////
// help function in assigning a score for feature to 
// centroid correlation. use also ms2 information to 
// compare if a feature belongs to this cluster or not
bool profile_cluster_tool::landmark_compare_FUNCTION( int IN_C, void* IN){
  return true;
}



///////////////////////////////////////////////////
// compare profiles of 2 features, score them and add to 
// result manager:
double profile_cluster_tool::OUTSIDE_feature_compare_profile(void *pInstance, void* IN_A, void* IN_B){
  
  double MAIN_SCORE = 0;
  // get the instance and feature:
  profile_cluster_tool *pThis = (profile_cluster_tool *)pInstance; 
  // get the instance and feature:
  feature* FEA_A = (feature *)IN_A;
  feature* FEA_B = (feature *)IN_B;
  MAIN_SCORE = pThis->compare_profile( FEA_A, FEA_B );
  return MAIN_SCORE;
}

///////////////////////////////////////////////////
// compare profiles of 2 cluster, score them
double profile_cluster_tool::OUTSIDE_cluster_compare_profile(void *pInstance, int IN_A, int IN_B){
  
  double MAIN_SCORE = 0;
  // get the instance and feature:
  profile_cluster_tool *pThis = (profile_cluster_tool *)pInstance; 
  map<int, double > MEAN_POINTS_A = (*(pThis->CLUSTER_CENTERS.find(IN_A))).second; 
  map<int, double > MEAN_POINTS_B = (*(pThis->CLUSTER_CENTERS.find(IN_B))).second; 
  profile_scorer* PRO_S = new profile_scorer( MEAN_POINTS_A ); 
  MAIN_SCORE = PRO_S->COMPARE_TO_TARGET_PROFILE( &MEAN_POINTS_B );
  delete PRO_S;
  PRO_S = NULL;
  return MAIN_SCORE;
  
 }


///////////////////////////////////////////////////
// compare 2 profiles which are in form of typedef CENTER
double profile_cluster_tool::compare_profile(vector<double> P_A, CENTER P_B){
  
  double MAIN_SCORE = 0;
  profile_scorer* PRO_S = new profile_scorer( P_A ); 
  MAIN_SCORE = PRO_S->COMPARE_TO_TARGET_PROFILE( &P_B );
  delete PRO_S;
  PRO_S = NULL;
  
  return MAIN_SCORE;
}


///////////////////////////////////////////////////
// compare profiles of 2 features, score them and add to 
// result manager:
double profile_cluster_tool::compare_profile(feature* A, feature* B){
  
  double MAIN_SCORE = 0;
  profile_scorer* PRO_S = new profile_scorer( A ); 
  MAIN_SCORE = PRO_S->COMPARE_TO_TARGET_PROFILE( B );
  delete PRO_S;
  PRO_S = NULL;
  return MAIN_SCORE;
}



///////////////////////////////////////////////////////
// get a feature by ID:
void* profile_cluster_tool::get_feature_by_ID(void *pInstance, int ID){
  
  profile_cluster_tool *pThis = (profile_cluster_tool *)pInstance; 
  
  if( !pThis->RANDOM_PROFILE_CLUSTERING ){
    // get start
    vector< LC_MS_FEATURE >::iterator P = pThis->MASTER->get_feature_list_begin();
    P = P + ID;
    return &(*P);
  }
  else{
    vector<feature>::iterator P = pThis->RANDOM_PROFILE_DATA.begin();
    P = P + ID;
    return &(*P);    
  }
}



////////////////////////////////////////////////////////
// deletes a cluster centers
void profile_cluster_tool::delete_cluster_center(void* pInstance, int ID){
  
  profile_cluster_tool* pThis = (profile_cluster_tool*)pInstance; 
  
  map<int, CENTER>::iterator P = pThis->CLUSTER_CENTERS.find(ID);
  if( P != pThis->CLUSTER_CENTERS.end() ){
    pThis->CLUSTER_CENTERS.erase(P);
  }

}

////////////////////////////////////////////////////////
// insert the input master cluster profiles:
void profile_cluster_tool::insert_target_cluster_centers(){
  
  map<int, vector<double> >::iterator P = TARGET_CLUSTER_PROFILES.begin();
  while( P != TARGET_CLUSTER_PROFILES.end() ){
  
    consens_profile_builder* CONS = new consens_profile_builder( get_MAX_PROFILE_NB() );
    CENTER TMP = CONS->normalize_profile( ((*P).second ) ); 
    delete CONS;
    CONS = NULL;
    
    // add to the cluster map:
    int NEW_CLUSTER_CENTER_ID = CLUSTER_CENTERS.size();
    CLUSTER_CENTERS.insert(pair<int, CENTER> (NEW_CLUSTER_CENTER_ID, TMP));
  
    // next
    P++;
  }
}


////////////////////////////////////////////////////////
// builds up cluster centers
void profile_cluster_tool::insert_cluster_center(void* pInstance, int NEW_DATA_ID ){
  
  profile_cluster_tool* pThis = (profile_cluster_tool *)pInstance; 
  
  // create a new center with this feature (ID):
  feature* C =  (feature*)(get_feature_by_ID(pThis, NEW_DATA_ID));
  
  consens_profile_builder* CONS = new consens_profile_builder( pThis->get_MAX_PROFILE_NB() );
  CENTER TMP = CONS->build_feature_profile( C ); 
  delete CONS;
  CONS = NULL;
  
  // add to the cluster map:
  int NEW_CLUSTER_CENTER_ID = pThis->CLUSTER_CENTERS.size();
  pThis->CLUSTER_CENTERS.insert(pair<int, CENTER> (NEW_CLUSTER_CENTER_ID, TMP));
  
  // plot initialization cluster centers
  // pThis->plot_all_init_cluster();  
}

/////////////////////////////////////////////////////////
// compute a new mean center of a cluster by ID:
bool profile_cluster_tool::compute_MEAN_center(void* pInstance, int CLUSTER_ID, vector<ELEMENT>::iterator START, vector<ELEMENT>::iterator END, double CONVERGENCE_STOPP){

  profile_cluster_tool *pThis = (profile_cluster_tool *)pInstance; 

  // get this cluster center:
  map<int, CENTER >::iterator THIS_CLUSTER = pThis->CLUSTER_CENTERS.find(CLUSTER_ID);

  // old center profile:
  CENTER OLD_CENTER = (*THIS_CLUSTER).second;
  map<int, double> MEAN_PROFILE;
  map<int, int> COUNTER;
  vector< feature* > CLUSTER_MEMBERS;
  
  ////////////////////////////////////////////////////////
  // create the mean over all features in this cluster:
  while(START != END){
    
    // the feature for this ID:
    int ID = (*START).first;
    feature* C =  (feature*)(get_feature_by_ID(pThis, ID));
    CLUSTER_MEMBERS.push_back( C );
    
    // next
    START++;
  }
  

  /////////////////////////////////////////////
  // build the profile mean:  
  consens_profile_builder CONS;
  map<int, double> NEW_PROFILE = CONS.build_MEDIAN_CONSENS( &CLUSTER_MEMBERS ); 
  CLUSTER_MEMBERS.clear();
  
  // now store this profile:  
  (*THIS_CLUSTER).second = NEW_PROFILE; 
 
  return pThis->check_cluster_center_convergence( &OLD_CENTER, &NEW_PROFILE, CONVERGENCE_STOPP);
}




/////////////////////////////////////////////////////////
// compute a new center of a cluster by ID ased on MEDIAN:
bool profile_cluster_tool::compute_MEDIAN_center(void* pInstance, int CLUSTER_ID, vector<ELEMENT>::iterator START, vector<ELEMENT>::iterator END, double CONVERGENCE_STOPP){
  
  profile_cluster_tool *pThis = (profile_cluster_tool *)pInstance; 
  
  // get this cluster center:
  map<int, CENTER >::iterator THIS_CLUSTER = pThis->CLUSTER_CENTERS.find(CLUSTER_ID);
  
  // old center profile:
  CENTER OLD_CENTER = (*THIS_CLUSTER).second;
  map<int, vector<double> > PROFILE_DATA;
  
  vector< feature* > CLUSTER_MEMBERS;
  
  ////////////////////////////////////////////////////////
  // collect data from all features in this cluster:
  while(START != END){
    
    // the feature for this ID:
    int ID = (*START).first;
    feature* C =  (feature*)(get_feature_by_ID(pThis, ID));
    // store the feature:
    CLUSTER_MEMBERS.push_back( C );
    
    // next
    START++;
  }
  
  consens_profile_builder CONS;
  map<int, double> NEW_PROFILE = CONS.build_MEDIAN_CONSENS( &CLUSTER_MEMBERS ); 
  CLUSTER_MEMBERS.clear();
  
  // now store this profile:  
  (*THIS_CLUSTER).second = NEW_PROFILE; 

  // compute if the centers have converged:
  return pThis->check_cluster_center_convergence( &OLD_CENTER, &NEW_PROFILE, CONVERGENCE_STOPP);
  
}


//////////////////////////////////////////////////////////////////////////
// check if the center has converged:
bool profile_cluster_tool::check_cluster_center_convergence( CENTER* OLD, CENTER* NEW, double CONV){

  double TOT = 0;
  CENTER::iterator A = OLD->begin();
  CENTER::iterator B = NEW->begin();
  while( (A != OLD->end()) && (B != NEW->end()) ){
    TOT += fabs( (*A).second - (*B).second );
    A++;
    B++;
  }
  
  OLD->clear();
  NEW->clear();

  // check if the difference is smaller than CONV:
  if( TOT <= CONV){
    return true;
  }
  else{
    return false;
  }
}


//////////////////////////////////////////////////////////////////////////
// prints the cluster proilfe
void profile_cluster_tool::print_cluster_profile( KMEAN_main_cluster* IN, int CLUSTER_ID){
  
  // get this cluster by ID:
  map<int, CENTER>::iterator P = CLUSTER_CENTERS.find(CLUSTER_ID);
  CENTER::iterator PROFILE = (*P).second.begin();
  
 
  while( PROFILE != (*P).second.end()){
    double value = (*PROFILE).second;
    int data_point_ID = (*PROFILE).first;
    int data_point_count = 0;
    
    // check how many data points for each ID:
    vector<ELEMENT>::iterator S = IN->get_END_CLUSTERS_element_start(CLUSTER_ID);
    while( S != IN->get_END_CLUSTERS_element_end( CLUSTER_ID ) ){
      
      feature* tmp = (feature*)get_feature_by_ID( this, (*S).first );
      // check now this feature if has this data point:
      if( tmp->check_match_by_id( data_point_ID) ){
        data_point_count++;
      }
      S++; 
    }
    printf("%f\t", value);
    //printf("%0.2f(%d)\t", value, data_point_count);
    PROFILE++;
  }
  cout<<endl;  
}

//////////////////////////////////////////////////////////////////////////
// prints the cluster proilfe
void profile_cluster_tool::print_cluster_profile(int CLUSTER_ID){
  
  // print ID:
  // printf("%d:: ", CLUSTER_ID);
  
  // get this cluster by ID:
  map<int, CENTER>::iterator P = CLUSTER_CENTERS.find(CLUSTER_ID);
  CENTER::iterator PROFILE = (*P).second.begin();
  
  while( PROFILE != (*P).second.end()){
    printf("%f\t", (*PROFILE).second);
    PROFILE++;
  }
  cout<<endl;  
}


//////////////////////////////////////////////////////////////////////////
// create profile groups from every cluster:
void profile_cluster_tool::transform_CLUSTERS_to_PROFILE_GROUPS( KMEAN_main_cluster* IN){

  ////////////////////////////////////
  // print the clustering overview:
  IN->print_clustering_overview();
  
  ///////////////////////////////////
  // build peptide profile cluster objects:
  OUT_CLUSTERS.clear();
  
  // loop through all end clusters:
  int counter = 0;
  map<int, vector<ELEMENT> >::iterator P = IN->get_END_CLUSTERS_START();
  while( P != IN->get_END_CLUSTERS_END() ){
    
    int cluster_ID = (*P).first;
    map<int, CENTER>::iterator C_M = CLUSTER_CENTERS.find( cluster_ID );  

    vector<ELEMENT>::iterator START = IN->get_END_CLUSTERS_element_start(cluster_ID);      
    vector<ELEMENT>::iterator END = IN->get_END_CLUSTERS_element_end(cluster_ID);  

    // make an object and set the consensu profile:
    profile_group* tmp = new profile_group( cluster_ID );
    tmp->set_PROFILE_GROUP_ID( counter );
    tmp->set_MASTER_ID( MASTER->get_MASTER_ID() );
    tmp->set_CLUSTER_PROFILE( (*C_M).second );
    
    while( START != END ){
      feature* F = (feature*)get_feature_by_ID( this, (*START).first );
      tmp->add_cluster_member( F );
      F = NULL;
      START++;
    }
  
    // insert:
    OUT_CLUSTERS.push_back( *tmp );
    tmp = NULL;
    P++;
    counter++;
  }
  
  IN = NULL;
}


//////////////////////////////////////////////////////////////////////////
// prints the feature proilfe
void profile_cluster_tool::print_feature_profile(feature* IN, int TARGET_ID){
 
  map< int, vector<double> >::iterator F = TARGET_CLUSTER_PROFILES.find( TARGET_ID );
  int size = (*F).second.size();
  vector<double> points;
  double tot = 0;
  for( int ID = 0; ID < size; ID++){
    double data = 0;
    if( IN->get_feature_ID() == ID ){
      data = IN->get_peak_area(); 
    }
    else{    
      map<int,feature>::iterator P = IN->find_match_by_id(ID);
      if( P != IN->get_match_list_end() ){
        data = (*P).second.get_peak_area();
      }
    }
    // insert:
    tot += data;
    points.push_back(data);
  }
  
  vector<double>::iterator X = points.begin();
  while( X != points.end() ){
    //printf("%f\t", (*X) / tot);
    printf("%f\t", (*X) / tot);
    X++;
  }
  printf("\n");
  points.clear();
}


/////////////////////////////////////////////////////////////////////////
// analyze the profile groups:
// set the stdev of 
void profile_cluster_tool::analyze_profile_groups(){
  
  vector<profile_group>::iterator P = OUT_CLUSTERS.begin();
  while(P != OUT_CLUSTERS.end()){
    profile_group* PRO = &(*P);
    map<int, double> CONS = PRO->get_CLUSTER_PROFILE();
    
    // now build the stdev from the consens:
    map<int, double> STDEV;
    vector< feature*>::iterator FEA = PRO->get_MEMBER_LIST_start();
    while( FEA != PRO->get_MEMBER_LIST_end() ){
      feature* THIS = (*FEA);

      map<int, double>::iterator S = CONS.begin();  
      double TOT_AREA = 0;
      double TOT_AREA_MEAN = 0;
      vector<double> DATA;
      while( S != CONS.end() ){
        double data = 0.0;
        
        if( (*S).first == THIS->get_spectrum_ID() ){
          data = THIS->get_peak_area();
        }
        else{
          map<int,feature>::iterator F = THIS->find_match_by_id( (*S).first );
          if( F != THIS->get_match_list_end() ){
            data = (*F).second.get_peak_area();
          }
        }
        TOT_AREA += data;
        TOT_AREA_MEAN += (*S).second;
        DATA.push_back(data);
        S++;
      }
      
      vector<double>::iterator D = DATA.begin();
      S = CONS.begin();  
      while( D != DATA.end() ){
        double delta = ((*S).second/TOT_AREA_MEAN) - ( (*D) / TOT_AREA );
        delta *= delta;
        
        map<int, double>::iterator ST = STDEV.find( (*S).first);
        if( ST != STDEV.end() ){
          (*ST).second += delta; 
        }
        else{
          STDEV.insert( make_pair( (*S).first, delta ) );
        }
      
        S++;
        D++;
      }
      
      FEA++;
    }

    // build the stedv and sum them:
    map<int, double>::iterator ST = STDEV.begin();
    double OUT_STD = 0;
    while( ST != STDEV.end() ){
      (*ST).second /= double(PRO->get_NB_MEMBERS());
      OUT_STD += (*ST).second;
      ST++;
    }
    
    OUT_STD = sqrt( OUT_STD );
    OUT_STD /= double( STDEV.size() );
    
    // save the STDEV:
    PRO->set_CLUSTER_STDEV( OUT_STD );
    
    
    P++;
  }
  
}


/////////////////////////////////////////////////////////////////////////
// plot all build cluser during the clustering:
void profile_cluster_tool::plot_all_end_cluster(){
  
  char outfile[255];
  sprintf( outfile, "KMEANS_cluster_profiles");
  string tmp = outfile;
  data_plotter* PLOT = new data_plotter(tmp);
  
  // now the cluster profiles:
  vector<profile_group>::iterator P = OUT_CLUSTERS.begin();
  while(P != OUT_CLUSTERS.end()){
    char buffer[255];
    sprintf( buffer, "%d::%0.3f::%d", (*P).get_CLUSTER_ID(), (*P).get_CLUSTER_STDEV(), (*P).get_NB_MEMBERS());  
    map< int, double > tmp = (*P).get_CLUSTER_PROFILE();
    PLOT->add_plot_data( &tmp, buffer );
    P++;
  }

  PLOT->define_MAX_Y();
  PLOT->set_POINT_SIZE( 2 );
  PLOT->print_TWOD_data_to_TXT();
  PLOT->plot_TWOD_data();
  delete PLOT;
  PLOT = NULL;

}


/////////////////////////////////////////////////////////////////////////
// plot all initit clusters before the clustering start:
void profile_cluster_tool::plot_all_init_cluster(){
  
  char outfile[255];
  sprintf( outfile, "init_KMEANS_cluster_profiles");
  string tmp = outfile;
  data_plotter* PLOT = new data_plotter(tmp);
  
   
  // now the cluster profiles:
  map<int, CENTER >::iterator P = CLUSTER_CENTERS.begin();
  while(P != CLUSTER_CENTERS.end()){
    char buffer[255];
    sprintf( buffer, "%d", (*P).first);  
    PLOT->add_plot_data( &(*P).second, buffer );
    P++;
  }
  
  PLOT->define_MAX_Y();
  PLOT->set_POINT_SIZE( 2 );
  PLOT->plot_TWOD_data();
  PLOT->print_MATRIX_data_to_TXT();
  delete PLOT;
  PLOT = NULL;
  
}



//////////////////////////////////////////////////
// write all profile_cluster to XML
void profile_cluster_tool::store_END_CLUSTERS_to_XML(){
  
  
  
  char file_name[255];
  sprintf( file_name, "KMEANS_cluster_groups.xml");
  char buffer[254];
    
  string target_dir = "ANALYSIS_";
  
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
  target_dir = target_dir + file_name;

  
  //////////////////////////////////
  // print the header:
  LC_MS_XML_writer* LC_W = new LC_MS_XML_writer( target_dir );
  LC_W->write_unformatted_XML_MAIN_HEADER();
  
  ofstream* WRITER = LC_W->get_WRITER();
  
  sprintf(buffer,"<CLUSTERING_OUTPUT type=\"k-means\">\n");  
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"<PROFILE_GROUPS nb_profile_groups=\"%d\">\n", OUT_CLUSTERS.size());  
  WRITER->write(buffer,strlen(buffer));
  
  // write the profile groups to XML:
  vector<profile_group>::iterator PRO_G = OUT_CLUSTERS.begin();
  while( PRO_G != OUT_CLUSTERS.end() ){
    // print content of the profile group:
    (*PRO_G).write_to_XML_file( LC_W ); 
    PRO_G++;
  }
  
  sprintf(buffer,"</PROFILE_GROUPS>\n");  
  WRITER->write(buffer,strlen(buffer));
  sprintf(buffer,"</CLUSTERING_OUTPUT>\n");  
  WRITER->write(buffer,strlen(buffer));
  LC_W->write_XML_MAIN_HEADER_CLOSE();
  
  delete LC_W;
  LC_W = NULL;
  
}


/////////////////////////////////////////////////////////////
// all following functions to build a random features
// profile data pool:
void profile_cluster_tool::build_random_feature_profiles( ){
  
  srand( time(NULL) );
  RANDOM_PROFILE_DATA.clear();
  int RAN_DATA_SIZE = ID_DATA.size();
  ID_DATA.clear();
  int count = 0;
  while( count <= RAN_DATA_SIZE ){
    // get a random profile:
    // create a random profile
    insert_random_feature_profile( );
    count++;
  }
  
  RANDOM_PROFILE_CLUSTERING = true;
}


/////////////////////////////////////////////////////////////
// create a random profile
void profile_cluster_tool::insert_random_feature_profile( ){
  
  feature* RAND_F = NULL;
  double MAX_A = 1.0;
  double MIN_A = 0.0;
  int MIN_ID = 0;
  int MAX_ID = MASTER->get_nb_raw_specs();
  int MAX_PROFILE_POINTS = MASTER->get_nb_raw_specs() + 1;
  int MIN_PROFILE_POINTS = MIN_REPLICATE_NB;

  // determine how many random profile points
  int nb_profile_points = MIN_PROFILE_POINTS + rand() % ( MAX_PROFILE_POINTS - MIN_PROFILE_POINTS );
  
  // build a feature profile:
  int i = 0;
  while( i < nb_profile_points ){

    // draw a random id:
    int ran_ID = MIN_ID + rand() % ( MAX_ID - MIN_ID );
    // random area:
    double AREA = MIN_A + rand() * (MAX_A - MIN_A) / RAND_MAX;
    if( RAND_F == NULL ){
      RAND_F = new feature();
      RAND_F->set_peak_area( AREA);
      RAND_F->set_spectrum_ID( ran_ID );
      
    }
    else{
      // check that the id is not duplicated:
      while( RAND_F->check_match_by_id( ran_ID)  ){
        ran_ID = MIN_ID + rand() % ( MAX_ID - MIN_ID );
      }
      
      feature* TMP = new feature();
      TMP->set_peak_area( AREA);
      TMP->set_spectrum_ID( ran_ID );
      RAND_F->add_matched_feature( TMP );
      TMP = NULL;
    }
    i++;
  }
 
  if( RAND_F ){
    RAND_F->set_feature_ID( RANDOM_PROFILE_DATA.size() );
    ID_DATA.push_back( RANDOM_PROFILE_DATA.size() );
    RANDOM_PROFILE_DATA.push_back( RAND_F );
    RAND_F = NULL;
  }
}
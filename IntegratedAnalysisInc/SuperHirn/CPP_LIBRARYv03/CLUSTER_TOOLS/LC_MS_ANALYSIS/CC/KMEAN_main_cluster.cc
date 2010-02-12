///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS KMEAN_main_cluster:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_KMEAN_MAIN_CLUSTER
#include "CLUSTER_LC_MS_ANALYSIS_USE.h"



////////////////////////////////////////////////
// constructor for the object KMEAN_main_cluster:
KMEAN_main_cluster::KMEAN_main_cluster(void *pinstance){
  
  DATA_MANAGER = NULL;
  Pget_object_by_ID = NULL;
  Pprint_info_by_ID = NULL;
  Pinsert_cluster_center = NULL;
  Pcompute_new_center = NULL;
  Pget_object_comparison = NULL; 
  Pget_feature_object_comparison = NULL; 
  
  SSQ_ERROR = 0;
  srand( time(NULL) );
 
  // host pointer
  DATA_MANAGER = pinstance;
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  
  // convergence stopp:
  def->search_tag("clustering convergence stopp", &CONVERGENCE_STOPP);    
  // number or clusters
  def->search_tag("number of clusters", &PERC_CLUSTERS);
  // max. number of iterations:
  def->search_tag("max. nb. of iterations", &MAX_NB_ITERATIONS);
  // max. number of iterations:
  def->search_tag("min. nb. of cluster members", &MIN_NB_CLUSTER_MEMBERS);
  // global cluster modus:
  def->search_tag("global cluster modus", &GLOBAL_SCORING_SCHEMA);
  
  delete def;
  def = NULL;
}

//////////////////////////////////////////////////
// class desctructor of KMEAN_main_cluster
KMEAN_main_cluster::~KMEAN_main_cluster(){
  DATA_MANAGER = NULL;
  K_MEANS.clear();
  ORDERED_END_CLUSTERS.clear();
  ID_EXCLUSION_LIST.clear();
}

//////////////////////////////////////////////////
// class copy constructor of KMEAN_main_cluster
KMEAN_main_cluster::KMEAN_main_cluster(const KMEAN_main_cluster& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of KMEAN_main_cluster
KMEAN_main_cluster::KMEAN_main_cluster(const KMEAN_main_cluster* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
KMEAN_main_cluster& KMEAN_main_cluster::operator=(const KMEAN_main_cluster& tmp){
  return *this;
}
    
//////////////////////////////////////////////////
// comparison of 2 cluster centers
double KMEAN_main_cluster::compare_cluster_elements(int A, int B){
  return Pget_cluster_object_comparison(DATA_MANAGER, A, B );
}

//////////////////////////////////////////////////
// compare cluster vs. element:
double KMEAN_main_cluster::compare_elements(int C, ELEMENT IN){
  return Pget_object_comparison(DATA_MANAGER, C, get_Object_by_ID( IN.first ) );
}

//////////////////////////////////////////////////
// compare 2 elements with each other:
double KMEAN_main_cluster::compare_elements(ELEMENT IN_A, ELEMENT IN_B){
  return Pget_feature_object_comparison(DATA_MANAGER, get_Object_by_ID( IN_A.first ), get_Object_by_ID( IN_B.first ) );
}

//////////////////////////////////////////////////
// compare cluster vs. element:
double KMEAN_main_cluster::compare_elements(int C, void* ELE){
  return Pget_object_comparison(DATA_MANAGER, C, ELE);
}

//////////////////////////////////////////////////
// get the object by id:
void* KMEAN_main_cluster::get_Object_by_ID(int A){
  // call the function of the host to get a score by ID 
  return Pget_object_by_ID(DATA_MANAGER, A);
}

//////////////////////////////////////////////////
// print info from a cluster object:
void KMEAN_main_cluster::print_info_by_ID(ELEMENT P, bool mode){
  // call the function of the host to get a score by ID 
  Pprint_info_by_ID(DATA_MANAGER, P, mode);
}


//////////////////////////////////////////////////
// print info from a cluster object:
void KMEAN_main_cluster::delete_cluster_center(int ID){
  
  map< int, vector<ELEMENT> >::iterator P = K_MEANS.find(ID);
  if( P != K_MEANS.end() ){
    Pdelete_cluster_center(DATA_MANAGER, ID);
    K_MEANS.erase(P);
  }
}


/////////////////////////////////////////////////////
// randomized access of the data:
int KMEAN_main_cluster::get_random_DATA(){
  
  if( !DATA.empty() ){
    
    int max = DATA.size();
    int pos = rand()%max;
    vector<int>::iterator p = DATA.begin();
    p += pos;
    if( p != DATA.end() ){
      return (*p);
    }
    else{
      printf("\t*ERROR:: random data access exceeds data array (size=%d) at pos %d *\n", DATA.size(), pos );
      return 0;
    }
  }
  else{
    return 0;
  }
}


//////////////////////////////////////////////////////
// start the clustering process:
void KMEAN_main_cluster::start_clustering(){
  
  // initialize:
  init_clustering();
  
  // do k-means clustering:
  start_K_MEANS_clustering();
      
}


//////////////////////////////////////////////////////
// init clustering:
void KMEAN_main_cluster::init_clustering(){
  
  NB_SAMPLES = DATA.size();
  ID_EXCLUSION_LIST.clear();
  END_CLUSTERS.clear();
  DELETED_CLUSTERS_PER_CYCLE = 0;
  K_MEANS.clear();
  SSQ_ERROR = 0;
  int ID = 0;
  vector< vector<double> > tmp;
  
  
  //////////////////////////////////////////////////////
  // check if there are any target start clusters defined in the param.def file:
  /*
  read_param* def = new read_param();
  // get the input cluster profiles:
  def->search_tag("clustering input profiles", &tmp);  
  delete def;
  def = NULL;
  tmp.clear();
  vector< vector<double> >::iterator P = tmp.begin();
  while(P != tmp.end()){
    vector<ELEMENT> TMP;
    K_MEANS.insert(pair<int, vector<ELEMENT> >(ID, TMP));
    ID_EXCLUSION_LIST.insert(pair<int, int>( ID, 0));
    ID++;
    P++;
  }
  */
  
  //////////////////////////////////////////////////////
  // insert the random clusters:
  if( PERC_CLUSTERS < 1.0 ){
    NB_CLUSTERS = int( double(NB_SAMPLES) * PERC_CLUSTERS );
  }
  else{
    NB_CLUSTERS = int( PERC_CLUSTERS );
  }
  
  // set random start means:
  while( ID < NB_CLUSTERS){
    // get random ID
    int ran_C = get_random_DATA();
    
    // check that this one has not yet been choosen:
    map<int, vector<ELEMENT> >::iterator Q = K_MEANS.find(ran_C);
    while( Q != K_MEANS.end() ){
      ran_C = get_random_DATA();
      Q = K_MEANS.find(ran_C);
    }
    // check that its not in the exclusion list:
    map<int, int >::iterator E = ID_EXCLUSION_LIST.find(ran_C);
    while( E != ID_EXCLUSION_LIST.end() ){
      ran_C = get_random_DATA();
      E = ID_EXCLUSION_LIST.find(ran_C);
    }
   
    // insert new cluster:
    Pinsert_cluster_center(DATA_MANAGER, ran_C);
    // build up this cluster array:
    vector<ELEMENT> TMP;
    K_MEANS.insert(pair<int, vector<ELEMENT> >(ID, TMP));
    // insert also into the exclusion list, so this center
    // will not be choosen again:
    ID_EXCLUSION_LIST.insert(pair<int, int>( ID, 0));

    // printf("%d(ID=%d):", ID, ran_C);
    // print_info_by_ID(pair<int, double>( ran_C, 0.0), true);
    ID++;
  }
}
  

//////////////////////////////////////////////////////
// K-MEANS clustering:
void KMEAN_main_cluster::start_K_MEANS_clustering(){
  
  progress_bar bar(MAX_NB_ITERATIONS , "done");
  
  NB_IT = 0;
  while( !(K_MEANS.empty()) && !(DATA.empty()) ){

    // associates every data point to one of the k cluster
    // by their arithmetic distance from the center of the clusters
    associate_DATA_to_centers();
    
    // check each cluster:
    check_cluster_content();
    
    // now recompute the centers of the k clusters:
    build_new_centers();
    
    // progress bar:
    bar.update_progress();
    NB_IT++;
    
    if(NB_IT > ( MAX_NB_ITERATIONS - 1) ){
      break;
    }
  }
  
  
  // associate now a last time the data to the remaining centers:
  save_remaing_data_points( );
    
  // remove clusters with no members:
  remove_empty_clusters();

}


/////////////////////////////////////////////////////////
// sets every point to the closest cluster center:
void KMEAN_main_cluster::associate_DATA_to_centers(){
    
  // put every data point into a center
  vector<int>::iterator CLUSTER_OBJECT = DATA.begin();
  while( CLUSTER_OBJECT != DATA.end()){
    
    //////////////////////////////////////
    // add this data id to the nearest center:
    int CLUSTER_OBJECT_ID =  (*CLUSTER_OBJECT);
    
    // find the closest centroid for this element:
    map<int, vector<ELEMENT> >::iterator BEST_ELEMENT = find_closest_centroid( CLUSTER_OBJECT_ID );
    
    // was not associated to any center;
    // -> remove it:
    if( BEST_ELEMENT == K_MEANS.end()){
      CLUSTER_OBJECT = DATA.erase( CLUSTER_OBJECT );
      NB_SAMPLES--;
    }
    else{      
      // now add it to this closest center:
      ELEMENT tmp = make_pair( CLUSTER_OBJECT_ID, 0.00);
      (*BEST_ELEMENT).second.push_back(tmp);
      
      // print_info_by_ID( make_pair( *CLUSTER_OBJECT , (*BEST_ELEMENT).first ), true );
      // next cluster data object
      CLUSTER_OBJECT++;
    }
  }
  
}

////////////////////////////////////////////////////////
// this function finds the closest cluster for a given object
map<int, vector<ELEMENT> >::iterator KMEAN_main_cluster::find_closest_centroid( int CLUSTER_OBJECT_ID ){
  return find_closest_centroid( CLUSTER_OBJECT_ID, &K_MEANS);
}


////////////////////////////////////////////////////////
// this function finds the closest cluster for a given object
map<int, vector<ELEMENT> >::iterator KMEAN_main_cluster::find_closest_centroid( int CLUSTER_OBJECT_ID, map<int, vector<ELEMENT> >* CENTROIDS ){
  
  void* ELM =  get_Object_by_ID( CLUSTER_OBJECT_ID );
  
  // do first the local scoring for every cluster
  // -> local score which should be small:
  int BEST_LOCAL_ID = -1;
  double BEST_LOCAL_SCORE = 1000000.00;
  
  // save the sum of all scores:
  double SUM_OF_SCORES = 0;
  map<int, double> ALL_SCORES;
  
  // go through all clusters and define the association score:
  map<int, vector<ELEMENT> >::iterator X = CENTROIDS->begin();
  while( X != CENTROIDS->end() ){
    
    // get the comparison of this feature
    // to the current center:
    double this_score = compare_elements( (*X).first, ELM );
    ALL_SCORES.insert(pair<int, double>( (*X).first, this_score));
    SUM_OF_SCORES += this_score;
    
    // check if best score and store it:
    if( BEST_LOCAL_SCORE > this_score){
      BEST_LOCAL_SCORE = this_score;
      BEST_LOCAL_ID = (*X).first;
    }
    
    // check next cluster: 
    X++;
  }
  
  
  ////////////////////////////////////////////////////
  // combine now the local 
  // scores with the global scoring
  // to all clusters:
  // -> the global score should be big, so that the best
  // center is far away from all other cluster centers:
  double BEST_GLOBAL_SCORE = (-1.0) * SUM_OF_SCORES;
  int BEST_GLOBAL_ID = -1;
  
  if( GLOBAL_SCORING_SCHEMA ) {
    
    double AVERAGE_LOCAL_SCORE = SUM_OF_SCORES / double(ALL_SCORES.size());
    map<int, double>::iterator S = ALL_SCORES.begin();		
    while( S != ALL_SCORES.end() ){
      
      double this_score = ( 1.0 - (*S).second ) + AVERAGE_LOCAL_SCORE; 
      
      if( BEST_GLOBAL_SCORE < this_score){
        BEST_GLOBAL_SCORE = this_score;
        BEST_GLOBAL_ID = (*S).first;
      }
      
      // next
      S++;
    }
  }
  
  
  ///////////////////////////////////////////////////////
  // ok, here we should have the best cluster by its ID:
  map<int, vector<ELEMENT> >::iterator BEST_ELEMENT;
  double BEST_SCORE = 0;
  
  if( GLOBAL_SCORING_SCHEMA ){
    BEST_ELEMENT = CENTROIDS->find( BEST_GLOBAL_ID );
    BEST_SCORE = BEST_GLOBAL_SCORE;
  }
  else{
    BEST_ELEMENT = CENTROIDS->find( BEST_LOCAL_ID );
    BEST_SCORE = BEST_LOCAL_SCORE;	
  }
  
  return BEST_ELEMENT;
}


/////////////////////////////////////////////////////////
// checks each cluster, if less then min. cluster members
// -> through the cluster away and randomly choose a new
// one:
void KMEAN_main_cluster::check_cluster_content(){
  
  int delete_count = 0;
  map<int, vector<ELEMENT> >::iterator X = K_MEANS.begin();
  while( X != K_MEANS.end() ){
    
    bool CHECK = true;
    int C_size = (*X).second.size();
    int this_ID = (*X).first; 
    
    // check if the cluster has enough members:
    if(  C_size <  MIN_NB_CLUSTER_MEMBERS ){
      CHECK = false;
    }
    
    /////////////////////////
    // cluster has not passed this filter:
    // -> remove it!
    if( !CHECK ){
      // delete old one:
      delete_cluster_center(this_ID);
      delete_count++;
    }
    
    // next cluster to check:
    X++;
  } 
  
  // replace_deleted_cluster_center( delete_count );
}


/////////////////////////////////////////////////////////
// fills up with new a cluster centers for onw which was removed:
void KMEAN_main_cluster::replace_deleted_cluster_center(int COUNT){

  int i = 0;
  while ( COUNT > i ){
    
    // get new random ID:
    int NEW_DATA_ID = get_random_DATA();
    int NEW_CLUSTER_ID = K_MEANS.size();
    
    // check that its not in the exclusion list:
    map<int, int >::iterator E = ID_EXCLUSION_LIST.find(NEW_DATA_ID);
    while( E != ID_EXCLUSION_LIST.end() ){
      NEW_DATA_ID = get_random_DATA();
      E = ID_EXCLUSION_LIST.find(NEW_DATA_ID);
    }
    
    // insert new one:      
    Pinsert_cluster_center(DATA_MANAGER, NEW_DATA_ID);
    // build up this cluster array:
    vector<ELEMENT> TMP;
    K_MEANS.insert(pair<int, vector<ELEMENT> >( NEW_CLUSTER_ID, TMP));
    
    i++;
  }
}

/////////////////////////////////////////////////////////
// build the new clusters centers:
void KMEAN_main_cluster::build_new_centers(){
 
  map<int, vector<ELEMENT> >::iterator X = K_MEANS.begin();
  map<int, vector<ELEMENT> > NEW_K_MEANS;
  while( X != K_MEANS.end() ){
    
    int cluster_ID = (*X).first;
    vector<ELEMENT>::iterator CLUSTER_DATA_START = (*X).second.begin();
    vector<ELEMENT>::iterator CLUSTER_DATA_END = (*X).second.end();
    
    // build a new mean center of this cluster;
    // and check if the old and new centers
    // have not changed:
    bool convergence_check = false;
    if( (*X).second.empty() ){
      convergence_check = false;
    }
    else{
      convergence_check = Pcompute_new_center( DATA_MANAGER, cluster_ID, CLUSTER_DATA_START, CLUSTER_DATA_END, CONVERGENCE_STOPP );
    }
    
    if( !convergence_check ){     
      vector<ELEMENT> TMP;
      NEW_K_MEANS.insert(pair<int, vector<ELEMENT> >(cluster_ID, TMP));
    }
    else{
      // save this cluster in the end cluster vector:
      save_end_cluster(X);      
    }
    
    X++;
  }
  
  // delete old one:
  K_MEANS.clear();
  // save the updated one:
  K_MEANS = NEW_K_MEANS;
  NEW_K_MEANS.clear();  
}
  
//////////////////////////////////////////////////////
// save the input cluster in the end clusters:
void KMEAN_main_cluster::save_end_cluster(map<int, vector<ELEMENT> >::iterator THIS){
  
  END_CLUSTERS.insert(pair<int, vector<ELEMENT> >( (*THIS).first, (*THIS).second));
  
  // remove the clustered elements from the data array
  vector<ELEMENT>::iterator P = (*THIS).second.begin();
  while( P != (*THIS).second.end() ) {
    vector<int>::iterator c = find( DATA.begin(), DATA.end(), (*P).first );
    DATA.erase(c);
    P++;
  }
  
}



//////////////////////////////////////////////////////
// remove empty clusters:
void KMEAN_main_cluster:: remove_empty_clusters(){

  map<int, vector<ELEMENT> >::iterator P = END_CLUSTERS.begin();
  while(P != END_CLUSTERS.end()){
    if( int( (*P).second.size() ) < MIN_NB_CLUSTER_MEMBERS){
      delete_cluster_center( (*P).first );
      END_CLUSTERS.erase(P);
    }
    P++;
  }
}



//////////////////////////////////////////////////////
// prints out the end cluster:
void KMEAN_main_cluster::print_out_end_cluster(int ID){
  
  // print cluster information:  
  map<int, vector<ELEMENT> >::iterator P = END_CLUSTERS.find(ID);
  
  // check if it contains any elements:
  if( P != END_CLUSTERS.end()){
    
    if( (*P).second.size() > 0){
      
      printf("\ncluster #%d, %d members:\n",(*P).first,(*P).second.size());
      
      vector<ELEMENT>::iterator X = (*P).second.begin();
      double average_score = 0;
      // print only the ids:
      while( X != (*P).second.end() ){
        average_score += (*X).second;
        print_info_by_ID((*X), false);
        X++;
      }
      average_score /= (*P).second.size();
      printf("AVERAGE CLUSTER SCORE: %f\n",average_score);
    }
  }
}

//////////////////////////////////////////////////////
// prints out the end cluster:
void KMEAN_main_cluster::print_out_ordered_end_cluster(){
  
  map<double, int>::iterator P = ORDERED_END_CLUSTERS.begin();
  while( P != ORDERED_END_CLUSTERS.end() ){
  
    // print cluster information:  
    map<int, vector<ELEMENT> >::iterator C = END_CLUSTERS.find( (*P).second );
  
    // check if it contains any elements:
    if( C != END_CLUSTERS.end()){
    
      if( (*C).second.size() > 0){
        printf("\ncluster #%d, %d members:\n",(*C).first,(*C).second.size());
      
        vector<ELEMENT>::iterator X = (*C).second.begin();
        // print only the ids:
        while( X != (*C).second.end() ){
          print_info_by_ID((*X), false);
          X++;
        }
        printf("AVERAGE CLUSTER SCORE: %f\n", (*P).first);
      }
    }
    
    P++;
  }
}


//////////////////////////////////////////////////////
// prints out the cluster overview
void KMEAN_main_cluster::print_clustering_overview(){
  
  printf("\n\t\t K-MEANS CLUSTERING OVERVIEW:\n");
  if( GLOBAL_SCORING_SCHEMA ){
    printf("\t\t\t# - GLOBAL SCORING USED\n");    
  }
  else{
    printf("\t\t\t# - LOCAL SCORING USED\n");    
  }
  printf("\t\t\t# input data:\t%d\n", NB_SAMPLES);
  printf("\t\t\t# nb. input random clusters:\t%d (%f)\n", NB_CLUSTERS, PERC_CLUSTERS);
  printf("\t\t\t# min. nb. cluster members:\t%d\n", MIN_NB_CLUSTER_MEMBERS);
  printf("\t\t\t# nb. end clusters:\t%d\n", END_CLUSTERS.size());
  printf("\t\t\t# iterations:\t%d (max=%d)\n", NB_IT, MAX_NB_ITERATIONS);
  printf("\t\t\t# delta convergence:\t%E\n", CONVERGENCE_STOPP);
  printf("\n");
} 


//////////////////////////////////////////////////////////
// reorders the clusters according to their average score
void KMEAN_main_cluster::order_clusters(){
  
  // print cluster information:  
  map<int, vector<ELEMENT> >::iterator P = END_CLUSTERS.begin();
  
  // check if it contains any elements:
  while( P != END_CLUSTERS.end()){
    
    vector<ELEMENT>::iterator X = (*P).second.begin();
    double average_score = 0;
    // print only the ids:
    while( X != (*P).second.end() ){
      average_score += (*X).second;
      X++;
    }
    average_score /= (*P).second.size();
  
    // insert into the map:
    ORDERED_END_CLUSTERS.insert(pair<double, int>(average_score, (*P).first));
    
    P++;
  }
}

//////////////////////////////////////////////////////////
// put the remaing DATA points at the end of the clustering
// into the best end clusters:
void KMEAN_main_cluster::save_remaing_data_points( ){
  
  // print clusters, which have not yet converged:
  map<int, vector<ELEMENT> >::iterator P = K_MEANS.begin();
  while( P != K_MEANS.end() ){
    printf("\t\t*Cluster %d has not converged*\n", (*P).first );    
    P++;
  }    

  if( !DATA.empty() ){
  
    printf("\t\t- add non clustered data points (%d) to end clusters\n", DATA.size() );  
    
    // put the remaining data points into the end clusters
    vector<int>::iterator CLUSTER_OBJECT = DATA.begin();
    while( CLUSTER_OBJECT != DATA.end()){
      
      //////////////////////////////////////
      // add this data id to the nearest center:
      int CLUSTER_OBJECT_ID =  (*CLUSTER_OBJECT);
      
      // find the closest centroid for this element:
      map<int, vector<ELEMENT> >::iterator BEST_ELEMENT = find_closest_centroid( CLUSTER_OBJECT_ID, &END_CLUSTERS );
      
      // was not associated to any center;
      // -> remove it:
      if( BEST_ELEMENT == END_CLUSTERS.end()){
        CLUSTER_OBJECT = DATA.erase( CLUSTER_OBJECT );
        NB_SAMPLES--;
      }
      else{      
        // now add it to this closest center:
        ELEMENT tmp = make_pair( CLUSTER_OBJECT_ID, 0.00);
        (*BEST_ELEMENT).second.push_back(tmp);
        
        // print_info_by_ID( make_pair( *CLUSTER_OBJECT , (*BEST_ELEMENT).first ), true );
        // next cluster data object
        CLUSTER_OBJECT++;
      }
    }
  }

}



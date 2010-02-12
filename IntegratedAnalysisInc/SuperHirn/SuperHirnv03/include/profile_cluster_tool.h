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


#ifndef PROFILE_CLUSTER_TOOL_H
#define PROFILE_CLUSTER_TOOL_H

////////////////////////
// structur which stores a cluster center:
typedef map<int,double> CENTER;
typedef pair<int, double> ELEMENT;
/////////////////////////


class profile_cluster_tool{

    
    ////////////////////////////////////////////////
    // declaration of the private members:

private:
  

  LC_MS* MASTER;
  result_manager* RES;
  bool PRINT_CLUSTER;
  vector<int> ID_DATA;
  int MIN_REPLICATE_NB;
  int MAX_PROFILE_NB;
  
  map<int, CENTER > CLUSTER_CENTERS;
  vector<profile_group> OUT_CLUSTERS;

  // targeted profiling:
  // input target cluster profiles:
  map< int, vector<double> > TARGET_CLUSTER_PROFILES;
  // the processed out profile groups:
  vector< profile_group > TARGET_PROFILE_GROUPS;
  

  // for the random feature library:
  vector<feature> RANDOM_PROFILE_DATA;
  bool RANDOM_PROFILE_CLUSTERING;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:

    static bool TARGET_PROFILE_CLUSTER_INITIAZTION;

    // class destructor
    ~profile_cluster_tool();
  
  // class constructor
  profile_cluster_tool(LC_MS*);
  // class copy constructor
  profile_cluster_tool(const profile_cluster_tool&);
  // class copy constructor
  profile_cluster_tool(const profile_cluster_tool*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  profile_cluster_tool& operator=(const profile_cluster_tool&);
  profile_cluster_tool& operator<=(const profile_cluster_tool&);
  profile_cluster_tool& operator>=(const profile_cluster_tool&);
  profile_cluster_tool& operator<(const profile_cluster_tool&);
  profile_cluster_tool& operator>(const profile_cluster_tool&);
  
  // start to cluster the input data:
  void start_profile_clustering();
  
    
  // compare profiles of 2 features, score them and add to 
  // result manager:
  double compare_profile(feature*, feature*);
  // renumerate the features for the clustering:
  void renumerate_features_in_MASTER();
  /////////////////////////////////////////////
  
    
  
  /////////////////////////////////////////////////////    
  // start the clustering process by the K meas method:
  //
  void start_K_MEAN_clustering();
  // start the X validation, determination of optimal
  // number of start kmeans cluster
  // -> see cluster_X_scorer class
  void start_K_MEAN_X_VALIDATION();
  // start a kmeans cluster with random profiling data
  void start_RANDOM_PROFILE_clustering();
  // compare profiles of 2 features, score them
  static double OUTSIDE_compare_profile(void*, int, void*);
  // compare profiles of 2 features, score them
  static double OUTSIDE_feature_compare_profile(void*, void*, void*);
  // compare profiles of 2 cluster, score them
  static double OUTSIDE_cluster_compare_profile(void*, int, int);
  // compare 2 profiles which are in form of typedef CENTER
  double compare_profile(vector<double> , CENTER);
  // help function in assigning a score for feature to 
  // centroid correlation. use also ms2 information to 
  // compare if a feature belongs to this cluster or not
  bool landmark_compare_FUNCTION( int , void* );
  // get a feature by ID:
  static void* get_feature_by_ID(void*, int);
  // deletes a cluster centers
  static void delete_cluster_center(void* , int );
  // insert the input master cluster profiles:
  void insert_target_cluster_centers();
  // builds up cluster centers
  static void insert_cluster_center(void*, int);
  // method to copmute the similarity between 2 feature accessed by ID:
  static double compute_cluster_similarity_score(void *pInstance, int);
  // method to print info of a feature by ID:
  static void print_feature_info(void *pInstance, ELEMENT, bool);
  // compute a new mean center of a cluster by ID:
  static bool compute_MEAN_center(void* , int , vector<ELEMENT>::iterator , vector<ELEMENT>::iterator, double);
  // compute a new MEDIAN center of a cluster by ID:
  static bool compute_MEDIAN_center(void* , int , vector<ELEMENT>::iterator , vector<ELEMENT>::iterator, double);
  // compute if the centers have converged:
  bool check_cluster_center_convergence( CENTER* , CENTER* , double);
  // prints the cluster proilfe
  void print_cluster_profile( KMEAN_main_cluster*, int);
  void print_cluster_profile(int CLUSTER_ID);
  // prints the feature proilfe
  void print_feature_profile(feature*, int);

  
  // create profile groups from every cluster:
  void transform_CLUSTERS_to_PROFILE_GROUPS( KMEAN_main_cluster* );
  // plot all build cluser during the clustering:
  void plot_all_end_cluster();
  // plot all initit clusters before the clustering start:
  void plot_all_init_cluster();
  // analyze the profile groups:
  // set the stdev of 
  void analyze_profile_groups();    
  // write all profile_cluster to XML
  void store_END_CLUSTERS_to_XML();

  // get the target profiles:
  map< int, vector<double> > get_TARGET_PROFILES(){return TARGET_CLUSTER_PROFILES;};

  
  
  /////////////////////////////////////////////////////////////
  // all following functions to build a random features
  // profile data pool:
  void build_random_feature_profiles( );
  // create a random profile
  void insert_random_feature_profile( );

  
  
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  LC_MS* get_MASTER(){return MASTER;};
  
  // return nb of LC/MS childs:
  int get_nb_LC_MS_childs(){return MASTER->get_nb_raw_specs();};
  
  int get_MAX_PROFILE_NB(){ return MAX_PROFILE_NB;};
  
  // access the best matches to the target profiles:
  vector< profile_group > get_TARGET_PROFILE_GROUPS(){return TARGET_PROFILE_GROUPS;};
  
};

#endif

    

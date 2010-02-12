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



#ifndef KMEAN_MAIN_CLUSTER_H
#define KMEAN_MAIN_CLUSTER_H

typedef pair<int,double> ELEMENT;


class KMEAN_main_cluster{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
  
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:

  // clustering parameters:
  double PERC_CLUSTERS;
  int NB_CLUSTERS;
  int NB_SAMPLES;
  int NB_IT;
  int MAX_NB_ITERATIONS;
  int MIN_NB_CLUSTER_MEMBERS;
  double CONVERGENCE_STOPP;
  double SSQ_ERROR;
  bool GLOBAL_SCORING_SCHEMA;
  
  /////////////
  // this vector contains objects which are the mean
  // of the cluster centers, objects are not know, only pointers:
  map< int, vector<ELEMENT> > K_MEANS;
  
  // this vector contains the id of the final clustering
  map< int, vector<ELEMENT> > END_CLUSTERS;
  map< double, int> ORDERED_END_CLUSTERS;
  
  /////////////
  // vector containing all ids of the data to be clustered
  vector<int> DATA;
  
  ////////////
  // id exclusion list;
  map<int, int> ID_EXCLUSION_LIST;

  // tracks count of how many cluster
  // centers were deleted in a cycle
  int DELETED_CLUSTERS_PER_CYCLE;
  
  ////////////
  // instance which calls this object 
  // and which hosts the real data to be clustered
  void *DATA_MANAGER;
  
  double (*Pget_object_comparison)(void *pInstance, int A, void* B); 
  double (*Pget_feature_object_comparison)(void *pInstance, void* A, void* B); 
  double (*Pget_cluster_object_comparison)(void *pInstance, int A, int B); 
  void* (*Pget_object_by_ID)(void *pInstance, int ID); 
  void (*Pprint_info_by_ID)(void *pInstance, ELEMENT, bool mode);
  void (*Pinsert_cluster_center)(void* pInstance, int ID);
  void (*Pdelete_cluster_center)(void* pInstance, int ID);
  bool (*Pcompute_new_center)(void* pInstance, int ID, vector<ELEMENT>::iterator, vector<ELEMENT>::iterator, double);
  
public:
  
  // class destructor
  ~KMEAN_main_cluster();
  
  // class constructor
  KMEAN_main_cluster(void*);
  // class copy constructor
  KMEAN_main_cluster(const KMEAN_main_cluster&);
  // class copy constructor
  KMEAN_main_cluster(const KMEAN_main_cluster*);
  
  //////////////////////////////////////////////////
  // overload operators:
  KMEAN_main_cluster& operator=(const KMEAN_main_cluster&);
  KMEAN_main_cluster& operator<=(const KMEAN_main_cluster&);
  KMEAN_main_cluster& operator>=(const KMEAN_main_cluster&);
  KMEAN_main_cluster& operator<(const KMEAN_main_cluster&);
  KMEAN_main_cluster& operator>(const KMEAN_main_cluster&);
  
  
  /////////////////////////////////////////////////
  // start the clustering process:
  void start_clustering();
  // init clustering:
  void init_clustering();
  // K-MEANS clustering:
  void start_K_MEANS_clustering();
  // sets every point to the closest cluster center:
  void associate_DATA_to_centers();
  // this function finds the closest cluster for a given object
  map<int, vector<ELEMENT> >::iterator find_closest_centroid( int );
  map<int, vector<ELEMENT> >::iterator find_closest_centroid( int , map<int, vector<ELEMENT> >*);
  // checks each cluster, if less then min. cluster members
  // -> through the cluster away and randomly choose a new one
  void check_cluster_content();
  // build the new clusters centers cluster center:
  void build_new_centers();
  // save the input cluster in the end clusters:
  void save_end_cluster(map<int, vector<ELEMENT> >::iterator);
  // access the parameters of the data:
  double compare_elements(int, void*);
  double compare_elements(int , ELEMENT);
  // compare 2 elements with each other:
  double compare_elements( ELEMENT , ELEMENT );
  // comparison of 2 cluster centers
  double compare_cluster_elements(int , int );
  // remove empty clusters:
  void remove_empty_clusters();
  // print info from a cluster object:
  void delete_cluster_center(int);
  // fills up with new a cluster centers for onw which was removed:
  void replace_deleted_cluster_center(int);
  // reorders the clusters according to their average score
  void order_clusters();
  // put the remaing DATA points at the end of the clustering
  // into the best end clusters:
  void save_remaing_data_points( );
    

  
  // prints out the cluster overview
  void print_clustering_overview();
  // prints out the end clusters:
  void print_out_end_cluster(int);
  void print_out_ordered_end_cluster();
  // print info from a cluster object:
  void print_info_by_ID(ELEMENT, bool);

  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // access teh K_MEANS:
  //double get_K_MEAN(int k){return (*( K_MEANS.find(k) )).second;};
  //void set_K_MEAN(int k, double mean){ (*( K_MEANS.find(k) )).second = mean; };
  
  // nb clusters:
  int get_nb_clusters(){return NB_CLUSTERS;};
  void set_nb_clusters(int IN){NB_CLUSTERS = IN;};

  // nb sample:
  int get_nb_samples(){return NB_SAMPLES;};
  void set_nb_samples(int IN){NB_SAMPLES = IN;};
  
  // DATA:
  void set_DATA(vector<int> IN){ DATA = IN;};
  vector<int> get_DATA(){ return DATA;};
  // access the parameters of the data:
  double get_Score_by_ID(int);
  // randomized access of the data:
  int get_random_DATA();

  // get the object by id:
  void* get_Object_by_ID(int);

  
  ///////////////////////////////////////////////
  // set functions for all the required access functions of the cluster object to the
  // data hosting object:
  // to delete cluster centers:
  void set_delete_cluster_center_FCT(void(*IN_Pdelete_cluster_center)(void *pInstance, int A)){Pdelete_cluster_center = IN_Pdelete_cluster_center;};
  // to insert a new cluster center:
  void set_insert_cluster_center_FCT(void(*IN_Pinsert_cluster_center)(void *pInstance, int A)){Pinsert_cluster_center = IN_Pinsert_cluster_center;};
  // to copmare a objects to a centroid and get a score:
  void set_compare_FCT(double(*IN_Pget_object_comparison)(void *pInstance, int A, void* B)){Pget_object_comparison = IN_Pget_object_comparison;};
  // to copmare 2 objects  and get a score:
  void set_feature_compare_FCT(double(*IN_Pget_object_comparison)(void *pInstance, void* A, void* B)){Pget_feature_object_comparison = IN_Pget_object_comparison;};
  // to copmare 2 objects  and get a score:
  void set_cluster_compare_FCT(double(*IN_Pget_object_comparison)(void *pInstance, int A, int B)){Pget_cluster_object_comparison = IN_Pget_object_comparison;};
  // to get an objects by ID:
  void set_get_object_by_id_FCT(void*(*IN_Pget_object_by_ID)(void *pInstance, int A)){Pget_object_by_ID = IN_Pget_object_by_ID;};
  // to print the content of an objects by ID:
  void set_print_object_by_id_FCT(void (*IN_Pprint_info_by_ID)(void *pInstance, ELEMENT, bool mode)){Pprint_info_by_ID = IN_Pprint_info_by_ID;};
  // to compute a new mean center of a cluster by ID:
  void set_compute_new_center_FCT(bool (*IN_Pcompute_new_center)(void *pInstance, int ID, vector<ELEMENT>::iterator, vector<ELEMENT>::iterator, double)){Pcompute_new_center = IN_Pcompute_new_center;};
  ///////////////////////////////////////////////
  

  // set the number of start clusters:
  void set_NB_START_CLUSTERS( double IN ){ PERC_CLUSTERS = IN;};
  double get_NB_START_CLUSTERS( ){ return PERC_CLUSTERS;};
  
  
  // get a end cluster:
  map< int, vector<ELEMENT> >::iterator get_END_CLUSTERS(int ID){return END_CLUSTERS.find(ID);};
  int get_nb_END_CLUSTERS(){return END_CLUSTERS.size();};

  // access elements in the end cluster:
  vector<ELEMENT>::iterator get_END_CLUSTERS_element_start(int ID){return (*get_END_CLUSTERS(ID)).second.begin();};
  vector<ELEMENT>::iterator get_END_CLUSTERS_element_end(int ID){return (*get_END_CLUSTERS(ID)).second.end();};
 
  // access elements in the end cluster:
  map<int, vector<ELEMENT> >::iterator get_END_CLUSTERS_START(){return END_CLUSTERS.begin();};
  map<int, vector<ELEMENT> >::iterator get_END_CLUSTERS_END(){return END_CLUSTERS.end();};
  
};

#endif

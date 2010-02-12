///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS cluster_X_scorer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef CLUSTER_X_SCORER_H
#define CLUSTER_X_SCORER_H



class cluster_X_scorer{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  KMEAN_main_cluster* MAIN_CLUSTERING;
  map<int, double> THIS_CLUSTER_GAP_SCORES;
  double INTER_CLUSTER_SCORE;
  double ELEMENT_COUNT;
  int NB_CLUSTER_DATA;
  int NB_CLUSTERS;

  // general need by both statistical evaluations:
  map<int, vector<double> > ITERATION_GAP_CLUSTERING_SCORE;
  
  // for the GAP statistics
  map<int, vector<double> > ITERATION_GAP_REFERENCE_SCORE;
  map<int, pair<double,double> > ITERATION_GAP_STATISTICS;

  // for the Calinski and Habasz statistics:
  map<int, vector<double> > ITERATION_INTER_CLUSTER_CLUSTERING_SCORE;
  map<int, pair<double,double> > ITERATION_CH_CLUSTERING_SCORE;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~cluster_X_scorer();
  
  // class constructor
  cluster_X_scorer();
  // class copy constructor
  cluster_X_scorer(const cluster_X_scorer&);
  // class copy constructor
  cluster_X_scorer(const cluster_X_scorer*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  cluster_X_scorer& operator=(const cluster_X_scorer&);
  bool operator==(const cluster_X_scorer&);
  cluster_X_scorer& operator<=(const cluster_X_scorer&);
  cluster_X_scorer& operator>=(const cluster_X_scorer&);
  cluster_X_scorer& operator<(const cluster_X_scorer&);
  cluster_X_scorer& operator>(const cluster_X_scorer&);
  
  
  // evaluate the clusters structure from the kmeans clustering
  void evaluate_kmeans_clustering( KMEAN_main_cluster*);
  // evaluate a clusters form the kmeans clustering
  void evaluate_kmeans_cluster( map<int, vector<ELEMENT> >::iterator );
  // evaluate a clusters element against its cluster consenus
  double evaluate_element_vs_cluster( int , ELEMENT );
  // evaluate a given clustering structure in the kmeans_main_cluster
  void evaluate_this_kmeans_clustering();


  //////////////////////////////////////////////////////////
  // GAP kmeans evaluation statistics:
  // compute a pairwise similarity for a cluster for the GAP statistics
  void cluster_GAP_statistics( map<int, vector<ELEMENT> >::iterator);
  // build up the GAP statitistics
  void build_GAP_clustering_statistics( );
  // build the INTER CLUSTER SCORE
  void build_inter_cluster_SCORE( );
  // build the calinski and harabasz score
  // -> "estimating number of clusters in a datase via gap statistics"
  // ->  Robert Tibshirani, Guenther Walther and Trevor Hastie
  void build_CH_clustering_SCORE( );
  
  
  ///////////////////////////////////////////////////////////////////
  // builds up a reference distribution to compare the real cluster against
  // does this by randomly sampling N data points from the pool of data 
  // and then repeat the clustering. do this b times to get statistics
  void build_GAP_REFERENCE_DISTRIBUTION( );
  // build up the GAP statitistics for reference distribution
  void build_GAP_clustering_reference_statistics( );
  // build the final GAP statistics
  // -> GAP(k) = 1/B sum( log( wk*) ) - Wk
  void build_ITERATION_GAP_STATISTICS( );
  
  // build the total score of the kmeans clustering
  void build_total_clustering_score( );
  // show the kmeans clustering evaluation
  void show_evaluation( );

  
  // plot the iteration scores
  void PRINT_ITERATION_SCORES( );

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

};

#endif

    

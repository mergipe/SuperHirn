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

#define USE_CLUSTER_X_SCORER
#include "CLUSTER_LC_MS_ANALYSIS_USE.h"




////////////////////////////////////////////////
// constructor for the object cluster_X_scorer:
cluster_X_scorer::cluster_X_scorer(){
  MAIN_CLUSTERING = NULL;
}

//////////////////////////////////////////////////
// class desctructor of cluster_X_scorer
cluster_X_scorer::~cluster_X_scorer(){
  MAIN_CLUSTERING = NULL;
}

//////////////////////////////////////////////////
// class copy constructor of cluster_X_scorer
cluster_X_scorer::cluster_X_scorer(const cluster_X_scorer& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of cluster_X_scorer
cluster_X_scorer::cluster_X_scorer(const cluster_X_scorer* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
cluster_X_scorer& cluster_X_scorer::operator=(const cluster_X_scorer& tmp){
  return *this;
}
    

//////////////////////////////////////////////////////////
// evaluate the clusters structure from the kmeans clustering
void cluster_X_scorer::evaluate_kmeans_clustering( KMEAN_main_cluster* IN_CLUSTERING){
  
  MAIN_CLUSTERING = IN_CLUSTERING;
  NB_CLUSTER_DATA = MAIN_CLUSTERING->get_nb_samples();
  NB_CLUSTERS = int(MAIN_CLUSTERING->get_NB_START_CLUSTERS( ));
  
  printf("\t\t\t- evaluate clustering (nb.data=%d|nb.clusters=%d)\n", NB_CLUSTER_DATA, NB_CLUSTERS );
  
  // evaluate this clustering by the GAP score:
  evaluate_this_kmeans_clustering();
  build_GAP_clustering_statistics( );
    
  ////////////////////
  // subscore for the calinski and harabasz score
  // build_inter_cluster_SCORE( );

}



//////////////////////////////////////////////////////////
// evaluate a given clustering structure in the kmeans_main_cluster
void cluster_X_scorer::evaluate_this_kmeans_clustering(){
  
  ELEMENT_COUNT = 0;
  THIS_CLUSTER_GAP_SCORES.clear();
  
  ////////////////////
  // go through the  cluster:  
  map<int, vector<ELEMENT> >::iterator P = MAIN_CLUSTERING->get_END_CLUSTERS_START();
  // check if it contains any elements:
  while( P != MAIN_CLUSTERING->get_END_CLUSTERS_END() ){
    ////////////////////
    // build the gap score:
    cluster_GAP_statistics( P );
    P++;
  }
 
}





//////////////////////////////////////////////////////////
// compute a pairwise similarity for a cluster for the GAP statistics
void cluster_X_scorer::cluster_GAP_statistics( map<int, vector<ELEMENT> >::iterator IN){
  
  double Dr = 0;    
  vector<ELEMENT>::iterator A = (*IN).second.begin();
  vector<ELEMENT>::iterator B;
  while( A != (*IN).second.end() ){
    
    B = A;
    B++;
    while( B != (*IN).second.end() ){
      Dr += MAIN_CLUSTERING->compare_elements( (*A), (*B) );
      B++;
    }
    A++;
  }
  
  // transform the score according to GAP:
  Dr = Dr * ( 1.0 / ( 2.0 * double( (*IN).second.size( ) ) ) );
  
  // store
  THIS_CLUSTER_GAP_SCORES.insert( make_pair( (*IN).first, Dr) );
  
}


//////////////////////////////////////////////////////////
// build up the GAP statitistics
void cluster_X_scorer::build_GAP_clustering_statistics( ){
  
  double GAP_CLUSTERING_SCORE = 0;
  map<int, double>::iterator C = THIS_CLUSTER_GAP_SCORES.begin();
  while( C != THIS_CLUSTER_GAP_SCORES.end() ){    
    GAP_CLUSTERING_SCORE += (*C).second;
    C++;
  }

  map<int, vector<double> >::iterator F = ITERATION_GAP_CLUSTERING_SCORE.find( NB_CLUSTERS );
  if( F != ITERATION_GAP_CLUSTERING_SCORE.end() ){
    (*F).second.push_back( GAP_CLUSTERING_SCORE ); 
  }
  else{
    vector<double> tmp;
    tmp.push_back( GAP_CLUSTERING_SCORE ); 
    ITERATION_GAP_CLUSTERING_SCORE.insert( make_pair( NB_CLUSTERS, tmp ) );
  }
  
  THIS_CLUSTER_GAP_SCORES.clear();
}

//////////////////////////////////////////////////////////
// build up the GAP statitistics for reference distribution
void cluster_X_scorer::build_GAP_clustering_reference_statistics( ){
  
  
  double GAP_CLUSTERING_SCORE = 0;
  map<int, double>::iterator C = THIS_CLUSTER_GAP_SCORES.begin();
  while( C != THIS_CLUSTER_GAP_SCORES.end() ){    
    GAP_CLUSTERING_SCORE +=  (*C).second;
    C++;
  }
  
  map<int, vector<double> >::iterator F = ITERATION_GAP_REFERENCE_SCORE.find( NB_CLUSTERS );
  if( F != ITERATION_GAP_REFERENCE_SCORE.end() ){
    (*F).second.push_back( GAP_CLUSTERING_SCORE ); 
  }
  else{
    vector<double> tmp;
    tmp.push_back( GAP_CLUSTERING_SCORE ); 
    ITERATION_GAP_REFERENCE_SCORE.insert( make_pair( NB_CLUSTERS, tmp ) );
  }
  THIS_CLUSTER_GAP_SCORES.clear();
}


//////////////////////////////////////////////////////////
// build the INTER CLUSTER SCORE
void cluster_X_scorer::build_inter_cluster_SCORE( ){
  
  double INTER_CLUSTER_SCORE = 0;
  map<int, vector<ELEMENT> >::iterator A = MAIN_CLUSTERING->get_END_CLUSTERS_START();
  map<int, vector<ELEMENT> >::iterator B;
  while( A != MAIN_CLUSTERING->get_END_CLUSTERS_END() ){    
    B = A;
    B++;
    while( B != MAIN_CLUSTERING->get_END_CLUSTERS_END() ){    
      INTER_CLUSTER_SCORE += MAIN_CLUSTERING->compare_cluster_elements( (*A).first, (*B).first );
      B++;
    }
    A++;
  }
  
  INTER_CLUSTER_SCORE /= double( MAIN_CLUSTERING->get_NB_START_CLUSTERS( ) );
  map<int, vector<double> >::iterator F = ITERATION_INTER_CLUSTER_CLUSTERING_SCORE.find( NB_CLUSTERS );
  if( F != ITERATION_INTER_CLUSTER_CLUSTERING_SCORE.end() ){
    (*F).second.push_back( INTER_CLUSTER_SCORE ); 
  }
  else{
    vector<double> tmp;
    tmp.push_back( INTER_CLUSTER_SCORE ); 
    ITERATION_INTER_CLUSTER_CLUSTERING_SCORE.insert( make_pair( NB_CLUSTERS, tmp ) );
  }
}

//////////////////////////////////////////////////////////
// build the calinski and harabasz score
// -> "estimating number of clusters in a datase via gap statistics"
// ->  Robert Tibshirani, Guenther Walther and Trevor Hastie
//
//    CH(k) = [ B(k) / ( k - 1 ) ] / [ W(k) / ( n - k )]
void cluster_X_scorer::build_CH_clustering_SCORE( ){

  simple_math MY_MATH;
  
  map<int, vector<double> >::iterator GAP = ITERATION_GAP_CLUSTERING_SCORE.begin();
  map<int, vector<double> >::iterator INTER_C = ITERATION_INTER_CLUSTER_CLUSTERING_SCORE.begin();
  while( GAP != ITERATION_GAP_CLUSTERING_SCORE.end() ){
    
    double k = (*GAP).first;
    vector<double> ALL_CH_SCORE;
    pair<double,double> CH_SCORE;
    vector<double>::iterator GAP_R = (*GAP).second.begin();
    vector<double>::iterator INTER_R = (*INTER_C).second.begin();
    while( GAP_R != (*GAP).second.end() ){
      
      double Wk = (*GAP_R);
      double Bk = (*INTER_R);
      double CH = 0;
      CH = Bk / ( k - 1.0 );
      CH /= ( Wk / ( double( NB_CLUSTER_DATA ) - k));
      ALL_CH_SCORE.push_back( CH );
      
      GAP_R++;
      INTER_R++;
    }
    
    // now build the average:
    CH_SCORE = MY_MATH.AVERAGE_and_STDEV( &ALL_CH_SCORE );
    ITERATION_CH_CLUSTERING_SCORE.insert( make_pair( int(k), CH_SCORE ) );
    
    GAP++;
    INTER_C++;
  }
  
}

//////////////////////////////////////////////////////////
// show the kmeans clustering evaluation
void cluster_X_scorer::show_evaluation( ){
  
  simple_math MY_MATH;
  double Bk = 0.0;
  pair<double,double> gap_stat;
  pair<double,double> CH_stat;
  CH_stat.first = 0;
  CH_stat.second = 0;
  
  printf("\t\tKMEANS clustering evaluation:\n");
  printf("\t\t\t\tNumber of data points:\t%d\n", NB_CLUSTER_DATA);    
  printf("\t\t\t\tNumber of clusters:\t%d\n", NB_CLUSTERS);      

  map<int, vector<double> >::iterator F = ITERATION_GAP_CLUSTERING_SCORE.find( NB_CLUSTERS );
  if( F != ITERATION_GAP_CLUSTERING_SCORE.end() ){
    pair<double,double> Wk = MY_MATH.AVERAGE_and_STDEV( &(*F).second );
    printf("\t\t\t\tGAP cluster score:\t%0.4f[%0.4f]\n", Wk.first, Wk.second);  
  }
  
  F = ITERATION_GAP_REFERENCE_SCORE.find( NB_CLUSTERS );
  if( F != ITERATION_GAP_REFERENCE_SCORE.end() ){
    pair<double,double> tmp = MY_MATH.AVERAGE_and_STDEV( &(*F).second );
    printf("\t\t\t\tGAP refrence distribution:\t%0.4f[%0.4f]\n", tmp.first, tmp.second); 
    /*
    vector<double>::iterator V = (*F).second.begin();
    cout<<"(";
    while( V != (*F).second.end() ){
      cout<<*V<<"\t";
      V++;
    }
    cout<<")"<<endl;
     */
  }
  
  F = ITERATION_INTER_CLUSTER_CLUSTERING_SCORE.find( NB_CLUSTERS );
  if( F != ITERATION_INTER_CLUSTER_CLUSTERING_SCORE.end() ){
    Bk = (MY_MATH.AVERAGE_and_STDEV( &(*F).second ) ).first;
    printf("\t\t\t\tIntercluster score:\t%0.4f\n", Bk);  
  }
  
  map<int, pair<double, double> >::iterator CH_F = ITERATION_CH_CLUSTERING_SCORE.find( NB_CLUSTERS );
  if( CH_F != ITERATION_CH_CLUSTERING_SCORE.end() ){
    CH_stat = (*CH_F).second;
    printf("\t\t\t\tCalinski and Harabasz score:\t%0.4f(%0.4f)\n", CH_stat.first, CH_stat.second );  
  }
  
  map<int, pair<double,double> >::iterator F_GAP = ITERATION_GAP_STATISTICS.find( NB_CLUSTERS );
  if( F_GAP != ITERATION_GAP_STATISTICS.end() ){
    gap_stat = (*F_GAP).second;
    printf("\t\t\t\tGAP statistics:\t%0.4f[%0.6f]\n", gap_stat.first, gap_stat.second);  
  }
  
  cout<<endl<<endl;
  
}


//////////////////////////////////////////////////////////
// plot the iteration scores
void cluster_X_scorer::PRINT_ITERATION_SCORES( ){
  
  char outfile[255];
  data_plotter* PLOT = NULL;
  string tmp;

  // print ch score…
  if( !ITERATION_CH_CLUSTERING_SCORE.empty() ){

    simple_math MY_MATH;
    vector< double > STD;
    map<int, double> AV;
    map<int, pair<double,double> >::iterator P = ITERATION_CH_CLUSTERING_SCORE.begin( );
    while( P != ITERATION_CH_CLUSTERING_SCORE.end() ){
      AV.insert( make_pair( (*P).first, (*P).second.first) );
      STD.push_back( (*P).second.second );
      P++;
    }
    sprintf( outfile, "CH_scores");
    tmp = outfile;
    PLOT = new data_plotter( tmp );
    PLOT->add_plot_data( &AV, &STD );
    PLOT->define_MAX_Y();
    PLOT->plot_TWOD_data();
    PLOT->print_TWOD_data_to_TXT();
    delete PLOT;
    PLOT = NULL;
  }

  // print gap score…
  if( ! ITERATION_GAP_STATISTICS.empty() ){
    vector< double > STD;
    map<int, double> AV;
    map<int, pair<double,double> >::iterator P = ITERATION_GAP_STATISTICS.begin( );
    while( P != ITERATION_GAP_STATISTICS.end() ){
      AV.insert( make_pair( (*P).first, (*P).second.first) );
      STD.push_back( (*P).second.second );
      P++;
    }
    sprintf( outfile, "GAP_statistics");
    tmp = outfile;
    PLOT = new data_plotter( tmp );
    PLOT->add_plot_data( &AV, &STD );
    PLOT->define_MAX_Y();
    PLOT->plot_TWOD_data();
    PLOT->print_TWOD_data_to_TXT();
    delete PLOT;
    PLOT = NULL;
  }
  
  /*
  // print gap score…
  if( !ITERATION_INTER_CLUSTER_CLUSTERING_SCORE.empty() ){
    sprintf( outfile, "INTER_score");
    tmp = outfile;
    PLOT = new data_plotter( tmp );
    PLOT->add_plot_data( &ITERATION_INTER_CLUSTER_CLUSTERING_SCORE );
    PLOT->define_MAX_Y();
    PLOT->plot_TWOD_data();
    PLOT->print_TWOD_data_to_TXT();
    delete PLOT;
    PLOT = NULL;
  }
  
  */
}



///////////////////////////////////////////////////////////////////
// builds up a reference distribution to compare the real cluster against
// does this by randomly sampling N data points from the pool of data 
// and then repeat the clustering. do this b times to get statistics
void cluster_X_scorer::build_GAP_REFERENCE_DISTRIBUTION( ){
  
  // printf("\t\t\t- build Gap reference disribution\n" );
  
  // start the clustering of the randome data:
  MAIN_CLUSTERING->start_clustering();
  // MAIN_CLUSTERING->print_clustering_overview();
  
  // evaluate this clustering by the GAP score:
  evaluate_this_kmeans_clustering();
  
  // build up the GAP statitistics for reference distribution
  build_GAP_clustering_reference_statistics( );  
  
}


///////////////////////////////////////////////////////////////////  
// build the final GAP statistics
// -> GAP(k) = 1/B sum( log( Wk*) ) - log( Wk )
void cluster_X_scorer::build_ITERATION_GAP_STATISTICS( ){

  map<int, vector<double> >::iterator F_REF = ITERATION_GAP_REFERENCE_SCORE.find( NB_CLUSTERS );
  map<int, vector<double> >::iterator F_GAP = ITERATION_GAP_CLUSTERING_SCORE.find( NB_CLUSTERS );
  if( ( F_GAP != ITERATION_GAP_CLUSTERING_SCORE.end() ) && ( F_REF != ITERATION_GAP_REFERENCE_SCORE.end() ) ){
    
    // compute the average from the GAP data array:    
    simple_math MY_MATH;
    double GAP = (MY_MATH.AVERAGE_and_STDEV( &(*F_GAP).second )).first;
    GAP = log( GAP );
    
    // transform the reference GAP into log
    vector<double> R_GAP;
    vector<double>::iterator P = (*F_REF).second.begin(); 
    while( P != (*F_REF).second.end() ){
      R_GAP.push_back( log( *P ) );
      P++;
    }    
    // get the averages:
    pair<double,double> AV_REFR = MY_MATH.AVERAGE_and_STDEV( &R_GAP );

    // compute the GAP statisics
    double GAP_STAT = AV_REFR.first - GAP;
    ITERATION_GAP_STATISTICS.insert( make_pair( NB_CLUSTERS, make_pair(GAP_STAT, AV_REFR.second) ) );
    
  }
}
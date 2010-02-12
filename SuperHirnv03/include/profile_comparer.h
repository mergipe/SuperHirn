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


#ifndef PROFILE_COMPARER_H
#define PROFILE_COMPARER_H



class profile_comparer{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:

  map<int, vector<protein_group> > PROFILE_PROTEIN_DATA;
  map<string, protein_group> ALL_PROTEINS;
  map<int, vector< protein_group> > BEST_PROTEINS;
  map<int, vector<double> > TARGET_CLUSTER_PROFILES;

  double PEPTIDE_PROBABILITY_THRESHOLD;
  double PROFILE_PROBABILITY_THRESHOLD;
  int NB_PROFILE_POINTS;
  bool PRINT_OUT;
  bool TRUE_TARGET_PROFILE;
  
  // data holder for the imported features:
  map< int, vector< feature> > ALL_MS1_FEATURES;
  // data holder for all profile groups:
  vector< profile_group > ALL_PROFILE_GROUPS;


public:
  
  // class destructor
  ~profile_comparer();
  
  // class constructor
  profile_comparer();
  // class copy constructor
  profile_comparer(const profile_comparer&);
  // class copy constructor
  profile_comparer(const profile_comparer*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  profile_comparer& operator=(const profile_comparer&);
  bool operator==(const profile_comparer&);
  profile_comparer& operator<=(const profile_comparer&);
  profile_comparer& operator>=(const profile_comparer&);
  profile_comparer& operator<(const profile_comparer&);
  profile_comparer& operator>(const profile_comparer&);
  
  // write a profile_group to XML
  void print_profile_group_to_XML( profile_group* );
  // write a profile_cluster to APML
  void print_profile_group_to_APML( profile_group*);
  // write a profile_cluster to space delimited txt file
  void print_profile_group_to_TXT( profile_group* );
  // plot the best matching clusters to the target profile:
  void plot_best_matching_clusters();
  // print a profile_group
  void print_PEPTIDE_profile_group( profile_group*, int);
  void print_PROTEIN_profile_group( profile_group*, int);
  // plot the best candidates by this functions:
  void plot_BEST_PROTEIN_HITS(  );
  // plot peptides of the best candidates by this functions:
  void plot_peptides_of_BEST_PROTEIN_HITS( );
  // parser that extracts profile groups from a superhirn clusterin
  // output
  void import_profiling_data_from_XML();
  // make a statistics for which input profiles the cluster
  // match best:
  void EXTRACT_BEST_TARGET_PROFILE_CLUSTERS();    
  // parses a profile group:
  void parse_profile_group( TiXmlNode* );
  // store the best candidates by this functions:
  void store_BEST_HITS( profile_group* IN );
  // process the best hits protein from the profiling
  void process_BEST_PROTEIN_HITS();



  
  // update created protein groups with information
  // from MASTER run
  void update_protein_groups_by_MASTER( map<int, LC_MS>*, profile_group*);
  // score all elements in a profile group versus the
  // target profile, then make statistics and assign p-value: 
  void statistical_cluster_analysis( profile_group* );
    // score all peptide groups versus the input
  // target profile, then make statistics and assign p-value: 
  void target_profiling_PEPTIDE_LEVEL( profile_group* , map<int, double>);    
  // score the protein groups versus the input
  // target profile, then make statistics and assign p-value: 
  void target_profiling_PROTEIN_LEVEL( profile_group* , map<int, double>);
  // extracts from the mastermap a profile group
  // filters for features with min profile lengths
  void extract_PROFILE_GROUP_from_MASTERMAP( LC_MS* );
    

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

  // add protein group from a profile:
  void add_protein_group_profile(int, map<int, vector< protein_group> >* );

  void set_PROFILE_GROUPS(vector< profile_group> IN){ALL_PROFILE_GROUPS=IN;};
  vector< profile_group> get_PROFILE_GROUPS(){return ALL_PROFILE_GROUPS;};
  vector< profile_group>::iterator get_PROFILE_GROUPS_START(){return ALL_PROFILE_GROUPS.begin();};
  vector< profile_group>::iterator get_PROFILE_GROUPS_END(){return ALL_PROFILE_GROUPS.end();};

  map<int, vector<protein_group> >::iterator get_BEST_PROTEINS_START(){return BEST_PROTEINS.begin();};
  map<int, vector<protein_group> >::iterator get_BEST_PROTEINS_END(){return BEST_PROTEINS.end();};

  vector<double> get_TARGET_CLUSTER_PROFILES( int ID){
    map<int, vector<double> >::iterator F = TARGET_CLUSTER_PROFILES.find( ID );
    if( F != TARGET_CLUSTER_PROFILES.end() ){
      return (*F).second;
    }
    else{
      vector<double> tmp;
      return tmp;
    }
  };

  
};

#endif

    

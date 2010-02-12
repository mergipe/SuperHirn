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


#ifndef PROTEIN_GROUP_H
#define PROTEIN_GROUP_H

typedef pair<double, bool> MS2_HIT;

class protein_group{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  double PROFILE_SCORE;
  int MASTER_ID;
  int PROTEIN_GROUP_ID;

  string AC;
  
  double CLUSTER_INTERNAL_SCORE;
  double PROFILE_PROBABILITY;
  
  vector<peptide_DELTA_group> PEPTIDE_MEMBERS;
  vector<peptide_DELTA_group> PROTEO_TYPIC_PEPTIDE_MEMBERS;
  vector<peptide_DELTA_group> OUTLIER_PEPTIDE_MEMBERS;
  vector<feature*> ALL_FEATURES;
  
  map<string, vector<MS2_HIT> > PEPTIDE_LIST;
  map<int, double> PROFILE_AVERAGE_CONSENS;
  map<int, double> PROFILE_AVERAGE_CONSENS_STD;

  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
    static bool PEPTIDE_PROTEOTYPE_MODE;
  static int NB_PROFILE_DATA_POINTS;
  static double TR_TOL;
  static map<int, vector<double> > LC_MS_Modification_Masses;
  double MR;
  
    // class destructor
  ~protein_group();
  
  // class constructor
  protein_group();
  protein_group(string);
  protein_group(peptide_DELTA_group*);
  // class copy constructor
  protein_group(const protein_group&);
  // class copy constructor
  protein_group(const protein_group*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  protein_group& operator=(const protein_group&);
  bool operator<=(const protein_group&);
  bool operator>=(const protein_group&);
  bool operator<(const protein_group&);
  bool operator>(const protein_group&);
  
  
  // check if this feature belongs to the group:
  bool check_belonging(feature*);
  bool check_belonging(peptide_DELTA_group*);
  
  
  // all feature access:
  vector<feature*>::iterator get_feature_members_list_start(){return ALL_FEATURES.begin();};
  vector<feature*>::iterator get_feature_members_list_end(){return ALL_FEATURES.end();};
  vector<feature*>* get_feature_members(){return &ALL_FEATURES;};

  // display function:
  void show_members();
  // display function:
  void show_protein_cluster_simple_view();
  // print protein profile to file:
  void print_protein_profile_to_file();


  // write into XML formated text into file:
  void write_to_XML_file(LC_MS_XML_writer* );
  
  // write into APML formated text into file:
  void write_to_APML_file(LC_MS_APML_writer*);

  // write into delimited TXT file:
  void write_to_TXT_file(ofstream* IN);

  // check with members if 2 features from with same LC/MS ,
  // if so take the one which is closest to 
  void filter_MEMBERS();

  // add feature MS2 info:
  void add_MS2_feature_INFO(feature*);
  // add features of a group into this one::
  void merge_peptide_DELTA_group(peptide_DELTA_group*);  
  // add features of a group into this one::
  void merge_protein_group(protein_group*);
  // give the iterators to the protein, so it can mark bad ones:
  void remove_outlier_peptide_groups( vector< vector< peptide_DELTA_group >::iterator > );

  
  // void build_protein_average_consens();
  void build_protein_average_consens();
  // set protein profile probability by averaging from peptides
  void set_profile_probability_from_peptide_groups( );

  // update protein group by a feature:
  void update_protein_by_feature(feature*);

  // info:
  void show_info();
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  int get_unique_peptides(){return PEPTIDE_LIST.size();};
  
  double get_MR(){return MR;};
  
  
  // get nb of MS1 featues in the PROTEIN group
  int get_nb_features( ){ return ALL_FEATURES.size();};
  // get nb of identified MS1 featues in the PROTEIN group
  int get_nb_assigned_features();


  // PEPTDIE DELTA GROUP LIST:
  int get_nb_peptide_members(){return ( PEPTIDE_MEMBERS.size() + PROTEO_TYPIC_PEPTIDE_MEMBERS.size() );};
  vector<peptide_DELTA_group>* get_peptide_members_list(){return &PEPTIDE_MEMBERS;};
  vector<peptide_DELTA_group>::iterator get_peptide_members_list_start(){return PEPTIDE_MEMBERS.begin();};
  vector<peptide_DELTA_group>::iterator get_peptide_members_list_end(){return PEPTIDE_MEMBERS.end();};
  vector<peptide_DELTA_group>::iterator get_proteotypic_peptide_members_list_start(){return PROTEO_TYPIC_PEPTIDE_MEMBERS.begin();};
  vector<peptide_DELTA_group>::iterator get_proteotypic_peptide_members_list_end(){return PROTEO_TYPIC_PEPTIDE_MEMBERS.end();};
  vector<peptide_DELTA_group>::iterator get_outlier_peptide_members_list_start(){return OUTLIER_PEPTIDE_MEMBERS.begin();};
  vector<peptide_DELTA_group>::iterator get_outlier_peptide_members_list_end(){return OUTLIER_PEPTIDE_MEMBERS.end();};
 
  map<int, double>* get_PROFILE_AVERAGE_CONSENS(){return &PROFILE_AVERAGE_CONSENS;};
  map<int, double>* get_PROFILE_AVERAGE_CONSENS_STDEV(){return &PROFILE_AVERAGE_CONSENS_STD;};
  
  map<string, vector<MS2_HIT> >::iterator get_PEPTIDE_LIST_START(){return PEPTIDE_LIST.begin();};
  map<string, vector<MS2_HIT> >::iterator get_PEPTIDE_LIST_END(){return PEPTIDE_LIST.end();};

  
  // access MASTER ID:
  void add_MASTER_ID(int);
  string get_AC(){return AC;};
  void set_AC(string IN){AC = IN;};
  
  double get_CLUSTER_INTERNAL_SCORE(){return CLUSTER_INTERNAL_SCORE;};
  void set_CLUSTER_INTERNAL_SCORE(double IN){CLUSTER_INTERNAL_SCORE = IN;};
  
  void set_PROFILE_SCORE( double IN){ PROFILE_SCORE = IN;};
  double get_PROFILE_SCORE(){ return PROFILE_SCORE;};
  
  void set_PROFILE_PROBABILITY(double IN){PROFILE_PROBABILITY = IN;};
  double get_PROFILE_PROBABILITY(){return PROFILE_PROBABILITY;};
  
  void set_PROTEIN_GROUP_ID(int IN){PROTEIN_GROUP_ID = IN;};
  int get_PROTEIN_GROUP_ID(){return PROTEIN_GROUP_ID;};

  bool get_PROTEOTYPE(){ return !(PROTEO_TYPIC_PEPTIDE_MEMBERS.empty()) ;};
  // check how many proteotypic peptides are in the protein
  int get_NB_PROTEOTYPIC_PEPTIDES();

};

#endif

    

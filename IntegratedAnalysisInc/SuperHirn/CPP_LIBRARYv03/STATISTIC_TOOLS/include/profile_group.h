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


#ifndef PROFILE_GROUP_H
#define PROFILE_GROUP_H

typedef feature* PEPTIDE;
typedef vector< PEPTIDE> PEPTIDE_LIST;

class profile_group{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  map< string, PEPTIDE_LIST> UNIQUE_SQ;
  map< string, PEPTIDE_LIST> UNIQUE_AC;
  vector< feature*> MEMBER_LIST;
  map<int, double> CLUSTER_PROFILE;
  map< int, map<int, double> > SELECTED_CLUSTER_PROFILES;
  map<int, double> TARGET_PROFILE;
  int TARGET_PROFILE_ID;
  int PROFILE_GROUP_ID;
  int CLUSTER_ID;
  
  
  ///////////
  // list of peptide groups within this cluster:
  vector<peptide_DELTA_group> PEPTIDE_GROUP_LIST;

  ///////////
  // list of protein groups within this cluster:
  vector<protein_group> PROTEIN_GROUPS;
  
  
  int MASTER_ID;
  double CLUSTER_SCORE;
  double CLUSTER_STDEV;
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
    
   static int PROFILE_LENGTH;
  static bool GLOBAL_SCORING;
  
  // class destructor
  ~profile_group();
  
  // class constructor
  profile_group(int);
  // class copy constructor
  profile_group(const profile_group&);
  // class copy constructor
  profile_group(const profile_group*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  profile_group& operator=(const profile_group&);
  bool operator==(const profile_group&);
  profile_group& operator<=(const profile_group&);
  profile_group& operator>=(const profile_group&);
  profile_group& operator<(const profile_group&);
  profile_group& operator>(const profile_group&);
  
  // add a peptide = feature to the cluster:
  void add_cluster_member(feature*);
  // megre another profile cluster into this cluster:
  void merge_profile_group(profile_group*);

  
  // show overview:
  void show_profile_group_overview();
  // show content:
  void show_cluster_members_by_SQ();
  void show_unique_protein_overview();
  // show a profile of a peptide:
  void show_peptide_profile(feature*);
  // print consens profile:
  void print_consens_profile();
  // show content ordered by the cluster association score:
  void show_cluster_members_ranking();
  // write into XML formated text into file:
  void write_to_XML_file( LC_MS_XML_writer* );
  // write into APML formated text into file:
  void write_to_APML_file(LC_MS_APML_writer* );



  // get the rank of a cluster association score:
  int get_rank(double);

  // access all members in the cluster:
  int get_NB_MEMBERS(){return MEMBER_LIST.size();};
  vector< feature*>::iterator get_MEMBER_LIST_start(){return MEMBER_LIST.begin();};
  vector< feature*>::iterator get_MEMBER_LIST_end(){return MEMBER_LIST.end();};
  vector< feature*>* get_MEMBER_LIST(){return &MEMBER_LIST;};
  vector< feature*>::iterator erase_MEMBER(vector< feature*>::iterator P){return MEMBER_LIST.erase( P );};
  
  // show info:
  void show_info();
  // sort the porteins according to profile probablity 
  void sort_proteins_by_profile_probability( );
  struct OPERATOR_PROT{
    // provide the compare function for sort:
    bool operator()(protein_group A,protein_group B) const{
      // check if they have same mass
      if( A.get_PROFILE_PROBABILITY() == B.get_PROFILE_PROBABILITY()){
        return A.get_PROFILE_SCORE() < B.get_PROFILE_SCORE();
      }
      else{
        return A.get_PROFILE_PROBABILITY() > B.get_PROFILE_PROBABILITY();
      }
    }
  };

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // add peptide delta cluster groups:
  void add_PEPTIDE_GROUPS( vector<peptide_DELTA_group> IN){PEPTIDE_GROUP_LIST = IN;};
  vector<peptide_DELTA_group>* get_PEPTIDE_GROUPS(){return &PEPTIDE_GROUP_LIST;};
  vector<peptide_DELTA_group>::iterator get_PEPTIDE_GROUPS_START(){return PEPTIDE_GROUP_LIST.begin();};
  vector<peptide_DELTA_group>::iterator get_PEPTIDE_GROUPS_END(){return PEPTIDE_GROUP_LIST.end();};
  vector<peptide_DELTA_group>::iterator erase_PEPTIDE_GROUP(vector<peptide_DELTA_group>::iterator P){return PEPTIDE_GROUP_LIST.erase(P);};
  int get_NB_PEPTIDE_GROUPS(){return PEPTIDE_GROUP_LIST.size();};

  // add peptide delta cluster groups:
  void add_PROTEIN_GROUPS( vector<protein_group> IN){PROTEIN_GROUPS = IN;};
  vector<protein_group>* get_PROTEIN_GROUPS(){return &PROTEIN_GROUPS;};
  vector<protein_group>::iterator get_PROTEIN_GROUPS_START(){return PROTEIN_GROUPS.begin();};
  vector<protein_group>::iterator get_PROTEIN_GROUPS_END(){return PROTEIN_GROUPS.end();};
  int get_NB_PROTEIN_GROUPS(){return PROTEIN_GROUPS.size();};
  
  void set_CLUSTER_PROFILE( map<int, double> IN){ CLUSTER_PROFILE = IN;};
  map<int, double> get_CLUSTER_PROFILE(){return CLUSTER_PROFILE;};

  int get_CLUSTER_ID(){return CLUSTER_ID;};
  void set_CLUSTER_ID(int IN){CLUSTER_ID = IN;};

  int get_MASTER_ID(){return MASTER_ID;};
  void set_MASTER_ID(int IN){MASTER_ID = IN;};
  
  void set_CLUSTER_SCORE(double IN){CLUSTER_SCORE = IN;};
  double get_CLUSTER_SCORE(){return CLUSTER_SCORE;};
  
  void set_CLUSTER_STDEV(double IN){CLUSTER_STDEV = IN;};
  double get_CLUSTER_STDEV(){return CLUSTER_STDEV;};
  
  void add_SELECTED_CLUSTER_PROFILE(int ID, map<int, double> IN){ SELECTED_CLUSTER_PROFILES.insert( make_pair( ID, IN ) );};
  map<int, map<int, double> >::iterator get_SELECTED_CLUSTER_PROFILE_START( ){ return SELECTED_CLUSTER_PROFILES.begin( );};
  map<int, map<int, double> >::iterator get_SELECTED_CLUSTER_PROFILE_END( ){ return SELECTED_CLUSTER_PROFILES.end( );};

  int get_TARGET_PROFILE_ID(){ return TARGET_PROFILE_ID;};
  void set_TARGET_PROFILE_ID( int IN){ TARGET_PROFILE_ID = IN;};

  int get_PROFILE_GROUP_ID(){ return PROFILE_GROUP_ID;};
  void set_PROFILE_GROUP_ID( int IN){ PROFILE_GROUP_ID = IN;};

  map<int, double> get_TARGET_PROFILE(){ return TARGET_PROFILE;};
  void set_TARGET_PROFILE( map<int,double> IN){ TARGET_PROFILE = IN;};
  void set_TARGET_PROFILE( vector<double>);
  

};

#endif

    

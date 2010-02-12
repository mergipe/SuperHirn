///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS peptide_DELTA_group:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef PEPTIDE_DELTA_GROUP_H
#define PEPTIDE_DELTA_GROUP_H



class peptide_DELTA_group{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  double TR;
  double pI;
  double MZ;
  int Z;
  
  int MASTER_ID;
  int PEPTIDE_GROUP_ID;
  
  map<string, vector<ms2_info*> > PROTEIN_ACs;
  vector<ms2_info*> ALL_MS2_INFO;
  vector<feature*> MEMBERS;
  map<double, vector<feature*> > MODIFIED_MEMBERS;
  map<int, double> AVERAGE_PEPTIDE_PROFILE;
  map<int, double> AVERAGE_PEPTIDE_PROFILE_STD;
  
  double PROFILE_SCORE;
  double PROFILE_PROBABILITY;
  double PROFILE_INTERNAL_SCORE;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
    
    static double TR_TOL;
  static double MOD_TR_TOL;
  static double PEPTIDE_PROBABILITY_THRESHOLD;
  static int NB_PROFILE_DATA_POINTS;
  static bool MOLECULAR_MASS_COMPARISON;


  static map<int, vector<double> > LC_MS_Modification_Masses;
  double MR;
  
    // class destructor
  ~peptide_DELTA_group();
  
  // class constructor
  peptide_DELTA_group();
  peptide_DELTA_group(feature*);
  //peptide_DELTA_group(double, double);
  // class copy constructor
  peptide_DELTA_group(const peptide_DELTA_group&);
  // class copy constructor
  peptide_DELTA_group(const peptide_DELTA_group*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  peptide_DELTA_group& operator=(const peptide_DELTA_group&);
  bool operator==( peptide_DELTA_group&);
  bool operator<=(const peptide_DELTA_group&);
  bool operator>=(const peptide_DELTA_group&);
  bool operator<(const peptide_DELTA_group&);
  bool operator>(const peptide_DELTA_group&);
  
  
  // check if this feature belongs to the group:
  bool check_belonging(feature*);
  bool check_belonging(peptide_DELTA_group*);
  // check a MR matches the MR of this peptide_group
  bool compare_MR(double , double , double );
  // check a mz matches using the mz of this peptide_group
  bool compare_MZ(double , double, double, int);


  // show info
  void show_info();
  // display function:
  void show_members();
  // write into XML formated text into file:
  void write_content_2_XML( LC_MS_XML_writer* );
  // write into APML formated text into file:
  void write_content_2_APML( LC_MS_APML_writer* );
  // check with members if 2 features from with same LC/MS ,
  // if so take the one which is closest to 
  void filter_MEMBERS();
  // get all members ( modifed also) from the group
  vector<feature*> get_ALL_FEATURE_MEMBERS();
  // build full ion current over peptide modification vairants
  void build_modification_resolved_feature_areas();
  // merge the fetaure areas over the replicates
  void merge_feature_areas( feature* , feature* );


  // add feature:
  void add_feature(feature*);
  // add modified feature:
  void add_modified_feature(feature*);
  // add features of a group into this one::
  void merge_peptide_DELTA_group(peptide_DELTA_group*);  
  // set modifications of peptide members:
  void set_PEPTIDE_MODIFICATIONS();
  // get the best observed AC ( best probability )
  ms2_info* get_BEST_MS2_SCAN();
  // get the best observed MS2 scan above input threshold( highest probability )
  ms2_info* get_BEST_MS2_SCAN(double);
  // get all observed MS2 scan above input threshold( highest probability )
  vector<ms2_info*> get_ALL_MS2_SCANS(double );
  // get the best observed SQ
  string get_SQ(){ return get_BEST_MS2_SCAN()->get_SQ();};
  // get the best observed SQ
  string get_MOD_SQ( double PP){ return get_BEST_MS2_SCAN(PP)->get_MOD_SQ();};
  string get_MOD_SQ(){ return get_BEST_MS2_SCAN()->get_MOD_SQ();};
  string get_MS2_TYPE_TAG(){ return get_BEST_MS2_SCAN()->get_MS2_TYPE_TAG();};
  // peptide probablity:
  double get_PEP_PROB(){ return get_BEST_MS2_SCAN()->get_PEP_PROB();};
  double get_PEP_PROB( double PP){ return get_BEST_MS2_SCAN(PP)->get_PEP_PROB();};

  // write out to inclusion list
  void write_2_inclusion_list(ofstream*);
  // write into XML formated text into file:
  void write_content_2_TXT( ofstream* );
  // write into a formated text for each feature:
  void write_Feature_content_2_TXT( ofstream* );

  // get the averaged TIC at a LC/MS point
  pair<double,double> get_peptide_TOTAL_TIC( int );
  // get the averaged TIC over all LC/MS point
  double get_peptide_TOTAL_TIC( );
  // get the delta mass list for a specific ID
  vector<double>* get_LC_MS_Modification_Masses( int );


  // build average peptide cluster profile:
  void build_peptide_average_consens();
  
  // get the average peptide ratio between 2 points:
  vector<double> get_peptide_ratio(int, int );
  // get the peptide ratio for labelled peptides:
  vector<double> get_peptide_ratio( double DELTA_SHIFT);

  // get the smallest charge state of all feature members:
  int get_min_charge_state();


  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

  double get_MR(){return MR;};
  double get_TR(){return TR;};

  int get_nb_members(){return ( MEMBERS.size() + get_modifications() );};
  vector<feature*>* get_members_list(){return &MEMBERS;};
  vector<feature*>::iterator get_members_list_start(){return MEMBERS.begin();};
  vector<feature*>::iterator get_members_list_end(){return MEMBERS.end();};
  
  int get_modifications(){return MODIFIED_MEMBERS.size();};
  map<double, vector<feature*> >* get_modified_members_list(){return &MODIFIED_MEMBERS;};
  map<double, vector<feature*> >::iterator get_modified_members_list_start(){return MODIFIED_MEMBERS.begin();};
  map<double, vector<feature*> >::iterator get_modified_members_list_end(){return MODIFIED_MEMBERS.end();};
  bool get_modification_state(){return !MODIFIED_MEMBERS.empty();};
  
  map<string, vector<ms2_info*> >::iterator get_MS2_INFO_START(){ return PROTEIN_ACs.begin();};
  map<string, vector<ms2_info*> >::iterator get_MS2_INFO_END(){ return PROTEIN_ACs.end();};

  
  map<int, double>* get_PROFILE_AVERAGE_CONSENS(){return &AVERAGE_PEPTIDE_PROFILE;};


  bool MS2_cluster_info(){ if( PROTEIN_ACs.empty() ) return false; else return true;};
  map<string, vector<ms2_info*> >* get_MS2_cluster(){ return &(PROTEIN_ACs);};
  bool Protein_CHECK(string);
  
  void set_MASTER_ID(int IN){ MASTER_ID = IN;};
  int get_MASTER_ID(){ return MASTER_ID;};

  void set_PROFILE_SCORE(double IN){PROFILE_SCORE = IN;};
  double get_PROFILE_SCORE(){return PROFILE_SCORE;};

  void set_PROFILE_PROBABILITY(double IN){PROFILE_PROBABILITY = IN;};
  double get_PROFILE_PROBABILITY(){return PROFILE_PROBABILITY;};
  
  void set_PEPTIDE_GROUP_ID(int IN){ PEPTIDE_GROUP_ID = IN;};
  int get_PEPTIDE_GROUP_ID(){ return PEPTIDE_GROUP_ID;};
  
  // check if its proteotypic:
  bool get_PROTEO_TYPE();
  
  // assess the pI parameter
  double get_PEPTIDE_PI(){return pI;};
  void set_PEPTIDE_PI( double IN){pI = IN;};
  


};

#endif

    

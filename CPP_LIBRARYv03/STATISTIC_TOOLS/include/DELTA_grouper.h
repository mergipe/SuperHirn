///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS DELTA_grouper:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef DELTA_GROUPER_H
#define DELTA_GROUPER_H



class DELTA_grouper{

  ////////////////////////////////////////////////
  // declaration of the private members:

  
private:
  
  bool PRINT_CLUSTER;
  LC_MS* MASTER;
  
  map<int, vector<double> > TARGET_CLUSTER_PROFILES;
  map<int, vector<double> > LC_MS_DELTA_masses;
  
  double TR_TOL;
  double PEP_PROBABILITY_THRESHOLD;
  
  vector<peptide_DELTA_group> PEPTIDE_GROUPS;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
    
    static bool MS_2_SELECTION;
  static int MIN_NB_PROFILE_POINTS;

  // class destructor
  ~DELTA_grouper();
  
  // class constructor
  DELTA_grouper();
  // class constructor
  DELTA_grouper(LC_MS*);
  // class copy constructor
  DELTA_grouper(const DELTA_grouper&);
  // class copy constructor
  DELTA_grouper(const DELTA_grouper*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  DELTA_grouper& operator=(const DELTA_grouper&);
  DELTA_grouper& operator<=(const DELTA_grouper&);
  DELTA_grouper& operator>=(const DELTA_grouper&);
  DELTA_grouper& operator<(const DELTA_grouper&);
  DELTA_grouper& operator>(const DELTA_grouper&);
  
  
  // initialize the parameters:
  void INIT_PARAM();
  // show protein groups:
  void show_peptide_groups();
  // show MASTER peptide groups:
  void show_MASTER_peptide_groups();
  // order peptide groups by their MR:
  void order_peptide_groups_by_MR();
  // write the peptide groups to a file:
  void write_peptide_groups_to_XML( );
  // write the peptide groups to a txt file:
  void write_peptide_groups_to_TXT();
  
  
  // check if contains a ID from every LC/MS:
  bool check_LC_MS_IDS(peptide_DELTA_group*);

  
  // start finding correpsonding features with
  // m/z delta shifts:
  void extract_PEPTIDE_DELTA_GROUPS_from_MASTERMAP();
    
  // start finding correpsonding features with
  // m/z delta shifts:
  void start_MASTER_LC_MS_PAIR_MATCHING();
  // see if it matches to a existing protein_group:
  bool check_peptide_cluster_belonging( feature*, map<double, peptide_DELTA_group>* );
  bool check_peptide_cluster_belonging( feature*, vector<peptide_DELTA_group>* );
  bool check_peptide_cluster_belonging( feature* );
  bool check_peptide_cluster_belonging( peptide_DELTA_group* );


  // find corresponding peptide groups for a feature
  void find_PEPTIDE_GROUPS_for_FEATURE(feature*);
  // groups feature in teh clusters into peptide groups
  // m/by MR:
  void extract_PEPTIDE_GROUPS_from_PROFILE_GROUP(profile_group*);
  // check for modifications in every peptide group in a profile group:
  void extract_PEPTIDE_GROUPS_MODIFICATIONS(profile_group*);
  // find overlap between MASTER peptide groups:
  void compare_MASTER_PEPTIDE_GROUPS();
  // process extracted delta pairs:
  void process_extracted_peptide_DELTA_groups();

  // group the existing peptide clusters into protein_group objects:
  void extract_PROTEIN_GROUPS_from_PROFILE_GROUP(profile_group*);
  // see if it matches to a existing protein_group:
  bool check_protein_cluster_belonging( peptide_DELTA_group*, vector<protein_group>*);
 
  // this structure provides the function to compare
  // protein cluster by their profile score:
  struct PROTEIN_CLUSTER_OPERATOR_by_SCORE{
    // provide the compare function for sort:
    bool operator()(protein_group A, protein_group B) const{
      return A.get_PROFILE_SCORE() < B.get_PROFILE_SCORE();
    }
  };


  // show MASTER protein groups:
  void show_MASTER_protein_groups();

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  vector<peptide_DELTA_group> get_EXTRACTE_PEPTIDE_GROUPS(){return PEPTIDE_GROUPS;};
  vector<double>* get_LC_MS_DELTA_mass(int ID){return &(*LC_MS_DELTA_masses.find(ID)).second;};
  
};

#endif

    

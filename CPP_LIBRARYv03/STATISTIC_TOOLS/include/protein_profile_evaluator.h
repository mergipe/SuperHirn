///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS protein_profile_evaluator:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef PROTEIN_PROFILE_EVALUATOR_H
#define PROTEIN_PROFILE_EVALUATOR_H

typedef vector<peptide_DELTA_group>::iterator PEP_ITERATOR;

class protein_profile_evaluator{

  
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  protein_group* MY_PROTEIN;
  map<int, vector<double> > TARGET_PROFILES;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
    static bool PEPTIDE_PROTEOTYPE_MODE;
  static int NB_PROFILE_DATA_POINTS;
  static double TR_TOL;
  
    // class destructor
  ~protein_profile_evaluator();
  
  // class constructor
  protein_profile_evaluator();
  // class copy constructor
  protein_profile_evaluator(const protein_profile_evaluator&);
  // class copy constructor
  protein_profile_evaluator(const protein_profile_evaluator*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  protein_profile_evaluator& operator=(const protein_profile_evaluator&);
  bool operator<=(const protein_profile_evaluator&);
  bool operator>=(const protein_profile_evaluator&);
  bool operator<(const protein_profile_evaluator&);
  bool operator>(const protein_profile_evaluator&);
  
  
  // detectes peptide outliers in protein:  
  void START_PEPTIDE_OUTLIER_DETECTION( protein_group*);
  // detectes peptide outliers in protein:  
  vector<PEP_ITERATOR> detected_peptide_outliers(  );
  // score vs. average protein profile:
  double compare_profiles( map<int, double>* , map<int, double>* );
  
  // print all peptides of a protein 
  void plot_this_protein( );
  void plot_this_protein( protein_group* );
  void plot_this_protein( vector<protein_group>* , string);
  void plot_this_protein( vector<protein_group>* , int, string);

  // print protein profile to file:
  void print_this_protein_profile( vector<protein_group>* , string);


  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  void set_TARGET_PROFILES( map<int, vector<double> > IN){ TARGET_PROFILES = IN;};
  
  
};

#endif

    

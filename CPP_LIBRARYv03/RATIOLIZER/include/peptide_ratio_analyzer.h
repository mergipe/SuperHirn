///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS peptide_ratio_analyzer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef PEPTIDE_RATIO_ANALYZER_H
#define PEPTIDE_RATIO_ANALYZER_H

typedef struct{
  
  peptide_DELTA_group* TARGET;
  int ID;

  // averaged value
  double AVERAGE_PEPTIDE_RATIO;
  double AVERAGE_PEPTIDE_RATIO_STD;
  
  // normalized value:
  double NORMALIZED_AVERAGE_PEPTIDE_RATIO;
  
  // confidence assessment:
  double P_RATIO_VALUE;
  
  // raw feature ratios
  vector< pair<double,double> > ALL_FEATURE_RATIOS;
  
  // if anaylsis is valid:
  bool validAnalysis;
  
}
PEPTIDE_RATIO;



class peptide_ratio_analyzer{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:

  static int RATIO_TARGET_LC_MS;
  static int REPLICATE_NUMBER;
  // select if only feature observed all replicates used
  static bool REPLICATE_CHECK;
  static int TARGET_ID;
  static map<int, vector<double> > LC_MS_Modification_Masses;

  // class destructor
  ~peptide_ratio_analyzer();
  
  // class constructor
  peptide_ratio_analyzer();
  // class copy constructor
  peptide_ratio_analyzer(const peptide_ratio_analyzer&);
  // class copy constructor
  peptide_ratio_analyzer(const peptide_ratio_analyzer*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  peptide_ratio_analyzer& operator=(const peptide_ratio_analyzer&);
  bool operator==(const peptide_ratio_analyzer&);
  peptide_ratio_analyzer& operator<=(const peptide_ratio_analyzer&);
  peptide_ratio_analyzer& operator>=(const peptide_ratio_analyzer&);
  peptide_ratio_analyzer& operator<(const peptide_ratio_analyzer&);
  peptide_ratio_analyzer& operator>(const peptide_ratio_analyzer&);
  
  // this structure provides the function to compare
  // features in the sorting algorithm:
  struct OPERATOR_MZ{
    // provide the compare function for sort:
    bool operator()(const feature A,const feature B) const{
      // check if they have same mass
      if(A.MONO_MZ == B.MONO_MZ){
        return A.TR < B.TR;
      }
      else{
        return A.MONO_MZ < B.MONO_MZ;
      }
    }
  };
  
  
  ///////////////////////////////////////////////////////////////////
  // Differential inter LC/MS analysis
  //
  // analyze this peptide group:
  PEPTIDE_RATIO analyze_peptide( peptide_DELTA_group* );
  // compute the total peptide ratio from the features ratios:
  void analyze_peptide_DELTA_group( PEPTIDE_RATIO* );
  // compute the total peptide ratio from the features ratios:
  void compute_PEPTIDE_ratio( PEPTIDE_RATIO* );
  // compute the ratio of a features:
  pair<double, double> get_FEATURE_RATIO( feature* );
  // computes the stdev incorporates peptide average ratio
  double computeAverageStdevWeightedRatio( vector< pair<double,double> >*);
  // computes the stdev incorporates peptide average ratio
  double computeAverageRatioStdev( vector< pair<double,double> >* );

  
  ///////////////////////////////////////////////////////////////////
  // ISOTOP analysis
  //
  // analyze now the isotopes, are all in teh same LC/MS run:
  void analyze_ISOTOPE_peptide_DELTA_group( PEPTIDE_RATIO*);
  // compare if masses A/B match ( check for modification shift)
  bool compare_Mass( double , double );


  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

};

#endif

    

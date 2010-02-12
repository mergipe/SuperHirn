///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS match_region:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef MATCH_REGION_H
#define MATCH_REGION_H

// data structre to save potential match candidates
typedef match_region_candidate CANDIDATE;


class match_region{
  
  
  ////////////////////////////////////////////////
  // declaration of the private members:
  int chrg;
  double Mz;
  double Tr;
  double T_LOW;
  double T_HIGH;
  vector<CANDIDATE> CANDIDATE_LIST;
  vector<CANDIDATE> BEST_CANDIDATE;
  feature* TARGET;
  
  MS2SpectraCorrelator* ms2Correlator;
  
private:
    
    ////////////////////////////////////////////////
    // declaration of the public members:

public:
    
    // class destructor
    ~match_region();
  
  // class constructor
  match_region(int, double, double, double, double);
  match_region( feature* );
  match_region( );
  // class copy constructor
  match_region(const match_region&);
  match_region(const match_region*);

  
  //////////////////////////////////////////////////
  // overload operators:
  match_region& operator=(const match_region&);
  match_region& operator<=(const match_region&);
  match_region& operator>=(const match_region&);
  match_region& operator<(const match_region&);
  match_region& operator>(const match_region&);
  
  
  // check if this fetaure is a candidate for the 
  // target feature
  void checkFeatureCandidate(feature* IN );    
  // adds a candidate to the lc_peak list: 
  void add_match_candidate( feature* );
		
  // get the best match, which is the last one remaining in a vector
  feature* get_best_match(){return (*BEST_CANDIDATE.begin()).myFeature;};
  
  // match the input lc_peak from LC_MS run B against the extracted region:
  // m/z has already been compared, check now which one is closest to the peak
  // and compare also if you can find similar charge states peptides
  void find_best_match(feature*);
  void find_best_match();
  
  // compare the charge state pattern of 2 features:
  int compare_charge_states(feature*, feature*);
  
  // remove a candidate from the list:
  void remove_candidate( feature* );
  // removes the best match from the list:
  void remove_best_match(  );
  // compare the two best matches of this to another 
  // match region and decided who should keep it:
  void compareBestCandidates( match_region* );
    
  
  ///////////////////////////////////////////////////
  // compute the MS2-MS2 spectra correlation
  double getMS2SpectraCorrelation( feature* );
  double getMS2SpectraCorrelation( feature* , feature* );
  MS2CorrelationAnalysis* getMS2SpectraCorrelationOfTwoFeatures( feature*,  feature* );

  // compute the aligned MS2-MS2 spectra correlation between two features:
  double getAlignedMS2SpectraCorrelation( feature*, feature*);

  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  int get_nb_candidates(){return CANDIDATE_LIST.size();};
  int get_charge_state(){return chrg;};
  double get_retention_time(){return Tr;};
  double get_Tr_low(){return T_LOW;};
  void set_Tr_low(double in){T_LOW = in;};
  double get_Tr_high(){return T_HIGH;};
  void set_Tr_high(double in){T_HIGH = in;};
  double get_MZ(){return Mz;};
  
  // get the last candidate in the list
  CANDIDATE get_last_candidate(){
    vector<CANDIDATE>::iterator p = CANDIDATE_LIST.end();
    p--;
    return (*p);
  }
  
  
  void set_TARGET(feature* IN){ TARGET = IN;};
  feature* get_TARGET(){ return TARGET;};
  
};

#endif

    

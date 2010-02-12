///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS profiling_evaluator:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef PROFILING_EVALUATOR_H
#define PROFILING_EVALUATOR_H



class profiling_evaluator{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  profile_comparer* PROFILING;
  
  // individual scores:
  double PROTEIN_CORRELATION_SCORE;
  map<int, vector<double> > TARGET_PROFILES;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~profiling_evaluator();
  
  // class constructor
  profiling_evaluator();
  // class constructor
  profiling_evaluator( profile_comparer* );
  // class copy constructor
  profiling_evaluator(const profiling_evaluator&);
  // class copy constructor
  profiling_evaluator(const profiling_evaluator*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  profiling_evaluator& operator=(const profiling_evaluator&);
  bool operator==(const profiling_evaluator&);
  profiling_evaluator& operator<=(const profiling_evaluator&);
  profiling_evaluator& operator>=(const profiling_evaluator&);
  profiling_evaluator& operator<(const profiling_evaluator&);
  profiling_evaluator& operator>(const profiling_evaluator&);
  
  
  //////////////////////////////////////////////////
  // start the evaluation process
  void START_PROFILING_EVALUATION( );
  // build protein correaltion score
  void BUILD_PROTEIN_CORRELATION_SCORE();
  // score a protein vs. target:
  double score_protein_vs_target( protein_group* , vector<double>* );
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  void set_TARGET_PROFILES( map<int, vector<double> > IN){ TARGET_PROFILES = IN;};

};

#endif

    

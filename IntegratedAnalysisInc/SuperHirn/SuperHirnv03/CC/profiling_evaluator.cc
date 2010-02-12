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

#define USE_PROFILING_EVALUATOR
#include "use.h"




////////////////////////////////////////////////
// constructor for the object profiling_evaluator:
profiling_evaluator::profiling_evaluator(){
  PROFILING = NULL;
  PROTEIN_CORRELATION_SCORE = -1;
}


////////////////////////////////////////////////
// constructor for the object profiling_evaluator:
profiling_evaluator::profiling_evaluator( profile_comparer* IN){
  PROFILING = IN;
  PROTEIN_CORRELATION_SCORE = -1;
}


//////////////////////////////////////////////////
// class desctructor of profiling_evaluator
profiling_evaluator::~profiling_evaluator(){
  PROFILING = NULL;
}

//////////////////////////////////////////////////
// class copy constructor of profiling_evaluator
profiling_evaluator::profiling_evaluator(const profiling_evaluator& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of profiling_evaluator
profiling_evaluator::profiling_evaluator(const profiling_evaluator* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
profiling_evaluator& profiling_evaluator::operator=(const profiling_evaluator& tmp){
  return *this;
}

//////////////////////////////////////////////////
// start the evaluation process
void profiling_evaluator::START_PROFILING_EVALUATION( ){
  if( PROFILING != NULL ){
    printf("\n\t\tStart Profiling analysis:\n");
    
    printf("\t\t\t- build protein correlation score...\n");
    BUILD_PROTEIN_CORRELATION_SCORE();
    printf("\t\t\t-> protein correlation score = %0.4f\n", PROTEIN_CORRELATION_SCORE );
    
    


  }
  else{
    printf("\t\t\t-> Profile analysis not set! (profiling_evaluator::74)\n");
  }
}



//////////////////////////////////////////////////
// build protein correaltion score
void profiling_evaluator::BUILD_PROTEIN_CORRELATION_SCORE(){
  PROTEIN_CORRELATION_SCORE = 0;
  printf("\t\t\t\t");
  /*
  // go through the best fit proteins determine a score
  // how well the fit to their target profiles:
  map<int, vector<protein_group> >::iterator PROT = PROFILING->get_BEST_PROTEINS_START();
  while( PROT != PROFILING->get_BEST_PROTEINS_END() ){
    
    vector<double> MY_TARGET = PROFILING->get_TARGET_CLUSTER_PROFILES( (*PROT).first );
    if( !MY_TARGET.empty() ){
      double this_s = score_protein_vs_target( &((*PROT).second) , &MY_TARGET );
      printf("#%d:%s(%0.3f)\t", (*PROT).first, (*PROT).second.get_AC().c_str(), this_s );
      PROTEIN_CORRELATION_SCORE += this_s;
    }

    PROT++; 
  }
  */
  
  printf("\n");
}



///////////////////////////////////////////////////////////
// score a protein vs. target:
double profiling_evaluator::score_protein_vs_target( protein_group* IN, vector<double>* TARGET){

  double SCORE = 0;
  profile_scorer* COMP = new profile_scorer( *TARGET ); 
  SCORE = COMP->COMPARE_TO_TARGET_PROFILE( IN->get_PROFILE_AVERAGE_CONSENS() );
    
  delete COMP;
  COMP = NULL;
  IN = NULL;
  TARGET = NULL;
  return SCORE;
}
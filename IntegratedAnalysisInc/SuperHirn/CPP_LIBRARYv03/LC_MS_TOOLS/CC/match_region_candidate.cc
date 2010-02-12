///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS match_region_candidate:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_MATCH_REGION_CANDIDATE
#include "LC_MS_TOOLS_USE.h"


////////////////////////////////////////////////
// constructor for the object match_region_candidate:
match_region_candidate::match_region_candidate( ){
}  

////////////////////////////////////////////////
// constructor for the object match_region_candidate:
match_region_candidate::match_region_candidate( feature* in){
  // initialize:
  myFeature = in;
  ms2Correlation = -10;
  trDiff = 0;
  areaDiff = 0;		
  mzDiff = 0;		
  
}

//////////////////////////////////////////////////
// class desctructor of match_region_candidate
match_region_candidate::~match_region_candidate(){
  myFeature = NULL;
}

//////////////////////////////////////////////////
// class copy constructor of match_region_candidate
match_region_candidate::match_region_candidate(const match_region_candidate& tmp){

  myFeature = tmp.myFeature;
  // ms2 spectrum correlation:
  ms2Correlation = tmp.ms2Correlation;
  // tr differnce:
  trDiff = tmp.trDiff;
		// intensity difference:
  areaDiff = tmp.areaDiff;		
  //mz difference:
  mzDiff = tmp.mzDiff;		
  
}

//////////////////////////////////////////////////
// class copy constructor of match_region_candidate
match_region_candidate::match_region_candidate(const match_region_candidate* tmp){
  myFeature = tmp->myFeature;
  // ms2 spectrum correlation:
  ms2Correlation = tmp->ms2Correlation;
  // tr differnce:
  trDiff = tmp->trDiff;
		// intensity difference:
  areaDiff = tmp->areaDiff;		
  //mz difference:
  mzDiff = tmp->mzDiff;		
}

//////////////////////////////////////////////////
// copy constructor:
match_region_candidate& match_region_candidate::operator=(const match_region_candidate& tmp){
  myFeature = tmp.myFeature;
  // ms2 spectrum correlation:
  ms2Correlation = tmp.ms2Correlation;
  // tr differnce:
  trDiff = tmp.trDiff;
		// intensity difference:
  areaDiff = tmp.areaDiff;		
  //mz difference:
  mzDiff = tmp.mzDiff;		
  return *this;
}
    
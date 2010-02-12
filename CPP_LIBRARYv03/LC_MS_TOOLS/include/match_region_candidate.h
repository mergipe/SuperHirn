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


#ifndef USE_MATCH_REGION_CANDIDATE_H
#define USE_MATCH_REGION_CANDIDATE_H



class match_region_candidate{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  feature* myFeature;
  
  // ms2 spectrum correlation:
  double ms2Correlation;
  // tr differnce:
  double trDiff;
		// intensity difference:
  double areaDiff;		
  //mz difference:
  double mzDiff;		
  
  
  // class destructor
  ~match_region_candidate();
  
  // class constructor
  match_region_candidate();
  match_region_candidate( feature*);
  // class copy constructor
  match_region_candidate(const match_region_candidate&);
  // class copy constructor
  match_region_candidate(const match_region_candidate*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  match_region_candidate& operator=(const match_region_candidate&);
  bool operator==(const match_region_candidate&);
  match_region_candidate& operator<=(const match_region_candidate&);
  match_region_candidate& operator>=(const match_region_candidate&);
  match_region_candidate& operator<(const match_region_candidate&);
  match_region_candidate& operator>(const match_region_candidate&);
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

};

#endif

    

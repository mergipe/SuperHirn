///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS cell:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_CELL
#include "CLUSTER_LC_MS_ANALYSIS_USE.h"


//////////////////////////////////////////////////
// class constructor of cell
cell::cell(){
  score = 0;
  
}


////////////////////////////////////////////////
// constructor for the object cell:
cell::cell(string IN_x, string IN_y, double IN_s){
  X = IN_x;
  Y = IN_y;
  score = IN_s;
}

//////////////////////////////////////////////////
// copy constructor:
cell::cell(const cell& tmp){

    X = tmp.X;
    Y = tmp.Y;
    score = tmp.score;
}

//////////////////////////////////////////////////
// copy constructor:
cell::cell(const cell* tmp){
  
  X = tmp->X;
  Y = tmp->Y;
  score = tmp->score;
}


//////////////////////////////////////////////////
// class desctructor of cell
cell::~cell(){
  

}


//////////////////////////////////////////////////
// copy constructor:
cell& cell::operator=(const cell& tmp){  

  X = tmp.X;
  Y = tmp.Y;
  score = tmp.score;

  return *this;
}
    

///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS matrix_element:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_CELL
#define USE_MATRIX_ELEMENT

#include "CLUSTER_LC_MS_ANALYSIS_USE.h"


//////////////////////////////////////////////////
// class constructor of matrix_element
matrix_element::matrix_element(){
    id = 0;
    edge_A = 0;
    edge_B = 0;
}

//////////////////////////////////////////////////
// copy constructor:
matrix_element::matrix_element(const matrix_element& tmp){
    edge_A = tmp.edge_A;
    edge_B = tmp.edge_B;
    name = tmp.name;
    id = tmp.id;
}


////////////////////////////////////////////////
// constructor for the object matrix_element:
matrix_element::matrix_element(string IN_name, int IN_id){
  name = IN_name;
  id = IN_id;
  edge_A = 0;
  edge_B = 0;
}


//////////////////////////////////////////////////
// class desctructor of matrix_element
matrix_element::~matrix_element(){
  

}


//////////////////////////////////////////////////
// copy constructor:
matrix_element& matrix_element::operator=(const matrix_element& tmp){

    edge_A = tmp.edge_A;
    edge_B = tmp.edge_B;
    name = tmp.name;
    id = tmp.id;
    return *this;
}
    

///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS SM_SM_cell:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_SM_CELL
#include "LC_MS_TOOLS_USE.h"

string SM_cell::SCORE_MODUS;


//////////////////////////////////////////////////
// class constructor of SM_cell
SM_cell::SM_cell(){
  MATCH_SCORE = 0;
  INT_SCORE = 0;
  ALIGN_SCORE = 0;
  TOTAL_SCORE = 0;
  NORM_SCORE = 0;
  ID = -1;
}


////////////////////////////////////////////////
// constructor for the object SM_cell:
SM_cell::SM_cell(int IN_x, int IN_y){
  X = IN_x;
  Y = IN_y;
  MATCH_SCORE = 0;
  INT_SCORE = 0;
  ALIGN_SCORE = 0;
  TOTAL_SCORE = 0;
  NORM_SCORE = 0;
  ID = -1;
}

////////////////////////////////////////////////
// constructor for the object SM_cell:
SM_cell::SM_cell(LC_MS* A, LC_MS* B){
  X = A->get_spectrum_ID();
  Y = B->get_spectrum_ID();
  X_name = A->get_spec_name(); 
  Y_name = B->get_spec_name(); 
  X_number_features = A->get_nb_features();
  Y_number_features = B->get_nb_features();
  MATCH_SCORE = 0;
  INT_SCORE = 0;
  ALIGN_SCORE = 0;
  TOTAL_SCORE = 0;
  NORM_SCORE = 0;
   ID = -1;
}


//////////////////////////////////////////////////
// copy constructor:
SM_cell::SM_cell(const SM_cell& tmp){
  X = tmp.X;
  Y = tmp.Y;
  X_name = tmp.X_name;
  Y_name = tmp.Y_name;  
  X_number_features = tmp.X_number_features;
  Y_number_features = tmp.Y_number_features;  
  MATCH_SCORE = tmp.MATCH_SCORE;
  INT_SCORE = tmp.INT_SCORE;
  ALIGN_SCORE = tmp.ALIGN_SCORE;
  TOTAL_SCORE = tmp.TOTAL_SCORE;
  NORM_SCORE = tmp.NORM_SCORE;
  ID = tmp.ID;
}

//////////////////////////////////////////////////
// copy constructor:
SM_cell::SM_cell(const SM_cell* tmp){
  X = tmp->X;
  Y = tmp->Y;
  X_name = tmp->X_name;
  Y_name = tmp->Y_name;
  X_number_features = tmp->X_number_features;
  Y_number_features = tmp->Y_number_features;  
  MATCH_SCORE = tmp->MATCH_SCORE;
  INT_SCORE = tmp->INT_SCORE;
  ALIGN_SCORE = tmp->ALIGN_SCORE;
  TOTAL_SCORE = tmp->TOTAL_SCORE;
  NORM_SCORE = tmp->NORM_SCORE;
  ID = tmp->ID;
}


//////////////////////////////////////////////////
// class desctructor of SM_cell
SM_cell::~SM_cell(){
}


//////////////////////////////////////////////////
// copy constructor:
SM_cell& SM_cell::operator=(const SM_cell& tmp){  
  X = tmp.X;
  Y = tmp.Y;
  X_name = tmp.X_name;
  Y_name = tmp.Y_name;
  X_number_features = tmp.X_number_features;
  Y_number_features = tmp.Y_number_features;  
  MATCH_SCORE = tmp.MATCH_SCORE;
  INT_SCORE = tmp.INT_SCORE;
  ALIGN_SCORE = tmp.ALIGN_SCORE;
  TOTAL_SCORE = tmp.TOTAL_SCORE;
  NORM_SCORE = tmp.NORM_SCORE;
  ID = tmp.ID;
  return *this;
}



//////////////////////////////////////////////////
// OUT score:
double SM_cell::get_OUT_SCORE(){
  
  if( SCORE_MODUS == "TOTAL" )
    return get_TOTAL_SCORE();
  
  if( SCORE_MODUS ==  "ALIGN")
    return get_ALIGN_SCORE();
  
  if( SCORE_MODUS ==  "INTNES") 
    return get_INT_SCORE();
  
  if( SCORE_MODUS ==  "PEAK_MATCHING" )
    return get_MATCH_SCORE();
  
  if( SCORE_MODUS == "NORM_TOTAL")
    return get_NORM_SCORE();
  
  return get_TOTAL_SCORE();

}


////////////////////////////////////////////////
// write to XML:
void SM_cell::write_to_XML( ofstream* WRITER){
  
  
  char buffer[255];
  
  sprintf(buffer, "<ELEMENT x_name=\"%s\" y_name=\"%s\" x_nb_features=\"%d\" y_nb_features=\"%d\">\n", X_name.c_str(), Y_name.c_str(), X_number_features, Y_number_features);  
  WRITER->write(buffer,strlen(buffer));

  // scores:
  sprintf(buffer, "<SCORE total=\"%f\" allign=\"%f\" intens=\"%f\" match=\"%f\" ></SCORE>\n", get_OUT_SCORE(), ALIGN_SCORE, INT_SCORE, MATCH_SCORE);  
  WRITER->write(buffer,strlen(buffer));

  
  sprintf(buffer, "</ELEMENT>\n");  
  WRITER->write(buffer,strlen(buffer));

  
  WRITER = NULL;
}



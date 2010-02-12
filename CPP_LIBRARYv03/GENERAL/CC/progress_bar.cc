///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS progress_bar:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#include <iostream>
#include <cmath>
using namespace std;

#include "progress_bar.h"




////////////////////////////////////////////////
// constructor for the object progress_bar:
progress_bar::progress_bar(int IN_iterations, string IN_text){

  internal_counter = 1;  
  progress_text = IN_text;
  
  // define number of iterations:
  nb_iterations = (unsigned long)(IN_iterations);
  
  // define the update step (%):
  update_step = 10;
  progress = 0;
  print_endline = false;
  set_progress();
}

////////////////////////////////////////////////
// constructor for the object progress_bar:
progress_bar::progress_bar(int IN_iterations){
  
  internal_counter = 1;
  
  progress_text = "updated";
  
  // define number of iterations:
  nb_iterations = (unsigned long)(IN_iterations);
  
  // define the update step (%):
  update_step = 10.0;
  progress = 0;
  print_endline = false;
  set_progress();
}


//////////////////////////////////////////////////
// class desctructor of progress_bar
progress_bar::~progress_bar(){
  if( print_endline ){
    cout<<endl;  
  }
}

//////////////////////////////////////////////////
// copy constructor:
progress_bar& progress_bar::operator=(const progress_bar& tmp){
  return *this;
}

//////////////////////////////////////////////////
// calculated the progress:
void progress_bar::set_progress(){
  progress = ceil(progress + (double)(nb_iterations) / update_step);
}


//////////////////////////////////////////////////
// update the progress:
void progress_bar::update_progress(int it){
  
  // add one to the counter it since usually start at 0:
  it++;
  if(progress == (double)(it)){
    print_progress((100*it)/nb_iterations);
    set_progress();
  }
  
}

//////////////////////////////////////////////////
// update the progress:
void progress_bar::update_progress(){
  
  internal_counter++;
  if(progress == (double)(internal_counter)){
    print_progress((100*internal_counter)/nb_iterations);
    set_progress();
  }
  
}


//////////////////////////////////////////////////
// print the progress:
void progress_bar::print_progress(int it){
    
  print_endline = true;  
  printf("\t");
  cout<<it<<"% ";
  cout<<progress_text;
  cout<<"... ";
  cout.flush();
  
  
  if( it == 50 ){
    printf("\t\n");
    // cout<<endl;
  }
  
  
}

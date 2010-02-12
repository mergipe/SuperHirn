///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS alignment_pair:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_SPEC_CORRELATION
#define USE_NODE
#define USE_ALIGNMENT_PAIR

#include "LC_MS_TOOLS_USE.h"



////////////////////////////////////////////////
// constructor for the object alignment_pair:
alignment_pair::alignment_pair(string IN){

  // save the spectrum data:
  // name:
  name = IN;
  correlation_before = 0;
  correlation_after = 0;
}

//////////////////////////////////////////////////
// copy constructor:
alignment_pair::alignment_pair(const alignment_pair& tmp){
  name.empty();
  name = tmp.name;
  correlation_before = tmp.correlation_before;
  correlation_after = tmp.correlation_after;
  comparison_list = tmp.comparison_list;
}

//////////////////////////////////////////////////
// copy constructor:
alignment_pair::alignment_pair(const alignment_pair* tmp){
  name = tmp->name;
  correlation_before = tmp->correlation_before;
  correlation_after = tmp->correlation_after;
  comparison_list = tmp->comparison_list;
}

//////////////////////////////////////////////////
// class desctructor of alignment_pair
alignment_pair::~alignment_pair(){
  
  // delete the spectra names:
  name.erase();
  // delete list:
  comparison_list.clear();
}

//////////////////////////////////////////////////
// copy constructor:
alignment_pair& alignment_pair::operator=(const alignment_pair& tmp){
  
  name.empty();
  name = tmp.name;
  correlation_before = tmp.correlation_before;
  correlation_after = tmp.correlation_after;
  comparison_list.clear();
  comparison_list = tmp.comparison_list;
  
  return *this;
}

//////////////////////////////////////////////////
// equal operator:
bool alignment_pair::operator==(const string& IN){

  if(this->get_name() == IN)
    return true;
  else
    return false;
}


//////////////////////////////////////////////////
// add comparisons of spectra
void alignment_pair::add_comparison_pair(string IN){
    
  alignment_pair* tmp = new alignment_pair(IN);
  
  // add the new comparision pair:
  add_to_comparison_list(*tmp);
  tmp = NULL;
  
}

//////////////////////////////////////////////////////////
// search the comparison list for a pair by the spectrum name
alignment_pair* alignment_pair::search_comparison_list_by_name(string IN){
  vector<alignment_pair>::iterator p;
  p = find(comparison_list.begin(),comparison_list.end(), IN);
  return &*p;
}


////////////////////////////////////////////////////
// compare the overlap of this to another spectrum
// AFTER the aliignment process:
void alignment_pair::set_pair_correlation_after(string A, double correlation){
  
  // find first the partner in the comparison_list
  alignment_pair* partner = search_comparison_list_by_name(A);
  
  // now set the overlap:
  partner->set_correlation_after(correlation);
}

////////////////////////////////////////////////////
// compare the overlap of this to another spectrum
// BEFORE the aliignment process:
void alignment_pair::set_pair_correlation_before(string A, double correlation){
  
  // find first the partner in the comparison_list
  alignment_pair* partner = search_comparison_list_by_name(A);
  
  // now set the overlap:
  partner->set_correlation_before(correlation);
}

////////////////////////////////////////////////////
// compare the overlap of this to partner spectrum
// AFTER the alignment process:
void alignment_pair::set_correlation_after(double IN){
  correlation_after = IN;
}

////////////////////////////////////////////////////
// compare the overlap of this to partner spectrum
// BEFORE the alignment process:
void alignment_pair::set_correlation_before(double IN){
  correlation_before = IN;
}



//////////////////////////////////////////////////////////////
// shows the alignment result in nice table format:
void alignment_pair::print_pair_alignment_overview(){
  
  string sep = ";";
  
  // print out the spectra header:
  vector<alignment_pair>::iterator p = comparison_list.begin();
  
  while(p != comparison_list.end()){
    
    printf("%f|%f%s", (*p).get_correlation_after(),(*p).get_correlation_before(),sep.c_str());
    p++;
  }
  
  
}


/////////////////////////////////////////////////////////////
// show content:
void alignment_pair::show_info(){
  printf("%s\n", name.c_str());
}

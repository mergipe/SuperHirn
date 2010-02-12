///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS result_manager:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_ALIGNMENT_PAIR
#define USE_RESULT_MANAGER

#include "LC_MS_TOOLS_USE.h"




////////////////////////////////////////////////
// constructor for the object result_manager:
result_manager::result_manager(){
}

//////////////////////////////////////////////////
// class desctructor of result_manager
result_manager::~result_manager(){

    alignment_partner.clear();
}


//////////////////////////////////////////////////
// copy constructor:
result_manager& result_manager::operator=(const result_manager& tmp){
  return *this;
}


///////////////////////////////////////////////////
// create a new alignment partner which can then be compared to others:
void result_manager::set_new_alignment_partner(string A){

    alignment_pair* tmp = new alignment_pair(A);
    // add to the vector:
    alignment_partner.push_back(*tmp);
    tmp = NULL;
}



///////////////////////////////////////////////////
// create a new alignment pair
void result_manager::create_new_pair(string daddy_o, string B){

    // find first the corresonding partner by spec_name daddy_o
    alignment_pair* partner = get_partner_by_name(daddy_o);

    // add now the pair_partner to the partner:
    partner->add_comparison_pair(B);

}

///////////////////////////////////////////////////
// calculate the overalap of an alignment pair BEFORE
// alignment
void result_manager::set_correlation_after(string A, string B, double correlation){

    // get first the corresponding partner A
    alignment_pair* partner_A = get_partner_by_name(A);
    partner_A->set_pair_correlation_after(B, correlation);

}

///////////////////////////////////////////////////
// calculate the correlation of an alignment pair BEFOR
// alignment
void result_manager::set_correlation_before(string A, string B, double correlation){

    // get first the corresponding partner A
    alignment_pair* partner_A = get_partner_by_name(A);
    partner_A->set_pair_correlation_before(B, correlation);

}

//////////////////////////////////////////////////////////
// search the alignment partner list for a pratner by name
alignment_pair* result_manager::get_partner_by_name(string IN){

    vector<alignment_pair>::iterator p;
    p = find(alignment_partner.begin(),alignment_partner.end(), IN);
    
    return &*p;

}


//////////////////////////////////////////////////////////////
// shows the alignment result in nice table format:
void result_manager::show_alignment_overview(){

  string sep = ";";


    int count = 0, c = 0;
    
    // print the overview titel
    print_titel();

    // print out the spectra header:
    vector<alignment_pair>::iterator p = alignment_partner.begin();
    printf("%s",sep.c_str());
    
    while(p != alignment_partner.end()){
        cout<<(*p).get_name()<<sep;
        count++;
        p++;
    }

    // now go through the list and print overlaps:
    p = alignment_partner.begin();
    
    c = 1;
    while(p != alignment_partner.end()){
      
      cout<<endl<<(*p).get_name()<<sep;
      
      for(int i = 0 ; i < c; i++){
	printf("-%s",sep.c_str());
      }
      
      
      // now the different pairs:
      (*p).print_pair_alignment_overview();
      
      c++;
      p++;
      
    }
    
}

//////////////////////////////////////////////////////////////
// print overview tutel
void result_manager::print_titel(){
    
    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"////////////////////////////////////////////////////////"<<endl;
    printf(" Alignment overview::\n");
        

        
}

//////////////////////////////////////////////////////////////
// show alignment info
void result_manager::show_info(){

  vector<alignment_pair>::iterator p = alignment_partner.begin();
  
  cout<<endl;
  
  while(p != alignment_partner.end()){
    cout<<endl<<" "<<(*p).get_name()<<":\t"<<(*p).get_nb_comparison();
    p++;
  }
    

}




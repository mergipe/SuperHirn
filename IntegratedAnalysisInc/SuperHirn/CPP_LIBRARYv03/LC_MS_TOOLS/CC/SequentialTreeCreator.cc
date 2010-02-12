///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS SequentialTreeCreator:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_SEQUENTIAL_TREE_CREATOR
#include "LC_MS_TOOLS_USE.h"

map<int, LC_MS*> SequentialTreeCreator::LC_MS_map;


////////////////////////////////////////////////
// constructor for the object SequentialTreeCreator:
SequentialTreeCreator::SequentialTreeCreator(){
}

//////////////////////////////////////////////////
// class desctructor of SequentialTreeCreator
SequentialTreeCreator::~SequentialTreeCreator(){
}

//////////////////////////////////////////////////
// class copy constructor of SequentialTreeCreator
SequentialTreeCreator::SequentialTreeCreator(const SequentialTreeCreator& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of SequentialTreeCreator
SequentialTreeCreator::SequentialTreeCreator(const SequentialTreeCreator* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
SequentialTreeCreator& SequentialTreeCreator::operator=(const SequentialTreeCreator& tmp){
  return *this;
}
    
//////////////////////////////////////////////////
// start the construction of the alignment tree
void SequentialTreeCreator::constructSequentialAlignmentTree(){
  
  if( !LC_MS_map.empty() ){
  
    mainTree = NULL;
    node_pool.clear();

    
    
    ////////////////////////////////////////
    // order LC_MS runs by decreasing peak number:
    vector<int> orderedLCMSRuns;
    map<int, LC_MS*>::iterator P = LC_MS_map.begin();
    while( P != LC_MS_map.end() ){
      orderedLCMSRuns.push_back( P->first );
      // insert a node for this runs:
      node* tmp = new node(P->first, P->second->get_spec_name());
      node_pool.insert(make_pair(P->first, tmp));            
      P++;
    }
    
    ////////////////////////////////////////
    // order the LC_MS runs
    buildAlignOrder( &orderedLCMSRuns ); 
    
    
    ////////////////////////////////////////
    // order LC_MS runs by decreasing peak number:
    /*
    map<int, int> orderedRuns;
    map<int, LC_MS*>::iterator P = LC_MS_map.begin();
    while( P != LC_MS_map.end() ){
      orderedRuns.insert( make_pair( P->second->get_nb_features(), P->first ) );
      // insert a node for this runs:
      node* tmp = new node(P->first, P->second->get_spec_name());
      node_pool.insert(make_pair(P->first, tmp));            
      P++;
    }
     */
    
    ////////////////////////////////////////
    // now add them by decreasing peak number:
    vector<int>::iterator R = orderedLCMSRuns.begin();
    
    int newID = LC_MS_map.size();
    int A = *R;
    R++;
    int B = *R;
    // create the first pair:
    create_node( A, B, newID );
    R++;
    
    // go throug the rest
    while( R != orderedLCMSRuns.end() ){
      A = newID;
      B = *R;
      // create the first pair:
      newID++;
      create_node( A, B, newID );
      
      R++;
    }
    
    
    // save the tree:
    
    tree_reader out;
    // out.set_guide_tree( mainTree);
    // out.print_tree();
    // out.save_guide_tree();
    
    
  }
}


////////////////////////////////////////////////////////////////////
// orders teh runs in the alignment order
void SequentialTreeCreator::buildAlignOrder( vector<int>* IN){
  
  // sort the vector according to the number of peaks:
  sort(IN->begin(),IN->end(),OPERATOR_NB_PEAKS());



}

////////////////////////////////////////////////////////////////////
// fuses 2 leafs into a node:
void SequentialTreeCreator::create_node( int A,  int B, int newID){
  
  int level = 0;
  
  node* n_A = search_node_by_id(A);
  node* n_B = search_node_by_id(B);
  
  if(n_A->get_level() >n_B->get_level())
    level = n_A->get_level();
  else
    level = n_B->get_level();
  
  level++;  
    
  node* tmp = new node(n_A , n_B, level, newID);
  node_pool.insert(make_pair(newID, tmp));
  
  // save the top level:
  if( mainTree == NULL){
    mainTree = tmp;
  }
  else{
    if( mainTree->get_level() < level)
      mainTree = tmp;
  }
  
  n_A = n_B = NULL;
  tmp = NULL;
}


////////////////////////////////////////////////////////////////////
// searches the pool of nodes
node* SequentialTreeCreator::search_node_by_id(int id){
  
  map<int, node*>::iterator p = node_pool.begin();
  
  while(p != node_pool.end()){
    
    if((*p).second->get_id() == id){
      return (*p).second;
    }
    p++;
  }
  return NULL;
}


///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS LC_MS_alignment_tree_parser:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_LC_MS_ALIGNMENT_TREE_PARSER
#include "LC_MS_TOOLS_USE.h"


////////////////////////////////////////////////
// constructor for the object LC_MS_alignment_tree_parser:
LC_MS_alignment_tree_parser::LC_MS_alignment_tree_parser( node* IN_tree ){
  alignment_tree = IN_tree;	
}

////////////////////////////////////////////////
// constructor for the object LC_MS_alignment_tree_parser:
LC_MS_alignment_tree_parser::LC_MS_alignment_tree_parser( ){
  alignment_tree = NULL;
  // read the alignment tree:
  read_alignment_tree();
}


//////////////////////////////////////////////////
// class desctructor of LC_MS_alignment_tree_parser
LC_MS_alignment_tree_parser::~LC_MS_alignment_tree_parser(){
  if( alignment_tree != NULL ){
    delete alignment_tree;  
    alignment_tree = NULL;	
  }
}

//////////////////////////////////////////////////
// class copy constructor of LC_MS_alignment_tree_parser
LC_MS_alignment_tree_parser::LC_MS_alignment_tree_parser(const LC_MS_alignment_tree_parser& tmp){
}


//////////////////////////////////////////////////
// copy constructor:
LC_MS_alignment_tree_parser& LC_MS_alignment_tree_parser::operator=(const LC_MS_alignment_tree_parser& tmp){
  return *this;
}



//////////////////////////////////////////////////////
//***************************************************
//
// starts the merging process using the guide tree:
void LC_MS_alignment_tree_parser::start_merging_process(){
  
	
  // get the highest id of teh alignemnt tree
  int merge_ID = find_max_id(alignment_tree);
  
  // create a list of basic IDs:
  // find the max ID:
  vector<int> IDs;
  for(int i = 0; i <= merge_ID; i++){
    IDs.push_back(i);
  }
  
  while(alignment_tree->get_below_left() != NULL){
    merge_ID++;
    // merge the closest pairs:
    merge_max_related_pair(merge_ID);
  }
  
}

//////////////////////////////////////////////////////
// merges the 2 closest relatest pairs in current tree
void LC_MS_alignment_tree_parser::merge_max_related_pair(int new_merge_ID){
  
  // will be the id's of an alignment pair
  ID_A = 0;
  ID_B = 0;
  
  // get an alignment pair: travers the tree down, always left:
  get_alignment_pair(alignment_tree);
  
  // show statues
  printf("\t-- Alignment/merging of LC/MS pair [%d,%d] into [%d]\n", ID_A, ID_B, new_merge_ID);
  
  
  // now remove the nodes with this id's
  // and set the father node with the new id:
  update_alignment_tree(alignment_tree, new_merge_ID);
  
}



//////////////////////////////////////////////////////
// recursively traverses the tree, always go left until
// lowest level is reached
int LC_MS_alignment_tree_parser::find_max_id( node* IN ){
	
  int max = -1;
  int tmp = 0;
  
  // check left:
  if(IN->get_below_left() != NULL)
    tmp = find_max_id(IN->get_below_left());
  else
    tmp = IN->get_id();
  
  if(max < tmp)
    max = tmp;
  
  // check right
  if(IN->get_below_right() != NULL)
    tmp = find_max_id(IN->get_below_right());
  else
    tmp = IN->get_id();
  
  if(max < tmp)
    max = tmp;
  
  return max;
}

  

//////////////////////////////////////////////////////
// get an alignment pair:
// go recursevly down the tree to the left until find pair:
pair<int, int> LC_MS_alignment_tree_parser::get_alignment_pair( ){
  
  get_alignment_pair( alignment_tree );
  
  pair<int, int> TREE_PAIR;
  TREE_PAIR.first = ID_A;
  TREE_PAIR.second = ID_B;
  
  return TREE_PAIR;
}

//////////////////////////////////////////////////////
// get an alignment pair:
// go recursevly down the tree to the left until find pair:
void LC_MS_alignment_tree_parser::get_alignment_pair( node* current ){
		
  // go first all the way left:
  while(current->get_below_left() != NULL){
    current = current->get_below_left();
  }
  
  ID_A = current->get_id();
  
  // go up to father of this terminal:
  current = current->get_upper();
  // get now the right branch:
  current = current->get_below_right();
  
  // check if its a terminal:
  if(current->get_below_left() == NULL){
    ID_B = current->get_id();
  }
  else{
    get_alignment_pair(current);
  }	
}

//////////////////////////////////////////////////////
// now remove the nodes with this id's
// and set the father node with the new id:
void LC_MS_alignment_tree_parser::update_alignment_tree( ){
  int NEW_NODE_ID = find_max_id( alignment_tree );
  NEW_NODE_ID++;
  update_alignment_tree( alignment_tree, NEW_NODE_ID);
}

//////////////////////////////////////////////////////
// now remove the nodes with this id's
// and set the father node with the new id:
void LC_MS_alignment_tree_parser::update_alignment_tree( int merge_ID ){
  int NEW_NODE_ID = find_max_id( alignment_tree );
  update_alignment_tree( alignment_tree, NEW_NODE_ID);
}

//////////////////////////////////////////////////////
// now remove the nodes with this id's
// and set the father node with the new id:
void LC_MS_alignment_tree_parser::update_alignment_tree(node* current, int merge_ID){
		
  // get A:
  while(current->get_below_left() != NULL){
    current = current->get_below_left();
  }
  
  if(current->get_id() == ID_A){
    current = current->get_upper();
    
    // set the new ID:
    current->set_id(merge_ID);
    
    // now delete the left / right branches:
    current->delete_left();
    current->delete_right();
  }
  else{
    current = current->get_upper();
    update_alignment_tree(current->get_below_right(), merge_ID);
  }
}



///////////////////////////////////////////////////////
// read the alignment tree:
void LC_MS_alignment_tree_parser::read_alignment_tree(){
  
  if( alignment_tree != NULL ){
    delete alignment_tree;     
    alignment_tree = NULL;	
  }
    
  // test to read the tree:
  tree_reader* IN_R = new tree_reader();
  
  // reade the tree from file:
  IN_R->read_tree_file();
  // IN_R->print_tree();
  
  // get the tree structure from tree_reader:
  alignment_tree = IN_R->get_guide_tree(); 
  if( alignment_tree == NULL ){
    printf("\n\t**** PROBLEM reading the aignment tree!!, LC_MS_alignment_tree_parser::220***\n");
  }
  
  delete IN_R;
  IN_R = NULL;
  
}


///////////////////////////////////////////////////////
// checks if end of the tree is reached:
bool LC_MS_alignment_tree_parser::check_END_OF_TREE( ){
  
  if( alignment_tree == NULL ){
    return true;
  }
  
  if( alignment_tree->get_below_left() == NULL ){
    return true;
  }
  else{
    return false;
  }
}


///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS tree_builder:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_TREE_BUILD_MATRIX
#define USE_LC_MS_SIMILARITY_MATRIX
#define USE_TREE_BUILDER

#include "CLUSTER_LC_MS_ANALYSIS_USE.h"




////////////////////////////////////////////////
// constructor for the object tree_builder:
tree_builder::tree_builder(){
  
  SIM_MATRIX = NULL;
  M = NULL;
  max_node_A = end_nodes.end();
  max_node_B = end_nodes.end();
  top = NULL;
  
  //////////////////////////////////////////////////
  // set true to see matrix progress of tree building
  tree_build_visual = false;
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
    
  //////////////
  // plotting:
  TREE_VIEW = false;
  ///////////////////////////////////
  
  // read input directory:
  tree_outfile = def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;
   
  // remove backslash at end:
  if(tree_outfile[tree_outfile.size()-1] != '/'){
    tree_outfile += '/';
  }
  
  string tmp = "ANALYSIS_";
  tmp += tree_outfile;

  // add extension for tree format
  tree_outfile = tmp + "my_guide_tree.tre";
}

//////////////////////////////////////////////////
// class desctructor of tree_builder
tree_builder::~tree_builder(){

  SIM_MATRIX = NULL;
  node_pool.clear();
  end_nodes.clear();
  max_node_A = end_nodes.end();
  max_node_B = end_nodes.end();

  if(M != NULL){
    delete M;
    M = NULL;
  }
  
}


//////////////////////////////////////////////////
// copy constructor:
tree_builder& tree_builder::operator=(const tree_builder& tmp){
  return *this;
}

//////////////////////////////////////////////////////////////
// functions which takes the alignment pairs and makes nodes
// out of them
void tree_builder::input_alignment_pairs(result_manager* RES){
  
  // get the alignment pairs and their overlap
  vector<alignment_pair> IN = RES->get_alignment_pairs();
  
  // iterate through all alignment pairs and transfrom each into terminal nodes:
  vector<alignment_pair>::iterator p = IN.begin();
  
  // create and initialize the similarity matrix:
  M = new tree_build_matrix(IN);
  
}


///////////////////////////////////////////////////////////////////
// set the terminal nodes form the sim. matrix:
void tree_builder::set_LC_MS_similarity_matrix(LC_MS_similarity_matrix* IN){

  SIM_MATRIX = IN;

  // input the terminal nodes:
  map<int, SM_cell>::iterator X = ((*SIM_MATRIX->get_X_ELEMENT_START()).second).begin();
  
  while( X != ((*SIM_MATRIX->get_X_ELEMENT_START()).second).end() ){
    
    SM_cell* ELE = & ( (*X).second ) ;

    // insert the node into the leaf list:    
    insert_terminal_node(ELE->get_X_name());
    insert_terminal_node(ELE->get_Y_name());
    
    X++;
  }
  
  // create and initialize the similarity matrix:
  M = new tree_build_matrix();
  
  // build up the matrix object:
  map<string, node*>::iterator p = end_nodes.begin();
  while(p != end_nodes.end()){
    M->insert_into_name_array((*p).second->get_name());
    p++;
  }
  
  M->build_up_matrix(IN);
  
  // print terminal nodes:
  // print_terminal_nodes();  
}


///////////////////////////////////////////////////////////////////
// start build up:
void tree_builder::start_building(){

  // copy first the pool of end nodes into 
  // the total pool
  node_pool.clear();
  node_pool = end_nodes;

  // set the top to NULL
  top = NULL;

  // nb of new cluster created at each merging step
  int cluster_nb = 0;
  // nb of elements in matrix, stopp if smaller than 2
  int matrix_element_size = M->get_nb_elements();
  
  while(matrix_element_size > 1){
    
    if(tree_build_visual){
      // print the matrix content:
      M->print_tree_build_matrix();
    }

    // find max
    int X,Y,X_id,Y_id;
    M->find_min( &X, &Y);
    X_id = M->get_element_id(X);
    Y_id = M->get_element_id(Y);       

    // merge the max elements
    cluster_nb = M->merge_elements(X, Y);
     
    // insert the elemnet into the guide tree
    create_node(X_id,Y_id,cluster_nb);

    // get the current nb of elements
    matrix_element_size = M->get_nb_elements();
    
  }
  
  if(TREE_VIEW){
    // save the guide tree structure to a file:
    save_guide_tree();
  }
  
}

////////////////////////////////////////////////////////////////////
// searches a terminal nodes with name
map<string, node*>::iterator tree_builder::search_terminal_node_by_name(string name){
  return end_nodes.find(name);
}


////////////////////////////////////////////////////////////////////
// searches the pool of nodes
node* tree_builder::search_node_by_id(int id){

  map<string, node*>::iterator p = node_pool.begin();
  
  while(p != node_pool.end()){
    
    if((*p).second->get_id() == id){
      return (*p).second;
    }
    p++;
  }
  return NULL;;
}


////////////////////////////////////////////////////////////////////
// print out all terminal nodes 
 void tree_builder::print_terminal_nodes(){
   
  map<string, node*>::iterator p = end_nodes.begin();
  printf("\n\n\nList of terminal nodes to build the guide tree:\n");
  
  while(p != end_nodes.end()){
    (*p).second->show_info(); 
    p++;
  }
  cout<<endl;
}


////////////////////////////////////////////////////////////////////
// print out all nodes in the pool 
 void tree_builder::print_pool_nodes(){
   
  map<string, node*>::iterator p = node_pool.begin();
  printf("\n\n\nList of ALL nodes in current guide tree:\n");
  
  while(p != node_pool.end()){
    (*p).second->show_info(); 
    p++;
  }
  cout<<endl;
}


/////////////////////////////////////////////////////////////////////
// here start functions to create a guided tree:


////////////////////////////////////////////////////////////////////
// fuses 2 leafs into a node:
void tree_builder::create_node( int A,  int B, int cluster_nb){
 
  int level = 0;
 
  node* n_A = search_node_by_id(A);
  node* n_B = search_node_by_id(B);

  if(n_A->get_level() >n_B->get_level())
    level = n_A->get_level();
  else
    level = n_B->get_level();
  
  level++;  

  //cout<<A<<"-"<<B<<" level="<<level<<" id="<<cluster_nb<<endl;
  
  node* tmp = new node(n_A , n_B, level, cluster_nb);
  tmp->set_edge_A(transform_distance_to_edge(M->get_cluster_edge_A(cluster_nb)));
  tmp->set_edge_B(transform_distance_to_edge(M->get_cluster_edge_B(cluster_nb)));
  node_pool.insert(pair<string, node*>(tmp->get_name(), tmp));


  // save the top level:
  if(top == NULL){
    top = tmp;
  }
  else{
    if(top->get_level() < level)
      top = tmp;
  }

  n_A = n_B = NULL;
  tmp = NULL;
}

///////////////////////////////////////////////////////////////////
// takes as input the distance score from the cluster matrix
// then transforms this one into a tree distance:
double tree_builder::transform_distance_to_edge(double IN){
  return 1 - IN;
}


//////////////////////////////////////////////////////////////////
// print the constructed guide tree to teh screen
void tree_builder::print_guide_tree(){

  printf("\n\nGuide Tree constructed from multiple LC-MS alignment...\n\n");

  node* start = top;
  start->print_tree();
  cout<<endl;
  start->print_tre_format();

}

///////////////////////////////////////////////////////////////////
// save the constructed guide tree to file
void tree_builder::save_guide_tree(){
  
  printf("\n\t-- Guide Tree structure was saved to file '%s'.\n",tree_outfile.c_str());
  
  // open a file, etc:
  ofstream* fout = new ofstream();
  // upper:
  fout->open(tree_outfile.c_str(), ofstream::out);
  // write nodes to file:
  top->write_node(fout);
  
  fout->close();
  delete fout;
  fout = NULL;
}

///////////////////////////////////////////////////////////////////
// save the constructed guide tree to file
void tree_builder::save_guide_tree( LC_MS* IN ){
  
  printf("\n\t-- Guide Tree structure was saved to file '%s'.\n",tree_outfile.c_str());
  
  // open a file, etc:
  ofstream* fout = new ofstream();
  // upper:
  fout->open(tree_outfile.c_str(), ofstream::out);
  // write nodes to file:
  *fout<<"("<<IN->get_spec_name()<<"["<<IN->get_spectrum_ID()<<"]);";

  fout->close();
  delete fout;
  fout = NULL;
}


///////////////////////////////////////////////////////////////////
// save the constructed guide tree to file
void tree_builder::save_guide_tree(string PREFIX){
  
  int pos = tree_outfile.find(".tre");
  PREFIX = "_" + PREFIX;
  tree_outfile.insert(pos, PREFIX);
  printf("\n\t-- '%s' Tree structure was saved to file '%s'.\n",PREFIX.c_str(), tree_outfile.c_str());

  // open a file, etc:
  ofstream* fout = new ofstream();
  // upper:
  fout->open(tree_outfile.c_str(), ofstream::out);
  // write nodes to file:
  top->write_node(fout);
  
  fout->close();
  delete fout;
  fout = NULL;
}


///////////////////////////////////////////////////////////////////
// insert terminal node:
void tree_builder::insert_terminal_node(string name){
  
  if( end_nodes.find(name) == end_nodes.end()){ 
    
    int ID = end_nodes.size();
    
    // create a terminal node from this alignment pair:
    node* tmp = new node(ID, name);
    // insert the node into the leaf list:
    end_nodes.insert(pair<string, node*>(name,tmp));
    tmp = NULL;    
  }
}
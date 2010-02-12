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


#ifndef TREE_BUILDER_H
#define TREE_BUILDER_H



class tree_builder{

    
  ////////////////////////////////////////////////
  // declaration of the private members:

private:

  ////////////////////////////////////////////////
  // declaration of the public members:
  
  // vector to hold the terminal nodes:
  map<string, node*> end_nodes;
 
  // vector to hold all nodes (non & terminals):
  map<string, node*> node_pool;
 
  map<string, node*>::iterator max_node_A, max_node_B;
  
  // similarity matrix:
  tree_build_matrix* M;

  LC_MS_similarity_matrix* SIM_MATRIX;
  
  // top of tree
  node* top;

  bool tree_build_visual;
  bool TREE_VIEW;
  
  // outfile to save the tree:
  string tree_outfile;
	
public:
  
  // class destructor
  ~tree_builder();
  
  // class constructor
  tree_builder();
  
  //////////////////////////////////////////////////
  // overload operators:
  tree_builder& operator=(const tree_builder&);
  tree_builder& operator<=(const tree_builder&);
  tree_builder& operator>=(const tree_builder&);
  tree_builder& operator<(const tree_builder&);
  tree_builder& operator>(const tree_builder&);
  
  
  /////////////////////////////////////////////////////////////////
  //           Distance Matrix functions:
  // these functions are all to assess the distance matrix:
  
  // get the pair of LC-MS runs with maximal overlap score ( = closest relatest)
  float get_matrix_max();
  // takes as input the distance score from the cluster matrix
  // then transforms this one into a tree distance:
  double transform_distance_to_edge(double);
  
  // print out all terminal nodes 
  void print_terminal_nodes();
  // print out all nodes in the pool 
  void print_pool_nodes();
  // save the constructed guide tree to file
  void save_guide_tree();
  void save_guide_tree( LC_MS* );
  void save_guide_tree(string);
  // print out the similarity matrix 
  void print_matrix(){M->print_tree_build_matrix();};
  // print out the similarity matrix 
  void print_matrix_to_file(){M->print_tree_build_matrix_to_file();};
  // print out the similarity matrix 
  void print_matrix_image_to_file(){M->print_tree_build_matrix_image_to_file();};
  
  
  /////////////////////////////////////////////////////////////////////
  // here start functions to create a guided tree:
  
  // fuses 2 leafs into a node:
  void create_node( int, int, int);
  
  // searches a terminal leaf with name
  map<string, node*>::iterator search_terminal_node_by_name(string);

  // searches the pool of nodes
  node* search_node_by_id(int);  

  // start build up:
  void start_building();

  // print the constructed guide tree to teh screen
  void print_guide_tree();


  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // functions which takes the alignment pairs and makes nodes
  // out of them
  void input_alignment_pairs(result_manager*);
  
	
  // get the alignment tree:
  node* get_guide_tree(){node* tmp = top; top = NULL;return tmp;};
  
  // input the LC_MS_similarity_matrix:
  void set_LC_MS_similarity_matrix(LC_MS_similarity_matrix*);

  // insert terminal node:
  void insert_terminal_node(string);

};
#endif

    

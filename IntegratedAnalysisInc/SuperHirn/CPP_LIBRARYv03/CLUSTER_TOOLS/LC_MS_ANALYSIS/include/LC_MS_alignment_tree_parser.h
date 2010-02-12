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


#ifndef LC_MS_ALIGNMENT_TREE_PARSER_H
#define LC_MS_ALIGNMENT_TREE_PARSER_H



class LC_MS_alignment_tree_parser{

  ////////////////////////////////////////////////
  // declaration of the private members:
  node* alignment_tree;
  
  // pair id's for the LC-MS pair
  int ID_A;
  int ID_B;
  
private:
    
    ////////////////////////////////////////////////
  // declaration of the public members:
    
public:
  
    // class destructor
    ~LC_MS_alignment_tree_parser();
  
  // class constructor
  LC_MS_alignment_tree_parser();
  LC_MS_alignment_tree_parser(node*);
  // class copy constructor
  LC_MS_alignment_tree_parser(const LC_MS_alignment_tree_parser&);
  
  // overload operators:
  LC_MS_alignment_tree_parser& operator=(const LC_MS_alignment_tree_parser&);
  LC_MS_alignment_tree_parser& operator<=(const LC_MS_alignment_tree_parser&);
  LC_MS_alignment_tree_parser& operator>=(const LC_MS_alignment_tree_parser&);
  LC_MS_alignment_tree_parser& operator<(const LC_MS_alignment_tree_parser&);
  LC_MS_alignment_tree_parser& operator>(const LC_MS_alignment_tree_parser&);
  
  ///////////////////////////////////////////////////		
  // starts the merging process using the guide tree:
  void start_merging_process();
  // merges the 2 closest relatest pairs in current tree
  void merge_max_related_pair(int);
  // get an alignment pair:
  // go recursevly down the tree to the left until find pair:
  pair<int, int> get_alignment_pair();
  void get_alignment_pair(node*);
  // now remove the nodes with this id's
  // and set the father node with the new id:
  void update_alignment_tree(node*, int);
  void update_alignment_tree( int );
  void update_alignment_tree( );
    
  // delete here the real LC-MS runs with
  // the correspoding ID:
  void delete_LC_MS_by_ID(int);
  
  // read the alignment tree:
  void read_alignment_tree();
  
  // checks if end of the tree is reached:
  bool check_END_OF_TREE( );


  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // starts the merging process using the guide tree:
  int find_max_id(node*);
  
};

#endif

    

///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS tree_reader:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef TREE_READER_H
#define TREE_READER_H



class tree_reader{

    
	////////////////////////////////////////////////
	// declaration of the private members:
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  string tree_file;
  ifstream* FIN;
  node* root;
  
public:
    
    // class destructor
    ~tree_reader();

  // class constructor
  tree_reader();
  tree_reader(char*);
  tree_reader(string);
  // class copy constructor
  tree_reader(const tree_reader&);
  tree_reader(const tree_reader*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  tree_reader& operator=(const tree_reader&);
  tree_reader& operator<=(const tree_reader&);
  tree_reader& operator>=(const tree_reader&);
  tree_reader& operator<(const tree_reader&);
  tree_reader& operator>(const tree_reader&);
  
		
		
		//////////////////////////////////////////////////
		// tree reading operations:		
		// create a new internal node:
		node* create_new_internal_node(node*);
		// create a new terminal node:
		void create_new_terminal_node(node*, string, string);
  
  
  
		//////////////////////////////////////////////////
		// file accession operations:
		// open / close a file:
		bool open_file();
		void close_file();
		// check end of file
		bool end_of_file();
		// reads a character in teh file
		char get_character();
		// reads the tree-file
		void read_tree_file();
		
		// printe the tree to screen:
		void print_tree();
		// save the constructed guide tree to file
		void save_guide_tree();
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
		// get the tree structure saved in the nodes:
		node* get_guide_tree(){node* tmp = root; root = NULL; return tmp;};
		// set the tree structure saved in the nodes:
		void set_guide_tree(node* in){root = in;};
		
};

#endif

    

///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS node:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef NODE_H
#define NODE_H



class node{

    
    ////////////////////////////////////////////////
    // declaration of the private members:

private:

    ////////////////////////////////////////////////
    // declaration of the public members:


  int level;
  bool terminal;
  int id;

  node* below_A;
  double edge_A;
  node* below_B;
  double edge_B;
  node* upper;
  
  // names
  string name;
  int nb_peaks;

public:

  // class destructor
  ~node();
  
  // class constructor
  node();
  node(string);
  node(const node&);
  node(const node*);
  node(int, string);
  node(node*, node*, int,int);
  node(alignment_pair*, int);
  

  
  //////////////////////////////////////////////////
  // overload operators:
  node& operator=(const node&);
  node& operator<=(const node&);
  node& operator>=(const node&);
  node& operator<(const node&);
  node& operator>(const node&);

  // print the tree recirsuvely
  void print_tree();
	// print the tree in "tre" -format 
	void write_node(ofstream*);

  
  // show info
  void show_info();

  // print the tree in "tre" -format
  void print_tre_format();

  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  
  // access the level parameter of a node
  void set_level(int i){level = i;};
  int get_level(){return level;};
  
  // true if terminal, else false
  bool get_terminal(){return terminal;};


  // get the connnectivity:
  node* get_below_A(){return below_A;};
  node* get_below_B(){return below_B;};
	
	 node* get_below_left(){return below_A;};
	 void set_below_left(node* IN){below_A=IN;};
	 void allocate_below_left(){below_A = new node(); below_A->set_upper(this);};
	 void allocate_below_left(string IN){below_A = new node(IN); below_A->set_upper(this);};
	 node* get_below_right(){return below_B;};
	 void set_below_right(node* IN){below_B=IN;};
	 void allocate_below_right(){below_B = new node(); below_B->set_upper(this);};
	 void allocate_below_right(string IN){below_B = new node(IN); below_B->set_upper(this);};
	 node* get_upper(){return upper;};
	 void set_upper(node* IN){upper = IN;};
	 
	 void delete_left(){
		 if(below_A != NULL){
			 delete below_A;
			 below_A = NULL;
		 }
	 }
	 void delete_right(){
		 if(below_B != NULL){
			 delete below_B;
			 below_B = NULL;
		 }
	 }
	 
  // get the names of the elements
  string get_name(){return name;};

  // get nb_peaks
  int get_nb_peaks(){return nb_peaks;};

  int get_id(){return id;};
  void set_id(int IN){id=IN;};

  // access the edges A / B
  double get_edge_A(){return edge_A;};
  void set_edge_A(double IN){edge_A=IN;};
  void set_edge_left(double IN){edge_A=IN;};
  double get_edge_B(){return edge_B;};
  void set_edge_B(double IN){edge_B=IN;};
  void set_edge_right(double IN){edge_B=IN;};
   
};

#endif

    

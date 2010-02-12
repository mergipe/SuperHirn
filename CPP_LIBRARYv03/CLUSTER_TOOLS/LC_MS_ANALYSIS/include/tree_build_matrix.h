///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS tree_build_matrix:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef TREE_BUILD_MATRIX_H
#define TREE_BUILD_MATRIX_H



class tree_build_matrix{

    
  ////////////////////////////////////////////////
  // declaration of the private members:

  map< int, map<int,cell> >  M;
  map<int, matrix_element>  name_array;
  map<int,double> average_distances;

  bool NJ;
  
  int cluster_nb;

private:

    ////////////////////////////////////////////////
    // declaration of the public members:

public:

    // class destructor
    ~tree_build_matrix();

    // class constructor
  tree_build_matrix();
  tree_build_matrix(vector<alignment_pair>);

  //////////////////////////////////////////////////
  // overload operators:
  tree_build_matrix& operator=(const tree_build_matrix&);
  tree_build_matrix& operator<=(const tree_build_matrix&);
  tree_build_matrix& operator>=(const tree_build_matrix&);
  tree_build_matrix& operator<(const tree_build_matrix&);
  tree_build_matrix& operator>(const tree_build_matrix&);
  
  // print content of tree_build_matrix:
  void print_tree_build_matrix();
  
  // print content of tree_build_matrix to a file:
  void print_tree_build_matrix_to_file();
  // make a picture of the tree_build_matrix:
  void print_tree_build_matrix_image_to_file();
  
  // function which defines an similarity score base on
  // - nb features in A and B
  // - overlapping features(Q)
  double set_correlation_score(double);
  
  // get dimension of the tree_build_matrix:
  int get_M_size();
  
  
  // merge elements
  int merge_elements(int, int);
  // merges name of the tree_build_matrix element in row/ column i:
  void merge_element_name(int , int);
  
  // get element score by name:
  
  // find element with max score in tree_build_matrix
  void find_min(int* , int* );
  
  // functions to access the tree_build_matrix elements:
  void erase_element(int);
  
  // adjuts now the elements according
  // to neighbourhood joining method
  // -> normalize each distance by
  // average distance!
  // Dij = dij - ( ri + rj)
  // ri = 1 / (L-2) * sum(dik)
  void tree_build_matrix_normalization();
  
  
  // insert element into the name array:
  void insert_into_name_array(string);
  // constructs a matrix from LC_MS_similarity_matrix
  void build_up_matrix(LC_MS_similarity_matrix*);

  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // functions to access the tree_build_matrix elements:
  cell* get_element(int, int);
  // functions to insert the tree_build_matrix elements:
  void set_element(cell*, int, int);
  
  // get name of the tree_build_matrix elemnet in row/ column i:
  string get_element_name(int);
  
  float get_score_by_name(string, string);
  // get element score by name:
  void set_score_by_name(string, string, float);
  
  // get index of an element in the tree_build_matrix by name:
  int get_index_by_name(string);
  
  // get name of the tree_build_matrix elemnet in row/ column i:
  int get_element_name_index(string);
  
  // get the number of elements in the tree_build_matrix:
  int get_nb_elements();
  
  // get current cluster nb:
  int get_current_cluster_nb(){return cluster_nb;};
  // increase current cluster nb:
  void increase_current_cluster_nb(){cluster_nb++;};
  
  // get name of the tree_build_matrix elemnet in row/ column i:
  int get_element_id(int);
  
  // access the cluster edges by id:
  double get_cluster_edge_A(int);
  double get_cluster_edge_B(int);
};

#endif

    

///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS multi_merger:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef MULTI_MERGER_H
#define MULTI_MERGER_H



class multi_merger{

  ////////////////////////////////////////////////
  // declaration of the private members:
  node* alignment_tree;

  // instance of spec_manager and the function get_LC_MS_by_ID():
  void *manager;
  LC_MS* (*Pget_LC_MS_ID)(void *pInstance, int ID); 
  void (*Pdelete_LC_MS_ID)(void *pInstance, int ID); 
  
  // created merged LC-MSs
  vector<LC_MS> merged_LC_MS;
  
  // pair id's for the LC-MS pair
  int ID_A;
  int ID_B;
  
  // show alignment plots during master map alignment:
  bool multiAView;
  
  
 private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
 public:
  
  // class destructor
  ~multi_merger();
  
  // class constructor
  multi_merger();

  multi_merger(void *pinstance, LC_MS*(*pfct_get_LC_MS_ID)(void *pInstance,int), void(*pfct_delete_LC_MS_ID)(void *pInstance,int) );
  
  // class copy constructor
  multi_merger(const multi_merger&);
  
 
  
  //////////////////////////////////////////////////
  // overload operators:
  multi_merger& operator=(const multi_merger&);
  multi_merger& operator<=(const multi_merger&);
  multi_merger& operator>=(const multi_merger&);
  multi_merger& operator<(const multi_merger&);
  multi_merger& operator>(const multi_merger&);
  
  ///////////////////////////////////////////////////		
  // starts the merging process using the guide tree:
  void start_merging_process();
  // merges the 2 closest relatest pairs in current tree
  void merge_max_related_pair(int);
  // go down the tree to the left until find pair:
  void get_alignment_pair(node *);
  // now remove the nodes with this id's
  // and set the father node with the new id:
  void update_alignment_tree(node*, int);
  
  
  ///////////////////////////////////////////////////////////////////////
  // in case no alignment tree is available
  // start creation of a fake alignment tree:
  void createFakeAlignmentTree();    

  
  ///////////////////////////////////////////////////
  // functions to align, search and merge:
  //
  // aligns 2 LC-MS runs and returns corrected LC_MS B
  void align_B_to_A(int, int);
  //
  // merge 2 LC-MS runs and returns the merged one
  LC_MS* merge_2_LC_MS(int, int);
  
  // delete here the real LC-MS runs with
  // the correspoding ID:
  void delete_LC_MS_by_ID(int);
  
  // reads the structure of an alignment tree into the memory:	
  void read_guide_tree();

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // get a spectrum from spec_manager or multi_merger by ID:
  LC_MS* get_LC_MS_by_ID(int);
  
  // insert a created merged spectrum:
  void insert_merged_LC_MS(LC_MS*);
  
  // starts the merging process using the guide tree:
  int find_max_id(node*);
  
  // get the last merged LC_MS == MASTER run
  LC_MS* get_master_run(){return &(*(merged_LC_MS.begin()));};

  // access the MASTER run:
  LC_MS* get_MASTER_LC_MS(){return get_master_run();};
};

#endif

    

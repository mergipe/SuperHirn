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


#ifndef SEQUENTIAL_TREE_CREATOR_H
#define SEQUENTIAL_TREE_CREATOR_H



class SequentialTreeCreator{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  node* mainTree;
  map<int, node*> node_pool;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:

    static map<int, LC_MS*> LC_MS_map;

  // class destructor
  ~SequentialTreeCreator();
  
  // class constructor
  SequentialTreeCreator();
  // class copy constructor
  SequentialTreeCreator(const SequentialTreeCreator&);
  // class copy constructor
  SequentialTreeCreator(const SequentialTreeCreator*);
  
  
  
  
  //////////////////////////////////////////////////
  // start the construction of the alignment tree
  void constructSequentialAlignmentTree();
  // searches the pool of nodes
  node* search_node_by_id(int);
  // fuses 2 leafs into a node:
  void create_node( int,int,int);

  // orders teh runs in the alignment order
  void buildAlignOrder( vector<int>*);

  
  //////////////////////////////////////////////////
  // overload operators:
  SequentialTreeCreator& operator=(const SequentialTreeCreator&);
  bool operator==(const SequentialTreeCreator&);
  SequentialTreeCreator& operator<=(const SequentialTreeCreator&);
  SequentialTreeCreator& operator>=(const SequentialTreeCreator&);
  SequentialTreeCreator& operator<(const SequentialTreeCreator&);
  SequentialTreeCreator& operator>(const SequentialTreeCreator&);
  
  
  
  //////////////////////////////////////////////////
  // this structure provides the function to compare
  // LC-MS by number of features:
  struct OPERATOR_NB_PEAKS{
    // provide the compare function for sort:
    bool operator()(const int A,const int B) const{

      map<int, LC_MS*>::iterator F = LC_MS_map.find( A ); 
      LC_MS* run =  F->second;
      int Anb =  run->get_nb_features();
      
      F = LC_MS_map.find( B ); 
      run =  F->second;
      int Bnb =  run->get_nb_features();
      
      
      // check which one has more features:
      if(Anb == Bnb){
        return false;
      }
      else{
        return Anb > Bnb;
      }
    }
  };
  
  
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // set teh map:
  void setLCMSMap(map<int, LC_MS*> in){LC_MS_map = in;};
  
  // get the constructed tree:
  node* getSequentialTree(){return mainTree;};
};

#endif

    

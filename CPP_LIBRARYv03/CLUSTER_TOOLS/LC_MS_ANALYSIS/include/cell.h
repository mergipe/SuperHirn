///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS cell:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef CELL_H
#define CELL_H



class cell{

    
  ////////////////////////////////////////////////
  // declaration of the private members:

private:

  ////////////////////////////////////////////////
  // declaration of the public members:
 
  string X;
  string Y;
  double score;  
  
 public:
  
  // class destructor
  ~cell();
  
  // class constructor
  cell();
  cell(const cell&);
  cell(const cell*);
  cell(string, string, double);
  
  //////////////////////////////////////////////////
  // overload operators:
  cell& operator=(const cell&);
  cell& operator<=(const cell&);
  cell& operator>=(const cell&);
  cell& operator<(const cell&);
  cell& operator>(const cell&);
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // get name
  string get_X(){return X;};
  string get_Y(){return Y;};
  double get_score(){return score;};
  void set_score(float IN){score = IN;};

};

#endif

    

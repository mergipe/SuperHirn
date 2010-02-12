///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS matrix_element:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef MATRIX_ELEMENT_H
#define MATRIX_ELEMENT_H



class matrix_element{

    
  ////////////////////////////////////////////////
  // declaration of the private members:

private:

  ////////////////////////////////////////////////
  // declaration of the public members:
 
  string name;
  int id;

  double edge_A;
  double edge_B;

 public:
  
  // class destructor
  ~matrix_element();
  
  // class constructor
  matrix_element();
  matrix_element(const matrix_element&);
  matrix_element(string, int);
  
  //////////////////////////////////////////////////
  // overload operators:
  matrix_element& operator=(const matrix_element&);
  matrix_element& operator<=(const matrix_element&);
  matrix_element& operator>=(const matrix_element&);
  matrix_element& operator<(const matrix_element&);
  matrix_element& operator>(const matrix_element&);
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  
  // get name
  string get_name(){return name;};
  void set_name(string IN){name = IN;};
  int get_id(){return id;};
  void set_id(int IN){id = IN;};

  // access the edges A / B
  double get_edge_A(){return edge_A;};
  void set_edge_A(double IN){edge_A=IN;};
  double get_edge_B(){return edge_B;};
  void set_edge_B(double IN){edge_B=IN;};
  
};

#endif

    

///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS <EXAMPLE>:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef <EXAMPLE>_H
#define <EXAMPLE>_H



class <EXAMPLE>{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~<EXAMPLE>();
  
  // class constructor
  <EXAMPLE>();
  // class copy constructor
  <EXAMPLE>(const <EXAMPLE>&);
  // class copy constructor
  <EXAMPLE>(const <EXAMPLE>*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  <EXAMPLE>& operator=(const <EXAMPLE>&);
  bool operator==(const <EXAMPLE>&);
  <EXAMPLE>& operator<=(const <EXAMPLE>&);
  <EXAMPLE>& operator>=(const <EXAMPLE>&);
  <EXAMPLE>& operator<(const <EXAMPLE>&);
  <EXAMPLE>& operator>(const <EXAMPLE>&);
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

};

#endif

    

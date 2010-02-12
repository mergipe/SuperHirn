///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS paramManager:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef PARAM_MANAGER_H
#define PARAM_MANAGER_H



class paramManager{
  
    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:

  map<string, string> allParameters;
  string XMLfile;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~paramManager();
  
  // class constructor
  paramManager();
  // class copy constructor
  paramManager(const paramManager&);
  // class copy constructor
  paramManager(const paramManager*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  paramManager& operator=(const paramManager&);
  bool operator==(const paramManager&);
  paramManager& operator<=(const paramManager&);
  paramManager& operator>=(const paramManager&);
  paramManager& operator<(const paramManager&);
  paramManager& operator>(const paramManager&);
  
  
  //////////////////////////////////////////////////////
  // extract all parameters from the ROOT and defined param file:
  void extractAndStoreParameters();
  // extract all parameters from the ROOT file:
  void extractRootParameters();
  // extract all parameters from the costum file:
  void extractCostumParameters();
  // extract all parameters from a file:
  void extractParametersFromFile( string );
  // store a parameter from a line: <tag>=<value>
  void storeParameterLine( string );
  // store all parameters in a file:
  void storeALLParameterinXMLFile( );

  ////////////////////////////////////////////////////////////
  // restore all parameters in a XML file to the paramRestore file:
  void restoreParamFile( string );
  // store all parameters in a param file:
  void storeALLParameterinPARAMFile( );

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

};

#endif

    

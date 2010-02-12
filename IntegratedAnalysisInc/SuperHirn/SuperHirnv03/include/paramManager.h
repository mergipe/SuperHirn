#ifndef PARAM_MANAGER_H
#define PARAM_MANAGER_H



/*
 * @author  written by Lukas N Mueller (Lukas.Mueller@imsb.biol.ethz.ch), Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
 * @brief Class to read and store parameters from the param.def and Root parameter file. These are then stored in a XML formatted file.
 */

class paramManager{
  
    
  
private:

  map<string, string> allParameters;
  string XMLfile;
    
public:
  
  // class destructor
  ~paramManager();  
  paramManager();
  paramManager(const paramManager&);
  paramManager(const paramManager*);
  
  paramManager& operator=(const paramManager&);
  bool operator==(const paramManager&);
  paramManager& operator<=(const paramManager&);
  paramManager& operator>=(const paramManager&);
  paramManager& operator<(const paramManager&);
  paramManager& operator>(const paramManager&);
  
  /*
   * @brief extract all parameters from the ROOT and defined param file:
   */
  void extractAndStoreParameters();
  /*
   * @brief extract all parameters from the ROOT file:
   */
  void extractRootParameters();
  /*
   * @brief extract all parameters from the costum file:
   */
  void extractCostumParameters();
  
  /*
   * @brief Replaces parameters extracted from parameter files with a global parameter that has been stored in the read_param class.
   */
  void replaceByGlobalParameters();
    
  /*
   * @brief extract all parameters from a file.
   * @param string path / name to the parameter file.
   */
  void extractParametersFromFile( string );
  
  /*
   * @brief store a parameter from a line: <tag>=<value>
   * @param string, parameter to store
   */
  void storeParameterLine( string );
 
  /*
   * @brief store all parameters in a file
   */
  void storeALLParameterinXMLFile( );

  /*
   * @brief restore all parameters in a XML file to the paramRestore file
   * @param string, path/name to the parameter file
   */
  void restoreParamFile( string );
  
  /*
   * @brief store all parameters in a param file:
   */
  void storeALLParameterinPARAMFile( );

  
  
};

#endif

    

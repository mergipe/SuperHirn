///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS ratiolizer_manager:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef RATIOLIZER_MANAGER_H
#define RATIOLIZER_MANAGER_H



class ratiolizer_manager{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  LC_MS* MY_MASTER;
  
  MS1_feature_ratiolizer* ms1RatioAnalyzer;
  protein_ratiolizer* proteinRatioAnalyzer; 
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~ratiolizer_manager();
  
  // class constructor
  ratiolizer_manager();
  // class copy constructor
  ratiolizer_manager(const ratiolizer_manager&);
  // class copy constructor
  ratiolizer_manager(const ratiolizer_manager*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  ratiolizer_manager& operator=(const ratiolizer_manager&);
  bool operator==(const ratiolizer_manager&);
  ratiolizer_manager& operator<=(const ratiolizer_manager&);
  ratiolizer_manager& operator>=(const ratiolizer_manager&);
  ratiolizer_manager& operator<(const ratiolizer_manager&);
  ratiolizer_manager& operator>(const ratiolizer_manager&);
  
  
  // start the ratiolizer analysis:
  void start_RATIOLIZER();
  
  
  
  // imports the MASTER LC/MS run:
  void import_MASTER_run();
  void insert_MASTER( LC_MS* );
  LC_MS* get_MASTER( ){ return MY_MASTER;};
  
  // make intensity normalization of the MASTER map
  void NORMALIZE_MASTER_MAP();

  /////////////////////////////////////////////////////////////////////
  // write the analysis to XML file:
  void write_analysis_2_XML();    

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

};

#endif

    

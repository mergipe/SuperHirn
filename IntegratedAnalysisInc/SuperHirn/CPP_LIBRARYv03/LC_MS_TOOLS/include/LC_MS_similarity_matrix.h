///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS LC_MS_similarity_matrix:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef LC_MS_SIMILARITY_MATRIX_H
#define LC_MS_SIMILARITY_MATRIX_H


typedef map<int, SM_cell> ROW;

class LC_MS_similarity_matrix{

  string MZXML_DIR;
  int ELEMENT_COUNT;
    
  // matrix elements:
  map< int,  ROW > ELEMENTS;

  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
    
  static string PerlPathName;
  
  
  // class destructor
  ~LC_MS_similarity_matrix();
  
  // class constructor
  LC_MS_similarity_matrix(string);
  // class copy constructor
  LC_MS_similarity_matrix(const LC_MS_similarity_matrix&);
  // class copy constructor
  LC_MS_similarity_matrix(const LC_MS_similarity_matrix*);
  
  
  // insert an element into the matrix:
  void insert_ELEMENT(SM_cell*);
  // write matrix to XML:
  void write_Matrix_to_XML();
  // plot the similarity matrix using gnuplot:
  void plot_matrix_by_gnuplot();
  // plot the similarity matrix using lukas' perl script:
  void plot_matrix_by_perl();
  // construct and check the perl script path/Name:
  string checkPerlPath();
    

  
  //////////////////////////////////////////////////
  // overload operators:
  LC_MS_similarity_matrix& operator=(const LC_MS_similarity_matrix&);
  bool operator==(const LC_MS_similarity_matrix&);
  LC_MS_similarity_matrix& operator<=(const LC_MS_similarity_matrix&);
  LC_MS_similarity_matrix& operator>=(const LC_MS_similarity_matrix&);
  LC_MS_similarity_matrix& operator<(const LC_MS_similarity_matrix&);
  LC_MS_similarity_matrix& operator>(const LC_MS_similarity_matrix&);
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // get start end rows:
  map<int, map<int, SM_cell> >::iterator get_X_ELEMENT_START(){return ELEMENTS.begin();};
  map<int, map<int, SM_cell> >::iterator get_X_ELEMENT_END(){return ELEMENTS.end();};
  
  // insert an element into the matrix:
  SM_cell* get_ELEMENT(int, int);

  // get total score:
  // insert an element into the matrix:
  double get_TOTAL_SCORE(int x, int y){ SM_cell* tmp = get_ELEMENT(x, y);if(tmp){return tmp->get_TOTAL_SCORE();}else{return 0.0;};};

  int get_ELEMENT_COUNT(){return ELEMENT_COUNT;};
};

#endif

    

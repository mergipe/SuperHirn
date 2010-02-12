///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS SM_SM_cell:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef SM_CELL_H
#define SM_CELL_H



class SM_cell{

    
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
  int X;
  int Y;
  int ID;
  string X_name;
  string Y_name;
  int X_number_features;
  int Y_number_features;
  double MATCH_SCORE;
  double INT_SCORE;
  double ALIGN_SCORE;
  double TOTAL_SCORE;
  double NORM_SCORE;
  
  
public:
    static string SCORE_MODUS;
    
    // class destructor
    ~SM_cell();
  
  // class constructor
  SM_cell();
  SM_cell(const SM_cell&);
  SM_cell(const SM_cell*);
  SM_cell(int, int);
  SM_cell(LC_MS*, LC_MS*);
  
  //////////////////////////////////////////////////
  // overload operators:
  SM_cell& operator=(const SM_cell&);
  SM_cell& operator<=(const SM_cell&);
  SM_cell& operator>=(const SM_cell&);
  SM_cell& operator<(const SM_cell&);
  SM_cell& operator>(const SM_cell&);
  
  // write to XML:
  void write_to_XML( ofstream*);
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // get name
  int get_X(){return X;};
  int get_Y(){return Y;};
  string get_X_name(){return X_name;};
  string get_Y_name(){return Y_name;};
  
  double get_TOTAL_SCORE(){return TOTAL_SCORE;};
  void set_TOTAL_SCORE(double IN){TOTAL_SCORE = IN;};
  double get_ALIGN_SCORE(){return ALIGN_SCORE;};
  void set_ALIGN_SCORE(double IN){ALIGN_SCORE = IN;};
  double get_INT_SCORE(){return INT_SCORE;};
  void set_INT_SCORE(double IN){INT_SCORE = IN;};
  double get_MATCH_SCORE(){return MATCH_SCORE;};
  void set_MATCH_SCORE(double IN){MATCH_SCORE = IN;};
  double get_NORM_SCORE(){return NORM_SCORE;};
  void set_NORM_SCORE(double IN){NORM_SCORE = IN;};
  // OUT score:
  double get_OUT_SCORE();
    
  void set_ID(int IN){ID = IN;};
  int get_ID(){return ID;};
  
  
  
};

#endif

    

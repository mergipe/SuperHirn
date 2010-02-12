///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS normalization_coeff_computer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef NORMALIZATION_COEFF_COMPUTER_H
#define NORMALIZATION_COEFF_COMPUTER_H



class normalization_coeff_computer{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
  int NB_MATCHES;
  
  LC_MS* MASTER;
  
  map<int, double> GLOBAL_LC_MS_COEFF;
  map<int, vector<double> > ALL_LOCAL_LC_MS_COEFF;
  map<int, double> ABSOLUTE_LC_MS_COEFF;
  map<int, double> GLOBAL_ABSOLUTE_LC_MS_COEFF;
  

private:

    ////////////////////////////////////////////////
    // declaration of the public members:

public:

    static vector<int> LC_MS_SELECTION_A;
  static vector<int> LC_MS_SELECTION_B;
  static double FEATURE_ALIGN_PERCENTAGE;
  static double TR_SLIDE_WINDOW;
  static double TR_STEP;


    // class destructor
    ~normalization_coeff_computer();

    // class constructor
    normalization_coeff_computer();
    normalization_coeff_computer(LC_MS*, int);
    normalization_coeff_computer(LC_MS*);
    // class copy constructor
    normalization_coeff_computer(const normalization_coeff_computer&);


    //////////////////////////////////////////////////
    // overload operators:
    normalization_coeff_computer& operator=(const normalization_coeff_computer&);
    normalization_coeff_computer& operator<=(const normalization_coeff_computer&);
    normalization_coeff_computer& operator>=(const normalization_coeff_computer&);
    normalization_coeff_computer& operator<(const normalization_coeff_computer&);
    normalization_coeff_computer& operator>(const normalization_coeff_computer&);
  
    
    // computes the coefficient at a given TR:
    map<int,double> compute_global_segment_coefficient(double );

    // copmutes the coefficient at a given TR:
    void compute_local_coefficient(double);
    
    // add the local variation to the global segment variation:
    void add_local_coefficent( map<int, double>*);
  
    // compute the global normalization coefficient:
    void compute_average_coefficients();
    void compute_median_coefficients();
    
    // compute the absolute coefficients:
    void compute_absolute_average_coefficients();
    
    
    
    
    ////////////////////////////////////////////////////////////////////////////////
    // compute a intensity coefficient based only on the overlap of a selection 
    // of LC/MS runs ( is based on the original alignmnet topology, which is not know in this class)
    void compute_LC_MS_selected_local_coefficient( double );
    // check if this features is detected in the current LC/MS selection
    bool check_FEATURE_LC_MS_selection( feature* );
    // get selected areas from this features according to the current LC/MS selection
    pair< vector<double>, vector<double> > get_selected_feature_areas( feature* );




    ///////////////////////////////
    // start here all the get / set
    // function to access the
    // variables of the class
    
    map<int, double> get_absolute_coefficients(){ return GLOBAL_ABSOLUTE_LC_MS_COEFF;};

};

#endif

    

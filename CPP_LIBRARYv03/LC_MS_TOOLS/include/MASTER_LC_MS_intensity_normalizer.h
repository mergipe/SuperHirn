///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS MASTER_LC_MS_intensity_normalizer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef MASTER_LC_MS_INTENSITY_NORMALIZER_H
#define MASTER_LC_MS_INTENSITY_NORMALIZER_H



class MASTER_LC_MS_intensity_normalizer{

    
    ////////////////////////////////////////////////
    // declaration of the private members:

private:

    ////////////////////////////////////////////////
    // declaration of the public members:
  
  double TR_SLIDE_WINDOW;
  double TR_STEP;
  double TR_MAX;
  double TR_MIN;
  
  
  bool PROFILE_VIEW;
  
  LC_MS* MASTER_RUN;
  
  // variation profiles, keys is the run ID:
  map<int, map<double,double> > RELATIVE_VARIATIONS; 
  map<int, map<double,double> > ABSOLUTE_VARIATIONS;

public:

    static bool ABSOLUTE_NORMALIZATION_MODE;
  static int NORMALIZATION_METHODE;

  // class destructor
  ~MASTER_LC_MS_intensity_normalizer();
  
  // class constructor
  MASTER_LC_MS_intensity_normalizer();
  // class copy constructor
  MASTER_LC_MS_intensity_normalizer(const MASTER_LC_MS_intensity_normalizer&);
  
  
  //**************************************************************
  // LC/MS RUN NORMALIZATION FUNCTIONS
  
  // NORMALIZE INTENSITY VARIATION ACROSS SAMPLES:
  // compute variation profiles along TR for every run:
  // and apply normalization to features for each input MASTER run:
  void start_GLOBAL_INTENSITY_NORMALIZATION();
  void start_GLOBAL_INTENSITY_NORMALIZATION(LC_MS*);
  // compute intensity correlation coefficint for every segment
  void compute_segment_INTENS_coefficient();
  // applies teh computed variations to the features:
  void normalize_feature_intensities();
  // adjust teh intensity of a feature:
  void adjust_intensity_of_feature(feature*);
  // print the variation profiles:
  void print_relative_variation_profiles();
  // print the absolute variation profiles:
  void print_absolute_variation_profiles();
  // print the variation profiles:
  void print_applied_normcoefficient();

  
  //**************************************************************
  // start intensity normalization by an input profile:
  void start_INPUT_PROFILE_INTENSITY_NORMALIZATION( );
  // extact the normalization values from a text file:
  void extractNormalizationProfiles( string );

  
  
  //**************************************************************
  // compute intensity correlation coefficient for every segment
  // according to the alignment tree
  void start_TREE_STRUCTURED_INTENSITY_NORMALIZATION( );
  // compute intensity correlation coefficient for every segment
  // but only for the current LC/MS runs according to the 
  // alignment tree
  void compute_segment_selected_LC_MS_INTENS_coefficient();
  
  
  

  //////////////////////////////////////////////////
  // overload operators:
  MASTER_LC_MS_intensity_normalizer& operator=(const MASTER_LC_MS_intensity_normalizer&);
  MASTER_LC_MS_intensity_normalizer& operator<=(const MASTER_LC_MS_intensity_normalizer&);
  MASTER_LC_MS_intensity_normalizer& operator>=(const MASTER_LC_MS_intensity_normalizer&);
  MASTER_LC_MS_intensity_normalizer& operator<(const MASTER_LC_MS_intensity_normalizer&);
  MASTER_LC_MS_intensity_normalizer& operator>(const MASTER_LC_MS_intensity_normalizer&);
  
    
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // insert MASTER run:
  void set_MASTER_RUN(LC_MS* IN){ MASTER_RUN = IN;};
  // get a master run:
  LC_MS* get_MASTER_RUN(){return MASTER_RUN;};
  
};

#endif

    

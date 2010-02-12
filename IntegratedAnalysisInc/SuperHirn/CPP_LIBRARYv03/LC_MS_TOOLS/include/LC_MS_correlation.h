///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS Spec COMPARE:
// compare of the input LC-MS spectra
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef LC_MS_CORRELATION_H
#define LC_MS_CORRELATION_H



class LC_MS_correlation{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
 private:
  
  vector<feature>::iterator MERGE_ITERATOR_A;
  SM_cell* SCORE;
  LC_MS* LCMS_A;
  int nb_feature_A;
  LC_MS* LCMS_B;
  int nb_feature_B;
  LC_MS_merger* MERGER;
  
  double corr;
  double UP_error;
  double DOWN_error;

  // vector which store all observed intensities:
  vector<double>intensity_rank_A;
  vector<double>intensity_rank_B;
  
  vector<double> rank_A;
  vector<double> rank_B;
  
  // list of common lc-peaks of the 2 spectra
  vector<feature*> peaks_A;
  vector<feature*> peaks_B;
  
  // their mean intensity:
  double mean_A;
  double mean_B;
  
  // score:
  double intensity_score;
  double total_score;
  double alignment_error_score;
  double normalize_score;
  double OVERLAP_SCORE;

  // store the alignment errors:
  vector<double> align_up_error;
  vector<double> align_down_error;
  
  // instance of aligner and the function get_LC_MS_by_ID():
  void *align_object;
  void (*P_get_alignment_error_to_extern)(void *pInstance, double IN, double* UP, double* DOWN); 
  
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
 public:
 
    static double mz_tol;
  static double tr_tol;
  static double min_align_error;
  static double max_align_error;
  static int intensity_bin_size;
  static double intensity_bin_tolerance;
  static bool VIEW;
  static double min_LC_MS_score;

  // class destructor
  ~LC_MS_correlation();
  
  // class constructor
  LC_MS_correlation(SM_cell*);
  
  ////////////////////////////////////////////////
  // compares for common lc_peak in two
  // spectra A / B
  void correlate_two_spectra(LC_MS*, LC_MS*);
  
  // computes the final peak correlation:
  double compute_peak_correlation();
  
  // empty the list of common peaks
  void clear_variables();
  
  // combines the scores:
  void combine_scores();

  // normalizes the total_score so it is more clear to 
  // o discriminate between bad / good runs
  void normalize_scores();
  
  // set the access to the retention time error:
  void set_alignment_error_access(void *pInstance, void(*pfct_get_alignment_error_to_extern)(void *pInstance, double, double*, double*));
  
  /////////////////////////////////////////////////////////
  // help functions for the intenisty correlation of 
  // two spectra
  //
  // create vectors of all intensities
  void process_intensity_ranking();
  // bin the list of intensities and create ranking accorindgly:
  void create_intensity_rank_list( vector<double>* , vector<double>* );

  //
  // get intensity bin by peak area
  double get_intensity_rank_A(double);
  double get_intensity_rank_B(double);
  //
  // compute spearman correlation
  void compute_spearman_correlation();
  
  // build up an alignment error score:
  void add_alignment_error();
  
  
  // this structure provides the function to compare
  // in the sorting algorithm:
  struct OPERATOR{
    // provide the compare function for sort:
    bool operator()(map<double,int>::iterator A,map<double,int>::iterator B) const{
      return (*A).first < (*B).first;
    }
  };
  
  // print the detailed output of the similarity scoring
  void print_detailed_analysis();
  // print ranking A
  void print_ranking_A(); 
  // print ranking B
  void print_ranking_B();
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  double get_correlation(){return (1/corr);};
  
  // build up the common feature list:
  void add_common_feature( LC_MS* );
  void add_common_feature(feature* IN_A, feature* IN_B){
    
    // save the peaks:
    peaks_A.push_back( IN_A );
    peaks_B.push_back( IN_B );
    
    // add up the intensity list:
    intensity_rank_A.push_back( IN_A->get_peak_area() );
    intensity_rank_B.push_back( IN_B->get_peak_area() );
    
    // define the means:
    mean_A += IN_A->get_peak_area();
    mean_B += IN_B->get_peak_area();
    
    IN_A = NULL;
    IN_B = NULL;
  };
  
  // return the size of teh peak list, does not matter which one, both the
  // same number of elements
  int get_nb_peaks(){return peaks_A.size();};
  double get_nb_common_peaks(){return double(peaks_A.size());};
  
  // get teh scores:
  double get_intensity_score(){return intensity_score;};
  double get_total_score(){return total_score;};
  
  // get the upper / lower error for a given retenetion time:
  void get_alignment_error(double);
  
  // get the alignment error score:
  double get_alignment_error_score(){return alignment_error_score;};

  // returns the normalized score!!!!
  double get_normalized_score(){return normalize_score;};

  ////////////////////////////////////////////////
  // returns the end score!!!!
  double get_main_score(){return normalize_score;};
  
};

#endif

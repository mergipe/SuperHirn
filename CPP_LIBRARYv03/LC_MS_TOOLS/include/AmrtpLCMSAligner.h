///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS ALIGNER:
// ALIGNMENT of the LC-MS spectra pair:
//
// according to Silva et al, 2005, "Quantitative Protemice Analysis by
// Accurate Mass Retention Time Pairs"
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef AMRTP_LC_MS_ALIGNER_H
#define AMRTP_LC_MS_ALIGNER_H

class AmrtpLCMSAligner : public AbstractLCMSAligner{

  ////////////////////////////////////////////////
  // declaration of the private members:

private:

  data_plotter* PLOT;
  
  int ITERATION_COUNT;
  vector<LC_MS_FEATURE>::iterator MERGE_ITERATOR_REFERENCE_LCMS;

  // vector of LCMS_alignment_feature_match:
  vector<LCMS_alignment_feature_match> match_list;
  vector<LCMS_alignment_feature_match> MS2_match_list;
  vector<LCMS_alignment_feature_match> non_match_list;
  vector<LCMS_alignment_feature_match> raw_match_list;
  
  // output directory for alignment statistics:
  string stat_out;
  
  /////////////////////////////////////////////////
  
  
  // this structure provides the function to compare
  // in the sorting algorithm:
  struct OPERATOR_by_RT_TO_BE_ALIGNED{
    // provide the compare function for sort:
    bool operator()(const LCMS_alignment_feature_match A,const LCMS_alignment_feature_match B) const
    {
      return A.TR_ToAlign < B.TR_ToAlign;
    }
  };
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  
  // class destructor
  ~AmrtpLCMSAligner();

  // class constructor
  AmrtpLCMSAligner();
  AmrtpLCMSAligner(bool);
  
  // sorts the saved LCMS_alignment_feature_match according to retention time B
  //void sort_LCMS_alignment_feature_match();
  //void sort_non_LCMS_alignment_feature_match();
  
  
  //////////////////////////////////////////////////
  // starts the preprocessing of the LCMS maps:
  void preprocessLCMSMaps();
  
  //////////////////////////////////////////////////
  // starts the modeling of the retention time drift:
  void modelRetentionTimeDrift();
    
  ///////////////////////////////
  // normalize the retention time scale in the run
  void normalizeRetentionTimeScales();
  
  
  // A. extract common features between LC/MS runs
  void extract_common_features();

  // create he retention time vs delta_retention time plot:
  void remove_multiple_LCMS_alignment_feature_match();
  
  // starts an alignemnt iteration of the LC-MS A to B:
  // 1. smoothes the Tr vs delta TR scatterplot with lowess procedure:
  // 2. adjusts the TR in spectrum B with the obtained delta_TrS
  void ITERATIVE_LOWESS_SMOOTH();
  // check alignment stopp, is ok if less than x% of the raw points
  // lay outside the alignemmt error bars:
  bool CHECK_ALIGNMENT_CONVERGENCE();
    
    
  
  // save alignment error:
  void save_alignment_error();
  
  // find the corresponding MS1 feature in LC/MS run A:
  void match_feature_in_LCMS_A( feature* );
  // finds the fitted delta TR at a given delta TR
  void save_MS1_feature_pair( feature* , feature* );

  // smoothes the Tr vs delta TR scatterplot with lowess procedure:
  void lowess_smooth();
  
  // browses through the spectra B and checks if it contains the
  // peak (TR | Mz):
  // void search_peak_in_REFERENCE_LCMS(feature*);
  // saves a MS2 matched peak pair: A = reference, B to be aligned
  //void save_MS2_match(double, double, double, double, int, int, int);
  // saves a MS1 matched peak pair: A = reference, B to be aligned
  // void save_match_by_shift(double, double, double, double, double);
  
  // combines MS1 and MS2 information:
  void combine_MS1_and_MS2_LCMS_alignment_feature_match();

  // saves not matched peak pair: A = reference, B to be aligned
  void save_non_LCMS_alignment_feature_match(double);
  
  // removes doubles in a vector of LCMS_alignment_feature_match
  void remove_non_match_doubles();
  void remove_doubles();
  
  // adjust the non matched delta TR:
  void adjust_non_matched_delta();
		
  // corrects a TR using the analysis results stored in
  // match_list
  void correct_TR(double*, double*, double*);
  
  // check the correlation of 2 LC_MS runs:
  void check_LC_MS_correlation(SM_cell*);
  
  // save the alignment error in the LC/MS run B:
  void save_alignment_error_in_B();
  
  /////////////////////////////////////////////////////////////////////////
  // here come all teh print out functions for teh statistics:
  // print the raw LCMS_alignment_feature_match = delta TR vs TR
  void PRINT_RAW_MATCHES();
  // print the fitted LCMS_alignment_feature_match = delta TR vs TR and the error
  void PRINT_ALIGNMENT();
  
  // clear up all previous analysis resulst / LC-MS runs:
  void clear_up();
  

    
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // save now the raw_match_list,
  // will be used later on for the error
  // calculations:
  void copy_2_raw_LCMS_alignment_feature_match(){raw_match_list = match_list;};
    
  
  int get_nb_features_REFERENCE(){return get_REFERENCE_LCMSMap( )->get_nb_features();};
  int get_nb_features_ALIGNED(){return get_ALIGNED_LCMSMap()->get_nb_features();};
  int get_nb_LCMS_alignment_feature_match(){return match_list.size();};
  
 //spectra names:
  string get_REFERENCE_LCMS_name(){return get_REFERENCE_LCMSMap( )->get_spec_name();};
  string get_ALIGNED_LCMS_name(){return get_ALIGNED_LCMSMap()->get_spec_name();};
  
  // get spec id:
  int get_REFERENCE_LCMS_ID(){return get_REFERENCE_LCMSMap( )->get_spectrum_ID();};
  int get_ALIGNED_LCMS_ID(){return get_ALIGNED_LCMSMap()->get_spectrum_ID();};
    
  // get the fitted LCMS_alignment_feature_match = one match per observed retention time
  // contains the up / down retention time correction uncertainty
  vector<LCMS_alignment_feature_match> get_alignment_error(){ return match_list;};
  
  // function which returns the retention time correction error
  // for a given input Tr B
  void get_alignment_error(double, double*, double*);
  // function which returns the search with at a given TR
  void get_TR_WINDOW(double , double* , double* );

  
  // function which returns the retention time correction error
  // for a given input Tr B and which is called from another object class:
  static void get_alignment_error_to_extern(void *pInstance, double, double*, double*);
  
  
};

#endif


///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS Spec MERGER:
// merges 2 spectra which have been preprocessed
// by spec_merge-> common lc_peaks are marked!!!!
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef LC_MS_MERGER_H
#define LC_MS_MERGER_H



class LC_MS_merger{

    
 ////////////////////////////////////////////////
 // declaration of the private members:
 
private:
 
 //////
 // the common lc-ms spectrum,
 // created from teh overlap of A and B
 LC_MS* MERGUS;
 LC_MS* A;
 LC_MS* B;
 vector<LC_MS_FEATURE>::iterator MERGE_ITERATOR_A;


 double DEBUG_MZ;
 
 vector<align_error> alignment_error;
 vector< match_region > ALL_MATCH_REGIONS;
 
 int common_counter;
 int non_common_counter;
 
 ////////////////////////////////////////////////
 // declaration of the public members:
 
public:
   
   static bool MS2_PROGRESSION;
 
 static bool DEBUG_LCMS_MERGER;
 static double FEATURE_MZ_MAX;
 static double FEATURE_MZ_MIN;
 
 
 // class destructor
 ~LC_MS_merger();
 // class constructor
 LC_MS_merger();
 
 
 /////////////////////////////////////////////////////
 // initialization functions:
 // set the spectra A / B which are the source for the
 // mmerged spectra:
 void set_input_spectra(LC_MS*, LC_MS*);
 // initialize the merged spectrum
 void init_merged_spectrum();
 
 
 /////////////////////////////////////////////////////
 // processing functions:
 // merges 2 spectra with each other and returns the
 void start_merging();		
 // extract the overlapping features betwween
 // 2 LC/MS runs
 void start_extracting_LCMS_overlap();
 // function check if 2 lc_peaks are similar or not!
 void check_common_lc_peaks(feature*,int,feature*);
 // go now again through both peak lists and add all the not common lc peaks:
 void add_non_common_peaks(LC_MS*, LC_MS*);
 // adds a common peak to the merged spectrum:
 void add_peak_to_MERGUS(feature*);
 
 
 // compare and extract the common peaks of A and B
 void extract_match_regions();
 // process the extracted match regions, check for 
 // double matches
 void PROCESS_MATCH_REGIONS();  
 // PROCESS the match regions:
 // check if its not matching to a feature,
 // which better matches to another one
 void CHECK_FEATURE_OVERLAP_MATCH( vector<match_region>::iterator);
 // creates a match region which contains all lc peaks falling within the
 // category of peaks fullfilling the input conditions:
 match_region* create_match_region( feature* );
 // searches a feature in merged spectrum by its ID:
 // ok, check here which region should have this matched feature
 void assign_feature_to_best_region( match_region*, match_region* );

 
 
 
 feature* find_feature_by_ID_in_MERGUS(int ID){return MERGUS->find_feature_by_ID(ID);};
 // search the features against the TARGET:
 void add_non_match_peaks(LC_MS*);
 // normalize the retention times over the matches:
 void normalize_retention_time(feature*, feature*);

 
 
 /////////////////////////////////////////////////////
 // access merged spectrum functions:
 // insert a common feature into the megred spectrum 
 void insert_fused_feature(feature* tmp){
  MERGUS->add_feature(tmp);
 };
 
 // this structure provides the function to compare
 // in the sorting algorithm:
 struct OPERATOR_MATCH_MZ{
   // provide the compare function for sort:
   bool operator()( match_region A, match_region B) const{
     // check if they have same mass
     if(A.get_MZ() == B.get_MZ()){
       return false;
     }
     else{
       return A.get_MZ() < B.get_MZ();
     }
   }
 };
 
 ///////////////////////////////
 // start here all the get / set
 // function to access the
 // variables of the class
 LC_MS* get_merged_spectrum(){return MERGUS;};
 
 // get the alignment error from LC/MS run B:
 void get_alignment_error(double TR, double* error_up,double* error_down){B->get_alignment_error(TR, error_up, error_down);};

 
 
};

#endif

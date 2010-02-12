///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS MultipleFeatureMatchRemover:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef MULTIPLE_FEATURE_MATCH_REMOVER_H
#define MULTIPLE_FEATURE_MATCH_REMOVER_H


class MultipleFeatureMatchRemover{
    
    ////////////////////////////////////////////////
    // declaration of the private members:

    vector<LCMS_alignment_feature_match>* DATA;

    // maximal number of high abundance peptides at same TR that is allowed
    
    
private:

    ////////////////////////////////////////////////
    // declaration of the public members:

public:

  static int max_nb_stripes;

    // class destructor
    ~MultipleFeatureMatchRemover();

    // class constructor
    MultipleFeatureMatchRemover( vector<LCMS_alignment_feature_match>* );

    //////////////////////////////////////////////////
    // copy constructor:
    MultipleFeatureMatchRemover& operator=(const MultipleFeatureMatchRemover&);

    // sorts the saved LCMS_alignment_feature_match according to Retention_To_Be_Aligned
    void sort_LCMS_alignment_feature_match_by_Retention_To_Be_Aligned();


    //////////////////////////////////////////////////
    // this structures provides the function to compare objects LCMS_alignment_feature_match
    // in the sorting algorithm:

    // sorts according to retention time of the toAlign LC-MS run
    struct OPERATOR_Retention_To_Be_Aligned{
        // provide the compare function for sort:
        bool operator()(const LCMS_alignment_feature_match A,const LCMS_alignment_feature_match B) const
     {
         return A.TR_ToAlign < B.TR_ToAlign;
     }
    };

  
    // sorts according to match index A
    struct OPERATOR_index_A{
        // provide the compare function for sort:
        bool operator()(const LCMS_alignment_feature_match A,const LCMS_alignment_feature_match B) const
     {
         return A.match_index_TR_Reference < B.match_index_TR_Reference;
     }
    };
    
    // sorts according to match index B
    struct OPERATOR_index_B{
        // provide the compare function for sort:
        bool operator()(const LCMS_alignment_feature_match A,const LCMS_alignment_feature_match B) const
     {
         return A.match_index_TR_ToAlign < B.match_index_TR_ToAlign;
     }
    };
    

    
    //////////////////////////////////////////////////
    // filter functions:

    // starts the smoothing process:
    // first smooth by TB, then by TA
    void start_smoothing();        

    // remove the high abundance in spectrum B first:
    // - sort according to retention time TB (=x)
    // - filter out those which have too many LCMS_alignment_feature_match at a TB
    void filter_by_Retention_To_Be_Aligned();


    // smooths the high abundance peptides out
    // gets the start and end of the stripes
    vector<LCMS_alignment_feature_match>::iterator smooth_high_abundance_by_index(vector<LCMS_alignment_feature_match>::iterator, vector<LCMS_alignment_feature_match>::iterator);


    // calculated the weighted peak of this
    // long eluting peptide
    void get_apex_in_sublist(vector<LCMS_alignment_feature_match>::iterator, vector<LCMS_alignment_feature_match>*);
        
    
    //////////////////////////////////////////////////
    // class operators


    ///////////////////////////////
    // start here all the get / set
    // function to access the
    // variables of the class

    // void set_raw_data( vector<LCMS_alignment_feature_match> IN ){DATA = IN;};
    // vector<LCMS_alignment_feature_match> get_smoothed_data(){return DATA;};
    int get_nb_data(){return DATA->size();};

};

#endif

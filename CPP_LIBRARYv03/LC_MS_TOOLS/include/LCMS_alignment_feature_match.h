///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS MATCHES:
// MATCHES features pair:
//
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef LCMS_ALIGNMENT_FEATURE_MATCH_H
#define LCMS_ALIGNMENT_FEATURE_MATCH_H
#define USE_STDIO
#define USE_NAMESPACE


class LCMS_alignment_feature_match{


    ////////////////////////////////////////////////
    // declaration of the private members:

private:


  feature* fReference;
  feature* fToAlign;

  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  
  
  
  // Retention times
  double TR_Reference_ORIGINAL;
  
  // To be aligned 
  // (to be corrected)
  double TR_ToAlign;
  double ToAlign_PeakArea;
  double TR_ToAlign_ORIGINAL;
  
    // index of which peak in other spectrum leads to the match
  int match_index_TR_Reference;
  int match_index_TR_ToAlign;    
  
		
  // retention time shift : TR_A - TR_B
  double delta;
  double delta_ORIGINAL;
  
  double weight;
  double alignment_error_up;
  double alignment_error_down;
  
  
  LCMS_alignment_feature_match(const LCMS_alignment_feature_match& tmp){
    
    fReference = tmp.fReference;
    fToAlign = tmp.fToAlign;
    ToAlign_PeakArea =  tmp.ToAlign_PeakArea;

    TR_Reference_ORIGINAL = tmp.TR_Reference_ORIGINAL;
    TR_ToAlign =  tmp.TR_ToAlign;
    TR_ToAlign_ORIGINAL = tmp.TR_ToAlign_ORIGINAL;
    weight = tmp.weight;
    match_index_TR_Reference = tmp.match_index_TR_Reference;
    match_index_TR_ToAlign = tmp.match_index_TR_ToAlign;
    delta = tmp.delta;
    delta_ORIGINAL = tmp.delta_ORIGINAL;
    alignment_error_down = tmp.alignment_error_down;
    alignment_error_up = tmp.alignment_error_up;
  };
  
  LCMS_alignment_feature_match(const LCMS_alignment_feature_match* tmp){

    fReference = tmp->fReference;
    fToAlign = tmp->fToAlign;
    ToAlign_PeakArea =  tmp->ToAlign_PeakArea;
    TR_Reference_ORIGINAL = tmp->TR_Reference_ORIGINAL;
    TR_ToAlign =  tmp->TR_ToAlign;
    TR_ToAlign_ORIGINAL = tmp->TR_ToAlign_ORIGINAL;
    weight = tmp->weight;
    match_index_TR_Reference = tmp->match_index_TR_Reference;
    match_index_TR_ToAlign = tmp->match_index_TR_ToAlign;
    delta = tmp->delta;
    delta_ORIGINAL = tmp->delta_ORIGINAL;
    alignment_error_down = tmp->alignment_error_down;
    alignment_error_up = tmp->alignment_error_up;
  };
  
  LCMS_alignment_feature_match(feature* iReference, feature* iToAlign){
    
    fReference = iReference;
    fToAlign = iToAlign;
    ToAlign_PeakArea = fToAlign->get_peak_area();
    TR_ToAlign = fToAlign->get_retention_time();

    initComponent();

  }

  ~LCMS_alignment_feature_match(){
    fReference = NULL;
    fToAlign = NULL;
  }
  
  // intializes the attributes of the component
  void initComponent(){
        
    if( ( fReference != NULL ) && (fToAlign != NULL) ){
      delta = get_TR_Reference() - get_TR_ToAlign();
    }
    else{
      delta = 0;
    }
    
    weight = 0;
    alignment_error_down = 0;
    alignment_error_up = 0;
    match_index_TR_Reference = 0;
    match_index_TR_ToAlign = 0;
  }
  
  
  LCMS_alignment_feature_match(double iTR_ToAlign){
    
    fReference = NULL;
    fToAlign = NULL;
    ToAlign_PeakArea = 0;

    initComponent();

    // define here  even if no match available:
    TR_ToAlign = iTR_ToAlign;
  }
  
  
  LCMS_alignment_feature_match& operator=(const LCMS_alignment_feature_match& tmp){

    fReference = tmp.fReference;
    fToAlign = tmp.fToAlign;
    ToAlign_PeakArea =  tmp.ToAlign_PeakArea;
    
    TR_Reference_ORIGINAL = tmp.TR_Reference_ORIGINAL;
    TR_ToAlign =  tmp.TR_ToAlign;
    TR_ToAlign_ORIGINAL = tmp.TR_ToAlign_ORIGINAL;
    
    weight = tmp.weight;
    match_index_TR_Reference = tmp.match_index_TR_Reference;
    match_index_TR_ToAlign = tmp.match_index_TR_ToAlign;
    delta = tmp.delta;
    delta_ORIGINAL = tmp.delta_ORIGINAL;
    alignment_error_down = tmp.alignment_error_down;
    alignment_error_up = tmp.alignment_error_up;
    return *this;
  };
    

  bool operator==(const LCMS_alignment_feature_match& tmp){
    if(TR_ToAlign == tmp.TR_ToAlign)
      return true;
    else
      return false;
  };
  
  /*
   bool operator<=(const LCMS_alignment_feature_match& tmp){
   return get_TR_ToAlign() <= tmp.get_TR_ToAlign();
   };
   
   bool operator>=(const LCMS_alignment_feature_match& tmp){
   return TR_ToAlign >= tmp.TR_ToAlign;
   };
  */
  
  
  double get_TR_Reference(){return fReference->get_retention_time();};
  double get_TR_Reference_ORIGINAL(){return TR_Reference_ORIGINAL;};
  
  double get_TR_ToAlign(){return TR_ToAlign;};
  
  double get_TR_ToAlign_ORIGINAL(){return TR_ToAlign_ORIGINAL;};
  
  double get_Delta(){return delta;};
  
  double get_Delta_ORIGINAL(){return delta_ORIGINAL;};
  
  void set_TR_ToAlign(double IN){TR_ToAlign = IN;};
  void set_TR_ToAlign_ORIGINAL(double IN){TR_ToAlign_ORIGINAL = IN;};
  
  void set_Delta(double IN){delta=IN;};
  void set_Delta_ORIGINAL(double IN){delta_ORIGINAL=IN;};
  
  void set_weight(double IN){weight=IN;};
    double get_weight(){return weight;};
  
  double get_alignment_error_up(){return alignment_error_up;};
  void set_alignment_error_up(double IN){alignment_error_up = IN;};
  double get_alignment_error_down(){return alignment_error_down;};
  void set_alignment_error_down(double IN){alignment_error_down = IN;};
  
  double get_peak_area_Reference(){    
    if( fReference != NULL ){
      return fReference->get_peak_area();
    }
    else{
      return 0;
    }
  };
  
  double get_peak_area_ToAlign(){return ToAlign_PeakArea;};
  void set_peak_area_ToAlign(double iArea){ToAlign_PeakArea = iArea;};
  
  void set_match_indexes(int iRefrIndex, int iToAlignIndex){match_index_TR_Reference = iToAlignIndex;match_index_TR_ToAlign = iToAlignIndex;};
  int get_match_index_TR_Reference(){return match_index_TR_Reference;};
  int get_match_index_TR_ToAlign(){return match_index_TR_ToAlign;};
  
};

#endif

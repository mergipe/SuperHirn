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


#ifndef MATCHES_H
#define MATCHES_H
#define USE_STDIO
#define USE_NAMESPACE


class matches{


    ////////////////////////////////////////////////
    // declaration of the private members:

private:

    ////////////////////////////////////////////////
    // declaration of the public members:

public:
  
  // TR_A
  double TA;
  double TA_ORIGINAL;
  
  // TR_B
  // (to be corrected)
  double TB;
  double TB_ORIGINAL;
  
    // index of which peak in other spectrum leads to the match
  int match_index_TA;
  int match_index_TB;    
  
  // index of which peak in other spectrum leads to the match
  double peak_area_A;
  double peak_area_B;    
		
		
  // retention time shift : TR_A - TR_B
  double delta;
  double delta_ORIGINAL;
  
  double weight;
  double alignment_error_up;
  double alignment_error_down;
  
  
  matches(const matches& tmp){
    TA =  tmp.TA;
    TA_ORIGINAL = tmp.TA_ORIGINAL;
    TB =  tmp.TB;
    TB_ORIGINAL = tmp.TB_ORIGINAL;
    weight = tmp.weight;
    peak_area_A = tmp.peak_area_A;
    peak_area_B = tmp.peak_area_B;
    match_index_TA = tmp.match_index_TA;
    match_index_TB = tmp.match_index_TB;
    delta = tmp.delta;
    delta_ORIGINAL = tmp.delta_ORIGINAL;
    alignment_error_down = tmp.alignment_error_down;
    alignment_error_up = tmp.alignment_error_up;
  };
  
  matches(const matches* tmp){
    TA =  tmp->TA;
    TA_ORIGINAL = tmp->TA_ORIGINAL;
    TB =  tmp->TB;
    TB_ORIGINAL = tmp->TB_ORIGINAL;
    weight = tmp->weight;
    peak_area_A = tmp->peak_area_A;
    peak_area_B = tmp->peak_area_B;
    match_index_TA = tmp->match_index_TA;
    match_index_TB = tmp->match_index_TB;
    delta = tmp->delta;
    delta_ORIGINAL = tmp->delta_ORIGINAL;
    alignment_error_down = tmp->alignment_error_down;
    alignment_error_up = tmp->alignment_error_up;
  };
  
  matches(double A, double B, double IN_peak_area_A, double IN_peak_area_B){
    ///////
    // define here what you x / y is:
    TA = A;
    TB = B;
    weight = 0;
    peak_area_A = IN_peak_area_A;
    peak_area_B = IN_peak_area_B;
    delta = TA - TB;
    alignment_error_down = 0;
    alignment_error_up = 0;
    match_index_TA = 0;
    match_index_TB = 0;
  }
  
  matches(double A, double B){
    ///////
    // define here what you x / y is:
    TA = A;
    TB = B;
    weight = 0;
    peak_area_A = 0;
    peak_area_B = 0;
    delta = TA - TB;
    alignment_error_down = 0;
    alignment_error_up = 0;
    match_index_TA = 0;
    match_index_TB = 0;
  }
  
  matches(double B){
    ///////
    // define here what you x / y is:
    TB = B;
    TA = 0;
    weight = 0;
    peak_area_A = 0;
    peak_area_B = 0;
    delta = 0;
    alignment_error_down = 0;
    alignment_error_up = 0;
    match_index_TA = 0;
    match_index_TB = 0;
  }
  
  void show_info(){
    printf("%f\t%f\n",TA,TB);
  };
  
  matches& operator=(const matches& tmp){
    TA =  tmp.TA;
    TA_ORIGINAL = tmp.TA_ORIGINAL;
    TB =  tmp.TB;
    TB_ORIGINAL = tmp.TB_ORIGINAL;
    weight = tmp.weight;
    match_index_TA = tmp.match_index_TA;
    match_index_TB = tmp.match_index_TB;
    peak_area_A = tmp.peak_area_A;
    peak_area_B = tmp.peak_area_B;
    delta = tmp.delta;
    delta_ORIGINAL = tmp.delta_ORIGINAL;
    alignment_error_down = tmp.alignment_error_down;
    alignment_error_up = tmp.alignment_error_up;
    return *this;
  };
    
  
  bool operator==(const matches& tmp){
    if((TA ==  tmp.TA)&&(TB ==  tmp.TB))
      return true;
    else
      return false;
  };
  
  bool operator<=(const matches& tmp){
    return TB <= tmp.TB;
  };
  
  bool operator>=(const matches& tmp){
    return TB >= tmp.TB;
  };
  
  
  double get_TA(){return TA;};
  double get_TA_ORIGINAL(){return TA_ORIGINAL;};
  double get_TB(){return TB;};
  double get_TB_ORIGINAL(){return TB_ORIGINAL;};
  double get_Delta(){return delta;};
  double get_Delta_ORIGINAL(){return delta_ORIGINAL;};
  void set_TA(double IN){TA = IN;};
  void set_TA_ORIGINAL(double IN){TA_ORIGINAL = IN;};
  void set_TB(double IN){TB = IN;};
  void set_TB_ORIGINAL(double IN){TB_ORIGINAL = IN;};
  void set_Delta(double IN){delta=IN;};
  void set_Delta_ORIGINAL(double IN){delta_ORIGINAL=IN;};
  void set_weight(double IN){weight=IN;};
  double get_weight(){return weight;};
  double get_alignment_error_up(){return alignment_error_up;};
  void set_alignment_error_up(double IN){alignment_error_up = IN;};
  double get_alignment_error_down(){return alignment_error_down;};
  void set_alignment_error_down(double IN){alignment_error_down = IN;};
  
  double get_peak_area_A(){return peak_area_A;};
  void set_peak_area_A(double IN){peak_area_A = IN;};
  double get_peak_area_B(){return peak_area_B;};
  void set_peak_area_B(double IN){peak_area_B = IN;};
  
  void set_match_indexes(int A, int B){match_index_TA = A;match_index_TB = B;};
  int get_match_index_TA(){return match_index_TA;};
  int get_match_index_TB(){return match_index_TB;};
  
};

#endif

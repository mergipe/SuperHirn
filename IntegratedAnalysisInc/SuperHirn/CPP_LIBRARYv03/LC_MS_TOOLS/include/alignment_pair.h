///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS alignment_pair:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef ALIGNMENT_PAIR_H
#define ALIGNMENT_PAIR_H



class alignment_pair{

  
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  string name;
  
  double correlation_before;
  double correlation_after;
  vector<alignment_pair> comparison_list;
  
  // add comparisons of spectra
  void add_to_comparison_list(alignment_pair IN){comparison_list.push_back(IN);};
  
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:

    // class destructor
    ~alignment_pair();
  
  // class constructor
  alignment_pair(string);
  alignment_pair(const alignment_pair&);
  alignment_pair(const alignment_pair*);

  
  //////////////////////////////////////////////////
  // overload operators:
  alignment_pair& operator=(const alignment_pair&);
  bool operator==(const string&);
  alignment_pair& operator<=(const alignment_pair&);
  alignment_pair& operator>=(const alignment_pair&);
  alignment_pair& operator<(const alignment_pair&);
  alignment_pair& operator>(const alignment_pair&);
  
    // shows the alignment result in nice table format:
  void print_pair_alignment_overview();
  
  // add comparisons of spectra
  void add_comparison_pair(string);
  
  // show content:
  void show_info();
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // spectra names:
  string get_name(){return name;};
  
  // correlation
  double get_correlation_before(){return correlation_before;};
  double get_correlation_after(){return correlation_after;};
  void set_pair_correlation_after(string,double);
  void set_pair_correlation_before(string,double);
  
  // compare the correlation of this to partner spectrum
  // after the alignment process:
  void set_correlation_after(double);
  // compare the correlation of this to partner spectrum
  // after the alignment process:
  void set_correlation_before(double);
  
        
  ///////////////////////////
  // here get parameters from comparison's
  alignment_pair get_alignment_pair(int i){return comparison_list[i];};
  alignment_pair* search_comparison_list_by_name(string);
  
  // get size of comparisons
  int get_nb_comparison(){return comparison_list.size();};
  
  // return the comparison list:
  vector<alignment_pair> get_alignment_list(){return comparison_list;};
  
};
#endif
    

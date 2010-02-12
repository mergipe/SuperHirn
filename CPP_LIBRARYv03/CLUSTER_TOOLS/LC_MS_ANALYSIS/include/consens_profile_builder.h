///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS consens_profile_builder:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef CONSENS_PROFILE_BUILDER_H
#define CONSENS_PROFILE_BUILDER_H



class consens_profile_builder{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  static int MAX_NB_PROILE_DATA_POINTS;
  static bool LOG_INTENSITY_TRANSFORMATION;
  // modus to normalize feature/peptide/profiles
  // 1: by manhatten distancs
  // 2: by the max of teh profile
  static int PROFILE_NORMALIZATION_MODE;
  
  // parameter defines the replicate number in a profile
  // these points will be averaged and a condensed profile is creates:
  static map<int, vector<int> > NB_REPLICATE_IDS;

  
  
  // class destructor
  ~consens_profile_builder();
  
  // class constructor
  consens_profile_builder();
  // class constructor
  consens_profile_builder(int);
  // class copy constructor
  consens_profile_builder(const consens_profile_builder&);
  // class copy constructor
  consens_profile_builder(const consens_profile_builder*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  consens_profile_builder& operator=(const consens_profile_builder&);
  bool operator==(const consens_profile_builder&);
  consens_profile_builder& operator<=(const consens_profile_builder&);
  consens_profile_builder& operator>=(const consens_profile_builder&);
  consens_profile_builder& operator<(const consens_profile_builder&);
  consens_profile_builder& operator>(const consens_profile_builder&);
  
  
  // build protein profile consensus
  map<int, pair<double, double> > build_AVERAGE_CONSENS( vector<feature*>* );
  map<int, pair<double, double> > build_AVERAGE_CONSENS( map<double, feature*>* );
  map<int, double> build_MEDIAN_CONSENS( vector<feature*>* );
  map<int, double> build_MEDIAN_CONSENS( map<double, feature*>* );
  
  // create a normalized profile:
  map< int, double > normalize_profile( vector<double> );
  map< int, double > normalize_profile( map<int, double> );
  // create a normalized profile:
  map< int, pair<double, double> > normalize_profile( map<int, pair<double,double> > );
  
  
  // if the replicate profile number is defined ( i.e. is bigger than 0 ), a 
  // condensed "replicate" profile is constrcuted where every n points are replaced by their 
  // mean ( n = NB_REPLICATE_POINTS)
  map<int, double> createReplicateCondensedProfile( map<int, double> );
    
  
  
  //*********************************************************************
  // here start different normalization functinalities  
  // 1. normalize the profile by dividing by the total sum
  // of y-values in the profile
  map< int, pair<double, double> > normalize_profile_by_TotalSUM( map<int, pair<double,double> >);
  map< int, double > normalize_profile_by_TotalSUM( map<int, double> );
  // 2. normalize the profile by dividing by the Max
  // of y-values in the profile
  map< int, double > normalize_profile_by_MAX( map<int, double> );
  map< int, pair<double, double> > normalize_profile_by_MAX( map<int, pair<double,double> > );

  
  // build profile of a feature
  map<int, double> build_feature_profile( feature* );

  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

};

#endif

    

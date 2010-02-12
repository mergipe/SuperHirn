///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS REGRESSOR:
// regression of Tr vs delta Tr
//
// methods implemented:
//
// - by median filter
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef FEATURE_MATCH_RETENTION_TIME_REGRESSOR_H
#define FEATURE_MATCH_RETENTION_TIME_REGRESSOR_H

class FeatureMatchRetentionTimeRegressor{


 ////////////////////////////////////////////////
 // declaration of the private members:
 
private:
 
 // vector of LCMS_alignment_feature_match:
 vector<LCMS_alignment_feature_match>* data;
 
 // list of retention times:
 vector<double> retention_time_list;
 
 ////////////////////////////////////////////////
 // declaration of the public members:

public:
   
  static double min_error;
  static double max_error;
  static double tr_error_smooth;
  
  // class destructor
  ~FeatureMatchRetentionTimeRegressor();
 
 // class constructor
 FeatureMatchRetentionTimeRegressor(vector<LCMS_alignment_feature_match>*);
 
 
 
 ///////////////////////////////////////////////////
 // functions for normal lowess:
 // fit the data with lowess fit, see lowess_manager
 void start_lowess_fit();
 // create the fit data; only consists of
 // single TR's, no delta's,
 void create_retention_time_list();
 // find the index of a given TR in the list
 int get_retention_time_index(double);
 // build up and down alignemnt errors:
 void create_LOWESS_ERRORS();

 ///////////////////////////////
 // start here all the get / set
 // function to access the
 // variables of the class
 
 // add a data from raw data to the bootstrape data sample
 void add_boot_data(int,int);
 
 double get_retention_time(int i){return retention_time_list[i];}
 int get_nb_retention_times(){return retention_time_list.size();}
 
 
};

#endif


///////////////////////////////////////////////////////////////////////////
//
//  PEAK DETECTION OF FOURIER TRANSFORME MS INSTRUMENT DATA
//
//  written by Markus Mueller, markus.mueller@imsb.biol.ethz.ch
//  and Lukas Mueller, Lukas.Mueller@imsb.biol.ethz.ch
//  October 2005
//
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//


#ifndef MS2_PROCESS_DATA_H
#define MS2_PROCESS_DATA_H

typedef map<int, ms_peak >::iterator elution_peak_iterator;

class MS2_Process_Data : public Process_Data{

    
    ////////////////////////////////////////////////
    // declaration of the private members:

private:

  // out vector of extracted MS2 features
  vector< MS2_feature > allMS2ConsenusSpectra;
  // map for the assembly of the ms2 spectra:
  multimap< double, MS2_feature > MS2ConsenusSpectraMap;
  
  bool gnuplot;
  
public:

  // minimal signal intensity
  static double MS2_INTENSITY_THRESHOLD;
  
  // MS1 (= precurosr) mass tolerance
  static double PRECURSOR_MZ_TOLERANCE;
  
  // how many times a fragment had to be detected at minima:
  static int MS2_MINIMAL_NB_FRAGMENT_DETECTION;
  // minimal number of MS2 consenus fragments in the MS2 spectrum:
  static int MINIMAL_NB_MS2_FRAGMENT_IN_SPECTRUM;
  // tr tolerance level to correlate elution profiles of ms2 feature with ms2 fragments
  static double MS2_TR_LC_CORRELATION_TOLERANCE_APEX;
  static double MS2_TR_LC_CORRELATION_TOLERANCE_BORDER;

  
  // MZ tolerance level to assemble MS2 frgament traces into MS2 consenus spectra
  static double MS2FragTraceOperatorTargetMZ;

  // MS2 fragment extraction:
  //static double MS2_intensity_apex_percentil_cutoff;
  static double MS2_TR_RESOLUTION;
  static bool CENTROID_DATA_MODUS;
  static double MS2_FRAGMENT_MIN_INTENSITY_AREA_THRESHOLD;

  // to track detected monoistopic mass for debugging:
  static bool MonoIsoDebugging;
  static double DebugMonoIsoMassMin; 
  static double DebugMonoIsoMassMax; 
  static bool POST_MSMS_SPECTRUM_CLUSTERING;
  
  

  // class destructor
  ~MS2_Process_Data();
  
  // class constructor
  MS2_Process_Data( );
  // class copy constructor
  MS2_Process_Data(const MS2_Process_Data&);
  
  // inputs raw data into the object:
  void add_scan_MS2_raw_data( int , double, float, int, float* );
  void add_scan_MS2_raw_data( Generic_MS_Scan*, CentroidData&);
  void add_scan_MS2_raw_data( double, float, int, vector<ms_peak>*);

  
  // check extracted MS2 level traces:
  // - dsicard those with less than minimal occurence (parameter 
  bool check_extracted_MS2_trace(elution_peak);
  // check if this is a valid ms2 consensus spectrum
  bool checkMS2ConsensusSpectrum( MS2_feature* );
  // checks if a mz value has already been seen,
  // also look for very close ones and cluster them
  MAIN_ITERATOR check_MZ_occurence(ms_peak*, bool);
  // function which check if a data structure iterator is similar
  // to a peak and should be considered 
  int compareIteratorToPeak(ms_peak*, MAIN_ITERATOR);
    

  // construction of MS2 Consensus Spectra:
  // - converts an extracted MS2 level traces into a MS2 fragment trace object
  // - assembles these then into MS2 consensus spectra
  void constructMS2ConsensusSpectra( );
    
  // construct conensus MS2 spectra by assembling the traces by their precursor MZ
  void assembleMS2FragmentTraceIntoMS2Scan( MS2FragmentTrace* );
  // check if a MS2 fragment is valid, i.e. kept for the following analysis steps
  bool checkMS2FragmentValid( MS2Fragment* );


  // find the correct MS2Consensus spectrum by precursor mz, z and tr:
  multimap< double, MS2_feature >::iterator findMS2ConsensSpectra( MS2Fragment* );
  
  // checks if MS2 fragment can be grouped into a existing MS2 feature,
  bool checkMS2FragmentToMS2FeatureBelonging(MS2Fragment* , MS2_feature*);


  // this structure provides the function to compare
  // in the lower_bound algorithm:
  class MS2FragTraceOperator{
    
public:
    
    static double TargetMZ;
    
    // provide the compare function for sort:
    bool operator()( MS2ConsensusSpectrum A, double value) {

      
      double delta = fabs( A.getPrecursorMZ() - value);
      if( delta <= MS2_Process_Data::PRECURSOR_MZ_TOLERANCE){
        return true;
      }
      else{
        return false;
      }
      
    }
    
    // with unary predicator to provide the compare function for sort:
    bool operator()( MS2ConsensusSpectrum A) {
      
      double delta = fabs( A.getPrecursorMZ() - MS2_Process_Data::MS2FragTraceOperatorTargetMZ);
      if( delta <= MS2_Process_Data::PRECURSOR_MZ_TOLERANCE){
        return true;
      }
      else{
        return false;
      }
      
    }
    
  };
  
  // sort the MS2_features:
  void sortMS2Features();
    
  // this structure provides the function to compare
  // in the sorting algorithm:
  struct OPERATOR_MS2_FEATURE{
    // provide the compare function for sort:
    bool operator()(const MS2_feature A,const MS2_feature B) const{
      // check if they have same mass
      double delta = fabs( A.precursorMZ - B.precursorMZ );
      if(delta <= MS2_Process_Data::PRECURSOR_MZ_TOLERANCE){
        return A.TR < B.TR;
      }
      else{
        return A.precursorMZ < B.precursorMZ;
      }
    }
  };
  
   
  //////////////////////////////////////////////////
  // overload operators:
  MS2_Process_Data& operator=(const MS2_Process_Data&);
  MS2_Process_Data& operator<=(const MS2_Process_Data&);
  MS2_Process_Data& operator>=(const MS2_Process_Data&);
  MS2_Process_Data& operator<(const MS2_Process_Data&);
  MS2_Process_Data& operator>(const MS2_Process_Data&);
  
 

  
  ///////////////////////////////////////////////////////////
  // access methods to the object variables:
  // print extracted MS2 level traces:
  void print_extracted_MS2_traces();
    
  // access to the fetaure vector:
  vector< MS2_feature >::iterator getMS2FeaturesStart(){ return allMS2ConsenusSpectra.begin();};
  vector< MS2_feature >::iterator getMS2FeaturesEnd(){ return allMS2ConsenusSpectra.end();};
  int getNbMS2Features(){ return allMS2ConsenusSpectra.size();};
};

#endif

    

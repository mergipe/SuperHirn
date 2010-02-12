///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 21.11.08
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS Abstract LC MS ALIGNER:
//
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef ABSTRACT_LC_MS_ALIGNER_H
#define ABSTRACT_LC_MS_ALIGNER_H

class AbstractLCMSAligner{

  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  // LC/MS runs which needed to be compared and aligned:
  // this one is the reference:
  LC_MS* referenceLCMSMap;
  // this one will be corrected in the Rt dimension:
  LC_MS* toAlignLCMSMap;
  
  
  
protected:
  
  // set the reference LC/MS run
  virtual LC_MS* get_REFERENCE_LCMSMap( ){ return referenceLCMSMap;};

  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  static bool print_out;
  static int MAX_ITERATION;
  
  // retention time window size:
  static double Tr_window;
  
  // mass window size:
  static double Mz_window;
  
  // percentage of delta points outsides of error bar:
  static double ERROR_DELTA_POINTS;
  
  // if sequence should be compared
  static bool ID_ALIGNMENT_WEIGHT;
  
  // mass tolerance:
  static double Mz_tolerance;
  
  // retention tolerance
  static double Tr_tolerance;
  
  // tolerance for delta tr in second fit:
  static double delta_tolerance;
  
  // class destructor
  virtual ~AbstractLCMSAligner();
  // class constructor
  AbstractLCMSAligner();
  AbstractLCMSAligner(bool);

  
  
  ///////////////////////////////
  // starts alignemnt of the 2 spec
  void START_LC_MS_Alignment();
  
  ///////////////////////////////
  // prepocess the LC-MS maps for the alignemnt
  virtual void preprocessLCMSMaps() = 0;
  
  ///////////////////////////////
  // determine the retention time model
  virtual void modelRetentionTimeDrift() = 0;
  
  ///////////////////////////////
  // prepocess the LC-MS maps for the alignemnt
  virtual void normalizeRetentionTimeScales() = 0;
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // set the reference LC/MS run
  virtual void set_REFERENCE_LCMSMap(LC_MS* IN){ referenceLCMSMap = IN;};

  // set the LC/MS run to align
  virtual void set_CORRECTION_LCMSMap(LC_MS* IN){ toAlignLCMSMap = IN;};
 
  // access the aligned LC/MS map:
  virtual LC_MS* get_ALIGNED_LCMSMap(){return toAlignLCMSMap;};
    
  // function which returns the retention time correction error
  // for a given input Tr B and which is called from another object class:
  static void get_alignment_error_to_extern(void *pInstance, double, double*, double*);
  
};

#endif


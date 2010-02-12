///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS LCMSmassCalibrator:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef LCMS_MASS_CALIBRATOR_H
#define LCMS_MASS_CALIBRATOR_H



class LCMSmassCalibrator{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  LC_MS* myRUN;
  
  
  // observed delta mass shifts:
  map<double, double> deltaMassShifts;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~LCMSmassCalibrator();
  
  // class constructor
  LCMSmassCalibrator(  );
  LCMSmassCalibrator( LC_MS* );
  // class copy constructor
  LCMSmassCalibrator(const LCMSmassCalibrator&);
  // class copy constructor
  LCMSmassCalibrator(const LCMSmassCalibrator*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  LCMSmassCalibrator& operator=(const LCMSmassCalibrator&);
  bool operator==(const LCMSmassCalibrator&);
  LCMSmassCalibrator& operator<=(const LCMSmassCalibrator&);
  LCMSmassCalibrator& operator>=(const LCMSmassCalibrator&);
  LCMSmassCalibrator& operator<(const LCMSmassCalibrator&);
  LCMSmassCalibrator& operator>(const LCMSmassCalibrator&);
  
  
  ////////////////////////////////////////////////////
  // start the mass calibration of MS1 features using the
  // theoretical mass of high quality MS2 informations
  void START_LCMS_MASS_CALIBRATION();
  void START_LCMS_MASS_CALIBRATION( LC_MS* );
  
  // extract first the subset of MS1 features which contain
  // high quality information:
  void extractDeltaMassDeviations();
  // store Delta Mass shift:
  void storeDeltaMassShift( double, double);
  
    
  // plot the mass shift:
  void plotMassShifts();

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

};

#endif

    

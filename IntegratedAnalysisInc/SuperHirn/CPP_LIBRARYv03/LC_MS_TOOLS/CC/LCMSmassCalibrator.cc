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

#define USE_LCMS_MASS_CALIBRATOR
#include "LC_MS_TOOLS_USE.h"


////////////////////////////////////////////////
// constructor for the object LCMSmassCalibrator:
LCMSmassCalibrator::LCMSmassCalibrator( ){
  myRUN = NULL;
}

////////////////////////////////////////////////
// constructor for the object LCMSmassCalibrator:
LCMSmassCalibrator::LCMSmassCalibrator(LC_MS* in){
  myRUN = in;
}

//////////////////////////////////////////////////
// class desctructor of LCMSmassCalibrator
LCMSmassCalibrator::~LCMSmassCalibrator(){
  myRUN = NULL;
  deltaMassShifts.clear();
}

//////////////////////////////////////////////////
// class copy constructor of LCMSmassCalibrator
LCMSmassCalibrator::LCMSmassCalibrator(const LCMSmassCalibrator& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of LCMSmassCalibrator
LCMSmassCalibrator::LCMSmassCalibrator(const LCMSmassCalibrator* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
LCMSmassCalibrator& LCMSmassCalibrator::operator=(const LCMSmassCalibrator& tmp){
  return *this;
}
    

////////////////////////////////////////////////////
// start the mass calibration of MS1 features using the
// theoretical mass of high quality MS2 informations
void LCMSmassCalibrator::START_LCMS_MASS_CALIBRATION(){
  START_LCMS_MASS_CALIBRATION( myRUN );
}  

////////////////////////////////////////////////////
// start the mass calibration of MS1 features using the
// theoretical mass of high quality MS2 informations
void LCMSmassCalibrator::START_LCMS_MASS_CALIBRATION( LC_MS* in){
  
  myRUN = in;
  deltaMassShifts.clear();
  
  // extract first the subset of MS1 features which contain
  // high quality information:
  extractDeltaMassDeviations();
  
  
}



////////////////////////////////////////////////////
// extract first the subset of MS1 features which contain
// high quality information:
void LCMSmassCalibrator::extractDeltaMassDeviations(){
  
  vector< LC_MS_FEATURE >::iterator I = myRUN->get_feature_list_begin();
  while( I != myRUN->get_feature_list_end() ){
    
    feature* fea = &(*I);
    
    // check if high quality MS2:
    if( fea->get_MS2_info() ){
      double oMZ = fea->get_MZ();
      double tMZ = fea->get_best_MS2_SCAN()->get_THEO_MZ();
      
      // store Delta Mass shift:
      storeDeltaMassShift( oMZ, tMZ);
    }
    I++;
  }
}


////////////////////////////////////////////////////
// store Delta Mass shift:
void LCMSmassCalibrator::storeDeltaMassShift( double oMZ, double tMZ){ 
  
  double delta = oMZ - tMZ;
  deltaMassShifts.insert( make_pair( oMZ, delta ) );
  
}


///////////////////////////////////////////////////////////
// plot the mass shift:
void LCMSmassCalibrator::plotMassShifts(){
  
  if( !deltaMassShifts.empty() ){

    char buffer[255];
    sprintf( buffer, "massDeviations_%s", myRUN->get_spec_name().c_str() );
    string tmp = buffer;
    data_plotter* PLOT = new data_plotter(tmp, true);
    
    sprintf( buffer, "deltaTR trend(%d points)", deltaMassShifts.size());
    
    PLOT->add_plot_data_dots( &deltaMassShifts, buffer );  
    
    // and now do the plotting:
    PLOT->plot_TWOD_data();
    
    delete PLOT;
    PLOT = NULL;
  }
}






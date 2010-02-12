///////////////////////////////////////////////////////////////////////////
//
//  PEAK DETECTION OF FOURIER TRANSFORME MS INSTRUMENT DATA
//
//  written by Markus Mueller, markus.mueller@imsb.biol.ethz.ch
//  ( and Lukas Mueller, Lukas.Mueller@imsb.biol.ethz.ch)
//  October 2005
//
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//

#define USE_GENERIC_MS_SCAN
#include "MS_PROCESSING_USE.h"




////////////////////////////////////////////////
// constructor for the object Generic_MS_Scan:
Generic_MS_Scan::Generic_MS_Scan(){
  rampMSPeakList = NULL;
  scanNumber = - 1;
  retentionTime = -1;
  preCursor = -1;
  preCursorZ = -1;
  scanMSLevel = -1;
  preCursorScan = -1;
  preCursorRT = -1;
  preCursorFound = false;
  
}

////////////////////////////////////////////////
// constructor for the object Generic_MS_Scan:
Generic_MS_Scan::Generic_MS_Scan( int iScan, double iRT, double iPreMz, int iPreZ, int iLevel ){
  rampMSPeakList = NULL;
  scanNumber = iScan;
  retentionTime = iRT;
  preCursor = iPreMz;
  preCursorZ = iPreZ;
  scanMSLevel = iLevel;
  preCursorScan = -1;
  preCursorRT = -1;
  preCursorFound = false;
  
}


//////////////////////////////////////////////////
// class desctructor of Generic_MS_Scan
Generic_MS_Scan::~Generic_MS_Scan(){
  
  if( rampMSPeakList != NULL ){
    delete rampMSPeakList;
    rampMSPeakList = NULL;
  }
  
}

//////////////////////////////////////////////////
// class copy constructor of Generic_MS_Scan
Generic_MS_Scan::Generic_MS_Scan(const Generic_MS_Scan& tmp){
  rampMSPeakList = tmp.rampMSPeakList;
  scanNumber = tmp.scanNumber;
  retentionTime = tmp.retentionTime;
  preCursor = tmp.preCursor;
  preCursorZ = tmp.preCursorZ;
  scanMSLevel = tmp.scanMSLevel;
  preCursorScan = tmp.preCursorScan;
  preCursorRT = tmp.preCursorRT;
  preCursorFound = tmp.preCursorFound;
  
  
}

//////////////////////////////////////////////////
// class copy constructor of Generic_MS_Scan
Generic_MS_Scan::Generic_MS_Scan(const Generic_MS_Scan* tmp){
  rampMSPeakList = tmp->rampMSPeakList;
  scanNumber = tmp->scanNumber;
  retentionTime = tmp->retentionTime;
  preCursor = tmp->preCursor;
  preCursorZ = tmp->preCursorZ;
  scanMSLevel = tmp->scanMSLevel;
  preCursorScan = tmp->preCursorScan;
  preCursorRT = tmp->preCursorRT;
  preCursorFound = tmp->preCursorFound;
}


//////////////////////////////////////////////////
// copy constructor:
Generic_MS_Scan& Generic_MS_Scan::operator=(const Generic_MS_Scan& tmp){
  rampMSPeakList = tmp.rampMSPeakList;
  scanNumber = tmp.scanNumber;
  retentionTime = tmp.retentionTime;
  preCursor = tmp.preCursor;
  preCursorZ = tmp.preCursorZ;
  scanMSLevel = tmp.scanMSLevel;
  preCursorScan = tmp.preCursorScan;
  preCursorRT = tmp.preCursorRT;
  preCursorFound = tmp.preCursorFound;
  return *this;
}
    
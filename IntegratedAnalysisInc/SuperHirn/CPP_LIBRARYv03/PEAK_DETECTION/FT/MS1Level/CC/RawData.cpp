/*
 *  RawData.cpp
 *  PeakDetection
 *
 *  Created by Markus Mueller on 10/19/06.
 *  Copyright 2006 ETHZ, IMSB, Switzerland. All rights reserved.
 *
 */

#define USE_RAW_DATA
#include "MS1_FT_PEAK_DETECTION_USE.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor & destructor
// This is the constructor used for mzXML files
RawData::RawData(
                 MY_RAMP_PEAKS* pProfileMassInt // Mass sample values in profile mode in mass/intensity pairs. -1 terminates
                 , double inMinIntensValue
                 ){
  
  LOW_INTENSITY_MS_SIGNAL_THRESHOLD = inMinIntensValue;  
  convert2Vector(pProfileMassInt);
}


// More general constructor
RawData::RawData(
                 vector<double>& pMassValues, // mass sample values 
                 vector<double>& pIntensValues // intensity sample values 
                 )
{
  fProfileMasses = pMassValues;
  fProfileIntens = pIntensValues;
}

// Destructor
RawData::~RawData()
{
}

// Operators

// Writes data to out stream using the << operator
ostream& operator<<(
                    ostream& pOut, // output stream 
                    RawData& pRawData) // 
{
  vector<double> m,h;
  vector<double>::iterator mi,hi;
  
  pRawData.get(m,h);
  for (mi=m.begin(),hi=h.begin();mi!=m.end();++mi,++hi) {
    pOut << fixed << setprecision(4) << *mi << " " << fixed << setprecision(2) << *hi << endl;
	}
  
  return pOut;
}


// Public methods

// Retrieve raw data as mass and intensity vectors
void RawData::get(
                  vector<double> &pProfileMasses, // Mass sample values in profile mode
                  vector<double> &pProfileIntens  // Intensity sample values in profile mode
                  ){
  
  pProfileMasses = fProfileMasses;
  pProfileIntens = fProfileIntens;

}

// Set raw data as mass and intensity vectors
void RawData::set(	vector<double> &pProfileMasses, // Mass sample values in profile mode
                   vector<double> &pProfileIntens  // Intensity sample values in profile mode
                   ){
  
	fProfileMasses = pProfileMasses;
	fProfileIntens = pProfileIntens;

}

// Set raw data as one float array of concat mass/intenstity values (for reading mzXML)
void RawData::set(
                   MY_RAMP_PEAKS* pProfileMassInt // Mass sample values in profile mode in mass/intensity pairs. -1 terminates
                  ){
  
  convert2Vector(pProfileMassInt);
}


///////////////////////////////////////////////////////////////////////////////
// Private methods

void RawData::convert2Vector(
                             MY_RAMP_PEAKS* pProfileMassInt // Mass sample values in profile mode in mass/intensity pairs. -1 terminates
                             ){
  int i = 0;
  while (pProfileMassInt[i]!= -1) {
    
    // discard low intens signals:
    if( pProfileMassInt[ i+ 1] >= LOW_INTENSITY_MS_SIGNAL_THRESHOLD){
      //cout<<pProfileMassInt[i]<<" "<<pProfileMassInt[i+1]<<endl;
      fProfileMasses.push_back(pProfileMassInt[i++]);
      fProfileIntens.push_back(pProfileMassInt[i++]);
    }
    else{
      i += 2;
    }
  }

}
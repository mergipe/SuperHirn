/*
 *  RawData.h
 *  PeakDetection
 *
 *  Created by Markus Mueller on 10/19/06.
 *  Copyright 2006 ETHZ, IMSB, Switzerland. All rights reserved.
 *
 */

#ifndef _RAW_DATA_h_
#define _RAW_DATA_h_


// Class for the storage of raw MS data
class RawData{

public:
  
  double LOW_INTENSITY_MS_SIGNAL_THRESHOLD;
   
		RawData( MY_RAMP_PEAKS*, double);
		RawData(vector<double>&,vector<double>&);
		virtual ~RawData();
		
		friend ostream& operator<<(ostream& pOut, RawData& pRawData);
  
		void get(vector<double>&,vector<double>&);
		void set(vector<double>&,vector<double>&);
		void set(MY_RAMP_PEAKS*);
		
		// Virtual functions
		virtual void smooth() {};
  
protected:
  
  void convert2Vector(MY_RAMP_PEAKS*);
  
		vector<double> fProfileMasses;
		vector<double> fProfileIntens;
};

ostream& operator<<(ostream& pOut, RawData& pRawData);

#endif

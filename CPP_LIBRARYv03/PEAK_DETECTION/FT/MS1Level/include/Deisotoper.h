/*
 *  Deisotoper.h
 *  PeakDetection
 *
 *  Created by Markus Mueller on 10/19/06.
 *  Copyright 2006 ETHZ, IMSB, Switzerland. All rights reserved.
 *
 */

#ifndef _DEISOTOPER_H_
#define _DEISOTOPER_H_

class CentroidData;
class DeconvPeak;

#include <list>
#include <vector>

using namespace std;

class Deisotoper 
{
public:
  static int sfMinCharge;
  static int	sfMaxCharge;

  
	Deisotoper();
	Deisotoper(CentroidData&);
	virtual ~Deisotoper();
	
	list<DeconvPeak>& getDeconvPeaks() {return fDeconvPeaks;}
	
	void go(CentroidData&);
	void cleanDeconvPeaks();
	
	inline int getMinPeakGroupSize() {return fMinPeakGroupSize;}
	inline double getTheta() {return fTheta;}
	inline int getScanNumber() {return fScanNumber;}
	inline bool getShortReportFlag() {return fShortReportFlag;}

	inline void setMinPeakGroupSize(int pMinPeakGroupSize) {fMinPeakGroupSize = pMinPeakGroupSize;}
	inline void setTheta(double pTheta) {fTheta = pTheta;}
	inline void setScanNumber(int pScanNumber) {fScanNumber = pScanNumber;}
	inline void setShortReportFlag(bool pShortReportFlag) {fShortReportFlag = pShortReportFlag;}
	
protected:

   list<DeconvPeak> fDeconvPeaks;
 
	int		fMinPeakGroupSize;
	double	fTheta;
	int		fScanNumber;
	bool	fShortReportFlag;
};

//ostream& operator<<(ostream&, Deisotoper&);

#endif
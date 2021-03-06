/*
 *  Deisotoper.cpp
 *  PeakDetection
 *
 *  Created by Markus Mueller on 10/19/06.
 *  Copyright 2006 ETHZ, IMSB, Switzerland. All rights reserved.
 *
 */

#include "Deisotoper.h"
#include "CentroidPeak.h"
#include "CentroidData.h"
#include "IsotopicDist.h"


#include <list>
#include <iostream>

int Deisotoper::sfMinCharge = 1; // minimum charge considered in peak detection
int	Deisotoper::sfMaxCharge = 5; // maximum charge considered in peak detection


// Default constructor
Deisotoper::Deisotoper()
{
  fMinPeakGroupSize = 0;
  fTheta = 0.0;
  fScanNumber = -1;
}

// Constructor. Takes centroide values and deisotopes them 
Deisotoper::Deisotoper(
                       CentroidData& pCentroidData) // Data objects containing centroid values
{
  go(pCentroidData);
}

// destructor
Deisotoper::~Deisotoper()
{
  fDeconvPeaks.clear();
}

// Operators

// Writes data to out stream using the << operator
ostream& operator<<(
                    ostream& pOut, // output stream 
                    Deisotoper& pDeisotoper) // 
{
  list<DeconvPeak> p;
  list<DeconvPeak>::iterator pi;
  
  p = pDeisotoper.getDeconvPeaks();
  
  for (pi=p.begin();pi!=p.end();++pi) {
    if (pDeisotoper.getShortReportFlag()) {
      pOut << *((CentroidPeak*) &(*pi)) << endl;
    } else {
      pOut << *pi << " " << pDeisotoper.getScanNumber() << endl;
    }
  }
  
  return pOut;
}

// Takes centroide values and deisotopes them 
void Deisotoper::go(
                    CentroidData& pCentroidData) // Data objects containing centroid values 
{
  int cnt,charge;
  double alpha;
  bool matched;
  list<CentroidPeak> centroidPeaks;
  list<CentroidPeak>::iterator start,end,pi;
  list<list<CentroidPeak>::iterator> matchedPeaks;
  
  pCentroidData.get(centroidPeaks);
  
  fMinPeakGroupSize = 2;
  
  if (CentroidData::sfMinIntensity<CentroidData::sfIntensityFloor) {
    pCentroidData.setNoise(30.0); // set noise level at 30 prcentile
    fTheta = pCentroidData.getNoise();
  } else {
    fTheta = CentroidData::sfMinIntensity;
  }
  
  pCentroidData.resetPeakGroupIter();
  while (pCentroidData.getNextPeakGroup(start,end)) { // isotopic patterns are withing the same peak group
    for (cnt=0,pi=start;pi!=end;++pi,++cnt);
    
    if (cnt>=fMinPeakGroupSize) { // Discard peak groups with only one peak
      for (pi=start;pi!=end;++pi,--cnt) {
        if (pi->getIntensity()<fTheta || cnt<fMinPeakGroupSize) continue; // skip small peaks

        if( CentroidData::MonoIsoDebugging ){
          if( ( CentroidData::DebugMonoIsoMassMin <= pi->getMass()) && ( CentroidData::DebugMonoIsoMassMax >= pi->getMass()) ){
            cout<<"To deisotope: "<<pi->getMass()<<" "<<pi->getIntensity()<<endl;
          }
        }
        
        for (charge=Deisotoper::sfMaxCharge;charge>=Deisotoper::sfMinCharge;--charge) {
                  
          matched = IsotopicDist::getMatchingPeaks(pi,end,charge,alpha,fTheta,matchedPeaks); // get peak that match isotopic pattern of charge
          if (matched && pi->getIntensity()>=fTheta) { // subtract isotopic match from peaks if match is significant
            
            if( CentroidData::MonoIsoDebugging ){
              if( ( CentroidData::DebugMonoIsoMassMin <= pi->getMass()) && ( CentroidData::DebugMonoIsoMassMax >= pi->getMass()) ){
                cout<<"matched to +"<<charge<<endl;
              }
            }
            
            DeconvPeak mono(pi->getMass(),0.0,charge,0,0.0,0.0);
            if( !pi->getExtraPeakInfo().empty() ){
              mono.setExtraPeakInfo(pi->getExtraPeakInfo());
            }
            
            if( CentroidData::MonoIsoDebugging ){
              if( ( CentroidData::DebugMonoIsoMassMin <= pi->getMass()) && ( CentroidData::DebugMonoIsoMassMax >= pi->getMass()) ){
                mono.show_info();
              }
            }
            
            
            IsotopicDist::subtractMatchingPeaks(matchedPeaks,charge,alpha,mono);
            fDeconvPeaks.push_back(mono);
                        
            if( CentroidData::MonoIsoDebugging ){
              if( ( CentroidData::DebugMonoIsoMassMin <= pi->getMass()) && ( CentroidData::DebugMonoIsoMassMax >= pi->getMass()) ){
                mono.show_info();
              }
            }
            
          }
          matchedPeaks.clear();
        }
      }
    }	
  }
}

// removes spooky or very small monoisotopic peaks
void Deisotoper::cleanDeconvPeaks()
{
	double tol,mass;
	list<DeconvPeak>::iterator pi,beg,end,most_intense;
	
	for (pi=fDeconvPeaks.begin();pi!=fDeconvPeaks.end();++pi) {
		beg = pi;
		mass = pi->getMass();
		most_intense = pi;
		tol =  CentroidData::sfMassTolPpm*mass/1.0e6 +  CentroidData::sfMassTolDa;
		++pi;
		for (;pi!=fDeconvPeaks.end();++pi) {// cluster peaks and define max intensity within cluster
			if (pi->getMass() > mass+2.0*tol) {
				break;
			}

			if (most_intense->getIntensity() < pi->getIntensity()) {
				most_intense = pi; // store most intense peak
			}
		}
		end = pi;
		
		for (pi=beg;pi!=fDeconvPeaks.end() && pi!=end;++pi) { // remove all 'very' small peak within cluster
//			cout << "remove: " << pi->getMass() << " " << pi->getIntensity() << " " << pi->getCharge() << " | " << most_intense->getMass() << " " << most_intense->getIntensity() << endl;
			if (most_intense->getIntensity() > 2.0*pi->getIntensity()) {
//				cout << "remove: " << pi->getMass() << " " << pi->getIntensity() << endl;
				pi = fDeconvPeaks.erase(pi);
				--pi;
			}
		}
		
		if (pi!=fDeconvPeaks.end()) {
			--pi;
		} else {
			break;
		}
	}
}

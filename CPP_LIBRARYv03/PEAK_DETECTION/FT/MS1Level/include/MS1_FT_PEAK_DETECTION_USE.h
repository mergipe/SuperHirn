///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 20.4.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
//  PARSER OF INTERACT FILE AND FOLLOWING EXTARCTION FROM mzXML
//
//
// **********************************************************************//


#ifndef __MS1_FT_PEAK_DETECTION_USE_H__
#define __MS1_FT_PEAK_DETECTION_USE_H__


///////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINED VARIABLES:
#define NOT_USED	-1
///////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////
// interface objects to the 
// FT PEAK DETECTION PROGRAM


#ifdef USE_ALL_MS1_FT_PEAK_DETECTION_CLASSES
#define USE_PROCESS_DATA
#define USE_LCMSCDATA
#define USE_LC_ELUTION_PEAK
#define USE_CENTROID_DATA
#define USE_DEISOTOPE_DATA
#define USE_PROCESS_DATA
#define USE_RAW_DATA
#define USE_ALL_MS_PROCESSING_CLASSES
#endif

#ifdef USE_PROCESS_DATA
#define USE_LC_ELUTION_PEAK
#define USE_CENTROID_DATA
#define USE_DEISOTOPE_DATA
#define USE_LC_MS_DATA
#define USE_ALL_MS_PROCESSING_CLASSES
#define INCLUDE_PROCESS_DATA
#endif

#ifdef USE_LC_MS_DATA
#define USE_LC_ELUTION_PEAK
#define USE_CENTROID_DATA
#define USE_ALL_MS_PROCESSING_CLASSES
#define INCLUDE_LC_MS_DATA
#endif


#ifdef USE_DEISOTOPE_DATA
#define USE_RAW_DATA
#define USE_CENTROID_DATA
#define USE_EXTERNAL_ISOTOPIC_DISTRIBUTION
#define USE_ALL_MS_PROCESSING_CLASSES
#define INCLUDE_DEISOTOPE_DATA
#endif

#ifdef USE_EXTERNAL_ISOTOPIC_DISTRIBUTION
#define USE_ALL_MS_PROCESSING_CLASSES
#define USE_CENTROID_DATA
#define INCLUDE_EXTERNAL_ISOTOPIC_DISTRIBUTION
#endif

#ifdef USE_CENTROID_DATA
#define USE_RAW_DATA
#define USE_ALL_MS_PROCESSING_CLASSES
#define INCLUDE_CENTROID_DATA
#endif

#ifdef USE_RAW_DATA
#define USE_ALL_MS_PROCESSING_CLASSES
#define INCLUDE_RAW_DATA
#endif

#ifdef USE_LC_ELUTION_PEAK
#define USE_ALL_MS_PROCESSING_CLASSES
#define INCLUDE_LC_ELUTION_PEAK
#endif

#ifdef USE_LC_ELUTION_PEAK
#define USE_ALL_MS_PROCESSING_CLASSES
#define INCLUDE_LC_ELUTION_PEAK
#endif




//////////////////////////////////////////////////
// reference to the ms processing classes:
#ifdef USE_ALL_MS_PROCESSING_CLASSES
#define INCLUDE_ALL_MS_PROCESSING_CLASSES
#endif
//////////////////////////////////////////////////


//////////////////////////////////////////////////
// reference to the general class library!!!!
#ifdef INCLUDE_ALL_MS_PROCESSING_CLASSES
#define USE_ALL_MS_PROCESSING_CLASSES
#include "MS_PROCESSING_USE.h"
#endif
//////////////////////////////////////////////////


////////////////////////////////////////////////////
// interface objects to the 
// FT PEAK DETECTION PROGRAM


#ifdef USE_LC_ELUTION_PEAK
#include "LC_elution_peak.h"
#endif

#ifdef INCLUDE_RAW_DATA
#include "RawData.h"
#endif

#ifdef INCLUDE_CENTROID_DATA
#include "CentroidData.h"
#endif


#ifdef INCLUDE_EXTERNAL_ISOTOPIC_DISTRIBUTION
#include "ExternalIsotopicDistribution.h"
#endif

#ifdef INCLUDE_DEISOTOPE_DATA
#define USE_ALL_MS_PROCESSING_CLASSES
#include "MS_PROCESSING_USE.h"
#include "CentroidPeak.h"
#include "ExternalIsotopicDistribution.h"
#include "Deisotoper.h"
#include "IsotopicDist.h"
#endif

#ifdef INCLUDE_LC_MS_DATA
#include "LCMSCData.h"
#endif

#ifdef INCLUDE_PROCESS_DATA
#include "Process_Data.h"
#endif

#ifdef INCLUDE_LC_ELUTION_PEAK
#include "LC_elution_peak.h"
#endif


#endif

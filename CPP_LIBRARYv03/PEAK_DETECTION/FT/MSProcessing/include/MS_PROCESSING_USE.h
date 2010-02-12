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


#ifndef __MS_PROCESSING_USE_H__
#define __MS_PROCESSING_USE_H__


///////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINED VARIABLES:
#define NOT_USED	-1
///////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////
// interface objects to the 


#ifdef USE_ALL_MS_PROCESSING_CLASSES
#define USE_BACKGROUND_CONTROL
#define USE_BACKGROUND_INTENSITY_BIN
#define USE_MS2_SPECTRUM_PROCESSOR
#define USE_PEPTIDE_ISOTOPE_DISTRIBUTION
#define USE_GENERIC_MS_SCAN
#define USE_MS_PEAK
#define USE_GENERAL_USE
#define USE_MZXML_ACCESS_USE
#define USE_ALL_MS_PROCESSING_CLASSES
#endif

#ifdef USE_BACKGROUND_CONTROL
#define USE_PEPTIDE_ISOTOPE_DISTRIBUTION
#define USE_MS_PEAK
#define USE_BACKGROUND_INTENSITY_BIN
#define USE_GENERAL_USE
#define INCLUDE_BACKGROUND_CONTROL
#endif

#ifdef USE_BACKGROUND_INTENSITY_BIN
#define USE_PEPTIDE_ISOTOPE_DISTRIBUTION
#define USE_MS_PEAK
#define USE_GENERAL_USE
#define INCLUDE_BACKGROUND_INTENSITY_BIN
#endif

#ifdef USE_MS2_SPECTRUM_PROCESSOR
#define USE_PEPTIDE_ISOTOPE_DISTRIBUTION
#define USE_MS2_FRAGMENT_TRACE
#define INCLUDE_MS2_SPECTRUM_PROCESSOR
#endif

#ifdef USE_MS_PEAK
#define USE_GENERAL_USE
#define USE_PEPTIDE_ISOTOPE_DISTRIBUTION
#define INCLUDE_MS_PEAK
#endif

#ifdef USE_GENERIC_MS_SCAN
#define USE_MZXML_ACCESS_USE
#define INCLUDE_GENERIC_MS_SCAN
#endif

#ifdef USE_CENTROID_DATA
#define USE_GENERAL_USE
#define USE_PEPTIDE_ISOTOPE_DISTRIBUTION
#define INCLUDE_CENTROID_DATA
#endif

#ifdef USE_PEPTIDE_ISOTOPE_DISTRIBUTION
#define USE_GENERAL_USE
#define INCLUDE_PEPTIDE_ISOTOPE_DISTRIBUTION
#endif


//////////////////////////////////////////////////
// reference to the general class library!!!!
#ifdef USE_GENERAL_USE
#define INCLUDE_GENERAL_USE
#endif

#ifdef USE_MZXML_ACCESS_USE
#define INCLUDE_MZXML_ACCESS_USE
#endif

//////////////////////////////////////////////////


//////////////////////////////////////////////////
// general mzXML access library!!!!
#ifdef INCLUDE_MZXML_ACCESS_USE
#define USE_ALL_MZXML_ACCESS_CLASSES
#include "MZXML_ACCESS_USE.h"
#endif


//////////////////////////////////////////////////
// reference to the general class library!!!!
#ifdef INCLUDE_GENERAL_USE
#define USE_ALL_GENERAL_CLASSES
#include "GENERAL_USE.h"
#endif
//////////////////////////////////////////////////


////////////////////////////////////////////////////
// interface objects to the 


#ifdef INCLUDE_PEPTIDE_ISOTOPE_DISTRIBUTION
#include "PeptideIsotopeDistribution.h"
#endif


#ifdef INCLUDE_CENTROID_DATA
#include "CentroidData.h"
#endif


#ifdef USE_MS_PEAK
#include "ms_peak.h"
#include "CentroidPeak.h"
#endif

#ifdef INCLUDE_GENERIC_MS_SCAN
#include "Generic_MS_Scan.h"
#endif


#ifdef INCLUDE_MS2_SPECTRUM_PROCESSOR
#include "MS2SpectrumProcessor.h"
#endif

#ifdef INCLUDE_BACKGROUND_INTENSITY_BIN
#include "BackgroundIntensityBin.h"
#endif

#ifdef INCLUDE_BACKGROUND_CONTROL
#include "BackgroundControl.h"
#endif


#endif

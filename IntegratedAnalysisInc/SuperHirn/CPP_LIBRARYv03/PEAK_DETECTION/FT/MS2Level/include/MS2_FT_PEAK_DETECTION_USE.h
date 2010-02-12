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


#ifndef __MS2_FT_PEAK_DETECTION_USE_H__
#define __MS2_FT_PEAK_DETECTION_USE_H__


///////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINED VARIABLES:
#define NOT_USED	-1
///////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////
// interface objects to the 
// FT PEAK DETECTION PROGRAM


#ifdef USE_ALL_MS2_FT_PEAK_DETECTION_CLASSES
#define USE_MS2_PROCESS_DATA
#define USE_MS2_FRAGMENT_TRACE
#define USE_ALL_MS1_FT_PEAK_DETECTION_CLASSES
#endif

#ifdef USE_MS2_PROCESS_DATA
#define USE_MS2_FRAGMENT_TRACE
#define USE_MS2_SPECTRUM_PROCESSOR
#define USE_ALL_MS1_FT_PEAK_DETECTION_CLASSES
#define INCLUDE_MS2_PROCESS_DATA
#endif

#ifdef USE_MS2_SPECTRUM_PROCESSOR
#define USE_MS2_FRAGMENT_TRACE
#define USE_ALL_MS1_FT_PEAK_DETECTION_CLASSES
#define INCLUDE_MS2_SPECTRUM_PROCESSOR
#endif

#ifdef USE_MS2_FRAGMENT_TRACE
#define USE_ALL_MS1_FT_PEAK_DETECTION_CLASSES
#define INCLUDE_MS2_FRAGMENT_TRACE
#endif


//////////////////////////////////////////////////
// reference to the general class library!!!!
#ifdef USE_ALL_MS1_FT_PEAK_DETECTION_CLASSES
#define INCLUDE_ALL_MS1_FT_PEAK_DETECTION_CLASSES
#endif
//////////////////////////////////////////////////


//////////////////////////////////////////////////
// reference to the general class library!!!!
#ifdef INCLUDE_ALL_MS1_FT_PEAK_DETECTION_CLASSES
#define USE_ALL_MS1_FT_PEAK_DETECTION_CLASSES
#include "MS1_FT_PEAK_DETECTION_USE.h"
#endif
//////////////////////////////////////////////////


////////////////////////////////////////////////////
// interface objects to the 
// FT PEAK DETECTION PROGRAM


#ifdef INCLUDE_MS2_FRAGMENT_TRACE
#include "MS2FragmentTrace.h"
#endif

#ifdef INCLUDE_MS2_SPECTRUM_PROCESSOR
#include "MS2SpectrumProcessor.h"
#endif

#ifdef INCLUDE_MS2_PROCESS_DATA
#include "MS2_Process_Data.h"
#endif



#endif

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


#ifndef __FT_PEAK_DETECTION_USE_H__
#define __FT_PEAK_DETECTION_USE_H__


///////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINED VARIABLES:
#define NOT_USED	-1
///////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////
// interface objects to the 
// FT PEAK DETECTION PROGRAM


#ifdef USE_ALL_FT_PEAK_DETECTION_CLASSES
#define USE_GENERAL_USE
#define USE_ALL_MS1_FT_PEAK_DETECTION_USE
#define USE_ALL_MS2_FT_PEAK_DETECTION_USE
#define USE_FT_PEAK_DETECT_CONTROLLER
#define USE_FT_PEAK_DETEC_MZXML_READER
#define USE_FT_PEAK_DETEC_INITIALIZER
#define USE_FT_PEAK_DETEC_PLOTTER
#endif


#ifdef USE_FT_PEAK_DETECT_CONTROLLER
#define USE_FT_PEAK_DETEC_MZXML_READER
#define USE_FT_PEAK_DETEC_INITIALIZER
#define USE_ALL_MS1_FT_PEAK_DETECTION_USE
#define USE_ALL_MS2_FT_PEAK_DETECTION_USE
#define INCLUDE_FT_PEAK_DETECT_CONTROLLER
#endif

#ifdef USE_FT_PEAK_DETEC_INITIALIZER
#define USE_FT_PEAK_DETEC_MZXML_READER
#define USE_FT_PEAK_DETEC_PLOTTER
#define USE_ALL_MS1_FT_PEAK_DETECTION_USE
#define USE_ALL_MS2_FT_PEAK_DETECTION_USE
#define INCLUDE_FT_PEAK_DETEC_INITIALIZER
#endif


#ifdef USE_FT_PEAK_DETEC_MZXML_READER
//#define USE_RAMP_PARSING_CLASSES
#define USE_GENERAL_USE
#define USE_ALL_MS1_FT_PEAK_DETECTION_USE
#define USE_ALL_MS2_FT_PEAK_DETECTION_USE
#define INCLUDE_FT_PEAK_DETEC_MZXML_READER
#endif

//////////////////////////////////////////////////
// RAMP Functions
/*
#ifdef USE_RAMP_PARSING_CLASSES
#define USE_CRAMP
#define USE_RAMP
#define USE_BASE64
#define INCLUDE_RAMP_PARSING_CLASSES
#endif

#ifdef USE_CRAMP
#define INCLUDE_CRAMP
#endif

#ifdef USE_RAMP
#define INCLUDE_RAMP
#endif

#ifdef USE_BASE64
#define INCLUDE_BASE64
#endif
 */


//////////////////////////////////////////////////
// reference to the general class library!!!!

#ifdef USE_ALL_MS2_FT_PEAK_DETECTION_USE
#define INCLUDE_ALL_MS2_FT_PEAK_DETECTION_USE
#endif

#ifdef USE_ALL_MS1_FT_PEAK_DETECTION_USE
#define INCLUDE_ALL_MS1_FT_PEAK_DETECTION_USE
#endif

#ifdef USE_GENERAL_USE
#define INCLUDE_GENERAL_USE
#endif
//////////////////////////////////////////////////


//////////////////////////////////////////////////
// reference to the general class library!!!!
#ifdef INCLUDE_GENERAL_USE
#define USE_ALL_GENERAL_CLASSES
#include "GENERAL_USE.h"
#endif

#ifdef INCLUDE_ALL_MS1_FT_PEAK_DETECTION_USE
#define USE_ALL_MS1_FT_PEAK_DETECTION_CLASSES
#include "MS1_FT_PEAK_DETECTION_USE.h"
#endif

#ifdef INCLUDE_ALL_MS2_FT_PEAK_DETECTION_USE
#define USE_ALL_MS2_FT_PEAK_DETECTION_CLASSES
#include "MS2_FT_PEAK_DETECTION_USE.h"
#endif
//////////////////////////////////////////////////



/*
////////////////////////////////////////////////////
// include RAMP-Function:
#ifdef INCLUDE_RAMP
#include "ramp.h"
#endif

#ifdef INCLUDE_BASE64
#include "base64.h"
#endif

#ifdef INCLUDE_CRAMP
#include "cramp.hpp"
#endif

*/


////////////////////////////////////////////////////
// interface objects to the 
// FT PEAK DETECTION PROGRAM


#ifdef INCLUDE_FT_PEAK_DETEC_MZXML_READER
#include "FT_PEAK_DETEC_mzXML_reader.h"
#endif

#ifdef INCLUDE_FT_PEAK_DETEC_INITIALIZER
#include "FT_PEAK_DETEC_initializer.h"
#endif

#ifdef INCLUDE_FT_PEAK_DETECT_CONTROLLER
#include "FT_PeakDetectController.h"
#endif


#endif

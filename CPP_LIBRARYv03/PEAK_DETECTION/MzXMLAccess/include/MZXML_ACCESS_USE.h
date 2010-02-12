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


#ifndef __MZXM_ACCESS_USE_H__
#define __MZXM_ACCESS_USE_H__


///////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINED VARIABLES:
#define NOT_USED	-1
///////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////
// interface objects to the 
// FT PEAK DETECTION PROGRAM


#ifdef USE_ALL_MZXML_ACCESS_CLASSES
#define USE_GENERAL_USE
#define USE_RAMP_PARSING_CLASSES
#endif

//////////////////////////////////////////////////
// RAMP Functions
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


//////////////////////////////////////////////////
// reference to the general class library!!!!

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
//////////////////////////////////////////////////




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

////////////////////////////////////////////////////
// interface objects to the 


#endif

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


#ifndef __MS2_TRACE_QUANTIFICATION_USE_H__
#define __MS2_TRACE_QUANTIFICATION_USE_H__


///////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINED VARIABLES:
#define NOT_USED	-1
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// define own classes:

#ifdef USE_ALL_MS2_TRACE_QUANTIFICATION_CLASSES
#define USE_ALIGNED_MS2_CONSENSUS_SPECTRUM
#define USE_MS2_QUANTIFICATION_CONTROLLER
#endif

#ifdef USE_MS2_QUANTIFICATION_CONTROLLER
#define USE_GENERAL_USE
#define USE_LC_MS_TOOLS_USE
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_ALIGNED_MS2_CONSENSUS_SPECTRUM
#define INCLUDE_MS2_QUANTIFICATION_CONTROLLER
#endif

#ifdef USE_ALIGNED_MS2_CONSENSUS_SPECTRUM
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_GENERAL_USE
#define INCLUDE_ALIGNED_MS2_CONSENSUS_SPECTRUM
#endif



//////////////////////////////////////////////////
// reference to the general class library!!!!
#ifdef USE_CLUSTER_LC_MS_ANALYSIS_USE
#define INCLUDE_CLUSTER_LC_MS_ANALYSIS_USE
#endif

#ifdef USE_LC_MS_TOOLS_USE
#define INCLUDE_LC_MS_TOOLS_USE
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

#ifdef INCLUDE_LC_MS_TOOLS_USE
#define USE_ALL_LC_MS_TOOLS_CLASSES
#include "LC_MS_TOOLS_USE.h"
#endif

#ifdef INCLUDE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_ALL_CLUSTER_LC_MS_ANALYSIS_CLASSES
#include "CLUSTER_LC_MS_ANALYSIS_USE.h"
#endif
//////////////////////////////////////////////////



#ifdef INCLUDE_ALIGNED_MS2_CONSENSUS_SPECTRUM
#include "AlignedMS2ConsensusSpectrum.h"
#endif


#ifdef INCLUDE_MS2_QUANTIFICATION_CONTROLLER
#include "MS2quantificationController.h"
#endif

#endif

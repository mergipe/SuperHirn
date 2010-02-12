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


#ifndef __RATIOLIZER_USE_H__
#define __RATIOLIZER_USE_H__


///////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINED VARIABLES:
#define NOT_USED	-1
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// define own classes:

#ifdef USE_ALL_RATIOLIZER_CLASSES
#define USE_RATIOLIZER_MANAGER
#define USE_PROTEIN_RATIOLIZER
#define USE_MS1_FEATURE_RATIOLIZER
#define USE_PEPTIDE_RATIO_ANALYZER
#define USE_STATISTIC_TOOLS_USE
#endif

#ifdef USE_RATIOLIZER_MANAGER
#define USE_STATISTIC_TOOLS_USE
#define USE_PROTEIN_RATIOLIZER
#define USE_MS1_FEATURE_RATIOLIZER
#define USE_PEPTIDE_RATIO_ANALYZER
#define INCLUDE_RATIOLIZER_MANAGER
#endif

#ifdef USE_PROTEIN_RATIOLIZER
#define USE_STATISTIC_TOOLS_USE
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define USE_MS1_FEATURE_RATIOLIZER
#define USE_PEPTIDE_RATIO_ANALYZER
#define INCLUDE_PROTEIN_RATIOLIZER
#endif

#ifdef USE_MS1_FEATURE_RATIOLIZER
#define USE_STATISTIC_TOOLS_USE
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define USE_PEPTIDE_RATIO_ANALYZER
#define INCLUDE_MS1_FEATURE_RATIOLIZER
#endif

#ifdef USE_PEPTIDE_RATIO_ANALYZER
#define USE_STATISTIC_TOOLS_USE
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define INCLUDE_PEPTIDE_RATIO_ANALYZER
#endif


//////////////////////////////////////////////////
// reference to the general class library!!!!
#ifdef USE_STATISTIC_TOOLS_USE
#define INCLUDE_STATISTIC_TOOLS_USE
#endif

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

#ifdef INCLUDE_STATISTIC_TOOLS_USE
#define USE_ALL_STATISTIC_TOOLS_CLASSES
#include "STATISTIC_TOOLS_USE.h"
#endif
//////////////////////////////////////////////////


#ifdef INCLUDE_PEPTIDE_RATIO_ANALYZER
#include "peptide_ratio_analyzer.h"
#endif

#ifdef INCLUDE_MS1_FEATURE_RATIOLIZER
#include "MS1_feature_ratiolizer.h"
#endif

#ifdef INCLUDE_PROTEIN_RATIOLIZER
#include "protein_ratiolizer.h"
#endif

#ifdef INCLUDE_RATIOLIZER_MANAGER
#include "ratiolizer_manager.h"
#endif

#endif

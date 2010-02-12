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


#ifndef __STATISTIC_TOOLS_USE_H__
#define __STATISTIC_TOOLS_USE_H__


///////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINED VARIABLES:
#define NOT_USED	-1
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// define own classes:

#ifdef USE_ALL_STATISTIC_TOOLS_CLASSES
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_FEATURE_UPDATER
#define USE_EM_FITTER
#define USE_DELTA_GROUPER
#define USE_PROTEIN_PROFILE_EVALUATOR
#define USE_PROFILE_GROUP
#define USE_PROTEIN_GROUP
#define USE_PEPTIDE_DELTA_GROUP
#define USE_PROFILE_SCORER
#endif

#ifdef USE_DELTA_GROUPER
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_FT_PEAK_DETECTION_CLASSES
#define USE_GENERAL_USE
#define USE_PROFILE_GROUP
#define USE_PROTEIN_GROUP
#define USE_PEPTIDE_DELTA_GROUP
#define INCLUDE_DELTA_GROUPER
#endif

#ifdef USE_FEATURE_UPDATER
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define USE_EM_FITTER
#define USE_DELTA_GROUPER
#define USE_PROFILE_GROUP
#define USE_PROTEIN_GROUP
#define USE_PEPTIDE_DELTA_GROUP
#define INCLUDE_FEATURE_UPDATER
#endif

#ifdef USE_PROTEIN_PROFILE_EVALUATOR
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_FT_PEAK_DETECTION_CLASSES
#define USE_PROFILE_GROUP
#define USE_PROTEIN_GROUP
#define USE_PEPTIDE_DELTA_GROUP
#define USE_GENERAL_USE
#define INCLUDE_PROTEIN_PROFILE_EVALUATOR
#endif

#ifdef USE_PROFILE_GROUP
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_FT_PEAK_DETECTION_CLASSES
#define USE_PROTEIN_GROUP
#define USE_PEPTIDE_DELTA_GROUP
#define USE_GENERAL_USE
#define INCLUDE_PROFILE_GROUP
#endif


#ifdef USE_PROTEIN_GROUP
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_PEPTIDE_DELTA_GROUP
#define USE_GENERAL_USE
#define INCLUDE_PROTEIN_GROUP
#endif

#ifdef USE_PEPTIDE_DELTA_GROUP
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_STATISTIC_TOOLS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define INCLUDE_PEPTIDE_DELTA_GROUP
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


#ifdef INCLUDE_PEPTIDE_DELTA_GROUP
#include "peptide_DELTA_group.h"
#endif

#ifdef INCLUDE_PROTEIN_GROUP
#include "protein_group.h"
#endif

#ifdef INCLUDE_PROFILE_GROUP
#include "profile_group.h"
#endif

#ifdef INCLUDE_PROTEIN_PROFILE_EVALUATOR
#include "protein_profile_evaluator.h"
#endif

#ifdef INCLUDE_FEATURE_UPDATER
#include "feature_updater.h"
#endif

#ifdef INCLUDE_DELTA_GROUPER
#include "DELTA_grouper.h"
#endif

#ifdef INCLUDE_EM_FITTER
#include "EM_fitter.h"
#endif

#endif

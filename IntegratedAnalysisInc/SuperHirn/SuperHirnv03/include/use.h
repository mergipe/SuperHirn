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


#ifndef __USE_H__
#define __USE_H__


///////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINED VARIABLES:
#define NOT_USED	-1
#define _Mono_H 1.00728
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// DEFINE first the namespaces:

///////////////////////////////////////////////////////////////////////////
// define own classes:

#ifdef USE_SUPERHIRN_MANAGER
#define USE_RATIOLIZER_USE
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_MS2_TRACE_QUANTIFICATION_USE
#define USE_STATISTIC_TOOLS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_FT_PEAK_DETECTION_CLASSES
#define USE_GENERAL_USE
#define USE_PROFILE_EVALUATOR
#define USE_TREE_BUILDER
#define USE_TREE_READER
#define USE_LCMS_DATA_IMPORTER
#define USE_DELTA_GROUPER
#define USE_STATISTIC_BOX
#define USE_PARAM_MANAGER
#define INCLUDE_SUPERHIRN_MANAGER
#endif


#ifdef USE_PARAM_MANAGER
#define USE_RATIOLIZER_USE
#define USE_PROFILE_EVALUATOR
#define USE_LCMS_DATA_IMPORTER
#define USE_STATISTIC_TOOLS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_FT_PEAK_DETECTION_CLASSES
#define USE_GENERAL_USE
#define USE_PARAM_INITIALIZER
#define INCLUDE_PARAM_MANAGER
#endif

#ifdef USE_PARAM_INITIALIZER
#define USE_RATIOLIZER_USE
#define USE_PROFILE_EVALUATOR
#define USE_LCMS_DATA_IMPORTER
#define USE_STATISTIC_TOOLS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_FT_PEAK_DETECTION_CLASSES
#define USE_GENERAL_USE
#define INCLUDE_PARAM_INITIALIZER
#endif

#ifdef USE_STATISTIC_BOX
#define USE_RATIOLIZER_USE
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_STATISTIC_TOOLS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_FT_PEAK_DETECTION_CLASSES
#define USE_GENERAL_USE
#define USE_COEFF_COMPUTER
#define USE_PROFILE_CLUSTER_TOOL
#define USE_LCMS_DATA_IMPORTER
#define USE_PROFILING_EVALUATOR
#define USE_PROFILE_COMPARER
#define INCLUDE_STATISTIC_BOX
#endif

#ifdef USE_PROFILING_EVALUATOR
#define USE_STATISTIC_TOOLS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_FT_PEAK_DETECTION_CLASSES
#define USE_GENERAL_USE
#define USE_PROFILE_COMPARER
#define INCLUDE_PROFILING_EVALUATOR
#endif

#ifdef USE_PROFILE_COMPARER
#define USE_STATISTIC_TOOLS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_FT_PEAK_DETECTION_CLASSES
#define USE_GENERAL_USE
#define INCLUDE_PROFILE_COMPARER
#endif

#ifdef USE_PROFILE_CLUSTER_TOOL
#define USE_STATISTIC_TOOLS_USE
#define USE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_FT_PEAK_DETECTION_CLASSES
#define USE_GENERAL_USE
#define USE_TREE_BUILDER
#define USE_MATRIX
#define USE_PROFILE_COMPARER
#define USE_PROFILE_GROUP
#define INCLUDE_PROFILE_CLUSTER_TOOL
#endif

#ifdef USE_LCMS_DATA_IMPORTER
#define USE_STATISTIC_TOOLS_USE
#define USE_LC_MS_TOOLS_USE
#define USE_FT_PEAK_DETECTION_CLASSES
#define USE_GENERAL_USE
#define USE_INITIALIZER
#define USE_MZXML_READER
#define USE_LCMSCDATA
#define USE_LC_ELUTION_PEAK
#define INCLUDE_LCMS_DATA_IMPORTER
#endif


//////////////////////////////////////////////////
// reference to the general class library!!!!
#ifdef USE_RATIOLIZER_USE
#define INCLUDE_RATIOLIZER_USE
#endif

#ifdef USE_CLUSTER_LC_MS_ANALYSIS_USE
#define INCLUDE_CLUSTER_LC_MS_ANALYSIS_USE
#endif

#ifdef USE_MS2_TRACE_QUANTIFICATION_USE
#define INCLUDE_MS2_TRACE_QUANTIFICATION_USE
#endif

#ifdef USE_STATISTIC_TOOLS_USE
#define INCLUDE_STATISTIC_TOOLS_USE
#endif

#ifdef USE_LC_MS_TOOLS_USE
#define INCLUDE_LC_MS_TOOLS_USE
#endif

#ifdef USE_FT_PEAK_DETECTION_CLASSES
#define INCLUDE_FT_PEAK_DETECTION_CLASSES
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

#ifdef INCLUDE_FT_PEAK_DETECTION_CLASSES
#define USE_ALL_FT_PEAK_DETECTION_CLASSES
#include "FT_PEAK_DETECTION_USE.h"
#endif

#ifdef INCLUDE_LC_MS_TOOLS_USE
#define USE_ALL_LC_MS_TOOLS_CLASSES
#include "LC_MS_TOOLS_USE.h"
#endif

#ifdef INCLUDE_STATISTIC_TOOLS_USE
#define USE_ALL_STATISTIC_TOOLS_CLASSES
#include "STATISTIC_TOOLS_USE.h"
#endif

#ifdef INCLUDE_CLUSTER_LC_MS_ANALYSIS_USE
#define USE_ALL_CLUSTER_LC_MS_ANALYSIS_CLASSES
#include "CLUSTER_LC_MS_ANALYSIS_USE.h"
#endif

#ifdef INCLUDE_MS2_TRACE_QUANTIFICATION_USE
#define USE_ALL_MS2_TRACE_QUANTIFICATION_CLASSES
#include "MS2_TRACE_QUANTIFICATION_USE.h"
#endif

#ifdef INCLUDE_RATIOLIZER_USE
#define USE_ALL_RATIOLIZER_CLASSES
#include "RATIOLIZER_USE.h"
#endif

//////////////////////////////////////////////////

#ifdef INCLUDE_LCMS_DATA_IMPORTER
#include "LCMSDataImporter.h"
#endif

#ifdef INCLUDE_PEPTIDE_DELTA_GROUP
#include "peptide_DELTA_group.h"
#endif

#ifdef INCLUDE_PROTEIN_GROUP
#include "protein_group.h"
#endif

#ifdef INCLUDE_COEFF_COMPUTER
#include "coeff_computer.h"
#endif

#ifdef INCLUDE_PROFILE_GROUP
#include "profile_group.h"
#endif

#ifdef INCLUDE_PROFILE_CLUSTER_TOOL
#include "profile_cluster_tool.h"
#endif

#ifdef INCLUDE_DELTA_GROUPER
#include "DELTA_grouper.h"
#endif

#ifdef INCLUDE_PROFILE_COMPARER
#include "profile_comparer.h"
#endif

#ifdef INCLUDE_PROFILING_EVALUATOR
#include "profiling_evaluator.h"
#endif

#ifdef INCLUDE_STATISTIC_BOX
#include "statistic_box.h"
#endif

#ifdef INCLUDE_PARAM_INITIALIZER
#include "param_initializer.h"
#endif

#ifdef INCLUDE_SUPERHIRN_MANAGER
#include "SuperHirn_manager.h"
#endif

#ifdef INCLUDE_PARAM_MANAGER
#include "paramManager.h"
#endif

#endif

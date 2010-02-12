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


#ifndef __CLUSTER_LC_MS_ANALYSIS_USE_H__
#define __CLUSTER_LC_MS_ANALYSIS__H__


///////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINED VARIABLES:
#define NOT_USED	-1
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// DEFINE first the namespaces:

///////////////////////////////////////////////////////////////////////////
// define own classes:

#ifdef USE_ALL_CLUSTER_LC_MS_ANALYSIS_CLASSES
#define USE_PROFILE_SCORER
#define USE_PROFILE_GROUP
#define USE_KMEAN_MAIN_CLUSTER
#define USE_CLUSTER_X_SCORER
#define USE_TREE_BUILDER
#define USE_SIM_MATRIX_PLOTTER
#define USE_TREE_BUILD_MATRIX
#define USE_MATRIX_ELEMENT
#define USE_CONSENS_PROFILE_BUILDER
#define USE_CELL
#endif

#ifdef USE_CLUSTER_X_SCORER
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define USE_KMEAN_MAIN_CLUSTER
#define INCLUDE_CLUSTER_X_SCORER
#endif

#ifdef USE_KMEAN_MAIN_CLUSTER
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define INCLUDE_KMEAN_MAIN_CLUSTER
#endif


#ifdef USE_TREE_BUILDER
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define USE_TREE_BUILD_MATRIX
#define INCLUDE_TREE_BUILDER
#endif

#ifdef USE_TREE_BUILD_MATRIX
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define USE_CELL
#define USE_SIM_MATRIX_PLOTTER
#define USE_MATRIX_ELEMENT
#define USE_LC_MS_SIMILARITY_MATRIX
#define INCLUDE_TREE_BUILD_MATRIX
#endif

#ifdef USE_SIM_MATRIX_PLOTTER
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define USE_CELL
#define USE_MATRIX_ELEMENT
#define INCLUDE_SIM_MATRIX_PLOTTER
#endif

#ifdef USE_MATRIX_ELEMENT
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define USE_CELL
#define INCLUDE_MATRIX_ELEMENT
#endif

#ifdef USE_CELL
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define INCLUDE_CELL
#endif

#ifdef USE_PROFILE_SCORER
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define USE_CONSENS_PROFILE_BUILDER
#define INCLUDE_PROFILE_SCORER
#endif

#ifdef USE_CONSENS_PROFILE_BUILDER
#define USE_LC_MS_TOOLS_USE
#define USE_GENERAL_USE
#define INCLUDE_CONSENS_PROFILE_BUILDER
#endif

//////////////////////////////////////////////////
// reference to the general class library!!!!

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
//////////////////////////////////////////////////

#ifdef INCLUDE_CONSENS_PROFILE_BUILDER
#include "consens_profile_builder.h"
#endif

#ifdef INCLUDE_PROFILE_SCORER
#include "profile_scorer.h"
#endif

#ifdef INCLUDE_CELL
#include "cell.h"
#endif

#ifdef INCLUDE_MATRIX_ELEMENT
#include "matrix_element.h"
#endif

#ifdef INCLUDE_TREE_BUILD_MATRIX
#include "tree_build_matrix.h"
#endif

#ifdef INCLUDE_TREE_BUILDER
#include "tree_builder.h"
#endif

#ifdef INCLUDE_KMEAN_MAIN_CLUSTER
#include "KMEAN_main_cluster.h"
#endif

#ifdef INCLUDE_CLUSTER_X_SCORER
#include "cluster_X_scorer.h"
#endif

#endif

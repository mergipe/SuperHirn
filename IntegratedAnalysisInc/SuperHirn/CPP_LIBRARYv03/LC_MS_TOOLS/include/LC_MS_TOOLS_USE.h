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


#ifndef __LC_MS_TOOLS_USE_H__
#define __LC_MS_TOOLS_USE_H__


///////////////////////////////////////////////////////////////////////////
// define LC_MS_TOOLS classes:


#ifdef USE_ALL_LC_MS_TOOLS_CLASSES
#define USE_GENERAL_USE
#define USE_AMRTP_LC_MS_ALIGNER
#define USE_MULTIPLE_FEATURE_MATCH_REMOVER
#define USE_MATCHES
#define USE_LC_MS_ALIGNMENT_TREE_PARSER
#define USE_TREE_READER
#define USE_NODE
#define USE_ALIGN_ERROR
#define USE_RESULT_MANAGER
#define USE_LC_MS_CORRELATION
#define USE_LC_MS_MERGER
#define USE_MS1_FEATURE_MERGER
#define USE_LC_MS_SIMILARITY_MATRIX
#define USE_SM_CELL
#define USE_MULTI_MERGER
#define USE_LCMS_MASS_CALIBRATOR
#define USE_MASTER_LC_MS_INTENSITY_NORMALIZER
#define USE_NORMALIZATION_COEFF_COMPUTER
#endif

#ifdef USE_MASTER_LC_MS_INTENSITY_NORMALIZER
#define USE_GENERAL_USE
#define USE_NORMALIZATION_COEFF_COMPUTER
#define USE_LC_MS_ALIGNMENT_TREE_PARSER
#define USE_IMPORTER
#define INCLUDE_MASTER_LC_MS_INTENSITY_NORMALIZER
#endif

#ifdef USE_NORMALIZATION_COEFF_COMPUTER
#define USE_GENERAL_USE
#define INCLUDE_NORMALIZATION_COEFF_COMPUTER
#endif

#ifdef USE_MULTI_MERGER
#define USE_GENERAL_USE
#define USE_TREE_READER
#define USE_NODE
#define USE_LC_MS_ALIGNER
#define USE_PLOTTER
#define USE_LC_MS_SIMILARITY_MATRIX
#define USE_LC_MS_MERGER
#define USE_SEQUENTIAL_TREE_CREATOR
#define USE_SEARCHER
#define INCLUDE_MULTI_MERGER
#endif

#ifdef USE_SEQUENTIAL_TREE_CREATOR
#define USE_GENERAL_USE
#define USE_TREE_READER
#define USE_NODE
#define INCLUDE_SEQUENTIAL_TREE_CREATOR
#endif


#ifdef USE_RESULT_MANAGER
#define USE_GENERAL_USE
#define USE_ALIGNMENT_PAIR
#define INCLUDE_RESULT_MANAGER
#endif

#ifdef USE_LC_MS_ALIGNMENT_TREE_PARSER
#define USE_GENERAL_USE
#define USE_TREE_READER
#define USE_NODE
#define INCLUDE_LC_MS_ALIGNMENT_TREE_PARSER
#endif

#ifdef USE_TREE_READER
#define USE_NODE
#define USE_GENERAL_USE
#define INCLUDE_TREE_READER
#endif

#ifdef USE_NODE
#define USE_GENERAL_USE
#define USE_ALIGNMENT_PAIR
#define INCLUDE_NODE
#endif

#ifdef USE_ALIGNMENT_PAIR
#define USE_GENERAL_USE
#define USE_SPEC_CORRELATION
#define USE_NODE
#define INCLUDE_ALIGNMENT_PAIR
#endif


#ifdef USE_AMRTP_LC_MS_ALIGNER
#define USE_GENERAL_USE
#define USE_LCMS_ALIGNMENT_FEATURE_MATCH
#define USE_ALIGN_ERROR
#define USE_MATCH_REGION
#define USE_FEATURE_MATCH_RETENTION_TIME_REGRESSOR
#define USE_PLOTTER
#define USE_LC_MS_MERGER
#define USE_LC_MS_CORRELATION
#define USE_SM_CELL
#define USE_MULTIPLE_FEATURE_MATCH_REMOVER
#define USE_ABSTRACT_LC_MS_ALIGNER
#define INCLUDE_AMRTP_LC_MS_ALIGNER
#endif

#ifdef USE_ABSTRACT_LC_MS_ALIGNER
#define USE_GENERAL_USE
#define INCLUDE_ABSTRACT_LC_MS_ALIGNER
#endif


#ifdef USE_LCMS_MASS_CALIBRATOR
#define USE_GENERAL_USE
#define USE_REGRESSOR
#define USE_PLOT_SMOOTHER
#define INCLUDE_LCMS_MASS_CALIBRATOR
#endif

#ifdef USE_MULTIPLE_FEATURE_MATCH_REMOVER
#define USE_GENERAL_USE
#define USE_LCMS_ALIGNMENT_FEATURE_MATCH
#define INCLUDE_MULTIPLE_FEATURE_MATCH_REMOVER
#endif

#ifdef USE_LC_MS_CORRELATION
#define USE_GENERAL_USE
#define USE_MATCH_REGION
#define USE_LC_MS_MERGER
#define USE_SM_CELL
#define INCLUDE_LC_MS_CORRELATION
#endif

#ifdef USE_LC_MS_MERGER
#define USE_GENERAL_USE
#define USE_ALIGN_ERROR
#define USE_MATCH_REGION
#define USE_MATCH_REGION_CANDIDATE
#define USE_SEARCHER
#define INCLUDE_LC_MS_MERGER
#endif

#ifdef USE_MATCH_REGION
#define USE_MS2_SPECTRUM_CORRELATOR
#define USE_GENERAL_USE
#define INCLUDE_MATCH_REGION
#endif

#ifdef USE_MATCH_REGION_CANDIDATE
#define USE_GENERAL_USE
#define INCLUDE_MATCH_REGION_CANDIDATE
#endif

#ifdef USE_MS2_SPECTRUM_CORRELATOR
#define USE_FT_PEAK_DETECTION_CLASSES
#define USE_GENERAL_USE
#define INCLUDE_MS2_SPECTRUM_CORRELATOR
#endif

#ifdef USE_FEATURE_MATCH_RETENTION_TIME_REGRESSOR
#define USE_GENERAL_USE
#define USE_LCMS_ALIGNMENT_FEATURE_MATCH
#define INCLUDE_FEATURE_MATCH_RETENTION_TIME_REGRESSOR
#endif

#ifdef USE_ALIGN_ERROR
#define USE_GENERAL_USE
#define INCLUDE_ALIGN_ERROR
#endif

#ifdef USE_LCMS_ALIGNMENT_FEATURE_MATCH
#define USE_GENERAL_USE
#define INCLUDE_LCMS_ALIGNMENT_FEATURE_MATCH
#endif

#ifdef USE_LC_MS_SIMILARITY_MATRIX
#define USE_GENERAL_USE
#define USE_SM_CELL
#define INCLUDE_LC_MS_SIMILARITY_MATRIX
#endif

#ifdef USE_SM_CELL
#define USE_GENERAL_USE
#define INCLUDE_SM_CELL
#endif


//////////////////////////////////////////////////
// reference to the general class library!!!!
#ifdef USE_FT_PEAK_DETECTION_CLASSES
#define INCLUDE_FT_PEAK_DETECTION_CLASSES
#endif


#ifdef USE_GENERAL_USE
#define INCLUDE_GENERAL_USE
#endif

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

//////////////////////////////////////////////////

////////////////////////////////////////////////////
//include own classes:

#ifdef INCLUDE_SM_CELL
#include "SM_cell.h"
#endif

#ifdef INCLUDE_LC_MS_SIMILARITY_MATRIX
#include "LC_MS_similarity_matrix.h"
#endif

#ifdef INCLUDE_LCMS_ALIGNMENT_FEATURE_MATCH
#include "LCMS_alignment_feature_match.h"
#endif

#ifdef INCLUDE_ALIGN_ERROR
#include "align_error.h"
#endif

#ifdef INCLUDE_FEATURE_MATCH_RETENTION_TIME_REGRESSOR
#include "FeatureMatchRetentionTimeRegressor.h"
#endif

#ifdef INCLUDE_MULTIPLE_FEATURE_MATCH_REMOVER
#include "MultipleFeatureMatchRemover.h"
#endif

#ifdef INCLUDE_LCMS_MASS_CALIBRATOR
#include "LCMSmassCalibrator.h"
#endif

#ifdef INCLUDE_ABSTRACT_LC_MS_ALIGNER
#include "AbstractLCMSAligner.h"
#endif


#ifdef INCLUDE_AMRTP_LC_MS_ALIGNER
#include "AmrtpLCMSAligner.h"
#endif

#ifdef INCLUDE_ALIGN_PROBABILITY
#include "align_probability.h"
#endif


#ifdef INCLUDE_MS2_SPECTRUM_CORRELATOR
#include "MS2SpectraCorrelator.h"
#endif

#ifdef INCLUDE_MATCH_REGION_CANDIDATE
#include "match_region_candidate.h"
#endif

#ifdef INCLUDE_MATCH_REGION
#include "match_region.h"
#endif

#ifdef INCLUDE_LC_MS_MERGER
#include "LC_MS_merger.h"
#endif

#ifdef INCLUDE_LC_MS_CORRELATION
#include "LC_MS_correlation.h"
#endif

#ifdef INCLUDE_ALIGNMENT_PAIR
#include "alignment_pair.h"
#endif

#ifdef INCLUDE_NODE
#include "node.h"
#endif

#ifdef INCLUDE_TREE_READER
#include "tree_reader.h"
#endif

#ifdef INCLUDE_LC_MS_ALIGNMENT_TREE_PARSER
#include "LC_MS_alignment_tree_parser.h"
#endif

#ifdef INCLUDE_RESULT_MANAGER
#include "result_manager.h"
#endif


#ifdef INCLUDE_SEQUENTIAL_TREE_CREATOR
#include "SequentialTreeCreator.h"
#endif

#ifdef INCLUDE_MULTI_MERGER
#include "multi_merger.h"
#endif

#ifdef INCLUDE_NORMALIZATION_COEFF_COMPUTER
#include "normalization_coeff_computer.h"
#endif

#ifdef INCLUDE_MASTER_LC_MS_INTENSITY_NORMALIZER
#include "MASTER_LC_MS_intensity_normalizer.h"
#endif

#endif

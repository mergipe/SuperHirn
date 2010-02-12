///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 21.12.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
//  INLCUDE FILE FOR GENERAL LIBRARY OBJECTS//
//
// - read param
// - file reader
// - file sys
// - progress bar
// - lowess manager
//
// - feature
// **********************************************************************//


#ifndef __GENERAL_USE_H__
#define __GENERAL_USE_H__


///////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINED VARIABLES:

// MODIFICATIONS SHIFTS:
#define DEAMIDATION_SHIFT 0.98402





///////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////
// define own classes:

#ifdef USE_ALL_GENERAL_CLASSES
#define USE_FILE_SYS
#define USE_FILE_READER
#define USE_LOWESS_REGRESSION_MANAGER
#define USE_READ_PARAM
#define USE_CRAMP
#define USE_RAMP
#define USE_BASE64
#define USE_EM_FITTER
#define USE_PS2OTHER
#define USE_DATA_PLOTTER
#define USE_G2
#define USE_PROGRESS_BAR
#define USE_TINY_XML
#define USE_LC_MS_OBJECTS
#define USE_TPP_OBJECTS
#define USE_ALL_MATH_TOOLS
#endif

#ifdef USE_LC_MS_OBJECTS
#define USE_MS1_FEATURE_MERGER
#define USE_LC_MS
#define USE_FEATURE
#define USE_MS2_CONSENSUS_SPECTRUM
#define USE_MS2_PROTEIN_GROUP
#define USE_MS2_INFO
#define USE_MS2_FEATURE
#define USE_MS2_FRAGMENT
#define USE_FEATURE_LC_PROFILE
#define USE_CONSENS_ISOTOPE_PATTERN
#define USE_ALL_MATH_TOOLS
#endif

#ifdef USE_TPP_OBJECTS
#define USE_PROT_XML_PARSER
#define USE_GENERAL_MSMS_XML_PARSER
#define USE_LC_MS_WRITER
#define USE_LC_MS_APML_WRITER
#define USE_LC_MS_XML_WRITER
#define USE_LC_MS_XML_READER
#define USE_ALL_MATH_TOOLS
#endif

#ifdef USE_LC_MS_WRITER
#define USE_LC_MS_XML_WRITER
#define USE_LC_MS_APML_WRITER
#define USE_LC_MS_XML_READER
#define INCLUDE_LC_MS_WRITER
#endif

#ifdef USE_LC_MS_XML_READER
#define USE_TINY_XML
#define USE_MATCHES
#define USE_LC_MS
#define USE_FEATURE
#define USE_FEATURE_LC_PROFILE
#define USE_CONSENS_ISOTOPE_PATTERN
#define USE_MS2_INFO
#define USE_LC_MS_XML_WRITER
#define USE_ALL_MATH_TOOLS
#define INCLUDE_LC_MS_XML_READER
#endif

#ifdef USE_LC_MS_APML_WRITER
#define USE_MATCHES
#define USE_MS2_MS1_MATCHER
#define USE_LC_MS_XML_WRITER
#define USE_LC_MS
#define USE_FEATURE
#define USE_FEATURE_LC_PROFILE
#define USE_MS2_INFO
#define USE_ALL_MATH_TOOLS
#define INCLUDE_LC_MS_APML_WRITER
#endif

#ifdef USE_LC_MS_XML_WRITER
#define USE_MATCHES
#define USE_MS2_MS1_MATCHER
#define USE_LC_MS
#define USE_FEATURE
#define USE_FEATURE_LC_PROFILE
#define USE_MS2_INFO
#define USE_ALL_MATH_TOOLS
#define INCLUDE_LC_MS_XML_WRITER
#endif

#ifdef USE_PROT_XML_PARSER
#define USE_FILE_SYS
#define USE_FILE_READER
#define USE_READ_PARAM
#define USE_FEATURE
#define USE_MS2_PROTEIN_GROUP
#define USE_MS2_INFO
#define USE_GENERAL_XML_PARSER
#define USE_ALL_MATH_TOOLS
#define INCLUDE_PROT_XML_PARSER
#endif

#ifdef USE_GENERAL_MSMS_XML_PARSER
#define USE_INTERACT_PARSER
#define USE_XQUEST_XML_PARSER
#define USE_GENERAL_XML_PARSER
#define INCLUDE_GENERAL_MSMS_XML_PARSER
#endif

#ifdef USE_XQUEST_XML_PARSER
#define USE_XML_FORMAT_PARSER_INTERFACE
#define USE_FILE_SYS
#define USE_FILE_READER
#define USE_READ_PARAM
#define USE_FEATURE
#define USE_INTERACT_PARSER
#define USE_MS2_INFO
#define USE_GENERAL_XML_PARSER
#define USE_ALL_MATH_TOOLS
#define INCLUDE_XQUEST_XML_PARSER
#endif

#ifdef USE_INTERACT_PARSER
#define USE_XML_FORMAT_PARSER_INTERFACE
#define USE_FILE_SYS
#define USE_FILE_READER
#define USE_READ_PARAM
#define USE_FEATURE
#define USE_MS2_INFO
#define USE_GENERAL_XML_PARSER
#define USE_ALL_MATH_TOOLS
#define INCLUDE_INTERACT_PARSER
#endif

#ifdef USE_XML_FORMAT_PARSER_INTERFACE
#define USE_MS2_INFO
#define USE_TINY_XML
#define USE_ALL_MATH_TOOLS
#define INCLUDE_XML_FORMAT_PARSER_INTERFACE
#endif


#ifdef USE_MS2_MS1_MATCHER
#define USE_LC_MS
#define USE_FEATURE
#define USE_ALL_MATH_TOOLS
#define INCLUDE_MS2_MS1_MATCHER
#endif

#ifdef USE_MS1_FEATURE_MERGER
#define USE_LC_MS
#define INCLUDE_MS1_FEATURE_MERGER
#endif

#ifdef USE_LC_MS
#define USE_MATCHES
#define USE_FEATURE
#define USE_MS2_FEATURE
#define USE_ALL_MATH_TOOLS
#define INCLUDE_LC_MS
#endif

#ifdef USE_FEATURE
#define USE_FEATURE_LC_PROFILE
#define USE_CONSENS_ISOTOPE_PATTERN
#define USE_MS2_INFO
#define USE_MS2_FEATURE
#define USE_READ_PARAM
#define USE_ALL_MATH_TOOLS
#define INCLUDE_FEATURE
#endif

#ifdef USE_FEATURE_LC_PROFILE
#define USE_READ_PARAM
#define USE_ALL_MATH_TOOLS
#define INCLUDE_FEATURE_LC_PROFILE
#endif

#ifdef USE_CONSENS_ISOTOPE_PATTERN
#define USE_READ_PARAM
#define USE_ALL_MATH_TOOLS
#define INCLUDE_CONSENS_ISOTOPE_PATTERN
#endif

#ifdef USE_MS2_FEATURE
#define USE_CLUSTERED_MS2_CONSENSUS_SPECTRUM
#define USE_MS2_CONSENSUS_SPECTRUM
#define USE_ALL_MATH_TOOLS
#define INCLUDE_MS2_FEATURE
#endif

#ifdef USE_CLUSTERED_MS2_CONSENSUS_SPECTRUM
#define USE_MS2_CONSENSUS_SPECTRUM
#define USE_DATA_PLOTTER
#define USE_ALL_MATH_TOOLS
#define INCLUDE_CLUSTERED_MS2_CONSENSUS_SPECTRUM
#endif

#ifdef USE_MS2_CONSENSUS_SPECTRUM
#define USE_MS2_FRAGMENT
#define USE_DATA_PLOTTER
#define USE_ALL_MATH_TOOLS
#define INCLUDE_MS2_CONSENSUS_SPECTRUM
#endif

#ifdef USE_MS2_PROTEIN_GROUP
#define USE_MS2_INFO
#define USE_ALL_MATH_TOOLS
#define INCLUDE_MS2_PROTEIN_GROUP
#endif

#ifdef USE_MS2_INFO
#define USE_FILE_SYS
#define USE_FILE_READER
#define USE_ALL_MATH_TOOLS
#define INCLUDE_MS2_INFO
#endif

#ifdef USE_MS2_FRAGMENT
#define USE_ALL_MATH_TOOLS
#define INCLUDE_MS2_FRAGMENT
#endif


#ifdef USE_LOWESS_REGRESSION_MANAGER
#define USE_LOWESS_C_FUNCTION
#define USE_ALL_MATH_TOOLS
#define INCLUDE_LOWESS_REGRESSION_MANAGER
#endif

#ifdef USE_PS2OTHER
#define USE_ALL_STANDART_CLASSES
#define USE_ALL_MATH_TOOLS
#define INCLUDE_PS2OTHER
#endif

#ifdef USE_EM_FITTER
#define USE_DATA_PLOTTER
#define USE_READ_PARAM
#define USE_ALL_MATH_TOOLS
#define INCLUDE_EM_FITTER
#endif

#ifdef USE_DATA_PLOTTER
#define USE_ALL_STANDART_CLASSES
#define USE_READ_PARAM
#define USE_ALL_MATH_TOOLS
#define INCLUDE_DATA_PLOTTER
#endif

#ifdef USE_READ_PARAM
#define USE_FILE_READER
#define USE_ALL_MATH_TOOLS
#define INCLUDE_READ_PARAM
#endif

#ifdef USE_FILE_READER
#define USE_FILE_SYS
#define USE_ALL_MATH_TOOLS
#define INCLUDE_FILE_READER
#endif

#ifdef USE_FILE_SYS
#define USE_ALL_MATH_TOOLS
#define INCLUDE_FILE_SYS
#endif

#ifdef USE_PROGRESS_BAR
#define USE_ALL_MATH_TOOLS
#define INCLUDE_PROGRESS_BAR
#endif

#ifdef USE_GENERAL_XML_PARSER
#define USE_TINY_XML
#define USE_ALL_MATH_TOOLS
#define INCLUDE_GENERAL_XML_PARSER
#endif

//////////////////////////////////////////////////
#ifdef USE_LOWESS_C_FUNCTION
#define INCLUDE_ALL_MATH_TOOLS
#define INCLUDE_LOWESS_C_FUNCTION
#endif

#ifdef USE_G2
#define INCLUDE_G2
#endif

#ifdef USE_TINY_XML
#define INCLUDE_TINY_XML
#endif

//////////////////////////////////////////////////


//////////////////////////////////////////////////
// reference to the general class library!!!!
#ifdef USE_ALL_MATH_TOOLS
#define INCLUDE_ALL_MATH_TOOLS
#endif

#ifdef INCLUDE_ALL_MATH_TOOLS
#define USE_ALL_MATH_TOOLS_CLASSES
#include "MATH_TOOLS_USE.h"
#endif
////////////////////////////////////////////////////


////////////////////////////////////////////////////
//include C-function:

#ifdef INCLUDE_TINY_XML
#include "tinyxml.h"
#endif

#ifdef INCLUDE_G2
extern "C"{
//#include <g2.h>
//#include <g2_PS.h>
}
#endif

#ifdef INCLUDE_LOWESS_C_FUNCTION
extern "C"{
#include <lowess.h>
}
#endif

////////////////////////////////////////////////////
//include own standart classes:

#ifdef INCLUDE_GENERAL_XML_PARSER
#include "GeneralXMLParser.h"
#endif

#ifdef INCLUDE_PROGRESS_BAR
#include "progress_bar.h"
#endif

#ifdef INCLUDE_FILE_READER
#include "file_reader.h"
#endif

#ifdef INCLUDE_FILE_SYS
#include "file_sys.h"
#endif

#ifdef INCLUDE_READ_PARAM
#include "read_param.h"
#endif

#ifdef INCLUDE_DATA_PLOTTER
#include "data_plotter.h"
#endif

#ifdef INCLUDE_EM_FITTER
#include "EM_fitter.h"
#endif

#ifdef INCLUDE_PS2OTHER
#include "ps2other.h"
#endif

#ifdef INCLUDE_MS2_FRAGMENT
#include "MS2Fragment.h"
#endif

#ifdef INCLUDE_MS2_INFO
#include "ms2_info.h"
#endif

#ifdef INCLUDE_MS2_PROTEIN_GROUP
#include "MS2ProteinGroup.h"
#endif

#ifdef INCLUDE_MS2_CONSENSUS_SPECTRUM
#include "MS2ConsensusSpectrum.h"
#endif

#ifdef INCLUDE_CLUSTERED_MS2_CONSENSUS_SPECTRUM
#include "ClusteredMS2ConsensusSpectrum.h"
#endif

#ifdef INCLUDE_MS2_FEATURE
#include "MS2_feature.h"
#endif

#ifdef INCLUDE_XML_FORMAT_PARSER_INTERFACE
#include "XMLFormatParserInterface.h"
#endif

#ifdef INCLUDE_INTERACT_PARSER
#include "interact_parser.h"
#endif

#ifdef INCLUDE_XQUEST_XML_PARSER
#include "XquestXMLParser.h"
#endif

#ifdef INCLUDE_GENERAL_MSMS_XML_PARSER
#include "GeneralMSMSXMLParser.h"
#endif

#ifdef INCLUDE_PROT_XML_PARSER
#include "ProtXMLParser.h"
#endif

#ifdef INCLUDE_CONSENS_ISOTOPE_PATTERN
#include "consensIsotopePattern.h"
#endif

#ifdef INCLUDE_FEATURE_LC_PROFILE
#include "featureLCprofile.h"
#endif

#ifdef INCLUDE_FEATURE
#include "feature.h"
#endif

#ifdef INCLUDE_LC_MS
#include "LC_MS.h"
#endif

#ifdef INCLUDE_MS1_FEATURE_MERGER
#include "MS1_feature_merger.h"
#endif

#ifdef INCLUDE_MS2_MS1_MATCHER
#include "MS2_MS1_matcher.h"
#endif

#ifdef INCLUDE_LC_MS_XML_WRITER
#include "LC_MS_XML_writer.h"
#endif

#ifdef INCLUDE_LC_MS_APML_WRITER
#include "LC_MS_APML_writer.h"
#endif

#ifdef INCLUDE_LC_MS_XML_READER
#include "LC_MS_XML_reader.h"
#endif

#ifdef INCLUDE_LC_MS_WRITER
#include "LC_MS_writer.h"
#endif

#ifdef INCLUDE_LOWESS_REGRESSION_MANAGER
#include "LowessRegressionManager.h"
#endif


#endif

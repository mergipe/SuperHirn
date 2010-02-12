#******************************************************************
# Makefile for the program SuperHirn 
# 
# contact: Lukas.Mueller@imsb.biol.ethz.ch

CC		= g++
C		= gcc 
CFLAGS		= -c -Wall -Wno-unknown-pragmas -Wno-format -O3 -DTIXML_USE_STL -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE 
#-DDEBUG
C_CODE_FLAGS	= -O3 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE 
#-DEBUG


#******************************************************************
# PROGRAM COMPILATION PATH:

# Set here the path where the executable is stored:
EXE_BIN			= ./

# Set the version flag for superhirn
SUPERHIRN_VERSION	= v03

# Set the name of the executable:
EXE_NAME		= $(EXE_BIN)SuperHirn$(SUPERHIRN_VERSION)

# Set the suffix of the test executable:
EXE_NAME_TEST		= $(EXE_NAME)_Test

# Set the path to the CPP_LIBRARY
ROOT_LIB		= ../../CPP_LIBRARY$(SUPERHIRN_VERSION)

# Set the path to the compiled objects
OBJ_DIR			= ../obj/


#******************************************************************
# EXTERNAL LIBRARIES TO LINK:
LIBS			= -lz


#******************************************************************
# MAIN LIBRARIES OF OWN CLASSES:

RAMP_LIB			= $(ROOT_LIB)/EXTERN/ramp
TINYXML_LIB			= $(ROOT_LIB)/EXTERN/tinyxml
GENERAL_LIB			= $(ROOT_LIB)/GENERAL
MATH_TOOLS_LIB			= $(GENERAL_LIB)/MATH_TOOLS
PEAK_DETECTION_LIB		= $(ROOT_LIB)/PEAK_DETECTION
MZXML_ACCESS_LIB		= $(PEAK_DETECTION_LIB)/MzXMLAccess
CONTROL_FTPEAK_LIB		= $(PEAK_DETECTION_LIB)/FT/control
MS_PROCESS_LIB			= $(PEAK_DETECTION_LIB)/FT/MSProcessing
MS1_FTPEAK_LIB			= $(PEAK_DETECTION_LIB)/FT/MS1Level
MS2_FTPEAK_LIB			= $(PEAK_DETECTION_LIB)/FT/MS2Level
LC_MS_TOOLS_LIB			= $(ROOT_LIB)/LC_MS_TOOLS
RATIOLIZER_LIB			= $(ROOT_LIB)/RATIOLIZER
STATISTIC_TOOLS_LIB		= $(ROOT_LIB)/STATISTIC_TOOLS
MS2_TRACE_QUANT_LIB		= $(STATISTIC_TOOLS_LIB)/MS2TraceQuantification
CLUSTER_LC_MS_ANALYSIS_LIB   	= $(ROOT_LIB)/CLUSTER_TOOLS/LC_MS_ANALYSIS

# Linker Flags:
LFLAGS				= -L$(TINYXML_LIB) -L$(RAMP_LIB) -L$(MATH_TOOLS_LIB) -L$(CONTROL_FTPEAK_LIB) -L$(MZXML_ACCESS_LIB) -L$(MS1_FTPEAK_LIB) -L$(MS_PROCESS_LIB) -L$(MS2_FTPEAK_LIB)\
				-L$(CLUSTER_LC_MS_ANALYSIS_LIB) -L$(GENERAL_LIB) -L$(LC_MS_TOOLS_LIB)\
				-L$(STATISTIC_TOOLS_LIB) -L$(MS2_TRACE_QUANT_LIB) -L$(RATIOLIZER_LIB) 

# Source paths:
VPATH				= ${SOURCE_DIR}
SOURCE_DIR      		= ../CC/\
				$(CONTROL_FTPEAK_LIB)/CC/\
				$(MZXML_ACCESS_LIB)/CC/\
				$(MS1_FTPEAK_LIB)/CC/\
				$(MS2_FTPEAK_LIB)/CC/\
				$(LC_MS_TOOLS_LIB)/CC/\
				$(MATH_TOOLS_LIB)/CC/\
				$(GENERAL_LIB)/CC/\
				$(CLUSTER_LC_MS_ANALYSIS_LIB)/CC\
				$(MS2_TRACE_QUANT_LIB)/CC\
				$(STATISTIC_TOOLS_LIB)/CC\
				$(RATIOLIZER_LIB)/CC\
				$(TINYXML_LIB)/CC\
				$(RAMP_LIB)/CC\
				$(MS_PROCESS_LIB)/CC


# include paths:
INCLUDE_DIR			= -I ../include/\
				-I $(CONTROL_FTPEAK_LIB)/include\
				-I $(MZXML_ACCESS_LIB)/include\
				-I $(MS1_FTPEAK_LIB)/include\
				-I $(MS2_FTPEAK_LIB)/include\
				-I $(LC_MS_TOOLS_LIB)/include\
				-I $(GENERAL_LIB)/include\
				-I $(MATH_TOOLS_LIB)/include\
				-I $(CLUSTER_LC_MS_ANALYSIS_LIB)/include\
				-I $(MS2_TRACE_QUANT_LIB)/include\
				-I $(STATISTIC_TOOLS_LIB)/include\
				-I $(RATIOLIZER_LIB)/include\
				-I $(TINYXML_LIB)/include\
				-I $(RAMP_LIB)/include\
				-I $(MS_PROCESS_LIB)/include


#******************************************************************
# listing of the C++ classes:

OBJECTS		= \
	\
	\
	simple_math progress_bar ps2other data_plotter\
	base64 ramp cramp\
	\
	\
	tinyxml tinyxmlparser tinyxmlerror tinystr\
	\
	\
	MS2_feature feature LC_MS ms2_info MS2ProteinGroup\
	MS2Fragment MS2FragmentTrace ClusteredMS2ConsensusSpectrum MS2ConsensusSpectrum\
	featureLCprofile consensIsotopePattern\
	\
	\
	FT_PeakDetectController MS2SpectrumProcessor\
	MS2_Process_Data Generic_MS_Scan\
	Process_Data LCMSCData ms_peak\
	BackgroundIntensityBin BackgroundControl\
	CentroidData CentroidPeak Deisotoper \
	LC_elution_peak IsotopicDist ExternalIsotopicDistribution PeptideIsotopeDistribution\
	FT_PEAK_DETEC_initializer\
	FT_PEAK_DETEC_mzXML_reader RawData\
	\
	\
	LCMSDataImporter LC_MS_XML_reader LC_MS_XML_writer\
	LC_MS_APML_writer LC_MS_writer file_reader file_sys\
	tree_reader interact_parser XMLFormatParserInterface XquestXMLParser GeneralMSMSXMLParser ProtXMLParser GeneralXMLParser\
	\
	\
	MS2_MS1_matcher LCMSmassCalibrator feature_updater\
	MASTER_LC_MS_intensity_normalizer normalization_coeff_computer\
	\
	\
	MS2SpectraCorrelator\
	MS2quantificationController AlignedMS2ConsensusSpectrum\
	\
	\
	protein_profile_evaluator\
	EM_fitter\
	MS1_feature_merger\
	\
	\
	AmrtpLCMSAligner AbstractLCMSAligner LC_MS_merger\
	FeatureMatchRetentionTimeRegressor MultipleFeatureMatchRemover\
	multi_merger match_region match_region_candidate\
	lowess LowessRegressionManager\
	LC_MS_correlation LC_MS_similarity_matrix SM_cell\
	\
	result_manager alignment_pair LC_MS_alignment_tree_parser\
	tree_builder tree_build_matrix cell node matrix_element\
	\
	\
	consens_profile_builder profiling_evaluator\
	statistic_box profile_cluster_tool KMEAN_main_cluster\
	cluster_X_scorer profile_comparer profile_scorer\
	peptide_DELTA_group DELTA_grouper profile_group\
	protein_group\
	\
	\
	ratiolizer_manager protein_ratiolizer MS1_feature_ratiolizer peptide_ratio_analyzer\
	\
	\
	read_param SequentialTreeCreator param_initializer protein_profile_evaluator\
	SuperHirn_manager paramManager main


#******************************************************************
# TARGETS

RM	= rm -rf
MKDIR	= mkdir -p



all:make_dir $(EXE_NAME)

test: $(EXE_NAME_TEST)

dir:	
	$(MKDIR) $(OBJ_DIR)

clean :
	$(RM) $(EXE_NAME) 
	$(RM) $(OBJ_DIR)
	$(MKDIR) $(OBJ_DIR)

cleanTest :
	$(RM) $(EXE_NAME_Test) 
	$(RM) $(OBJ_DIR)
	$(MKDIR) $(OBJ_DIR)


cleano :
	$(RM) $(OBJ_DIR)
	$(MKDIR) $(OBJ_DIR)


#******************************************************************
# MAKE THE FOLDER FOR THE OBJECTS:

make_dir: $(OBJ_DIR)

$(OBJ_DIR):
	$(MKDIR) $@



#******************************************************************
#  build the EXECUTABLE by linking the classes

$(EXE_NAME): $(foreach o,$(OBJECTS),$(OBJ_DIR)$(o).o) 
	$(CC) $(LIBS) -g -o $(EXE_NAME) $^ $(LFLAGS) 

#******************************************************************
#  build the testing EXECUTABLE by linking the classes

$(EXE_NAME_TEST): $(foreach o,$(OBJECTS),$(OBJ_DIR)$(o).o) 
	$(CC) -g -o $(EXE_NAME_TEST) $^ $(LFLAGS) $(LIBS)


#******************************************************************
#  build C and C++ classes

$(OBJ_DIR)%.o: %.cc
	$(CC) $(INCLUDE_DIR) $(CFLAGS) $(MACROS) -o $@ -c $<  $(LFLAGS) $(LIBS)

$(OBJ_DIR)%.o: %.cpp
	$(CC) $(INCLUDE_DIR) $(CFLAGS) $(MACROS) -o $@ -c $<  $(LFLAGS) $(LIBS)

$(OBJ_DIR)%.o: %.c
	$(C) $(INCLUDE_DIR) $(C_CODE_FLAGS) $(MACROS) -o $@ -c $<  $(LFLAGS) $(LIBS)

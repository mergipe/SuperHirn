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


/////////////////////////////////////////////////////////////
// lc-ms objects
#define USE_SUPERHIRN_MANAGER
#define USE_PARAM_INITIALIZER
#include "use.h"


//--------------------------------------------------------------------------
// declare prototypes:
void print_help();
void PARAMETER_HELP_FUNC( string );
void DEFINE_ANALYSIS_OUTPUT();
int check_run_modus( string );
void start();
void end();
void program_exit();
void storeSuperHirnRuningParameters();
  


struct FuncCalSequence{
  bool ArgumentModeON;
  int LastArgumentMode;
  map<int, bool> SuperHirnProcessesIDs;
  map<int, string> SuperHirnProcessArguments;
};
// create a global instance:
FuncCalSequence my_RUN;

//-------------------------------------------------------------------------
// function which prints information about the superhirn parameters parameters
void PARAMETER_HELP_FUNC(string TAG ){
  read_param def;
  def.search_param_help( TAG );
}

//-------------------------------------------------------------------------
// function which prepares the ANALYSIS directory
void DEFINE_ANALYSIS_OUTPUT(){

  //////////////////////////////////
  // read parameters from "param.def"
  // and create the required data output folders:
  read_param* def = new read_param();
  string IN = def->search_tag("MY PROJECT NAME");
  bool GNUPLOT;
  def->search_tag("gnuplot plot generator", &GNUPLOT);
  delete def;
  def = NULL;
  //////////////////////////////////

  // add backslash at end:
  if(IN[IN.size()-1] != '/')
    IN += "/";
  string OUT = "ANALYSIS_";
  OUT += IN;

  string gnu_dir = OUT;
  gnu_dir += "gnu_plots";      
  
  // create a new one:
  int check =  mkdir(OUT.c_str(),0777);
  // erase content of the directory:
  if(check != 0){
    check = mkdir(OUT.c_str(),0777);
    if( GNUPLOT ){
      check = mkdir(gnu_dir.c_str(),0777);
    }
  }
  else if(check == 0){
    // add also a directory for the statistic output of the 
    // LC-MS alignment 
    if( GNUPLOT ){
      check = mkdir(gnu_dir.c_str(),0777);
    }
  }
  else{
    printf("\n\tCould not create the output directory '%s'!",OUT.c_str());
  }
}



//--------------------------------------------------------------------------
// welcome text:
void start(){

  cout<<endl<<"\t********************************************************************";
  cout<<endl<<"\t*                                                                  *";
  cout<<endl<<"\t*       S  |  U  |  P  |  E  |  R |  H  |  I  |  R  |  N           *";
  cout<<endl<<"\t*                                                                  *";
  cout<<endl<<"\t*     SuperHirn Release 0.3, 31/01/09                              *";
  cout<<endl<<"\t*     Software by the Group of Prof. R. Aebersold                  *";
  cout<<endl<<"\t*     Homepage: http://tools.proteomecenter.org                    *";
  cout<<endl<<"\t*     Forum:    http://groups.google.com/group/superhirn           *";
  cout<<endl<<"\t*     Institute of Molecular Systems Biology (IMSB)                *";
  cout<<endl<<"\t*     Swiss Federal Institute of Technology (ETH), Zurich          *";
  cout<<endl<<"\t*                                                                  *";
  cout<<endl<<"\t********************************************************************"<<endl<<endl;
  
}

//--------------------------------------------------------------------------
// end text:
void end(){

  cout<<endl<<endl;
  cout<<endl<<"\t********************************************************************";
  cout<<endl<<"\t*                                                                  *";
  cout<<endl<<"\t*      S U P E R H I R N   says thanks and goodbye                 *";
  cout<<endl<<"\t*                                                                  *";
  cout<<endl<<"\t********************************************************************";
  cout<<endl<<endl;
}


//--------------------------------------------------------------------------
// this function goes through all input parameters and checks if they are
// correct and maps them then into a sequence of function calls:
void defineRunSequence( char* ARGUMENTS[] , int NB_args){
  
  if( NB_args == 1 ){
    print_help();
  }
    
  // cycle through the list of input parameters and 
  // map them to function ids
  for( int S = 1; S < NB_args; S++ ){
    string mode = ARGUMENTS[S];
    int test = check_run_modus( mode );
    if( test == -1 ){
      print_help();
    }
    else if( test != 0 ) {
      my_RUN.SuperHirnProcessesIDs.insert( make_pair( test, true) );
    }
  }  
  
  if( my_RUN.SuperHirnProcessesIDs.empty() ){
    print_help();
  }
  
}


//--------------------------------------------------------------------------
// check the run modus parameter:
int check_run_modus(string run_status){
  
  // check for help function:
  if( (run_status == "-help") || (run_status == "help") || (run_status == "-h")){
    print_help();
    return -1;
  }
    
  // create the guide tree, LC-MS similarity analysis:
  if( ( run_status == "-FEATURE_EXTRACTION" ) || ( run_status == "-FE" ) ){
    my_RUN.LastArgumentMode = 1;
    my_RUN.ArgumentModeON = true;
    return 1;
  }
 
  // create the guide tree, LC-MS similarity analysis:
  if( ( run_status == "-BUILD_GUIDE_TREE" ) || ( run_status == "-BT" ) ){
    return 2;
  }
  
  // create the mastermap by multiple LC-MS alignment
  if( ( run_status == "-CREATE_MASTER" ) || ( run_status == "-CM" ) ){
    my_RUN.LastArgumentMode = 3;
    my_RUN.ArgumentModeON = true;
    return 3;
  }
    
  // inclusion list annotation
  if( ( run_status == "-DO_INCLUSION_LIST_ANNOTATION" )  || ( run_status == "-ILA" ) ){
    my_RUN.LastArgumentMode = 4;
    my_RUN.ArgumentModeON = true;
    return 4;
  }  
  // intenisty normalizataion of the master map
  if( ( run_status == "-DO_INTENSITY_NORMALIZATION" ) || ( run_status == "-IN" ) ){
    return 5;
  }
  // convert a LC-MS run from XML to apml
  if( ( run_status == "-CONVERT_TO_APML" )  || ( run_status == "-C2A" ) ){
    my_RUN.LastArgumentMode = 6;
    my_RUN.ArgumentModeON = true;
  return 6;
  }  
  
  // create tab deliminated file from mastermap
  if( ( run_status == "-MASTER2FILE" ) || ( run_status == "-M2F" ) ){
    my_RUN.LastArgumentMode = 7;
    my_RUN.ArgumentModeON = true;
    return 7;
  }
  
  // detected delta Mr pairs
  if( ( run_status == "-SEARCH_DELTA_SHIFTS" ) || ( run_status == "-SDS" ) ){
    return 8;
  }
  
  // MS1 feature profiling and peptide/protein profile evalutation
  if( ( run_status == "-DO_PROFILING" ) || ( run_status == "-DP" ) ){
    return 9;
  }
  if( ( run_status == "-EM_PROFILING_EVALUATION" ) || ( run_status == "-EME" ) ){
    return 10;
  }    

  // ratiolizer: 
  if( ( run_status == "-START_RATIOLIZER" ) || ( run_status == "-SR" ) ){
    return 11;
  }
    
  // ms2 trace analysis: 
  if( ( run_status == "-MS2_TRACE_ANALYSIS" ) || ( run_status == "-MTA" ) ){
    return 12;
  }

  // create the SuperMap from existing MasterMaps: 
  if( ( run_status == "-CREATE_SUPER_MAP" ) || ( run_status == "-CSM" ) ){
    return 13;
  }
  
  
  /////////////////////////////////////////////
  // here follow the help functions which are
  // tagged with negative ids:
  
  // search root param file for parameters
  if( ( run_status == "-SEARCH_SUPERHIRN_PARAMETER" ) || ( run_status == "-SSP" ) ){
    my_RUN.LastArgumentMode = -1;
    my_RUN.ArgumentModeON = true;
    return -1;
  }
  
  // convert a parameter xml file into a param.def
  if( ( run_status == "-RESTORE_PARAM" ) || ( run_status == "-RE_PARAM" ) ){
    my_RUN.LastArgumentMode = -2;
    my_RUN.ArgumentModeON = true;
    return -2;
  }

  // specify the custom parameter file to use: 
  if( ( run_status == "-PARAMETER_FILE" ) || ( run_status == "-PARAM" ) ){
    my_RUN.LastArgumentMode = -10;
    my_RUN.ArgumentModeON = true;
    return -10;
  }  
  
  
  // this is the argument mode, the string here should be an argument
  // of the previous superhirn mode:
  if( ( my_RUN.ArgumentModeON ) && ( my_RUN.LastArgumentMode != -1) ){
 
    // get the corresponding mode:
    int mode = my_RUN.LastArgumentMode;
    
    // insert the argument:
    my_RUN.SuperHirnProcessArguments.insert( make_pair(mode, run_status) );
    my_RUN.LastArgumentMode = -1;
    my_RUN.ArgumentModeON = false;
    return 0;
  }

  printf("\n\t **** SUPERHIRN RUN MODE '%s' NOT RECOGNIZED AS VALID ****\n \t [ Please check help menu ]\n", run_status.c_str() ); 
  print_help();
  return -1;
}



//--------------------------------------------------------------------------
// help text:
void print_help(){ 
  
  cout<<endl<<endl;
  
  printf("\tS U P E R H I R N   is run as following:\n");
  printf("\tSuperHirn -<RUN MODUS> <OPTIONAL PARAMETERS>\n");
  printf("\twith RUN MODUS...\n");
  printf("\t\tA.\t-FEATURE_EXTRACTION(FE) (<MzXML PATH>):\t\tdo peak extraction\n");
  printf("\t\tB.\t-BUILD_GUIDE_TREE(BT):\t\t\t\tdo peak extraction and create alignment topology\n");
  printf("\t\tC.\t-CREATE_MASTER(CM) (<PepXML PATH>):\t\tcreate a MasterMap by multile LC-MS alignment\n");
  printf("\t\tD1.\t-MASTER2TABFILE(M2F) <FOLDER>:\t\t\tconvert MasterMap to a tab-file\n");
  printf("\t\tD2.\t-DO_PROFILING(DP):\t\t\t\tdo profile cluster analysis on MasterMap\n");
  printf("\t\tD3.\t-EM_PROFILING_EVALUATION(EME):\t\t\tevaluation of profiling analysis\n");
  printf("\t\tD4.\t-START_RATIOLIZER(SR):\t\t\t\tdo MS1 feature ratio analysis\n");
  printf("\t\tD5.\t-DO_INTENSITY_NORMALIZATION(IN):\t\tMS1 feature intensity normalization\n");
  printf("\t\tD6.\t-MAP_ANNOTATION(ILA) (<ILA PepXML Folder>):\tInclusion list annotation of MasterMap\n");
  printf("\t\tD7.\t-CONVERT_TO_APML(C2A) <FOLDER>:\t\t\tconvert XML LC-MS runs in <FOLDER> to APML\n");
  printf("\t\tD8.\t-RESTORE_PARAM(RE_PARAM) <FILE>:\t\trestore 'param_RESTORE.def' from a XML parameter file\n");
  printf("\t\tH1.\t-SEARCH_SUPERHIRN_PARAMETER(SSP) <STRING>:\tstring search in ROOT_PARAM.def of parameters \n");
 
  // special options:
  printf("\n\t\tOther SuperHirn argument options:\n");
  printf("\t\t\t 1* add -PARAM <File> to specify another parameter file than 'param.def'\n");
  printf("\t\t\t (example: SuperHirn -BT -CM -PARAM myOldParam.def)\n");
  
  
  cout<<endl<<endl;
  exit(0);
}

//--------------------------------------------------------------------------
// program exit function:
void program_exit(){
  end();
  exit(0);
}


//--------------------------------------------------------------------------
// storage of running parameters:
void storeSuperHirnRuningParameters(){
  paramManager pManager;
  pManager.extractAndStoreParameters();
}


//--------------------------------------------------------------------------
// main program which is called by running the program:
int main( int argc, char* argv[]){
  
  //-----------------------------------------------------------------------
  // start print function
  start();
  
  //-----------------------------------------------------------------------
  // check input arguments:
  defineRunSequence( argv , argc);
  
  //-------------------------------------------------------------------------
  // check if user specified path for the param.def file:
  map<int, bool>::iterator P = my_RUN.SuperHirnProcessesIDs.find( -10 );
  if( P != my_RUN.SuperHirnProcessesIDs.end() ){
    map<int, string>::iterator s = my_RUN.SuperHirnProcessArguments.find( -10 );
    param_initializer::DEFINE_COSTUM_PARAM ( (*s).second );
    // remove the process:
    my_RUN.SuperHirnProcessesIDs.erase( -10 );
    my_RUN.SuperHirnProcessArguments.erase( -10 );
  }
  
  
  //--------------------------------------------------------------------------
  //
  // LOOP HERE THROUGH THE FUNCTION IDS AND CALL THE SUPERHIRN ROUTINES:
  //
  SuperHirn_manager* manager = new SuperHirn_manager();
  
  map<int, string>::iterator F;
  map<int, bool>::iterator PROCESS = my_RUN.SuperHirnProcessesIDs.begin();
  while( PROCESS != my_RUN.SuperHirnProcessesIDs.end() ){
    
    if( (*PROCESS).second ){
      
      switch( (*PROCESS).first ){
          
        case 1:
          
          //--------------------------------------------------------------------------
          // Start Peak Extraction:
          printf("\n\n\t*** START MS1 FEATURE EXTRACTION ... ***\n\n");
          DEFINE_ANALYSIS_OUTPUT();
          
          //--------------------------------------------------------------------------
          // check here if any parameters are supplied specifying the data input folders/filers:
          F = my_RUN.SuperHirnProcessArguments.find( (*PROCESS).first );
          if( F != my_RUN.SuperHirnProcessArguments.end() ){
            manager->start_peak_extraction( (*F).second );
          }
          else{
            manager->start_peak_extraction();
          }
          break;
          
          case 2:
          
          //--------------------------------------------------------------------------
          // start the pairwise alignmentand correlation process
          printf("\n\n\t*** START LC/MS CORRELATION ... ***\n\n");
          manager->start_PAIRWISE_ALIGNMENT();
          
          
          //--------------------------------------------------------------------------
          // start the alignment process
          printf("\n\n\t*** BUILD GUIDE TREE ... ***");
          manager->start_build_guide_tree();
          break;
      
          case 3:
          
          //--------------------------------------------------------------------------
          // start the multiple alignment guided by the alignment tree:
          printf("\n\t*** START THE MULTIPLE LC-MS ALIGNMENT ... ***\n\n");
          
          //--------------------------------------------------------------------------
          // check here if any parameters are supplied specifying the data input folders/filers:
          F = my_RUN.SuperHirnProcessArguments.find( (*PROCESS).first );
          if( F != my_RUN.SuperHirnProcessArguments.end() ){
            manager->start_creating_MASTER_MAP((*F).second);
          }
          else{
            manager->start_creating_MASTER_MAP();
          }
          break;
          
          
          case 4:
          //--------------------------------------------------------------------------
          // update the mastermap by inlcuions list ms2 scans
          printf("\n\t*** INCLUSION LIST ANNOTATION OF MASTERMAP... ***\n\n");  
          
          //--------------------------------------------------------------------------
          // check here if any parameters are supplied specifying the data input folders/filers:
          F = my_RUN.SuperHirnProcessArguments.find( (*PROCESS).first );
          if( F != my_RUN.SuperHirnProcessArguments.end() ){
            manager->start_MASTERMAP_INCLUSION_LIST_UPDATING( (*F).second );
          }
          else{
            manager->start_MASTERMAP_INCLUSION_LIST_UPDATING( );
          }
          break; 
          
          
          case 5:
          //--------------------------------------------------------------------------
          // do intensity normalization on the MasterMap
          printf("\n\t*** DO INTENSITY NORMALIZATION ON MASTERMAP... ***\n\n");    
          manager->start_INTENSITY_NORMALIZATION();
          break;
          
          
          case 6:
          //--------------------------------------------------------------------------
          // converts LC-MS runs in a imput folder to APML
          // get the run argument:
          F = my_RUN.SuperHirnProcessArguments.find( (*PROCESS).first );
          if( F != my_RUN.SuperHirnProcessArguments.end() ){
            printf("\n\t*** CONVERT XML files in '%s' to APML... ***\n\n", (*F).second.c_str());  
            manager->convert_XML_to_AMPL( (*F).second );
          }
          break;
          
          
          case 7:
          //--------------------------------------------------------------------------
          // start the multiple alignment guided by the alignment tree:
          printf("\n\t*** CONVERT MASTERMAP / LC-MS RUN  TO TAB FILE... ***\n\n");
          // get the run argument:
          F = my_RUN.SuperHirnProcessArguments.find( (*PROCESS).first );
          if( F != my_RUN.SuperHirnProcessArguments.end() ){
            manager->convert_Master_2_excel( (*F).second );
          }
          break;
          
          
          case 8:
          //--------------------------------------------------------------------------
          // search for delta shifts over the matched MASTER run:
          printf("\n\t*** START DELTA SEARCH ***\n\n");    
          manager->start_DELTA_SEARCH_MASTER_analysis();
          break;  
          
          
          case 9:
          //--------------------------------------------------------------------------
          // start profiling analysis of MASTER run:
          printf("\n\t*** START THE MASTER PROFILING ANALYSIS PROCESS... ***\n\n");    
          manager->start_PROFILING_MASTER_analysis();
          break;
          
          
          case 10:
          //--------------------------------------------------------------------------
          // start profiling analysis of MASTER run:
          printf("\n\t*** START THE PROFILING EVALUATION... ***\n\n");    
          manager->start_PROFILING_EVALUATION();
          break;
          
          
          case 11:
          //--------------------------------------------------------------------------
          // do analyis of feature ratios in the MASTER run:
          printf("\n\t*** START RATIOLIZER ANALYSIS IN MASTERMAP... ***\n\n");    
          manager->start_RATIOLIZER_analysis();
          break;
          
          
          case 12:
          //--------------------------------------------------------------------------
          // do a ms2 trace fragment analysis 
          printf("\n\t*** START MS2 FRAGMENT TRACE ANALYSIS IN MASTERMAP... ***\n\n");    
          manager->start_MS2_TRACE_FRAGMENT_analysis();
          break;
          
          
          case 13:
          //--------------------------------------------------------------------------
          // do a ms2 trace fragment analysis 
          printf("\n\t*** START CONSTRUCTION OF THE SUPERMAP... ***\n\n");    
          manager->start_creating_SUPER_MAP();
          break;
          
          
          case -1:
          //-------------------------------------------------------------------------
          // search root param for superhirn parameters:
          F = my_RUN.SuperHirnProcessArguments.find( (*PROCESS).first );
          if( F != my_RUN.SuperHirnProcessArguments.end() ){
            PARAMETER_HELP_FUNC( (*F).second );
          }
          break;
          
          case -2:
          //--------------------------------------------------------------------------
          // restore the param.def from a XML parameter file:
          // get the run argument:
          F = my_RUN.SuperHirnProcessArguments.find( (*PROCESS).first );
          if( F != my_RUN.SuperHirnProcessArguments.end() ){
            printf("\n\t*** RESTORING A PRAMETER FILE FROM XML '%s... ***\n", (*F).second.c_str());    
            paramManager pMan;
            pMan.restoreParamFile( (*F).second );
          }
          break;
          
          
          default:
          print_help();
      }
    }
    
    // next process:
    PROCESS++; 
  }
    
  //--------------------------------------------------------------------------
  // delete variables
  if(manager != NULL){
    delete manager;
    manager = NULL;
  }


  //--------------------------------------------------------------------------
  // storage of running parameters:
  storeSuperHirnRuningParameters();  
  
  // print end text
  program_exit();  
}

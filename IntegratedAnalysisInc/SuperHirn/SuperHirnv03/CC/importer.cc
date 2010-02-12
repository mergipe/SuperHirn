///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS Importer:
// management of the input LC-MS spectra
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#define USE_IMPORTER
#include "use.h"	

		
	
///////////////////////////////////////////////
// constructor for the object ana_summarizer:
importer::importer(){
	
  reader = NULL;
  LC_MS_UPDATE = NULL;
  
  // counter which sets the spectrum ids
  id_counter = 0;
  feature_count = 0;
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  
  SOURCE_DIR = def->search_tag("MY PROJECT NAME");
  if(SOURCE_DIR[SOURCE_DIR.size() - 1]	!= '/')
    SOURCE_DIR +=		"/";	
  
  ////////////	
  // define the retention time window:
  
  // start of the window
  def->search_tag("start elution window", &elution_window_start);
  
  // end of the window
  def->search_tag("end elution window", &elution_window_end);
  
  // MS2 mass accuracy:
  def->search_tag("MS2 m/z tolerance", &MZ2_tolerance);
  
  delete def;
  def = NULL;
  
  MASTER = NULL;
}

//////////////////////////////////////////////////
// class desctructor
importer::~importer(){
  
  if(!lc_ms_spectra.empty())
    lc_ms_spectra.clear();
  
  if(reader != NULL){
    delete reader;
    reader = NULL;
  } 

  MASTER = NULL;
  LC_MS_UPDATE = NULL;
}


//***********************************************************************************
// here read input from ".XML" files:


////////////////////////////////////////////////
// read the lc/ms output files from LS-MS-XML
void importer::read_XML_files(string IN_target){
  
  target = IN_target;
  
  // create a new LC_MS reader:
  LC_MS_XML_reader* READ = new LC_MS_XML_reader();
  
  // now read all features:
  READ->start_LC_MS_parsing( target );

  // get the parsed run:
  vector<LC_MS>::iterator P = READ->get_parsed_LCMS_list_start();
  while( P != READ->get_parsed_LCMS_list_end() ){

    LC_MS* TMP = &(*P);
    
    if( TMP != NULL ){
    
      process_validated_LCMS( TMP );
      // add the LC/MS run to the vector
      lc_ms_spectra.push_back( *TMP );
      TMP = NULL;
    }
    P++;
  }
}



//////////////////////////////////////////////////////////////////////////////////////////////
// add MS2 info to the LC/MS run:
void importer::add_MS2_info_to_run(ms2_info* IN){
  
  feature* SEARCH = NULL;
  bool info_check = false;

  // compare now the m/z
  double MZ = IN->get_MONO_MZ();
  double MAX_MZ = MZ + MZ2_tolerance;
  int CHRG = IN->get_CHRG();
  
  // search now the original peak:
  vector<feature>::iterator P = LC_MS_UPDATE->get_feature_list_begin();
  while( P != LC_MS_UPDATE->get_feature_list_end()){
    
    SEARCH = &(*P);
    
    if( SEARCH->get_MZ() > MAX_MZ )
      break;
    
    // check if its in the lc elution range:
    if( ( IN->get_SCAN_START() >= SEARCH->get_scan_start() ) && ( IN->get_SCAN_END() <= SEARCH->get_scan_end() ) ){
      
      // check with M/Z:
      double delta_MZ = fabs(MZ - SEARCH->get_MZ());

      printf("%f-%f:%f\n",MZ,SEARCH->get_MZ(), delta_MZ);
      
      if( delta_MZ <= MZ2_tolerance){
        
        // charge states:
        if( CHRG == SEARCH->get_charge_state()){
          info_check = true;
          SEARCH->add_MS2_info(IN);
        }
      }
    }
    P++;
  }
  
  // print if MS2 info could not be added to any peak:
  if(!info_check){
    printf("* MS2INFO(%d,%f[+%d][%s|%s|%f])could not be associated with a MS1 raw peak*\n", IN->get_SCAN_START(), MZ, IN->get_CHRG(),IN->get_SQ().c_str(),IN->get_AC().c_str(),IN->get_PEP_PROB());
  }  
}


//############################################################################################
//
// FUNCTIONS TO read the MASTER LC/MS run from a text file!!!!
//

//////////////////////////////////////////////////
// reads a MASTER LC/MS from a file:
void importer::read_MASTER_from_file(string IN){
  
  /////////////////////////////////
  // create a new LC_MS reader:
  LC_MS_XML_reader* READ = new LC_MS_XML_reader( );
  
  // now read all features:
  READ->start_LC_MS_parsing( IN );
  MASTER = &(*READ->get_parsed_LCMS_list_start());
  
}



//////////////////////////////////////////////////////////////////////
// process the input run:
void importer::process_validated_LCMS( LC_MS* IN ){
  // set ID:
  IN->set_spectrum_ID(lc_ms_spectra.size());
  // set the correct LCMS id if teh run is not a mastermap:
  if( IN->check_raw_spec_name_empty() ){
    IN->setFeatureLCMSID();
  }
  
}

///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS LC_MS_writer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_LC_MS_WRITER
#include "GENERAL_USE.h"


////////////////////////////////////////////////
// constructor for the object LC_MS_writer:
LC_MS_writer::LC_MS_writer(LC_MS* iRun){
  LC_MS_MAP = iRun;
  namePrefix = "";
}

////////////////////////////////////////////////
// constructor for the object LC_MS_writer:
LC_MS_writer::LC_MS_writer( string IN ){
  namePrefix = "";
}

//////////////////////////////////////////////////
// class desctructor of LC_MS_writer
LC_MS_writer::~LC_MS_writer( ){
  LC_MS_MAP = NULL;
}


//////////////////////////////////////////////////////////
// outputs a MasterMap into XML formatted file with this fileName:
void LC_MS_writer::storeMasterMapToXML( string iPrefix ){
  namePrefix = iPrefix;
  storeMasterMapToXML( );
}


//////////////////////////////////////////////////////////
// outputs a MasterMap into XML formatted file with this fileName:
void LC_MS_writer::storeMasterMapToXML( ){
  
  // read parameters from "param.def"
  target_dir = "ANALYSIS_";
  read_param* def = new read_param();  
  target_dir += def->search_tag("MY PROJECT NAME");  
  delete def;
  def = NULL;    
  if( target_dir[ target_dir.size() - 1 ] != '/'){
    target_dir += "/";
  }

  if( LC_MS_XML_reader::getXMLFileEnding() == ".xml" ){
    storeMasterMapToSuperMuellerLNXML( );
  }
  else if( LC_MS_XML_reader::getXMLFileEnding() == ".apml" ){
    storeMasterMapToAPML( );
  }
  
}



//////////////////////////////////////////////////////////
// outputs into Mueller Superhirn LN XML formatted file
void LC_MS_writer::storeMasterMapToSuperMuellerLNXML( ){
  
  // set the full path:
  out_file = namePrefix + LC_MS_MAP->get_spec_name();
  out_file = target_dir + out_file;
  out_file = processOutName( out_file );
  out_file += LC_MS_XML_reader::getXMLFileEnding();
  
  // write LC/MS stuff:
  // write first the LC/MS MASTER run:
  LC_MS_XML_writer* LC_W = new LC_MS_XML_writer( out_file );
  LC_W->write_XML_MAIN_HEADER();
  LC_W->write_LC_MS_run_2_XML_tag( LC_MS_MAP );
  
  // write the individual features to file:
  vector< LC_MS_FEATURE >::iterator FEA = LC_MS_MAP->get_feature_list_begin();    
  while( FEA != LC_MS_MAP->get_feature_list_end() ){
    LC_W->write_feature_2_XML_tag( &(*FEA), true );
    FEA++;
  }
  LC_W->write_LC_MS_run_close_tag();
  LC_W->write_XML_MAIN_HEADER_CLOSE();
  
  delete LC_W;
  LC_W = NULL;
  
}

////////////////////////////////////////////////////////////////////////
// removes which spaces in the string:
string LC_MS_writer::processOutName( string in ){
  
  string tmp = " ";
  string repl = "";
  while( in.find( tmp ) != string::npos ){
    in.replace( in.find( tmp ), tmp.size(), repl);
  }
  
  return in;
}




////////////////////////////////////////////////////////////////////////
// outputs into Brusniak APML XML formatted file
void LC_MS_writer::storeMasterMapToAPML( ){
  
  // create a new LC_MS reader:
  LC_MS_APML_writer* LC_W = new LC_MS_APML_writer( target_dir );  
  
  // check if its a mastermap:
  if( LC_MS_MAP->check_raw_spec_name_empty() ){
    LC_W->WRITE_COMPLETE_LC_MS_2_APML( LC_MS_MAP->get_spec_name(), LC_MS_MAP );
  }
  else{
    LC_W->WRITE_MASTER_LC_MS_2_APML( LC_MS_MAP->get_spec_name(), LC_MS_MAP );
  }

  delete LC_W;
  LC_W = NULL;  
  
}


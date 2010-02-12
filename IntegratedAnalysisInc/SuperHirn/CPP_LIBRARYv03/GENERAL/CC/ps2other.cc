///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS ps2other:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_STRING
#define USE_IOSTREAM
#define USE_STD_DEF
#define USE_SYS_TYPES
#define USE_UNISTD
#define USE_NAMESPACE
#define USE_PS2OTHER
#include "GENERAL_USE.h"


string ps2other::GS_CMD = "gs -q -dNOPROMPT -dBATCH -sDEVICE=";
string ps2other::GS_JPEG_PACK = "jpeg";
string ps2other::GS_TIFF_PACK = "tiffpack";
string ps2other::EXT_JPG = ".jpg";
string ps2other::EXT_TIF = ".tiff";
string ps2other::RESOLUTION = "400";


////////////////////////////////////////////////
// constructor for the object ps2other:
ps2other::ps2other(){
}

//////////////////////////////////////////////////
// class desctructor of ps2other
ps2other::~ps2other(){
}

//////////////////////////////////////////////////
// class copy constructor of ps2other
ps2other::ps2other(const ps2other& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of ps2other
ps2other::ps2other(const ps2other* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
ps2other& ps2other::operator=(const ps2other& tmp){
  return *this;
}
    

/////////////////////////////////////////////////
// convert to tif file:
void ps2other::convert2tif(){
  
  string TMP = create_output_string(ps_file, EXT_TIF);
  cout<<TMP<<endl;
  printf("\t***PLEASE PRESS RETURN***\n");
  execute_command(TMP);
}

/////////////////////////////////////////////////
// convert to jpeg file:
void ps2other::convert2jpeg(){
  
  string TMP = create_output_string(ps_file, EXT_JPG);
  cout<<TMP<<endl;
  printf("\t***PLEASE PRESS RETURN***\n");
  execute_command(TMP);
  
}

/////////////////////////////////////////////////
// write commans:
void ps2other::execute_command(string MAIN_CMD){

  if( system(MAIN_CMD.c_str()) ){
    printf("\n** ERROR:: Command '%s' could not be executed!!! ps2other::87***\n",MAIN_CMD.c_str() );
  }
      
  rm_ps_file();
}

/////////////////////////////////////////////////
// deletes the ps file
void ps2other::rm_ps_file(){
  remove(ps_file.c_str());
}



//////////////////////////////////////////////////
// create out path:
string ps2other::create_output_string(string IN, string ext){
  
  string outfile = IN;
  // remove the .ps
  outfile.erase(IN.find(".ps"), IN.size() - IN.find(".ps"));
  // add the extenstion:
  outfile += ext;
  
  // build the command:
  string OUT = GS_CMD;
  
  if( ext == ".jpg" ){
    OUT += GS_JPEG_PACK; 
  }
  
  if( ext == ".tiff" ){
    OUT += GS_TIFF_PACK; 
  }
  
  // add the resolution:
  OUT += " -r";
  OUT += RESOLUTION;
  
  // add the output:
  OUT += " -sOutputFile=" + outfile;
  
  // add infile:
  OUT += " " + IN;
  
  return OUT;
    
}


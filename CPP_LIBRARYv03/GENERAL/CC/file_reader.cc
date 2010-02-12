///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS file reader:
// provides function to open /reade / modify and close text files
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#define USE_FILE_SYS
#define USE_FILE_READER

#include "GENERAL_USE.h"

////////////////////////////////////////////////
// constructor for the object ana_summarizer:
file_reader::file_reader(){
 
  // initialize the variables
  FIN = NULL;
  max_nb_character_per_line = 5000;
  line = NULL;
  COMMENT = true;
}

//////////////////////////////////////////////////
// class desctructor

file_reader::~file_reader(){
 
 if(line != NULL){
  delete line;
  line = NULL;
 }
 
 if(FIN != NULL){
  FIN->close();
  
  delete FIN;
  FIN = NULL;
 }
  
}



///////////////////////////////////////////////////
// open a file:
bool file_reader::open_file(){
     
  close_file();
  
  if(FIN != NULL){
    delete FIN;
    FIN = NULL;
  }
  
  FIN = new ifstream();   
  
  FIN->open(current_file.c_str());
  
  if(FIN->good()){
    
    // set space for the read line
    if(line !=NULL){
      delete line;
      line = NULL;
    }
    line = new char[max_nb_character_per_line+1];
    line[max_nb_character_per_line] = '\0';
    
    return true;
  }
  else{
    if( COMMENT ){
      printf("\n\tERROR reading file '%s' in file_reader::open_file().\n", current_file.c_str());
    }
    return false;
  }
}



///////////////////////////////////////////////////
// jumpto start of a file:
void file_reader::jump_to_file_start(){
 if( FIN->good() ){
  FIN->seekg(0, ios::beg);
 }
}

///////////////////////////////////////////////////
// jump n positions back in the file 
void file_reader::move_cursor_back(int IN){
  if( FIN->good() ){
    int this_pos = FIN->tellg();
    // begining of line:
    this_pos -= IN;
    if( this_pos < 0 ){
      this_pos = 0;
    }
    FIN->seekg( this_pos, ios::beg);
  }
}

///////////////////////////////////////////////////
// jump to the start of the line:
void file_reader::move_to_start_of_line(){
  if( FIN->good() ){
    int this_pos = FIN->tellg();
    // begining of line:
    this_pos -= FIN->gcount();
    if( this_pos < 0 ){
      this_pos = 0;
    }
    FIN->seekg( this_pos, ios::beg);
  }
}

///////////////////////////////////////////////////
// get character at this stream position
string file_reader::get_character(){

  string t;
  if( FIN->good() ){
    t = FIN->get();
  }
  return t;
}

///////////////////////////////////////////////////
// close a file:
void file_reader::close_file(){
 
 if(FIN != NULL){
  FIN->close();
  
  // delete the instance:
  delete FIN;
  FIN = NULL;
 }
}

///////////////////////////////////////////////////
// check if end of file  reached:
bool file_reader::check_end_of_file(){
 
 if(FIN != NULL){
  if(FIN->eof())
   return true;
  else
   return false;
 }
 else
  return true;
}



///////////////////////////////////////////////////
// read a file:
void file_reader::read_file_line(){
  
  if(FIN->good()){
    FIN->getline(line, max_nb_character_per_line);
    while( !strncmp(line, "//", 2) ){
      FIN->getline(line, max_nb_character_per_line);
    }
  }
  else{
    if(line != NULL){
      delete line;
      line = NULL;
    }
  }
}

///////////////////////////////////////////////////
// read a file ad comments
void file_reader::read_file_line_with_comments(){
  
  if( FIN->good() ){
    FIN->getline(line, max_nb_character_per_line);
  }
  else{
    if(line != NULL){
      delete line;
      line = NULL;
    }
  }
}

///////////////////////////////////////////////////
// read a file:
void file_reader::read_file_line(char END){
  
  if(FIN->good()){
    FIN->getline(line, max_nb_character_per_line, '\r' );
    while( !strncmp(line, "//", 2) ){
      FIN->getline(line, max_nb_character_per_line, '\r' );
    }
  }
  else{
    if(line != NULL){
      delete line;
      line = NULL;
    }
  }
}




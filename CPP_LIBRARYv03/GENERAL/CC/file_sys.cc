///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS FILE SYSTEM :
//
// provides functions to access the filesystem:
//  - read / write files
//  - get directory list
//
// **********************************************************************//

#define USE_FILE_SYS
#define USE_FSTREAM
#define USE_STRING
#define USE_IO
#define USE_STRING
#define USE_STD_DEF
#define USE_SYS_TYPES
#define USE_UNISTD
#define USE_STDIO
#define USE_NAMESPACE
#define USE_DIRENT

#include "GENERAL_USE.h"


///////////////////////////////////////////////////////////////////
// constructor:
file_sys::file_sys(){
  
  // counter to zero:
  nb_files=0;
  
  ///////////////////////////////////
  // define here the file types for
  // the dirent object
  file_type = 8;
  dir_type = 4;
  DIR_FILES.clear();
}


///////////////////////////////////////////////////////////////////
// this function takes a string and checks if it is a path to a
// directory(true) or a file(false)
bool file_sys::check_directory(string IN_target){
    
  int dir_count = 0;
  DIR* dp = NULL;
  struct dirent *ep;
  dp = opendir( IN_target.c_str() );
  
  if( dp != NULL ){
    while( ( ep = readdir( dp ) ) ){
      dir_count++;
    }
    //close
    (void)closedir( dp );
  }
  
  if( !dir_count )
    return false;
  else
    return true;
  
}

////////////////////////////////////////////////////////////////
// checks if a file exists:
bool file_sys::check_file_exists(string IN){
  
  ifstream T; 
  T.open(IN.c_str(), ifstream::in);
  
  if( T.fail() ){
    T.clear(ios::failbit);    
    T.close();
    return false;
  }
  else{
    return true;
  }
}

////////////////////////////////////////////////////////////////
// checks if a file exists in directory:
bool file_sys::check_file_exists_in_Dir(string IN, string file_extension){
  
  string file, dir;
  
  // extract file and dir:
  // check if the directory ends with a backslash:
  if( IN[IN.size()-1] == '/'){
    return false;
  }
  else{
    int POS = IN.rfind( '/' ) + 1;
    dir = IN;
    dir.erase( POS, IN.size() - POS );
    file = IN.erase( 0, POS );
  }
  
		// start reading the directory:
  DIR* DP;
  struct dirent *EP;
  DP = opendir(dir.c_str());
  if(DP != NULL){
    
    while( ( EP = readdir(DP) ) ){
      // this file name
      string this_file = EP->d_name;
      
      // copy now only the specific files into the variable:
      // skip the file index . and ..
      if( ( strstr( this_file.c_str() , file_extension.c_str()) != NULL) && ( this_file != ".." ) && ( this_file != "." ) && (strstr( this_file.c_str() , "~" ) == NULL)){
    
        // check if its the file matches the input file:
        if( this_file.find( file ) != string::npos ){
          return true;
        }
      }
    }

    (void)closedir(DP);
  }
  
  return false;
}


////////////////////////////////////////////////////////////////
// get the exact file name in directory:
string file_sys::get_file_name_in_Dir(string IN, string file_extension){
  
  string file, dir;
  // extract file and dir:
  // check if the directory ends with a backslash:
  if( IN[IN.size()-1] != '/'){
    int POS = IN.rfind( '/' ) + 1;
    dir = IN;
    dir.erase( POS, IN.size() - POS );
    file = IN.erase( 0, POS );
  }
  else{
    dir = IN;
  }
  
		// start reading the directory:
  DIR* DP;
  struct dirent *EP;
  DP = opendir(dir.c_str());
  if(DP != NULL){
    
    while( ( EP = readdir(DP) ) ){
      // this file name
      string this_file = EP->d_name;
      
      // copy now only the specific files into the variable:
      // skip the file index . and ..
      if( ( strstr( this_file.c_str() , file_extension.c_str()) != NULL) && ( this_file != ".." ) && ( this_file != "." ) && (strstr( this_file.c_str() , "~" ) == NULL)){
        
        // check if its the file matches the input file:
        if( this_file.find( file ) != string::npos ){
          this_file = dir + this_file;
          return this_file;
        }
      }
    }
    
    (void)closedir(DP);
  }
  
  return "";
}



///////////////////////////////////////////////////////////////////
// this function takes a directory and extracts its files ending
// with the target_extension
void file_sys::read_dir(string target, string ending){

  nb_files = 0;
  DIR_FILES.clear();
	
  DIR* dp;
  struct dirent *ep;
  dp = opendir(target.c_str());
  
  // check if the directory ends with a backslash:
  if(target[target.size()-1] != '/'){
    target += "/";
  }
		
  // set here the file extension for the
  // dta file:
  string file_extension = ending;
	
  if(dp != NULL){
    
    nb_files = 0;
    while( (ep = readdir(dp)) ){

      string this_file = ep->d_name;

      // copy now only the specific files into the variable:
      // skip the file index . and ..
      if((strstr( this_file.c_str() , file_extension.c_str()) != NULL) && ( this_file != ".." ) && ( this_file != "." ) && (strstr( this_file.c_str() , "~" ) == NULL)){
        string save_file = target + this_file;
        DIR_FILES.insert( make_pair( save_file, nb_files ) );
        nb_files++;
      }
    }
    (void)closedir(dp);
  }
  else{
    char err[200];
    sprintf(err,"\t\t\tCould not read from directory %s",target.c_str());
    cout<<err<<endl;
  }
  
  // set the current counter to the start of the dir file list:
  CURRENT_FILE = DIR_FILES.begin();  
}

///////////////////////////////////////////////////////////////////
// this function takes a directory and extracts its files ending
// with the target_extension
void file_sys::read_dir(string target, vector<string> ENDINGS){
  
  nb_files = 0;
  DIR_FILES.clear();
  
  DIR* dp;
  struct dirent *ep;
  dp = opendir(target.c_str());
  
  // check if the directory ends with a backslash:
  if(target[target.size()-1] != '/'){
    target += "/";
  }
		
  // set here the file extension for the
  // dta file:  
  if(dp != NULL){
    
    nb_files = 0;
    while( (ep = readdir(dp)) ){
      
      string this_file = ep->d_name;
      
      // skip the file index . and ..
      if( ( this_file != ".." ) && ( this_file != "." ) && (strstr( this_file.c_str() , "~" ) == NULL)){       
        
        
        // check for the specific files:
        vector<string>::iterator this_end = ENDINGS.begin();
        while( this_end != ENDINGS.end() ){
          
          if( strstr( this_file.c_str() , (*this_end).c_str()) != NULL ){
            string save_file = target + this_file;
            DIR_FILES.insert( make_pair( save_file, nb_files ) );
            nb_files++;
            break;
          }
          
          this_end++;
        }
      }
    }
    (void)closedir(dp);
  }
  else{
    char err[200];
    sprintf(err,"\t\t\tCould not read from directory %s",target.c_str());
    cout<<err<<endl;
  }
  
  // set the current counter to the start of the dir file list:
  CURRENT_FILE = DIR_FILES.begin();  
}


///////////////////////////////////////////////////////////////////
// this function takes a directory and extracts all files 
// except .. and .
void file_sys::read_dir(string target){
  
  nb_files = 0;
  DIR_FILES.clear();
  
  DIR* dp;
  struct dirent *ep;
  dp = opendir(target.c_str());
  
  // check if the directory ends with a backslash:
  if(target[target.size()-1] != '/'){
    target += "/";
  }
		
  if(dp != NULL){
    
    nb_files = 0;
    while( (ep = readdir(dp)) ){
      
      string this_file = ep->d_name;
      
      // copy now only the specific files into the variable:
      // skip the file index . and .. and "~"- containing names
      if( ( this_file != ".." ) && ( this_file != "." ) && (strstr( this_file.c_str() , "~" ) == NULL) ){
        string save_file = target + this_file;
        DIR_FILES.insert( make_pair( save_file, nb_files ) );
        nb_files++;
      }
    }
    (void)closedir(dp);
  }
  else{
    char err[200];
    sprintf(err,"\t\t\tCould not read from directory %s",target.c_str());
    cout<<err<<endl;
  }
  
  // set the current counter to the start of the dir file list:
  CURRENT_FILE = DIR_FILES.begin();  
}



////////////////////////////////////////////////
// destructor of the object

file_sys::~file_sys(){
  DIR_FILES.clear();
}


////////////////////////////////////////////////
// gets the next file:
char* file_sys::get_next_file(){
  
  if( CURRENT_FILE != DIR_FILES.end() ){
    char* out = (char*)((*CURRENT_FILE).first.c_str());
    CURRENT_FILE++;
    return out;
  }
  else{
    return NULL;
  }
}


///////////////////////////////////////////////
// removes a directory:
int file_sys::remove_directory(string IN){
  
  // check if its really a directrory:
  if(!check_directory(IN)){
    remove(IN.c_str());
    return 0;
  }
  else{
   
    // read the content:
    read_dir(IN);
    char* file = get_next_file();
    while(file != NULL){
      // check if its a directory:
      if(check_directory(IN)){
        remove_directory(file);
        read_dir(IN);
      }
      else{
        remove(file);
      }
      file = get_next_file();
    }
    rmdir(IN.c_str());
    return 0;
  }
}

///////////////////////////////////////////////
// removes the content of a directory:
int file_sys::remove_directory_content(string IN){
  
  string NOT = IN + "LC_MS_RUNS";
  bool end_del = true;
  
  // check if its really a directrory:
  if(!check_directory(IN)){
    return 0;
  }
  else{
    
    // read the content:
    read_dir(IN);
    char* file = get_next_file();
    while(file != NULL){
      
      // check if its not a directory which should be kept:
      if(!strncmp(file, NOT.c_str(),NOT.size())){
        // check if its not empty:
        read_dir(file);
        // if no file, then its empty:
        if(get_nb_files() == 0){
          end_del = true;
          remove_directory(NOT.c_str());
        }
        else{
          end_del = false;
        }
        read_dir(IN);
        file = get_next_file();
      }
      
      // check if this should be removed:
      if(strncmp(file, NOT.c_str(),NOT.size())){
        // check if its a directory:
        if(check_directory(file)){
          remove_directory(file);
          read_dir(IN);
        }
        else{
          remove(file);
        }
      }
      file = get_next_file();
    }

    if(end_del)
      rmdir(IN.c_str());
    return 0;
  }
}

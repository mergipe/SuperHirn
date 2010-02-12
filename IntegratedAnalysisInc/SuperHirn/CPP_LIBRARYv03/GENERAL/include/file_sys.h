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

#ifndef FILE_SYS_H
#define FILE_SYS_H


class file_sys{
  
 private:
  int file_type;
  int dir_type;
 
  map<string, int> DIR_FILES;
  map<string, int>::iterator CURRENT_FILE;
  int nb_files;
  
 public:
  ~file_sys();
  file_sys();
  
  // get the number of extracted files:
  int get_nb_files(){return nb_files;};
  
  // extracts all the files with a certain extension:
  void read_dir(string,string);
  void read_dir(string, vector<string> );
  // this function takes a directory and extracts all files 
  // except .. and .
  void read_dir(string);
  
  // checks if a file exists:
  bool check_file_exists(string);
  // checks if a file exists in directory:
  bool check_file_exists_in_Dir(string, string);
  // get the exact file name in directory:
  string get_file_name_in_Dir(string, string);

  
  // checks if the target is a directory(true) or file(false):
  bool check_directory(string);
  
  // gets the next file:
  char* get_next_file();
  
  // removes a directory:
  int remove_directory(string);
  
  // removes the content of a directory:
  int remove_directory_content(string);


  
};

#endif


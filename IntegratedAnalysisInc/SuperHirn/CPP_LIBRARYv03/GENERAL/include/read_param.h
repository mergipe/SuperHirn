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


#ifndef READ_PARAM_H
#define READ_PARAM_H

#include "file_reader.h" 

class read_param{

    
 ////////////////////////////////////////////////
 // declaration of the private members:
 
private:
 
  bool file_check;
  bool ROOT_FILE_OPEN, TAG_ROOT_CHECK;
  file_reader* reader;
 ////////////////////////////////////////////////
 // declaration of the public members:
 
public:
  static string ROOT_PARAM_FILE;
  static string COSTUM_PARAM_FILE;

  
  // class destructor
   ~read_param();
 // class constructor
 read_param();
 
 // print error message:
 void print_error();
 // print error message of the tag search
 void print_search_error(string);
 // stopp the program since a parameter was not found:
 void stopp_program();
 // OPENS THE CUSTOM PARAMETER FILE:
 void open_CUSTOM_PARAM( );
 // OPENS THE ROOT PARAMETER FILE:
 bool open_ROOT_PARAM( string );

 
 ///////////////////////////////////////////////////
 // PARAMETER HELP SEARCH:
 // searches the root parameter file for tags
 // by string search:
 void search_param_help(string );
   // print the text of a parameter to screen
 void show_parameter_to_screen();

 ///////////////////////////////////////////////////
 // searches the definition file for a tag and
 // and returns the full line where the tag was found:
 string search_line_by_tag(string);
 // trim the end of a  string, remove space etc.
 string trim_end_of_string( string , int);

 
 // searches the definition file for a tag and
 // copy the found string into OUT, then tranforms it
 // if required into double / int via overloading
 string search_tag(string);
 void search_tag(string , double* );
 void search_tag(string , float* );
 void search_tag(string , int* );
 void search_tag(string , unsigned int* );
 void search_tag(string , bool* );
 void search_tag(string , vector<double>* );
 void search_tag(string , map<double,double>* );
 void search_tag(string , vector< vector<double> >*);
 void search_tag(string , map<int, vector<int> >*);
 void search_tag(string , map<int, vector<double> >*);
 void search_tag(string , map<string, vector<double> >*);
 void search_tag(string , map<string, vector<string> >*);

 
 ///////////////////////////////
 // start here all the get / set
 // function to access the
 // variables of the class
 char* get_line(){return reader->get_line();};
 void read_next_line(){reader->read_file_line();};
 
};

#endif


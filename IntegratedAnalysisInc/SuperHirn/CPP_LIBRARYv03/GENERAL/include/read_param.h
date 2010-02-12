#ifndef READ_PARAM_H
#define READ_PARAM_H


/*
 * 
 * @author Lukas N Mueller (Lukas.Mueller@imsb.biol.ethz.ch), Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
 *
 * @brief Class to parse parameters from a text based parameter file. By default, parameters are extracted from the file "param.def" 
 * that has to be located in the program startup folder. If a parameter is not found in this file, the fall back mechanisme extracts
 * parameters from the root parameter file.
 * Parameters can also be statically stored in the static variable global_parameters. If a parameters exists in this map, it is preferred to those in the parameters files.
 */


#include "file_reader.h" 

class read_param{

    
private:
 
  bool file_check;
  bool ROOT_FILE_OPEN, TAG_ROOT_CHECK;
  file_reader* reader;
  

  
public:
 
  static string ROOT_PARAM_FILE;
  static string COSTUM_PARAM_FILE;
  // @brief global parameters replace those in the parameter file 
  static map<string, string> global_parameters;

  
   ~read_param();
  read_param();
  
  /*
   * @brief print error message:
   */
  void print_error();
  /*
   * @brief print error message of the tag search
   */
  void print_search_error(string);
  /*
   * @brief stopp the program since a parameter was not found
   */
  void stopp_program();

  
  /*
   * @brief OPENS THE CUSTOM PARAMETER FILE
   */
 void open_CUSTOM_PARAM( );
  
  /*
   * @brief OPENS THE ROOT PARAMETER FILE:
   */
  bool open_ROOT_PARAM( string );
  
 
  /*
   * @brief searches the root parameter file for tags by string search:
   */
  void search_param_help(string );

  /*
   * @brief print the text of a parameter to screen
   */
  void show_parameter_to_screen();
  

  /*
   * @brief searches the definition file for a tag and and returns the full line where the tag was found:
   */
  string search_line_by_tag(string);
  
  /*
   * @brief trim the end of a  string, remove space etc.
   */
  string trim_end_of_string( string , int);
  
  
  /*
   * @brief searches the definition file for a tag and
   * copy the found string into OUT, then tranforms it if required into double / int via overloading
   */
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

  
  /*
   * @brief start here all the get / set function to access the variables of the class
   */
  char* get_line(){return reader->get_line();};
  void read_next_line(){reader->read_file_line();};
 
  
  /*
   * @brief Store a global parameter as Key/Value. This parameter replaces parameters with identical key in the parameter files.
   * @param iKey string, key of the parameter
   * @param iVallue string, value of the parameter
   */
  static void addGlobalParameter(string iKey, string iValue);

};

#endif


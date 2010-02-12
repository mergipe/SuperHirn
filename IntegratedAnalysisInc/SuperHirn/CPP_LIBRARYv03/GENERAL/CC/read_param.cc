///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//

// **********************************************************************//
// CLASS Read param:
// reads the program parameter from the target file
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_IO
#define USE_STDLIB
#define USE_MATH
#define USE_STRING
#define USE_VECTOR
#define USE_FILE_READER
#define USE_NAMESPACE
#define USE_READ_PARAM

#include "GENERAL_USE.h"


string read_param::COSTUM_PARAM_FILE = "param.def";
string read_param::ROOT_PARAM_FILE;

////////////////////////////////////////////////
// constructor for the object ana_summarizer:
read_param::read_param(){
  
  // parameter which checks if file is ok
  // readable
  file_check = false;
  ROOT_FILE_OPEN = false;
  
  // construct the file reader object
  reader = NULL;
  // open the custom parameter file:
  open_CUSTOM_PARAM( );
 
}

//////////////////////////////////////////////////
// class desctructor

read_param::~read_param(){
 
 if(reader != NULL){
  reader->close_file();
  
  delete reader;
  reader = NULL;
  
 }
 
}


///////////////////////////////////////////////////
// searches the definition file for a tag and
// and returns the full line where the tag was found:
string read_param::search_line_by_tag(string tag){
  
  // if the root param is open,
  // then close it and open again
  // since always check first the param.def:
  
  if( ROOT_FILE_OPEN ){
    open_CUSTOM_PARAM(); 
  }
    
  // check if file is readable:
  if( file_check ){
    
    // put instream to start of file:
    reader->jump_to_file_start();
    
    int tag_len = tag.size();
    TAG_ROOT_CHECK = false; 
    // read file until the tag or end is reached:
    while( get_line() != NULL){

      // check if tag found:
      if(!strncmp(get_line(),tag.c_str(),tag_len)){
        break;
      }
      
      //  check if end of file is reached:
      if(reader->check_end_of_file()){
        if( ! open_ROOT_PARAM( tag ) ){
          print_search_error(tag);
          return NULL;
        }
      }
      
      read_next_line();
    }
    
    // save this line from after start tag on:
    string OUT = get_line();
    OUT = trim_end_of_string( OUT , tag.size());
    return OUT;
  }
  else{
    print_error();
    return NULL;
  }
}


///////////////////////////////////////////////////
// trim from a pos of teh string the end of a  
// string, remove space etc.
string read_param::trim_end_of_string( string IN, int pos ){
  
  // erase all after the content:
  unsigned int h = pos;
  while( (IN[h]!='\0') && (h < IN.size()) ){
//  while( (IN[h]!='\0') && (IN[h]!=' ') && (h < IN.size()) ){
    h++;
  }
  IN.erase(h, IN.size());
  return IN;
}

///////////////////////////////////////////////////
// searches the definition file for a tag and
// copy the found string into OUT, then tranforms it
// if required into double / int via overloading
// here for chars
string read_param::search_tag(string tag){
  
  // save this line from after start tag on:
  string OUT = search_line_by_tag( tag );
  
  // erase stuff before the tag:
  OUT.erase(0, tag.size() +1);      
  return OUT;
}




////////////////////////////////////////////////
// here for double
void read_param::search_tag(string tag, double* d_OUT){
  
  // save this line from after start tag on:
  string OUT = search_line_by_tag( tag );
  // erase stuff before the tag:
  OUT.erase(0, tag.size() +1);    
  // convert:
  *d_OUT = atof(OUT.c_str());
  
}



///////////////////////////////////////////////////
// here for int's
void read_param::search_tag(string tag, int* d_OUT){
  
  // save this line from after start tag on:
  string OUT = search_line_by_tag( tag );
  // erase stuff before the tag:
  OUT.erase(0, tag.size() +1);    
  *d_OUT = atoi(OUT.c_str());
}

////////////////////////////////////////////////////////
// here for unsigned int's
void read_param::search_tag(string tag, unsigned int* d_OUT){
  
  // save this line from after start tag on:
  string OUT = search_line_by_tag( tag );
  
  // erase stuff before the tag:
  OUT.erase(0, tag.size() +1);    
  *d_OUT = atoi(OUT.c_str());
  
}


/////////////////////////////////////////////////////
// here for bool's
void read_param::search_tag(string tag, bool* d_OUT){
  
  
  // save this line from after start tag on:
  string OUT = search_line_by_tag( tag );
  
  // erase stuff before the tag:
  OUT.erase(0, tag.size() +1);    
  
  if( atoi(OUT.c_str())){
    *d_OUT = true;
  }
  else{
    *d_OUT = false;
  }  
}

///////////////////////////////////////////////////////
// here for float's
void read_param::search_tag(string tag, float* d_OUT){
  
  // save this line from after start tag on:
  string OUT = search_line_by_tag( tag );
  
  // erase stuff before the tag:
  OUT.erase(0, tag.size() +1);    
  *d_OUT = atof(OUT.c_str());
}


////////////////////////////////////////////////////////////
// here for vector of doubles:
void read_param::search_tag(string tag, vector<double>* d_OUT){
  
  d_OUT->clear();
  
  // save this line from after start tag on:
  string OUT = search_line_by_tag( tag );
  
  // erase stuff before the tag:
  OUT.erase(0, tag.size() +1);    
      
  // now extract the individual values separated by comma:
  while(OUT.find(",") != string::npos){
    string T = OUT.substr(0, OUT.find(",") );
    d_OUT->push_back(atof(T.c_str()));
    OUT.erase(0, OUT.find(",") + 1);
  }
  d_OUT->push_back(atof(OUT.c_str()));

}

////////////////////////////////////////////////////////////////
// here for vector of vectors<doubles>:
void read_param::search_tag(string tag, vector< vector<double> >* d_OUT){
  
  d_OUT->clear();
  
  // save this line from after start tag on:
  string OUT = search_line_by_tag( tag );
  
  // save this line from after start tag on:
  string tmp = get_line();
  // erase stuff before the tag:
  tmp.erase(0,tag.size());
  
  // check if teh # signs is there, otherwise there is something wrong:
  if( tmp[0] == '#' ){  
    
    read_next_line();
    if( get_line() != NULL )
      tmp = get_line();
    
    // jump into the reading loop, for every row/line a vector
    while( ( tmp[0] != '#' ) && ( get_line() != NULL )) { 
      
      vector<double> out;
      
      // now extract the individual values separated by comma:
      while(tmp.find(",") != string::npos){
        string T = tmp.substr(0, tmp.find(",") );
        out.push_back(atof(T.c_str()));
        tmp.erase(0, tmp.find(",") + 1);
      }
      out.push_back(atof(tmp.c_str()));
      
      d_OUT->push_back(out);
      
      read_next_line();
      if( get_line() == NULL )
        break;
      
      tmp = get_line();
    }
    
  }
}

//////////////////////////////////////////////////////////////////
// here for vector of map<doubles, double>:
void read_param::search_tag(string tag, map<double,double>* d_OUT){
  
  d_OUT->clear();
  
  // save this line from after start tag on:
  string OUT = search_line_by_tag( tag );

  string tmp;
  read_next_line();
  if( get_line() != NULL )
    tmp = get_line();
  
  // jump into the reading loop, for every row/line a vector
  while( ( tmp[0] != '#' ) && ( get_line() != NULL )) { 
    
    string T = tmp.substr(0, tmp.find(",") );
    double A = atof(T.c_str());
    tmp.erase(0, tmp.find(",") + 1);
    double B = atof(tmp.c_str());
    d_OUT->insert( make_pair( A, B) );
    
    read_next_line();
    if( get_line() == NULL )
      break;
    
    tmp = get_line();
  }
  
}


////////////////////////////////////////////////////////////////////////
// here for map <int, vectors<int>:
void read_param::search_tag(string tag, map<int, vector<int> >* d_OUT){
  
  d_OUT->clear();
    
  map<int, vector<double> > tmp;
  search_tag( tag, &tmp );
  
  // convert the double to ints:
  map<int, vector<double> >::iterator P = tmp.begin();
  while( P != tmp.end() ){
    
    vector<int> vec;
    vector<double>::iterator V = (*P).second.begin();
    while( V != (*P).second.end() ){
      vec.push_back( (int)(*V) );
      V++;
    }
    
    (*d_OUT).insert( make_pair( (*P).first, vec ) );
    P++;
  }
}

////////////////////////////////////////////////////////////////////////
// here for map <int,  vectors<doubles>:
void read_param::search_tag(string tag, map<int, vector<double> >* d_OUT){

  d_OUT->clear();
  
  // save this line from after start tag on:
  string OUT = search_line_by_tag( tag );
      
  // erase stuff before the tag:
  OUT.erase(0, tag.size() +1);    
  
  
  // save this line from after start tag on:
  string tmp = get_line();
  // erase stuff before the tag:
  tmp.erase(0,tag.size());
  
  // check if teh # signs is there, otherwise there is something wrong:
  if( tmp[0] == '#' ){  
    
    read_next_line();
    if( get_line() != NULL ){
      tmp = get_line();
    }
    
    // jump into the reading loop, for every row/line a vector
    while( ( tmp[0] != '#' ) && ( get_line() != NULL )) { 
      
      vector<double> out;
      // now extract the individual values separated by comma:
      while(tmp.find(",") != string::npos){
        string T = tmp.substr(0, tmp.find(",") );
        out.push_back(atof(T.c_str()));
        tmp.erase(0, tmp.find(",") + 1);
      }
      out.push_back(atof(tmp.c_str()));
      
      int first = d_OUT->size();
      d_OUT->insert(pair<int, vector<double> >(first,out));
      
      read_next_line();
      if( get_line() == NULL )
        break;
      
      tmp = get_line();
    }
  }

}

//////////////////////////////////////////////////////////////////////////
// here for map <string,  vectors<doubles>:
void read_param::search_tag(string tag, map<string, vector<double> >* d_OUT){
  
  d_OUT->clear();
  
  // save this line from after start tag on:
  string OUT = search_line_by_tag( tag );
      
  // erase stuff before the tag:
  OUT.erase(0, tag.size() +1);    
  // save this line from after start tag on:
  string tmp = get_line();
  // erase stuff before the tag:
  tmp.erase(0, tag.size());
  
  // check if teh # signs is there, otherwise there is something wrong:
  if( tmp[0] == '#' ){  
    
    read_next_line();
    if( get_line() != NULL ){
      tmp = get_line();
    }
    
    // jump into the reading loop, for every row/line a vector
    while( ( tmp[0] != '#' ) && ( get_line() != NULL )) { 
      string KEY_STRING = tmp.substr(0, tmp.find(",") );
      tmp.erase(0, tmp.find(",") + 1);
      vector<double> V_OUT;
      // now extract the individual values separated by comma:
      while(tmp.find(",") != string::npos){
        string T = tmp.substr(0, tmp.find(",") );
        V_OUT.push_back(atof(T.c_str()));
        tmp.erase(0, tmp.find(",") + 1);
      }
      V_OUT.push_back(atof(tmp.c_str()));        
      d_OUT->insert(make_pair(KEY_STRING , V_OUT));
      V_OUT.clear();
      read_next_line();
      if( get_line() == NULL )
        break;
      
      tmp = get_line();
    }
  }
}

/////////////////////////////////////////////////////////////////////
// here for map <string,  vectors<doubles>:
void read_param::search_tag(string tag, map<string, vector<string> >* d_OUT){
  
  d_OUT->clear();
  
  // save this line from after start tag on:
  string OUT = search_line_by_tag( tag );
      
  // erase stuff before the tag:
  OUT.erase(0, tag.size() +1);    
  
  // save this line from after start tag on:
  string tmp = get_line();
  
  // erase stuff before the tag:
  tmp.erase(0,tag.size());
  
  // check if teh # signs is there, otherwise there is something wrong:
  if( tmp[0] == '#' ){  
    
    read_next_line();
    if( get_line() != NULL ){
      tmp = get_line();
    }
    
    // jump into the reading loop, for every row/line a vector
    while( ( tmp[0] != '#' ) && ( get_line() != NULL )) { 
      string KEY_STRING = tmp.substr(0, tmp.find(",") );
      tmp.erase(0, tmp.find(",") + 1);
      vector<string> V_OUT;
      // now extract the individual values separated by comma:
      while(tmp.find(",") != string::npos){
        string T = tmp.substr(0, tmp.find(",") );
        V_OUT.push_back( T );
        tmp.erase(0, tmp.find(",") + 1);
      }
      V_OUT.push_back( tmp );        
      d_OUT->insert(make_pair(KEY_STRING , V_OUT));
      V_OUT.clear();
      read_next_line();
      if( get_line() == NULL )
        break;
      
      tmp = get_line();
    }
  }
}



///////////////////////////////////////////////////
// print error message:
void read_param::print_error(){
  if( ROOT_FILE_OPEN ){
    printf("\n *** \n ERROR opening file '%s', read_param.cc::113 \n *** \n", ROOT_PARAM_FILE.c_str() );
  }
  else{
    printf("\n *** \n ERROR opening file 'param.def', read_param.cc::113 \n *** \n");
  }
  stopp_program();
}

///////////////////////////////////////////////////
// print error message of the tag search
void read_param::print_search_error(string tag){
  if( ROOT_FILE_OPEN ){
    printf("\n *** \n ERROR:: tag '%s' was not found in '%s' \n *** \n", tag.c_str(), ROOT_PARAM_FILE.c_str() );
  }
  else{
    printf("\n *** \n ERROR:: tag '%s' was not found in 'param.def'\n *** \n", tag.c_str());
  }
  stopp_program();
}

///////////////////////////////////////////////////
// stopp the program since a parameter was not found:
void read_param::stopp_program(){
  printf("*** PROGRAM EXIT::incomplete parameter input *** \n\n\n");
  exit(0);
}



///////////////////////////////////////////////////
// OPENS THE ROOT PARAMETER FILE:
bool read_param::open_ROOT_PARAM( string TAG ){
  
  if( ROOT_FILE_OPEN ){
    return false;
  }
  
  if( TAG == "ROOT PARAMETER FILE"){
    return false; 
  }
  
  if( file_check ){
    reader->open_file();
    ROOT_PARAM_FILE = search_tag( "ROOT PARAMETER FILE" );
  }
  
  if( file_check || ROOT_FILE_OPEN ){
    reader->close_file();
  }
  
  if( reader != NULL ){
    delete reader;
    reader = NULL;
  }
  
  file_check = false;
  reader = new file_reader();
  // set the parameter file:
  reader->set_current_file( ROOT_PARAM_FILE );
  
  // open file and save the status
  ROOT_FILE_OPEN = reader->open_file();
  ROOT_FILE_OPEN = true;
  return ROOT_FILE_OPEN;
}


///////////////////////////////////////////////////
// OPENS THE ROOT PARAMETER FILE:
void read_param::open_CUSTOM_PARAM( ){
  
  if( file_check || ROOT_FILE_OPEN ){
    reader->close_file();
  }
  
  if( reader != NULL ){
    delete reader;
    reader = NULL;
  }

  file_check = false;
  ROOT_FILE_OPEN = false;
  reader = new file_reader(  );
  reader->set_COMMENT( false );
  // set the parameter file:
  reader->set_current_file( COSTUM_PARAM_FILE );
  
  // open file and save the status
  file_check = reader->open_file();
  if( !file_check ){
    open_ROOT_PARAM( "" );
  }
}



///////////////////////////////////////////////////
// searches the root parameter file for tags
// by string search:
void read_param::search_param_help(string tag){
  
  printf( "\t\tParameter file search for string '%s':\n", tag.c_str() );
  
  open_ROOT_PARAM( tag );
  
  // check if file is readable:
  if( ROOT_FILE_OPEN ){
    
    // put instream to start of file:
    reader->jump_to_file_start();
    reader->read_file_line_with_comments();
    
    bool FOUND = false;
    
    // read file until the tag or end is reached:
    while( get_line() != NULL){
      
      string CONTENT = get_line();
      
      // check if tag found:
      if( CONTENT.find( tag ) != string::npos ){
        show_parameter_to_screen();
        FOUND = true;
      }
      
      //  check if end of file is reached:
      if(reader->check_end_of_file()){        
        if( !FOUND ){
          print_search_error(tag);
        }
        break;
      }
      reader->read_file_line_with_comments();
    }
  }
  else{
    print_error();
  }
}



////////////////////////////////////////////
// print the text of a parameter to screen
void read_param::show_parameter_to_screen(){
  

  int TAG_START = reader->get_stream_pos();
  reader->move_to_start_of_line();
  while( get_line() ){

    reader->move_cursor_back(2);
    while( reader->get_character() != "\n" ){
      reader->move_cursor_back(2);
    }
    string tmp = reader->get_character();
    reader->move_cursor_back(2);
    if( ( tmp[0] != '\n' ) ||( tmp[0] != '/' ) || ( tmp.size() == 0) ){
      break;
    }
  }
  
  char buffer[10000];
  while( reader->get_stream_pos() <= TAG_START ){
    reader->read_file_line_with_comments();
    sprintf(buffer, "%s%s\n", buffer,get_line());
  }
  printf("%s\n",buffer);
  
}
  
///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//


#ifndef FILE_READER_H
#define FILE_READER_H


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


class file_reader{

    
 ////////////////////////////////////////////////
 // declaration of the private members:
 
protected:
 
 string current_file;
 ifstream* FIN;
 char* line;
 int max_nb_character_per_line;
 bool COMMENT;
 ////////////////////////////////////////////////
 // declaration of the public members:
 
public:

  // class destructor
  ~file_reader();
 // class constructor
 file_reader();
 
 //open file
 bool open_file();
 
 //close file
 void close_file();
 
 //read a line in the file
 void read_file_line();
 void read_file_line( char );
 // read a file ad comments
 void read_file_line_with_comments();
 
 // jumpto start of a file:
 void jump_to_file_start();
 // jump n positions back in the file 
 void move_cursor_back(int);
 // jump to the start of the line:
 void move_to_start_of_line();
 // get character at this stream position
 string get_character();

 
 // check if end of file  reached:
 bool check_end_of_file();
 
 ///////////////////////////////
 // start here all the get / set
 // function to access the
 // variables of the class
 void set_current_file(string IN_file){current_file = IN_file;};
 char* get_line(){return line;};
 int get_max_per_line_to_read(){return max_nb_character_per_line;};
 int get_stream_pos(){return FIN->tellg();};
 
 void set_COMMENT( bool IN ){COMMENT = IN; };
};

#endif


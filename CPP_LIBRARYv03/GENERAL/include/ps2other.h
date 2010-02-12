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


#ifndef _PS2OTHER_H
#define _PS2OTHER_H


class ps2other{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:

  string ps_file;
  
  static string GS_CMD;
  static string EXT_JPG;
  static string EXT_TIF;
  static string RESOLUTION;
  static string GS_JPEG_PACK;
  static string GS_TIFF_PACK;

  
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~ps2other();
  
  // class constructor
  ps2other();
  // class copy constructor
  ps2other(const ps2other&);
  // class copy constructor
  ps2other(const ps2other*);
  
  
  // convert to tif file:
  void convert2tif();
  // convert to jpeg file:
  void convert2jpeg();

  // write commans:
  void execute_command(string);
  // deletes the ps file
  void rm_ps_file();


  // create out path:
  string create_output_string(string, string);

  
  
  //////////////////////////////////////////////////
  // overload operators:
  ps2other& operator=(const ps2other&);
  bool operator==(const ps2other&);
  ps2other& operator<=(const ps2other&);
  ps2other& operator>=(const ps2other&);
  ps2other& operator<(const ps2other&);
  ps2other& operator>(const ps2other&);
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

  void set_ps_file(string IN){ps_file = IN;};
};

#endif

    

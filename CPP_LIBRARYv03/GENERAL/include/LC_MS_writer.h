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


#ifndef LC_MS_WRITER_H
#define LC_MS_WRITER_H



class LC_MS_writer{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  LC_MS* LC_MS_MAP;
  string out_file;
  string target_dir;
  string namePrefix;
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
    
  // class destructor
  ~LC_MS_writer();
  
  // class constructor
  LC_MS_writer();
  LC_MS_writer( LC_MS* );
  LC_MS_writer( string );
  // class copy constructor
  LC_MS_writer(const LC_MS_writer&);
  // class copy constructor
  LC_MS_writer(const LC_MS_writer*);
  

  
  //////////////////////////////////////////////////////////
  // outputs a MasterMap into XML formatted file with this fileName:
  void storeMasterMapToXML( );
  void storeMasterMapToXML( string  );
  
  //////////////////////////////////////////////////////////
  // outputs into Mueller Superhirn LN XML formatted file
  void storeMasterMapToSuperMuellerLNXML( );
    
  ////////////////////////////////////////////////////////////////////////
  // outputs into Brusniak APML XML formatted file
  void storeMasterMapToAPML( );
  
  
  // removes which spaces in the string:
  string processOutName( string );
    
  
  //////////////////////////////////////////////////
  // overload operators:
  LC_MS_writer& operator=(const LC_MS_writer&);
  LC_MS_writer& operator<=(const LC_MS_writer&);
  LC_MS_writer& operator>=(const LC_MS_writer&);
  LC_MS_writer& operator<(const LC_MS_writer&);
  LC_MS_writer& operator>(const LC_MS_writer&);
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  

};

#endif

    

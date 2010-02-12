///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS LC_MS_similarity_matrix:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_LC_MS_SIMILARITY_MATRIX
#include "LC_MS_TOOLS_USE.h"


string LC_MS_similarity_matrix::PerlPathName;


////////////////////////////////////////////////
// constructor for the object LC_MS_similarity_matrix:
LC_MS_similarity_matrix::LC_MS_similarity_matrix(string IN){
  MZXML_DIR = IN;
  ELEMENT_COUNT = 0;
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  
  // read which scoring system to use:
  string tmp = def->search_tag("LC/MS sim. score modus");
  SM_cell::SCORE_MODUS = tmp;
  
  delete def;
  def = NULL;
  
}

//////////////////////////////////////////////////
// class desctructor of LC_MS_similarity_matrix
LC_MS_similarity_matrix::~LC_MS_similarity_matrix(){
  ELEMENTS.clear();
  ELEMENT_COUNT = 0;
}

//////////////////////////////////////////////////
// class copy constructor of LC_MS_similarity_matrix
LC_MS_similarity_matrix::LC_MS_similarity_matrix(const LC_MS_similarity_matrix& tmp){
  MZXML_DIR = tmp.MZXML_DIR;
  ELEMENTS = tmp.ELEMENTS;
  ELEMENT_COUNT = tmp.ELEMENT_COUNT;
}

//////////////////////////////////////////////////
// class copy constructor of LC_MS_similarity_matrix
LC_MS_similarity_matrix::LC_MS_similarity_matrix(const LC_MS_similarity_matrix* tmp){
  MZXML_DIR = tmp->MZXML_DIR;
  ELEMENTS = tmp->ELEMENTS;
  ELEMENT_COUNT = tmp->ELEMENT_COUNT;
}

//////////////////////////////////////////////////
// copy constructor:
LC_MS_similarity_matrix& LC_MS_similarity_matrix::operator=(const LC_MS_similarity_matrix& tmp){
  MZXML_DIR = tmp.MZXML_DIR;
  ELEMENTS = tmp.ELEMENTS;
  ELEMENT_COUNT = tmp.ELEMENT_COUNT;
  return *this;
}
    

//////////////////////////////////////////////////
// insert an element into the matrix:
void LC_MS_similarity_matrix::insert_ELEMENT(SM_cell* IN){
  
  // set ID:
  IN->set_ID(ELEMENT_COUNT);
  
  map<int, ROW>::iterator P = ELEMENTS.find( IN->get_X() );
  
  if( P != ELEMENTS.end() ){
    (*P).second.insert(pair<int, SM_cell>(IN->get_Y(), *IN));
  }
  else{
    
    ROW tmp;
    tmp.insert(pair<int, SM_cell>(IN->get_Y(), *IN));
    ELEMENTS.insert(pair<int, ROW>(IN->get_X(), tmp));
  }
  IN = NULL;
  ELEMENT_COUNT++;
}


//////////////////////////////////////////////////
// get an element into the matrix:
SM_cell* LC_MS_similarity_matrix::get_ELEMENT(int x, int y){
  
  map<int, ROW>::iterator P = ELEMENTS.find( x );
  
  if( P != ELEMENTS.end() ){
    
    ROW::iterator Z = (*P).second.find(y);

    if( Z != (*P).second.end() ){      
      return &( (*Z).second);
    }
    else{
      return NULL;
    }
  }
  else{
    
    return NULL;
  }
}




//////////////////////////////////////////////////
// write matrix:
void LC_MS_similarity_matrix::write_Matrix_to_XML(){
  
  char file_name[255];
  sprintf( file_name, "LC_MS_similarity_matrix.xml");
  
  string target_dir = "ANALYSIS_";
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();  
  target_dir += def->search_tag("MY PROJECT NAME");  
  delete def;
  def = NULL;
  //////////////////////////////////

  if( target_dir[ target_dir.size() - 1 ] != '/'){
    target_dir += "/";
  }
  target_dir = target_dir + file_name;
  
  string XSLT_FILE = "LC_MS_SuperQual/display_LC_MS_SuperQual.xsl";
  LC_MS_XML_writer* LC_W = new LC_MS_XML_writer( target_dir );
  LC_W->write_unformatted_XML_MAIN_HEADER(XSLT_FILE);
  ofstream* WRITER = LC_W->get_WRITER();
  char buffer[255];
  
  int nb_cols = ELEMENTS.size();
  map<int, map<int, SM_cell> >::iterator R = get_X_ELEMENT_START();
  int nb_rows = (*R).second.size();
  
  sprintf(buffer, "<SIMILARITY_MATRIX number_rows=\"%d\" number_cols=\"%d\">\n", nb_cols+1, nb_rows+1);  
  WRITER->write(buffer,strlen(buffer));    
    
  sprintf(buffer, "<MATRIX_ELEMENTS>\n");  
  WRITER->write(buffer,strlen(buffer));

    
  map<int, map<int, SM_cell> >::iterator X = get_X_ELEMENT_START();
  while( X != get_X_ELEMENT_END() ){
    
    map<int, SM_cell>::iterator Y = (*X).second.begin();
    while( Y != (*X).second.end()){
      
      SM_cell* ELE = &((*Y).second);
      ELE->write_to_XML( WRITER );
      
      Y++;
    }
    X++;
  }
  
  sprintf(buffer, "</MATRIX_ELEMENTS>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  sprintf(buffer, "</SIMILARITY_MATRIX>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  LC_W->write_XML_MAIN_HEADER_CLOSE();

  delete LC_W;
  LC_W = NULL;
}



///////////////////////////////////////////////////////////////////////////////////////////
// plot the similarity matrix using gnuplot:
void LC_MS_similarity_matrix::plot_matrix_by_gnuplot(){
  
  vector< pair<double, pair<double,double> > > DATA;

  map<int, map<int, SM_cell> >::iterator P = get_X_ELEMENT_START();
  while( P != get_X_ELEMENT_END() ) {
    
    map<int, SM_cell>::iterator Z = (*P).second.begin();
    
    while(Z != (*P).second.end()){
      
      SM_cell* CELL = &((*Z).second);
      double X = CELL->get_X();
      double RES = 0.05;
      double SCORE = CELL->get_OUT_SCORE();
      
      while( X <= (CELL->get_X() + 1.0) ){ 

        double Y = CELL->get_Y();
        while( Y <= ( CELL->get_Y() + 1.0) ){ 
          pair<double,double> tmp( make_pair( double( Y ), SCORE ) );
          DATA.push_back( make_pair( double( X ), tmp ) ); 
          Y += RES;
        }
        X += RES;
      }

      Z++;
    }
    
    P++;
  }
  // PLO->close();
  
  // create a pic:
  char buffer[255];
  sprintf( buffer, "sim_matrix");
  data_plotter* PLOT = new data_plotter(buffer);
  PLOT->set_Z_RANGE(0.0, 1.0);
  PLOT->add_3D_plot_data( &DATA );
  PLOT->plot_THREED_data();
  delete PLOT;
  PLOT = NULL;


}  


///////////////////////////////////////////////////////////////////////////////////////////
// plot the similarity matrix using perl:
void LC_MS_similarity_matrix::plot_matrix_by_perl(){
   
  // check the pathe name:
  string perlPath = checkPerlPath();
  
  if( !perlPath.empty() ) {
    
    string target_dir;    
    read_param* def = new read_param();  
    target_dir = "ANALYSIS_" + def->search_tag("MY PROJECT NAME");  
    delete def;
    def = NULL;
    if( target_dir[ target_dir.size() -1 ] != '/'){
      target_dir += "/";
    }
    string OUTFILE = target_dir + "LC_MS_similarity_matrix.xml";
    
    
    
    // define the output name:
    //string SCRIPT_NAME = "start_M_plot.pl";
    //string BIN_FOLDER = "/bin/";
    
    char buffer_S[256]; 
    char buffer_O[256]; 

    sprintf( buffer_S, "perl ");
    sprintf( buffer_O, "perl ");

    /*
    char* USER = NULL;
    if( getenv("LOGNAME") ){
      USER = getenv("LOGNAME");
      USER += '~';
      if( USER != "~muellelu" ){
        USER = "~muellelu";
      }
      sprintf( buffer_S, "%s%s", buffer_S, USER);
      sprintf( buffer_O, "%s%s", buffer_O, USER);
    }
    else{
      sprintf( buffer_S, "%s/", buffer_S);
      sprintf( buffer_O, "%s/", buffer_O);
    }
     */
    
    //sprintf( buffer_S, "%s%s%s %s 1", buffer_S, BIN_FOLDER.c_str(), SCRIPT_NAME.c_str(), OUTFILE.c_str() );
    //sprintf( buffer_S, "%s %s %s 1", buffer_S, SCRIPT_NAME.c_str(), OUTFILE.c_str());
    sprintf( buffer_S, "%s%s %s 1", buffer_S, perlPath.c_str(), OUTFILE.c_str() );
    system( buffer_S );
    
    //sprintf( buffer_O, "%s %s %s 0", buffer_O, SCRIPT_NAME.c_str(), OUTFILE.c_str() );
    sprintf( buffer_O, "%s%s %s 2", buffer_O, perlPath.c_str(), OUTFILE.c_str() );
    system( buffer_O );
  }
  else{
    printf( "\t -> LC-MS similarity matrix could not be plotted by Path '%s' \n", LC_MS_similarity_matrix::PerlPathName.c_str());
  }
}



///////////////////////////////////////////////////////////////////////
// construct and check the perl script path/Name:
string LC_MS_similarity_matrix::checkPerlPath(){
  
  // define names:
  string SCRIPT_NAME = "start_M_plot.pl";
  SCRIPT_NAME = LC_MS_similarity_matrix::PerlPathName;
  
  
  file_sys tester;
  if( ! tester.check_file_exists( SCRIPT_NAME ) ){
    SCRIPT_NAME = "";
  }
  
  
  /*
  string BIN_FOLDER = "/bin/";

  char buffer_S[256]; 
  char buffer_O[256]; 
  sprintf( buffer_S, "perl ");
  sprintf( buffer_O, "perl ");
  
  char* USER = NULL;
  if( getenv("LOGNAME") ){
    USER = getenv("LOGNAME");
    USER += '~';
    if( USER != "~muellelu" ){
      USER = "~muellelu";
    }
    sprintf( buffer_S, "%s%s", buffer_S, USER);
    sprintf( buffer_O, "%s%s", buffer_O, USER);
  }
  else{
    sprintf( buffer_S, "%s/", buffer_S);
    sprintf( buffer_O, "%s/", buffer_O);
  }
   */
  
  return SCRIPT_NAME;
  

}



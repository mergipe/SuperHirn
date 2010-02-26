///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS data_plotter:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_DATA_PLOTTER
#include "GENERAL_USE.h"
#include <stdio.h>
using namespace std;

string data_plotter::GNU_PLOT_DIR = "gnu_plots/";

////////////////////////////////////////////////
// constructor for the object data_plotter:
data_plotter::data_plotter(){
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  OUT_DIR = def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;  
  
  SCREEN_TEXT = true;
  X_RANGE.set = false;
  Y_RANGE.set = false;
  LEGEND = false;
  POINT_SIZE = 1;
}

////////////////////////////////////////////////
// constructor for the object data_plotter:
data_plotter::data_plotter(bool IN){
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  OUT_DIR = def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;  
  
  out_file = "test";
  
  SCREEN_TEXT = IN;
  X_RANGE.set = false;
  Y_RANGE.set = false;
  LEGEND = false;
  POINT_SIZE = 1;
}

////////////////////////////////////////////////
// constructor for the object data_plotter:
data_plotter::data_plotter(string IN){
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  OUT_DIR = def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;  

  SCREEN_TEXT = true;
  X_RANGE.set = false;
  Y_RANGE.set = false;
  LEGEND = false;
  out_file = IN;
  POINT_SIZE = 1;
}

////////////////////////////////////////////////
// constructor for the object data_plotter:
data_plotter::data_plotter(string IN, bool DIS){
  
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  OUT_DIR = def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;  
  
  SCREEN_TEXT = DIS;
  X_RANGE.set = false;
  Y_RANGE.set = false;
  LEGEND = false;
  out_file = IN;
  POINT_SIZE = 1;
  
}

//////////////////////////////////////////////////
// class desctructor of data_plotter
data_plotter::~data_plotter(){
  // out_file = "my_plot";
  close_plot();
  GNU_PLOT = NULL;
  PLOT_DATA.clear();
}

//////////////////////////////////////////////////
// class copy constructor of data_plotter
data_plotter::data_plotter(const data_plotter& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of data_plotter
data_plotter::data_plotter(const data_plotter* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
data_plotter& data_plotter::operator=(const data_plotter& tmp){
  return *this;
}


//////////////////////////////////////////////////
// create path to the outfile 
void data_plotter::create_plot_file(){
    
  // extract the analysis dircetory:
  // remove backslash at end:
  if(OUT_DIR[OUT_DIR.size()-1] != '/')
    OUT_DIR += "/";
  
  if(OUT_DIR.find("ANALYSIS_") == string::npos){
    OUT_DIR.insert(0,"ANALYSIS_");
  }
  
  if(OUT_DIR.find(GNU_PLOT_DIR) == string::npos){
    OUT_DIR += GNU_PLOT_DIR;
  }
      
  //////////////////////////
  // check if the out_dir is there,
  // otherwise make one:
  file_sys dir_check;
  if( !dir_check.check_directory(OUT_DIR)){
    // create a new one:
    mkdir(OUT_DIR.c_str(),0777);
  }
    
  // add extension for tree format
  plot_file = OUT_DIR + out_file + ".png";
}


//////////////////////////////////////////////////
// create path to the text outfile 
void data_plotter::create_text_file(){
  
  // extract the analysis dircetory:
  // remove backslash at end:
  if(OUT_DIR[OUT_DIR.size()-1] != '/')
    OUT_DIR += "/";
  
  if(OUT_DIR.find("ANALYSIS_") == string::npos){
    OUT_DIR.insert(0,"ANALYSIS_");
  }
  
  if(OUT_DIR.find(GNU_PLOT_DIR) == string::npos){
    OUT_DIR += GNU_PLOT_DIR;
  }
  
  
  //////////////////////////
  // check if the out_dir is there,
  // otherwise make one:
  file_sys dir_check;
  if( !dir_check.check_directory(OUT_DIR)){
    // create a new one:
    mkdir(OUT_DIR.c_str(),0777);
  }
  
  // add extension for tree format
  text_file = OUT_DIR + out_file + ".txt";
}



//////////////////////////////////////////////////
// Open a pipe for gnuplot
bool data_plotter::open_plot(){
  
  // create path to the outfile 
  create_plot_file();
  
  
  GNU_PLOT = popen("gnuplot", "w");
  if( GNU_PLOT == NULL ){
    printf("\n** ERROR, could not open pipe to gnuplot,data_plotter::open_plot **\n");
    return false;
  }
  else{
    int max = 10000000;
    char* pBuffer = new char [max];
    setbuffer( GNU_PLOT , pBuffer , max * sizeof(char) );
    pBuffer = NULL;
    return true;
  }
  
}

//////////////////////////////////////////////////
// close a pipe for gnuplot
void data_plotter::close_plot(){
  
  if( GNU_PLOT != NULL ){ 
    pclose(GNU_PLOT);
    GNU_PLOT = NULL;
  }
  
  PLOT_DATA.clear();
}


//////////////////////////////////////////////////
// initialize the data unit:
void data_plotter::INIT_DATA_UNIT( DATA_UNIT* IN ){
  
  // plotting styples:
  IN->boxes = false;
  IN->dot_lines = false;
  IN->points = false;
  IN->errorbars = false;
  IN->dot = false;
  IN->impulses = false;
  
  // data:
  IN->DATA.clear();
  IN->DATA_STDEV.clear();
  IN->THREED_DATA.clear();
  IN->name.clear();
  IN = NULL;
}

//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data(map<int,double>* IN){
  
  if( !IN->empty() ){
    DATA_UNIT tmp;
    INIT_DATA_UNIT( &tmp );

    map<int,double>::iterator P = IN->begin();
    while( P != IN->end() ){
      tmp.DATA.insert( make_pair( double((*P).first), (*P).second ) );
      P++;
    }
    
    PLOT_DATA.push_back(tmp);
  }
  IN = NULL;
}

//////////////////////////////////////////////////
// add 3D data
void data_plotter::add_3D_plot_data(vector< pair<double, pair<double,double> > >* IN){
  
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.THREED_DATA = *IN;
  PLOT_DATA.push_back(tmp);
  IN = NULL;

}

//////////////////////////////////////////////////
// add data with impulses
void data_plotter::add_plot_data_impulses(map<double, double>* IN, string NAME){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.name = NAME;
  tmp.impulses = true;
  tmp.DATA = *IN;
  PLOT_DATA.push_back(tmp);
  IN = NULL;
}


//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data_points(map<double, double>* IN){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.points = true;
  tmp.DATA = *IN;
  PLOT_DATA.push_back(tmp);
  IN = NULL;
}


//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data_points(map<double,double>* IN, string NAME){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.name = NAME;
  tmp.points = true;
  tmp.DATA = *IN;
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  LEGEND = true;
}


//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data_points(map<int,double>* IN, string NAME){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.name = NAME;
  tmp.points = true;
  
  map<int,double>::iterator P = IN->begin();
  while( P != IN->end() ){
    tmp.DATA.insert( make_pair( double((*P).first), (*P).second ) );
    P++;
  }
  
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  LEGEND = true;
}

//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data_points(map<int,double>* IN, vector<double>* STDEV, string NAME){

  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.name = NAME;
  tmp.DATA_STDEV = (*STDEV);
  tmp.errorbars = true;
  
  map<int,double>::iterator P = IN->begin();
  while( P != IN->end() ){
    tmp.DATA.insert( make_pair( double((*P).first), (*P).second ) );
    P++;
  }
  
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  LEGEND = true;
}


//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data_dots(map<double,double>* IN){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.dot = true;
  tmp.DATA = *IN;
  PLOT_DATA.push_back(tmp);
  IN = NULL;
}
//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data_dots(map<double,double>* IN, string NAME){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.name = NAME;
  tmp.dot = true;
  tmp.DATA = *IN;
  PLOT_DATA.push_back(tmp);
  IN = NULL;
}


//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data_dots(map<int,double>* IN, string NAME){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.name = NAME;
  tmp.dot = true;
  
  map<int,double>::iterator P = IN->begin();
  while( P != IN->end() ){
    tmp.DATA.insert( make_pair( double((*P).first), (*P).second ) );
    P++;
  }
  
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  LEGEND = true;
}


//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data(map<int,double>* IN, vector<double>* STDEV, string NAME){
  
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.name = NAME;
  tmp.DATA_STDEV = (*STDEV);
  tmp.errorbars = true;
  
  map<int,double>::iterator P = IN->begin();
  while( P != IN->end() ){
    tmp.DATA.insert( make_pair( double((*P).first), (*P).second ) );
    P++;
  }
  
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  LEGEND = true;
}

//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data(map<int,double>* IN, vector<double>* STDEV){
  
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.DATA_STDEV = (*STDEV);
  tmp.errorbars = true;
  
  map<int,double>::iterator P = IN->begin();
  while( P != IN->end() ){
    tmp.DATA.insert( make_pair( double((*P).first), (*P).second ) );
    P++;
  }
  
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  // LEGEND = true;
}


//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data(map<double,double>* IN){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.DATA = *IN;
  PLOT_DATA.push_back(tmp);
  IN = NULL;
}

//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data(vector<double>* IN, string NAME){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.name = NAME;
  
  vector<double>::iterator P = IN->begin();
  int ID = 0;
  while( P != IN->end() ){
    tmp.DATA.insert( make_pair(  ID, *P) );    
    P++;
    ID++;
  }
  
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  LEGEND = true;
}

//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data(map<double,double>* IN, string NAME){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.name = NAME;
  tmp.DATA = *IN;
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  LEGEND = true;
}

//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data(map<int,double>* IN, string NAME){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.name = NAME;

  map<int,double>::iterator P = IN->begin();
  while( P != IN->end() ){
    tmp.DATA.insert( make_pair( double((*P).first), (*P).second ) );
    P++;
  }
  
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  LEGEND = true;
}

//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data(map<int,double>* IN, int NAME){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  char N[255];
  sprintf(N, "%d", NAME);
  tmp.name = N;
  
  map<int,double>::iterator P = IN->begin();
  while( P != IN->end() ){
    tmp.DATA.insert( make_pair( double((*P).first), (*P).second ) );
    P++;
  }
  
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  LEGEND = true;
}

//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data(map<int, map<int,double> >* IN){
  map<int, map<int,double> >::iterator P = IN->begin();
  while( P != IN->end() ){
    DATA_UNIT tmp;
    INIT_DATA_UNIT( &tmp );

    map<int,double>::iterator D = (*P).second.begin();
    while( D != (*P).second.end() ){
      tmp.DATA.insert( make_pair( double((*D).first), (*D).second ) );
      D++;
    }
    
    
    PLOT_DATA.push_back(tmp);
    P++;
  }
  IN = NULL;
}

//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data(map<int, map<int,double> >* IN, int NAME){
  map<int, map<int,double> >::iterator P = IN->begin();
  while( P != IN->end() ){
    char N[255];
    sprintf(N, "%d_%d", NAME, (*P).first);
    DATA_UNIT tmp;
    INIT_DATA_UNIT( &tmp );
    tmp.name = N;

    map<int,double>::iterator D = (*P).second.begin();
    while( D != (*P).second.end() ){
      tmp.DATA.insert( make_pair( double((*D).first), (*D).second ) );
      D++;
    }
        
    PLOT_DATA.push_back(tmp);
    P++;
  }
  LEGEND = true;
  IN = NULL;
}

//////////////////////////////////////////////////
// add data
void data_plotter::add_plot_data(map<int, map<int,double> >* IN, string NAME){
  map<int, map<int,double> >::iterator P = IN->begin();
  while( P != IN->end() ){
    char N[255];
    sprintf(N, "%s_%d", NAME.c_str(), (*P).first);
    DATA_UNIT tmp;
    INIT_DATA_UNIT( &tmp );
    tmp.name = N;

    map<int,double>::iterator D = (*P).second.begin();
    while( D != (*P).second.end() ){
      tmp.DATA.insert( make_pair( double((*D).first), (*D).second ) );
      D++;
    }
        
    
    PLOT_DATA.push_back(tmp);
    P++;
  }
  LEGEND = true;
  IN = NULL;
}

//////////////////////////////////////////////////
// add data with dot lines plotted
void data_plotter::add_plot_data_dotlines(map<int,double>* IN, string NAME){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.name = NAME;
  tmp.dot_lines = true;
  
  map<int,double>::iterator P = IN->begin();
  while( P != IN->end() ){
    tmp.DATA.insert( make_pair( double((*P).first), (*P).second ) );
    P++;
  }
  
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  LEGEND = true;
}

//////////////////////////////////////////////////
// add data with dot lines plotted
void data_plotter::add_plot_data_dotlines(map<double,double>* IN, string NAME){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.name = NAME;
  tmp.dot_lines = true;
  tmp.DATA = *IN;
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  LEGEND = true;
}



//////////////////////////////////////////////////
// add data histogram
void data_plotter::add_plot_histogram_data(map<double,double>* IN, string NAME){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.boxes = true;
  tmp.name = NAME;
  tmp.DATA = *IN;
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  LEGEND = true;
}

//////////////////////////////////////////////////
// add data histogram
void data_plotter::add_plot_histogram_data(map<double,double>* IN){
  DATA_UNIT tmp;
  INIT_DATA_UNIT( &tmp );
  tmp.boxes = true;
  tmp.DATA = *IN;
  PLOT_DATA.push_back(tmp);
  IN = NULL;
  // LEGEND = true;
}


//////////////////////////////////////////////////
// find the max Y value
void data_plotter::define_MAX_Y(){
  
  // open the plot
  if( !PLOT_DATA.empty() ){
    double Y_MAX = -1000000;
    
    vector<DATA_UNIT>::iterator P = PLOT_DATA.begin();
    while( P != PLOT_DATA.end() ){
      
      // plot this data set:
      map<double,double>::iterator D = (*P).DATA.begin();
      vector<double>::iterator S = (*P).DATA_STDEV.begin();
      while( D != (*P).DATA.end() ){

        
        if( Y_MAX < (*D).second ){
          Y_MAX = (*D).second;
        }   
        
        D++;
      }
      P++;
    }    
    set_Y_RANGE( 0.0, Y_MAX ); 
  }
}

//////////////////////////////////////////////////
// write data to gnuplot
void data_plotter::plot_TWOD_data(){

   // open the plot
  if( open_plot() && !PLOT_DATA.empty()){
        
   // outline the plot
    outline_TWOD_plot();
    
    vector<DATA_UNIT>::iterator P = PLOT_DATA.begin();
    fprintf( GNU_PLOT , "plot '-' title " );
    if( (*P).name.empty() ){
      fprintf( GNU_PLOT , "'all'" );      
    }
    else{
      fprintf( GNU_PLOT , "'%s'", (*P).name.c_str() );
    }
    
    if( ( (*P).boxes )){        
      fprintf( GNU_PLOT , " with boxes" );
    }
    else if( ( (*P).dot_lines ) ){        
      fprintf( GNU_PLOT , " with linespoints" );
    }
    else if( ( (*P).points ) ){        
      fprintf( GNU_PLOT , " with points" );
    }
    else if( ( (*P).errorbars ) ){        
      fprintf( GNU_PLOT , " with yerrorbars" );
    }
    else if( ( (*P).dot ) ){        
      fprintf( GNU_PLOT , " with dots" );
    }
    else if( ( (*P).impulses ) ){        
      fprintf( GNU_PLOT , " with impulses" );
    }
    else{
      fprintf( GNU_PLOT , " with lines" );
    }
    
    P++;
    while( P != PLOT_DATA.end() ){
     
      fprintf( GNU_PLOT , ", '-'");
      if( !(*P).name.empty() ){
       fprintf( GNU_PLOT , " title '%s'", (*P).name.c_str() );
      }
      
      if( ( (*P).boxes ) ){        
        fprintf( GNU_PLOT , " with boxes" );
      }
      else if( ( (*P).points ) ){        
        fprintf( GNU_PLOT , " with points" );
      }
      else if( ( (*P).errorbars ) ){        
        fprintf( GNU_PLOT , " with yerrorbars" );
      }
      else if( ( (*P).dot_lines ) ){        
        fprintf( GNU_PLOT , " with linespoints" );
      }
      else if( ( (*P).dot ) ){        
        fprintf( GNU_PLOT , " with dots" );
      }
      else if( ( (*P).impulses ) ){        
        fprintf( GNU_PLOT , " with impulses" );
      }      
      else{
        fprintf( GNU_PLOT , " with lines" );
      }
      P++;
    }
    fprintf( GNU_PLOT , "\n" );
    
    
    // plot all the data in teh data pool:
    P = PLOT_DATA.begin();
    while( P != PLOT_DATA.end() ){
      
      // plot this data set:
      map<double,double>::iterator D = (*P).DATA.begin();
      vector<double>::iterator S = (*P).DATA_STDEV.begin();
      while( D != (*P).DATA.end() ){

        if( S != (*P).DATA_STDEV.end() ){
          fprintf( GNU_PLOT , "%f %0.2f %f\n" , (*D).first , (*D).second, (*S) );
          S++;
        }
        else{
          fprintf( GNU_PLOT , "%f %0.2f\n" , (*D).first , (*D).second );
        }   
        
        D++;
      }
      // Send end of data set
      fprintf( GNU_PLOT , "e\n" );    
      
      P++;
    }
    
    if( SCREEN_TEXT )
      printf( "\t\t- GnuPlot file written to \"%s\"\n" , plot_file.c_str() );
    
    // close the output:
    close_plot();
  
  }
}

//////////////////////////////////////////////////
// outline the plot of gnuplot
void data_plotter::outline_TWOD_plot(){
  
  // set the terminal:
  fprintf( GNU_PLOT , "set terminal png\n" , plot_file.c_str() );
  
  // set scaling of the axes:
  if( X_RANGE.set ){
    fprintf( GNU_PLOT , "set xrange[%f:%f]\n", X_RANGE.start, X_RANGE.end);
  }
  if( Y_RANGE.set ){
    fprintf( GNU_PLOT , "set yrange[%f:%f]\n", Y_RANGE.start, Y_RANGE.end);    
  }
  if( !(X_RANGE.set) && !(Y_RANGE.set) ){
    fprintf( GNU_PLOT , "set autoscale\n");    
  }
  
  // set the axes:
  fprintf( GNU_PLOT , "set xtics\n" );
  fprintf( GNU_PLOT , "set ytics\n" );
  fprintf( GNU_PLOT , "set xtics nomirror\n" );
  fprintf( GNU_PLOT , "set ytics nomirror\n" );

  // set titel
  fprintf( GNU_PLOT , "set title '%s(%d)'\n", out_file.c_str(), PLOT_DATA.size());
  fprintf( GNU_PLOT , "set border 1\n" );
  fprintf( GNU_PLOT , "set nolabel\n" );
  
  // set point size:
  fprintf( GNU_PLOT , "set pointsize %d\n", POINT_SIZE );

  // set legend:
  if( !LEGEND ){
    fprintf( GNU_PLOT , "unset key\n" );
  }
  
  // set the output file:
  fprintf( GNU_PLOT , "set output \"%s\"\n" , plot_file.c_str() );
    
}

//////////////////////////////////////////////////
// write data to gnuplot
void data_plotter::plot_THREED_data(){
  
  // open the plot
  if( open_plot() && !PLOT_DATA.empty()){
    
    // outline the plot
    outline_THREED_plot();

    vector<DATA_UNIT>::iterator P = PLOT_DATA.begin();
    fprintf( GNU_PLOT , "splot '-'\n" );

    // plot all the data in the data pool:
    P = PLOT_DATA.begin();
    while( P != PLOT_DATA.end() ){
      
      // plot this data set:
      vector< pair<double, pair<double,double> > >::iterator D = (*P).THREED_DATA.begin();
      while( D != (*P).THREED_DATA.end() ){
        fprintf( GNU_PLOT , "%f %f %f using %f \n" , (*D).first , (*D).second.first, (*D).second.second, (*D).second.second );
        D++;
      }
      // Send end of data set
      fprintf( GNU_PLOT , "e\n" );    
      
      P++;
    }
    
    if( SCREEN_TEXT )
      printf( "\t\t- GnuPlot file written to \"%s\"\n" , plot_file.c_str() );
    
    close_plot();
  }
}

//////////////////////////////////////////////////
// outline the plot of gnuplot
void data_plotter::outline_THREED_plot(){
  
  // set the terminal:
  fprintf( GNU_PLOT , "set terminal png\n" , plot_file.c_str() );
  
  // set scaling of the axes:
  if( X_RANGE.set ){
    fprintf( GNU_PLOT , "set xrange[%f:%f]\n", X_RANGE.start, X_RANGE.end);
  }
  if( Y_RANGE.set ){
    fprintf( GNU_PLOT , "set yrange[%f:%f]\n", Y_RANGE.start, Y_RANGE.end);    
  }
  if( Y_RANGE.set ){
    fprintf( GNU_PLOT , "set zrange[%f:%f]\n", Z_RANGE.start, Z_RANGE.end);
  }
  if( !(X_RANGE.set) && !(Y_RANGE.set) && !(Z_RANGE.set) ){
    fprintf( GNU_PLOT , "set autoscale\n");    
  }
    
  // set titel
  fprintf( GNU_PLOT , "set title '%s'\n", out_file.c_str());
  fprintf( GNU_PLOT , "set border 4095\n" );
  
  // set legend:
  if(!LEGEND){
    fprintf( GNU_PLOT , "unset key\n" );
  }
  // set the output file:
  fprintf( GNU_PLOT , "set output \"%s\"\n" , plot_file.c_str() );

  fprintf( GNU_PLOT , "set pm3d map explicit\n");
  fprintf( GNU_PLOT , "set surface\n");
  fprintf( GNU_PLOT , "set sample 25\n");
  fprintf( GNU_PLOT , "set isosample 20\n");
  //fprintf( GNU_PLOT , "set contour\n");
  
}


//////////////////////////////////////////////////
// print the data from gnuplot to txt file
void data_plotter::print_TWOD_data_to_TXT(){
  
  // create the text outfile:
  create_text_file();
  
  ofstream* WRITER = new ofstream();
  WRITER->open(text_file.c_str(), ofstream::out); 
  if( WRITER->good() ){
    
    string SEP = "\t";
    char buffer[255];
    
    // plot all the data in the data pool:
    vector<DATA_UNIT>::iterator P = PLOT_DATA.begin();
    while( P != PLOT_DATA.end() ){
      // plot this data set:
      map<double,double>::iterator D = (*P).DATA.begin();
      vector<double>::iterator S = (*P).DATA_STDEV.begin();
      while( D != (*P).DATA.end() ){
      
        if( S != (*P).DATA_STDEV.end() ){
          sprintf( buffer , "%0.4f%s%0.4f%s%0.4f\n" , (*D).first, SEP.c_str(), (*D).second, SEP.c_str(), (*S) );
          S++;
        }
        else{
          sprintf( buffer , "%0.4f%s%0.4f\n" , (*D).first, SEP.c_str(), (*D).second );
         }   
        WRITER->write(buffer,strlen(buffer));
        D++;
      }

      sprintf( buffer , "\nNEXT\n\n" );
      WRITER->write(buffer,strlen(buffer));
      
      P++;
    }
    
    WRITER->close();
  }
  
  delete WRITER;
  WRITER = NULL;
}

//////////////////////////////////////////////////
// print the data from gnuplot to txt file in "matrix" format
void data_plotter::print_MATRIX_data_to_TXT(){
  
  // open a file for writing:
  string text_file = plot_file;
  text_file = text_file.erase( text_file.find("."), text_file.size() - text_file.find(".") ); 
  text_file = text_file + ".txt";
  
  ofstream* WRITER = new ofstream();
  WRITER->open(text_file.c_str(), ofstream::out); 
  if( WRITER->good() ){
    
    string SEP = "\t";
    char buffer[255];
    // plot all the data in the data pool:
    vector<DATA_UNIT>::iterator C = PLOT_DATA.begin();
    map<double,double> X_POINTS = (*C).DATA;
    map<double,double>::iterator ID_C = X_POINTS.begin();
    
    while( ID_C != X_POINTS.end() ){

      sprintf( buffer , "%0.3f%s", (*ID_C).first, SEP.c_str() );
      WRITER->write(buffer,strlen(buffer));
    
      vector<DATA_UNIT>::iterator P = PLOT_DATA.begin();      
      while( P != PLOT_DATA.end() ){
        // plot this data set:
        map<double,double>::iterator D = (*P).DATA.find( (*ID_C).first );
        if( D != (*P).DATA.end() ){
          sprintf( buffer , "%0.4f%s" , (*D).second, SEP.c_str() );
          WRITER->write(buffer,strlen(buffer));
        }        
        P++;
      }
      
      sprintf( buffer , "\n" );
      WRITER->write(buffer,strlen(buffer));
      ID_C++;
    }
    
    WRITER->close();
  }
  
  delete WRITER;
  WRITER = NULL;
}



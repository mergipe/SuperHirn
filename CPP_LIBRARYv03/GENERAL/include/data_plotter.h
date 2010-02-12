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


#ifndef DATA_PLOTTER_H
#define DATA_PLOTTER_H


typedef struct{
  
  map<double, double> DATA;
  vector<double> DATA_STDEV;
  vector< pair<double, pair<double,double> > > THREED_DATA;
  string name;
  bool boxes;
  bool dot_lines;
  bool dot;
  bool points;
  bool errorbars;
  bool impulses;
}
DATA_UNIT;

typedef struct{
  double start;
  double end;
  bool set;
}
RANGE;

class data_plotter{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
private:
  
  string OUT_DIR;
  string out_file;
  string plot_file;
  string text_file;
  
  bool SCREEN_TEXT;
  
  FILE* GNU_PLOT;
  
  RANGE X_RANGE;
  RANGE Y_RANGE;
  RANGE Z_RANGE;

  
  int POINT_SIZE;
  
  bool LEGEND;
  // vector holding the data to be plotted:
  vector<DATA_UNIT> PLOT_DATA;
  ////////////////////////////////////////////////
  // declaration of the public members:
public:
  
    static string GNU_PLOT_DIR;

    
  // class destructor
  ~data_plotter();
  
  // class constructor
  data_plotter();
  data_plotter(bool);
  data_plotter(string);
  data_plotter(string, bool);
  // class copy constructor
  data_plotter(const data_plotter&);
  // class copy constructor
  data_plotter(const data_plotter*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  data_plotter& operator=(const data_plotter&);
  bool operator==(const data_plotter&);
  data_plotter& operator<=(const data_plotter&);
  data_plotter& operator>=(const data_plotter&);
  data_plotter& operator<(const data_plotter&);
  data_plotter& operator>(const data_plotter&);
  
  
  // create path to the outfile 
  void create_plot_file();
  // create path to the text outfile 
  void create_text_file();

  // Open a pipe for gnuplot
  bool open_plot();
  // close a pipe for gnuplot
  void close_plot();
  // initialize the data unit:
  void INIT_DATA_UNIT( DATA_UNIT*);

  
  
  // write data to gnuplot
  void plot_TWOD_data();
  // outline the plot of gnuplot
  void outline_TWOD_plot();
  // add data
  void add_plot_data(map<int,double>*);
  void add_plot_data(map<double,double>*);
  void add_plot_data(map<double,double>*, string);
  void add_plot_data(map<int,double>*,string);
  void add_plot_data(map<int,double>*, int);
  void add_plot_data(vector<double>*,string);
  void add_plot_data(map<int, map<int,double> >*);
  void add_plot_data(map<int, map<int,double> >*, int);
  void add_plot_data(map<int, map<int,double> >*, string);
  void add_plot_data(map<int,double>*, vector<double>* , string);
  void add_plot_data(map<int,double>*, vector<double>* );
  void add_plot_data_dots(map<double,double>*);
  void add_plot_data_dots(map<double,double>*, string);
  void add_plot_data_dots(map<int,double>*, string );
  
  // add data
  void add_plot_data_points(map<double,double>*, string);
  void add_plot_data_points(map<double,double>*);
  void add_plot_data_points(map<int,double>*, string);
  void add_plot_data_points(map<int,double>*, vector<double>*, string);

  
  // add data with impulses
  void add_plot_data_impulses(map<double, double>* , string);
  
  // add data with dot lines plotted
  void add_plot_data_dotlines(map<int,double>* , string);
  void add_plot_data_dotlines(map<double,double>* , string);

  // find the max Y value
  void define_MAX_Y();

  // print the data from gnuplot to txt file
  void print_TWOD_data_to_TXT();
  // print the data from gnuplot to txt file in "matrix" format
  void print_MATRIX_data_to_TXT();

  // add data histogram
  void add_plot_histogram_data(map<double,double>*, string);
  void add_plot_histogram_data(map<double,double>*);

  
  
  // add 3D data
  void plot_THREED_data();
  // outline the plot of gnuplot
  void outline_THREED_plot();
  void add_3D_plot_data(vector< pair<double, pair<double,double> > >*);

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  void set_out_file(string IN){out_file = IN;};
  string get_out_file(){return out_file;};

  void set_X_RANGE(double A, double B){ X_RANGE.start = A; X_RANGE.end = B; X_RANGE.set = true;};
  void set_Y_RANGE(double A, double B){ Y_RANGE.start = A; Y_RANGE.end = B; Y_RANGE.set = true;};
  void set_Z_RANGE(double A, double B){ Z_RANGE.start = A; Z_RANGE.end = B; Z_RANGE.set = true;};
  
  void set_POINT_SIZE( int IN ){ POINT_SIZE = IN;};
  void screen_text_off(){ SCREEN_TEXT = false;};
  
};


#endif

    

///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS MASTER_LC_MS_intensity_normalizer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_COEFF_COMPUTER
#define USE_MASTER_LC_MS_INTENSITY_NORMALIZER

#include "LC_MS_TOOLS_USE.h"

bool MASTER_LC_MS_intensity_normalizer::ABSOLUTE_NORMALIZATION_MODE = false;


//////////////////////////////////////////////////////
// normalization method to use:
// 1: normalization by tree structure
// 2: normalization by input intensity profiles
int MASTER_LC_MS_intensity_normalizer::NORMALIZATION_METHODE = 1;


////////////////////////////////////////////////
// constructor for the object MASTER_LC_MS_intensity_normalizer:
MASTER_LC_MS_intensity_normalizer::MASTER_LC_MS_intensity_normalizer(){  
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  
  // slide window over TR
  def->search_tag("intensity coeff. window", &TR_SLIDE_WINDOW);
  // TR step
  def->search_tag("retention time step", &TR_STEP);
  // TR min
  def->search_tag("RT start elution window", &TR_MIN);
  // TR max
  def->search_tag("RT end elution window", &TR_MAX);
  
  // slide window over TR
  def->search_tag("intensity coeff. window", &normalization_coeff_computer::TR_SLIDE_WINDOW);
  
  // TR step
  def->search_tag("retention time step", &normalization_coeff_computer::TR_STEP);
    
  // feature align percentage:
  def->search_tag("normalization feature align percentage", &normalization_coeff_computer::FEATURE_ALIGN_PERCENTAGE);
  
  // plotting:
  // print background correction profiles to screen
  def->search_tag("gnuplot plot generator", &PROFILE_VIEW);
  ///////////////////////////////////
  
  delete def;
  def = NULL;
  
}


//////////////////////////////////////////////////
// class desctructor of MASTER_LC_MS_intensity_normalizer
MASTER_LC_MS_intensity_normalizer::~MASTER_LC_MS_intensity_normalizer(){
  RELATIVE_VARIATIONS.clear();
  ABSOLUTE_VARIATIONS.clear();
}

//////////////////////////////////////////////////
// class copy constructor of MASTER_LC_MS_intensity_normalizer
MASTER_LC_MS_intensity_normalizer::MASTER_LC_MS_intensity_normalizer(const MASTER_LC_MS_intensity_normalizer& tmp){
}

//////////////////////////////////////////////////
// copy constructor:
MASTER_LC_MS_intensity_normalizer& MASTER_LC_MS_intensity_normalizer::operator=(const MASTER_LC_MS_intensity_normalizer& tmp){
  return *this;
}
    

//**************************************************************
// LC/MS RUN NORMALIZATION FUNCTIONS
// NORMALIZE INTENSITY VARIATION ACROSS SAMPLES:
// compute variation profiles along TR for every run:
// and apply normalization to features for each input MASTER run:
void MASTER_LC_MS_intensity_normalizer::start_GLOBAL_INTENSITY_NORMALIZATION(){
  
  
  // check which normalization method to use:
  switch( NORMALIZATION_METHODE ){
    
    case 1:
      // compute_segment_INTENS_coefficient();
      start_TREE_STRUCTURED_INTENSITY_NORMALIZATION( );
      break;
      
    case 2:
      // start teh intensity normalization by an input profile:
      start_INPUT_PROFILE_INTENSITY_NORMALIZATION( );
      break;
      
    default:
      start_TREE_STRUCTURED_INTENSITY_NORMALIZATION( );

  }
  
  
}




//////////////
//
void MASTER_LC_MS_intensity_normalizer::start_GLOBAL_INTENSITY_NORMALIZATION(LC_MS* IN){
  
  MASTER_RUN = IN;
  
  if( IN->get_nb_raw_specs() > 1 ){
    start_GLOBAL_INTENSITY_NORMALIZATION();
  }
  else{
    printf("\t\t\tOnly 1 raw LC/MS run in MasterMap, skip normalization.\n");
  }
}




////////////////////////////////////////////////////////
// compute intensity correlation coefficint for every segment
void MASTER_LC_MS_intensity_normalizer::compute_segment_INTENS_coefficient(){
  
  string MODE;
  if( ABSOLUTE_NORMALIZATION_MODE ){
    MODE = "ABSOLUTE";
  }
  else{
    MODE = "RELATIVE";
  }
  printf("\t\t- compute %s segment normalization coefficient for '%s' [ID=%d]\n", MODE.c_str(), MASTER_RUN->get_spec_name().c_str(), MASTER_RUN->get_MASTER_ID());
  RELATIVE_VARIATIONS.clear();
  ABSOLUTE_VARIATIONS.clear();
  // map containing coefficient for a spectrum:
  map<int, map<double, double > >::iterator VAR;
  normalization_coeff_computer* COMP = new normalization_coeff_computer( MASTER_RUN );
  
  // set here which 
  
  double TR = TR_MIN;
  while(TR <= ( TR_MAX + TR_STEP) ){
  
    // compute the coefficient for this time segment in 
    // every original LC/MS run:
    map<int, double> SEG_COEFF = COMP->compute_global_segment_coefficient(TR);
    map<int, double> ABS_SEG_COEFF = COMP->get_absolute_coefficients();
    
    //////////////////////////////////
    // copy the relative coefficients:
    map<int, double>::iterator P = SEG_COEFF.begin();
    while( P != SEG_COEFF.end() ){
      VAR = RELATIVE_VARIATIONS.find( (*P).first);
      if( VAR != RELATIVE_VARIATIONS.end() ){
        (*VAR).second.insert(pair<double,double>(TR,(*P).second));
      }
      else{
        map<double,double> tmp;
        tmp.insert(pair<double,double>(TR,(*P).second));
        RELATIVE_VARIATIONS.insert(pair<int, map<double, double> >((*P).first, tmp));
      }

      P++;
    }

    //////////////////////////////////
    // copy the absolute coefficients:
    P = ABS_SEG_COEFF.begin();
    while( P != ABS_SEG_COEFF.end() ){
      VAR = ABSOLUTE_VARIATIONS.find( (*P).first);
      if( VAR != ABSOLUTE_VARIATIONS.end() ){
        (*VAR).second.insert(pair<double,double>(TR,(*P).second));
      }
      else{
        map<double,double> tmp;
        tmp.insert(pair<double,double>(TR,(*P).second));
        ABSOLUTE_VARIATIONS.insert(pair<int, map<double, double> >((*P).first, tmp));
      }
      P++; 
    }
  
    TR += TR_STEP;
  }
    
  // print them:
  if(PROFILE_VIEW){
    // print_applied_normcoefficient();
    print_absolute_variation_profiles( );
    print_relative_variation_profiles( );
  }
  
  // normalize
  normalize_feature_intensities();

}



////////////////////////////////////////////////////////
// compute intensity correlation coefficient for every segment
// according to the alignment tree
void MASTER_LC_MS_intensity_normalizer::start_TREE_STRUCTURED_INTENSITY_NORMALIZATION( ){
  
  
  // read first the alignment tree:
  LC_MS_alignment_tree_parser* TREE_PARSE = new LC_MS_alignment_tree_parser();
  string MODE;
  if( ABSOLUTE_NORMALIZATION_MODE ){
    MODE = "ABSOLUTE";
  }
  else{
    MODE = "RELATIVE";
  }
  printf("\t\t- start TREE STRUCTURED LC/MS %s segment normalization for '%s'\n", MODE.c_str(), MASTER_RUN->get_spec_name().c_str() );
  
  
  /////////////
  // go here through the alignment tree and use selectively only these LC/MS for normalization
  map< int, vector<int> > LC_MS_TREE_NODES; 
  map< int, vector<int> >::iterator F; 
  int node_counter = MASTER_RUN->get_nb_raw_specs();
  while( !TREE_PARSE->check_END_OF_TREE() ){
    
    pair<int, int> TMP = TREE_PARSE->get_alignment_pair();
    
    vector<int> CURRENT_LC_MS_RUNS_A;
    F = LC_MS_TREE_NODES.find( TMP.first );
    if( F == LC_MS_TREE_NODES.end()){
      CURRENT_LC_MS_RUNS_A.push_back( TMP.first );
    }
    else{
      CURRENT_LC_MS_RUNS_A.insert( CURRENT_LC_MS_RUNS_A.begin(), (*F).second.begin(), (*F).second.end() );
    }
    
    vector<int> CURRENT_LC_MS_RUNS_B;
    F = LC_MS_TREE_NODES.find( TMP.second );
    if( F == LC_MS_TREE_NODES.end()){
      CURRENT_LC_MS_RUNS_B.push_back( TMP.second );
    }
    else{
      CURRENT_LC_MS_RUNS_B.insert( CURRENT_LC_MS_RUNS_B.begin(), (*F).second.begin(), (*F).second.end() );
    }
    
    vector<int> CURRENT_LC_MS_RUNS_ALL;
    CURRENT_LC_MS_RUNS_ALL.insert( CURRENT_LC_MS_RUNS_ALL.begin(), CURRENT_LC_MS_RUNS_A.begin(), CURRENT_LC_MS_RUNS_A.end() );
    CURRENT_LC_MS_RUNS_ALL.insert( CURRENT_LC_MS_RUNS_ALL.begin(), CURRENT_LC_MS_RUNS_B.begin(), CURRENT_LC_MS_RUNS_B.end() );
    LC_MS_TREE_NODES.insert( make_pair( node_counter, CURRENT_LC_MS_RUNS_ALL ) );
    node_counter++;
    
    // print screen overview:
    printf("\t\t\t\t- normalizing of LC/MS runs ");

    printf("[");
    vector<int>::iterator C = CURRENT_LC_MS_RUNS_A.begin();
    while( C != CURRENT_LC_MS_RUNS_A.end() ){
      printf("%d ", *C);      
      C++; 
    }
    printf("] vs [");
    C = CURRENT_LC_MS_RUNS_B.begin();
    while( C != CURRENT_LC_MS_RUNS_B.end() ){
      printf("%d ", *C);      
      C++; 
    }
    printf("]\n");
    
    
    // set the LC/MS selection for the coefficient computer
    normalization_coeff_computer::LC_MS_SELECTION_A = CURRENT_LC_MS_RUNS_A;
    normalization_coeff_computer::LC_MS_SELECTION_B = CURRENT_LC_MS_RUNS_B;
    
    // compute the coefficients
    compute_segment_selected_LC_MS_INTENS_coefficient();
    
    // normalize
    normalize_feature_intensities();
    
    // update the alignment tree
    TREE_PARSE->update_alignment_tree();
 
    // print them:
    if(PROFILE_VIEW){
      if( ABSOLUTE_NORMALIZATION_MODE ){
        print_absolute_variation_profiles( );
      }
      else{
        print_relative_variation_profiles( );
      }    
    }
    
  }
  
  
  delete TREE_PARSE;
  TREE_PARSE = NULL;
}



//////////////////////////////////////////////////////////////
// compute intensity correlation coefficient for every segment
// but only for the current LC/MS runs according to the 
// alignment tree
void MASTER_LC_MS_intensity_normalizer::compute_segment_selected_LC_MS_INTENS_coefficient(){
  
  RELATIVE_VARIATIONS.clear();
  ABSOLUTE_VARIATIONS.clear();
  
  // map containing coefficient for a spectrum:
  map<int, map<double, double > >::iterator VAR;
  normalization_coeff_computer* COMP = new normalization_coeff_computer( MASTER_RUN );
  
  // set here which 
  double TR = TR_MIN;
  while(TR <= ( TR_MAX + TR_STEP) ){
    
    // compute the coefficient for this time segment in 
  // every original LC/MS run:
    map<int, double> SEG_COEFF = COMP->compute_global_segment_coefficient(TR);
    map<int, double> ABS_SEG_COEFF = COMP->get_absolute_coefficients();
    
    //////////////////////////////////
    // copy the relative coefficients:
    map<int, double>::iterator P = SEG_COEFF.begin();
    while( P != SEG_COEFF.end() ){
      VAR = RELATIVE_VARIATIONS.find( (*P).first);
      if( VAR != RELATIVE_VARIATIONS.end() ){
        (*VAR).second.insert(pair<double,double>(TR,(*P).second));
      }
      else{
        map<double,double> tmp;
        tmp.insert(pair<double,double>(TR,(*P).second));
        RELATIVE_VARIATIONS.insert(pair<int, map<double, double> >((*P).first, tmp));
      }
      
    P++;
    }
    
    //////////////////////////////////
    // copy the absolute coefficients:
    P = ABS_SEG_COEFF.begin();
    while( P != ABS_SEG_COEFF.end() ){
      VAR = ABSOLUTE_VARIATIONS.find( (*P).first);
      if( VAR != ABSOLUTE_VARIATIONS.end() ){
      (*VAR).second.insert(pair<double,double>(TR,(*P).second));
      }
      else{
        map<double,double> tmp;
        tmp.insert(pair<double,double>(TR,(*P).second));
        ABSOLUTE_VARIATIONS.insert(pair<int, map<double, double> >((*P).first, tmp));
      }
      P++; 
    }
    
    TR += TR_STEP;
  }
}



//////////////////////////////////////////////////////////////
// applies the computed variations to the features:
void MASTER_LC_MS_intensity_normalizer::normalize_feature_intensities( ){
  
  //  printf("\t\t- normalize features by computed segment normalization of '%s' [ID=%d]\n", MASTER_RUN->get_spec_name().c_str(), MASTER_RUN->get_MASTER_ID());
  
  feature* PEAK = NULL;
  vector<LC_MS_FEATURE>::iterator FEA = MASTER_RUN->get_feature_list_begin();
  while( FEA != MASTER_RUN->get_feature_list_end() ){
    
    // adjust the intensities:
    
    // do for the feature:
    PEAK = &(*FEA);
    adjust_intensity_of_feature(PEAK);
    
    // do for its matched features:
    map<int,feature>::iterator P = PEAK->get_match_list_start();
    feature* SIS_PEAK = NULL;
    while( P != PEAK->get_match_list_end()){
      SIS_PEAK = &( (*P).second);
      adjust_intensity_of_feature(SIS_PEAK);      
      P++;
    }
    
    // next feature:
    FEA++;
  }
}

//////////////////////////////////////////////////////////////
// adjust teh intensity of a feature:
void MASTER_LC_MS_intensity_normalizer::adjust_intensity_of_feature(feature* IN){
  
  double TR = IN->get_retention_time();
  int ID = IN->get_spectrum_ID();
  double factor = 0;  
  
  //////////////////////////////////////////////
  // decide here if the relative of absolute
  // coefficients to be used:
  map<int, map<double, double> >::iterator START;
  map<int, map<double, double> >::iterator END;
  
  if( ABSOLUTE_NORMALIZATION_MODE ){
    START = ABSOLUTE_VARIATIONS.find(ID);
    END = ABSOLUTE_VARIATIONS.end();
  }
  else{
    START = RELATIVE_VARIATIONS.find(ID);
    END = RELATIVE_VARIATIONS.end();
  }
  
  
   if( START != END ){
    
    map<double, double>::iterator T = (*START).second.lower_bound(TR);
    
    // exact match
    if( (*T).first == IN->get_retention_time() ){
      factor = (*T).second;
    }
    // not found and at end
    else if ( T == (*START).second.end() ){
      T--;
      factor = (*T).second;
    }
    // smaller than begining:
    else if( T == (*START).second.begin()){
      factor = (*T).second;
    }
    // in middle of list:
    else{
      double UP = (*T).second;
      double T_UP = (*T).first;
      T--;
      double DOWN = (*T).second;
      double T_DOWN = (*T).first;
      factor = DOWN;
      factor += ((UP-DOWN)*(T_UP - TR )) / (T_UP-T_DOWN); 
      
    }
    
    factor = 1.0 / factor;
    // adjust now the intensity of the peak
    IN->normalize_peak_area_by_factor( factor );
  }
}


//////////////////////////////////////////////////////////////
// print the variation profiles:
void MASTER_LC_MS_intensity_normalizer::print_relative_variation_profiles(){

  char A[255],B[255];
  char outfile[255];
  sprintf( outfile, "relative_normalization_coeff");
  
  if( !normalization_coeff_computer::LC_MS_SELECTION_A.empty() ){
    sprintf(A, "");      
    // add the selected LC/MS id to it
    vector<int>::iterator C = normalization_coeff_computer::LC_MS_SELECTION_A.begin();
    while( C != normalization_coeff_computer::LC_MS_SELECTION_A.end() ){
      sprintf(A, "%s_%d", A, *C);      
      C++; 
    }
  }
  if( !normalization_coeff_computer::LC_MS_SELECTION_B.empty() ){
    sprintf(B, "");      
    // add the selected LC/MS id to it
    vector<int>::iterator C = normalization_coeff_computer::LC_MS_SELECTION_B.begin();
    while( C != normalization_coeff_computer::LC_MS_SELECTION_B.end() ){
      sprintf(B, "%s_%d", B, *C);      
      C++; 
    }
  }

  
  sprintf(outfile, "%s%s_to%s", outfile, A, B);        
  data_plotter* PLOT = new data_plotter(outfile, false);
  
  if( !normalization_coeff_computer::LC_MS_SELECTION_A.empty() ){
    vector<int>::iterator C = normalization_coeff_computer::LC_MS_SELECTION_A.begin();
    map<int, map<double,double> >::iterator RUN = RELATIVE_VARIATIONS.find( *C );
    PLOT->add_plot_data( &(*RUN).second, A);    
  }

  if( !normalization_coeff_computer::LC_MS_SELECTION_B.empty() ){
    vector<int>::iterator C = normalization_coeff_computer::LC_MS_SELECTION_B.begin();
    map<int, map<double,double> >::iterator RUN = RELATIVE_VARIATIONS.find( *C );
    PLOT->add_plot_data( &(*RUN).second, B);    
  }
  
  
  PLOT->set_Y_RANGE( 0.6, 1.4);
  PLOT->plot_TWOD_data();
  delete PLOT;
  PLOT = NULL;

}



//////////////////////////////////////////////////////////////
// print the absolute variation profiles:
void MASTER_LC_MS_intensity_normalizer::print_absolute_variation_profiles( ){
  /*
  char outfile[255];
  sprintf( outfile, "absolute_normalization_coeff");
  if( !normalization_coeff_computer::LC_MS_SELECTION.empty() ){
    // add the selected LC/MS id to it
    vector<int>::iterator C = normalization_coeff_computer::LC_MS_SELECTION.begin();
    while( C != normalization_coeff_computer::LC_MS_SELECTION.end() ){
      sprintf(outfile, "%s_%d", outfile, *C);      
      C++; 
    }
  }
    
  string tmp = outfile;
  data_plotter* PLOT = new data_plotter(tmp, false);
  
  map<int, map<double,double> >::iterator RUN = ABSOLUTE_VARIATIONS.begin();
  while( RUN != ABSOLUTE_VARIATIONS.end()  ){
    char buffer[255];
    sprintf( buffer, "%s [%d]", MASTER_RUN->get_raw_spec_name( (*RUN).first ).c_str(), (*RUN).first );    
    PLOT->add_plot_data( &(*RUN).second, buffer);
    RUN++;
  }
  
  PLOT->plot_TWOD_data();
  delete PLOT;
  PLOT = NULL;
   */
}


//////////////////////////////////////////////////////////////
// print the variation profiles:
void MASTER_LC_MS_intensity_normalizer::print_applied_normcoefficient(){
  
  string SEP = "\t";
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  // slide window over TR
  string outfile = "ANALYSIS_";
  outfile = outfile + def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;
  
  outfile = outfile + "/normalization_coeff.txt";
  ofstream WRITER;
  WRITER.open(outfile.c_str(), ofstream::out); 
  
  if(WRITER.good()){
    
    char buffer[254];
    map<int, map<double,double> >* CO = NULL;
    if( ABSOLUTE_NORMALIZATION_MODE ){
      CO = &ABSOLUTE_VARIATIONS;
    }
    else{
      CO = &RELATIVE_VARIATIONS;
    }

    map<int, map<double,double> >::iterator RUN = CO->begin();
    while( RUN != CO->end()  ){

      // print retention time header:
      if( RUN == CO->begin() ){
        sprintf(buffer,"TR:%s",SEP.c_str());  
        WRITER.write(buffer,strlen(buffer));      
        map<double,double>::iterator C = (*RUN).second.begin();
        while( C != (*RUN).second.end() ){
          sprintf(buffer,"%f%s", (*C).first, SEP.c_str());  
          WRITER.write(buffer,strlen(buffer));
          C++;
        }
        sprintf(buffer,"\n");  
        WRITER.write(buffer,strlen(buffer));      
      }
      
      // print the run coefficients:
      sprintf(buffer,"%d%s",(*RUN).first, SEP.c_str());  
      WRITER.write(buffer,strlen(buffer));      
      
      map<double,double>::iterator C = (*RUN).second.begin();
      while( C != (*RUN).second.end() ){
        sprintf(buffer,"%f%s", (*C).second, SEP.c_str());  
        WRITER.write(buffer,strlen(buffer));
        C++;
      }
      
      sprintf(buffer,"\n");  
      WRITER.write(buffer,strlen(buffer));

      RUN++;
    }
    printf("\t\t- normalization coefficient were saved to '%s'\n", outfile.c_str());
  }
  else{
    printf("\nERROR: opening file '%s'\n", outfile.c_str());
  }
}




///////////////////////////////////////////////////////////////
// start intensity normalization by an input profile:
void MASTER_LC_MS_intensity_normalizer::start_INPUT_PROFILE_INTENSITY_NORMALIZATION( ){
  
  
  printf("\n\t\t * start MasterMap normalization according to input profiles\n");
  
  
  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();
  string projName = "ANALYSIS_" + def->search_tag("MY PROJECT NAME");
  if( projName[ projName.size() - 1] != '/' ){
    projName += "/";
  }
  string fileName = projName + "intensityProfile.txt";

  // extact the normalization values from a text file:
  extractNormalizationProfiles( fileName );
  
  // if absolute, then copy the values:
  if( ABSOLUTE_NORMALIZATION_MODE ){
    ABSOLUTE_VARIATIONS = RELATIVE_VARIATIONS;
  }
  
  // applies the computed variations to the features:
  normalize_feature_intensities( );

}


///////////////////////////////////////////////////////////////
// extact the normalization values from a text file:
void MASTER_LC_MS_intensity_normalizer::extractNormalizationProfiles( string fileName ){
  
  printf("\t\t\t- reading input file '%s'\n", fileName.c_str());  
  
  // open a file:
  file_reader* reader = new file_reader();
  // set the parameter file:
  reader->set_current_file( fileName );
  // open file and save the status
  if( reader->open_file() ){
    
    // put instream to start of file:
    reader->jump_to_file_start();    
    while( reader->get_line() != NULL){
      
      // get the line:
      string line = reader->get_line(); 
      string sep = "\t";
      
      if( line.find( sep ) != string::npos ){
        
        string sub = line.substr(0, line.find( sep ));
        double TR = atof( sub.c_str() );
        
        vector<double> correctValues;
        
        do{
          line = line.substr(line.find( sep )+1, line.size());
          string sub;
          if( line.find( sep ) != string::npos ){
            sub = line.substr(0, line.find( sep ));
          }
          else{
            sub = line;
          }
          correctValues.push_back( atof( sub.c_str()) );
          
        }
        while( line.find( sep ) != string::npos );


        if( !correctValues.empty() ){
          
          int id = 0;
          vector<double>::iterator P = correctValues.begin();
          while( P != correctValues.end() ){
            
            map<double,double> tmp;
            tmp.insert( make_pair( TR, (*P) ) );
            RELATIVE_VARIATIONS.insert( make_pair( id, tmp ) );
            
            id++;
            P++; 
          }
          
        }
      
      
      }

      //  check if end of file is reached:
      if(reader->check_end_of_file()){
        break;
      }
      
      reader->read_file_line();
    }
    
    
  }
}
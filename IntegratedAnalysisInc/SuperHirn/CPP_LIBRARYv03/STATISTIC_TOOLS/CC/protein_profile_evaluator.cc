///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS protein_profile_evaluator:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_PROTEIN_PROFILE_EVALUATOR
#include "STATISTIC_TOOLS_USE.h"


int protein_profile_evaluator::NB_PROFILE_DATA_POINTS;
double protein_profile_evaluator::TR_TOL;
bool protein_profile_evaluator::PEPTIDE_PROTEOTYPE_MODE;

////////////////////////////////////////////////
// constructor for the object protein_profile_evaluator:
protein_profile_evaluator::protein_profile_evaluator(){
  MY_PROTEIN = NULL;

}


//////////////////////////////////////////////////
// class desctructor of protein_profile_evaluator
protein_profile_evaluator::~protein_profile_evaluator(){
  MY_PROTEIN = NULL;
}

//////////////////////////////////////////////////
// class copy constructor of protein_profile_evaluator
protein_profile_evaluator::protein_profile_evaluator(const protein_profile_evaluator& tmp){
  MY_PROTEIN = tmp.MY_PROTEIN;
}

//////////////////////////////////////////////////
// class copy constructor of protein_profile_evaluator
protein_profile_evaluator::protein_profile_evaluator(const protein_profile_evaluator* tmp){
  MY_PROTEIN = tmp->MY_PROTEIN;
}

//////////////////////////////////////////////////
// copy constructor:
protein_profile_evaluator& protein_profile_evaluator::operator=(const protein_profile_evaluator& tmp){
  MY_PROTEIN = tmp.MY_PROTEIN;
  return *this;
}
    

//////////////////////////////////////////////////////////
// detectes peptide outliers in protein:  
void protein_profile_evaluator::START_PEPTIDE_OUTLIER_DETECTION( protein_group* IN){

  MY_PROTEIN = IN;
  MY_PROTEIN->build_protein_average_consens( );

  // get the iterator of outlier peptides:
  vector< PEP_ITERATOR > TO_REMOVE = detected_peptide_outliers( );
  unsigned int OLD_SIZE = 0;
  
  while( !(TO_REMOVE.empty()) && (OLD_SIZE != TO_REMOVE.size() ) ){

    // store the size of the old outliers
    OLD_SIZE = TO_REMOVE.size();
    // remove the outliers:
    MY_PROTEIN->remove_outlier_peptide_groups( TO_REMOVE );
    // recompute the protein profile
    MY_PROTEIN->build_protein_average_consens( );
    // new outlier detection:
    TO_REMOVE = detected_peptide_outliers( );  
    
  }
  
  IN = NULL;
}

//////////////////////////////////////////////////////////
// detectes peptide outliers in protein:  
vector< PEP_ITERATOR > protein_profile_evaluator::detected_peptide_outliers( ){
  
  map< double, vector<PEP_ITERATOR> >  RAW_DATA;
  map< double, vector<PEP_ITERATOR> >::iterator F;
  // get the probabilities of proteotypic peptide groups:
  vector<peptide_DELTA_group>::iterator X = MY_PROTEIN->get_proteotypic_peptide_members_list_start(); 
  while(X != MY_PROTEIN->get_proteotypic_peptide_members_list_end()){
    // compute the sim score to the protein consens profile 
    double score = compare_profiles( MY_PROTEIN->get_PROFILE_AVERAGE_CONSENS(), (*X).get_PROFILE_AVERAGE_CONSENS() );
    // outlier detect data:
    // use probablity, score etc...
    //double outlier_data = (*X).get_PROFILE_PROBABILITY( );
    double outlier_data = score;
    // insert into the vector
    F = RAW_DATA.find( outlier_data ); 
    if( F == RAW_DATA.end() ){
      vector< PEP_ITERATOR > TMP;
      TMP.push_back( X );
      RAW_DATA.insert( make_pair( outlier_data, TMP ) );
    }
    else{
      (*F).second.push_back( X );
    }
    X++; 
  }
  if( !PEPTIDE_PROTEOTYPE_MODE ){
    
    // get teh probabilities of non proteotypic peptide groups:
    X = MY_PROTEIN->get_peptide_members_list_start(); 
    while(X != MY_PROTEIN->get_peptide_members_list_end()){
      // compute the sim score to the protein consens profile 
      double score = compare_profiles( MY_PROTEIN->get_PROFILE_AVERAGE_CONSENS(), (*X).get_PROFILE_AVERAGE_CONSENS() );
      // outlier detect data:
      // use probablity, score etc...
      // double outlier_data = (*X).get_PROFILE_PROBABILITY( );
      double outlier_data = score;
      // insert into the vector
      F = RAW_DATA.find( outlier_data ); 
      if( F == RAW_DATA.end() ){
        vector< PEP_ITERATOR > TMP;
        TMP.push_back( X );
        RAW_DATA.insert( make_pair( outlier_data, TMP ) );
      }
      else{
        (*F).second.push_back( X );
      }
      X++;
    }
  }
  
  // store now all the iterators in a container which is marked
  // by the outlier data, so u know which one to remove later on:
  vector< double > PROCESS_DATA;
  F = RAW_DATA.begin();
  while( F != RAW_DATA.end() ){
    double data = (*F).first;
    vector< PEP_ITERATOR >::iterator P = (*F).second.begin();
    while( P != (*F).second.end() ){
      PROCESS_DATA.push_back( data );
      P++;
    }
    F++;
  }
  
  ////////////////////////////
  // do outlier detection:
  simple_math MY_MATH;
  MY_MATH.set_LOW_CHECK( false );
  MY_MATH.OUTLIER_DETECTION_BY_DIXON( &PROCESS_DATA );
  
  ///////////////////////////
  // remove those which are NOT outliers:
  F = RAW_DATA.begin();
  vector< PEP_ITERATOR > TO_REMOVE;
  while( F != RAW_DATA.end() ){
    vector< double >::iterator O = PROCESS_DATA.begin();
    while( O != PROCESS_DATA.end() ){
      if( ((*F).first == (*O)) ){
        break;
      }
      O++;
    }
    // in this case its an outlier:
    if( O == PROCESS_DATA.end()){
      TO_REMOVE.insert( TO_REMOVE.begin(), (*F).second.begin(), (*F).second.end() );        
    }
    F++;
  }
  
  PROCESS_DATA.clear();  
  RAW_DATA.clear();
  return TO_REMOVE;

  // give the iterators to the protein, so it can mark bad ones:
  //MY_PROTEIN->remove_outlier_peptide_groups( TO_REMOVE );
  
}


//////////////////////////////////////////////////
// print all peptides of a protein 
void protein_profile_evaluator::plot_this_protein( protein_group* IN ){
  MY_PROTEIN = IN;
  plot_this_protein();
  IN = NULL;
  
}

//////////////////////////////////////////////////
// print all peptides of a protein 
void protein_profile_evaluator::plot_this_protein(){
  
  char buffer[255];
  sprintf( buffer, "%s_%0.4f", MY_PROTEIN->get_AC().c_str(), MY_PROTEIN->get_PROFILE_SCORE());
  data_plotter* PLOT = new data_plotter(buffer, false);  
  // PLOT->set_Y_RANGE(0.0,0.8);
  
  // first the protein profile:
  map<int, double>* PROF = MY_PROTEIN->get_PROFILE_AVERAGE_CONSENS();
  sprintf( buffer, "consens");
  PLOT->add_plot_data_dotlines( PROF, buffer);
  
  // get the probabilities of proteotypic peptide groups:
  vector<peptide_DELTA_group>::iterator X = MY_PROTEIN->get_proteotypic_peptide_members_list_start(); 
  while(X != MY_PROTEIN->get_proteotypic_peptide_members_list_end()){
    sprintf( buffer, "%s(%d)[%s]: %0.3f | %0.3f ,", (*X).get_MOD_SQ().c_str(),(*X).get_nb_members(), (*X).get_MS2_TYPE_TAG().c_str(),(*X).get_PROFILE_PROBABILITY(), (*X).get_PROFILE_SCORE() );
    PLOT->add_plot_data( (*X).get_PROFILE_AVERAGE_CONSENS(), buffer );
    X++; 
  }
  if( !PEPTIDE_PROTEOTYPE_MODE ){
    // get teh probabilities of non proteotypic peptide groups:
    X = MY_PROTEIN->get_peptide_members_list_start(); 
    while(X != MY_PROTEIN->get_peptide_members_list_end()){
      sprintf( buffer, "%s(%d)[%s]: %0.3f / %0.3f,", (*X).get_MOD_SQ().c_str(), (*X).get_nb_members(), (*X).get_MS2_TYPE_TAG().c_str(),(*X).get_PROFILE_PROBABILITY(), (*X).get_PROFILE_SCORE() );
      PLOT->add_plot_data( (*X).get_PROFILE_AVERAGE_CONSENS(), buffer );
      X++; 
    }
  }
  
  // plot outlier peptide groups:
  X = MY_PROTEIN->get_outlier_peptide_members_list_start(); 
  while(X != MY_PROTEIN->get_outlier_peptide_members_list_end()){
    sprintf( buffer, "OUTLIER: %s(%d)[%s]: %0.3f | %0.3f ,", (*X).get_MOD_SQ().c_str(),(*X).get_nb_members(), (*X).get_MS2_TYPE_TAG().c_str(),(*X).get_PROFILE_PROBABILITY(), (*X).get_PROFILE_SCORE() );
    PLOT->add_plot_data_dotlines( (*X).get_PROFILE_AVERAGE_CONSENS(), buffer );
    X++; 
  }
  
  PLOT->set_POINT_SIZE( 1 );
  PLOT->plot_TWOD_data();  
  // PLOT->print_MATRIX_data_to_TXT();
  // PLOT->print_TWOD_data_to_TXT();
  delete PLOT;
  PLOT = NULL;
  
}

//////////////////////////////////////////////////
// print all peptides of a protein 
void protein_profile_evaluator::plot_this_protein( vector<protein_group>* IN, string name){
  
  char buffer[255];
  sprintf( buffer, "%s", name.c_str());
  data_plotter* PLOT = new data_plotter(buffer, false);  
  
  vector<protein_group>::iterator PG = IN->begin();
  while( PG != IN->end() ){
    

    protein_group* PROTEIN = &(*PG);    
    // first the protein profile:
    map<int, double>* PROF = PROTEIN->get_PROFILE_AVERAGE_CONSENS();
    sprintf( buffer, "%s, %0.2f", PROTEIN->get_AC().c_str(), PROTEIN->get_PROFILE_PROBABILITY());
    PLOT->add_plot_data_dotlines( PROF, buffer);
    
    // plot the individual ones:
    MY_PROTEIN = PROTEIN;
    plot_this_protein( );
    
    PG++; 
  }
  
  PLOT->set_POINT_SIZE( 1 );
  PLOT->plot_TWOD_data();  
  delete PLOT;
  PLOT = NULL;
  
}

//////////////////////////////////////////////////
// print all peptides of a protein 
void protein_profile_evaluator::plot_this_protein( vector<protein_group>* IN, int T_ID, string name){
  
  if( !IN->empty() ){
    char buffer[255];
    sprintf( buffer, "%s", name.c_str());
    data_plotter* PLOT = new data_plotter(buffer, false);  
    
    if( !TARGET_PROFILES.empty() ){
      map<int, vector<double> >::iterator F = TARGET_PROFILES.find( T_ID );
      if( F != TARGET_PROFILES.end() ){
        consens_profile_builder CONS;
        map<int, double> my_p = CONS.normalize_profile( ((*F).second ) ) ;
        sprintf( buffer, "target profile");
        PLOT->add_plot_data( &my_p, buffer );
      }
    }
    
    vector<protein_group>::iterator PG = IN->begin();
    while( PG != IN->end() ){
      
      protein_group* PROTEIN = &(*PG);    
      // first the protein profile:
      map<int, double>* PROF = PROTEIN->get_PROFILE_AVERAGE_CONSENS();
      sprintf( buffer, "%s, %0.2f", PROTEIN->get_AC().c_str(), PROTEIN->get_PROFILE_PROBABILITY());
      
      PLOT->add_plot_data_dotlines( PROF, buffer);
      
      // plot the individual ones:
      MY_PROTEIN = PROTEIN;
      plot_this_protein( );
      
      PG++; 
    }
    
    PLOT->set_POINT_SIZE( 1 );
    PLOT->plot_TWOD_data();  
    // PLOT->print_MATRIX_data_to_TXT();
    
    delete PLOT;
    PLOT = NULL;
  }
}


////////////////////////////////////////////////////
// score vs. average protein profile:
double protein_profile_evaluator::compare_profiles( map<int, double>* TARGET, map<int, double>* CHECK ){
  
  // score against main cluster profile:  
  profile_scorer PRO_S; 
  return PRO_S.COMPARE_TO_TARGET_PROFILE( TARGET, CHECK );

  
}


//////////////////////////////////////////////////
// print all peptides of a protein 
void protein_profile_evaluator::print_this_protein_profile( vector<protein_group>* IN, string name){
  
  string file = "ANALYSIS_";
  read_param* def = new read_param();
  file = file + def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;  
  
  if( file[ file.size() - 1 ] != '/' ){
    file += "/";
  }
  file = file + name + ".txt";
  
  ofstream* WRITER = new ofstream();
  WRITER->open(file.c_str(), ofstream::out); 
  if( WRITER->good() ){
    
    char buffer[255];
    string SEP = "\t";
    
    vector<protein_group>::iterator PG = IN->begin();
    while( PG != IN->end() ){
      
      protein_group* PROTEIN = &(*PG);
      sprintf( buffer , "\n%s\n", PROTEIN->get_AC().c_str() );
      WRITER->write(buffer,strlen(buffer));
      
      // protein profile:
      map<int, double>* PROF = PROTEIN->get_PROFILE_AVERAGE_CONSENS();
      map<int, double>::iterator P = PROF->begin();
      while( P != PROF->end() ){
        
        sprintf( buffer , "%d%s%0.3f\n", (*P).first, SEP.c_str(), (*P).second  );
        WRITER->write(buffer,strlen(buffer));
        P++;
      }
      
      PG++;
    }
  }    
  
  delete WRITER;
  WRITER = NULL;
  
}


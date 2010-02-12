///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS protein_ratiolizer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_PROTEIN_RATIOLIZER
#define USE_MS1_FEATURE_RATIOLIZER

#include "RATIOLIZER_USE.h"


map<int, vector<double> > protein_ratiolizer::LC_MS_Modification_Masses;


////////////////////////////////////////////////
// constructor for the object protein_ratiolizer:
protein_ratiolizer::protein_ratiolizer(  ){
  INPUT_PEP_LIST = NULL;
  INIT();
}

//////////////////////////////////////////////////
// class desctructor of protein_ratiolizer
protein_ratiolizer::~protein_ratiolizer(){
  PROTEIN_GROUPS.clear();
  PROTEIN_GROUP_ANALYSIS.clear(); 
  INPUT_PEP_LIST = NULL;
}

//////////////////////////////////////////////////
// class copy constructor of protein_ratiolizer
protein_ratiolizer::protein_ratiolizer(const protein_ratiolizer& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of protein_ratiolizer
protein_ratiolizer::protein_ratiolizer(const protein_ratiolizer* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
protein_ratiolizer& protein_ratiolizer::operator=(const protein_ratiolizer& tmp){
  return *this;
}
    
//////////////////////////////////////////////////
// initialize parameters:
void protein_ratiolizer::INIT(){

  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();  
  
  // print out
  def->search_tag("gnuplot plot generator", &PRINT_OUT);
  
  // get the LC/MS number to which ratios are build
  def->search_tag("ratio target LC/MS run", &RATIO_TARGET_LC_MS);
  // set also for the peptide ratio class:
  peptide_ratio_analyzer::RATIO_TARGET_LC_MS = RATIO_TARGET_LC_MS;

  // get the LC/MS number to which ratios are build
  def->search_tag("gauss data percentile", &DATA_PRECENTILE);

  // probability of significant peptide ratio change
  def->search_tag("ratiolizer probabilty thershold", &PROBABILITY_THRESHOLD);
  
  // if to plot every portein ratio
  def->search_tag("plot indv. protein ratios", &INDIV_PROTEIN_RATIO_VIEW);
  
  // target directory
  target_dir = "ANALYSIS_" + def->search_tag("MY PROJECT NAME");  

  delete def;
  def = NULL;
}



///////////////////////////////////////////////////
// extract the target feature and insert ratios:
void protein_ratiolizer::start_RATIO_ANALYSIS(){
      
  if( INPUT_PEP_LIST == NULL ){
    printf("\t\t Input peptide analysis list not set in protein analyzer::90\n");    
  }
  else if( INPUT_PEP_LIST->empty() ){
    printf("\t\t Input peptide analysis list is empty! (protein analyzer::97)\n");    
  }
  else{
    
    // extract proteins:
    start_extracing_protein_groups();
    
    // analyze protein groups:
    printf("\t\t- analyze %d protein groups...\n", PROTEIN_GROUP_ANALYSIS.size() );
    vector<PROTEIN_RATIO>::iterator ST = PROTEIN_GROUP_ANALYSIS.begin();
    while( ST != PROTEIN_GROUP_ANALYSIS.end() ){
      analyze_protein_groups( &(*ST) );
      ST++;
    }
    
    // make a gauss fit:
    do_GAUSS_FIT();
    
    // plot analysis:
    print_to_plot_RATIO_ANALYSIS();
  }
  
}



///////////////////////////////////////////////////
// call all the plotting functions
void protein_ratiolizer::print_to_plot_RATIO_ANALYSIS(){
  
  if( PRINT_OUT ){
    
    printf("\t\t- plot protein analysis...\n");

    if( INDIV_PROTEIN_RATIO_VIEW ){

      printf("\t\t\t* plot individual protein ratios\n");
      
      // print individual protein ratios:
      vector<PROTEIN_RATIO>::iterator ST = PROTEIN_GROUP_ANALYSIS.begin();
      while( ST != PROTEIN_GROUP_ANALYSIS.end() ){
        if( (*ST).validAnalysis ){
          print_protein_analysis( &(*ST) );
        }
        ST++;
      }
    }

    printf("\t\t\t* plot protein ratio summary\n");
    // print all proteins in one plot
    print_all_protein_analysis();
  }    
}
  
  
///////////////////////////////////////////////////
// create a protein ratio group:
void protein_ratiolizer::start_extracing_protein_groups( ){
  
  printf("\t\t- build protein groups...\n");  
  
  vector< PEPTIDE_RATIO>::iterator PEP = INPUT_PEP_LIST->begin();
  while( PEP != INPUT_PEP_LIST->end() ){
    
    PEPTIDE_RATIO* MY_PEP = &(*PEP);
    
    // check if it contains any MS2 info:
    if( MY_PEP->TARGET->get_BEST_MS2_SCAN() != NULL ){
    
      // check if the corresponding protein group already exists
      // otherwise make one:
      if( !( find_PROTEIN_GROUPS_for_PEPTIDE( MY_PEP ) ) ){
        
        // create for every protein, for which a peptide codes the protein:
        map<string, vector<ms2_info*> >::iterator Q = MY_PEP->TARGET->get_MS2_INFO_START();
        while( Q != MY_PEP->TARGET->get_MS2_INFO_END() ){
          
          PROTEIN_RATIO PROT;
          
          protein_group* PROT_G = new protein_group( (*Q).first );
          PROT_G->merge_peptide_DELTA_group( MY_PEP->TARGET );
          PROT_G->set_PROTEIN_GROUP_ID( PROTEIN_GROUPS.size() );
          
          // insert into the vector of the main structure:
          PROTEIN_GROUPS.push_back( PROT_G );        
          
          // add the peptide ratios:
          PROT.ALL_PEPTIDE_RATIOS.push_back( make_pair( MY_PEP->NORMALIZED_AVERAGE_PEPTIDE_RATIO, MY_PEP ) );
          
          PROT.TARGET = PROT_G;
          PROT.ID = PROTEIN_GROUP_ANALYSIS.size();
          PROTEIN_GROUP_ANALYSIS.push_back( PROT );

          PROT_G = NULL;
          Q++;
        }      
      }
    }
    
    MY_PEP = NULL;
    PEP++;
  }
}



///////////////////////////////////////////////////
// find the corresponding protein ration group:
bool protein_ratiolizer::find_PROTEIN_GROUPS_for_PEPTIDE( PEPTIDE_RATIO* IN ){
  
  bool CHECK = false;
  vector<PROTEIN_RATIO>::iterator ST = PROTEIN_GROUP_ANALYSIS.begin();
  while( ST != PROTEIN_GROUP_ANALYSIS.end() ){

    PROTEIN_RATIO* THIS_PROT = &(*ST);
    protein_group* this_PG = THIS_PROT->TARGET;
    
    if( this_PG->check_belonging( IN->TARGET ) ){
      
      // merge the peptide into this protein:
      this_PG->merge_peptide_DELTA_group( IN->TARGET );
      
      // add the peptide ratios:
      THIS_PROT->ALL_PEPTIDE_RATIOS.push_back( make_pair( IN->NORMALIZED_AVERAGE_PEPTIDE_RATIO, IN) );      
      
      CHECK = true;
    }
    ST++;
  }
  return CHECK;
}

///////////////////////////////////////////////////
// analyze protein groups:
void protein_ratiolizer::analyze_protein_groups( PROTEIN_RATIO* THIS_PROT ){

    
  THIS_PROT->AVERAGE_PROTEIN_RATIO = 0.0;
  THIS_PROT->AVERAGE_PROTEIN_RATIO_STDEV = 0.0;
  
  /*
  if( THIS_PROT->TARGET->get_AC() == "FBgn0013984" ){
    cout<<" "<<endl;
  }
  */
  
  ////////////////////////////
  // OUTLIER DETECTION::
  // store the raw data before the outlier detection:
  THIS_PROT->ALL_PEPTIDE_RATIOS_RAW = THIS_PROT->ALL_PEPTIDE_RATIOS;
  
  ///////////////////////////
  // filter out no existing peptide ratios ( those which are marked by -1)
  vector< pair< double, PEPTIDE_RATIO* > >::iterator TP = THIS_PROT->ALL_PEPTIDE_RATIOS.begin();
  while( TP != THIS_PROT->ALL_PEPTIDE_RATIOS.end() ){
    if( !(*TP).second->validAnalysis ){
      TP = THIS_PROT->ALL_PEPTIDE_RATIOS.erase( TP );
    }
    else{
      TP++;
    }
  }

  if( THIS_PROT->ALL_PEPTIDE_RATIOS.size() > 1 ){
    
    
    vector< pair<double, void*> > V_POINTER;
    /////////////////////////////////////
    // here to NOT weight by different charge states of the same peptide 
    //V_POINTER = (vector< pair<double, void*> >*)(&(THIS_PROT->ALL_PEPTIDE_RATIOS));
    
    /////////////////////////////////////
    // here to DO weight by different charge states of the same peptide 
    vector< pair< double, PEPTIDE_RATIO* > >::iterator PEP_RATIO = THIS_PROT->ALL_PEPTIDE_RATIOS.begin();
    while( PEP_RATIO != THIS_PROT->ALL_PEPTIDE_RATIOS.end() ){
      vector< pair<double,double> >::iterator RAW_RATIO = (*PEP_RATIO).second->ALL_FEATURE_RATIOS.begin();
      while( RAW_RATIO != (*PEP_RATIO).second->ALL_FEATURE_RATIOS.end() ){
        V_POINTER.push_back( pair<double, void*>( (*RAW_RATIO).first, (*PEP_RATIO).second ) );     
        RAW_RATIO++;
      }
      
      PEP_RATIO++;
    }
    
    
    // make an outlier detection on the peptide level:
    simple_math MY_MATH;
    MY_MATH.OUTLIER_DETECTION_BY_DIXON( &V_POINTER );
    
    ///////////////////
    // OLD VERSION:
    // get average and stdev of the all peptide ratios:
    //pair<double, double> AVERAGE_PROTEIN_RATIO = MY_MATH.AVERAGE_and_STDEV( &V_POINTER );
    //THIS_PROT->AVERAGE_PROTEIN_RATIO = AVERAGE_PROTEIN_RATIO.first;
    //THIS_PROT->AVERAGE_PROTEIN_RATIO_STDEV = AVERAGE_PROTEIN_RATIO.second;
    
    ///////////////////
    // NEW VERSION:
    // convert into a vector of pair<Ratio, Stdev>
    vector< pair<double, double> > pepData;
    vector< pair<double, void*> >::iterator I = V_POINTER.begin();
    while( I != V_POINTER.end() ){
      pair<double, void*> p = (*I);
      PEPTIDE_RATIO* pep = (PEPTIDE_RATIO*)p.second;
      pepData.push_back( make_pair( pep->AVERAGE_PEPTIDE_RATIO, pep->AVERAGE_PEPTIDE_RATIO_STD ) );
      I++;
    }
    
    peptide_ratio_analyzer pep;
    // compute the ratios:
    THIS_PROT->AVERAGE_PROTEIN_RATIO = pep.computeAverageStdevWeightedRatio( &pepData );
    THIS_PROT->AVERAGE_PROTEIN_RATIO_STDEV = pep.computeAverageRatioStdev( &pepData );
        
    // clean up:
    V_POINTER.clear();    
    
    THIS_PROT->validAnalysis = true;
    
  }
  else if( !THIS_PROT->ALL_PEPTIDE_RATIOS.empty() ){
    vector< pair< double, PEPTIDE_RATIO* > >::iterator TP = THIS_PROT->ALL_PEPTIDE_RATIOS.begin();
    THIS_PROT->AVERAGE_PROTEIN_RATIO = (*TP).second->AVERAGE_PEPTIDE_RATIO;
    THIS_PROT->AVERAGE_PROTEIN_RATIO_STDEV = (*TP).second->AVERAGE_PEPTIDE_RATIO_STD;    
    THIS_PROT->validAnalysis = true;

  }
  else{
    THIS_PROT->validAnalysis = false;
  }
  
}



//////////////////////////////////////////////////////////////
// print the protein ratio plot:
void protein_ratiolizer::print_protein_analysis( PROTEIN_RATIO* IN ){
  
  string AC = IN->TARGET->get_AC();
  peptide_DELTA_group* PEP = NULL;
  char outfile[255];
  sprintf( outfile, "%s_ratiolizer_plot_ratio=%f(p=%f)", AC.c_str(), IN->AVERAGE_PROTEIN_RATIO, IN->AVERAGE_PROTEIN_RATIO_STDEV);
  string tmp = outfile;
  data_plotter* PLOT = new data_plotter(tmp, false);

  PLOT->set_Y_RANGE(0.0, 3.0);
  PLOT->set_X_RANGE(0.0, double(IN->ALL_PEPTIDE_RATIOS_RAW.size()));
  
  char buffer[255];
  map<int, double> TMP;
  int NB = 1;
  vector< pair<double, PEPTIDE_RATIO* > >::iterator P = IN->ALL_PEPTIDE_RATIOS_RAW.begin();
  while( P != IN->ALL_PEPTIDE_RATIOS_RAW.end() ){

    PEP = (*P).second->TARGET;
    ms2_info* MS2 = PEP->get_BEST_MS2_SCAN(-3.0);
      
    TMP.insert( make_pair( NB, (*P).first ));
    if( MS2 != NULL ){
      sprintf( buffer, "%d:RAW r=%0.2f: %0.2fDa, %s", NB,(*P).first, PEP->get_MR(), MS2->get_MOD_SQ().c_str() );
    }
    else{
      sprintf( buffer, "%d:RAW r=%0.2f: %0.2fDa", NB, (*P).first, PEP->get_MR() );
    }
     
    PLOT->add_plot_data_points( &(TMP), buffer );
    TMP.clear();
    
    ///////////////
    // try to find it in the processed ratios:
    vector< pair<double, PEPTIDE_RATIO* > >::iterator F = 
      find( IN->ALL_PEPTIDE_RATIOS.begin(), IN->ALL_PEPTIDE_RATIOS.end(), *P );
    
    if( F != IN->ALL_PEPTIDE_RATIOS.end() ){
     
      PEP = (*P).second->TARGET;
      ms2_info* MS2 = PEP->get_BEST_MS2_SCAN(-3.0);
      
      TMP.insert( make_pair( NB, (*P).first ));
      if( MS2 != NULL ){
        sprintf( buffer, "%d:r=%0.2f: %0.2fDa, %s", NB, (*P).first, PEP->get_MR(),MS2->get_MOD_SQ().c_str() );
      }
      else{
        sprintf( buffer, "%d:r=%0.2f: %0.2fDa", NB, (*P).first, PEP->get_MR() );
      }
      
      PLOT->add_plot_data_points( &(TMP), buffer );
      TMP.clear();      
    }
    
  
    
    NB++;
    P++;
  }
    
      
  PLOT->plot_TWOD_data();
  delete PLOT;
  PLOT = NULL;


}


//////////////////////////////////////////////////////////////
// print all protein ratio plot:
void protein_ratiolizer::print_all_protein_analysis(){
  
  int count = 0;
  int ID = 0;
  double Y_MAX = 5.0;
  data_plotter* PLOT = NULL;
  char outfile[255];
  map<string, pair< map<int, double> , vector<double> > > PLOT_DATA;
  vector< map<double, double> > RAW_DATA;
  
  
  // loop through proteins:
  vector<PROTEIN_RATIO>::iterator ST = PROTEIN_GROUP_ANALYSIS.begin();
  while( ST != PROTEIN_GROUP_ANALYSIS.end() ){
        
    PROTEIN_RATIO* IN = &(*ST);
    
    if( !IN->ALL_PEPTIDE_RATIOS.empty() ){

      char buffer[255];
      sprintf(buffer, "%d:%s[%d], R=%.2f", count, IN->TARGET->get_AC().c_str(), IN->ALL_PEPTIDE_RATIOS.size(), IN->AVERAGE_PROTEIN_RATIO );
      map<int, double> TMP;
      vector<double> TMP_STDEV;
      TMP_STDEV.push_back( IN->AVERAGE_PROTEIN_RATIO_STDEV );
      TMP.insert( make_pair(count, IN->AVERAGE_PROTEIN_RATIO ) );
      PLOT_DATA.insert( make_pair( buffer, make_pair( TMP, TMP_STDEV ) ) );
      TMP.clear();
      TMP_STDEV.clear();
      
      // individual data:
      map<double, double> TMP_2;
      vector< pair<double, PEPTIDE_RATIO* > >::iterator P = IN->ALL_PEPTIDE_RATIOS.begin();
      while( P != IN->ALL_PEPTIDE_RATIOS.end() ){      
        TMP_2.insert( make_pair( double(count), (*P).first ));
        P++;
      }
      RAW_DATA.push_back( TMP_2 );      
      count++;
    }
    
    if( count == 30 ){
      sprintf( outfile, "Ratiolizer_protein_%d-%d_plot", ID, ID + 30 );
      ID += 30;
      string tmp = outfile;
      PLOT = new data_plotter(tmp);
      PLOT->set_Y_RANGE( 0, Y_MAX );
      
      map<string, pair< map<int, double> , vector<double> > >::iterator X =  PLOT_DATA.begin();
      vector< map<double, double> >::iterator X_2 =  RAW_DATA.begin();
      while( X != PLOT_DATA.end() ){
        PLOT->add_plot_data_points( &((*X).second.first), &((*X).second.second), (*X).first );
        X++;
        X_2++;
      }
      
      PLOT->plot_TWOD_data();
      delete PLOT;
      PLOT = NULL;
      PLOT_DATA.clear();
      RAW_DATA.clear();
      count = 0;
    }
    
    ST++; 
  }
  
  if( count > 0 ){
    sprintf( outfile, "Ratiolizer_protein_%d-%d_plot", ID, ID + count);
    string tmp = outfile;
    PLOT = new data_plotter(tmp);
    PLOT->set_Y_RANGE( 0, Y_MAX );

    map<string, pair< map<int, double> , vector<double> > >::iterator X =  PLOT_DATA.begin();
    vector< map<double, double> >::iterator X_2 =  RAW_DATA.begin();
    while( X != PLOT_DATA.end() ){
      PLOT->add_plot_data_points( &((*X).second.first), &((*X).second.second), (*X).first );
      X++;
      X_2++;
    }
    
    PLOT->plot_TWOD_data();
    delete PLOT;
    PLOT = NULL;
    PLOT_DATA.clear();
    RAW_DATA.clear();
  }
   
}

//////////////////////////////////////////////////////////////
// print all protein ratio plot to simple text file
void protein_ratiolizer::print_text_protein_analysis(){

  char text_buffer[500];
  ofstream* WRITER;
  
  string file = target_dir + "/all_protein_ratios.txt";
  WRITER = new ofstream();
  WRITER->open(file.c_str(), ofstream::out);  

  // print the header:
  sprintf(text_buffer, "AC\tProtein Ratio\tRatio Stdev\tNb.decharged peptides\tAll.peps\n");
  WRITER->write(text_buffer,strlen(text_buffer));
  
  // print the proteins:
  vector<PROTEIN_RATIO>::iterator ST = PROTEIN_GROUP_ANALYSIS.begin();
  while( ST != PROTEIN_GROUP_ANALYSIS.end() ){
    
    PROTEIN_RATIO* IN = &(*ST);
    if( IN->ALL_PEPTIDE_RATIOS.empty() ){
      sprintf(text_buffer, "%s\tNA\tNA\tNA\t%d\n", IN->TARGET->get_AC().c_str(), IN->ALL_PEPTIDE_RATIOS_RAW.size() );
      WRITER->write(text_buffer,strlen(text_buffer));
    }
    else{
      sprintf(text_buffer, "%s\t%0.2f\t%0.2f\t%d\t%d\n", IN->TARGET->get_AC().c_str(), IN->AVERAGE_PROTEIN_RATIO, IN->AVERAGE_PROTEIN_RATIO_STDEV, IN->ALL_PEPTIDE_RATIOS.size(), IN->ALL_PEPTIDE_RATIOS_RAW.size() );
      WRITER->write(text_buffer,strlen(text_buffer));
    }
    ST++; 
    
  }
  
  printf("\t\t- Protein ratios stored in '%s'\n", file.c_str() ); 
  
  delete WRITER;
  WRITER = NULL;
}


/////////////////////////////////////////////////////////////////////
// write all portein groups analysis to XML file:
void protein_ratiolizer::write_protein_group_analysis_2_XML(ofstream* WRITER){
  
  char buffer[254];
  
  sprintf(buffer,"<AllProteinRatios nb=\"%d\">\n", PROTEIN_GROUP_ANALYSIS.size());  
  WRITER->write(buffer,strlen(buffer));
  
  vector< PROTEIN_RATIO>::iterator P = PROTEIN_GROUP_ANALYSIS.begin();
  while( P != PROTEIN_GROUP_ANALYSIS.end() ){
    write_PROTEIN_GROUP_ANALYSIS_2_XML( &(*P), WRITER );
    P++;
  }
  sprintf(buffer,"</AllProteinRatios>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  WRITER = NULL;
}


/////////////////////////////////////////////////////////////////////
// write PROTEIN_GROUP_ANALYSIS group out:
void protein_ratiolizer::write_PROTEIN_GROUP_ANALYSIS_2_XML( PROTEIN_RATIO* ratio, ofstream* WRITER){
  
  char buffer[1064];
  
  sprintf(buffer,"<ProteinRatio id=\"%d\" AC=\"%s\">\n", ratio->ID, ratio->TARGET->get_AC().c_str());  
  WRITER->write(buffer,strlen(buffer));
  

  // print the list of associated peptide ratios:
  sprintf(buffer,"<FeatureRatioIDs size=\"%d\" ", ratio->ALL_PEPTIDE_RATIOS_RAW.size());  
  WRITER->write(buffer,strlen(buffer));

  // print their ids:
  sprintf(buffer,"");  
  vector< pair< double, PEPTIDE_RATIO* > >::iterator P = ratio->ALL_PEPTIDE_RATIOS_RAW.begin();
  while( P != ratio->ALL_PEPTIDE_RATIOS_RAW.end() ){
    sprintf(buffer,"%s%d,", buffer, (*P).second->ID);
    P++;
  }  
  string tmp = buffer;
  // remove last comma:
  tmp.erase( tmp.size() - 1 , tmp.size() );
  
  sprintf(buffer,"ids=\"%s\">\n", tmp.c_str());  
  WRITER->write(buffer,strlen(buffer));
             
             
  sprintf(buffer,"</FeatureRatioIDs>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  
  sprintf(buffer,"</ProteinRatio>\n");  
  WRITER->write(buffer,strlen(buffer));
  WRITER = NULL;
}




/////////////////////////////////////////////////////////////
// make a gauss fit to the ratio histogram:
void protein_ratiolizer::do_GAUSS_FIT( ){
  
  CURRENT_GAUSS.mean = 0.0;
  CURRENT_GAUSS.variance = 0.0;
  
  printf("\t\t- statistical analysis (PROTEIN LEVEL)\n");
  
  map<double, double> DATA;
  vector< PROTEIN_RATIO>::iterator P =  PROTEIN_GROUP_ANALYSIS.begin();
  while( P != PROTEIN_GROUP_ANALYSIS.end() ){
    if( (*P).validAnalysis ){
      DATA.insert( make_pair( log10( (*P).AVERAGE_PROTEIN_RATIO), 0 ) );
    }
    P++;
  }
  
  /////////////////////////////////
  // start statistical analysis:
  char buffer[255];
  sprintf(buffer, "ratiolizer_gauss_fit_PROTEIN_LEVEL");
  EM_fitter* ANA = new EM_fitter(buffer);
  ANA->set_DATA( &DATA );
  ANA->start_EM_1COMP_analysis( DATA_PRECENTILE );
  delete ANA;
  ANA = NULL;
  
}




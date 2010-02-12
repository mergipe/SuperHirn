///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS MS1_feature_ratiolizer:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_MS1_FEATURE_RATIOLIZER
#define USE_DELTA_GROUPER
#define USE_PROFILE_GROUP
#define USE_PROTEIN_GROUP
#define USE_PEPTIDE_DELTA_GROUP

#include "RATIOLIZER_USE.h"


map<int, vector<double> > MS1_feature_ratiolizer::LC_MS_Modification_Masses;
bool MS1_feature_ratiolizer::TRYPTIC_PEPTIDE_CHECK;
bool MS1_feature_ratiolizer::MODIFICATION_TIC_PEPTIDE_INTEGRATION;
bool MS1_feature_ratiolizer::SAVE_SINGLE_PROTEINS = true;

////////////////////////////////////////////////
// constructor for the object MS1_feature_ratiolizer:
MS1_feature_ratiolizer::MS1_feature_ratiolizer( LC_MS* IN ){
  MASTER = IN;
  INIT();
}

//////////////////////////////////////////////////
// class desctructor of MS1_feature_ratiolizer
MS1_feature_ratiolizer::~MS1_feature_ratiolizer(){
  MASTER = NULL;
  PEPTIDE_GROUPS.clear();
  PEPTIDE_GROUP_ANALYSIS.clear();
}

//////////////////////////////////////////////////
// class copy constructor of MS1_feature_ratiolizer
MS1_feature_ratiolizer::MS1_feature_ratiolizer(const MS1_feature_ratiolizer& tmp){
}

//////////////////////////////////////////////////
// class copy constructor of MS1_feature_ratiolizer
MS1_feature_ratiolizer::MS1_feature_ratiolizer(const MS1_feature_ratiolizer* tmp){
}

//////////////////////////////////////////////////
// copy constructor:
MS1_feature_ratiolizer& MS1_feature_ratiolizer::operator=(const MS1_feature_ratiolizer& tmp){
  return *this;
}
    
//////////////////////////////////////////////////
// initialize parameters:
void MS1_feature_ratiolizer::INIT(){

  //////////////////////////////////
  // read parameters from "param.def"
  read_param* def = new read_param();  
  
  // print out
  def->search_tag("gnuplot plot generator", &PRINT_OUT);
  
  // get the LC/MS number to which ratios are build
  def->search_tag("ratio target LC/MS run", &RATIO_TARGET_LC_MS);
  
  // get the LC/MS number to which ratios are build
  def->search_tag("ratiolizer replicate number", &REPLICATE_NUMBER);
  
  // set also for the peptide ratio class:
  peptide_ratio_analyzer::RATIO_TARGET_LC_MS = RATIO_TARGET_LC_MS;
  peptide_ratio_analyzer::REPLICATE_NUMBER = REPLICATE_NUMBER;

  
  // get the LC/MS number to which ratios are build
  bool TMP_B;
  def->search_tag("ratiolizer replicate check", &TMP_B);
  // select if only feature observed all replicates used
  peptide_ratio_analyzer::REPLICATE_CHECK = TMP_B;

  // get the LC/MS number to which ratios are build
  def->search_tag("gauss data percentile", &DATA_PRECENTILE);

  // probability of significant peptide ratio change
  def->search_tag("ratiolizer probabilty thershold", &PROBABILITY_THRESHOLD);

  // probability of significant peptide ratio change
  def->search_tag("Tryptic peptide selection", &TRYPTIC_PEPTIDE_CHECK);

  // probability of significant peptide ratio change
  def->search_tag("TIC integration over peptide modification variants", &MODIFICATION_TIC_PEPTIDE_INTEGRATION);

  delete def;
  def = NULL;
  
}



///////////////////////////////////////////////////
// extract the target feature and insert ratios:
void MS1_feature_ratiolizer::start_RATIO_ANALYSIS(){

  PEPTIDE_GROUPS.clear();
  PEPTIDE_GROUP_ANALYSIS.clear();
  
  ////////////////////////////////////////////////
  // PEPTIDE LEVEL:
  start_extracing_peptide_DELTA_groups( );   
  
  // extract the ratio for extracted peptide groups:
  extract_peptide_group_ratios( );
  
  // make a normalization with EM:
  do_GAUSS_FIT(  ); 
  
}


///////////////////////////////////////////////////
// extract peptide groups (decharging)
void MS1_feature_ratiolizer::start_extracing_peptide_DELTA_groups( ){
  
  printf("\t\t- Update features by MS2 scans & build peptide groups...");
  
  DELTA_grouper* D_C = new DELTA_grouper();
  // start the feature MS2 updating process:
  feature_updater* UPDATER = new feature_updater(); 
  UPDATER->INIT_ALL();

  progress_bar bar( MASTER->get_nb_features(), "processed");
  // go through all features:  
  vector< LC_MS_FEATURE >::iterator ST = MASTER->get_feature_list_begin();
  while( ST != MASTER->get_feature_list_end() ){
    
    feature* THIS_F = &(*ST);
    
    // try to update the MS2 info of a feature:
    UPDATER->update_feature_identity( THIS_F );
    
    // check if present i target run:
    if( RATIO_TARGET_LC_MS == -1 ){
      D_C->find_PEPTIDE_GROUPS_for_FEATURE( THIS_F );
    }
    else if( THIS_F->get_spectrum_ID() == RATIO_TARGET_LC_MS){
      D_C->find_PEPTIDE_GROUPS_for_FEATURE( THIS_F );
    }
    else{
      map<int,feature>::iterator F = THIS_F->find_match_by_id( RATIO_TARGET_LC_MS );
      if( F != THIS_F->get_match_list_end() ){
        D_C->find_PEPTIDE_GROUPS_for_FEATURE( THIS_F );
      }
    }
    
    // progress bar:
    bar.update_progress();
    
    ST++;
  }

  // get the list of created Peptide Delta Groups:
  PEPTIDE_GROUPS = D_C->get_EXTRACTE_PEPTIDE_GROUPS();
  
  delete D_C;
  D_C = NULL;
  
  UPDATER->print_update_summary();
  delete UPDATER;
  UPDATER = NULL;

  // filter the peptide groups:
  filter_extracted_peptide_groups();
  
  // print info:
  printf("\t\t\t\t-> %d decharged peptide groups extracted\n", PEPTIDE_GROUPS.size());

  
}



///////////////////////////////////////////////////
// extract the ratio for peptide groups:
void MS1_feature_ratiolizer::extract_peptide_group_ratios(  ){

  peptide_ratio_analyzer ANALYZE;
  
  printf("\t\t\t- extract peptide abundance ratios\n");
  // go through all features:  
  vector<peptide_DELTA_group>::iterator ST = PEPTIDE_GROUPS.begin();
  while( ST != PEPTIDE_GROUPS.end() ){
        
    PEPTIDE_RATIO TMP = ANALYZE.analyze_peptide( &(*ST) ); 
    // set id:
    TMP.ID = PEPTIDE_GROUP_ANALYSIS.size();
    
    if( TMP.TARGET != NULL ){ 
      PEPTIDE_GROUP_ANALYSIS.push_back( TMP );
    }
    
    // next
    ST++;
  }
  
  
}


/////////////////////////////////////////////////////////////
// make a gauss fit to the ratio histogram:
void MS1_feature_ratiolizer::do_GAUSS_FIT( ){
  
  CURRENT_GAUSS.mean = 0.0;
  CURRENT_GAUSS.variance = 0.0;
  
  printf("\t\t- statistical analysis (DECHARGED PEPTIDE LEVEL)\n");

  map<double, double> DATA;
  vector<PEPTIDE_RATIO>::iterator P = PEPTIDE_GROUP_ANALYSIS.begin();
  while( P != PEPTIDE_GROUP_ANALYSIS.end() ){
    if( (*P).validAnalysis ){
      DATA.insert( make_pair( log10( (*P).AVERAGE_PEPTIDE_RATIO), 0 ) );
    }
    P++;
  }
  
  /////////////////////////////////
  // start statistical analysis:
  char buffer[255];
  sprintf(buffer, "ratiolizer_gauss_fit_PEPTIDE_LEVEL");
  EM_fitter* ANA = new EM_fitter(buffer);
  ANA->set_DATA( &DATA );
  ANA->start_EM_1COMP_analysis( DATA_PRECENTILE );
  
  // get Gauss parameters:
  CURRENT_GAUSS.mean = ANA->get_G1_mean(); 
  double POT_MEAN = pow( 10, CURRENT_GAUSS.mean ); 
  CURRENT_GAUSS.variance = ANA->get_G1_variance();
  
  
  // computer first the half area:
  double TOT_AREA = 0.0;
  double Y = 1.0;
  double BIN = 0.001;
  double X = CURRENT_GAUSS.mean;
  while( Y > 0.0 ){
    Y = ANA->get_GAUSS_VALUE( CURRENT_GAUSS.mean, CURRENT_GAUSS.variance, X );
    TOT_AREA += ( Y * BIN );
    X += BIN;
  }
  
  // compute now the peptide probabilities:
  P = PEPTIDE_GROUP_ANALYSIS.begin();
  while( P != PEPTIDE_GROUP_ANALYSIS.end() ){
    
    if( (*P).validAnalysis ){

      double THIS_A = 0.0;
      double LOG_R = log10( (*P).AVERAGE_PEPTIDE_RATIO );
      double tmp = fabs( LOG_R - CURRENT_GAUSS.mean );
      double END = CURRENT_GAUSS.mean + tmp;
      X = CURRENT_GAUSS.mean;
      while( X <= END ){
        Y = ANA->get_GAUSS_VALUE( CURRENT_GAUSS.mean, CURRENT_GAUSS.variance, X );
        THIS_A += ( Y * BIN );
        X += BIN;
      }
    
      // set probability:
      double DELTA = TOT_AREA - THIS_A;
      if( DELTA < 0.0 ){
        DELTA = 0.0;
      }
      
      double PROB = 1.0 - DELTA / TOT_AREA;
      (*P).P_RATIO_VALUE = PROB;
      
      ////////////////////////////////////
      // normalize now the ratio according 
      // to the shift or the gauss distribution:
      (*P).NORMALIZED_AVERAGE_PEPTIDE_RATIO = (*P).AVERAGE_PEPTIDE_RATIO / POT_MEAN;
    }
    P++;
  }
  
  delete ANA;
  ANA = NULL;
  
}

/////////////////////////////////////////////////////////////////////
// compute probability of significant ratio 
double MS1_feature_ratiolizer::set_Probability( double RATIO ){
  
  // computer first the half area:
  EM_fitter ANA;
  double TOT_AREA = 0.0;
  double Y = 1.0;
  double BIN = 0.001;
  double X = CURRENT_GAUSS.mean;
  while( Y > 0.0 ){
    Y = ANA.get_GAUSS_VALUE( CURRENT_GAUSS.mean, CURRENT_GAUSS.variance, X );
    TOT_AREA += ( Y * BIN );
    X += BIN;
  }
  
  double THIS_A = 0.0;
  double LOG_R = log10( RATIO );
  double tmp = fabs( LOG_R - CURRENT_GAUSS.mean);
  double END = CURRENT_GAUSS.mean + tmp;
  X = CURRENT_GAUSS.mean;
  while( X <= END ){
    Y = ANA.get_GAUSS_VALUE( CURRENT_GAUSS.mean, CURRENT_GAUSS.variance, X );
    THIS_A += ( Y * BIN );
    X += BIN;
  }
  
  // set probability:
  double DELTA = TOT_AREA - THIS_A;
  if( DELTA < 0.0 ){
    DELTA = 0.0;
  }
  
  return ( 1.0 - DELTA / TOT_AREA ); 
}

  
  
/////////////////////////////////////////////////////////////////////
// show peptide group analysis
void MS1_feature_ratiolizer::show_peptide_group_analysis(){

  vector< PEPTIDE_RATIO>::iterator P = PEPTIDE_GROUP_ANALYSIS.begin(); 

  while( P != PEPTIDE_GROUP_ANALYSIS.end()){
    PEPTIDE_RATIO* TMP = &(*P);
    printf("\n*RATIO=%f, PROB=%f:", TMP->AVERAGE_PEPTIDE_RATIO, TMP->P_RATIO_VALUE);
    TMP->TARGET->show_members();
  
    P++;
  }
}

/////////////////////////////////////////////////////////////////////
// write peptide group analysis to XML file:
void MS1_feature_ratiolizer::write_peptide_group_analysis_2_XML(ofstream* WRITER){
    
  char buffer[254];

  sprintf(buffer,"<AllFeatureRatios nb=\"%d\">\n", PEPTIDE_GROUP_ANALYSIS.size());  
  WRITER->write(buffer,strlen(buffer));

  vector< PEPTIDE_RATIO>::iterator P = PEPTIDE_GROUP_ANALYSIS.begin();
  while( P != PEPTIDE_GROUP_ANALYSIS.end() ){
    write_PEPTIDE_GROUP_ANALYSIS_2_XML( &(*P), WRITER );
    P++;
  }
  sprintf(buffer,"</AllFeatureRatios>\n");  
  WRITER->write(buffer,strlen(buffer));
  
  WRITER = NULL;
}


/////////////////////////////////////////////////////////////////////
// write PEPTIDE_GROUP_ANALYSIS group out:
void MS1_feature_ratiolizer::write_PEPTIDE_GROUP_ANALYSIS_2_XML( PEPTIDE_RATIO* ratio, ofstream* WRITER){

  char buffer[254];
  
  sprintf(buffer,"<FeatureRatio id=\"%d\">\n", ratio->ID);  
  WRITER->write(buffer,strlen(buffer));
  
  // write the feature(s):
  LC_MS_XML_writer out;
  
  vector<feature*>::iterator fea = ratio->TARGET->get_members_list_start();
  
  if( peptide_ratio_analyzer::RATIO_TARGET_LC_MS == -1 ){

    sprintf(buffer,("<deltaMZGroup>\n") );  
    WRITER->write(buffer,strlen(buffer));
    
    sprintf(buffer,("<root>\n") );  
    WRITER->write(buffer,strlen(buffer));
    out.write_feature_2_XML_tag( *fea, WRITER );
    sprintf(buffer,("</root>\n") );  
    WRITER->write(buffer,strlen(buffer));
    
    // get the partner:
    fea++;
    if( fea != ratio->TARGET->get_members_list_end() ){
      sprintf(buffer,("<partner>\n") );  
      WRITER->write(buffer,strlen(buffer));
      out.write_feature_2_XML_tag( *fea, WRITER );
      sprintf(buffer,("</partner>\n") );  
      WRITER->write(buffer,strlen(buffer));
    }
    
    sprintf(buffer,("</deltaMZGroup>\n"));  
    WRITER->write(buffer,strlen(buffer));
    
  }
  else{
    out.write_feature_2_XML_tag( *fea, WRITER );
    
  }
  
  
  sprintf(buffer,"</FeatureRatio>\n");  
  WRITER->write(buffer,strlen(buffer));  
  WRITER = NULL;
}

//////////////////////////////////////////////////////////////
// print all peptide ratios to simple text file
void MS1_feature_ratiolizer::print_text_peptide_analysis(){

  string file = "ANALYSIS_";
  read_param* def = new read_param();    
  file = file + def->search_tag("MY PROJECT NAME");
  if( file[ file.size() - 1] != '/' ){
    file += "/";
  }

  char text_buffer[500];
  ofstream* WRITER;
  file = file + "all_identifed_peptide_ratios.txt";
  WRITER = new ofstream();
  WRITER->open(file.c_str(), ofstream::out);  
  
  // print a header:
  sprintf(text_buffer, "Mr\t#Chrg.states\tSQ\tPep.Prob.\tAC\tRatio\tStdev\tNorm.Ratio\n" );
  WRITER->write(text_buffer,strlen(text_buffer));
  
  vector< PEPTIDE_RATIO>::iterator ST = PEPTIDE_GROUP_ANALYSIS.begin();
  while( ST != PEPTIDE_GROUP_ANALYSIS.end() ){
    
    PEPTIDE_RATIO* TMP = &(*ST);
    peptide_DELTA_group* PEP = TMP->TARGET;
    ms2_info* INFO = PEP->get_BEST_MS2_SCAN();

    // print only those which were identified:
    if( INFO != NULL ){
      // print first molecular parameters
      sprintf(text_buffer, "%0.3f\t%d\t", PEP->get_MR(), PEP->get_nb_members() );
      WRITER->write(text_buffer,strlen(text_buffer));
      // print ID:
      sprintf(text_buffer, "%s\t%0.2f\t%s\t", INFO->get_TOTAL_SQ().c_str(), INFO->get_PEP_PROB(), INFO->get_AC().c_str());
      WRITER->write(text_buffer,strlen(text_buffer));
      // ratio stuff:
      if( TMP->validAnalysis ){
        sprintf(text_buffer, "%0.2f\t%0.2f\t%0.2f\n", TMP->AVERAGE_PEPTIDE_RATIO, TMP->AVERAGE_PEPTIDE_RATIO_STD, TMP->NORMALIZED_AVERAGE_PEPTIDE_RATIO );
        WRITER->write(text_buffer,strlen(text_buffer));
      }
      else{
        sprintf(text_buffer, "NA\tNA\tNA\n" );
        WRITER->write(text_buffer,strlen(text_buffer));
      }
    }
    
    ST++; 
  }
  
  printf("\t\t- Ratios of identifed peptides stored in '%s'\n", file.c_str() ); 
  delete WRITER;
  WRITER = NULL;
}



////////////////////////////////////////////////////////////////////////
// filter the peptide groups:
void MS1_feature_ratiolizer::filter_extracted_peptide_groups(){
  
  // show info:
  printf("\n\t\t Filter extracted peptide groups (%d) by...\n", PEPTIDE_GROUPS.size());
  if( TRYPTIC_PEPTIDE_CHECK ){
    printf("\t\t\t\t- Tryptic peptide filter\n");
  }
  if( MODIFICATION_TIC_PEPTIDE_INTEGRATION ){
    printf("\t\t\t\t- Integration of TIC over peptide modifications\n");
  }
  
  vector<peptide_DELTA_group>::iterator ST = PEPTIDE_GROUPS.begin();
  while( ST != PEPTIDE_GROUPS.end() ){
    
    /////////////////////////////
    // apply if option on the methionine
    // oxidation assembly
    if( MODIFICATION_TIC_PEPTIDE_INTEGRATION ){
      apply_methionine_oxidation_assembly( &(*ST) );
    }
    
    /////////////////////////////
    // apply peptide filter:
    if( !check_peptide_group_valid( &(*ST) ) ){
      ST = PEPTIDE_GROUPS.erase( ST );
    }
    else{
      ST++;
    }
  }
  
  
}


////////////////////////////////////////////////////////////////////////
// check if this is a good peptide delta group
bool MS1_feature_ratiolizer::check_peptide_group_valid( peptide_DELTA_group* IN ){

  /////////////////////////////////
  // check if its a non tryptic peptide:
  ms2_info* INFO = IN->get_BEST_MS2_SCAN();
  if( TRYPTIC_PEPTIDE_CHECK && INFO ){
    if( INFO->get_TRYPTIC_STATE() != 2 ){
      return false;
    }
  }
  
  
  if( RATIO_TARGET_LC_MS == -1){
    
    /////////////////////////////////
    // check the members in the peptide group for 
    // labeling peptides 
    // -> if only 1 feature then can build ratio
    // discard this peptide_Delta_group:
    if( IN->get_nb_members() <= 1 ){
  
      // only enabl to save those with good id:
      if( INFO ){
        if( !MS1_feature_ratiolizer::SAVE_SINGLE_PROTEINS ){ 
          return false;
        }
      }
      else{
        return false;
      }
    }
    
  }
  
  return true;
}



/////////////////////////////////////////////////////////////////////
// apply if option on the methionine
// oxidation assembly
void MS1_feature_ratiolizer::apply_methionine_oxidation_assembly( peptide_DELTA_group* IN ){
  // set the peptide modifications:
  IN->set_PEPTIDE_MODIFICATIONS();
  // build full ion current over peptide modification vairants
  IN->build_modification_resolved_feature_areas();

}

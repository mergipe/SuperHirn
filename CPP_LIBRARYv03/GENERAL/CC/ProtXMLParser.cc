///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//

#define USE_PROT_XML_PARSER
#include "GENERAL_USE.h"


// **********************************************************************//
//  INTERACT_parser:
//  extracts all the search results from the different output
//  files in the sub directory
//  
//
// **********************************************************************//



////////////////////////////////////////////////
// constructor for the object INTERACT_MS_ProtXMLParser:
ProtXMLParser::ProtXMLParser(string IN) : GeneralXMLParser(IN){

   // check for the correct file name, either "_c.xml" or "_p.xml"
  completeProtXMLFile();
 
  // read parameters from "param.def"
  read_param* def = new read_param();
  // FT feature mono-isotopic toggle correction:
  // def->search_tag("FT monoisotopic toggle correction", &FT_MONO_TOGGLE_CORRECTION);
  // MS2 mass tolerance:
  // def->search_tag("MS1 m/z tolerance", &MZ_TOL);
  
  delete def;
  def = NULL;
  
  
  currentProteinGroup = NULL;
}

////////////////////////////////////////////////
// constructor for the object INTERACT_MS_ProtXMLParser:
ProtXMLParser::ProtXMLParser( ) : GeneralXMLParser( ){
  
  // read parameters from "param.def"
  read_param* def = new read_param();
  // FT feature mono-isotopic toggle correction:
  // def->search_tag("FT monoisotopic toggle correction", &FT_MONO_TOGGLE_CORRECTION);
  // MS2 mass tolerance:
  // def->search_tag("MS1 m/z tolerance", &MZ_TOL);
  
  delete def;
  def = NULL;
  
  currentProteinGroup = NULL;
}




//////////////////////////////////////////////////
// class desctructor
ProtXMLParser::~ProtXMLParser(){
  proteinGroups.clear();
  currentProteinGroup = NULL;
}


//////////////////////////////////////////////////
// starts the parsing process of a directory with protxml files:
void ProtXMLParser::parseProtXMLFileDirectory( string dir ){
  file_sys accessor;
  accessor.read_dir( dir, ".xml");
  char* file = accessor.get_next_file();
  while(file != NULL){   
    // set file:
    setXMLParsingFile( file );
    // parse it
    parseProtXMLFile();
    // next protXML file:
    file = accessor.get_next_file();
  }
  
}


//////////////////////////////////////////////////
// starts the parsing process of an protxml file:
void ProtXMLParser::parseProtXMLFile(){
  
  
  if( openXMLFile( ) ){
    
    // get the element of the ms search summary and go through each of them:
    TiXmlNode* ALL_PROTEIN_SUMMARY = this->getXMLDocument( )->FirstChild( "protein_summary" );
    
    TiXmlNode* PROTEIN_GROUP;
    if( ALL_PROTEIN_SUMMARY != NULL ){
      for( PROTEIN_GROUP = ALL_PROTEIN_SUMMARY->FirstChild( "protein_group" ) ; PROTEIN_GROUP; PROTEIN_GROUP = PROTEIN_GROUP->NextSibling() ){
        
        TiXmlNode* protein;
        for( protein = PROTEIN_GROUP->FirstChild("protein"); protein; protein = protein->NextSibling() ){
        
          // extract protein summary info:
          extractProteinGroupInfo( protein );
          
          TiXmlNode* PEPTIDES;
          for( PEPTIDES = protein->FirstChild("peptide"); PEPTIDES; PEPTIDES = PEPTIDES->NextSibling() ){
            parsePeptide( PEPTIDES );
          }
        
          addProteinGroup( currentProteinGroup );
          delete currentProteinGroup;
          currentProteinGroup = NULL;

        }
        
      }
      //show_summary();
    }
    else{
      printf("\t\t**** No MS2 data available, skip file... \n" );
    }
  }
   
}


//////////////////////////////////////////////////////////////////
// add a protein to the map:
void ProtXMLParser::addProteinGroup( MS2ProteinGroup* prot){
  
  map<string, MS2ProteinGroup>::iterator F = proteinGroups.find( currentProteinGroup->getProteinName() );
  if( F != proteinGroups.end() ){
    if( F->second.getProteinProbability() < prot->getProteinProbability() ){
      F->second.setProteinProbability( prot->getProteinProbability()); 
    }
    
    // add the peptides
    map<string, MS2Peptide>::iterator P = prot->getPeptidesStart();
    while( P != prot->getPeptidesEnd() ){
      F->second.addPeptide( &(P->second) );      
      P++;
    }
  }
  else{
    proteinGroups.insert( make_pair( currentProteinGroup->getProteinName(), *currentProteinGroup ) );
  }
}
  
  

//////////////////////////////////////////////////////////////////
// extract protein summary info:
void ProtXMLParser::extractProteinGroupInfo( TiXmlNode* iProtNode){

  if( iProtNode != NULL ){
    TiXmlElement* THIS_E = iProtNode->ToElement();
    string Pname = THIS_E->Attribute("protein_name");
    double Pprob = atof( THIS_E->Attribute("probability") );
    double Pcov = 0;
    if( THIS_E->Attribute("percent_coverage") != NULL )
      Pcov = atof( THIS_E->Attribute("percent_coverage") );
    int PnbP = 0;
    if( THIS_E->Attribute("total_number_peptides") != NULL )
      PnbP = atof( THIS_E->Attribute("total_number_peptides") );
  
    MS2ProteinGroup* prot = new MS2ProteinGroup( Pname, Pprob, Pcov, PnbP);
    currentProteinGroup = prot;
    prot = NULL;
    
  }
}


//////////////////////////////////////////////////////////////////
// parse peptide node
void ProtXMLParser::parsePeptide( TiXmlNode* peptide ){

  TiXmlElement* peptideE = peptide->ToElement();
  
  // parameters to parse out from the peptide:
  string SQ;
  int CHRG;
  double weight = 0;
  double pepProb = 0;

  SQ = peptideE->Attribute("peptide_sequence");
  CHRG = atoi( peptideE->Attribute("charge") );
  weight = atof( peptideE->Attribute("weight") );
  pepProb = atof( peptideE->Attribute("nsp_adjusted_probability") );
  
  //printf("\t\t-- %s, %d, %0.2f, %0.2f\n", SQ.c_str(), CHRG, weight, pepProb);
  // add teh peptide:
  currentProteinGroup->addPeptide( SQ, CHRG, weight, pepProb );

}

//////////////////////////////////////////////////////////////////
// find the most likely peptide for a ms2 scan
void ProtXMLParser::checkProteinBeloning( ms2_info* ms2Scan ){
  
  if( !proteinGroups.empty() ){
    
    map<string, double>::iterator AC = ms2Scan->get_ALL_AC_START();
    while( AC != ms2Scan->get_ALL_AC_END() ){
      
      map< string, MS2ProteinGroup>::iterator P = proteinGroups.find( AC->first );
      if( P != getMS2ProteinGroupEND() ){
        double prob = P->second.getPeptideProb( ms2Scan->get_SQ() );
        AC->second = prob;
        AC++;
      }
      else{
        ms2Scan->getACListRefr()->erase( AC++ ); 
      }
    }
  }
  
}




////////////////////////////////////////////////////////////////
// check for the correct file name, either "_c.xml" or "_p.xml"
void ProtXMLParser::completeProtXMLFile(){
  
  file_sys CHECK;
  string TMP = getXMLParsingFile( );
  
  // remove the last _p
  string suffix = "_p";
  if( TMP.rfind( suffix ) != string::npos ){
    TMP.erase( TMP.rfind( suffix ), TMP.size( ) - TMP.rfind( suffix ) );
  }

  // remove the last _c
  suffix = "_c";
  if( TMP.rfind( suffix ) != string::npos ){
    TMP.erase( TMP.rfind( suffix ), TMP.size( ) - TMP.rfind( suffix ) );
  }
  
  if( CHECK.check_file_exists_in_Dir( TMP , ".xml" ) ){
    setXMLParsingFile( CHECK.get_file_name_in_Dir( TMP , ".xml" ) );
  }
  
}

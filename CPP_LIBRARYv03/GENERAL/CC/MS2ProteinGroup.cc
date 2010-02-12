///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS MS2ProteinGroup:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_MS2_PROTEIN_GROUP
#include "GENERAL_USE.h"




////////////////////////////////////////////////
// constructor for the object MS2ProteinGroup:
MS2ProteinGroup::MS2ProteinGroup(string iproteinName, double iproteinProbability, double iSQcoverage,int inbPeptides){
  proteinName = iproteinName;
  proteinProbability = iproteinProbability;
  SQcoverage = iSQcoverage;
  nbPeptides = inbPeptides;
  
}

//////////////////////////////////////////////////
// class desctructor of MS2ProteinGroup
MS2ProteinGroup::~MS2ProteinGroup(){
  peptides.clear();
}

//////////////////////////////////////////////////
// class copy constructor of MS2ProteinGroup
MS2ProteinGroup::MS2ProteinGroup(const MS2ProteinGroup& tmp){
  proteinName = tmp.proteinName;
  proteinProbability = tmp.proteinProbability;
  SQcoverage = tmp.SQcoverage;
  nbPeptides = tmp.nbPeptides;
  peptides = tmp.peptides;
  
}

//////////////////////////////////////////////////
// class copy constructor of MS2ProteinGroup
MS2ProteinGroup::MS2ProteinGroup(const MS2ProteinGroup* tmp){
  proteinName = tmp->proteinName;
  proteinProbability = tmp->proteinProbability;
  SQcoverage = tmp->SQcoverage;
  nbPeptides = tmp->nbPeptides;
  peptides = tmp->peptides;
}

//////////////////////////////////////////////////
// copy constructor:
MS2ProteinGroup& MS2ProteinGroup::operator=(const MS2ProteinGroup& tmp){
  proteinName = tmp.proteinName;
  proteinProbability = tmp.proteinProbability;
  SQcoverage = tmp.SQcoverage;
  nbPeptides = tmp.nbPeptides;
  peptides = tmp.peptides;
  return *this;
}
    


/////////////////////////////////////////////////
// get the peptide probability for a SQ
double MS2ProteinGroup::getPeptideProb( string sq ){
 
  map<string, MS2Peptide>::iterator F = peptides.find( sq );
  if( F != peptides.end() ){
    return F->second.pepProb;
  }
  
  return 0.0;
}


/////////////////////////////////////////////////
// add a peptide
void MS2ProteinGroup::addPeptide(string iSQ, int iZ, double iW, double iProb){
  
  MS2Peptide pep;

  pep.pepSQ = iSQ;
  pep.pepZ = iZ;
  pep.pepWeight = iW;
  pep.pepProb = iProb;
  
  peptides.insert( make_pair( pep.pepSQ, pep ) );
}

/////////////////////////////////////////////////
// add a peptide
void MS2ProteinGroup::addPeptide(MS2Peptide* iPep){
  
  map<string, MS2Peptide>::iterator F = peptides.find( iPep->pepSQ );
  if( F != peptides.end()){
    if( iPep->pepProb > F->second.pepProb){
      F->second.pepProb = iPep->pepProb;
    }
  }
  else{
    peptides.insert( make_pair( iPep->pepSQ, *iPep) );
  }
  nbPeptides++;
  
}


/////////////////////////////////////////////////
// show info:
void MS2ProteinGroup::show_info(){
  printf("\t\t%s, Prob=%0.2f, Perc=%0.2f,Nb.Peps=%d\n\t\t", proteinName.c_str(), proteinProbability, SQcoverage, nbPeptides); 
  map<string, MS2Peptide>::iterator P = peptides.begin();
  while( P != peptides.end() ){
    cout<<P->first<<"+";
    P++; 
  }
  cout<<endl;
  
}

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


#ifndef _MS2_PROTEIN_GROUP_H
#define _MS2_PROTEIN_GROUP_H



struct MS2Peptide{

  string pepSQ;
  int pepZ;
  double pepWeight;
  double pepProb;
  
};


class MS2ProteinGroup{

    
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  ////////////////////////////////////////////////
  // declaration of the public members:
  
  string proteinName;
  double proteinProbability;
  double SQcoverage;
  int nbPeptides;
  
  map<string, MS2Peptide> peptides;
  
  
public:
  
  // class destructor
  ~MS2ProteinGroup();
  
  // class constructor
  MS2ProteinGroup(string iproteinName, double iproteinProbability, double iSQcoverage,int inbPeptides);
  // class copy constructor
  MS2ProteinGroup(const MS2ProteinGroup&);
  // class copy constructor
  MS2ProteinGroup(const MS2ProteinGroup*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  MS2ProteinGroup& operator=(const MS2ProteinGroup&);
  bool operator==(const MS2ProteinGroup&);
  MS2ProteinGroup& operator<=(const MS2ProteinGroup&);
  MS2ProteinGroup& operator>=(const MS2ProteinGroup&);
  MS2ProteinGroup& operator<(const MS2ProteinGroup&);
  MS2ProteinGroup& operator>(const MS2ProteinGroup&);
  
  
  // show info:
  void show_info();
  
  // add a peptide
  void addPeptide(string iSQ, int iZ, double iW, double iProb);
  void addPeptide(MS2Peptide*);

  // get the peptide probability for a SQ
  double getPeptideProb( string sq );

  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  string getProteinName(){return proteinName;};
  double getProteinProbability(){return proteinProbability;};
  void setProteinProbability(double iProb){proteinProbability = iProb;};
  double getSQcoverage(){return SQcoverage;};
  int getNBPeptides(){return nbPeptides;};
  map<string, MS2Peptide>::iterator getPeptidesStart(){return peptides.begin();};
  map<string, MS2Peptide>::iterator getPeptidesEnd(){return peptides.end();};
  

};

#endif

    

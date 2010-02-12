///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS consensIsotopePattern:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_CONSENS_ISOTOPE_PATTERN
#include "GENERAL_USE.h"


double consensIsotopePattern::FT_MZ_TOLERANCE;


////////////////////////////////////////////////
// constructor for the object consensIsotopePattern:
consensIsotopePattern::consensIsotopePattern(){
}

//////////////////////////////////////////////////
// class desctructor of consensIsotopePattern
consensIsotopePattern::~consensIsotopePattern(){
  
  isotopesTrace.clear(); 
  mzIsotopesStDev.clear(); 
  intensIsotopesStDev.clear(); 
  rawIsotopes.clear();
  
}

//////////////////////////////////////////////////
// class copy constructor of consensIsotopePattern
consensIsotopePattern::consensIsotopePattern(const consensIsotopePattern& tmp){
  isotopesTrace = tmp.isotopesTrace; 
  mzIsotopesStDev = tmp.mzIsotopesStDev; 
  intensIsotopesStDev = tmp.intensIsotopesStDev; 
  rawIsotopes = tmp.rawIsotopes;
}

//////////////////////////////////////////////////
// class copy constructor of consensIsotopePattern
consensIsotopePattern::consensIsotopePattern(const consensIsotopePattern* tmp){
  isotopesTrace = tmp->isotopesTrace; 
  mzIsotopesStDev = tmp->mzIsotopesStDev; 
  intensIsotopesStDev = tmp->intensIsotopesStDev; 
  rawIsotopes = tmp->rawIsotopes;
}

//////////////////////////////////////////////////
// copy constructor:
consensIsotopePattern& consensIsotopePattern::operator=(const consensIsotopePattern& tmp){
  isotopesTrace = tmp.isotopesTrace; 
  mzIsotopesStDev = tmp.mzIsotopesStDev; 
  intensIsotopesStDev = tmp.intensIsotopesStDev; 
  rawIsotopes = tmp.rawIsotopes;
  return *this;
}
    

  
/////////////////////////////////////////////////
// order an isotope trace in the correct cluster:
void consensIsotopePattern::addIsotopeTrace( double mz, double intens){
  
  map<double, pair< vector<double>, vector<double> > >::iterator F = rawIsotopes.lower_bound( mz );
  bool match = false;
  if( F != rawIsotopes.end() ){
  
    // compute teh delta:
    if( simple_math::compareMassValuesAtPPMLevel( mz, (*F).first, consensIsotopePattern::FT_MZ_TOLERANCE) ){
      (*F).second.first.push_back(mz);
      (*F).second.second.push_back(mz);
      match = true;
    }
    else if( F != rawIsotopes.begin() ){
      F--;
      if( simple_math::compareMassValuesAtPPMLevel( mz, (*F).first, consensIsotopePattern::FT_MZ_TOLERANCE ) ){
        (*F).second.first.push_back(mz);
        (*F).second.second.push_back(mz);
        match = true;
      }
    
    }
  
  }

  if( !match ){
    vector< double > mzTmp;
    mzTmp.push_back( mz );
    vector< double > intensTmp;
    intensTmp.push_back( intens );
    rawIsotopes.insert( make_pair( mz, make_pair( mzTmp, intensTmp ) ) );
  }
  
  
}


/////////////////////////////////////////////////
// construc the consenus pattern:
void consensIsotopePattern::constructConsusPattern( ){
  
  
  map<double, pair< vector<double>, vector<double> > >::iterator I = rawIsotopes.begin();
  while( I != rawIsotopes.end() ){
    // condens a isotope peak trace:
    condensIsotopePattern( &(*I).second );    
    I++;
  }
  
}


//////////////////////////////////////////////////
// condens the pattern, make averge peaks from the traces:
void consensIsotopePattern::condensIsotopePattern( pair< vector<double>, vector<double> >* in){
  simple_math math;

  // mz 
  pair<double, double> mz = math.AVERAGE_and_STDEV( &(in->first) );
  // intens:
  pair<double, double> intens = math.AVERAGE_and_STDEV( &(in->second) );
  
  isotopesTrace.insert( make_pair( mz.first, intens.first) ); 
  mzIsotopesStDev.push_back( mz.second ); 
  intensIsotopesStDev.push_back( intens.second ); 
  

}





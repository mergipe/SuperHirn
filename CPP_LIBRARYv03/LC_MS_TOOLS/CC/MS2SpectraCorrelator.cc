///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS MS2SpectraCorrelator:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_MS2_SPECTRUM_CORRELATOR
#include "LC_MS_TOOLS_USE.h"

bool MS2SpectraCorrelator::DEBUG_MS2_CORRELATOR = false;
double MS2SpectraCorrelator::MIN_NB_OVERLAP = 0.2;

////////////////////////////////////////////////
// constructor for the object MS2SpectraCorrelator:
MS2SpectraCorrelator::MS2SpectraCorrelator(){
  // target = NULL;
  // checker = NULL;
}

////////////////////////////////////////////////
// constructor for the object MS2SpectraCorrelator:
MS2SpectraCorrelator::MS2SpectraCorrelator(MS2_feature* iTarget){
  setTargetSpectra(iTarget);
  // checker = NULL;
}


////////////////////////////////////////////////
// set teh input checker spectra
void MS2SpectraCorrelator::setMS2CheckSpectra(MS2ConsensusSpectrum* iChecker){
  
  
  checkerFragments.clear();
  MS2SpectrumProcessor proc;
  proc.computeMS2SpectrumApexThreshold( iChecker );
  double thresh = proc.getFragmentIntensThreshold(); 
  
  checkerIntensityMap.clear();
  
  // go through the target MS2 scan and find corresponding
  // fragments in the checker:  
  map<double, MS2Fragment>::iterator tFrag = iChecker->getMS2FragmentPeakStart();
  while( tFrag != iChecker->getMS2FragmentPeakEnd() ){
    MS2Fragment* f = &(tFrag->second);
    if( f->getFragmentPeakArea() > thresh ){
      checkerIntensityMap.push_back( f->getFragmentPeakArea() );
      checkerFragments.insert( make_pair( f->getFragmentMz() ,f) );
    }
    tFrag++;
  }
  
  // sort the map descending:
  sort( checkerIntensityMap.begin(), checkerIntensityMap.end() );
  
  
}



//////////////////////////////////////////////////
// class desctructor of MS2SpectraCorrelator
MS2SpectraCorrelator::~MS2SpectraCorrelator(){
  targetIntensityMap.clear();
  checkerIntensityMap.clear();
  targetFragments.clear();
  checkerFragments.clear();
  extractedMS2FragmentPairs.clear();
}

//////////////////////////////////////////////////
// class copy constructor of MS2SpectraCorrelator
MS2SpectraCorrelator::MS2SpectraCorrelator(const MS2SpectraCorrelator& tmp){
  /*
  target = tmp.target;
  checker = tmp.checker;
  extractedMS2FragmentPairs = tmp.extractedMS2FragmentPairs;
   */
  //correlation = tmp.correlation;
}

//////////////////////////////////////////////////
// class copy constructor of MS2SpectraCorrelator
MS2SpectraCorrelator::MS2SpectraCorrelator(const MS2SpectraCorrelator* tmp){
  /*
  target = tmp->target;
  checker = tmp->checker;
  extractedMS2FragmentPairs = tmp->extractedMS2FragmentPairs;
   */
  //correlation = tmp->correlation;
}

//////////////////////////////////////////////////
// copy constructor:
MS2SpectraCorrelator& MS2SpectraCorrelator::operator=(const MS2SpectraCorrelator& tmp){
  /*
  target = tmp.target;
  checker = tmp.checker;
  extractedMS2FragmentPairs = tmp.extractedMS2FragmentPairs;
  correlation = tmp.correlation;
   */
  return *this;
}

//////////////////////////////////////////////////////////////////
// set teh target spectrum
void MS2SpectraCorrelator::setTargetSpectra(MS2_feature* iTarget){
  targetFragments.clear();
  // target = iTarget;
  
  MS2SpectrumProcessor proc;
  proc.computeMS2SpectrumApexThreshold( iTarget );
  double thresh = proc.getFragmentIntensThreshold(); 
    
  targetIntensityMap.clear();
  
  // go through the target MS2 scan and find corresponding
  // fragments in the checker:  
  map<double, MS2Fragment>::iterator tFrag = iTarget->getMS2FragmentPeakStart();
  while( tFrag != iTarget->getMS2FragmentPeakEnd() ){
    MS2Fragment* f = &(tFrag->second);
    if( f->getFragmentPeakArea() > thresh ){
      targetIntensityMap.push_back( f->getFragmentPeakArea() );
      targetFragments.insert( make_pair( f->getFragmentMz() ,f) );
    }
    tFrag++;
  }
  
  // sort the map descending:
  sort( targetIntensityMap.begin(), targetIntensityMap.end() );

}

  


//////////////////////////////////////////////////////////////////
// comparison of an input spectrum to a set target spectrum
void MS2SpectraCorrelator::computeMS2SpectrumCorrelation(MS2_feature* iChecker){
  setMS2CheckSpectra( iChecker );
  computeMS2SpectrumCorrelation();
}



//////////////////////////////////////////////////////////////////
// comparison of an input spectrum to a set target spectrum
void MS2SpectraCorrelator::computeMS2SpectrumCorrelation(){
  
  mainAnalysis.overlapCorrelationScore = 0;
  mainAnalysis.intensityCorrelationScore = 0;
  mainAnalysis.mainCorrelationScore = 0;
  mainAnalysis.peakOverlap = 0;
  meanIntensTarget = 0;
  meanIntensChecker = 0;
  
  
  if( ( !targetFragments.empty() ) && (!checkerFragments.empty()) ){
    
    /*
    if( DEBUG ){
      cout<<"InputTarget = "<<target->getNbMS2Fragments()<<endl;
      cout<<"InputSearch = "<<checker->getNbMS2Fragments()<<endl;
    }
     */
    
    
    // extract fragment pairs from the scans:
    extractMS2FragmentPairs();
    
    
    // compute the overlapscore:
    mainAnalysis.peakOverlap = (double)extractedMS2FragmentPairs.size();
    mainAnalysis.nbTargetPeaks = (double)targetFragments.size();
    mainAnalysis.nbCheckerPeaks = (double)checkerFragments.size();
    
  
    if( ( mainAnalysis.nbCheckerPeaks < 5 ) || ( mainAnalysis.nbTargetPeaks < 5 ) ){
      mainAnalysis.mainCorrelationScore = 0;
    }
    else 
     if( ( MIN_NB_OVERLAP > mainAnalysis.peakOverlap / mainAnalysis.nbCheckerPeaks ) || ( MIN_NB_OVERLAP > mainAnalysis.peakOverlap /mainAnalysis.nbTargetPeaks ) ){
      mainAnalysis.mainCorrelationScore = 0;
    }
    else{
      mainAnalysis.overlapCorrelationScore = 2.0 * mainAnalysis.peakOverlap;
      mainAnalysis.overlapCorrelationScore /= ( mainAnalysis.nbTargetPeaks + mainAnalysis.nbCheckerPeaks);
    
      // compute intensity correlation score:
      computeSpearmanCorrelationIntensityScore();
      // compute kendall tau correlation
      //computeKendallCorrelationIntensityScore();
      
    
      // assemble the scores:
      mainAnalysis.mainCorrelationScore = ( mainAnalysis.peakOverlap *  mainAnalysis.overlapCorrelationScore );
      
    }    
      
  }
}


//////////////////////////////////////////////////////////////////
// extract fragment pairs from the scans:
void MS2SpectraCorrelator::extractMS2FragmentPairs(){
  
  extractedMS2FragmentPairs.clear();
  
  // go through the target MS/MS scan and find corresponding
  // fragments in the checker:  
  map<double, MS2Fragment*>::iterator tFrag = targetFragments.begin();
  while( tFrag != targetFragments.end() ){

    MS2Fragment* fragTarget = (tFrag->second);

    /*
    if( DEBUG ){
      cout<<fragTarget->getFragmentMz()<<" : ";      
    }
    */
    
    // find teh corresponding one:
    MS2Fragment* fragChecker = findMS2FragmentInMap( fragTarget, &checkerFragments );
    if( fragChecker != NULL){
      
      MS2FragmentPair newPair;
      newPair.targetMZ = fragTarget->getFragmentMz();
      newPair.checkerMZ = fragChecker->getFragmentMz();
      newPair.targetIntens = fragTarget->getFragmentPeakArea();
      newPair.checkerIntens = fragChecker->getFragmentPeakArea();

      // add pair to spectra correlation:
      addSpectralCorrelationFragment( newPair );
      
    }
    
    fragTarget = NULL;
    tFrag++;
  }
  
  
  
}


////////////////////////////////////////////////
// add pair to spectra correlation:
void MS2SpectraCorrelator::addSpectralCorrelationFragment(MS2FragmentPair newPair ){
  
  // get teh intensity ranks:  
  newPair.targetRank = getIntensityRank( &targetIntensityMap, newPair.targetIntens );
  newPair.checkerRank = getIntensityRank( &checkerIntensityMap, newPair.checkerIntens );
  
  meanIntensTarget += newPair.targetIntens;
  meanIntensChecker += newPair.checkerIntens;

  /*
  if( DEBUG ){
    printf("mz=%.3f::%.3f\nRank=%.0f::%.0f\n\n",
           newPair.targetMZ, newPair.checkerMZ, 
           newPair.targetRank, newPair.checkerRank);
  }
   */
  
  extractedMS2FragmentPairs.push_back( newPair );

}

///////////////////////////////////////////////////////////////////////////////////
// compute spearman correlation
// r = A / B, where
// A = sum[ ( Ri - Rmean)*(Si - Smean)]
// B = sqrt[sum( Ri - Rmean)^2] * sqrt[sum( Si - Smean)^2]
// Ri = rank of peak i in spectra A
// Si = rank of peak i in spectra B
// Rmean = rank of the mean
// Smean = rank of the mean
//
void MS2SpectraCorrelator::computeSpearmanCorrelationIntensityScore(){
  
  // go through all the matched peaks:
  vector<MS2FragmentPair>::iterator F = extractedMS2FragmentPairs.begin();
  
  double RC = 0;
  double DIV_A = 0;
  double DIV_B = 0;
  double NUM = 0;
  
  double nb = extractedMS2FragmentPairs.size();
  meanIntensTarget /= nb;
  meanIntensChecker /= nb;

  // get mean rank:
  double meanRankTarget = getIntensityRank( &targetIntensityMap, meanIntensTarget );
  double meanRankChecker = getIntensityRank( &checkerIntensityMap, meanIntensChecker );

  
  
  if( DEBUG_MS2_CORRELATOR ){
    
    printf( "mean target Rank: %.1f\n mean checker Rank: %.1f\n\n",
            meanRankTarget, meanRankChecker);
  }
  
  while( F != extractedMS2FragmentPairs.end()){
    
    if( DEBUG_MS2_CORRELATOR ){
    
      printf( "mass: %.2f - %.2f \n rank: %.0f - %.0f\n\n",
              F->targetMZ, F->checkerMZ, F->targetRank, F->checkerRank);
    }
    
    
    NUM += ( (F->targetRank - meanRankTarget) * ( F->checkerRank - meanRankChecker) );
    DIV_A += (F->targetRank - meanRankTarget)*(F->targetRank - meanRankTarget);
    DIV_B += (F->checkerRank - meanRankChecker)*(F->checkerRank - meanRankChecker);
    F++;
  }
  
  // take teh square root
  DIV_A = sqrt(DIV_A);
  DIV_B = sqrt(DIV_B);
  
  if(DIV_A*DIV_B > 0)
    // combine the factors:
    RC = NUM / ( DIV_A * DIV_B);
  
  mainAnalysis.intensityCorrelationScore = RC;
}


///////////////////////////////////////////////////////////////////////////////////
// compute kendall tau correlation
//
void MS2SpectraCorrelator::computeKendallCorrelationIntensityScore(){
  
  if( extractedMS2FragmentPairs.size() > 1 ){
    // go through all the matched peaks:
    vector<MS2FragmentPair>::iterator F = extractedMS2FragmentPairs.begin();
    map<int, MS2FragmentPair*> targetRanking;
    vector<int> checkerRanking;
    
    // create ranking:
    while( F != extractedMS2FragmentPairs.end()){
      targetRanking.insert( make_pair( (int)F->targetIntens, &(*F) ) );
      checkerRanking.push_back( (int)F->checkerIntens );    
      F++;
    }
    
    // compute the kedaull correlation:
    double nom = 0;
    sort( checkerRanking.begin(), checkerRanking.end() );
    map<int, MS2FragmentPair*>::iterator P = targetRanking.begin();
    // create ranking:
    while( P != targetRanking.end()){
      
      // find the rank:
      vector<int>::iterator I = find( checkerRanking.begin(), checkerRanking.end(), P->second->checkerIntens);
      double rank = I - checkerRanking.begin();
      nom += rank;  
      P++;
    }
    double denom = 0.5 * (double)checkerRanking.size() * ((double)checkerRanking.size() - 1);
    mainAnalysis.intensityCorrelationScore = (2.0 * nom / denom - 1.0);
  }
  else{
    mainAnalysis.intensityCorrelationScore = 0;
  }
}

////////////////////////////////////////////////
// get the intensity rank in an intensity map
int MS2SpectraCorrelator::getIntensityRank(vector<double>* map, double intensity){
  
  
  vector<double>::iterator F = lower_bound( map->begin(), map->end(), intensity );
  
  
  if( DEBUG_MS2_CORRELATOR ){
    cout<<intensity<<" "<<(*F)<<endl;
  }

  
  int n = F - map->begin();
  return ( map->size() - ( n ) );
  
}



////////////////////////////////////////////////////////
// find a corresponding MS2 fragment in the target spectrum
MS2Fragment* MS2SpectraCorrelator::findMS2FragmentInTarget( MS2Fragment* check ){
  
  double mass = check->getFragmentMz();
  multimap<double, MS2Fragment*>* searchMap = &targetFragments;

  // lower bound search:
  multimap<double, MS2Fragment*>::iterator F = searchMap->lower_bound( mass );

  
  ///////////////////////
  // collect a list of iterators with potential candidates:
  map< double, map< double, MS2Fragment* >::iterator > candidates;
  
  // scan lower mass tolerance:
  map< double, MS2Fragment* >::iterator I = F;
  
  if( I != searchMap->begin() ){
    I--;
  }
  
  while( simple_math::compareMassValuesAtPPMLevel( I->second->getFragmentMz(), mass, MS2ConsensusSpectrum::MS2_MZ_TOLERANCE) ){
    
    // compare 2 fragments to decided if they should be matched
    if( compareM2Fragments( (I->second) , check) ){
      candidates.insert( make_pair( fabs( I->second->getFragmentMz() - mass ) , I) );
    }
    
    if( I == searchMap->begin() ){
      break;
    }
    
    // next:
    I--;
  }
  
  
  // scan upper mass tolerance:
  I = F;
  if( ( I != searchMap->end() ) && ( I != searchMap->begin() ) ){

    while( simple_math::compareMassValuesAtPPMLevel( I->second->getFragmentMz(), mass, MS2ConsensusSpectrum::MS2_MZ_TOLERANCE ) ){
      
      // compare 2 fragments to decided if they should be matched
      if( compareM2Fragments( (I->second) , check) ){
        candidates.insert( make_pair( fabs( I->second->getFragmentMz() - mass ), I) );
      }
      
      I++;
      if( I == searchMap->end() ){
        break;
      }
    }
  }
  
  
  /////////////////////////////////////////////////////
  // find now the one with the best match:
  // i.e. take the one with the smallest Mz difference:
  if( !candidates.empty() ){
    return ((candidates.begin())->second->second);
  }
  
  return NULL;
  
}

////////////////////////////////////////////////////////
// find a corresponding MS2 fragment in a searchMap
MS2Fragment* MS2SpectraCorrelator::findMS2FragmentInMap( MS2Fragment* check, multimap<double, MS2Fragment*>* searchMap){
  
  double mass = check->getFragmentMz();
  
  // lower bound search:
  multimap<double, MS2Fragment*>::iterator F = searchMap->lower_bound( mass );
  
  
  ///////////////////////
  // collect a list of iterators with potential candidates:
  map< double, map< double, MS2Fragment* >::iterator > candidates;
  
  // scan lower mass tolerance:
  map< double, MS2Fragment* >::iterator I = F;
  
  if( I != searchMap->begin() ){
    I--;
  }
  
  double deltaMass = fabs(I->second->getFragmentMz() - mass);
  while( deltaMass <= 2* simple_math::getMassErrorAtPPMLevel( I->second->getFragmentMz(), MS2ConsensusSpectrum::MS2_MZ_TOLERANCE) ){
    
    // compare 2 fragments to decided if they should be matched
    if( compareM2Fragments( (I->second) , check) ){
      candidates.insert( make_pair( deltaMass , I) );
    }
    
    if( I == searchMap->begin() ){
      break;
    }
    
    deltaMass = fabs(I->second->getFragmentMz() - mass);
    
    // next:
    I--;
  }
  
  
  // scan upper mass tolerance:
  I = F;
  if( ( I != searchMap->end() ) && ( I != searchMap->begin() ) ){
    
    double deltaMass = fabs(I->second->getFragmentMz() - mass);
    while( deltaMass <= 2* simple_math::getMassErrorAtPPMLevel( I->second->getFragmentMz(), MS2ConsensusSpectrum::MS2_MZ_TOLERANCE)  ){
      
      // compare 2 fragments to decided if they should be matched
      if( compareM2Fragments( (I->second) , check) ){
        candidates.insert( make_pair( deltaMass,  I) );
      }
      
      I++;
      if( I == searchMap->end() ){
        break;
      }
      deltaMass = fabs(I->second->getFragmentMz() - mass);

    }
  }
  
  
  /////////////////////////////////////////////////////
  // find now the one with the best match:
  // i.e. take the one with the smallest Mz difference:
  if( !candidates.empty() ){
    return ((candidates.begin())->second->second);
  }
  
  return NULL;
  
}


///////////////////////////////////////////////////////////
// compare 2 fragments to decided if they should be matched
bool MS2SpectraCorrelator::compareM2Fragments(MS2Fragment* A, MS2Fragment* B){
    
  // check fragment mass difference:
  if( !simple_math::compareMassValuesAtPPMLevel( A->getFragmentMz(), B->getFragmentMz(), MS2ConsensusSpectrum::MS2_MZ_TOLERANCE ) ){
    return false;
  }
  

  return true;
  
}



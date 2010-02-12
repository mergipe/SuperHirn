///////////////////////////////////////////////////////////////////////////
//
//  PEAK DETECTION OF FOURIER TRANSFORME MS INSTRUMENT DATA
//
//  written by Markus Mueller, markus.mueller@imsb.biol.ethz.ch
//  ( and Lukas Mueller, Lukas.Mueller@imsb.biol.ethz.ch)
//  October 2005
//
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//

#define USE_BACKGROUND_CONTROL
#include "MS_PROCESSING_USE.h"


////////////////////////////////////////////////
// constructor for the object BackgroundControl:
BackgroundControl::BackgroundControl(){
  init();
}


////////////////////////////////////////////////
// initialization of the BackgroundControl classe:
void BackgroundControl::init(){
  
  // create a vector of intensity bin objects
  
  // first in the tr dimension:
  double trStart = LC_MS_XML_reader::TR_MIN;
  while( trStart <= LC_MS_XML_reader::TR_MAX ){
  
    // inner loop is the mzBins:
    map<double, BackgroundIntensityBin> mzArray;
    double mzStart = LC_MS_XML_reader::FEATURE_MZ_MIN;
    while( mzStart <= LC_MS_XML_reader::FEATURE_MZ_MAX ){
      
      BackgroundIntensityBin* bin = new BackgroundIntensityBin(mzStart,trStart);
      mzArray.insert( make_pair( mzStart, *bin ) );
      delete bin;
      bin = NULL;
    
      mzStart += BackgroundIntensityBin::MZ_BINS;
    }
    
    intensityBinMap.insert( make_pair (trStart, mzArray) );
    trStart += BackgroundIntensityBin::TR_BINS;
  }
  
}


//////////////////////////////////////////////////
// add peaks of a ms scan:
void BackgroundControl::addPeakMSScan( double TR, vector<ms_peak>* peakList ){
  
  map<double, map< double, BackgroundIntensityBin>  >::iterator F = findTrKey( TR );
  if( F != intensityBinMap.end() ){
    
    // find the mz bins:
    map< double, BackgroundIntensityBin>* mzMap = &(F->second); 
    
    vector<ms_peak>::iterator P = peakList->begin();
    while( P != peakList->end() ){
      
      ms_peak* peak = &(*P);
      
      // add the centroid peaks:
      vector<CentroidPeak>::iterator cen = peak->get_isotopic_peaks_start();
      while( cen != peak->get_isotopic_peaks_end() ){
        map< double, BackgroundIntensityBin>::iterator F_mz = findMzKey( cen->getMass(), mzMap );
        if(  F_mz != mzMap->end() ){
          F_mz->second.addMSPeak( peak );
        }
        cen++;
      }
      P++;
    }
    
  }
  
}


//////////////////////////////////////////////////
// add peaks of a ms scan:
void BackgroundControl::addPeakMSScan( double TR, list<CentroidPeak>* peakList ){
  
  map<double, map< double, BackgroundIntensityBin>  >::iterator F = findTrKey( TR );
  if( F != intensityBinMap.end() ){
    
    // find the mz bins:
    map< double, BackgroundIntensityBin>* mzMap = &(F->second); 
  
    list<CentroidPeak>::iterator mpi;
    for (mpi=peakList->begin();mpi!=peakList->end();++mpi) {
  
      map< double, BackgroundIntensityBin>::iterator F_mz = findMzKey( mpi->getMass(), mzMap );
      if(  F_mz != mzMap->end() ){
        F_mz->second.addIntensity( mpi->getIntensity() );
      }
    }
  
    
  }
  
}


//////////////////////////////////////////////////
// find a key in the m/z map:
map< double, BackgroundIntensityBin>::iterator BackgroundControl::findMzKey( double mz, map< double, BackgroundIntensityBin>* mzMap ){
  
  
  double constraint = BackgroundIntensityBin::MZ_BINS / 2.0;
  map<double, map< double, BackgroundIntensityBin>::iterator > outMap;
  
  map< double, BackgroundIntensityBin>::iterator F = mzMap->lower_bound( mz );
  if( F != mzMap->end() ){
    double delta = fabs( F->first - mz );
    if( delta <= constraint ){ 
      outMap.insert( make_pair( delta, F) );
    }
  }
  
  
  if( F != mzMap->begin() ){
    F--;
    double delta = fabs( mz - F->first );    
    if( delta <= constraint ){
      outMap.insert( make_pair( delta, F) );
    }
  }
  
  if( !outMap.empty() ){
    return outMap.begin()->second;
  }
  
  return mzMap->end();
}


//////////////////////////////////////////////////
// find a key in the intensity map:
map<double, map< double, BackgroundIntensityBin>  >::iterator BackgroundControl::findTrKey( double Tr ){
  
  double constraint = BackgroundIntensityBin::TR_BINS * 2;
  
  map<double, map<double, map< double, BackgroundIntensityBin>  >::iterator > outMap;
  map<double, map< double, BackgroundIntensityBin>  >::iterator F = intensityBinMap.lower_bound( Tr );
  if( F != intensityBinMap.end() ){
    double delta = fabs( Tr - F->first );    
    if( delta <= constraint ){
      outMap.insert( make_pair( delta, F) );
    }
    
  }

  if( F != intensityBinMap.begin() ){
    F--;

    double delta = fabs( Tr - F->first );    
    if( delta <= constraint ){
      outMap.insert( make_pair( delta, F) );
    }
  }
  
  if( !outMap.empty() ){
    return outMap.begin()->second;
  }

  return intensityBinMap.end();
}



//////////////////////////////////////////////////
// add a peak to the BackgroundControl
void BackgroundControl::addPeak( ms_peak* in){
  // find the corresponding retention time bin:
 
}


//////////////////////////////////////////////////
// get the background intensity level for a peak:
double BackgroundControl::getBackgroundLevel( ms_peak* in){
  return getBackgroundLevel( in->get_MZ(), in->get_retention_time()  );
}


//////////////////////////////////////////////////
// get the background intensity level for a peak:
double BackgroundControl::getBackgroundLevel( double mz, double tr){
  // find the corresponding retention time bin:
  map<double, map< double, BackgroundIntensityBin> >::iterator F = findTrKey( tr  );
  if( F != intensityBinMap.end() ){
    map< double, BackgroundIntensityBin>::iterator F2 = findMzKey( mz, &(F->second) );
    if( F2 != F->second.end() ){
      return F2->second.getMean();
    }
  }
  return -1.0;
}



//////////////////////////////////////////////////
// write out intensity maps:
void BackgroundControl::writeIntensityMaps(  ){
  
  map<double, map< double, BackgroundIntensityBin> >::iterator P1 = intensityBinMap.begin();
  while( P1 != intensityBinMap.end() ){

    map< double, BackgroundIntensityBin> ::iterator P2 = P1->second.begin();
    while( P2 != P1->second.end() ){
      P2->second.writeIntensityMap();
  
      P2++;
    }
    
    P1++;
  }
}

//////////////////////////////////////////////////
// process the intensity maps:
void BackgroundControl::processIntensityMaps(  ){
  
  map<double, map< double, BackgroundIntensityBin> >::iterator P1 = intensityBinMap.begin();
  while( P1 != intensityBinMap.end() ){
    
    map< double, BackgroundIntensityBin> ::iterator P2 = P1->second.begin();
    while( P2 != P1->second.end() ){
      
      P2->second.processIntensities( );
      P2++;
    }
    
    P1++;
  }
}


//////////////////////////////////////////////////
// gnuplot intensity maps:
void BackgroundControl::plotIntensityMaps(  ){
  
  char buffer[255];
  sprintf( buffer, "allBins");
  string tmp = buffer;
  data_plotter* PLOT2 = new data_plotter(tmp); 
  
  
    map<double, map< double, BackgroundIntensityBin> >::iterator P1 = intensityBinMap.begin();
  while( P1 != intensityBinMap.end() ){
    
    map< double, BackgroundIntensityBin> ::iterator P2 = P1->second.begin();
    while( P2 != P1->second.end() ){
      
      
      /*
      vector<double>* IntensityMap =  P2->second.getIntensityMap();

      if( IntensityMap->size() > 0 ){
        // sort the intensities:
        sort( IntensityMap->begin(), IntensityMap->end() );
        int count = 0; 
        map<double, double> PLOT_DATA;
        vector<double>::iterator P = IntensityMap->begin();
        while( P != IntensityMap->end() ){
          // print hist data:
          PLOT_DATA.insert( make_pair( (double)count,(*P) ) );  
          count++;
          P++;
        }
        
        sprintf( buffer, "Bin %0.0fDa | %0.0fmin, (%d points) ", P2->first, P1->first, PLOT_DATA.size());
        //PLOT->add_plot_data( &PLOT_DATA, buffer );
        PLOT->add_plot_data( &PLOT_DATA );
      }
      */
      
      // plot from each bin teh histogram:      
      P2->second.processIntensities( );
      map<double, double>* IntensityMap =  P2->second.getIntensityHist();

      if( IntensityMap->size() > 0 ){

        // normalize the map:
        // to the max count = 1.0
        // log10 of intensities
        double maxCount = 0;
        map<double, double>::iterator z = IntensityMap->begin();
        while( z != IntensityMap->end()){
          if( z->second > maxCount ) {
            maxCount = z->second;
          }
          
          z++;
        }
        
        // normalize:
        map<double, double> DATA;
        z = IntensityMap->begin();
        while( z != IntensityMap->end()){
          DATA.insert( make_pair( log10(z->first), z->second / maxCount ) );
          z++;
        }
        PLOT2->add_plot_data( &DATA );
      }      
      
      if( IntensityMap->size() > 0 ){
        
        char buffer[255];
        sprintf( buffer, "Bin_%0.0fDa_%0.0fmin_(%d points)", P2->first, P1->first, IntensityMap->size());
        string tmp = buffer;
        data_plotter* PLOT = new data_plotter(tmp); 
        //PLOT->add_plot_data( &IntensityMap, buffer );
        sprintf( buffer, "mean=%0.2f", P2->second.getMean());
        PLOT->add_plot_data( IntensityMap,  buffer );
        PLOT->plot_TWOD_data();
        delete PLOT;
        PLOT = NULL;
      }
       
      P2++;
    }
    
    P1++;
  }
  
  PLOT2->plot_TWOD_data();
  delete PLOT2;
  PLOT2 = NULL;
  
}

//////////////////////////////////////////////////
// class desctructor of BackgroundControl
BackgroundControl::~BackgroundControl(){
  intensityBinMap.clear();
}

//////////////////////////////////////////////////
// class copy constructor of BackgroundControl
BackgroundControl::BackgroundControl(const BackgroundControl& tmp){
  intensityBinMap = tmp.intensityBinMap;
}

//////////////////////////////////////////////////
// class copy constructor of BackgroundControl
BackgroundControl::BackgroundControl(const BackgroundControl* tmp){
  intensityBinMap = tmp->intensityBinMap;
}


//////////////////////////////////////////////////
// copy constructor:
BackgroundControl& BackgroundControl::operator=(const BackgroundControl& tmp){
  intensityBinMap = tmp.intensityBinMap;
  return *this;
}

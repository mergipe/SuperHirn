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


#ifndef GENERIC_MS_SCAN_H
#define GENERIC_MS_SCAN_H

typedef RAMPREAL MY_RAMP_PEAKS;

class Generic_MS_Scan{

    
    ////////////////////////////////////////////////
    // declaration of the private members:

private:

  MY_RAMP_PEAKS* rampMSPeakList;
  
  int scanNumber;
  double retentionTime;
  int scanMSLevel;

  // MS scan precursor information
  double preCursor;
  int preCursorZ;
  int preCursorScan;
  double preCursorRT;
  
  bool preCursorFound;
  ////////////////////////////////////////////////
  // declaration of the public members:
  
public:
  
  // class destructor
  ~Generic_MS_Scan();
  
  // class constructor
  Generic_MS_Scan();
  Generic_MS_Scan( int , double , double, int, int  );
  // class copy constructor
  Generic_MS_Scan(const Generic_MS_Scan&);
  // class copy constructor
  Generic_MS_Scan(const Generic_MS_Scan*);
  
  
  //////////////////////////////////////////////////
  // overload operators:
  Generic_MS_Scan& operator=(const Generic_MS_Scan&);
  bool operator==(const Generic_MS_Scan&);
  Generic_MS_Scan& operator<=(const Generic_MS_Scan&);
  Generic_MS_Scan& operator>=(const Generic_MS_Scan&);
  Generic_MS_Scan& operator<(const Generic_MS_Scan&);
  Generic_MS_Scan& operator>(const Generic_MS_Scan&);
  
  
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class

  
  MY_RAMP_PEAKS* getRampMSPeakList( ){return rampMSPeakList;};
  void setRampMSPeakList(MY_RAMP_PEAKS* iData ){rampMSPeakList = iData;};
  
  double getMZPrecursor(){return preCursor;};
  void setMZPrecursorFound(double iMz){preCursor = iMz; preCursorFound = true;};
  int getZPrecursor(){return preCursorZ;};
  int getScanPrecursor(){return preCursorScan;};
  void setScanPrecursor(int iScan){preCursorScan = iScan;};
  double getRTPrecursor(){return preCursorRT;};
  void setRTPrecursor(double iRT){preCursorRT = iRT;};
  int getMSScanLevel(){return scanMSLevel;};
  int getMSScanNumber(){return scanNumber;};
  double getMSScanRetentionTime(){return retentionTime;};
  
  bool getPrecursorFoundState(){ return preCursorFound;};
  
};

#endif

    

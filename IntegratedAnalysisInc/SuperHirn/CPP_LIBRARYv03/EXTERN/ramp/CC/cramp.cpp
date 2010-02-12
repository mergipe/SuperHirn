/***************************************************************************
cramp.cpp

***************************************************************************
cramp.hpp

  A C++ wrapper for the RAMP code.

  Use this library to parse an mzXML file in a non-sequential way, by
  taking advantage of the index element.  

  (C) 2004 by Brian Pratt, Insilicos LLC 

  Based on mzXML2Other, which has this copyright:
	 -------------------
	 begin					 : Wed Apr 2
	 copyright				 : (C) 2002 by Pedrioli Patrick, ISB, Proteomics
	 email					 : ppatrick@student.ethz.ch
 ***************************************************************************/

/***************************************************************************
*																								  *
*	 This program is free software; you can redistribute it and/or modify  *
*	 it under the terms of the GNU Library or "Lesser" General Public 	  *
*	 License (LGPL) as published by the Free Software Foundation;			  *
*	 either version 2 of the License, or (at your option) any later		  *
*	 version.																				  *
*																								  *
***************************************************************************/



#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "stdio.h"
#if !defined(_MSC_VER) && !defined(__MINGW32__)
#include "sys/errno.h"
#endif
#include "cramp.hpp"


/**
 * This function performs a non-sequential parsing operation on an indexed
 * msxml file.
 *
 * @param fileName: Name of the msxml file
 * @param startSCan: Number of the scan we want to read from
 * @param what: -HEADER will return num, msLevel and retentionTime
 *              -SCAN will return only the peaks
 *              -ALL will return everything found in scan, precursorMz and peaks
 *
 * @return pData is dynamically allocate and becomes property of the caller, who
 *         is responsible for its deallocation!!
 */



cRamp::cRamp( const char* fileName ) : m_filename(fileName), m_runInfo()
{
   m_handle = rampOpenFile(fileName);
   m_scanOffsets = NULL;
   m_runInfo = NULL;
   if (!OK()) {
     // HENRY -- I would prefer this to be silent, and let the caller deals with it
     // cout << "Error: Could not open file " << fileName << ": " << strerror(errno) << endl;
   } else {
      m_runInfo = getRunInfo();
      if (m_runInfo->m_data.scanCount < 0) { // undeclared scan count
         // this will provoke reading of index, which sets scan count
         rampScanInfo* tmp = getScanHeaderInfo ( 1 );
         free(tmp);
      }
   }
}

cRamp::~cRamp() {
   rampCloseFile(m_handle);
   
   // HENRY -- in case it is not OK(), and m_scanOffsets and m_runInfo remains NULL
   // check nullity before freeing... 
   if (m_scanOffsets) free(m_scanOffsets);
   if (m_runInfo) free(m_runInfo);
}

//
// here are the private guts
//
rampInfo* cRamp::do_ramp( ramp_fileoffset_t arg , eWhatToRead	what )
{
   
   switch( what ) {
   case RAMP_RUNINFO:
   case RAMP_HEADER:
   case RAMP_PEAKS:
      break; // OK
   default:
	  std::cerr << "unknown read type!\n";
      return NULL;
      break;
   }	
   
   rampInfo* returnPtr=NULL;
   
   if ((RAMP_RUNINFO != what) && !m_scanOffsets) {
      int iLastScan = 0; 
     // we need the index to get anything besides the header
      ramp_fileoffset_t indexOffset = getIndexOffset(m_handle);
      m_scanOffsets = readIndex(m_handle, indexOffset, &iLastScan);
      if (iLastScan >= m_runInfo->m_data.scanCount) {
         m_runInfo->m_data.scanCount = iLastScan;
      }
   }

   
   // HENRY -- arg is out of bounds. instead of creating havoc in RAMP, let's just kill it here.
   if (RAMP_RUNINFO != what && (arg > m_runInfo->m_data.scanCount || arg < 1)) {
     return (NULL);
   }
     
   if (m_scanOffsets || (RAMP_RUNINFO == what)) {
      ramp_fileoffset_t scanOffset=-1;
      if (RAMP_RUNINFO == what) {
         scanOffset = 0; // read from head of file
      } else {
         scanOffset = m_scanOffsets[arg]; // ramp is one-based
      }
      
      if (scanOffset >= 0) {
         
         // -----------------------------------------------------------------------
         // And now we can parse the info we were looking for
         // -----------------------------------------------------------------------
         
         
         // Ok now we have to copy everything in our structure
         switch( what )
         {
         case RAMP_RUNINFO:
            returnPtr = new rampRunInfo( m_handle );
            break;
         case RAMP_HEADER:
            returnPtr = new rampScanInfo( m_handle, scanOffset, (int)arg );
            if (returnPtr) {
              ((rampScanInfo *)returnPtr)->m_data.filePosition = scanOffset; // for future reference
            
              // HENRY -- error checking here
              if (((rampScanInfo*)returnPtr)->m_data.acquisitionNum < 0) {
                // something failed in RAMP, possibly because it's a missing scan
                delete ((rampScanInfo*)returnPtr);
                returnPtr = NULL;
              }
            }
            break;           
         case RAMP_PEAKS:
            returnPtr = new rampPeakList( m_handle, scanOffset);
            
            // HENRY -- error checking here
            if (returnPtr && ((rampPeakList*)returnPtr)->getPeakCount() <= 0) {
              // something failed in RAMP, possibly because it's a missing scan
              delete ((rampPeakList*)returnPtr);
              returnPtr = NULL;
            }
            break;
            
         }
         
      }
   }
   
   
   
   return returnPtr;
}


/**
 * This function performs a non-sequential parsing operation on an indexed
 * msxml file to obtain minimal info on the msRun contained in the file.  

 *
 * @return rapRunInfo* is dynamically allocate and becomes property of the caller, who
 *         is responsible for its deallocation!!
 */

rampRunInfo* cRamp::getRunInfo (  ) {
   rampRunInfo* result;
   if (m_runInfo) { // did we derive this already?
      result = new rampRunInfo(*m_runInfo);
   } else {
      result = (rampRunInfo*) do_ramp(0, RAMP_RUNINFO);
   }
   return result;
}

/**
 * This function performs a non-sequential parsing operation on an indexed
 * msxml file to obtain minimal header info for a numbered scan (thus minimizing parse time).  
 *
 * @param fileName: Name of the msxml file
 * @param startSCan: Number of the scan we want to read from
 * @return rapHeaderInfo* is dynamically allocate and becomes property of the caller, who
 *         is responsible for its deallocation!! returns just the minimal header info num, msLevel and retentionTime
 */

rampScanInfo* cRamp::getScanHeaderInfo ( int whichScan  ) {
   return (rampScanInfo*) do_ramp((ramp_fileoffset_t)whichScan, RAMP_HEADER);
}


/**
 * This function performs a non-sequential parsing operation on an indexed
 * msxml file to obtain peak info for a numbered scan.
 *
 * @param fileName: Name of the msxml file
 * @param startSCan: Number of the scan we want to read from
 * @return rapPeakList* is dynamically allocate and becomes property of the caller, who
 *         is responsible for its deallocation!! returns everything found in scan, precursorMz and peaks
 */

rampPeakList* cRamp::getPeakList ( int whichScan ) {
   return (rampPeakList*) do_ramp((ramp_fileoffset_t)whichScan, RAMP_PEAKS);
}

/**
 * populate from a file handle
 **/
rampPeakList::rampPeakList(RAMPFILE *handle, ramp_fileoffset_t index) {
   init();
   m_peaksCount = readPeaksCount(handle,index);
   m_pPeaks = (rampPeakInfoStruct *)readPeaks(handle,index);
}

/**
 * populate from a file handle
 **/
rampScanInfo::rampScanInfo(RAMPFILE *handle, ramp_fileoffset_t index, int seqNum) {
   init();
   readHeader(handle,index,&m_data);
   m_data.seqNum = seqNum;
}

/**
 * populate from a file handle
 **/
rampRunInfo::rampRunInfo(RAMPFILE *handle) {
   init();
   readMSRun(handle,&m_data);
}

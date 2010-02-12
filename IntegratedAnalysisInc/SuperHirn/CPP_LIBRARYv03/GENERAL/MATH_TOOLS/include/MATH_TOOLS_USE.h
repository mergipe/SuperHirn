///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 21.12.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
//  INLCUDE FILE FOR MATH TOOLS LIBRARY OBJECTS//
#ifndef __MATH_TOOLS_USE_H__
#define __MATH_TOOLS_USE_H__


///////////////////////////////////////////////////////////////////////////
// GLOBAL DEFINED VARIABLES:


///////////////////////////////////////////////////////////////////////////
// define own classes:

#ifdef USE_ALL_MATH_TOOLS_CLASSES
#define USE_SIMPLE_MATH
#define USE_ALL_STANDART_CLASSES
#endif


#ifdef USE_SIMPLE_MATH
#define USE_ALL_STANDART_CLASSES
#define INCLUDE_SIMPLE_MATH
#endif

#ifdef USE_ALL_STANDART_CLASSES
#define USE_NAMESPACE
#define USE_MATH
#define USE_IO
#define USE_STDLIB
#define USE_STD_DEF
#define USE_STDIO
#define USE_SYS_TYPES
#define USE_IOMANIP
#define USE_SYS_STAT
#define USE_DIRENT
#define USE_UNISTD
#define USE_STRING
#define USE_CSTDIO
#define USE_FSTREAM
#define USE_CTYPE
#define USE_VECTOR
#define USE_ALGORITHM
#define USE_LIST
#define USE_MAP
#define USE_IONMANIP
#define USE_ZLIB
#endif

//////////////////////////////////////////////////
//include standard

#ifdef USE_ALGORITHM
#define INCLUDE_ALGORITHM
#endif

#ifdef USE_ZLIB
#define INCLUDE_ZLIB
#endif

#ifdef USE_FUNCTIONAL
#define INCLUDE_FUNCTIONAL
#endif

#ifdef USE_STD_DEF
#define INCLUDE_STD_DEF 
#endif

#ifdef USE_CTYPE
#define INCLUDE_CTYPE
#endif

#ifdef USE_CCTYPE
#define INCLUDE_CCTYPE
#endif

#ifdef USE_DIRENT
#define INCLUDE_DIRENT 
#endif

#ifdef USE_UNISTD
#define INCLUDE_UNISTD 
#endif

#ifdef USE_IOMANIP
#define INCLUDE_IOMANIP 
#endif

#ifdef USE_SYS_TYPES
#define INCLUDE_SYS_TYPES 
#endif

#ifdef USE_SYS_STAT
#define INCLUDE_SYS_STAT 
#endif

#ifdef USE_TIME
#define INCLUDE_TIME 
#endif

#ifdef USE_IO
#define INCLUDE_IO
#endif

#ifdef USE_FSTREAM
#define INCLUDE_FSTREAM
#endif

#ifdef USE_STDLIB
#define INCLUDE_STDLIB
#endif

#ifdef USE_MATH
#define INCLUDE_MATH
#endif

#ifdef USE_STRING
#define INCLUDE_STRING
#endif

#ifdef USE_STDIO
#define INCLUDE_STDIO
#endif

#ifdef USE_VECTOR
#define INCLUDE_VECTOR
#endif

#ifdef USE_LIST
#define INCLUDE_LIST
#endif

#ifdef USE_MAP
#define INCLUDE_MAP
#endif

#ifdef USE_IONMANIP
#define INCLUDE_IONMANIP
#endif


#ifdef USE_NAMESPACE
#define INCLUDE_NAMESPACE
#endif


////////////////////////////////////////////////////
//include standard
#ifdef INCLUDE_NAMESPACE
using namespace std;
#endif

#ifdef IONMANIP
#include <iomanip>
#endif

#ifdef INCLUDE_MAP
#include <map>
#endif

#ifdef INCLUDE_LIST
#include <list>
#endif

#ifdef INCLUDE_VECTOR
#include <vector>
#endif

#ifdef INCLUDE_STDIO
#include <stdio.h>
#endif

#ifdef INCLUDE_CTYPE
#include <ctype.h>
#endif

#ifdef INCLUDE_IO
#include <iostream> 
#endif

#ifdef INCLUDE_FSTREAM
#include <fstream>
#endif

#ifdef INCLUDE_STDLIB
#include <stdlib.h> 
#endif

#ifdef INCLUDE_STRING
#include <string> 
#endif

#ifdef INCLUDE_MATH
#include <math.h> 
#endif

#ifdef INCLUDE_STD_DEF
#include <stddef.h> 
#endif

#ifdef INCLUDE_UNISTD
#include <unistd.h>
#endif

#ifdef INCLUDE_DIRENT
#include <dirent.h> 
#endif

#ifdef INCLUDE_CCTYPE
#include <cctype>
#endif

#ifdef INCLUDE_CTYPE
#include <ctype.h>
#endif

#ifdef INCLUDE_SYS_STAT
#include <sys/stat.h> 
#endif

#ifdef INCLUDE_SYS_TYPES
#include <sys/types.h> 
#endif

#ifdef INCLUDE_IOMANIP
#include <iomanip>
#endif

#ifdef INCLUDE_TIME
#include <time.h> 
#endif

#ifdef INCLUDE_ALGORITHM
#include <algorithm>
#endif

#ifdef INCLUDE_FUNCTIONAL
#include <functional>
#endif

#ifdef INCLUDE_ZLIB
#include <zlib.h>
#endif

////////////////////////////////////////////////////
// include own classes

#ifdef INCLUDE_SIMPLE_MATH
#include "simple_math.h"
#endif



#endif

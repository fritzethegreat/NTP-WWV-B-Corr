/*
 * In developing crsCorr, a need arose for a place to keep global, generic
 *   macros, definitions and the like. This serves that purpose.
 *
 * Modified: 07/29/10
 * Notes:    --Initial creation
 */

#include <iostream>

#ifndef CRSCORR_GLOBAL_H
#define CRSCORR_GLOBAL_H

//-----< DATA TYPE CONSTANTS >--------------------------------------------------
/*
 * These should be used by children classes to associate each label with a data
 *   type.
 */
#define DATATYPE_END -2
#define DATATYPE_FILENAME -1
#define DATATYPE_IGNORE 0
#define DATATYPE_INT    1
#define DATATYPE_DOUBLE 2

#define START_TIME_IGNORE -1
#define RESOLUTION_IGNORE -1

//----< DEBUG CODE >------------------------------------------------------------
#define _DEBUG_
#ifdef _DEBUG_

  /*
   * DEBUG LEVELS
   * ------------
   *   Since pretty much all files include this header, debug levels need to be
   *   globally coordinated (as the name of the file would imply). Each
   *   successive level includes the output from the higher levels. Levels
   *   are assigned off the basis of depth in the "class stack." Since more
   *   classes will be added to the top of the stack, base-level classes are
   *   assigned lower numbers allowing for growth of the stack.
   *
   *   0 - AbstractDataSeries: Detailed output
   *   1 - AbstractDataSeries: Function Unit-Blocks
   *   2 - AbstractDataSeries: Function Calls
   *   3 - DataSeries
   *   4 - DataSeries
   *   5 - DataSeries
   *   6 - FileParser
   *   7 - FileParser
   *   8 - FileParser
   *   9 - ChildParsers
   *  10 - ChildParsers
   *  11 - ChildParsers
   *  12 - CrossCorr: Detailed Output
   *  13 - CrossCorr: Function Unit-Blocks
   *  14 - CrossCorr: Function Calls
   *
   *  Any programs using this library should consider using levels 15+ for
   *  their own debugging purposes.
   */
  namespace debug {
    const int debug_level = 99;
  }


  /*
   * LOG_DEBUG( level, message ) -- allows for easier printing of debug messages
   *    and ordering these messages by "levels".
   */
  #define LOG_DEBUG( level, message ) { \
    if( debug::debug_level <= level ) \
      std::cout << __FUNCTION__ << message << std::endl; \
  }

#endif

//-----< UTILITY MACROS >-------------------------------------------------------

/*
 * LOG_ERR( message ) -- allwos for easier printing of error messages
 */
#define LOG_ERR( message ) { \
  std::cerr << __FUNCTION__ << ": " << message << std::endl; \
}

#endif // end CRSCORR_GLOBAL_H
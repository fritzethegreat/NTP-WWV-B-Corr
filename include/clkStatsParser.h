/*
 * Parses files specifically of the clock stats format from the WWV/H
 *   NTP reference clock. This format is subject to regular changes during
 *   developement.
 *
 * Modified: 07/17/10
 * Notes:    Initial Creation
 */

#include <crsCorr/global.h>
#include <crsCorr/fileParser.h>
#include <crsCorr/dataSeries.h>


#ifndef CRSCORR_CLKSTATS_FILEPARSER_H
#define CRSCORR_CLKSTATS_FILEPARSER_H

/*
 * These constants define indecies, numerical values, and strings that are
 *   significant for the processing of the clockstats data file. Terms that
 *   involve tokens refer to terms used in extracting the data values from the
 *   input data line, typically retrieved via a getline statement. Terms that
 *   involve data indecies refer to the index of the data series in the large
 *   data array.
 */
#define WWV5_LINESIZE 180             // size to use for getLine when parsing
#define WWV5_AGC_INDEX 5              // token index of the audio gain control
#define WWV5_FREQ_VALS 13             // number of values per station(freq)
#define WWV5_VALUES 26                // count of values in a wwv5 txt line
#define WWV5_REFID_INDEX 11           // token index of the reference id
#define WWV5_REFID_H_INDEX 18         // token index of the wwvh ref id
#define WWV5_LABEL_2 "WV2"            // identifier for wwv2 station
#define WWV5_START_2 11               // data index of the wwv2 station
#define WWV5_LABEL_5 "WV5"            // identifier for wwv2 station
#define WWV5_LABEL_10 "WV10"          // identifier for wwv2 station
#define WWV5_LABEL_15 "WV15"          // identifier for wwv2 station
#define WWV5_LABEL_20 "WV20"          // identifier for wwv2 station


class ClkStatsParser : public FileParser {

  public:
    //----< (DE/CON)STRUCTORS >-------------------------------------------------
    /*
     * Constructors are supplied without parameters as those shouldn't be
     *   needed (minus the copy constructor). Destructors and the assignment
     *   operator are also provided here.
     */
    ClkStatsParser();
    ClkStatsParser( string fileName );
    ClkStatsParser( const ClkStatsParser& copy );
    ~ClkStatsParser();
    ClkStatsParser& operator=(const ClkStatsParser& copy );

    //----< FILE METHODS >------------------------------------------------------
    /*
     * Begins the actual file processing and fills the associated data file
     *   with various types of DataSeries.
     */
    void parseFile();

  protected:
    //----< MISC >--------------------------------------------------------------
     /*
      * Static array which contains all of the labels used for the data
      *   series collected by the file parser. This is available to the public
      *   so that a user can select which data series to retrieve.
      */
   static const FileParser::DataTag tags[];
};

#endif
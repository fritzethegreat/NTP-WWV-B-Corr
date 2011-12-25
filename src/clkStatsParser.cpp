/*
 * Modified:  07/17/10
 * Notes:     Expanding file parser to extract significant data from
 *              clockstats file generated by WWV NTP reference clock.
 */

#include <crsCorr/clkStatsParser.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>

    /*
     * Determines which values to extract from the given data file.
     * Below, the available columns are specified along with their units.
     * Data ranges are specified and should be considered when chosing which
     * data series(es) use.
     *
     * Name        Value                               Suggested Type
     *-------------------------------------------------------------------------
 1   * PID         Process ID for NTP                  (ignore)
 2   * epoch       Time from midnight                  Double
 3   * IP          127.127.36.0                        (ignore)
 4   * Ident       wwv5                                (ignore)
 5   * status      0x0000 - 0xFFFF                     (ignore)
 6   * gain        0 - 255                             Int
 7   * yepoch      0 - 5999                            Int
 8   * epomax      0 - 7000(?)                         Int
 9   * eposnr      0 - 40 dB                           Double
10   * datsig      0 - 7000(?)                         Int
11   * datsnr      0 - 40 dB                           Double
12   * sp->refid   WV##                                (ignore)
13   * reach       0x0000 - 0xFFFF                     (ignore)
14   * metric      0 - 100                             Int
15   * synmax      0 - 7000(?)                         Int
16   * synsnr      0 - 40dB                            Double
17   * min_epoch   0 - 479,999                         Int
18   * sec_epoch   0 - 5999                            Int
19   * rp->refid   WH##                                (ignore)
20   * reach       0x0000 - 0xFFFF                     ...
21   * metric      0 - 100                             Int
22   * synmax      0 - 7000(?)                         Int
23   * synsnr      0 - 40dB                            Double
24   * min_epoch   0 - 479,999                         Int
25   * sec_epoch   0 - 5999                            Int
26   * delay       0 - 5999                            Int
     */

const FileParser::DataTag ClkStatsParser::tags[] = {
  { "pid", DATATYPE_IGNORE, 5, 0 },
  { "epoch", DATATYPE_IGNORE, 5, 0 },
  { "ip", DATATYPE_IGNORE, 5, 0 },
  { "ident", DATATYPE_IGNORE, 5, 0 },
  { "status", DATATYPE_IGNORE, 5, 0 },
  { "gain", DATATYPE_IGNORE, 5, 0 },
  { "yepoch", DATATYPE_IGNORE, 5, 0 },
  { "epomax", DATATYPE_IGNORE, 5, 0 },
  { "eposnr", DATATYPE_IGNORE, 5, 0 },
  { "datsig", DATATYPE_IGNORE, 5, 0 },
  { "datsnr", DATATYPE_IGNORE, 5, 0 },
  { "vreach2", DATATYPE_IGNORE, 5, 0 }, // 11
  { "vmetric2", DATATYPE_IGNORE, 5, 0 },
  { "vsynmax2", DATATYPE_INT, 5, 0 },
  { "vsynsnr2", DATATYPE_IGNORE, 5, 0 },
  { "vmin_epoch2", DATATYPE_IGNORE, 5, 0 },
  { "vsec_epoch2", DATATYPE_IGNORE, 5, 0 },
  { "hreach2", DATATYPE_IGNORE, 5, 0 },
  { "hmetric2", DATATYPE_IGNORE, 5, 0 },
  { "hsynmax2", DATATYPE_INT, 5, 0 },
  { "hsynsnr2", DATATYPE_IGNORE, 5, 0 },
  { "hmin_epoch2", DATATYPE_IGNORE, 5, 0 },
  { "hsec_epoch2", DATATYPE_IGNORE, 5, 0 },
  { "delay2", DATATYPE_IGNORE, 5, 0 },
  { "vreach5", DATATYPE_IGNORE, 5, 0 }, // 24
  { "vmetric5", DATATYPE_IGNORE, 5, 0 },
  { "vsynmax5", DATATYPE_INT, 5, 0 },
  { "vsynsnr5", DATATYPE_IGNORE, 5, 0 },
  { "vmin_epoch5", DATATYPE_IGNORE, 5, 0 },
  { "vsec_epoch5", DATATYPE_IGNORE, 5, 0 },
  { "hreach5", DATATYPE_IGNORE, 5, 0 },
  { "hmetric5", DATATYPE_IGNORE, 5, 0 },
  { "hsynmax5", DATATYPE_INT, 5, 0 },
  { "hsynsnr5", DATATYPE_IGNORE, 5, 0 },
  { "hmin_epoch5", DATATYPE_IGNORE, 5, 0 },
  { "hsec_epoch5", DATATYPE_IGNORE, 5, 0 },
  { "delay5", DATATYPE_IGNORE, 5, 0 },
  { "vreach10", DATATYPE_IGNORE, 5, 0 }, // 37
  { "vmetric10", DATATYPE_IGNORE, 5, 0 },
  { "vsynmax10", DATATYPE_INT, 5, 0 },
  { "vsynsnr10", DATATYPE_IGNORE, 5, 0 },
  { "vmin_epoch10", DATATYPE_IGNORE, 5, 0 },
  { "vsec_epoch10", DATATYPE_IGNORE, 5, 0 },
  { "hreach10", DATATYPE_IGNORE, 5, 0 },
  { "hmetric10", DATATYPE_IGNORE, 5, 0 },
  { "hsynmax10", DATATYPE_INT, 5, 0 },
  { "hsynsnr10", DATATYPE_IGNORE, 5, 0 },
  { "hmin_epoch10", DATATYPE_IGNORE, 5, 0 },
  { "hsec_epoch10", DATATYPE_IGNORE, 5, 0 },
  { "delay10", DATATYPE_IGNORE, 5, 0 },
  { "vreach15", DATATYPE_IGNORE, 5, 0 }, // 50
  { "vmetric15", DATATYPE_IGNORE, 5, 0 },
  { "vsynmax15", DATATYPE_INT, 5, 0 },
  { "vsynsnr15", DATATYPE_IGNORE, 5, 0 },
  { "vmin_epoch15", DATATYPE_IGNORE, 5, 0 },
  { "vsec_epoch15", DATATYPE_IGNORE, 5, 0 },
  { "hreach15", DATATYPE_IGNORE, 5, 0 },
  { "hmetric15", DATATYPE_IGNORE, 5, 0 },
  { "hsynmax15", DATATYPE_INT, 5, 0 },
  { "hsynsnr15", DATATYPE_IGNORE, 5, 0 },
  { "hmin_epoch15", DATATYPE_IGNORE, 5, 0 },
  { "hsec_epoch15", DATATYPE_IGNORE, 5, 0 },
  { "delay15", DATATYPE_IGNORE, 5, 0 },
  { "vreach20", DATATYPE_IGNORE, 5, 0 }, // 63
  { "vmetric20", DATATYPE_IGNORE, 5, 0 },
  { "vsynmax20", DATATYPE_INT, 5, 0 },
  { "vsynsnr20", DATATYPE_IGNORE, 5, 0 },
  { "vmin_epoch20", DATATYPE_IGNORE, 5, 0 },
  { "vsec_epoch20", DATATYPE_IGNORE, 5, 0 },
  { "hreach20", DATATYPE_IGNORE, 5, 0 },
  { "hmetric20", DATATYPE_IGNORE, 5, 0 },
  { "hsynmax20", DATATYPE_INT, 5, 0 },
  { "hsynsnr20", DATATYPE_IGNORE, 5, 0 },
  { "hmin_epoch20", DATATYPE_IGNORE, 5, 0 },
  { "hsec_epoch20", DATATYPE_IGNORE, 5, 0 },
  { "delay20", DATATYPE_IGNORE, 5, 0 },
  { "END", DATATYPE_END, -1, -1 } };

//----< (DE)(CON)STRUCTORS >---------------------------------------------------
ClkStatsParser::ClkStatsParser()
    : FileParser( NULL, ClkStatsParser::tags )
{
  LOG_DEBUG( 11, "()" )

}// end ClkStatsParser::ClkStatsParser()

ClkStatsParser::ClkStatsParser( string fileName )
  : FileParser( fileName, ClkStatsParser::tags )
{
  LOG_DEBUG( 11, "( " << fileName << " )" )

  parseFile();
  finalizeSeriesData();
}// end ClkStatsParser::ClkStatsParser( string )


ClkStatsParser::ClkStatsParser( const ClkStatsParser& copy )
  : FileParser( copy )
{
  LOG_DEBUG( 11, "( const ClkStatsParser& copy )" )

}// end ClkStatsParser::ClkStatsParser( const ClkStatsParser& )

ClkStatsParser::~ClkStatsParser() {
  LOG_DEBUG( 11, "()" )

}// end ClkStatsParser::~ClkStatsParser()

//----< ACCESSOR METHODS >-----------------------------------------------------

//----< OPERATORS >------------------------------------------------------------
ClkStatsParser& ClkStatsParser::operator=( const ClkStatsParser& copy ) {
  LOG_DEBUG( 11, "( const ClkStatsParser& copy )" )

  return *this;
}// end ClkStatsParser& ClkStatsParser::operator=( const ClkStatsParser& )


//----< FILE METHODS >---------------------------------------------------------
void ClkStatsParser::parseFile() {
  LOG_DEBUG( 11, "()" )

  // begin parsing data lines
  /*
   * This is somewhat complicated because each wwv5 line could contain a
   *   different data series. First, the data series is determined from the
   *   wwv5 line, and then the current index is offset to "move" to that series'
   *   set of slots.
   */
  char dataLine[ WWV5_LINESIZE ];
  int startTime = 0;
  int lineCount = 0;
  dataStream->getline( dataLine, WWV5_LINESIZE );
  while( dataStream->good() && !dataStream->eof() ) {
    // grab line and check for key label (wwv5)
    if( dataLine != NULL && strstr( dataLine, "wwv5" )) {
      lineCount++;

      // iterate through line tokens and pull out data values
      // --make sure to capture the start values on the first entries
      char* asciiValues[ WWV5_VALUES ];
      char* dataTok = strtok( dataLine, " /" );
      for( int i = 0; i < WWV5_VALUES; i++ ) {
        asciiValues[i] = dataTok;
        dataTok = strtok( NULL, " /" );
      }

      // determine the start index for loading data into the series(es)
      char* refId = asciiValues[WWV5_REFID_INDEX];
      int dataIndex = 0;
      if( !strcmp( refId, WWV5_LABEL_2 )) {
        dataIndex = WWV5_START_2;
      } else if( !strcmp( refId, WWV5_LABEL_5 )) {
        dataIndex = WWV5_START_2 + WWV5_FREQ_VALS;
      } else if( !strcmp( refId, WWV5_LABEL_10 )) {
        dataIndex = WWV5_START_2 + WWV5_FREQ_VALS * 2;
      } else if( !strcmp( refId, WWV5_LABEL_15 )) {
        dataIndex = WWV5_START_2 + WWV5_FREQ_VALS * 3;
      } else if( !strcmp( refId, WWV5_LABEL_20 )) {
        dataIndex = WWV5_START_2 + WWV5_FREQ_VALS * 4;
      } else {
        LOG_ERR( "Unknown RefId token recovered: " << refId )
        dataIndex = -1;
      }

      // extract the gain
      int gain = atoi( asciiValues[ WWV5_AGC_INDEX ] );

      // process data line and load in tokens
      LOG_DEBUG( 9, ": Loading tokens" )
      for( int tokenIndex = WWV5_REFID_INDEX + 1;
           tokenIndex < WWV5_VALUES;
           tokenIndex++ )
      {
        if( tokenIndex == WWV5_REFID_H_INDEX ) {
          // no op -- wwvh identifier
        } else {
          if( tags[dataIndex].type == DATATYPE_DOUBLE ) {
            double value = atof( asciiValues[tokenIndex] );
            data[dataIndex]->addValue( &value, 5, startTime );
          } else if( tags[dataIndex].type == DATATYPE_INT
                  && tags[dataIndex].label.find( "synmax" ) != string::npos ) {
            int value = atoi( asciiValues[tokenIndex] );
            value *= gain;
            data[dataIndex]->addValue( &value, 5, startTime );
          } else if( tags[dataIndex].type == DATATYPE_INT ) {
            int value = atoi( asciiValues[tokenIndex] );
            data[dataIndex]->addValue( &value, 5, startTime );
          } else {
            // ignore case
          }
          dataIndex++;
        }
      }// finished with data line

      // increment startTime; wrap from 0 to 4 minutes UTC
      startTime++;
      if( startTime > 4 ) {
        startTime = 0;
      }
    }// good data line

    // grab next line for processing, should trigger any flags
    dataStream->getline( dataLine, WWV5_LINESIZE );
  }// finished with data file
}// end void ClkStatsParser::parseFile()
/*
 * Modified:  07/16/10
 * Notes:     Expanding file parser to extract significant data from
 *              ace magnetometer readings
 */

#include <crsCorr/gsMagParser.h>
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
     * Hp, He, Hn, Total = nT
     * Missing Data -1.00e+05
     *
     * Label        Value                               Suggested Type
     *-------------------------------------------------------------------------
     *  YR           Year                                Int
     *  MO           Month                               Int
     *  DA           Day                                 Int
     *  HHMM         Hrs/Min                             Int
     *  DAY          Julian Calendar day                 Int
     *  SEC          Seconds of the day                  Int
     *  STATUS       Status of the data reading          Int
     *  Hp           Parallel to Earth's spin axis       Double
     *  He           Down to Earth                       Double
     *  Hn           Eastwards, orthogonal               Double
     *  Total        Total Field                         Double
     */
const FileParser::DataTag GsMagParser::tags[] = {
 { "YR", DATATYPE_INT, 1, 0 },
 { "MO", DATATYPE_INT, 1, 0 },
 { "DA", DATATYPE_INT, 1, 0 },
 { "HHMM", DATATYPE_INT, 1, 0 },
 { "DAY", DATATYPE_INT, 1, 0 },
 { "SEC", DATATYPE_INT, 1, 0 },
 { "HP", DATATYPE_DOUBLE, 1, 0 },
 { "HE", DATATYPE_DOUBLE, 1, 0 },
 { "HN", DATATYPE_DOUBLE, 1, 0 },
 { "TOTAL", DATATYPE_DOUBLE, 1, 0 },
 { "END", DATATYPE_END, -1, -1 } };

//----< (DE)(CON)STRUCTORS >---------------------------------------------------
GsMagParser::GsMagParser()
    : FileParser( NULL, GsMagParser::length, GsMagParser::tags )
{
  LOG_DEBUG( 11, "()" )

}// end GsMagParser::GsMagParser()

GsMagParser::GsMagParser( string fileName )
  : FileParser( fileName, GsMagParser::length, GsMagParser::tags )
{
  LOG_DEBUG( 11, "( " << fileName << " )" )

  parseFile();
  finalizeSeriesData();
}// end GsMagParser::GsMagParser( string )


GsMagParser::GsMagParser( const GsMagParser& copy ) {
  LOG_DEBUG( 11, "( const GsMagParser& copy )" )

}// end GsMagParser::GsMagParser( const GsMagParser& )

GsMagParser::~GsMagParser() {
  LOG_DEBUG( 11, "()" )

}// end GsMagParser::~GsMagParser()

//----< ACCESSOR METHODS >-----------------------------------------------------

//----< OPERATORS >------------------------------------------------------------
GsMagParser& GsMagParser::operator=( const GsMagParser& copy ) {
  LOG_DEBUG( 11, "( const GsMagParser& copy )" )

  return *this;
}// end GsMagParser& GsMagParser::operator=( const GsMagParser& )


//----< FILE METHODS >---------------------------------------------------------
void GsMagParser::parseFile() {
  LOG_DEBUG( 11, "()" )

  // move to first data line
  if( findArtifact( "#---------------" )) {
    int year;
    int month;
    int day;
    int hourmin;
    int julianDay;
    int sec;
    double hp;
    double he;
    double hn;
    double total;

    // load data into storage
    int curLine = 0;
    while( dataStream->good() && !dataStream->eof() ) {
      curLine++;
      (*dataStream) >> year >> month >> day >> hourmin >> julianDay
                    >> sec >> hp >> he >> hn >> total;
      int i = 0;
      data[i++]->addValue( &year );
      data[i++]->addValue( &month );
      data[i++]->addValue( &day );
      data[i++]->addValue( &hourmin );
      data[i++]->addValue( &julianDay );
      data[i++]->addValue( &sec );
      if( hp == -1.00e+05 ) {
        hp = 0.0;
      }
      data[i++]->addValue( &hp );
      if( he == -1.00e+05 ) {
         he = 0.0;
      }
      data[i++]->addValue( &he );
      if( hn == -1.00e+05 ) {
        hn = 0.0;
      }
      data[i++]->addValue( &hn );
      if( total == -1.00e+05 ) {
        total = 0.0;
      }
      data[i++]->addValue( &total );
    }
    LOG_DEBUG( 10, ": Parsed " << curLine << " lines." )
  } else {
    LOG_ERR( "Failed to find separator bar." )
  }
}// end void GsMagParser::parseFile()


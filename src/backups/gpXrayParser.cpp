/*
 * Modified:  07/16/10
 * Notes:     Expanding file parser to extract significant data from
 *              ace magnetometer readings
 */

#include <crsCorr/gpXrayParser.h>
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
     * Short = 0.05 - 0.4 nanometer, measured in watts / m^2
     * Long  = 0.1  - 0.8 nanometer, measured in watts / m^2
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
     *  SHORT        Short xray power, 0.05 - 0.4 nm     Double
     *  LONG         Long xray power, 0.1 - 0.8 nm       Double
     */
const FileParser::DataTag GpXrayParser::tags[] = {
 { "YR", DATATYPE_INT, 1, 0 },
 { "MO", DATATYPE_INT, 1, 0 },
 { "DA", DATATYPE_INT, 1, 0 },
 { "HHMM", DATATYPE_INT, 1, 0 },
 { "DAY", DATATYPE_INT, 1, 0 },
 { "SEC", DATATYPE_INT, 1, 0 },
 { "SHORT", DATATYPE_DOUBLE, 1, 0 },
 { "LONG", DATATYPE_DOUBLE, 1, 0 },
 { "END", DATATYPE_END, -1, -1 } };

//----< (DE)(CON)STRUCTORS >---------------------------------------------------
GpXrayParser::GpXrayParser()
    : FileParser( NULL, GpXrayParser::length, GpXrayParser::tags )
{
  LOG_DEBUG( 11, "()" )

}// end GpXrayParser::GpXrayParser()

GpXrayParser::GpXrayParser( string fileName )
  : FileParser( fileName, GpXrayParser::length, GpXrayParser::tags )
{
  LOG_DEBUG( 11, "( " << fileName << " )" )

  parseFile();
  finalizeSeriesData();
}// end GpXrayParser::GpXrayParser( string )


GpXrayParser::GpXrayParser( const GpXrayParser& copy )
  : FileParser( copy )
{
  LOG_DEBUG( 11, "( const GpXrayParser& copy )" )

}// end GpXrayParser::GpXrayParser( const GpXrayParser& )

GpXrayParser::~GpXrayParser() {
  LOG_DEBUG( 11, "()" )

}// end GpXrayParser::~GpXrayParser()

//----< ACCESSOR METHODS >-----------------------------------------------------

//----< OPERATORS >------------------------------------------------------------
GpXrayParser& GpXrayParser::operator=( const GpXrayParser& copy ) {
  LOG_DEBUG( 11, "()" )

  return *this;
}// end GpXrayParser& GpXrayParser::operator=( const GpXrayParser& )


//----< FILE METHODS >---------------------------------------------------------
void GpXrayParser::parseFile() {
  LOG_DEBUG( 11, "()" )

  // move to first data line
  if( findArtifact( "#----------------------" )) {
    int year;
    int month;
    int day;
    int hourmin;
    int julianDay;
    int sec;
    double shortData;
    double longData;
  
    // load data into storage
    LOG_DEBUG( 10, ": Loading data." )
    int curLine = 0;
    while( dataStream->good() && !dataStream->eof() ) {
      curLine++;
      (*dataStream) >> year >> month >> day >> hourmin >> julianDay
                    >> sec >> shortData >> longData;
      int i = 0;
      data[i++]->addValue( &year );
      data[i++]->addValue( &month );
      data[i++]->addValue( &day );
      data[i++]->addValue( &hourmin );
      data[i++]->addValue( &julianDay );
      data[i++]->addValue( &sec );
      if( shortData == -1.00e+05 ) shortData = 0.0;
      data[i++]->addValue( &shortData );
      if( longData == -1.00e+05 ) longData = 0.0;
      data[i++]->addValue( &longData );
    }// finished iteration
  } else {
    LOG_ERR( "Failed to find separator artifact" )
  }
}// end void GpXrayParser::parseFile()
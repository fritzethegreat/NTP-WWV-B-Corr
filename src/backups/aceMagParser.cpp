/*
 * Modified:  07/16/10
 * Notes:     Expanding file parser to extract significant data from
 *              ace magnetometer readings
 */

#include <crsCorr/aceMagParser.h>
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
     * Bx, By, Bz, Bt = nT
     * yF2, D, hmE, h'F, h', hmF2 = km
     * Missing Data -999.9
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
     *  Bx           Horizontal component                Double
     *  By           Vertical Component                  Double
     *  Bz           Other Component                     Double
     *  Bt           Total Magnitude                     Double
     *  Lat          Latitude                            Double
     *  Long         Longitude                           Double
     */
const FileParser::DataTag AceMagParser::tags[] = {
 { "YR", DATATYPE_INT, 1, 0 },
 { "MO", DATATYPE_INT, 1, 0 },
 { "DA", DATATYPE_INT, 1, 0 },
 { "HHMM", DATATYPE_INT, 1, 0 },
 { "DAY", DATATYPE_INT, 1, 0 },
 { "SEC", DATATYPE_INT, 1, 0 },
 { "STATUS", DATATYPE_INT, 1, 0 },
 { "BX", DATATYPE_DOUBLE, 1, 0 },
 { "BY", DATATYPE_DOUBLE, 1, 0 },
 { "BZ", DATATYPE_DOUBLE, 1, 0 },
 { "BT", DATATYPE_DOUBLE, 1, 0 },
 { "LAT", DATATYPE_DOUBLE, 1, 0 },
 { "LONG", DATATYPE_DOUBLE, 1, 0 },
 { "END", DATATYPE_END, -1, -1 } };

//----< (DE)(CON)STRUCTORS >---------------------------------------------------
AceMagParser::AceMagParser()
    : FileParser( NULL, AceMagParser::tags )
{
  LOG_DEBUG( 0, "()" )

}// end AceMagParser::AceMagParser()

AceMagParser::AceMagParser( string fileName )
  : FileParser( fileName, AceMagParser::tags )
{
  LOG_DEBUG( 0, "( " << fileName << " )" )

  parseFile();
  finalizeSeriesData();
}// end AceMagParser::AceMagParser( string )


AceMagParser::AceMagParser( const AceMagParser& copy )
  : FileParser( copy )
{
  LOG_DEBUG( 0, "( const AceMagParser& copy )" )

}// end AceMagParser::AceMagParser( const AceMagParser& )

AceMagParser::~AceMagParser() {
  LOG_DEBUG( 0, "()" )

}// end AceMagParser::~AceMagParser()

//----< ACCESSOR METHODS >-----------------------------------------------------

//----< OPERATORS >------------------------------------------------------------
AceMagParser& AceMagParser::operator=( const AceMagParser& copy ) {
  LOG_DEBUG( 1, "( const AceMagParser& copy )" )

  return *this;
}// end AceMagParser& AceMagParser::operator=( const AceMagParser& )


//----< FILE METHODS >---------------------------------------------------------
void AceMagParser::parseFile() {
  LOG_DEBUG( 0, "()" )

  // move to first data line
  if( findArtifact( "#-------------------------" )) {
    int year;
    int month;
    int day;
    int hourmin;
    int julianDay;
    int sec;
    int status;
    double bx;
    double by;
    double bz;
    double bt;
    double lat;
    double longitude;

    // load data into storage
    LOG_DEBUG( 4, ": Loading data" )
    int curLine = 0;
    while( dataStream->good() && !dataStream->eof() ) {
      curLine++;
      (*dataStream) >> year >> month >> day >> hourmin >> julianDay
                    >> sec >> status >> bx >> by >> bz >> bt
                    >> lat >> longitude;
      int i = 0;
      data[i++]->addValue( &year );
      data[i++]->addValue( &month );
      data[i++]->addValue( &day );
      data[i++]->addValue( &hourmin );
      data[i++]->addValue( &julianDay );
      data[i++]->addValue( &sec );
      data[i++]->addValue( &status );
      if( bx == -999.9 ) {
         bx = 0.0;
      }
      data[i++]->addValue( &bx );
      if( by == -999.9 ) {
         by = 0.0;
      }
      data[i++]->addValue( &by );
      if( bz == -999.9 )  {
        bz = 0.0;
      }
      data[i++]->addValue( &bz );
      if( bt == -999.9 ) {
        bt = 0.0;
      }
      data[i++]->addValue( &bt );
      if( lat == -999.9 ) {
        lat = 0.0;
      }
      data[i++]->addValue( &lat );
      if( longitude == -999.9 ) {
        longitude = 0.0;
      }
      data[i++]->addValue( &longitude );
    }// finished iteration of a data line
  } else {
    LOG_ERR( "Failed to find dataLine. Finalizing empty DataSeries." )
  }
}// end void AceMagParser::parseFile()


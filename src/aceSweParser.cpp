/*
 * Modified:  07/16/10
 * Notes:     Expanding file parser to extract significant data from
 *              ace solar wind electron proton alpha monitor readings
 */

#include <crsCorr/aceSweParser.h>
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
     * Proton Density = p/cc
     * Bulk Speed = km/s
     * Ion temp = degrees K
     * Missing Data -9999.9, -1.00e+05
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
     *  Proton       Proton density                      Double
     *  Speed        Bulk Speed                          Double
     *  Temp         Ion Temperature                     Double
     */
const FileParser::DataTag AceSweParser::tags[] = {
 { "YR", DATATYPE_INT, 1, 0 },
 { "MO", DATATYPE_INT, 1, 0 },
 { "DA", DATATYPE_INT, 1, 0 },
 { "HHMM", DATATYPE_INT, 1, 0 },
 { "DAY", DATATYPE_INT, 1, 0 },
 { "SEC", DATATYPE_INT, 1, 0 },
 { "STATUS", DATATYPE_INT, 1, 0 },
 { "PROTON", DATATYPE_DOUBLE, 1, 0 },
 { "SPEED", DATATYPE_DOUBLE, 1, 0 },
 { "TEMP",  DATATYPE_DOUBLE, 1, 0 },
 { "END", DATATYPE_END, -1, -1 } };

//----< (DE)(CON)STRUCTORS >---------------------------------------------------
AceSweParser::AceSweParser()
    : FileParser( NULL, AceSweParser::tags )
{
  LOG_DEBUG( 11, "()" )

}// end AceSweParser::AceSweParser()

AceSweParser::AceSweParser( string fileName )
  : FileParser( fileName, AceSweParser::tags )
{
  LOG_DEBUG( 11, "( " << fileName << " )" )

  parseFile();
  finalizeSeriesData();
}// end AceSweParser::AceSweParser( string )


AceSweParser::AceSweParser( const AceSweParser& copy )
  : FileParser( copy )
{
  LOG_DEBUG( 11, "( const AceSweParser& copy )" )

}// end AceSweParser::AceSweParser( const AceSweParser& )

AceSweParser::~AceSweParser() {
  LOG_DEBUG( 11, "()" )

}// end AceSweParser::~AceSweParser()

//----< ACCESSOR METHODS >-----------------------------------------------------

//----< OPERATORS >------------------------------------------------------------
AceSweParser& AceSweParser::operator=( const AceSweParser& copy ) {
  LOG_DEBUG( 11, "( const AceSweParser& copy )" )

  return *this;
}// end AceSweParser& AceSweParser::operator=( const AceSweParser& )


//----< FILE METHODS >---------------------------------------------------------
void AceSweParser::parseFile() {
  LOG_DEBUG( 11, "()" )

  // move to first data line
  LOG_DEBUG( 10, ": Locating separator line." )
  if( findArtifact( "#----------------------------" )) {
    int year;
    int month;
    int day;
    int hourmin;
    int julianDay;
    int sec;
    int status;
    double proton;
    double speed;
    double temp;

    // load data into storage
    LOG_DEBUG( 10, ": Loading data." )
    int curLine = 0;
    (*dataStream) >> year >> month >> day >> hourmin >> julianDay
                  >> sec >> status >> proton >> speed >> temp;

    while( dataStream->good() && !dataStream->eof() ) {
      curLine++;
      int i = 0;
      data[i++]->addValue( &year );
      data[i++]->addValue( &month );
      data[i++]->addValue( &day );
      data[i++]->addValue( &hourmin );
      data[i++]->addValue( &julianDay );
      data[i++]->addValue( &sec );
      data[i++]->addValue( &status );
      if( proton == -9999.9 )
        proton = 0;
      data[i++]->addValue( &proton );
      if( speed == -9999.9 )
        speed = 0;
      data[i++]->addValue( &speed );
      if( temp == -1.00e+05 )
        temp = 0;
      data[i++]->addValue( &temp );

      (*dataStream) >> year >> month >> day >> hourmin >> julianDay
                    >> sec >> status >> proton >> speed >> temp;
    }// finished iteration of a data line
  } else {
    LOG_ERR( "Failed to find separator line. Finalizing empty datasets" )
  }
}// end void AceSweParser::parseFile()

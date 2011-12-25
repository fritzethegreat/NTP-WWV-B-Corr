/*
 * Modified:  07/16/10
 * Notes:     Expanding file parser to extract significant data from
 *              ace magnetometer readings
 */

#include <crsCorr/gpPartParser.h>
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
     * Hp, He, Hn = nT
     *
     * Label        Value                               Suggested Type
     *-------------------------------------------------------------------------
     *  YR           Year                                Int
     *  MO           Month                               Int
     *  DA           Day                                 Int
     *  HHMM         Hrs/Min                             Int
     *  DAY          Julian Calendar day                 Int
     *  SEC          Seconds of the day                  Int
     *  P1           Particles at >1Mev                  Double
     *  P5           Particles at >5Mev                  Double
     *  P10          Particles at >10Mev                 Double
     *  P30          Particles at >30Mev                 Double
     *  P50          Particles at >50Mev                 Double
     *  P100         Particles at >100Mev                Double
     *  E0.8         Electrons at >0.8Mev                Double
     *  E2.0         Electrons at >2.0Mev                Double
     *  E4.0         Electrons at >4.0Mev                Double
     */
const FileParser::DataTag GpPartParser::tags[] = {
 { "YR", DATATYPE_INT, 5, 0 },
 { "MO", DATATYPE_INT, 5, 0 },
 { "DA", DATATYPE_INT, 5, 0 },
 { "HHMM", DATATYPE_INT, 5, 0 },
 { "DAY", DATATYPE_INT, 5, 0 },
 { "SEC", DATATYPE_INT, 5, 0 },
 { "P1", DATATYPE_DOUBLE, 5, 0 },
 { "P5", DATATYPE_DOUBLE, 5, 0 },
 { "P10", DATATYPE_DOUBLE, 5, 0 },
 { "P30", DATATYPE_DOUBLE, 5, 0 },
 { "P50", DATATYPE_DOUBLE, 5, 0 },
 { "P100", DATATYPE_DOUBLE, 5, 0 },
 { "E08", DATATYPE_DOUBLE, 5, 0 },
 { "E20", DATATYPE_DOUBLE, 5, 0 },
 { "E40", DATATYPE_DOUBLE, 5, 0 },
 { "END", DATATYPE_END, -1, -1 } };

//----< (DE)(CON)STRUCTORS >---------------------------------------------------
GpPartParser::GpPartParser()
    : FileParser( NULL, GpPartParser::tags )
{
  LOG_DEBUG( 11, "()" )

}// end GpPartParser::GpPartParser()

GpPartParser::GpPartParser( string fileName )
  : FileParser( fileName, GpPartParser::tags )
{
  LOG_DEBUG( 11, "( " << fileName << " )" )

  parseFile();
  finalizeSeriesData();
}// end GpPartParser::GpPartParser( string )


GpPartParser::GpPartParser( const GpPartParser& copy ) {
  LOG_DEBUG( 11, "( const GpPartParser& copy )" )

}// end GpPartParser::GpPartParser( const GpPartParser& )

GpPartParser::~GpPartParser() {
  LOG_DEBUG( 11, "()" )

}// end GpPartParser::~GpPartParser()

//----< ACCESSOR METHODS >-----------------------------------------------------

//----< OPERATORS >------------------------------------------------------------
GpPartParser& GpPartParser::operator=( const GpPartParser& copy ) {
  LOG_DEBUG( 11, "( const GpPartParser& copy )" )

  return *this;
}// end GpPartParser& GpPartParser::operator=( const GpPartParser& )


//----< FILE METHODS >---------------------------------------------------------
void GpPartParser::parseFile() {
  LOG_DEBUG( 11, "()" )

  // move to first data line
  if( findArtifact( "#-------------------" )) {
    int year;
    int month;
    int day;
    int hourmin;
    int julianDay;
    int sec;
    double p1;
    double p5;
    double p10;
    double p30;
    double p50;
    double p100;
    double e08;
    double e20;
    double e40;

    // load data into storage
    LOG_DEBUG( 10, ": Loading Data." )

    int curLine = 0;
    (*dataStream) >> year >> month >> day >> hourmin >> julianDay
                  >> sec >> p1 >> p5 >> p10 >> p30 >> p50 >> p100
                  >> e08 >> e20 >> e40;
    while( dataStream->good() && !dataStream->eof() ) {
      curLine++;

      int i = 0;
      data[i++]->addValue( &year );
      data[i++]->addValue( &month );
      data[i++]->addValue( &day );
      data[i++]->addValue( &hourmin );
      data[i++]->addValue( &julianDay );
      data[i++]->addValue( &sec );

      if( p1 == -1.00e+05 ) p1 = 0.0;
      data[i++]->addValue( &p1 );

      if( p5 == -1.00e+05 ) p5 = 0.0;
      data[i++]->addValue( &p5 );

      if( p10 == -1.00e+05 ) p10 = 0.0;
      data[i++]->addValue( &p10 );

      if( p30 == -1.00e+05 ) p30 = 0.0;
      data[i++]->addValue( &p30 );

      if( p50 == -1.00e+05 ) p50 = 0.0;
      data[i++]->addValue( &p50 );

      if( p100 == -1.00e+05 ) p100 = 0.0;
      data[i++]->addValue( &p100 );

      if( e08 == -1.00e+05 ) e08 = 0.0;
      data[i++]->addValue( &e08 );

      if( e20 == -1.00e+05 ) e20 = 0.0;
      data[i++]->addValue( &e20 );

      if( e40 == -1.00e+05 ) e40 = 0.0;
      data[i++]->addValue( &e40 );

      (*dataStream) >> year >> month >> day >> hourmin >> julianDay
                    >> sec >> p1 >> p5 >> p10 >> p30 >> p50 >> p100
                    >> e08 >> e20 >> e40;
    }
  } else {
    LOG_ERR( "Failed to find separator artifact" )
  }
}// end void GpPartParser::parseFile()

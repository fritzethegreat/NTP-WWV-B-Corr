/*
 * Modified:  07/16/10
 * Notes:     Expanding file parser to extract significant data from
 *              ace magnetometer readings
 */

#include <crsCorr/gpMagParser.h>
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
     * Missing Data: -1.00e+05
     *
     * Label        Value                               Suggested Type
     *-------------------------------------------------------------------------
     *  YR           Year                                Int
     *  MO           Month                               Int
     *  DA           Day                                 Int
     *  HHMM         Hrs/Min                             Int
     *  DAY          Julian Calendar day                 Int
     *  SEC          Seconds of the day                  Int
     *  HP           Parallel to earth's spin axis       Double
     *  HE           Directed Earthwards from space      Double
     *  HN           Directed Eastwards orthogonal       Double
     *  TOTAL        Total Field                         Double
     */
const FileParser::DataTag GpMagParser::tags[] = {
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
GpMagParser::GpMagParser()
    : FileParser( NULL, GpMagParser::tags )
{
  LOG_DEBUG( 11, "()" )

}// end GpMagParser::GpMagParser()

GpMagParser::GpMagParser( string fileName )
  : FileParser( fileName, GpMagParser::tags )
{
  LOG_DEBUG( 11, "( " << fileName << " )" )

  parseFile();
  finalizeSeriesData();
}// end GpMagParser::GpMagParser( string )


GpMagParser::GpMagParser( const GpMagParser& copy ) {
  LOG_DEBUG( 11, "( const GpMagParser& copy )" )

}// end GpMagParser::GpMagParser( const GpMagParser& )

GpMagParser::~GpMagParser() {
  LOG_DEBUG( 11, "()" )

}// end GpMagParser::~GpMagParser()

//----< ACCESSOR METHODS >-----------------------------------------------------

//----< OPERATORS >------------------------------------------------------------
GpMagParser& GpMagParser::operator=( const GpMagParser& copy ) {
  LOG_DEBUG( 11, "( const GpMagParser& copy )" )

  return *this;
}// end GpMagParser& GpMagParser::operator=( const GpMagParser& )


//----< FILE METHODS >---------------------------------------------------------
void GpMagParser::parseFile() {
  LOG_DEBUG( 11, "()" )

  // move to first data line
  if( findArtifact( "#-------------------" )) {
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
    LOG_DEBUG( 10, ": Loading Data." )
    int curLine = 0;
    (*dataStream) >> year >> month >> day >> hourmin >> julianDay
                  >> sec >> hp >> he >> hn >> total;

    while( dataStream->good() && !dataStream->eof() ) {
      curLine++;

      int i = 0;
      data[i++]->addValue( &year );
      data[i++]->addValue( &month );
      data[i++]->addValue( &day );
      data[i++]->addValue( &hourmin );
      data[i++]->addValue( &julianDay );
      data[i++]->addValue( &sec );
      if( hp == -1.00e+05 ) hp = 0.0;
      data[i++]->addValue( &hp );
      if( he == -1.00e+05 ) he = 0.0;
      data[i++]->addValue( &he );
      if( hn == -1.00e+05 ) hn = 0.0;
      data[i++]->addValue( &hn );
      if( total == -1.00e+05 ) total = 0.0;
      data[i++]->addValue( &total );

      (*dataStream) >> year >> month >> day >> hourmin >> julianDay
                    >> sec >> hp >> he >> hn >> total;
    }
  } else {
    LOG_ERR( "Failed to find separator artifact" )
  }
}// end void GpMagParser::parseFile()

/*
 * Modified: 08/19/10
 * Notes:    --Added findArtifact function to help with locating significant
 *             markings in a datafile
 *
 * Modified: 08/11/10
 * Notes:    Removed macro for sample data; could include in global header
 *           Updated to use debugging macros
 *
 * Modified: 07/28/10
 * Notes:    Included macro for printing sample data
 *
 * Modified: 07/15/10
 * Notes:    Initial creation.
 *           Side effect of manipulating class definition
 */

#include <crsCorr/fileParser.h>
#include <cstring>
#include <string>

//----< CONSTANTS >-------------------------------------------------------------



//----< ACCESSOR METHODS >------------------------------------------------------
const int FileParser::getLength() const {
  LOG_DEBUG( 7, "()" )

  return length;
}// end const int FileParser::getLength()


const FileParser::DataTag* const FileParser::getTags() const {
  LOG_DEBUG( 7, "()" )

  return dataTags;
}// end const FileParser::DataTag* const FileParser::getTags()

//----< (DE)(CON)STRUCTORS >----------------------------------------------------
FileParser::FileParser()
  : localFileName( "EMPTY" ),
    dataTags( NULL )
{
  LOG_DEBUG( 8, "()" )

  length = 0;
  dataStream = NULL;
  data = NULL;
  resamples = NULL;
  resamplesEnd = NULL;
}// end FileParser::FileParser()


FileParser::FileParser( string fileName,
                        const DataTag* const dataTags )
  : localFileName( fileName ),
    dataTags( dataTags )
{
  LOG_DEBUG( 8, "( " << fileName << ", labelSet )" )

  length = 0;
  while( dataTags[(length++) + 1 ].type != DATATYPE_END );

  dataStream = new std::ifstream( localFileName.c_str() );
  data = NULL;
  resamples = NULL;
  resamplesEnd = NULL;

  initDataSeries();
}// end FileParser::FileParser( std::ifstream, int, const char* [] )


FileParser::FileParser( const FileParser& copy )
  : localFileName( copy.localFileName ),
    dataTags( copy.dataTags )
{
  LOG_DEBUG( 8, "( const FileParser& copy)" )

  data = NULL;
  length = copy.length;
  dataStream = new std::ifstream( localFileName.c_str() );
  copyDataSeries( &data, copy.data, copy.length );
}// end FileParser::FileParser( const FileParser& )


FileParser::~FileParser() {
  LOG_DEBUG( 8, "()" )

  // clean up the data stream
  if( dataStream != NULL ) {
    dataStream->close();
    delete dataStream;
    dataStream = NULL;
  }

  // clean up the data
  if( data != NULL ) {
    for( int i = 0; i < length; i++ ) {
      if( data[i] != NULL ) {
        delete data[i];
        data[i] = NULL;
      }
    }
    delete[] data;
    data = NULL;
  }

  // clean up any lingering resamples
  if( resamples != NULL ) {
    resamplesEnd = resamples->next;
    while( resamplesEnd != NULL ) {
      delete resamples->series;
      delete resamples;
      resamples = resamplesEnd;
      resamplesEnd = resamplesEnd->next;
    }
    delete resamples->series;
    delete resamples;
    resamples = NULL;
    resamplesEnd = NULL;
  }
}// end FileParser::~FileParser()


//----< UTILITIES >-------------------------------------------------------------
void FileParser::copyDataSeries( AbstractDataSeries*** store,
                     const AbstractDataSeries * const * const copy,
                     const int seriesLength ) const
{
  LOG_DEBUG( 8, "( store, copy, " << seriesLength << " )" )

  if( seriesLength < 1 ) {
    LOG_DEBUG( 7, ": Invalid length of copy series" )
    return;
  } else if( copy == NULL ) {
    LOG_DEBUG( 7, ": Copy series is null." )
    return;
  }

  // free store location if necessary
  if( store != NULL && *store != NULL ) {
    delete[] *store;
    *store = NULL;
  }

  // create new storage area and copy objects over
  *store = new AbstractDataSeries*[ seriesLength ];
  for( int i = 0; i < seriesLength; i++ ) {
    if( dataTags[i].type == DATATYPE_DOUBLE ) {
      (*store)[i] = new DataSeries<double>( *copy[i] );
    } else if( dataTags[i].type == DATATYPE_INT ) {
      (*store)[i] = new DataSeries<int>( *copy[i] );
    } else {
      (*store)[i] = NULL;
    }
  }
}// end void copyDataSeries( AbstractDataSeries**, ... )


void FileParser::finalizeSeriesData() {
  LOG_DEBUG( 8, "()" )

  // finalize all series
  int minutesPerDay = 60 * 24;
  int zeroInt = 0;
  double zeroDouble = 0.0;
  for( int i = 0; i < length; i++ ) {
    if( data[i] != NULL ) {
      int addData = ( minutesPerDay / dataTags[i].reso ) - data[i]->getLength();
      if( dataTags[i].type == DATATYPE_DOUBLE ) {
        while( addData > 0 ) {
          data[i]->addValue( &zeroDouble );
          addData--;
        }
      } else if( dataTags[i].type == DATATYPE_INT ) {
        while( addData > 0 ) {
          data[i]->addValue( &zeroInt );
          addData--;
        }
      } else {
         //no-op, unknown data type
      }
      data[i]->finalizeData();
    }
  }
}// end void FileParser::finalizeDataSeries()


bool FileParser::findArtifact( const char* artifact ) {
  LOG_DEBUG( 8, "( " << artifact << " )" )
  string dataLine;
  bool found = false;

  while( !found && dataStream->good() && !dataStream->eof() ) {
    std::getline( *dataStream, dataLine );
    if( dataLine.find( artifact ) != string::npos ) {
      found = true;
    }
  }

  return found;
}// end bool FileParser::findArtifact( const char* )


void FileParser::initDataSeries() {
  LOG_DEBUG( 8, "()" )

  if( data != NULL ) {
    LOG_ERR( "Internal data was not null" )
    return;
  }

  getLength();
  data = new AbstractDataSeries*[ length ];

  for( int i = 0; i < length; i++ ) {
    if( dataTags[i].type == DATATYPE_DOUBLE ) {
      // DOUBLE
      LOG_DEBUG( 7, ": " << dataTags[i].label << " as DOUBLE" )
      data[i] = new DataSeries<double>( dataTags[i].label,
                                        dataTags[i].reso,
                                        dataTags[i].start );
    } else if( dataTags[i].type == DATATYPE_INT ) {
      // INT
      LOG_DEBUG( 7, ": " << dataTags[i].label << " as INT" )
      data[i] = new DataSeries<int>( dataTags[i].label,
                                     dataTags[i].reso,
                                     dataTags[i].start );
    } else {
      // IGNORE
      data[i] = NULL;
    }// determined type
  }// finished iterating
}// end void FileParser::FileParser initDataSeries()

//----< DATA METHODS >----------------------------------------------------------
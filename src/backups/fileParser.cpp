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
    length( 0 ),
    dataTags( NULL )
{
  LOG_DEBUG( 8, "()" )

  dataStream = NULL;
  data = NULL;
}// end FileParser::FileParser()

FileParser::FileParser( string fileName,
                        int length,
                        const DataTag* const dataTags )
  : localFileName( fileName ),
    length( length ),
    dataTags( dataTags )
{
  LOG_DEBUG( 8, "( " << fileName << ", " << length << ", labelSet )" )

  dataStream = new std::ifstream( localFileName.c_str() );
  data = NULL;

  initDataSeries();
}// end FileParser::FileParser( std::ifstream, int, const char* [] )

FileParser::FileParser( const FileParser& copy )
  : localFileName( copy.localFileName ),
    length( copy.length ),
    dataTags( copy.dataTags )
{
  LOG_DEBUG( 8, "( const FileParser& copy)" )

  data = NULL;
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
      }
    }
    delete[] data;
    data = NULL;
  }

  // clean up data tags
  if( dataTags != NULL ) {
//    delete[] dataTags;
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
  for( int i = 0; i < length; i++ ) {
    if( dataTags[i].type != DATATYPE_IGNORE ) {
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

  length = 0;
  while( dataTags[length++].type != DATATYPE_END );

  data = new AbstractDataSeries*[ length ];

  int i = 0;
  while( dataTags[i].type != DATATYPE_END ) {
    if( dataTags[i].type == DATATYPE_DOUBLE ) {
      // DOUBLE
      LOG_DEBUG( 7, ": " << dataTags[i].label << " as DOUBLE" )
      data[i] = new DataSeries<double>( dataTags[i].label );
    } else if( dataTags[i].type == DATATYPE_INT ) {
      // INT
      LOG_DEBUG( 7, ": " << dataTags[i].label << " as INT" )
      data[i] = new DataSeries<int>( dataTags[i].label );
    } else {
      // IGNORE
      data[i] = NULL;
    }// determined type
    i++;
  }// finished iterating
}// end void FileParser::FileParser initDataSeries()

//----< DATA METHODS >---------------------------------------------------------

/*
 * Modified:  08/12/10
 * Notes:     --Initial creation
 */

#include <crsCorr/global.h>
#include <crsCorr/abstractDataSeries.h>

// Initialize the reference count to zero
int AbstractDataSeries::refCount = 0;

//----<(DE|CON)STRUCTORS>-------------------------------------------------------
AbstractDataSeries::AbstractDataSeries( const string descriptor,
                                        const int reso,
                                        const int start )
  : label( string( descriptor ))
{
  LOG_DEBUG( 20, "( " << descriptor << ", "
                << reso << ", " << start << " )" )

  collecting = false;
  final = false;
  length = 0;

  if( reso >= 0 )
    resolution = reso;
  else
    resolution = DEFAULT_RESOLUTION;

  if( start >= 0 )
    startTime = start;
  else
    startTime = DEFAULT_START_TIME;

  AbstractDataSeries::refCount++;
}// end AbstractDataSeries::AbstractDataSeries( const char* )

AbstractDataSeries::AbstractDataSeries( const AbstractDataSeries& copy )
  : label( string( copy.getLabel()) )
{
  LOG_DEBUG( 2, "( const AbstractDataSeries& copy )" )

  // copy the basics
  collecting = copy.collecting;
  final = copy.final;
  length = copy.length;
  resolution = copy.resolution;
  startTime = copy.startTime;

  // increment ref count
  AbstractDataSeries::refCount++;
}// end AbstractDataSeries::AbstractDataSeries( const AbstractDataSeries& )

AbstractDataSeries::~AbstractDataSeries() {
  LOG_DEBUG( 2, "(): " << --AbstractDataSeries::refCount << " left" )

}// end AbstractDataSeries::~AbstractDataSeries()

//----<ACCESSOR METHODS>--------------------------------------------------------
const string& AbstractDataSeries::getLabel() const {
  //LOG_DEBUG( 2, "()" )

  return label;
}// end const string AbstractDataSeries::getLabel() const

const int AbstractDataSeries::getLength() const {
  //LOG_DEBUG( 2, "()" )

  return length;
}// end const int AbstractDataSeries::getLength() const

const int AbstractDataSeries::getResolution() const {
  //LOG_DEBUG( 2, "()" )

  return resolution;
}// end const int AbstractDataSeries::getResolution() const

const int AbstractDataSeries::getStartTime() const {
  //LOG_DEBUG( 2, "()" )

  return startTime;
}// end const int AbstractDataSeries::getStartTime() const

void AbstractDataSeries::incrementLength() {
  //LOG_DEBUG( 2, "()" )

  length++;
}// end void AbstractDataSeries::incrementLength() {

const bool AbstractDataSeries::isCollecting() const {
  //LOG_DEBUG( 2, "()" )

  return collecting;
}// end const bool AbstractDataSeries::isCollecting() const

const bool AbstractDataSeries::isFinal() const {
  //LOG_DEBUG( 2, "()" )

  return final;
}// end const bool AbstractDataSeries::isFinal() const

void AbstractDataSeries::setCollecting( const int res,
                                        const int start ) {
  LOG_DEBUG( 2, "()" )

  if( !final && !collecting ) {
    collecting = true;
    if( res > 0 && resolution == DEFAULT_RESOLUTION ) {
      resolution = res;
    } else if( resolution == DEFAULT_RESOLUTION ) {
      LOG_ERR( "Resolution value must be positive." )
      resolution = 1;
    } else {
      // no-op, see definition of ignore
    }

    if( start >= 0 && start == DEFAULT_START_TIME ) {
      startTime = start;
    } else if( start == DEFAULT_START_TIME ) {
      LOG_ERR( "Start time must be greater than or equal to zero." )
      startTime = 0;
    } else {
      // no-op, see definition of ignore
    }
  }
}// end void AbstractDataSeries::setCollecting()

void AbstractDataSeries::setFinal() {
  LOG_DEBUG( 2, "()" )

  if( collecting ) {
    collecting = false;
    final = true;
  }
}// end void AbstractDataSeries::setFinal()

const bool AbstractDataSeries::setParams(
    const int len,
    const int start,
    const int newRes )
{
  LOG_DEBUG( 2, "( " << len << ", " \
                << start << ", " \
                << newRes << " )" )

  // check state
  if( !collecting && !final ) {
    // check values
    if( len <= 0 ) {
      LOG_ERR( "Bad length ( non-positive )." )
    } else if( start < 0 ) {
      LOG_ERR( "Bad start time (less than zero)." )
    } else if( newRes <= 0 ) {
      LOG_ERR( "Bad resolution ( non-positive )." )
    } else {
      length = len;
      startTime = start;
      resolution = newRes;
      final = true;
      collecting = false;
      LOG_DEBUG( 3, ": Updated values. Returning true." )
      return true;
    }// good value

    return false;
  }// good state

  if( collecting ) {
    LOG_ERR( "Improper state. Collecting." )
  } else if( final ) {
    LOG_ERR( "Improper state. Finalized." )
  } else {
    LOG_ERR( "UNKNOWN ERR!!" )
  }
  return false;
}// end bool AbstractDataSeries::setParams( const int, const int, ... )

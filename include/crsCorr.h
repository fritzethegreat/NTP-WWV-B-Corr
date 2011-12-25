/*
 * Class which actually performs cross correlations on to provided arrays.
 *
 * Future work might involve including the nttw library to perform these
 *   calculations.
 *
 * Modified: 07/29/10
 * Notes:    --Modified function signatures for const
 *
 * Modified: 07/15/10
 * Notes:    --Initial creation
 */

#include <crsCorr/global.h>
#include <crsCorr/dataSeries.h>

#ifndef CRSCORR_CRSCORR_H
#define CRSCORR_CRSCORR_H

class crsCorr {
  public:
    /*
     * Perform a cross correlation on the two provided arrays.
     *
     * Calculation is performed by using various portions of the long signal
     *   and summing the product terms over an interval. A window function
     *   the same length as the short signal progresses backwards through the
     *   long signal to provide the long terms. The summed product for each
     *   delay term is placed in progressive slots of the result so that each
     *   index represents the delay term between the two signals.
     *
     * Calculation will only be performed if both data sets have matching
     *   resolutions. Length of the larger data must be a multiple of the
     *   smaller data set.
     *
     * Param:
     *  const DataSeries<ShortType>* -- short data set
     *  const DataSeries<LongType>* -- long data set
     *
     * Return: New array containing cross correlation. Each index implies
     *           one unit of delay equal to both DataSeries resolutions.
     *           If both data series contain five minute data, each delay is
     *           five minutes.
     */
    template<typename LongType, typename ShortType>
    static const DataSeries<double>* crossCorrDouble(
                           const DataSeries<ShortType>* shortSeries,
                           const DataSeries<LongType>* longSeries )
    {
      if( shortSeries == NULL ) {
        LOG_ERR( "Passed NULL for short data series." )
        return NULL;
      } else if( longSeries == NULL ) {
        LOG_ERR( "Passed NULL for long data series." )
        return NULL;
      }

      // debug code
      LOG_DEBUG( 13, ": Correlating " << shortSeries->getLength() << " with " \
                     << longSeries->getLength() << " values." )

      // parameter checks
      if( shortSeries->getResolution() != longSeries->getResolution() ) {
        LOG_ERR( "Mismatching resolutions." )
        return NULL;
      } else if( shortSeries->getStartTime() != longSeries->getStartTime() ) {
        LOG_ERR( "WARNING: Mismatching start times." )

      } else if( shortSeries->getLength() > longSeries->getLength() ) {
        LOG_ERR( "Short series is longer than long series" )
        return NULL;
      }

      // perform calcuation
      int shortLen = shortSeries->getLength();
      int longLen = longSeries->getLength();
      int length = longLen - shortLen + 1;
      double result[length][2];
      const LongType* const longSeriesData = longSeries->getData();
      const ShortType* const shortSeriesData = shortSeries->getData();
      double term = 0.0;
      for( int delay = 0; delay < length; delay++ ) {
        int startTerm = length - delay - 1;
        result[delay][0] = 0.0;
        result[delay][1] = 0.0;
        for( int index = 0; index < shortLen; index++ ) {
          double term = shortSeriesData[index] * longSeriesData[startTerm + index];
          if( term > 0.0 ) {
            result[delay][0] += term;
          } else {
            result[delay][1] += term;
          }
        }
        result[delay][0] += result[delay][1];
      }


      // generate result series
      DataSeries<double>* resultSeries = new DataSeries<double>( "RESULT" );
      resultSeries->addValue( result,
                             shortSeries->getResolution(),
                             shortSeries->getStartTime() );
      for( int i = 1; i < length; i++ ) {
        resultSeries->addValue( &result[i][0] );
      }

      // cleanup and return
      resultSeries->finalizeData();
      return resultSeries;
    }// end static DataSeries<double>* crsCorr::crsCorrDouble( ... )


    template<typename LongType, typename ShortType>
    static const DataSeries<int>* crossCorrInt(
                           const DataSeries<ShortType>* shortSeries,
                           const DataSeries<LongType>* longSeries )
    {
      if( shortSeries == NULL ) {
        LOG_ERR( "Passed NULL for short series" )
        return NULL;
      } else if( longSeries == NULL ) {
        LOG_ERR( "Passed NULL for long series" )
        return NULL;
      }

      // debug code
      LOG_DEBUG( 14, "( ... )" )
      LOG_DEBUG( 13, ": Correlating " << shortSeries->getLength() << " with " \
                     << longSeries->getLength() << " values." )

      // parameter checks
      if( shortSeries->getResolution() != longSeries->getResolution() ) {
        LOG_ERR( "Mismatching resolutions." )
        return NULL;
      } else if( shortSeries->getStartTime() != longSeries->getStartTime() ) {
        LOG_ERR( "WARNING: Mismatching start times." )
      } else if( shortSeries->getLength() > longSeries->getLength() ) {
        LOG_ERR( "Short series is longer than long series" )
        return NULL;
      }

      // perform calcuation
      int shortLen = shortSeries->getLength();
      int longLen = longSeries->getLength();
      int length = longLen - shortLen + 1;
      int result[length][2];
      const LongType* const longSeriesData = longSeries->getData();
      const ShortType* const shortSeriesData = shortSeries->getData();
      for( int delay = 0; delay < length; delay++ ) {
        int startTerm = length - delay - 1;
        result[delay][0] = 0;
        result[delay][1] = 0;
        for( int index = 0; index < shortLen; index++ ) {
          int term = shortSeriesData[index] * longSeriesData[startTerm + index]; 
          if( term > 0 ) {
            result[delay][0] += term;
          } else {
            result[delay][1] += term;
          }
        }
        result[delay][0] += result[delay][1];
      }
      LOG_DEBUG( 13, ": Result length -- " << length )

      // generate result series
      DataSeries<int>* resultSeries = new DataSeries<int>( "RESULT" );
      resultSeries->addValue( result,
                             shortSeries->getResolution(),
                             shortSeries->getStartTime() );
      for( int i = 1; i < length; i++ ) {
        resultSeries->addValue( &result[i][0] );
      }

      // cleanup and return
      resultSeries->finalizeData();
      return resultSeries;
    }// end static DataSeries<int>* crsCorr::crsCorrInt( ... )
};

#endif
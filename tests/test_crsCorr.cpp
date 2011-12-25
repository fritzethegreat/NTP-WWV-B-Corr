/*
 * Test software included with the crsCorr library to ensure proper
 *   functioning of the DoubleDataSeries class.
 *
 * Modified: 08/17/10
 * Notes:    --Initial Creation
 */

/*****************************************************************************
* Test Results
*--------------
* Functions:
* --crossCorr, int int        
* --crossCorr, double double  
* --crossCorr, int double     
* --crossCorr, double int     
*
* Valgrind:
* --leak-check=full           08/12/10 - NF
* --Other Errors              08/12/10 - NF
*****************************************************************************/

#include <iostream>
#include <iomanip>
#include <string>

//----------------------Testing files-----------------------------------------

#include <crsCorr/dataSeries.h>
#include <crsCorr/crsCorr.h>
#include <crsCorr/clkStatsParser.h>
#include <crsCorr/aceMagParser.h>

//----------------------Testing Vars------------------------------------------
#define START_VALUE -20
#define RANGE 40

using namespace std;

template<typename DataType>
DataType* loadSeries( const char* fileName );

void passFail( bool result );

template <class DataType>
bool testSeries( const DataSeries<DataType>& series,
                 const char* label,
                 const bool collecting,
                 const bool finalized,
                 const int resolution,
                 const int startTime,
                 const DataType* const data = NULL,
                 const int length = 0 );


int main() {
  bool crsCorrIntInt = false;
  bool crsCorrIntDouble = false;
  bool crsCorrDoubleInt = false;
  bool crsCorrDoubleDouble = false;
/*
  DataSeries<int> smallSeries( "SMALL" );
  for( int i = 2; i < 7; i++ ) {
    smallSeries.addValue( &i, 1, 0 );
  }

  DataSeries<int> longSeries( "LARGE" );
  for( int i = 2; i < 7; i++ ) {
    longSeries.addValue( &i, 1, 0 );
  }
  for( int i = 2; i < 7; i++ ) {
    longSeries.addValue( &i, 1, 0 );
  }

  smallSeries.finalizeData();
  longSeries.finalizeData();

  const DataSeries<int>* result = crsCorr::crossCorrInt( smallSeries, longSeries );
  const int* resultData = result->getData();
  for( int i = 0; i < result->getLength(); i++ )
    std::cout << resultData[i] << std::endl;


  std::ifstream dataFile( "/usr/local/src/crsCorr_pre-1.1/data/clockstats.20100716" );
  ClkStatsParser clkStats( &dataFile );

  std::ifstream dataFile_TWO( "/usr/local/src/crsCorr_pre-1.1/data/ace/20100813_ace_mag_1m.txt" );
  AceMagParser aceMag( &dataFile_TWO );

  std::ifstream dataFile_TWOTWO( "/usr/local/src/crsCorr_pre-1.1/data/ace/20100814_ace_mag_1m.txt" );
  AceMagParser aceMagTwo( &dataFile_TWOTWO );

  const FileParser::DataTag vsynmax10_tag = { "vsynmax10", DATATYPE_INT };
  const FileParser::DataTag hsynmax10_tag = { "hsynmax10", DATATYPE_INT };
  const FileParser::DataTag bx_tag = { "BX", DATATYPE_DOUBLE };
  const FileParser::DataTag bz_tag = { "BZ", DATATYPE_DOUBLE };

  const DataSeries<int>* vsynmax10 =
      clkStats.getSeries<int>( &vsynmax10_tag );
  const DataSeries<int>* hsynmax10 =
      clkStats.getSeries<int>( &hsynmax10_tag );
  DataSeries<double>*bxStart = new DataSeries<double>(
                             *aceMag.getSeries<double>( &bx_tag ),
                             "BX",
                             5,
                             vsynmax10->getStartTime() );
  DataSeries<double>*bxEnd = new DataSeries<double>(
                             *aceMagTwo.getSeries<double>( &bx_tag ),
                             "BX",
                             5,
                             vsynmax10->getStartTime() );
  DataSeries<double>*bx = *bxStart + *bxEnd;
  const DataSeries<double>* bz =
      aceMag.getSeries<double>( &bz_tag );

  const DataSeries<double>* result_IntDbl = crsCorr::crossCorrDouble(
          vsynmax10, bx );

  std::cout << "results: " << result_IntDbl << std::endl;
  const double* resultData_intdbl = result_IntDbl->getData();
  const int length = result_IntDbl->getLength();

  std::cout << "Results: " << result_IntDbl->getLabel() << std::endl;
  std::cout << "Result Data: " << resultData_intdbl << std::endl;
  std::cout << "Result Length: " << length << std::endl;

  std::ifstream testData( "/usr/local/src/crsCorr_pre-1.1/data/vsynmax10_BX.txt" );
  double checkValue = 0;
  int count = 0;
  for( int i = 0; i < length; i++ ) {
    testData >> checkValue;
    std::cout << checkValue << std::endl;
    if( checkValue != resultData_intdbl[i] ) {
      if( count == 0 ) {
        std::cerr << "Warning: Value mismatch. Expected " << checkValue
                  << "." << "Received " << resultData_intdbl[i] << std::endl;
      }
      count++;
    }
  }
  std::cerr << count << " other mismatches surpressed." << std::endl;
*/
  cout << endl;
  cout << "Test Summary:" << endl;
  cout << left << setfill( '-' ) << setw( 80 ) << "-" << setfill( ' ' ) << endl;
  cout << '|' << setw( 40 ) << " Test Name" << '|'
       << right << setw( 37 ) << " Result "  << '|' << left << endl;

  cout << setfill( '-' ) << setw( 80 ) << "-" << setfill( '.' ) << endl;
  cout << setw( 40 ) << " Cross Corr Int Int: ";
       passFail( crsCorrIntInt );
  cout << setw( 40 ) << " Cross Corr Int Double: ";
       passFail( crsCorrIntDouble );
  cout << setw( 40 ) << " Cross Corr Double Int: ";
       passFail( crsCorrDoubleInt );
  cout << setw( 40 ) << " Cross Corr Double Double: ";
       passFail( crsCorrDoubleDouble );

  return 0;
}// end int main()

template<typename DataType>
DataSeries<DataType>* loadSeries( const char* fileName ) {
  std::ifstream dataFile( fileName );
  DataSeries<DataType>* series = new DataSeries<DataType>( "TEST DATA" );

  DataType value;
  while( dataFile.good() && !dataFile.eof() ) {
    dataFile >> value;
    series.addValue( &value );
  }// end reading file

  series.finalizeData();

  return series;

}// end DataType* loadSeries( const char* filename )


void passFail( const bool result ) {
  if( result )
    cout << right << setw( 38 ) << "PASSED" << left << endl;
  else
    cout << right << setw( 38 ) << "FAILED" << left << endl;
}

template <class DataType>
bool testSeries( const DataSeries<DataType>& series,
                 const char* label,
                 const bool collecting,
                 const bool finalized,
                 const int resolution,
                 const int startTime,
                 const DataType* const data,
                 const int length )
{
  bool passed = true;

  // confirm label
  if( strcmp( series.getLabel(), label ) != 0 ) {
    cout << "ERR: Label mismatch." << endl;
    cout << "     ( " << series.getLabel() << " ) expected: "
         << label << endl;
    passed = false;
  }

  // confirm collection state
  if( series.isCollecting() != collecting ) {
    cout << "ERR: Collecting mismatch." << endl;
    cout << "     ( " << series.isCollecting() << " ) expected: "
         << collecting << endl;
    passed = false;
  }

  // confirm finalized state
  if( series.isFinal() != finalized ) {
    cout << "ERR: Finalized mismatch." << endl;
    cout << "     ( " << series.isFinal() << " ) expected: "
         << finalized << endl;
    passed = false;
  }

  // confirm resolution
  if( series.getResolution() != resolution ) {
    cout << "ERR: Resolution mismatch." << endl;
    cout << "     ( " << series.getResolution() << " ) expected: "
         << resolution << endl;
    passed = false;
  }

  // confirm start time
  if( series.getStartTime() != startTime ) {
    cout << "ERR: Start Time mismatch." << endl;
    cout << "     ( " << series.getStartTime() << " ) expected: "
         << startTime << endl;
    passed = false;
  }

  // confirm data (if possible)
  if( series.isFinal() ) {
    const DataType* const seriesData = series.getData();
    if( data != NULL && seriesData == NULL ) {
      cout << "ERR: Data is non-null. Expected null." << endl;
      passed = false;
    } else if( data == NULL && seriesData != NULL ) {
      cout << "ERR: Data is null. Expected values." << endl;
      passed = false;
    }// finished checking data values

    if( data != NULL && seriesData != NULL ) {
      int errCount = 0;
      for( int i = 0; i < length; i++ ) {
        if( passed && data[i] != seriesData[i] ) {
          cout << "ERR: Data value mismatch." << endl;
          cout << "     ( " << seriesData[i] << " ) expected: "
               << data[i] << endl;
          passed = false;
        } else if( data[i] != seriesData[i] ) {
          errCount++;
        }// checked data value
      }// iterated over data values

      cout << "   : " << errCount << " others surpressed." << endl;
    }// non-null data check
  }// avilable data check

  // output comparison result
  if( passed ) {
    cout << "PASSED!!!" << endl;
    return true;
  } else {
    cout << "FAILED." << endl;
    return false;
  }
}// end bool testSeries( const DoubleDataSeries&, const char* const, ... )
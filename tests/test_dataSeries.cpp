/*
 * Test software included with the crsCorr library to ensure proper
 *   functioning of the DoubleDataSeries class.
 *
 * Modified: 09/11/10
 * Notes:    --Updated default values for data series to reflect changes
 *             in the class ( resolution is now 1 instead of zero )
 *
 * Modified: 08/11/10
 * Notes:    --Continuing tests for old datasets
 *
 * Modified: 07/28/10
 * Notes:    --Licked wounds as we learned we signed off on tests too quickly
 *           --Problems related to sampling and retrieving data
 *
 * Modified: 07/15/10
 * Notes:    --Continuing tests.
 *           --Learned significance of Rule of Three
 *               {Copy constructor, Destructor, assignment operator}
 *
 * Modified: 07/13/10
 * Notes:    --Initial Creation
 */

/*****************************************************************************
* Test Results
*--------------
* Constructors:
* --Default Constructor       08/12/10 - NF
* --Param. Constructor        08/12/10 - NF
* --Copy Constructor
*   --Without Data            08/12/10 - NF
*   --With Data Collecting    08/12/10 - NF
*   --With Data Final
*     --Resample              08/24/10 - NF
*     --Resample / Start      08/24/10 - NF
*     --Start                 08/24/10 - NF
* Operators:
* --Assigment Operator        08/12/10 - NF
* --Output Operator           08/12/10 - NF
* --Addition Operator         08/14/10 - NF
* --Concatenate Operator      09/10/10 - NF
*
* Data Methods:
* --addValue                  08/12/10 - NF
* --finalizeData              08/12/10 - NF
* --finalizeEmptySeries       08/13/10 - NF
*
* Accessor Methods:
* --getLabel                  08/12/10 - NF
* --getLength                 08/12/10 - NF
* --getResolution             08/12/10 - NF
* --getStartTime              08/12/10 - NF
* --isCollecting              08/12/10 - NF
* --isFinal                   08/12/10 - NF
* --getData                   08/12/10 - NF
*
* Destructor:                 08/12/10 - NF
*
* Valgrind:
* --leak-check=full           08/12/10 - NF
* --Other Errors              08/12/10 - NF
*****************************************************************************/

#include <iostream>
#include <iomanip>
#include <string>

//-----------------------Testing files------------------------------------------

#include <crsCorr/dataSeries.h>

//-----------------------Testing Vars-------------------------------------------
#define START_VALUE -20
#define RANGE 40

//----------------------MACROS--------------------------------------------------
#define DELETE_OBJ_PTR( pointer ) \
  if( pointer != NULL ) {         \
    delete pointer;               \
    pointer = NULL;               \
  }


using namespace std;

void passFail( bool result );

template <class DataType>
bool testSeries( const DataSeries<DataType>* series,
                 const string label,
                 const bool collecting,
                 const bool finalized,
                 const int resolution,
                 const int startTime,
                 const DataType* const data = NULL,
                 const int length = 0 );


int main() {
  bool defCon = false;
  bool lblCon = false;
  bool copyConNoData = false;
  bool oneValue = false;
  bool copyConDataCollect = false;
  bool multiValue = false;
  bool additionOp = false;
  bool concatenateOp = false;
  bool restartValues = false;
  bool resampleValues = false;
  bool resampleStartValues = false;
  bool copyConDataFinal = false;
  bool reResampleValues = false;

  // default constructor
  {
    cout << "Testing Default Constructor<int>:" << endl;
    const char* testLabel_empty = "EMPTY";
    DataSeries<int>* genericInt = new DataSeries<int>();
    defCon = testSeries( genericInt, testLabel_empty, false, false, 1, 0 );
    cout << endl;
    DELETE_OBJ_PTR( genericInt )
  }

  cout << endl;

  // constructor with label
  {
    cout << "Testing Label Constructor<int>:" << endl;
    const char* testLabel = "JONES";
    DataSeries<int>* genericInt = new DataSeries<int>( testLabel );
    lblCon = testSeries( genericInt, testLabel, false, false, 1, 0 );
    cout << endl;
    DELETE_OBJ_PTR( genericInt )
  }

  cout << endl;

  // copy constructor -- no data
  {
    cout << "Testing Copy Constructor<int> with no data:" << endl;
    const char* testLabel = "WALTER";
    DataSeries<int>* genericInt = new DataSeries<int>( testLabel );
    DataSeries<int>* copiedInt = new DataSeries<int>( *genericInt );
    copyConNoData = testSeries( copiedInt, testLabel, false, false, 1, 0 );
    cout << endl;
    DELETE_OBJ_PTR( genericInt )
    DELETE_OBJ_PTR( copiedInt )
  }

  cout << endl;

  // add and retrieve 25 values
  {
    cout << "Testing Storing One Value<int>:" << endl;
    const char* testLabel = "FREDDY";
    DataSeries<int>* genericInt = new DataSeries<int>( testLabel );
    int length = 5000;
    int* testData = new int[ length ];
    int firstValue = 0;
    testData[ 0 ] = firstValue;
    genericInt->addValue( &firstValue );
    oneValue = testSeries( genericInt, testLabel, true, false, 1, 0, testData, 1 );
    cout << endl;

    cout << "Testing Copy Constructor -- with data collecting:" << endl;
    DataSeries<int>* copyCollect = new DataSeries<int>( *genericInt );
    copyConDataCollect = testSeries( copyCollect, testLabel, true, false, 1, 0,
                                     testData, 1 );
    cout << endl;


    cout << "Testing with more values<int>:" << endl;
    for( int i = 1; i < length; i++ ) {
      testData[i] = i;
      genericInt->addValue( &i );
    }
    genericInt->finalizeData();
    multiValue = testSeries( genericInt, testLabel, false, true, 1, 0, testData, length );
    cout << endl;

    cout << "Testing Addition Operator:" << endl;
    DataSeries<int> summation = *genericInt + *genericInt;
    int* combinedData = new int[ length * 2 ];
    for( int i = 0; i < length; i++ ) {
      combinedData[i] = i;
      combinedData[i+length] = i;
    }
    additionOp = testSeries( &summation, testLabel, false, true, 1, 0,
                             combinedData, length * 2 );
    cout << endl;

    cout << "Testing Concatenate Operator:" << endl;
    DataSeries<int> concat = *genericInt;
    concat += *genericInt;
    concatenateOp = testSeries( &summation, testLabel, false, true, 1, 0,
                                combinedData, length * 2 );
    cout << endl;

    cout << "Testing Resampling Values<int>:" << endl;
    int resampleRate = 5;
    DataSeries<int>* resampled = new DataSeries<int>( *genericInt,
                                                      "RESAMPLED",
                                                      resampleRate );
    int resampleLength = length / resampleRate;
    int* resampledTestData = new int[ resampleLength ];
    for( int i = 0; i < resampleLength; i++ ) {
      resampledTestData[i] = testData[i * resampleRate];
    }
    resampleValues = testSeries( resampled, "RESAMPLED", false, true,
                                 resampleRate, 0,
                                 resampledTestData, resampleLength );
    cout << endl;

    cout << "Testing Restarting Values<int>:" << endl;
    int newStart = 5;
    DataSeries<int>* restarted = new DataSeries<int>( *genericInt,
                                                      "RESTARTED",
                                                      1,
                                                      newStart );
    int* restartedData = new int[ length - newStart ];
    for( int i = 0; i < length - 1; i++ ) {
      restartedData[i] = testData[i + newStart ];
    }
    restartValues = testSeries( restarted, "RESTARTED", false, true,
                                1, newStart,
                                restartedData, length - newStart );
    cout << endl;

    cout << "Testing Resampling Restart Values<int>:" << endl;
    DataSeries<int>* resampledRestart = new DataSeries<int>( *genericInt,
                                                             "RESAMPLED",
                                                             resampleRate,
                                                             newStart );
    int* resampledRestartTestData = new int[ resampleLength ];
    for( int i = 0; i < resampleLength; i++ ) {
      resampledRestartTestData[i] = testData[i * resampleRate + newStart];
    }
    resampleStartValues = testSeries( resampledRestart, "RESAMPLED",
                                      false, true, resampleRate,  newStart,
                                      resampledRestartTestData, resampleLength );
    cout << endl;

    cout << "Testing Copy Constructor -- with data final:" << endl;
    DataSeries<int>* copy = new DataSeries<int>( *resampled );
    copyConDataFinal = testSeries( copy, "RESAMPLED", false, true, resampleRate, 0,
                                   resampledTestData, resampleLength );
    cout << endl;

    cout << "Testing Re-Resampling Values<int>:" << endl;
    int reReSampleRate = 10;
    DataSeries<int>* reReSampled = new DataSeries<int>( *resampled,
                                                        "RE-RESAMPLED",
                                                        reReSampleRate );
    int reReSampleLength = resampleLength / reReSampleRate;
    int* reReSampledTestData = new int[ reReSampleLength ];
    for( int i = 0; i < reReSampleLength; i++ ) {
      reReSampledTestData[i] = testData[ i * reReSampleRate ];
    }
    reResampleValues = testSeries( reReSampled, "RE-RESAMPLED", false, true, reReSampleRate, 0,
                                   reReSampledTestData, reReSampleLength );

    DELETE_OBJ_PTR( resampledRestart )
    DELETE_OBJ_PTR( reReSampled )
    DELETE_OBJ_PTR( resampled )
    DELETE_OBJ_PTR( copy )
    DELETE_OBJ_PTR( copyCollect )
    DELETE_OBJ_PTR( genericInt )
    delete[] combinedData;
    delete[] reReSampledTestData;
    delete[] resampledRestartTestData;
    delete[] resampledTestData;
    delete[] testData;
  }

  cout << endl;
  cout << "Test Summary:" << endl;
  cout << left << setfill( '-' ) << setw( 80 ) << "-" << setfill( ' ' ) << endl;
  cout << '|' << setw( 40 ) << " Test Name" << '|'
       << right << setw( 37 ) << " Result "  << '|' << left << endl;
  cout << setfill( '-' ) << setw( 80 ) << "-" << setfill( '.' ) << endl;
  cout << setw( 40 ) << " Default Constructor: ";
       passFail( defCon );
  cout << setw( 40 ) << " Label Constructor: ";
       passFail( lblCon );
  cout << setw( 40 ) << " Copy Constructor - No Data: ";
       passFail( copyConNoData );
  cout << setw( 40 ) << " Copy Consturctor - With Data Collect: ";
       passFail( copyConDataCollect );
  cout << setw( 40 ) << " Copy Consturctor - With Data Final: ";
       passFail( copyConDataFinal );
  cout << setw( 40 ) << " Restarted Values: ";
       passFail( restartValues );
  cout << setw( 40 ) << " Add Operator: ";
       passFail( additionOp );
  cout << setw( 40 ) << " Concatenate Operator: ";
       passFail( concatenateOp );
  cout << setw( 40 ) << " One Value: ";
       passFail( oneValue );
  cout << setw( 40 ) << " MultiValues: ";
       passFail( multiValue );
  cout << setw( 40 ) << " Once Sampled: ";
       passFail( resampleValues );
  cout << setw( 40 ) << " Once Sampled with new Start: ";
       passFail( resampleStartValues );
  cout << setw( 40 ) << " Twice Sampled: ";
       passFail( reResampleValues );

  return 0;
}// end int main()

void passFail( const bool result ) {
  cout << right << setw( 38 );

  if( result )
    cout << "  PASSED  ";
  else
    cout << "<<FAILED>>";

 cout << left << endl;
}// end void passFail( const bool )

template <class DataType>
bool testSeries( const DataSeries<DataType>* series,
                 const string label,
                 const bool collecting,
                 const bool finalized,
                 const int resolution,
                 const int startTime,
                 const DataType* const data,
                 const int length )
{
  bool passed = true;

  // confirm label
  if( series->getLabel().compare( label ) != 0 ) {
    cout << "ERR: Label mismatch." << endl;
    cout << "     ( " << series->getLabel() << " ) expected: "
         << label << endl;
    passed = false;
  }

  // confirm collection state
  if( series->isCollecting() != collecting ) {
    cout << "ERR: Collecting mismatch." << endl;
    cout << "     ( " << series->isCollecting() << " ) expected: "
         << collecting << endl;
    passed = false;
  }

  // confirm finalized state
  if( series->isFinal() != finalized ) {
    cout << "ERR: Finalized mismatch." << endl;
    cout << "     ( " << series->isFinal() << " ) expected: "
         << finalized << endl;
    passed = false;
  }

  // confirm resolution
  if( series->getResolution() != resolution ) {
    cout << "ERR: Resolution mismatch." << endl;
    cout << "     ( " << series->getResolution() << " ) expected: "
         << resolution << endl;
    passed = false;
  }

  // confirm start time
  if( series->getStartTime() != startTime ) {
    cout << "ERR: Start Time mismatch." << endl;
    cout << "     ( " << series->getStartTime() << " ) expected: "
         << startTime << endl;
    passed = false;
  }

  // confirm data (if possible)
  if( series->isFinal() ) {
    const DataType* const seriesData = series->getData();
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
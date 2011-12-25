/*
 * Test software included with the crsCorr library to ensure proper
 *   functioning of the AceMagParser class.
 *
 * Testing all of the class will prove difficult, especially since access to
 *   data members and internal state is limited and almost non-existent.
 * It should be sufficient to show that proper values are parsed and stored
 *   from the data file.
 *
 * Modified: 08/13/10
 * Notes:    --Initial Creation
 */

/*****************************************************************************
* Test Results
*--------------
* Constructors:
* --Default Constructor       08/13/10 - NF
* --Param. Constructor        08/13/10 - NF
* --Copy Constructor
*   --Without Data            
*   --With Data               08/13/10 - NF
*
* Operators:
* --Assigment Operator        
*
* Data Methods:
* --parseFile                 08/13/10 - NF
*
* Destructor:                 08/13/10 - NF
*
* Valgrind:
* --leak-check=full           08/13/10 - NF
* --Other Errors              08/13/10 - NF
*****************************************************************************/

#include <iostream>
#include <string>

//----------------------Testing files-----------------------------------------

#include <crsCorr/dataSeries.h>
#include <crsCorr/aceMagParser.h>

//----------------------Testing Vars------------------------------------------
#define TEST_DATA_FILE "/usr/local/src/crsCorr_pre-1.1/data/ace/20100813_ace_mag_1m.txt"
const char TEST_DATA_PATH[] = "/usr/local/src/crsCorr_pre-1.1/data/ace";

using namespace std;

void passFail( bool result );

template <class DataType>
bool testSeries( const DataSeries<DataType>& series,
                 const string label,
                 const bool collecting,
                 const bool finalized,
                 const int resolution,
                 const int startTime,
                 const DataType* const data = NULL,
                 const int length = 0 );

bool testValues( const AceMagParser& parser );

int main() {
  bool defCon = false;
  bool paramCon = false;
  bool copyConNoData = false;
  bool copyConData = false;
  bool assignOp = false;
  bool parseFile = false;

  // default constructor
  // NOTE -- trivial test since it accomplishes nothing. Getting through the
  //         contstructor without error is sufficient.
  {
    cout << "Testing Default Constructor:" << endl;
    AceMagParser generic();
    defCon = true;
    passFail( defCon );
    cout << endl;
  }

  cout << endl;

  // constructor with parameters
  {
    cout << "Testing Param Constructor:" << endl;
    std::ifstream inputFile( TEST_DATA_FILE );
    AceMagParser generic( &inputFile );
    paramCon = true;
    passFail( paramCon );
    cout << endl;

    // parsing file
    cout << "Testing File Parsing:" << endl;
    generic.parseFile();
    parseFile = testValues( generic );
    passFail( parseFile );
    cout << endl;

    cout << "Testing Copy Constructor - With Data:" << endl;
    AceMagParser copy( generic );
    copyConData = testValues( copy );
    passFail( copyConData );
    cout << endl;


  }// finished test block

  cout << "Test Summary:" << endl;
  cout << "Default Constructor: "; passFail( defCon );
  cout << "Param Constructor: "; passFail( paramCon );
  cout << "Copy Constructor - No Data: "; passFail( copyConNoData );
  cout << "Copy Consturctor - With Data: "; passFail( copyConData );
  cout << "Assignment Operator: "; passFail( assignOp );
  cout << "Parse File: "; passFail( parseFile );

  return 0;
}// end int main()

void passFail( const bool result ) {
  if( result )
    cout << "PASSED" << endl;
  else
    cout << "FAILED" << endl;
}

template <class DataType>
bool testSeries( const DataSeries<DataType>& series,
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
  if( series.getLabel().compare( label ) != 0 ) {
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


bool testValues( const AceMagParser& parser ) {
  bool result = true;

  for( int i = 0; i < AceMagParser::length; i++ ) {
    string dataName = AceMagParser::tags[i].label;
    int dataType = AceMagParser::tags[i].type;
    string dataFileName;
    dataFileName += TEST_DATA_PATH;
    dataFileName += dataName;
    std::ifstream dataFile( dataFileName.c_str() );

    if( !dataFile.fail() ) {
      std::cerr << "Testing: " << dataName << std::endl;
      if( dataType == DATATYPE_INT ) {
        const DataSeries<int>* series =
            parser.getSeries<int>( &AceMagParser::tags[i] );
        int len = series->getLength();
        const int* values = series->getData();
        int check;
        for( int i = 0; i < len; i++ ) {
          dataFile >> check;
          if( values[i] != check ) {
            result = false;
          }
        }
      } else if( dataType == DATATYPE_DOUBLE ) {
        const DataSeries<double>* series =
           parser.getSeries<double>( &AceMagParser::tags[i] );
        int len = series->getLength();
        const double* values = series->getData();
        int check;
        for( int i = 0; i < len; i++ ) {
          dataFile >> check;
          if( values[i] != check ) {
            result = false;
          }
        }
      } else {
      }
    }// good data file
  }// finished data label

  return result;

}// end bool testValues( const AceMagParser )

/*
 * Test software included with the crsCorr library to ensure proper
 *   functioning of the various parser classes.
 *
 * Testing all of the class will prove difficult, especially since access to
 *   data members and internal state is limited and almost non-existent.
 * It should be sufficient to show:
 *   --Proper values are parsed and stored within DataSeries
 *   --DataSeries can be retrieved
 *   --Retrieving a non-stored DataSeries returns default DataSeries
 *
 * Modified: 09/10/10
 * Notes:    --Further tests indicated a flaw in the parsing methods which
 *             resulted in an additional duplicated value at the end of each
 *             series.
 *           --Tests are now designed to ensure appropriate length of each
 *             series.
 *
 * Modified: 08/19/10
 * Notes:    --Reconstructed for testing all parsers in a template fashion
 *           --Including a test for non-included DataSeries
 *
 * Modified: 08/13/10
 * Notes:    --Initial Creation
 */

/*******************************************************************************
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
*******************************************************************************/

#include <iomanip>
#include <iostream>
#include <string>

//----------------------Testing files-------------------------------------------

#include <crsCorr/dataSeries.h>
#include <crsCorr/aceMagParser.h>
#include <crsCorr/aceSweParser.h>
#include <crsCorr/clkStatsParser.h>
#include <crsCorr/gpMagParser.h>
#include <crsCorr/gpXrayParser.h>
#include <crsCorr/gsMagParser.h>
#include <crsCorr/gpPartParser.h>
#include <crsCorr/gsPartParser.h>

//----------------------Testing Vars--------------------------------------------

const string TEST_DATA_PATH = "/usr/local/src/crsCorr_pre-1.1/data/";

const string TEST_DATA_DIR_ACEMAG = "aceMag/";
const string TEST_DATA_FILE_ACEMAG = "20100813_ace_mag_1m.txt";

const string TEST_DATA_DIR_ACESWE = "aceSwe/";
const string TEST_DATA_FILE_ACESWE = "20100813_ace_swepam_1m.txt";

const string TEST_DATA_DIR_CLKSTATS = "clkStats/";
const string TEST_DATA_FILE_CLKSTATS = "clockstats.20100716";

const string TEST_DATA_DIR_GSMAG = "gsMag/";
const string TEST_DATA_FILE_GSMAG = "20100813_Gs_mag_1m.txt";

const string TEST_DATA_DIR_GPMAG = "gpMag/";
const string TEST_DATA_FILE_GPMAG = "20100813_Gp_mag_1m.txt";

const string TEST_DATA_DIR_GPXRAY = "gpXray/";
const string TEST_DATA_FILE_GPXRAY = "20100813_Gp_xr_1m.txt";

const string TEST_DATA_DIR_GPPART = "gpPart/";
const string TEST_DATA_FILE_GPPART = "20100813_Gp_part_5m.txt";

const string TEST_DATA_DIR_GSPART = "gsPart/";
const string TEST_DATA_FILE_GSPART = "20100813_Gs_part_5m.txt";

//#define ACEMAG
//#define ACESWE
#define CLKSTATS
//#define GPMAG
//#define GPXRAY
//#define GSMAG
//#define GSPART
//#define GPPART

//------------------------------------------------------------------------------
using namespace std;

/*
 * Displays formatted "N/A" for test result signifying test was not performed.
 */
void notApp();

/*
 * Displays a boolean as either a PASSED(true) or FAILED(false)
 */
void passFail( bool result );

/*
 * Iterates through the provided file parser's DataSeries and tests each one
 *   against a provided DataSet. If all values match as expected, function
 *   returns true. Otherwise, discrepancies are reported.
 */
bool testValues( FileParser* parser,
                 string testDataPath,
                 const int length );

/*
 * Exercises the provided parser to bring out defects. Any failed tests are
 *   reported. Otherwise, the function returns true.
 */
bool testParser( FileParser* parser,
                 string testDataPath );


int main() {
  bool test_aceMag = false;
  bool test_aceSwe = false;
  bool test_clkStats = false;
  bool test_gpMag = false;
  bool test_gpXray = false;
  bool test_gsMag = false;
  bool test_gsPart = false;
  bool test_gpPart = false;

  // AceMagParser
  #ifdef ACEMAG
  {
    cout << "Testing AceMagParser: " << endl;
    string testDataPath = TEST_DATA_PATH + TEST_DATA_DIR_ACEMAG;
    string testDataFile = testDataPath + TEST_DATA_FILE_ACEMAG;
    cout << "  Loading " << testDataFile << endl;
    AceMagParser aceMag( testDataFile );
    test_aceMag = testValues( &aceMag, testDataPath, 1440 );
  }
  #endif

  // AceSweParser
  #ifdef ACESWE
  {
    cout << "Testing AceSweParser: " << endl;
    string testDataPath = TEST_DATA_PATH + TEST_DATA_DIR_ACESWE;
    string testDataFile = testDataPath + TEST_DATA_FILE_ACESWE;
    cout << "  Loading " << testDataFile << endl;
    AceSweParser aceSwe( testDataFile );
    test_aceSwe = testValues( &aceSwe, testDataPath, 1440 );
  }
  #endif

  // ClkStatsParser
  #ifdef CLKSTATS
  {
    cout << "Testing ClkStatsParser: " << endl;
    string testDataPath = TEST_DATA_PATH + TEST_DATA_DIR_CLKSTATS;
    string testDataFile = testDataPath + TEST_DATA_FILE_CLKSTATS;
    cout << "  Loading " << testDataFile << endl;
    ClkStatsParser clkStats( testDataFile );
    test_clkStats = testValues( &clkStats, testDataPath, 288 );
  }
  #endif

  // GpMagParser
  #ifdef GPMAG
  {
    cout << "Testing GpMagParser: " << endl;
    string testDataPath = TEST_DATA_PATH + TEST_DATA_DIR_GPMAG;
    string testDataFile = testDataPath + TEST_DATA_FILE_GPMAG;
    cout << "  Loading " << testDataFile << endl;
    GpMagParser gpMag( testDataFile );
    test_gpMag = testValues( &gpMag, testDataPath, 1440 );
  }
  #endif

  // GpXrayParser
  #ifdef GPXRAY
  {
    cout << "Testing GpXrayParser: " << endl;
    string testDataPath = TEST_DATA_PATH + TEST_DATA_DIR_GPXRAY;
    string testDataFile = testDataPath + TEST_DATA_FILE_GPXRAY;
    cout << "  Loading " << testDataFile << endl;
    GpXrayParser gpXray( testDataFile );
    test_gpXray = testValues( &gpXray, testDataPath, 1440 );
  }
  #endif

  // GsMagParser
  #ifdef GSMAG
  {
    cout << "Testing GsMagParser: " << endl;
    string testDataPath = TEST_DATA_PATH + TEST_DATA_DIR_GSMAG;
    string testDataFile = testDataPath + TEST_DATA_FILE_GSMAG;
    cout << "  Loading " << testDataFile << endl;
    GsMagParser gsMag( testDataFile );
    test_gsMag = testValues( &gsMag, testDataPath, 1440 );
  }
  #endif

  // GsPartParser
  #ifdef GSPART
  {
    cout << "Testing GsPartParser: " << endl;
    string testDataPath = TEST_DATA_PATH + TEST_DATA_DIR_GSPART;
    string testDataFile = testDataPath + TEST_DATA_FILE_GSPART;
    cout << "  Loading " << testDataFile << endl;
    GsPartParser gsPart( testDataFile );
    test_gsPart = testValues( &gsPart, testDataPath, 288 );
  }
  #endif

  // GpMagParser
  #ifdef GPPART
  {
    cout << "Testing GpPartParser: " << endl;
    string testDataPath = TEST_DATA_PATH + TEST_DATA_DIR_GPPART;
    string testDataFile = testDataPath + TEST_DATA_FILE_GPPART;
    cout << "  Loading " << testDataFile << endl;
    GpPartParser gpPart( testDataFile );
    test_gpPart = testValues( &gpPart, testDataPath, 288 );
  }
  #endif

  cout << "Test Summary:" << endl;
  cout << left << setfill( '-' ) << setw( 80 ) << "-" << setfill( ' ' ) << endl;
  cout << '|' << setw( 40 ) << " Test Name" << '|'
       << right << setw( 37 ) << " Result "  << '|' << left << endl;
  cout << setfill( '-' ) << setw( 80 ) << "-" << setfill( '.' ) << endl;

  cout << setw( 40 ) << " Ace Mag Parser: ";
  #ifdef ACEMAG
    passFail( test_aceMag );
  #else
    notApp();
  #endif

  cout << setw( 40 ) << " Ace Swe Parser: ";
  #ifdef ACESWE
     passFail( test_aceSwe );
  #else
    notApp();
  #endif

  cout << setw( 40 ) << " Clockstats Parser: ";
  #ifdef CLKSTATS
     passFail( test_clkStats );
  #else
    notApp();
  #endif

  cout << setw( 40 ) << " Gp Mag Parser: ";
  #ifdef GPMAG
     passFail( test_gpMag );
  #else
    notApp();
  #endif

  cout << setw( 40 ) << " Gp Xray Parser: ";
  #ifdef GPXRAY
     passFail( test_gpXray );
  #else
    notApp();
  #endif

  cout << setw( 40 ) << " Gs Mag Parser: ";
  #ifdef GSMAG
     passFail( test_gsMag );
  #else
    notApp();
  #endif

  cout << setw( 40 ) << " Gs Part Parser: ";
  #ifdef GSPART
     passFail( test_gsPart );
  #else
    notApp();
  #endif

  cout << setw( 40 ) << " Gp Part Parser: ";
  #ifdef GPPART
     passFail( test_gpPart );
  #else
    notApp();
  #endif

  return 0;
}// end int main()

void notApp() {
  cout << right << setw( 38 );

  cout <<   "    N/A   " << endl;

  cout << left;
}

void passFail( const bool result ) {
  cout << right << setw( 38 );

  if( result )
    cout << "  PASSED  " << endl;
  else
    cout << "<<FAILED>>" << endl;

  cout << left;
}// end void passFail( const bool )


bool testValues( FileParser* parser,
                 const string testDataPath,
                 const int length )
{
  bool result = true;
  int seriesCount = parser->getLength();
  const FileParser::DataTag* const tags = parser->getTags();

  for( int i = 0; i < seriesCount; i++ ) {
    string dataName = tags[i].label;
    int dataType = tags[i].type;
    int errCount = 0;

    // begin tests of the dataType is of interest
    if( dataType == DATATYPE_INT || dataType == DATATYPE_DOUBLE ) {

      // form file name
      string dataFileName;
      dataFileName +=  testDataPath;
      dataFileName += dataName;
      std::ifstream dataFile( dataFileName.c_str() );

      // load data file and check values
      if( !dataFile.fail() ) {
        // data file was found
        std::cerr << "Testing: " << tags[i].label << " using values from "
                  << dataFileName.c_str() << std::endl;

        // test based off data type
        if( dataType == DATATYPE_INT ) {
          // int case
          const DataSeries<int>* series =
              parser->getSeries<int>( &tags[i] );
          if( series == NULL || series->getData() == NULL )  {
            std::cerr << "ERR: No data for " << dataName << std::endl;
            result = false;
          } else {
            int len = series->getLength();
            if( len != length ) {
              LOG_ERR( "Data length mismatch. Expected " << length << "." \
                       << " Received " << len )
              result = false;
            }
            const int* values = series->getData();
            cout << "DataSeries('" << series->getLabel() << "'): "
                 << series->getResolution() << " " << series->getStartTime() << endl;
            int check;
            for( int i = 0; i < len; i++ ) {
              dataFile >> check;
              if( errCount == 0 && values[i] != check ) {
                LOG_ERR( "Data mismatch. Correct Value: " << check << ". Test Value:  " \
                         << values[i] << "." )
                result = false;
                errCount++;
              } else if( values[i] != check ) {
                errCount++;
              }
            }
          }
        } else if( dataType == DATATYPE_DOUBLE ) {
          // double case
          const DataSeries<double>* series =
             parser->getSeries<double>( &tags[i] );
          if( series == NULL || series->getData() == NULL ) {
            std::cerr << "ERR: No data for " << dataName << std::endl;
            result = false;
          } else {
            int len = series->getLength();
            if( len != length ) {
              LOG_ERR( "Data length mismatch. Expected " << length << "." \
                       << " Received " << len )
              result = false;
            }
            const double* values = series->getData();
            double check;
            cout << "DataSeries('" << series->getLabel() << "'): "
                 << series->getResolution() << " " << series->getStartTime() << endl;
            for( int i = 0; i < len; i++ ) {
              dataFile >> check;
              if( check == -9999.9 || check == -999.9 || check == -100000.0 ) {
                check = 0.0;
              }
              if( errCount == 0 && values[i] != check ) {
                LOG_ERR( "Data mismatch. Expected " << check << ". Received " \
                         << values[i] << "." )
                result = false;
                errCount++;
              } else if( values[i] != check ) {
                errCount++;
              }
            }
          }
        } else if( dataType == DATATYPE_IGNORE ) {
          // no-op, see definition of ignore
        } else {
          std::cerr << "ERR: Non-recognized data type." << std::endl;
        }
        if( errCount > 0 ) {
          LOG_ERR( errCount << " total errors." )
        }
      } else {
        LOG_ERR( "Unable to find data file: " << dataFileName.c_str() )
        result = false;
      }
    }// finished significants check
  }// finished data label

  return result;
}// end bool testValues( const FileParser* )

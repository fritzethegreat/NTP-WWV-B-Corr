
/*
 * Parses files specifically from the ace satellite magnetometer provided by the
 *   National Space Weather Prediction Center (NSWPC). Examples of this format
 *   are available at:
 *      ftp://ftp.swpc.noaa.gov/pub/lists/ace/ace_mag_1m.txt
 *
 * Modified: 07/16/10
 * Notes:    Initial Creation
 */

#include <crsCorr/fileParser.h>
#include <crsCorr/dataSeries.h>

#ifndef CRSCORR_ACESWE_FILEPARSER_H
#define CRSCORR_ACESWE_FILEPARSER_H

#define ACESWE_LINESIZE 180

class AceSweParser : public FileParser {

  public:
    /*
     * Determines which values to extract from the given data file.
     * Below, the available columns are specified along with their units.
     * Data ranges are specified and should be considered when chosing which
     * data series(es) use.
     *
     * Proton Density = p/cc
     * Bulk Speed = km/s
     * Ion temp = degrees K
     * Missing Data -9999.9, -1.00e+05
     *
     * Label        Value                               Suggested Type
     *-------------------------------------------------------------------------
     *  YR           Year                                Int
     *  MO           Month                               Int
     *  DA           Day                                 Int
     *  HHMM         Hrs/Min                             Int
     *  DAY          Julian Calendar day                 Int
     *  SEC          Seconds of the day                  Int
     *  STATUS       Status of the data reading          Int
     *  Proton       Proton density                      Double
     *  Speed        Bulk Speed                          Double
     *  Temp         Ion Temperature                     Double
     */
   static const FileParser::DataTag tags[];

    //----< (DE/CON)STRUCTORS >------------------------------------------------
    /*
     * Constructors are supplied without parameters as those shouldn't be
     *   needed (minus the copy constructor). Destructors and the assignment
     *   operator are also provided here.
     */
    AceSweParser();
    AceSweParser( string fileName );
    AceSweParser( const AceSweParser& copy );
    ~AceSweParser();
    AceSweParser& operator=(const AceSweParser& copy );

  protected:
    //----< FILE METHODS >-----------------------------------------------------
    /*
     * Begins the actual file processing and fills the associated data file
     *   with various types of DataSeries.
     */
    void parseFile();

};

#endif
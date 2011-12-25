
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

#ifndef CRSCORR_GPXRAY_FILEPARSER_H
#define CRSCORR_GPXRAY_FILEPARSER_H

#define GPXRAY_LINESIZE 180

class GpXrayParser : public FileParser {

  public:
    /*
     * Determines which values to extract from the given data file.
     * Below, the available columns are specified along with their units.
     * Data ranges are specified and should be considered when chosing which
     * data series(es) use.
     *
     * Short = 0.05 - 0.4 nanometer, measured in watts / m^2
     * Long  = 0.1  - 0.8 nanometer, measured in watts / m^2
     * Missing Data -1.00e+05
     *
     * Label        Value                               Suggested Type
     *-------------------------------------------------------------------------
     *  YR           Year                                Int
     *  MO           Month                               Int
     *  DA           Day                                 Int
     *  HHMM         Hrs/Min                             Int
     *  DAY          Julian Calendar day                 Int
     *  SEC          Seconds of the day                  Int
     *  SHORT        Short xray power, 0.05 - 0.4 nm     Double
     *  LONG         Long xray power, 0.1 - 0.8 nm       Double
     */
   static const FileParser::DataTag tags[];

    //----< (DE/CON)STRUCTORS >------------------------------------------------
    /*
     * Constructors are supplied without parameters as those shouldn't be
     *   needed (minus the copy constructor). Destructors and the assignment
     *   operator are also provided here.
     */
    GpXrayParser();
    GpXrayParser( string fileName );
    GpXrayParser( const GpXrayParser& copy );
    ~GpXrayParser();
    GpXrayParser& operator=(const GpXrayParser& copy );

  protected:
    //----< FILE METHODS >-----------------------------------------------------
    /*
     * Begins the actual file processing and fills the associated data file
     *   with various types of DataSeries.
     */
    void parseFile();

};

#endif
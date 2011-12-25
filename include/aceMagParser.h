
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

#ifndef CRSCORR_ACEMAG_FILEPARSER_H
#define CRSCORR_ACEMAG_FILEPARSER_H

#define ACEMAG_LINESIZE 180

class AceMagParser : public FileParser {

  public:
    /*
     * Determines which values to extract from the given data file.
     * Below, the available columns are specified along with their units.
     * Data ranges are specified and should be considered when chosing which
     * data series(es) use.
     *
     * Bx, By, Bz, Bt = nT
     * yF2, D, hmE, h'F, h', hmF2 = km
     * Missing Data -999.9
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
     *  BX           Horizontal component                Double
     *  BY           Vertical Component                  Double
     *  BZ           Other Component                     Double
     *  BT           Total Magnitude                     Double
     *  LAT          Latitude                            Double
     *  LONG         Longitude                           Double
     */
   static const FileParser::DataTag tags[];

    //----< (DE/CON)STRUCTORS >------------------------------------------------
    /*
     * Constructors are supplied without parameters as those shouldn't be
     *   needed (minus the copy constructor). Destructors and the assignment
     *   operator are also provided here.
     */
    AceMagParser();
    AceMagParser( string fileName );
    AceMagParser( const AceMagParser& copy );
    ~AceMagParser();
    AceMagParser& operator=(const AceMagParser& copy );

  protected:
    //----< FILE METHODS >-----------------------------------------------------
    /*
     * Begins the actual file processing and fills the associated data file
     *   with various types of DataSeries.
     */
    void parseFile();

};

#endif
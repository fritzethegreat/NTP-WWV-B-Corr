
/*
 * Parses files specifically from the GOES-13 satellite geomagnetic components
 *   provided by the National Space Weather Prediction Center (NSWPC).
 *   Examples of this format are available at:
 *      ftp://ftp.swpc.noaa.gov/pub/lists/geomag/20100718_Gp_mag_1m.txt
 *
 * Modified: 07/16/10
 * Notes:    Initial Creation
 */

#include <crsCorr/fileParser.h>
#include <crsCorr/dataSeries.h>

#ifndef CRSCORR_GPMAG_FILEPARSER_H
#define CRSCORR_GPMAG_FILEPARSER_H

#define GPMAG_LINESIZE 180

class GpMagParser : public FileParser {

  public:
    /*
     * Determines which values to extract from the given data file.
     * Below, the available columns are specified along with their units.
     * Data ranges are specified and should be considered when chosing which
     * data series(es) use.
     *
     * Hp, He, Hn = nT
     *
     * Label        Value                               Suggested Type
     *-------------------------------------------------------------------------
     *  YR           Year                                Int
     *  MO           Month                               Int
     *  DA           Day                                 Int
     *  HHMM         Hrs/Min                             Int
     *  DAY          Julian Calendar day                 Int
     *  SEC          Seconds of the day                  Int
     *  HP           Parallel to earth's spin axis       Double
     *  HE           Directed Earthwards from space      Double
     *  HN           Directed Eastwards orthogonal       Double
     *  TOTAL        Total Field                         Double
     */
   static const FileParser::DataTag tags[];

    //----< (DE/CON)STRUCTORS >------------------------------------------------
    /*
     * Constructors are supplied without parameters as those shouldn't be
     *   needed (minus the copy constructor). Destructors and the assignment
     *   operator are also provided here.
     */
    GpMagParser();
    GpMagParser( string fileName );
    GpMagParser( const GpMagParser& copy );
    ~GpMagParser();
    GpMagParser& operator=(const GpMagParser& copy );

  protected:
    //----< FILE METHODS >-----------------------------------------------------
    /*
     * Begins the actual file processing and fills the associated data file
     *   with various types of DataSeries.
     */
    void parseFile();

};

#endif
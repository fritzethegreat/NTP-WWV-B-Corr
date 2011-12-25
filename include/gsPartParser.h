
/*
 * Parses files specifically from the GOES-13 satellite geomagnetic components
 *   provided by the National Space Weather Prediction Center (NSWPC).
 *   Examples of this format are available at:
 *      ftp://ftp.swpc.noaa.gov/pub/lists/particle/20100718_Gp_part_5m.txt
 *
 * Modified: 07/16/10
 * Notes:    Initial Creation
 */

#include <crsCorr/fileParser.h>
#include <crsCorr/dataSeries.h>

#ifndef CRSCORR_GSPART_FILEPARSER_H
#define CRSCORR_GSPART_FILEPARSER_H

#define GSPART_LINESIZE 180

class GsPartParser : public FileParser {

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
     *  P1           Particles at >1Mev                  Double
     *  P5           Particles at >5Mev                  Double
     *  P10          Particles at >10Mev                 Double
     *  P30          Particles at >30Mev                 Double
     *  P50          Particles at >50Mev                 Double
     *  P100         Particles at >100Mev                Double
     *  E0.8         Electrons at >0.8Mev                Double
     *  E2.0         Electrons at >2.0Mev                Double
     *  E4.0         Electrons at >4.0Mev                Double
     */
   static const FileParser::DataTag tags[];

    //----< (DE/CON)STRUCTORS >------------------------------------------------
    /*
     * Constructors are supplied without parameters as those shouldn't be
     *   needed (minus the copy constructor). Destructors and the assignment
     *   operator are also provided here.
     */
    GsPartParser();
    GsPartParser( string fileName );
    GsPartParser( const GsPartParser& copy );
    ~GsPartParser();
    GsPartParser& operator=(const GsPartParser& copy );

  protected:
    //----< FILE METHODS >-----------------------------------------------------
    /*
     * Begins the actual file processing and fills the associated data file
     *   with various types of DataSeries.
     */
    void parseFile();
};

#endif
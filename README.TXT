+----------+
| Contents |
+----------+

  I.  Developer's Notes
  II. File Parser Labels

---< I. Developer's Notes >-----------------------------------------------------

+----------+
| 09/17/10 |
+----------+
  Currently, the project has been versioned to pre-release v1.3. Most likely,
an official release will be made once a master's thesis has been obtained by
the main author.

To date, the library is cabable of basic file parsing which extracts the time-
series data from a provided data file path. Start times (phase offsets) for
the data are specified within each parser, and the resolution of the series
data is also manually specified. Addition of two time series is possible as
well.  Ultimately, the library serves as a file parser capable of dumping
data files of parsed data so that a 3rd party system can perform the necessary
calculations.

However, this is unsatisfactory and this next version should be capable of
performing the calcuations resulting in the output of data files with complete
statistical analyses performed. The main objectives surround cross-correlations,
normalized cross-correlations, fast fourier transforms, normalized fast-fourier
transforms, and primitive statisitcs such as mean, standard deviation,
correlation coefficients, and the time-series data filtering.

---< II. File Parser Labels >---------------------------------------------------

+--------------+
| AceMagParser |
+--------------+
    /*
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
     *  Bx           Horizontal component                Double
     *  By           Vertical Component                  Double
     *  Bz           Other Component                     Double
     *  Bt           Total Magnitude                     Double
     *  Lat          Latitude                            Double
     *  Long         Longitude                           Double
     */

+--------------+
| AceSweParser |
+--------------+
    /*
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

+----------------+
| ClkStatsParser |
+----------------+
    /*
     * Name        Value                               Suggested Type
     *-------------------------------------------------------------------------
 1   * PID         Process ID for NTP                  (ignore)
 2   * epoch       Time from midnight                  Double
 3   * IP          127.127.36.0                        (ignore)
 4   * Ident       wwv5                                (ignore)
 5   * status      0x0000 - 0xFFFF                     (ignore)
 6   * gain        0 - 255                             Int
 7   * yepoch      0 - 5999                            Int
 8   * epomax      0 - 7000(?)                         Int
 9   * eposnr      0 - 40 dB                           Double
10   * datsig      0 - 7000(?)                         Int
11   * datsnr      0 - 40 dB                           Double
12   * sp->refid   WV##                                (ignore)
13   * reach       0x0000 - 0xFFFF                     (ignore)
14   * metric      0 - 100                             Int
15   * synmax      0 - 7000(?)                         Int
16   * synsnr      0 - 40dB                            Double
17   * min_epoch   0 - 479,999                         Int
18   * sec_epoch   0 - 5999                            Int
19   * rp->refid   WH##                                (ignore)
20   * reach       0x0000 - 0xFFFF                     (ignore)
21   * metric      0 - 100                             Int
22   * synmax      0 - 7000(?)                         Int
23   * synsnr      0 - 40dB                            Double
24   * min_epoch   0 - 479,999                         Int
25   * sec_epoch   0 - 5999                            Int
26   * delay       0 - 5999                            Int
     */


    /*
     * Hp, He, Hn = nT
     * Missing Data: -1.00e+05
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


    /*
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


    /*
     * Hp, He, Hn, Total = nT
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
     *  STATUS       Status of the data reading          Int
     *  Proton       Proton density                      Double
     *  Speed        Bulk Speed                          Double
     *  Temp         Ion Temperature                     Double
     */

    /*
     * foF2, MUF(D), foEs, foE, fMUF, foF1, fxI, fbEs = MHz
     * yF2, D, hmE, h'F, h', hmF2 = km
     * ITEC 10^16 el/m^2
     * Missing Data -1.0, -1, -1.00
     *
     * Label        Value                               Suggested Type
     *-------------------------------------------------------------------------
     *  YR           Year                                Int
     *  MO           Month                               Int
     *  DA           Day                                 Int
     *  HHMM         Hrs/Min                             Int
     *  foF2         F2 critical Frequenecy              Double
     *  hmF2         F2 Max height                       Int
     *  MUF(D)       Max Useable Freq. @ D               Double
     *  D            Distance = 3000 km                  Int
     *  h'F          min F Virt reflect height           Int
     *  yF2          Half thickness F2 layer             Int
     *  fMUF         MUF / OblFactor                     Double
     *  h'           Virtual height at MUF               Int
     *  fxI          Max Freq. F trace                   Double
     *  foE          E critical frequency                Double
     *  hmE          E maximum height                    Int
     *  foEs         Es critical frequency               Double
     *  fbEs         Blanketing frequency Es layer       Double
     *  ITEC         Total electron content              Double
     */


    /*
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


    /*
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


/*
 * Represents a single, abstract data series extracted from a data file.
 *   This is an abstract (base) class that trys to allow for as much flexibility
 *   as possible in storing data. It is intended to be inherited by a template
 *   class which will provide the actual implementation for storing data values
 *   using the provided inner, templated linked list structures.
 *
 * The time data contained within the series carries a resolution. The smallest
 *   resolution is one which implies that the data was gathered every minute.
 *   Larger resolutions means that each point persists over that many minutes.
 *
 * A start time is included within the class which means the number of minutes
 *   from 00:00 UTC. Future version might include smaller time resolutions.
 *   However, higher resolution data seems unlikely at this point.
 *
 * The series occupies three different states: Initial, Collecting, Final
 *   Initial    -- no data has been added; initial creation; adding a value
 *                 transitions to Collecting
 *
 *   Collecting -- at least one data value has been added; data is accepted;
 *                 finalizeData() transitions to Final
 *
 *   Final      -- data is finalized; no more values are accepted; end state
 *
 * Modified: 08/12/10
 * Notes:    --Reverting back to inheritance for dynamic polymorphism
 *           --Pushing template into child class
 *
 * Modified: 08/11/10
 * Notes:    --Modifying to include internal resolution and start time
 *           --DataSeries unmodifiable once finalized, and mostly after creation
 *
 *
 * Modified: 07/30/10
 * Notes:    --Broke out in to a template class
 *
 * BRANCHED: 07/30/10 -- VERSION pre-1.1
 *
 * Modified: 07/29/10
 * Notes:    --Updated debug code
 *           --Added collect() and finalize() for more rigid state machine
 *             control
 *           --Combined default and parameterized constructor
 *           --Changed getNumericalData() to a constant function
 *           --Added sample( int ) for altering data resolution
 *
 * Modified: 07/13/10
 * Notes:    --Initial creation
 */
#include <string>
#include <cstring>

#ifndef CRSCORR_ABSTRACTDATASERIES_H
#define CRSCORR_ABSTRACTDATASERIES_H

// internal class constants
#define NODE_SIZE 10
#define DEFAULT_RESOLUTION 1
#define DEFAULT_START_TIME 0

// namespace convention
using std::string;

class AbstractDataSeries {

  private:
    // local member variables
    const string label;       // descriptor of the data series
    bool collecting;          // values have been and are being added
    bool final;               // data is finalized

    static int refCount;      // reference counter for smart deletion

  protected:
    int length;               // length of the data series
    int resolution;           // time resolution of the data, minutes
    int startTime;            // start time in HHMM format of the data set
                              //   measured from 00:00 UTC
    //----<ACCESSOR METHODS>----------------------------------------------------
    /*
     * Updates the collecting state. This will only convert collecting to true
     *   if final is false to ensure state consistency.
     *
     * Uses the provided values to set the resolution and the time start.
     *
     * Param:
     *   const int res -- resolution, > 0
     *   const int start -- start time >= 0
     *
     * If parameters are incorrect, uses default values of 1 and 0.
     */
    void setCollecting( const int res = DEFAULT_RESOLUTION,
                        const int start = DEFAULT_START_TIME );

    /*
     * Updates the final state. This will only convert final to true if
     *   collecting is true. Collecting will then be set to false, and no
     *   more state updates will be possible.
     */
    void setFinal();

    /*
     * Updates the length, start time, and resolution of the series.
     *   All positive values are accepted. Only non-zero values are accepted
     *   for the length and the resolution. Error case leaves values unchanged.
     *
     * This should only be used when resampling data as after the parameters
     *   are set, the series is put in the finalized state preventing any
     *   state machine changes.
     *
     * Param: const int len -- new length
     *        const int start -- new start time
     *        const int reso -- new resolution
     *
     * Return: bool -- true on success
     */
    const bool setParams( const int len, const int start, const int reso );

    /*
     * Updates the length variable
     */
    void incrementLength();

  public:
    //----<(DE|CON)STRUCTORS>---------------------------------------------------
    /*
     * Basic constructor creates a AbstractDataSeries with an empty label or the
     * specified label.
     *
     * const char* const descriptor -- label for the data set
     * const int resolution -- resolution of the data set
     * const startTime -- beginning time interval of the series
     */
    AbstractDataSeries( const string descriptor = string( "EMPTY" ),
                        const int reso = DEFAULT_RESOLUTION,
                        const int start = DEFAULT_START_TIME );

    /*
     * Copy(clone) constructor -- produces an exact replica of the provided
     *                            data series.
     *
     * const AbstractDataSeries<DataType>& copy -- series to copy
     */
    AbstractDataSeries( const AbstractDataSeries& copy );

    /*
     * Destructor
     */
    virtual ~AbstractDataSeries();

    /*
     * Clone function that returns a complete, independent copy of the object.
     */
    virtual AbstractDataSeries* clone() const = 0;

    //----< OPERATORS >--------------------------------------------------------
    /*
     * Provides a way to visually print out a Data series. Just prints out
     *   the label, length, and state machine values (final, collecting)
     */
    friend std::ostream& operator<<(
        std::ostream& _ostr,
        const AbstractDataSeries& output )
    {
      LOG_DEBUG( 2, "( const AbstractDataSeries& output )" )

      _ostr << output.getLabel() << ": " << output.getLength() << " values. ";
      if( output.isFinal() ) {
        _ostr << "Final.";
      } else if ( output.isCollecting() ) {
        _ostr << "Collecting.";
      }

      return _ostr;
    }// end friend std::ostream& AbstractDataSeries::operator<<( ... )


    //----< DATA METHODS >-----------------------------------------------------
    /*
     * Allows for addition of values to a polymorphic version of this
     *   data series.
     */
    virtual const bool addValue( void* value,
                                 const int res = DEFAULT_RESOLUTION,
                                 const int start = DEFAULT_START_TIME ) = 0;

    /*
     * Condenses values into a useable format, namely an array.
     */
    virtual void finalizeData() = 0;

    //----<ACCESSOR METHODS>---------------------------------------------------
    /*
     * Retrieves the label of the data series
     */
    const string& getLabel() const;

    /*
     * Retrieves the length of the data series
     */
    const int getLength() const;

    /*
     * Retrieves the resolution of the data series
     */
    const int getResolution() const;

    /*
     * Retrieves the start time of the data series
     */
    const int getStartTime() const;

    /*
     * Returns whether the series is in collection mode or not.
     *
     * The "collecting" data implies that data inside of the data series may
     *   or may not be in a retrieveable state. This usually implies that a
     *   file processor is still utilizing the data series and collection is
     *   not yet complete, or the data has not been compiled into a usable
     *   format.
     */
    const bool isCollecting() const;

    /*
     * Returns whether the data is finalized or not.
     *
     * Finalized data implies that the data inside of the series is in its
     *   final form. All file processing is complete, and the data can now be
     *   used for further analysis.
     */
    const bool isFinal() const;
};

#endif

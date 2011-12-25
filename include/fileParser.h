/*
 * Defines the abstract base class of all FileParsers. These classes serve to
 *   extract DataSeries objects from a file via processing functions defined
 *   within the children classes.
 *
 * Modified: 08/11/10
 * Notes:    Restructuring to use new template class data series.
 *           Using only input stream commands for reading lines
 *           --No longer requires line length
 *
 * Modified: 7/15/10
 * Notes:    Trying "open" architecture. Should participate with DataFile class
 *             in a host/guest relationship
 *           Adding accessor methods to enable retrieval of only one line of
 *             data from the data stream. Should prevent others from getting
 *             at the data stream directly.
 *
 * Modified: 7/13/10
 * Notes:    Initial Creation
 */

#include <fstream>
#include <string>
#include <crsCorr/global.h>
#include <crsCorr/dataSeries.h>

#ifndef CRSCORR_FILEPARSER_H
#define CRSCORR_FILEPARSER_H

class FileParser {
  public:
    /*
     * DataTag
     *
     * Small structure that defines what the data series of any given file are,
     * and defines what the types of those series are. Data Type Constants are
     * defined above. IGNORE will result in a NULL data series to conserve
     * space.
     */
    typedef struct DataTag {
      const string label;
      const int type;
      const int reso;
      const int start;
    } DataTag;

/*****< PRIVATE >**************************************************************/
  private:
    /*
     * Sampled
     *
     * Small structure that maintains a linked list of the resampled data
     * series for this parser. Ensures that all references are deleted with
     * this parser.
     */
    typedef struct Resample {
      AbstractDataSeries* series;
      struct Resample* next;
    } Resample;

    //----< DATA MEMBERS >------------------------------------------------------
    const string localFileName;         // name of the opened file
    int length;                         // length of data series
    Resample* resamples;                // linked list of resampled data series
    Resample* resamplesEnd;             // last resampled data series in list

    //----< UTILITIES >---------------------------------------------------------
    /*
     * Copies a data series from the provided copy into the indicated storage
     *   location. The store variable will be deleted and reallocated to store
     *   the indicated copy series.
     *
     * Params:
     *  DataSeries<DataType>** store -- place to store copy
     *  const DataSeries<DataType>* const * const copy -- data to copy
     *  const int seriesLength -- length of the series to copy
     *
     * Returns:
     *  (via reference) Copy of the indicated series
     */
    void copyDataSeries( AbstractDataSeries*** store,
                         const AbstractDataSeries* const * const copy,
                         const int seriesLength ) const;

    /*
     * Iterates through dataTags and initializes all of the data series
     *   contained within data to teh appropriate type.
     * If data is not null when invoked, function prints err and returns.
     */
    void initDataSeries();

/*****< PROTECTED >************************************************************/
  protected:
    //----< DATA MEMBERS >------------------------------------------------------
    AbstractDataSeries** data;         // data values
    std::ifstream* dataStream;        // input stream for parsing
    const DataTag* const dataTags;      // data descriptors

    //----< PARSING METHODS >---------------------------------------------------
    /*
     * parseFile -- processes data stream into several unique DataSeries
     */
    virtual void parseFile() = 0;

    //----< UTILITIES >---------------------------------------------------------
    /*
     * Ensures that all of the data series contained within the file parser
     *   is finalized data by invoking DataSeries::finalizeData() on all
     *   DataSeries within the data array.
     */
    void finalizeSeriesData();

    /*
     * Some data files contain a header and some sort of significant artifact
     *   prior to the actual data. This utility function advances the dataStream
     *   up to the first data line.
     *
     * If the artifact is not found, the function returns false and displays an
     *   error so that the file parser can take the appropriate action.
     *
     * Param:
     *  const char* artifact -- character string containing the significant
     *                          text to locate
     * Return:
     *   true if the artifact was found; dataStream rests just at the artifact.
     *   false if the artifact was not found; dataStream at end of file.
     */
    bool findArtifact( const char* artifact );

/*****< PUBLIC >***************************************************************/
  public:
    //----< ACCESSOR METHODS >--------------------------------------------------
    /*
     * Returns the number of data series contained within the parser.
     */
    const int getLength() const;

    /*
     * Returns the array of data labels associated with each data series,
     *   ulmiately obtained by the parser from within the specified data file.
     */
    const DataTag* const getTags() const;

    //----< DATA METHODS >------------------------------------------------------
    /*
     * Provide access to the data the file parser has extraced from the data
     *   file. If that type of data is not available for the given label, the
     *   request must return a null pointer.
     *
     * Param:
     *   const DataTag* label -- specifies the label and type
     *
     * Return: the DataSeries<Type> or NULL
     */
    template<typename DataType>
    const DataSeries<DataType>* getSeries(
        const FileParser::DataTag* tag )
    {
      LOG_DEBUG( 8, "( { " << tag->label << ", " << tag->type << ", " \
                    << tag->reso << ", " << tag->start << " } )" )

      if( data == NULL ) {
        LOG_ERR( "Data series are null" )
        return NULL;
      }

      // iterate through datasets looking for specified label
      bool found = false;
      int index = 0;
      while( !found && index < length ) {
        if( data[index] != NULL ) {
          // non-null data series
          //LOG_DEBUG( 6, ": checking " << data[index]->getLabel() )
          if( !tag->label.compare( data[index]->getLabel() )) {
            found = true;
          } else {
            index++;
          }
        } else if( dataTags[index].type != DATATYPE_IGNORE ) {
          LOG_ERR( "Null data series with non-null data type. Expected " \
                   << dataTags[index].label )
          index++;
        } else {
          index++;
        }
      }

      // return NULL if dataset not found.
      if( !found ) {
        LOG_DEBUG( 7, ": Failed to find dataset: " << tag->label << "." )
        return NULL;
      }

      // return NULL if dataType does not match
      if( tag->type != dataTags[index].type ) {
        LOG_ERR( "Data types do not match. Found " << dataTags[index].type \
                 << ". Expected " << tag->type )
        return NULL;
      }

      // return Data Series
      if( tag->reso != RESOLUTION_IGNORE
          && tag->reso > 1
          && tag->start != START_TIME_IGNORE
          && tag->start >= 0 )
      {
        LOG_DEBUG( 7, ": Resampling series with " << tag->reso << ", " \
                      << tag->start )

        DataSeries<DataType>* toReturn = new DataSeries<DataType>(
                                         *data[index],
                                         data[index]->getLabel(),
                                         tag->reso,
                                         tag->start );
        if( resamples == NULL ) {
          resamples = new Resample;
          resamples->series = toReturn;
          resamples->next = NULL;
          resamplesEnd = resamples;
        } else {
          resamplesEnd->next = new Resample;
          resamplesEnd = resamplesEnd->next;
          resamplesEnd->series = toReturn;
          resamplesEnd->next = NULL;
        }
        return toReturn;
      } else {
        return dynamic_cast<const DataSeries<DataType>*>( data[index] );
      }
    }// end DataSeries<DataType>* getSeries( const DataTag* const )


    //----< (DE)(CON)STRUCTORS >------------------------------------------------
    /*
     * Generic constructor is supplied but highly discouraged as the resulting
     *   FileParser object will be useless..
     * Parameterized constructor is recommended with a dataStream.
     * Copy constructor supplied.
     */
    FileParser();
    FileParser( string fileName,
                const DataTag* const dataTags );
    FileParser( const FileParser& copy );
    virtual ~FileParser();
};

#endif
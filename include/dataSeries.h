
/*
 * Provides the data storage implementation of the abstract base class,
 *   abstractDataSeries.
 *
 * Modified: 08/19/10
 * Notes:    --Adjusted cases on the add value routine to promote most used
 *             cases to the top
 *
 * Modified: 08/12/10
 * Notes:    --Initial creation
 */
#include <string>
#include <cstring>

#include <crsCorr/global.h>
#include <crsCorr/abstractDataSeries.h>

#ifndef CRSCORR_DATASERIES_H
#define CRSCORR_DATASERIES_H

// namespace convention
using std::string;

template<typename DataType>
class DataSeries : public AbstractDataSeries {

  private:
    /*
     * Struct Node --  private data structure for temporarily holding
     *                  collected data during file parsing.
     *
     * node* nextNode               -- the next node in the list
     * int count                    -- number of values stored in this node
     * DataType dataCell[NODE_SIZE] -- storage for values
     */
    typedef struct DataNode {
      DataNode* nextNode;
      int count;
      DataType dataCell[NODE_SIZE];
    } Node;

    DataType* data;             // array containing all of the final data
                                //   values
    Node* tempDataList;         // linked list of temporary data values
    Node* curDataListNode;      // last node in the linked list of values
    int dataNodeCount;          // number of temporary data nodes

    //----< DATA METHODS >----------------------------------------------------
    /*
     * Provides a means of copying temporary data from one linked list to
     *   another. Entire list is copied and unchanged. Null is stored in the
     *   event a null list is provided.
     *
     * Param: const Node* const list -- list to copy
     */
    void copyTempData( const Node* const list )
    {
      LOG_DEBUG( 5, "( " << list << " )" )

      // free up data if present
      if( tempDataList != NULL ) {
        freeTempData();
      }

      dataNodeCount = 0;

      if( list != NULL ) {
        // setup initial conditions
        tempDataList = new Node;
        curDataListNode = tempDataList;
        dataNodeCount++;
        const Node* copyDataNode = list;
        for( int i = 0; i < copyDataNode->count; i++ ) {
          curDataListNode->dataCell[i] = copyDataNode->dataCell[i];
        }
        curDataListNode->count = copyDataNode->count;
        copyDataNode = copyDataNode->nextNode;

        // loop through data nodes
        while( copyDataNode != NULL ) {
          curDataListNode->nextNode = new Node;
          curDataListNode = curDataListNode->nextNode;
          dataNodeCount++;

          // copy node data
          for( int i = 0; i < copyDataNode->count; i++ ) {
            curDataListNode->dataCell[i] = copyDataNode->dataCell[i];
          }
          curDataListNode->count = copyDataNode->count;

          // iterate copyDataNode
          copyDataNode = copyDataNode->nextNode;
        }

        curDataListNode->nextNode = NULL;
      }// finished complex copy case
    }// end void copyTempData( ... )

    /*
     * Iterates through the temporary data storage and frees data as needed.
     *   Terminates by setting both tempDataList and curDataListNode to NULL.
     */
    void freeTempData() {
      LOG_DEBUG( 5, "()" )

      if( tempDataList != NULL ) {
        curDataListNode = tempDataList->nextNode;

        while( curDataListNode != NULL ) {
          delete tempDataList;
          dataNodeCount--;
          tempDataList = curDataListNode;
          curDataListNode = curDataListNode->nextNode;
        }

        if( tempDataList != NULL ) {
          delete tempDataList;
          dataNodeCount--;
          tempDataList = NULL;
        }
      }

      LOG_DEBUG( 5, "(): Data Node Count = " << dataNodeCount )
    }// end void freeTempData()


    /*
     * Actually performs the resampling on the provided data set with the
     *   indicated resolution. Data is sampled via the new resolution and
     *   stored in this DataSeries.
     *
     * Param:
     *  const DataSeries<DataType>& fullData
     *                                 -- full data series to down sample
     *  const int newRes               -- new data set resolution
     *  const int newStart             -- new start time
     *
     */
    void resample( const DataSeries& series,
                   const int newRes,
                   const int newStart )
    {
      LOG_DEBUG( 5, "( " << series << ", " \
                    << newRes << " )" )

      // attempt to resample based off case
      if( series.getResolution() == 1 ) {
        LOG_DEBUG( 3, ": Resampling from one minute data." )
        // minute data (easiest case)
        if( setParams( series.getLength() / newRes,
                       newStart,
                       newRes ))
        {
          int len = getLength();
          if( data != NULL ) {
            delete[] data;
            data = NULL;
          }
          data = new DataType[ len ];
          int start = getStartTime();
          for( int i = 0; i < len; i++ ) {
            data[i] = series.data[i * newRes + start ];
          }// copy values
        }// good parameters
      } else if ( newRes > series.getResolution()
                  && newRes % series.getResolution() == 0 )
      {
        LOG_DEBUG( 3, ": Resampling from compatible data." )
        // new resolution is larger and multiple of old resolution
        int step = newRes / series.getResolution();
        if( setParams( series.getLength() / step,
                       newStart,
                       newRes ))
        {
          if( data != NULL ) {
            delete[] data;
            data = NULL;
          }
          int len = getLength();
          int start = getStartTime();
          data = new DataType[ len ];
          for( int i = 0; i < len; i++ ) {
            data[i] = series.data[i * step + start ];
          }// copy values
        }// good values
      } else {
        // DEFAULT -- not resampling at all, just copy data
        if( newRes == series.getResolution() ) {
          LOG_DEBUG( 3, ": Resolutions are the same." )
        } else {
          LOG_DEBUG( 3, ": Cannot resample. Copying data." );
        }
        setParams( series.getLength(),
                   series.getStartTime(),
                   series.getResolution() );
        if( data != NULL ) {
          delete[] data;
          data = NULL;
        }
        int len = getLength();
        data = new DataType[ len ];
        for( int i = 0; i < len ; i++ ) {
          data[i] = series.data[i];
        }// copy values
      }// resampling
    }// end void resample( const DataType* const, const int )

  public:
    //----< (DE|CON)STRUCTORS>-------------------------------------------------
    /*
     * Basic constructor creates a DataSeries with an empty data set.
     *
     * const string* descriptor -- label for the data set
     */
    DataSeries<DataType>( const string descriptor = "EMPTY",
                          const int reso = DEFAULT_RESOLUTION,
                          const int start = DEFAULT_START_TIME )
      : AbstractDataSeries( descriptor, reso, start )
    {
      LOG_DEBUG( 5, "( " << descriptor << ", " << reso << ", " \
                    << start << " )" )

      data = NULL;
      tempDataList = NULL;
      curDataListNode = NULL;
      dataNodeCount = 0;
    }// end DataSeries( const string* )

    /*
     * Copy(clone) constructor -- designed to copy an exact replica of the
     *                            provided Abstract Data Series.
     */
    DataSeries( const AbstractDataSeries& copy )
      : AbstractDataSeries( copy )
    {
      LOG_DEBUG( 5, "( const AbstractDataSeries &copy )" )

      // determine type
      const DataSeries<DataType>* copyClone =
          dynamic_cast<const DataSeries<DataType>*>( &copy );
      if( copyClone == NULL ) {
        LOG_DEBUG( 3, ": Type mismatch." );

        data = NULL;
        tempDataList = NULL;
        curDataListNode = NULL;
        dataNodeCount = 0;
        return;
      }

      // determine current state
      if( isFinal() ) {
        // final state
        LOG_DEBUG( 3, ": Final state. " )
        data = new DataType[ getLength() ];
        for( int i = 0; i < getLength(); i++ ) {
          data[i] = copyClone->data[i];
        }
        tempDataList = NULL;
        curDataListNode = NULL;
        dataNodeCount = 0;
      } else if( isCollecting() ) {
        // collecting state
        LOG_DEBUG( 3, ": Collecting state." )
        data = NULL;
        copyTempData( copyClone->tempDataList );
      } else {
        // idle state
        LOG_DEBUG( 3, ": Idle state." )
        data = NULL;
        tempDataList = NULL;
        curDataListNode = NULL;
        dataNodeCount = 0;
      }
    }// end DataSeries( const AbstractDataSeries& )

    /*
     * Copy(clone) constructor -- designed to copy an exact replica of the
     *                            provided DataSeries.
     */
    DataSeries( const DataSeries& copy )
      : AbstractDataSeries( copy )
    {
      LOG_DEBUG( 5, "( const DataSeries& copy ) " )

      tempDataList = NULL;
      curDataListNode = NULL;
      dataNodeCount = 0;
      data = NULL;

      // determine current state
      if( isFinal() ) {
        // final state
        LOG_DEBUG( 3, ": Final state. " )
        data = new DataType[ getLength() ];
        for( int i = 0; i < getLength(); i++ ) {
          data[i] = copy.data[i];
        }
      } else if( isCollecting() ) {
        // collecting state
        LOG_DEBUG( 3, ": Collecting state." )
        copyTempData( copy.tempDataList );
      } else {
        // idle state
        LOG_DEBUG( 3, ": Idle state." )
      }
    }// end DataSeries( const DataSeries& )

    /*
     * Copy constructor -- designed to allow creation of simliar DataSeries with
     *                     different resolutions. Non-collecting and collecting
     *                     DataSeries will be copied directly. Finalized data
     *                     can be manipulated by altering the resolution,
     *                     which will result in a sampled data series.
     *
     *  const DataSeries<DataType>& copy -- old DataSeries
     *  const int newRes -- new data resolution relative to old DataSeries
     *                      resolution
     *  const string* newLabel -- label for the new DataSeries
     *  const int startTime -- start time for new DataSeries
     */
    DataSeries( const DataSeries& copy,
                const string newLabel,
                const int newRes = DEFAULT_RESOLUTION,
                const int startTime = DEFAULT_START_TIME )
      : AbstractDataSeries( newLabel, newRes, startTime )
    {
      LOG_DEBUG( 5, "( const DataSeries& copy, " << newLabel << ", " \
                    << newRes << " )" )

      // initialize to default values for sure
      data = NULL;
      tempDataList = NULL;
      curDataListNode = NULL;
      dataNodeCount = 0;

      // copy the data
      if( newRes <= 0 && startTime < 0 ) {
        resample( copy, DEFAULT_RESOLUTION, DEFAULT_START_TIME );
      } else if( newRes <= 0 ) {
        resample( copy, DEFAULT_RESOLUTION, startTime );
      } else if( startTime < 0 ) {
        resample( copy, newRes, DEFAULT_START_TIME );
      } else {
        resample( copy, newRes, startTime );
      }
    }// end DataSeries( const DataSeries&, const string*, const int, ... )

    /*
     * Destructor
     */
    ~DataSeries() {
      LOG_DEBUG( 5, "()" )

      if( !isCollecting() && !isFinal() ) {
        // never activated, nothing is occuring
      } else if( isCollecting() && !isFinal() ) {
        // collecting data, free temp data
        freeTempData();
      } else if( isFinal() ) {
        // data finalized, kill it
        delete[] data;
        data = NULL;
      }
      LOG_DEBUG( 5, ": EXIT" )
    }// end ~DataSeries()

    /*
     * Clone function that returns a complete, independent copy of the object.
     */
    DataSeries<DataType>* clone() const {
      LOG_DEBUG( 5, "()" )
      DataSeries<DataType>* toReturn = new DataSeries( *this,
                                                       this->getLabel(),
                                                       this->getResolution(),
                                                       this->getStartTime() );
      return toReturn;
    }// end AbstractDataSeries* clone()

    //----< OPERATORS >--------------------------------------------------------
    /*
     * Deep copies all of the information contained within the provided
     *   DataSeries.
     */
    DataSeries<DataType>* operator=( const DataSeries<DataType>& copy ) const {
      LOG_DEBUG( 5, "( const AbstractDataSeries& copy )" )

      return copy.clone();
    }

    /*
     * Concatenates the provided data series onto this data series if the
     *   start times and the resolutions match. Base data series is left
     *   unchanged.
     *
     * Params:
     *   const DataSeries& toAdd -- series to append
     *
     * Return:
     *   Reference to new DataSeries with combined values or a reference to
     *     an empty data set if it was not possible to combine the values
     */
    DataSeries<DataType> operator+( const DataSeries<DataType>& toAdd ) const {
      LOG_DEBUG( 5, "( const AbstractDataSeries& )" )

      // ensure both are finalized
      if( !this->isFinal() || !toAdd.isFinal() ) {
        return *this;
      }

      // ensure similar resolution and start times, then append data
      if( toAdd.getResolution() == this->getResolution()
          && toAdd.getStartTime() == this->getStartTime() )
      {
        LOG_DEBUG( 3, ": Adding values." )
        DataSeries<DataType> sum( this->getLabel() );
        sum.setParams( this->getLength() + toAdd.getLength(),
                        this->getStartTime(),
                        this->getResolution() );
        sum.data = new DataType[ sum.getLength() ];
        int lenThis = this->getLength();
        for( int i = 0; i < lenThis; i++ ) {
          sum.data[i] = this->data[i];
        }
        int lenAdd = toAdd.getLength();
        for( int i = 0; i < lenAdd; i++ ) {
          sum.data[i + lenThis] = toAdd.data[i];
        }

        LOG_DEBUG( 3, ":Returning value." );
        return sum;
      }

      LOG_DEBUG( 3, ": Improper resolutions." )
      return *this;

    }// end AbstractDataSeries& operator+( const AbstractDataSeries& ) const


    /*
     * Concatenates the provided data series onto this data series if the
     *   start times and the resolutions match. Base data series is left
     *   unchanged.
     *
     * Params:
     *   const DataSeries& toAdd -- series to append
     *
     * Return:
     *   Reference to new DataSeries with combined values or a reference to
     *     an empty data set if it was not possible to combine the values
     */
    void operator+=( const DataSeries<DataType>& toAdd ) {
      LOG_DEBUG( 5, "( const AbstractDataSeries& )" )

      // ensure both are finalized
      if( !this->isFinal() || !toAdd.isFinal() ) {
        return;
      }

      // ensure similar resolution and start times, then append data
      if( toAdd.getResolution() == this->getResolution()
          && toAdd.getStartTime() == this->getStartTime() )
      {
        LOG_DEBUG( 3, ": Adding values." )

        DataType* tempData = this->data;
        int lenThis = this->getLength();
        int lenAdd = toAdd.getLength();
        int newLength = lenThis + lenAdd;
        this->data = new DataType[ newLength ];
        for( int i = 0; i < lenThis; i++ ) {
          this->data[i] = tempData[i];
        }
        for( int i = 0; i < lenAdd; i++ ) {
          this->data[i + lenThis] = toAdd.data[i];
        }
        this->length = newLength;

        delete[] tempData;
        tempData = NULL;
        LOG_DEBUG( 3, ":Returning value." );
        return;
      }

      LOG_DEBUG( 3, ": Improper resolutions." )
      return;

    }// end AbstractDataSeries& operator+( const AbstractDataSeries& ) const


    /*
     * Provides a way to visually print out a Data series. Just prints out
     *   the label, length, and state machine values (final, collecting)
     */
    friend std::ostream& operator<<( std::ostream& _ostr,
                                     const DataSeries& output )
    {
      LOG_DEBUG( 5, "( const DataSeries& output )" )

      _ostr << output.getLabel() << ": " << output.getLength() << " values. ";
      if( output.isFinal() ) {
        _ostr << "Final.";
      } else if ( output.isCollecting() ) {
        _ostr << "Collecting.";
      }

      return _ostr;
    }// end friend std::ostream& operator<<( std::ostream&, const DataSeries& )

    //----< DATA METHODS >------------------------------------------------------
    /*
     * Adds a data value to the end of this data series. Cannot accept values
     *   once the data series has been finalized.
     *
     * Follows one of three cases:
     *  1) Creates the first node and adds the first value
     *  2) Adds a value to the current partially filled node
     *  3) Adds a new node, inserts the value, and updates the count
     *
     * Param:
     *   const void* value -- value to add
     *   const int resolution -- data resolution, used only for first value
     *   const int startTime -- data start time, used only for first value
     *
     * Return:
     *   bool -- whether the addition was successful or not
     */
    const bool addValue( void* value,
                         const int reso = DEFAULT_RESOLUTION,
                         const int start = DEFAULT_START_TIME )
    {
      //LOG_DEBUG( 3, "( " << value << " )" )

      if( isFinal() ) {
        // data set is closed
        std::cerr << __FUNCTION__ << ": Attempting to add value to finalized "
                  << "data series." << std::endl;
        return false;
      } else if( !isCollecting() ) {
        // data set is open, only modify resolution if it hasn't been set
        if( getResolution() == DEFAULT_RESOLUTION
            && getStartTime() == DEFAULT_START_TIME )
        {
          setCollecting( reso, start );
        } else {
          setCollecting();
        }
      }

      // check bogus value
      if( value == NULL ) {
        std::cerr << __FUNCTION__ << ": Attempting to add NULL value."
                  << std::endl;
        return false;
      }

      // Perform the cast and hope that things do not blow up. Unfortunately,
      //   any checking would require RTTI or some more complicated
      //   mechanisms which we are not particularly interested in. So long
      //   as whomever uses the library is wise...this shouldn't be too much
      //   of an issue.
      DataType* valuePtr = (DataType*)value;

      // insert the value into the next open spot in the linked list
      if( curDataListNode != NULL && curDataListNode->count < NODE_SIZE ) {
        // one more data value in a node
        curDataListNode->dataCell[ curDataListNode->count ] = *valuePtr;
        curDataListNode->count += 1;
      } else if( curDataListNode != NULL
                 && curDataListNode->count == NODE_SIZE )
      {
        // create new data node
        curDataListNode->nextNode = new Node;
        curDataListNode = curDataListNode->nextNode;
        curDataListNode->nextNode = NULL;
        curDataListNode->dataCell[ 0 ] = *valuePtr;
        curDataListNode->count = 1;
        dataNodeCount += 1;
      } else {
        // first data
        tempDataList = new Node;
        curDataListNode = tempDataList;
        curDataListNode->nextNode = NULL;
        curDataListNode->dataCell[ 0 ] = *valuePtr;
        curDataListNode->count = 1;
        dataNodeCount = 1;
      }

      // update the length of the data series
      incrementLength();

      return true;
    }// end const bool addValue( const void* value )

    /*
     * Takes all available data in the temporary linked list and converts into
     *   an array. State is updated to final in the process.
     */
    void finalizeData()
    {
      LOG_DEBUG( 5, "()" )

      if( !isCollecting() ) {
        // data set was not collecting...?
        LOG_ERR( "Request to finalize data series ('" << getLabel() << \
                 "') with no data." )
        return;
      }

      if( isFinal() ) {
        // data set was already finalized
        LOG_ERR( "Data set was already finalized." )
        return;
      }

      // update state machine
      setFinal();

      // iterate and condense values
      if( data != NULL ) {
        delete[] data;
        data = NULL;
      }
      data = new DataType[ getLength() ];
      curDataListNode = tempDataList;
      data[0] = curDataListNode->dataCell[0];
      for( int i = 1 ; i < getLength(); i++ ) {
        if( i % NODE_SIZE == 0 ) {
          curDataListNode = curDataListNode->nextNode;
        }
        data[i] = curDataListNode->dataCell[ i % NODE_SIZE ];
      }

      // release old data storage
      freeTempData();
    }// end void finalizeData()

    //----<ACCESSOR METHODS>---------------------------------------------------
    /*
     * Allows access to the internal data array if the state is final.
     */
    const DataType* const getData() const {
      LOG_DEBUG( 5, "()" )

      if( isFinal() ) {
        return data;
      }
      return NULL;
    }// end const DataType* const getData()
};

#endif

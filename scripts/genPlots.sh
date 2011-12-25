#!/bin/bash
#
# Used to generate plots for data files contained within the DATA_DIR
#   directory. Plots are stored in the GRAPH_DIR in a heirarchy created by
#   tokens within the file name. This allows for browsing by satellite, station,
#   and frequency.
#
# Modified: 08/11/11
# Notes:    --Reverting to full-size graph, since other size is too small
#
# Modified: 08/10/10
# Notes:    --Updated for titles, labels, and half-sized graph
#
# Modified: 08/09/10
# Notes:    --Updated to secure copy last three days of files over to
#             a remote location for display (currently mick.eng.uiowa.edu)
#           --Updated to load from a specified directory
#           --Outputs to file heirarchy based off file tokens
#           --Some functional parts for easier update and programming
#
# Modified: 08/01/10
# Notes:    --Initial creation
#

# Directory for Result Files
DATA_DIR="/mnt/storage/results"

# Directory for storing graphs
GRAPH_DIR="/mnt/storage/processed"

# Remote address and directory
USER_NAME="fritze"
REMOTE_HOST="mick.eng.uiowa.edu"
REMOTE_PATH="/var/www/propstats/"

# Generates the output path for the processed data
#   $1 -- Full current data file path and name which contains the tokens for
#         the station, frequency, satellite, and variable
#
#  /mnt/storage/processed/20100809/hsynmax10_Speed__clockstats.txt__ace_swepam_1m.txt
#
function genOutputPath() {
  # CURFILE contains the currently processing file in the results directory
  CURFILE="$1"

  # Construct the processed file path from the current file name
  # which is used to store the processed results (graphs)

  # Extract the necessary tokens form the file name to categorize the graph
  ## Save and duplicate the token with the station and frequency information
  FILE_TOKENS=`echo $CURFILE | cut -d/ -f6`
  STATION=`echo $FILE_TOKENS | cut -d_ -f1`
  FREQ=$STATION

  ## Extract the station (wwv or wwvh) via the first letter of the token (v|h)
  STATION=${STATION:0:1}
  if [ $STATION == "v" ]; then
    # WWV signal, extract frequency
  	FREQ=${FREQ#"vsynmax"}
	  STATION="wwv"
  else
    # WWVH signal, extract frequency
	  FREQ=${FREQ#"hsynmax"}
	  STATION="wwvh"
	fi

  ## Extract the variable and satellite tokens for creating directories
	VARIABLE=`echo $FILE_TOKENS | cut -d_ -f2`
  SATELLITE=`echo $FILE_TOKENS | cut -d_ -f6`

  # Create the graph directories if they haven't already been made
  ## Satellite directory
  if [ ! -e ${GRAPH_DIR}/${SATELLITE}_${VARIABLE} ]; then
    mkdir ${GRAPH_DIR}/${SATELLITE}_${VARIABLE}
  fi

  ## Frequency directory
  if [ ! -e ${GRAPH_DIR}/${SATELLITE}_${VARIABLE}/$FREQ ]; then
    mkdir ${GRAPH_DIR}/${SATELLITE}_${VARIABLE}/$FREQ
  fi

  ## Compose final directory path
  DIR=${GRAPH_DIR}/${SATELLITE}_${VARIABLE}/${FREQ}

  ## Construct file name
  DATE=`date +%Y%m%d`
  NAME="${DATE}_${SATELLITE}${VARIABLE}_${FREQ}.png"
  echo $DIR/$NAME
}

#
# genPrevDay -- computes the previous day's date
#
function genPrevDay() {
  # Formulate month day and year based off parameter or current date
  if [ $1 ]; then
    YEAR=${1:0:4}
    MONTH=${1:4:2}
    DAY=${1:6:2}
  else
    YEAR=`date +%Y`
    MONTH=`date +%m`
    DAY=`date +%d`
  fi

  # First month and first day
  if [ $MONTH -eq 01 ] && [$DAY -eq 1 ]; then
    YEAR=$(($YEAR - 1 ));
    MONTH=12
    DAY=31

  # Single digit month case and first day
  elif [ $MONTH -lt 10 ] && [ $DAY -eq 1 ]; then
    MONTH=${MONTH:1};
    MONTH=$(($MONTH - 1 ));
    MONTH="0${MONTH}"
    DAY=`cal $MONTH $YEAR | egrep -v '[A-za-z]' | wc -w`

  # Tenth month case and first day
  elif [ $MONTH -eq 10 ] && [ $DAY -eq 1 ]; then
    MONTH="09"
    DAY=`cal $MONTH $YEAR | egrep -v '[A-za-z]' | wc -w`

  # Double digit month case and first day
  elif [ $DAY -eq 1 ]; then
    MONTH=$(($MONTH - 1 ));
    DAY=`cal $MONTH $YEAR | egrep -v '[A-za-z]' | wc -w`

  # 10th Day case
  elif [ $DAY -eq 10 ]; then
    DAY="09"

  # Single digit day case
  elif [ $DAY -lt 10 ]; then
    DAY=${DAY:1}
    DAY=$(($DAY - 1 ));
    DAY="0${DAY}"

  # Generic day case
  else
    DAY=$(($DAY - 1 ));
  fi

  DATE="$YEAR$MONTH$DAY"
  echo $DATE
}

function main() {

  DATE=`date +%Y%m%d`
  FILES=`ls ${DATA_DIR}/${DATE}/vsynmax*.txt`

  for CURFILE in $FILES
  do
    # Generate the output path
    OUTPUT_FULL=`genOutputPath $CURFILE`

    # Generate the output name
    OUTPUT_NAME=${OUTPUT_FULL##/*/}

    # Generate the graph commands using the current file location
    VARIABLE=`echo $OUTPUT_NAME | cut -d_ -f2`
    FREQ=`echo $OUTPUT_NAME | cut -d_ -f3`
    FREQ=`echo $FREQ | cut -d. -f1`
    WWVHFILE=${CURFILE/"vsynmax"/"hsynmax"}

    echo "set terminal png" > data.plt
    echo "set output \"${OUTPUT_FULL}\"" >> data.plt
    echo "set title \"$VARIABLE With ${FREQ}Mhz Data - ${DATE}\"" >> data.plt
    echo "set xlabel \"Delay ( 5 minute units )\"" >> data.plt
    echo "plot \"$CURFILE\" using 5:4 title 'WWV Amplitude', \"${WWVHFILE}\" using 5:4 title 'WWVH Amplitude'" >> data.plt

    # Generate graph
    gnuplot < data.plt

    # Copy the last three days' graphs over to the remote location
    YESTERDAY=`genPrevDay`
    YESTWODAY=`genPrevDay $YESTERDAY`

    YESTERFILE=${OUTPUT_FULL/$DATE/$YESTERDAY}
    YESTERNAME=${OUTPUT_NAME/$DATE/"yesterday"}
    YESTWOFILE=${OUTPUT_FULL/$DATE/$YESTWODAY}
    YESTWONAME=${OUTPUT_NAME/$DATE/"yestwoday"}
    OUTPUT_NAME=${OUTPUT_NAME/$DATE/"today"}

    scp $OUTPUT_FULL ${USER_NAME}@${REMOTE_HOST}:${REMOTE_PATH}/$OUTPUT_NAME
    scp $YESTERFILE ${USER_NAME}@${REMOTE_HOST}:${REMOTE_PATH}/$YESTERNAME
    scp $YESTWOFILE ${USER_NAME}@${REMOTE_HOST}:${REMOTE_PATH}/$YESTWONAME
  done

  rm data.plt
}

# Call pseudo-main function
main

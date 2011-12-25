#!/bin/bash
#
# DESCRIPTION
# -----------
# Provided for copying data files from various web servers so that cross
# correlations can be performed on the data sets.
#
# Will copy the previous day's files so that automated calculations can take
# place.
#
# Copies and appends some data files that are not maintained throughout the
# entire day.
#

# Local information
LOCAL_LOCATION="/mnt/storage/data"
REMOTE_LOCATION="/mnt/storage/ntpstats"

# Global variable for using the date
DATE="now"

#
# genPrevDay -- computes the previous day's date
#
function genPrevDay() {
  # Forumlate the date for the current day
  YEAR=`date --date="$1 days ago" +%Y`
  MONTH=`date --date="$1 days ago" +%m`
  DAY=`date --date="$1 days ago" +%d`

  DATE="$YEAR$MONTH$DAY"
}

day=180

while [ $day -gt 0 ]
do
  genPrevDay $day

  if [ -e ${LOCAL_LOCATION}/clockstats.${DATE} ]; then
    echo "$DATE -- Done"
  else
    cp ${REMOTE_LOCATION}/clockstats.${DATE} ${LOCAL_LOCATION}/${DATE}_clockstats.txt
    echo "$DATE -- Copied"
  fi
  day=$(($day - 1 ))
done
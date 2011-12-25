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

# Global variable for using the date
DATE="now"

#
# checkClockStats -- ensures that the clockstats file is good quality data.
#   All wwv5 lines must be present for the 24 hour period and wwv1, wwv2,
#   wwv3, wwv
#
#function checkClockStats() {
#  # Check for the correct number
#}

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

while [ $day -gt 0 ];
do
  genPrevDay $day

  if [ -e ${LOCAL_LOCATION}/${DATE}_ace_mag_1m.txt ] &&
     [ -e ${LOCAL_LOCATION}/${DATE}_Gp_mag_1m.txt ] &&
     [ -e ${LOCAL_LOCATION}/${DATE}_Gs_mag_1m.txt ] &&
     [ -e ${LOCAL_LOCATION}/${DATE}_Gp_xr_1m.txt ] &&
     [ -e ${LOCAL_LOCATION}/${DATE}_Gp_part_5m.txt ] &&
     [ -e ${LOCAL_LOCATION}/${DATE}_Gs_part_5m.txt ] &&
     [ -e ${LOCAL_LOCATION}/${DATE}_clockstats.txt ]; then
      echo "$DATE -- COMPLETE BUT NOT NECESSARILY QUALITY -- $day";
#     checkClockStats
  else
    echo "$DATE -- PARTIAL -- $day"
    if [ ! -e ${LOCAL_LOCATION}/${DATE}_ace_mag_1m.txt ]; then
      echo "  MISSING ACE MAG"
    fi

    if [ ! -e ${LOCAL_LOCATION}/${DATE}_Gp_mag_1m.txt ]; then
      echo "  MISSING GP MAG"
    fi

    if [ ! -e ${LOCAL_LOCATION}/${DATE}_Gs_mag_1m.txt ]; then
      echo "  MISSING GS MAG"
    fi

    if [ ! -e ${LOCAL_LOCATION}/${DATE}_Gp_xr_1m.txt ]; then
      echo "  MISSING GP XRAY"
    fi

    if [ ! -e ${LOCAL_LOCATION}/${DATE}_Gp_part_5m.txt ]; then
      echo "  MISSING GP PART"
    fi

    if [ ! -e ${LOCAL_LOCATION}/${DATE}_Gs_part_5m.txt ]; then
      echo "  MISSING GS PART"
    fi

    if [ ! -e ${LOCAL_LOCATION}/${DATE}_clockstats.txt ]; then
      echo "  MISSING CLOCKSTATS"
    fi
  fi

  day=$(( $day - 1 ))
done
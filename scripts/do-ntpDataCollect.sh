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
# genPrevDay -- computes the previous day's date
#
function genPrevDay() {
  # Forumlate the date for the current day
  YEAR=`date +%Y`
  MONTH=`date +%m`
  DAY=`date +%d`

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
}

genPrevDay

if [ -e ${LOCAL_LOCATION}/${DATE}_ace_mag_1m.txt ]; then
  echo >> /dev/null;
else
  wget http://www.swpc.noaa.gov/ftpdir/lists/ace/${DATE}_ace_mag_1m.txt --output-document=/mnt/storage/data/${DATE}_ace_mag_1m.txt
  wget http://www.swpc.noaa.gov/ftpdir/lists/ace/${DATE}_ace_swepam_1m.txt --output-document=/mnt/storage/data/${DATE}_ace_swepam_1m.txt
  wget ftp://ftp.swpc.noaa.gov/pub/lists/geomag/${DATE}_Gp_mag_1m.txt --output-document=/mnt/storage/data/${DATE}_Gp_mag_1m.txt
  wget ftp://ftp.swpc.noaa.gov/pub/lists/geomag/${DATE}_Gs_mag_1m.txt --output-document=/mnt/storage/data/${DATE}_Gs_mag_1m.txt
  wget ftp://ftp.swpc.noaa.gov/pub/lists/xray/${DATE}_Gp_xr_1m.txt --output-document=/mnt/storage/data/${DATE}_Gp_xr_1m.txt
  wget ftp://ftp.swpc.noaa.gov/pub/lists/particle/${DATE}_Gp_part_5m.txt --output-document=/mnt/storage/data/${DATE}_Gp_part_5m.txt
  wget ftp://ftp.swpc.noaa.gov/pub/lists/particle/${DATE}_Gs_part_5m.txt --output-document=/mnt/storage/data/${DATE}_Gs_part_5m.txt
fi

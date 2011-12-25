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
  YEAR=`date --date="$1 days ago" +%Y`
  MONTH=`date --date="$1 days ago" +%m`
  DAY=`date --date="$1 days ago" +%d`

  DATE="$YEAR$MONTH$DAY"
}

day=30

while [ $day -gt 0 ]
do
  genPrevDay $day
  echo $DATE

  ## ACE MAG ##
  if [ -e ${LOCAL_LOCATION}/${DATE}_ace_mag_1m.txt ]; then
    echo >> /dev/null;
  else
    wget http://www.swpc.noaa.gov/ftpdir/lists/ace/${DATE}_ace_mag_1m.txt --output-document=/mnt/storage/data/${DATE}_ace_mag_1m.txt
  fi

  ## ACE SWEPAM ##
  if [ -e ${LOCAL_LOCATION}/${DATE}_ace_swepam_1m.txt ]; then
    echo >> /dev/null;
  else
    wget http://www.swpc.noaa.gov/ftpdir/lists/ace/${DATE}_ace_swepam_1m.txt --output-document=/mnt/storage/data/${DATE}_ace_swepam_1m.txt
  fi

  ## Gp MAG ##
  if [ -e ${LOCAL_LOCATION}/${DATE}_Gp_mag_1m.txt ]; then
    echo >> /dev/null;
  else
    wget ftp://ftp.swpc.noaa.gov/pub/lists/geomag/${DATE}_Gp_mag_1m.txt --output-document=/mnt/storage/data/${DATE}_Gp_mag_1m.txt
  fi

  ## Gs MAG ##
  if [ -e ${LOCAL_LOCATION}/${DATE}_Gs_mag_1m.txt ]; then
    echo >> /dev/null;
  else
    wget ftp://ftp.swpc.noaa.gov/pub/lists/geomag/${DATE}_Gs_mag_1m.txt --output-document=/mnt/storage/data/${DATE}_Gs_mag_1m.txt
  fi

  ## Gp XRAY ##
  if [ -e ${LOCAL_LOCATION}/${DATE}_Gp_xr_1m.txt ]; then
    echo >> /dev/null;
  else
    wget ftp://ftp.swpc.noaa.gov/pub/lists/xray/${DATE}_Gp_xr_1m.txt --output-document=/mnt/storage/data/${DATE}_Gp_xr_1m.txt
  fi

  ## Gp PART ##
  if [ -e ${LOCAL_LOCATION}/${DATE}_Gp_part_5m.txt ]; then
    echo >> /dev/null;
  else
    wget ftp://ftp.swpc.noaa.gov/pub/lists/particle/${DATE}_Gp_part_5m.txt --output-document=/mnt/storage/data/${DATE}_Gp_part_5m.txt
  fi

  ## Gs PART ##
  if [ -e ${LOCAL_LOCATION}/${DATE}_Gs_part_5m.txt ]; then
    echo >> /dev/null;
  else
    wget ftp://ftp.swpc.noaa.gov/pub/lists/particle/${DATE}_Gs_part_5m.txt --output-document=/mnt/storage/data/${DATE}_Gs_part_5m.txt
  fi

  day=$(($day - 1 ))
done
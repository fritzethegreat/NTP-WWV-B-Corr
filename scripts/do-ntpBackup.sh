#!/bin/bash
#
# DESCRIPTION
# -----------
# Provided for copying the essential ntp files from whatever server is out there
# to this local location. Requires that a public/private rsa key pair is
# available and accessible to the script and installed on the remote computer.
#
# Will copy the previous day's clockstats, peerstats, and loopstats files to
# the specified local directory.
#
# SETUP
# -----
# You will need an account on this local computer that has the same name as an
# account on the remote computer. From the user's account on this local
# computer, open a shell and execute:
#
#    user_name$ ssh-keygen -t rsa
#
# Do NOT enter a passphrase. This will allow for automated login capability.
# Put the private key (file: id_rsa) into this local account's ~/.ssh/
# Transfer a copy of the public key (file: id_rsa.pub) into the remote
# account's ~/.ssh/authorized_keys file.
#
# NOTE: MAIN IS LOCATED AT THE BOTTOM OF THE SCRIPT
#
# Functions:
#   genPrevDate -- generates the previous date
#   doBackups   -- actually copies the files
#

# Foreign location's information
REMOTE_LOGIN="fritze"
REMOTE_LOCATION="chronos.eng.uiowa.edu"
REMOTE_RSA="/usr/local/bin/.id_rsa"

# Local information
LOCAL_LOCATION="/mnt/storage/ntpstats"

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

#
# doBackup -- actually performs the copies. Modify the list of files to include
#             different files in the backup
#
function doBackup() {
  for file in clockstats peerstats loopstats; do
    # Test if file exists, otherwise copy the file
    if [[ -e "${LOCAL_LOCATION}/$file.$DATE" ]]; then
      continue;
    else
      scp -i $REMOTE_RSA $REMOTE_LOGIN@$REMOTE_LOCATION:/var/log/ntpstats/$file.$DATE $LOCAL_LOCATION
    fi
  done
}

genPrevDay
doBackup

cp ${LOCAL_LOCATION}/clockstats.${DATE} /mnt/storage/data/${DATE}_clockstats.txt

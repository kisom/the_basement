#!/bin/sh

lookup=$(which geoip_lu)

if [ -z "$1" ]; then
    echo "need to specify which logfile to use!"
    exit 1
elif [ -n "$2" ]; then
    $lookup="$lookup -b $2"
fi

if echo $1 | grep -q "access.log"   ; then
    $lookup $(cat $1| awk '{ print $1 }' | uniq | xargs )
elif echo $1 | grep -q "error.log" ; then
    $lookup $(cat $1 | awk -F[ '{ print $4 }' | awk -F] '{print $1}' | \
    awk '{print $2}')
else
    echo "unrecognized logfile"
    exit 1
fi



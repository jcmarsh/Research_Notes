#!/bin/sh

if [ $# -eq 0 ]
then
    echo What up with that?
fi

if [ "$1" = "-a" ]
then
    echo ALLL
elif [ "$1" = "-t" ]
then
    echo TEXT
else
    echo YOU NEED HELP
fi
    


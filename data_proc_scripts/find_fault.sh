#!/bin/bash

if [ -z $1 ]
then
  echo "Specify main run log name.\n"
  exit $E_MISSING_POS_PARAM
fi

awk '/Iteration/{i++}{print > "run_log_"i".txt";}' $1

# There's plenty more to automate here.

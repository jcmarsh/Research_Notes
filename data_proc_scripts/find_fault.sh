#!/bin/bash

awk '/Iteration/{i++}{print > "run_log_"i".txt";}' run_log.txt

# There's plenty more to automate here.

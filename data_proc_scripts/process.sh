#!/bin/bash

# $1 should be the top level directory

moveLogs () {
    sudo chown jcmarsh *
    mv *log.txt ./logs
}

moveAll () {
    moveLogs
    mv *injector* ./inject_logs
}

processAll () {
    # delete first two lines of each file, last line of logs?
    sed -i -e 1,2d *.txt
    sed -i -e 1,2d ./logs/*.txt
    sed -i '$ d' ./logs/*.txt
    # combine the files into one!
    cat *.txt >> combined.txt
    cat ./logs/*log.txt >> ./logs/log_combined.txt
}

echo $1

cd $1
tar -cvf backup.tar *

cd ./all
moveLogs
processAll
cd ../

cd ./all_tri
moveLogs
processAll
cd ../
 
cd ./art
moveLogs
processAll
cd ../

cd ./rl_other
moveLogs
processAll
cd ../

cd ./filter
moveLogs
processAll
cd ../

cd ./art_kill
moveAll
processAll
cd ../

cd ./kill
moveAll
processAll
cd ../

cd ./sdc
moveAll
processAll
cd ../

# cd ./fault
# moveAll
# processAll
# cd ../

cd ./freeze
moveAll
processAll
cd ../

cd ./all_wl
moveLogs
processAll
cd ../

cd ./all_tri_wl
moveLogs
processAll
cd ../
 
cd ./art_wl
moveLogs
processAll
cd ../

cd ./rl_other_wl
moveLogs
processAll
cd ../

cd ./filter_wl
moveLogs
processAll
cd ../

cd ./art_kill_wl
moveAll
processAll
cd ../

cd ./kill_wl
moveAll
processAll
cd ../

cd ./sdc_wl
moveAll
processAll
cd ../

cd ./freeze_wl
moveAll
processAll
cd ../

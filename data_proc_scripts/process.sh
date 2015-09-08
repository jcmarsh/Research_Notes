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

fixName () {
    for i in `seq 0 9`;
    do
	mv $1_$i.txt $1_0$i.txt
    done
}

echo $1

cd $1
tar -cvf backup.tar *

cd ./all
moveLogs
processAll
fixName "baseline_all"
cd ../

cd ./all_tri
moveLogs
processAll
fixName "baseline_all_tri"
cd ../
 
cd ./art
moveLogs
processAll
fixName "baseline_art_tmr_planning_dmr_filter_smr"
cd ../

cd ./rl_other
moveLogs
processAll
fixName "baseline_rl_tri_other_dmr"
cd ../

cd ./filter
moveLogs
processAll
fixName "baseline_filter_smr"
cd ../

cd ./art_kill
moveAll
processAll
fixName "art_kill_test_tri"
fixName "inject_logs/art_kill_test_tri_injector"
cd ../

cd ./kill
moveAll
processAll
fixName "kill_test_tri"
fixName "inject_logs/kill_test_tri_injector"
cd ../

cd ./sdc
moveAll
processAll
fixName "sdc_test_tri"
fixName "inject_logs/sdc_test_tri_injector"
cd ../

# cd ./fault
# moveAll
# processAll
# fixName "fault_test_tri"
# fixName "inject_logs/fault_test_tri_injector"
# cd ../

# cd ./fault_wl
# moveAll
# processAll
# fixName "fault_test_tri_wl"
# fixName "inject_logs/fault_test_tri_injector_wl"
# cd ../

cd ./freeze
moveAll
processAll
fixName "freeze_test_tri"
fixName "inject_logs/freeze_test_tri_injector"
cd ../

cd ./all_wl
moveLogs
processAll
fixName "baseline_all_wl"
cd ../

cd ./all_tri_wl
moveLogs
processAll
fixName "baseline_all_tri_wl"
cd ../
 
cd ./art_wl
moveLogs
processAll
fixName "baseline_art_tmr_planning_dmr_filter_smr_wl"
cd ../

cd ./rl_other_wl
moveLogs
processAll
fixName "baseline_rl_tri_other_dmr_wl"
cd ../

cd ./filter_wl
moveLogs
processAll
fixName "baseline_filter_smr_wl"
cd ../

cd ./art_kill_wl
moveAll
processAll
fixName "art_kill_test_tri_wl"
fixName "inject_logs/art_kill_test_tri_injector_wl"
cd ../

cd ./kill_wl
moveAll
processAll
fixName "kill_test_tri_wl"
fixName "inject_logs/kill_test_tri_injector_wl"
cd ../

cd ./sdc_wl
moveAll
processAll
fixName "sdc_test_tri_wl"
fixName "inject_logs/sdc_test_tri_injector_wl"
cd ../

cd ./freeze_wl
moveAll
processAll
fixName "freeze_test_tri_wl"
fixName "inject_logs/freeze_test_tri_injector_wl"
cd ../

Data Processing Scripts
=======================

## Workflow

Run process.sh, then use other scripts to further interrogate.

TODO: Examples.

* `.../mini_test> sh ../../scripts/process.sh ./21_08_15/`

Let Dropbox move the files.

Look at histograms of the runs:
* `.../mini_test/21_08_15> sh ../../../scripts/examine_each.sh 'python ../../../scripts/2d_histo.py' ./all/logs/`

`python ../../../scripts/run_stats.py ./filter/logs/19*`


## Scripts shell
* `examine_each.sh` - simple script to repeat a command for each file in a directory.
* `fix_number_ten.sh` - renames files to have single digit suffixes replaced with 0x.
* `process.sh` - Prepares the logs from a run to be processed further. MUST BE PASSED THE TOP LEVEL DIRECTORY OF THE TEST RUN RESULTS. Backups the results, updates permissions, moves log files into appropriate directories, removes first two entries, and the final entry for logs (removes startup timing errors and unfinished last lines), and creates combined files.

## Scripts python
* `2d_histo.py` - Makes a pretty hexagonal heatmap of the run from the coordinates reported in the log file.
* `display_runs_points.py` - Displays the timing data recorded by voter / benchmarker.
* `run_stats.py` - Combine maze run results from multiple runs to give average completion time, std dev, and worse case.

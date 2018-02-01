Research_Notes
==============

## Directories / Contents
* `./data_proc_scripts/` - Mostly python programs to process data from PINT experiments. See `./data_proc_scripts/scripts.md`
* `./grid_draw/` - Python program to draw grids and mazes for use in Player\Stage.
* `./log_serv/` - Likely abandoned attempt to write a logger server using unix domain sockets
* `./notes/` - Notes on various topics. Many now in the "Notes" section of my site.
* `./notes/page_fault_test/` - Trying to rid my system of all page faults (after initialization)
* `./misc_test/` - Miscellaneous test programs
  * `./misc_test/asm_test/` - Test out / exercise various things in assembly
  * `./misc_test/c_test/` - Test out various things in c (call conventions, sizeof, etc.)
    * `./misc_test/c_test/anon_vma/` - Run this to see if anon_vma_clone is allowing linear chain growth when children keep forking.
  * `./misc_test/flex_bison/` - Trying out flex and bison.
  * `./misc_test/python_test/` - Fun with python.
    * `new_application.py` - create a new cFE application similar to the example app but with user defined names.
* `./data_results` - 
* `./qemu_net/` - Scripts for (hopefully) setting up the tap based network for qemu.
* `./rt_ipc/` - 
* `./rt_timer/` - 
* `./script_gen/` - A now defunct python script which creates a bash script to run Player\Stage experiments, plus the associated config files.
* `./signal_timing/` - wait, how is this different from rt_timer?
* `./task_inspect/` - A system call to a loadable module intended to allow for quick debugging of a process's kernel side data structures.
* `./tmux_dev` - Script to set up a tmux working environment
* `./wiki_script` - Scripts for updating my personal website from my local wiki.

## Random bits

So, github does a few funny things when considering what counts in the running green ticker.
Commits done to forks, for example, don't seem to count.
Which is all fine, until you are using it to track a resolution to work at least one hour every day and the only commit you made was to a fork.

Fortunately, the date for a commit can be specified:
git commit --date 2018-01-08T19:49:00

Alright, I need to remember to commit my Daily Log everyday.

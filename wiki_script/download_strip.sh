#!/bin/sh

# Downloads the wiki hosted on the local machine (127.0.0.1)
# Strips out all *.pdf files and some unecessary pages (Edit,
# History, and Print) to reduce size.

# "download" the localhost's wiki
wget -r -k -np -p -l 0 http://127.0.0.1/wiki/pmwiki.php --html-extension
mv 127.0.0.1/wiki/* ./
rm -rf 127.0.0.1/

# remove the diff, edit, print, and RecentChanges
rm *action=diff.html
rm *action=edit.html
rm *action=print.html
rm *RecentChanges.html

# remove most, but not all .pdfs.
cd uploads/Background
rm *.pdf
cd ..
rm ./System/Kan13SchedulingOPRoS.pdf
rm ./System/Parker01Evaluating.pdf
rm ./Notes/c200-chipset.pdf
cd Research
rm AdvancedLinuxProgramming.pdf
rm gat_jpl-lisp.pdf
rm Composite_Style_Guide.pdf
rm JPL_C_Coding_Practices.pdf
rm LinuxVirtualMemoryManagement.pdf

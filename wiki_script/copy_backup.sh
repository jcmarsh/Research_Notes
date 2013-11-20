#!/bin/sh

# Copies the user's wiki from /var/www/wiki to ./actual, so this
# should be run in the directory in which the backups are kept.
# A tar file is then made, either of everything (-a) or just the
# text files (-t)

# Original contents of ./actual are deleted.

# If keeping on a USB drive formated with a windows format, then
# will have to be run with 'sh copy_backup -[a|t]' (since the
# execute bit can not be set.

if [ "$1" = "-a" ] || [ "$1" = "-t" ]
then
    rm -r ./actual

    # Everything is always copied into ./actual
    echo "Copying /var/www/wiki to ./actual"
    cp -r /var/www/wiki ./actual

    DATE=`date +%Y_%m_%d`
    if [ "$1" = "-a" ]
    then
	# Tar everything
	FILE_NAME="actual_"$DATE"_all.tar"
	echo "Taring ./all to "$FILE_NAME
	tar -cf $FILE_NAME ./actual
    elif [ "$1" = "-t" ]
    then
	# Only Tar the text (found in wiki.d)
	echo "Making a temp folder"
	cp -r ./actual/wiki.d ./temp_text

	FILE_NAME="actual_"$DATE"_text.tar"
	echo "Taring ./wiki.d to "$FILE_NAME
	tar -cf $FILE_NAME ./temp_text

	rm -r ./temp_text
    fi
else
    echo "-a for all files, -t for just wiki.d (the text)"
fi


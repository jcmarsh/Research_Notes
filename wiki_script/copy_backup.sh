#!/bin/sh

if [ $# -eq 0 ]
then
    echo "-a for all files, -t for just wiki.d (the text)"
else
    rm -r ./actual

    echo "Copying /var/www/wiki to ./actual"
    cp -r /var/www/wiki ./actual

    if [ "$1" = "-a" ]
    then
	
	echo "Making a temp folder"
	cp -r ./actual ./all

	echo "Taring ./all to actual_date.tar"
	tar -cf actual_`date +%Y_%m_%d`_all.tar ./all

	rm -r ./all
    elif [ "$1" = "-t" ]
    then
	echo "Making a temp folder"
	cp -r ./actual/wiki.d ./wiki.d

	echo "Taring ./wiki.d to actual_date.tar"
	tar -cf actual_`date +%Y_%m_%d`_text.tar ./wiki.d

	rm -r ./wiki.d
    fi
fi


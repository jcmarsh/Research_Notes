#!/bin/bash

echo $1

for i in `seq 0 9`;
do
    mv $1_$i.txt $1_0$i.txt
done

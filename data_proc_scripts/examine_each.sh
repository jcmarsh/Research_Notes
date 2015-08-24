#!/bin/bash

for f in $2/*
do
    $1 $f
done

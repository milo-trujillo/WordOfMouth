#!/bin/sh
#
# This is a quick and dirty script that fires up two copies of WordOfMouth, running config
# A and B for easy debugging
#
cp example-config/encrypted-a.conf ./enco.txt
./wordofmouth
cp example-config/encrypted-b.conf ./enco.txt
./wordofmouth

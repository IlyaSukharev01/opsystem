#!/usr/bin/bash
cd /home/ilya/Desktop/
#Creating directories
#Go to created directory
mkdir 9091
cd 9091
mkdir Sukharev
cd Sukharev
#Writing date to created file
date > Ilya.txt
#Writing date of next mon to created file
date --date='next Mon' > filedate.txt
cat Ilya.txt filedate.txt > result.txt
cat result.txt

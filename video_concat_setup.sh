#!/bin/sh

#ls *.avi | while read each; do echo "file '$each'" >> mylist.txt
ffmpeg -f concat -i newlist.txt -c copy cam1.avi
done
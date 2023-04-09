#!/bin/sh

#ls *.avi | while read each; do echo "file '$each'" >> mylist.txt
cp /home/vadl/FullscalePrime/video_concat_setup.py /home/vadl/FullscalePrime/build/ImageCapture/cam1/
cp /home/vadl/FullscalePrime/video_concat_setup.py /home/vadl/FullscalePrime/build/ImageCapture/cam2/
cp /home/vadl/FullscalePrime/video_concat_setup.py /home/vadl/FullscalePrime/build/ImageCapture/cam3/
num_vids_1=$1
num_vids_2=$2
num_vids_3=$3
cd /home/vadl/FullscalePrime/build/ImageCapture/cam1/
python video_concat_setup.py -n $num_vids_1
ffmpeg -f concat -i newlist.txt -c copy cam1.avi
cd /home/vadl/FullscalePrime/build/ImageCapture/cam2/
python video_concat_setup.py -n $num_vids_2
ffmpeg -f concat -i newlist.txt -c copy cam2.avi
cd /home/vadl/FullscalePrime/build/ImageCapture/cam3/
python video_concat_setup.py -n $num_vids_3
ffmpeg -f concat -i newlist.txt -c copy cam3.avi
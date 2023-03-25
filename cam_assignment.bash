#!/bin/bash
sudo unlink /dev/videoCam1
sudo unlink /dev/videoCam3
sudo unlink /dev/videoCam2

v4l2-ctl --list-devices > cam_devices.txt
SUB='Arducam USB Camera: Arducam USB'
KERNEL1='1.4):'
KERNEL2="1.3.1):"
KERNEL3="1.1):"
FOUND1=false
FOUND2=false
FOUND3=false
while read p; do
  FULL_LINE=$p
  if [ "$FOUND1" = true ]; then
      sudo ln -s "$p" "/dev/videoCam1"
      echo "sudo ln -s $p dev/videoCam1"
      FOUND1=false
  elif [ "$FOUND2" = true ]; then
      sudo ln -s "$p" "/dev/videoCam2"
      echo "sudo ln -s $p dev/videoCam2"
      FOUND2=false
  elif [ "$FOUND3" = true ]; then
      sudo ln -s "$p" "/dev/videoCam3"
      echo "sudo ln -s $p dev/videoCam3"
      FOUND3=false
  fi
  if [[ "$FULL_LINE" =~ .*"$SUB".* ]]; then
    port_number=${FULL_LINE:50}
    echo "$port_number"
    if [ "$port_number" = "$KERNEL1" ]; then
      FOUND1=true
    elif [ "$port_number" = "$KERNEL2" ]; then
      FOUND2=true
    elif [ "$port_number" = "$KERNEL3" ]; then
      FOUND3=true
    fi
  fi
done <cam_devices.txt
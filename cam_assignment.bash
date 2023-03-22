#!/bin/bash
sudo unlink /dev/videoLaserbeak
sudo unlink /dev/videoRavage
sudo unlink /dev/videoRumble

v4l2-ctl --list-devices > cam_devices.txt
SUB='Arducam USB Camera: Arducam USB'
KERNEL1='1.1):'
KERNEL2="1.2.1):"
KERNEL3="1.4):"
FOUND1=false
FOUND2=false
FOUND3=false
while read p; do
  FULL_LINE=$p
  if [ "$FOUND1" = true ]; then
      sudo ln -s "$p" "/dev/videoLaserbeak"
      echo "sudo ln -s $p dev/videoLaserbeak"
      FOUND1=false
  elif [ "$FOUND2" = true ]; then
      sudo ln -s "$p" "/dev/videoRavage"
      echo "sudo ln -s $p dev/videoRavage"
      FOUND2=false
  elif [ "$FOUND3" = true ]; then
      sudo ln -s "$p" "/dev/videoRumble"
      echo "sudo ln -s $p dev/videoRumble"
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
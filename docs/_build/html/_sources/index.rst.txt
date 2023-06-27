
Fullscale Prime
===============

Welcome to the VADL 2023 software repository! The goal of this project was to autonomously run the team's payload. This involves launch-apogee-landing detection, RCB to properly place the key nacelle at the top, lift to elevate the key nacelle, and run the SDRs and cameras to autonomously decode APRS packets and perform the proper image processing actions. The code was written by Kush Hari, Meredith Hunter, Andrew Noonan, Matthew McGowan, and technically Eric Holst with README written by Kush Hari.

Installation
------------

This code is designed to work with a Raspberry Pi 4 with the Raspbian Bullseye operating system (but it could probably work with Ubuntu or Ubuntu MATE). The only major installation is OpenCV. OpenCV is an open source computer vision library with lots of cool and efficient CV algorithms, so you don't got to write it yourself.

OpenCV Installation
^^^^^^^^^^^^^^^^^^^

The tutorial I installed OpenCV was a bit extra, but it works. You may have issues with the computer crashing, which you can fix with the swap memory (see end of tutorial)

.. code-block::

   sudo apt-get -y purge wolfram-engine
   sudo apt-get -y purge libreoffice*
   sudo apt-get -y clean
   sudo apt-get -y autoremove

   echo "OpenCV installation by learnOpenCV.com"
   cvVersion="master"

   # Clean build directories
   rm -rf opencv/build
   rm -rf opencv_contrib/build

   # Create directory for installation
   mkdir installation
   mkdir installation/OpenCV-"$cvVersion"

   # Save current working directory
   cwd=$(pwd)

   sudo apt -y update
   sudo apt -y upgrade

   sudo apt-get -y remove x264 libx264-dev

   ## Install dependencies
   sudo apt-get -y install build-essential checkinstall cmake pkg-config yasm
   sudo apt-get -y install git gfortran
   sudo apt-get -y install libjpeg8-dev libjasper-dev libpng12-dev

   sudo apt-get -y install libtiff5-dev

   sudo apt-get -y install libtiff-dev

   sudo apt-get -y install libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev
   sudo apt-get -y install libxine2-dev libv4l-dev
   cd /usr/include/linux
   sudo ln -s -f ../libv4l1-videodev.h videodev.h
   cd $cwd

   sudo apt-get -y install libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev
   sudo apt-get -y install libgtk2.0-dev libtbb-dev qt5-default
   sudo apt-get -y install libatlas-base-dev
   sudo apt-get -y install libmp3lame-dev libtheora-dev
   sudo apt-get -y install libvorbis-dev libxvidcore-dev libx264-dev
   sudo apt-get -y install libopencore-amrnb-dev libopencore-amrwb-dev
   sudo apt-get -y install libavresample-dev
   sudo apt-get -y install x264 v4l-utils

   # Optional dependencies
   sudo apt-get -y install libprotobuf-dev protobuf-compiler
   sudo apt-get -y install libgoogle-glog-dev libgflags-dev
   sudo apt-get -y install libgphoto2-dev libeigen3-dev libhdf5-dev doxygen

   git clone https://github.com/opencv/opencv.git
   cd opencv
   git checkout $cvVersion
   cd ..

   git clone https://github.com/opencv/opencv_contrib.git
   cd opencv_contrib
   git checkout $cvVersion
   cd ..

   cd opencv
   mkdir build
   cd build

   cmake -D CMAKE_BUILD_TYPE=RELEASE \
               -D CMAKE_INSTALL_PREFIX=$cwd/installation/OpenCV-"$cvVersion" \
               -D INSTALL_C_EXAMPLES=ON \
               -D INSTALL_PYTHON_EXAMPLES=ON \
               -D WITH_TBB=ON \
               -D WITH_V4L=ON \
               -D OPENCV_PYTHON3_INSTALL_PATH=$cwd/OpenCV-$cvVersion-py3/lib/python3.5/site-packages \
               -D WITH_QT=ON \
               -D WITH_OPENGL=ON \
               -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
               -D BUILD_EXAMPLES=ON ..

   make -j$(nproc)
   make install

   sudo sed -i 's/CONF_SWAPSIZE=1024/CONF_SWAPSIZE=100/g' /etc/dphys-swapfile
   sudo /etc/init.d/dphys-swapfile stop
   sudo /etc/init.d/dphys-swapfile start

   echo "sudo modprobe bcm2835-v4l2" &amp;amp;amp;gt;&amp;amp;amp;gt; ~/.profile

Based on this link: https://learnopencv.com/install-opencv-4-on-raspberry-pi/

Other installations
-------------------

Since we get pigpio from source, it should automatically compile. You may be missing a few installations here and there, but you should be able to install them with apt.

Package Explanations/Running the Code
-------------------------------------

To compile and build the code, run the following. You will see that all the other packages will build first, then the MainDriver package. This is because the MainDriver relies on the other packages to be compiled libraries before it can compile. The full payload code can be run with the following executable, but it is not recommended to run this cold as it relies on the payload to be perfectly setup. Also, always compile from the FullscalePrime directory, don't individually compile stuff in their own folders, just makes things messy.

.. code-block::

   mkdir build
   cd build
   cmake ..
   make
   cd build/MainDriver
   sudo ./realDisplayTest

Package Explanation
-------------------

This project has a lot of different components, so that's why there are so many packages. Each package written this year should have detailed comments. Some packages may not have detailed comments, but they have been previously verified to work. Here is a brief description on each package and if it is useful.

FieldTest: Includes a series of different starter tests we would run in the field to help debug issues quickly (Useful)

HFSM: Includes all of the states for the state machine (Useful)

ImageCapture: Has some random OpenCV image tests. (Meh, may have some useful stuff if you want to learn OpenCV, but poorly documented and may not work, sorry)

IMU: Includes wrapper code for the VN Library for IMU. This package was written in a previous year, and I don't know about the mutex stuff, but it works. (Useful)

Log: Includes code for the log to save flight data for easy integration with our MATLAB sim. This package was writtein in a previous year. (Useful)

MainDriver: The bread and butter, the meat and potatoes, the autobots and decepticons, this is where all the code comes together to make the target executable. (Useful)

MultithreadTest: Mini test to see how we can do multithreading in C++. (Meh, helpful to see some multithreading code, but it may not work)

PacketReception: Includes wrapper code for our TCP socket to receive APRS packets. (Useful)

pigpio-master: Source code to allow for GPIO interaction. Fun fact, it is Pi GPIO and not Pig Pio. (Useful)

SerialCute++ (Name credits go to Meredith): Wrapper code for Serial communication objects. (Useful)

ServoTest: Mini test to see how to move servo in C++ (Not Useful)

Stepper: Includes stepper motor library for Raspberry Pi. Based on Arduino library. (Useful)

VectorNav: VN-100 library provided by VectorNav. Allows us to read IMU data from each register at 20 Hz. If you want 2 registers, you get 10 Hz. (Useful)

Bash/Python Script Explanations
-------------------------------

cam_assignment.bash: Looks at which cameras are connected to the Pi, assigns them with static names based on which port they are plugged in (Useful)
kill_sdr.bash: Kills all SDR running processes. Important because SDR can't run if it is already running (Useful)
processaltitude.py: Interprets log files to make altitude plots (Meh, MATLAB sims are better. Cool to get this running and see plots on the field, but it might not work anymore)
video_concat scripts: We save the flight videos in increments, so this will just merge it all together (Meh, cool to get this running if relevant, but probably doesn't work anymore)

Limitations
-----------

The code in its current form did not successfully operate in the NASA USLI competition despite functioning through all of our tests. We were able to successfully record flight video, perform RCB, detect all cameras and SDRS post-landing, and lift. However, during the lift, we lost all USB connections despite no hardware disruptions. With a software reboot, we were able to successfully execute the payload. Further testing revealed that this error happens 5-10% of the time. The leading reason for failure is the fact that we powered the Raspberry Pi with GPIO pins, rather than the USB-C or barrel jack port. This is bad because while it is safe voltage-wise, you can only run limited current through the GPIO pins. Perhaps when running the camera, the current spiked up, leading to undefined behavior of the USBs disconnecting. However, this should be thouroughly tested.
https://forums.raspberrypi.com/viewtopic.php?t=196531

.. toctree::
   :caption: Getting Started
   :maxdepth: 2
   :hidden:

   FieldTest/summary
   HFSM/summary
   IMU/summary
   Log/summary
   MainDriver/summary
   PacketReception/summary
   SerialCute++/summary
   Stepper/summary

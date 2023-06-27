
HFSM
====

Contains all of the states and the root pointer for the state machine library used in this project. This project has no executables. See MainDriver to see how to implement HFSM library.

Root
-----------------
Contains all common variables and functions accessible to every state in the state machine. It is important to note that we have some variables for display mode, DO NOT RUN DISPLAY MODE, IT DOESN'T WORK

.. doxygenfile:: Root.h
   :project: Fullscale Prime

State
-----------------
Base state.

.. doxygenfile:: State.h
   :project: Fullscale Prime

State_RAFCO_Mission
-----------------
State responsible for receiving APRS packets and perform image processing with camera nacelles

.. doxygenfile:: State_RAFCO_Mission.h
   :project: Fullscale Prime

State_Enums
-----------------
Contains all StateName and EventName enums used in project

.. doxygenfile:: State_Enums.h
   :project: Fullscale Prime
State_RCB_Motor
-----------------
State responsible for testing RCB motor without IMU

.. doxygenfile:: State_RCB_Motor.h
   :project: Fullscale Prime
State_Camera_Check
-----------------
State responsible for determining primary camera for RCB

.. doxygenfile:: State_Camera_Check.h
   :project: Fullscale Prime
State_Lift_Motor
-----------------
State responsible for testing lift motor without limit switch

.. doxygenfile:: State_Lift_Motor.h
   :project: Fullscale Prime
State_Nacelle_Servo
-----------------
State responsible for testing nacelle locking servo movement

.. doxygenfile:: State_Nacelle_Servo.h
   :project: Fullscale Prime
State_Lift_Servo
-----------------
State responsible for testing lift servo to see if it can move

.. doxygenfile:: State_Lift_Servo.h
   :project: Fullscale Prime
State_Level_Servo
-----------------
State responsible for testing movement for the level servo

.. doxygenfile:: State_Level_Servo.h
   :project: Fullscale Prime
State_Stepper1
-----------------
State responsible for testing movement for nacelle 1

.. doxygenfile:: State_Stepper1.h
   :project: Fullscale Prime
State_Stepper2
-----------------
State responsible for testing movement for nacelle 2

.. doxygenfile:: State_Stepper2.h
   :project: Fullscale Prime
State_Stepper3
-----------------
State responsible for testing movement for nacelle 3

.. doxygenfile:: State_Stepper3.h
   :project: Fullscale Prime
State_Prelaunch
-----------------
State responsible for verifying parameters before launch detection

.. doxygenfile:: State_Prelaunch.h
   :project: Fullscale Prime
State_Launch_Detection
-----------------
State responsible for detecting launch with IMU

.. doxygenfile:: State_Launch_Detection.h
   :project: Fullscale Prime
State_Apogee_Detection
-----------------
State responsible for detecting apogee with IMU

.. doxygenfile:: State_Apogee_Detection.h
   :project: Fullscale Prime
State_Landing_Detection
-----------------
State responsible for detecting landing with the IMU

.. doxygenfile:: State_Landing_Detection.h
   :project: Fullscale Prime
State_Full_Lift
-----------------
State responsible for elevating the key nacelle with the lift

.. doxygenfile:: State_Full_Lift.h
   :project: Fullscale Prime
State_Full_Level
-----------------
State responsible for levelling the key nacelle parallel with the horizon

.. doxygenfile:: State_Full_Level.h
   :project: Fullscale Prime
State_Full_RCB
-----------------
State responsible for RCB to rotate key nacelle on top

.. doxygenfile:: State_Full_RCB.h
   :project: Fullscale Prime
State_PDS_Delay
-----------------
State responsible for program to "sleep" while waiting for Teensy parachute to detach

.. doxygenfile:: State_PDS_Delay.h
   :project: Fullscale Prime
State_SDR1
-----------------
State responsible for debugging SDR 1

.. doxygenfile:: State_SDR1.h
   :project: Fullscale Prime
State_SDR2
-----------------
State responsible for debugging SDR 2

.. doxygenfile:: State_SDR2.h
   :project: Fullscale Prime
State Generation Script
-------

The writestate.py script should be able to automatically write a state header and cpp file along with update the CMakeLists. It should update the State Enum header, but it may not add any events. 
``python writestate.py -n <STATE_NAME>``

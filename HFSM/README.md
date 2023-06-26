# HFSM
Contains all of the states and the root pointer for the state machine library used in this project. This project has no executables. See MainDriver to see how to implement HFSM library.

## File Explanations
Root: Contains all common variables and functions accessible to every state in the state machine. It is important to note that we have some variables for display mode, DO NOT RUN DISPLAY MODE, IT DOESN'T WORK

State: Base state

State_RAFCO_Mission: State responsible for receiving APRS packets and perform image processing with camera nacelles

State_Enums: Contains all StateName and EventName enums used in project

State_RCB_Motor: State responsible for testing RCB motor without IMU

State_Camera_Check: State responsible for determining primary camera for RCB

State_Lift_Motor: State responsible for testing lift motor without limit switch

State_Nacelle_Servo: State responsible for testing nacelle locking servo movement

State_Lift_Servo: State responsible for testing lift servo to see if it can move

State_Level_Servo: State responsible for testing movement for the level servo

State_Stepper1: State responsible for testing movement for nacelle 1

State_Stepper2: State responsible for testing movement for nacelle 2

State_Stepper3: State responsible for testing movement for nacelle 3

State_Prelaunch: State responsible for verifying parameters before launch detection

State_Launch_Detection: State responsible for detecting launch with IMU

State_Apogee_Detection: State responsible for detecting apogee with IMU

State_Landing_Detection: State responsible for detecting landing with the IMU

State_Full_Lift: State responsible for elevating the key nacelle with the lift

State_Full_Level: State responsible for levelling the key nacelle parallel with the horizon

State_Full_RCB: State responsible for RCB to rotate key nacelle on top

State_PDS_Delay: State responsible for program to "sleep" while waiting for Teensy parachute to detach

State_SDR1: State responsible for debugging SDR 1

State_SDR2: State responsible for debugging SDR 2

## Scripts
The writestate.py script should be able to automatically write a state header and cpp file along with update the CMakeLists. It should update the State Enum header, but it may not add any events. 
```python writestate.py -n <STATE_NAME>```
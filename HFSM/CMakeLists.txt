include_directories(include)
set (CMAKE_PREFIX_PATH "/home/vadl/opencv/build")
find_package( OpenCV REQUIRED )

add_library(HFSM
        src/State_RAFCO_Mission.cpp
        src/State_SDR2.cpp
        src/State_SDR1.cpp
        src/State_PDS_Delay.cpp
        src/State_Full_RCB.cpp
        src/State_Full_Level.cpp
        src/State_Full_Lift.cpp
        src/State_Landing_Detection.cpp
        src/State_Apogee_Detection.cpp
        src/State_Launch_Detection.cpp
        src/State_Prelaunch.cpp
        src/State_Stepper3.cpp
        src/State_Level_Servo.cpp
        src/State_Lift_Motor.cpp
        src/State_Lift_Servo.cpp
        src/State_Nacelle_Servo.cpp
        src/State_RCB_Motor.cpp
        src/State_Stepper2.cpp
        src/State_Stepper1.cpp
        src/State_RAFCO_Mission.cpp
        src/Root.cpp
        src/State.cpp
        )

target_link_libraries(HFSM Stepper pigpio PacketReceiver IMU Log VN PacketReceiver SerialObject ${OpenCV_LIBS})
target_include_directories(HFSM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
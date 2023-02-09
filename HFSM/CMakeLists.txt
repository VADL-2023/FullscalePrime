include_directories(include)

add_library(HFSM
        src/State_Prelaunch.cpp
        src/State_Stepper3.cpp
        src/State_Level_Servo.cpp
        src/State_Lift_Motor.cpp
        src/State_Lift_Servo.cpp
        src/State_Nacelle_Servo.cpp
        src/State_RCB_Motor.cpp
        src/State_Stepper2.cpp
        src/State_Stepper1.cpp
        src/Root.cpp
        src/State.cpp
        )

target_link_libraries(HFSM Stepper pigpio IMU Log VN)
target_include_directories(HFSM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
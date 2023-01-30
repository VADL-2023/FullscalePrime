include_directories(include)

add_library(HFSM
        src/State_Stepper2.cpp
        src/State_Final.cpp
        src/State_Initial.cpp
        src/State_Stepper1.cpp
        src/Root.cpp
        src/State.cpp
        src/State_A.cpp
        src/State_B.cpp
        )

target_link_libraries(HFSM Stepper pigpio)
target_include_directories(HFSM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
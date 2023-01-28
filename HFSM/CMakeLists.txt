include_directories(include)

add_library(HFSM
<<<<<<< HEAD
        src/State_Stepper_Test.cpp
=======
        src/State_C.cpp
>>>>>>> 6e12645 (Added enums to represent states and events)
        src/Root.cpp
        src/State.cpp
        src/State_A.cpp
        src/State_B.cpp
        )

target_link_libraries(HFSM Stepper pigpio)
target_include_directories(HFSM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
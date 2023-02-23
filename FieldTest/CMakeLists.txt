project(lift_servo_test)
cmake_minimum_required(VERSION 3.18)
include_directories(include)

add_executable(liftServoTest src/LiftServoTest.cpp)
add_executable(nacelleServoTest src/NacelleLockTest.cpp)
add_executable(liftArrowTest src/LiftArrowTest.cpp)
add_executable(rcbArrowTest src/RCBArrowTest.cpp)
add_executable(swivel2Test src/Swivel2Test.cpp)
add_executable(swivel3Test src/Swivel3Test.cpp)
add_executable(swivel3TestBlowup src/Swivel3TestBlowup.cpp)
add_executable(swivel3TestStop src/Swivel3TestStop.cpp)


target_link_libraries(liftServoTest pigpio)
target_link_libraries(nacelleServoTest pigpio)
target_link_libraries(liftArrowTest pigpio)
target_link_libraries(rcbArrowTest pigpio VN IMU)
target_link_libraries(swivel2Test pigpio Stepper)
target_link_libraries(swivel3Test pigpio Stepper)
target_link_libraries(swivel3TestBlowup pigpio Stepper ncurses)
target_link_libraries(swivel3TestStop pigpio Stepper ncurses)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(liftServoTest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(liftArrowTest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(rcbArrowTest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(swivel2Test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(swivel3Test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(swivel3TestBlowup ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(swivel3TestStop ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(nacelleServoTest ${CMAKE_THREAD_LIBS_INIT})




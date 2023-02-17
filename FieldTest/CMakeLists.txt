project(lift_servo_test)
cmake_minimum_required(VERSION 3.18)
include_directories(include)

add_executable(liftServoTest src/LiftServoTest.cpp)
add_executable(nacelleServoTest src/NacelleLockTest.cpp)
add_executable(liftArrowTest src/LiftArrowTest.cpp)
add_executable(rcbArrowTest src/RCBArrowTest.cpp)
add_executable(swivel2Test src/Swivel2Test.cpp)



target_link_libraries(liftServoTest pigpio)
target_link_libraries(nacelleServoTest pigpio)
target_link_libraries(liftArrowTest pigpio)
target_link_libraries(rcbArrowTest pigpio VN IMU)
target_link_libraries(swivel2Test pigpio Stepper)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(liftServoTest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(liftArrowTest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(rcbArrowTest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(swivel2Test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(nacelleServoTest ${CMAKE_THREAD_LIBS_INIT})




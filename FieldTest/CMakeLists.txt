project(lift_servo_test)
cmake_minimum_required(VERSION 3.18)
include_directories(include)

add_executable(liftServoTest src/LiftServoTest.cpp)
add_executable(liftArrowTest src/LiftArrowTest.cpp)

target_link_libraries(liftServoTest pigpio)
target_link_libraries(liftArrowTest pigpio)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(liftServoTest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(liftArrowTest ${CMAKE_THREAD_LIBS_INIT})

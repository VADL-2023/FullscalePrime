cmake_minimum_required(VERSION 2.8)
project( servo_test )

include_directories( include )

add_executable(servoTest src/servo_test.cpp)
target_link_libraries(servoTest pigpio)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(servoTest ${CMAKE_THREAD_LIBS_INIT})
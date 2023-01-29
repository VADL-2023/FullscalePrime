project(main_driver)
cmake_minimum_required(VERSION 3.18)
include_directories(include)

add_executable(exampleProject src/example.cpp)
add_executable(stepper1Test src/unit_tests/stepper1_test.cpp)
target_link_libraries(exampleProject HFSM)
target_link_libraries(stepper1Test HFSM pigpio)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(exampleProject ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(stepper1Test ${CMAKE_THREAD_LIBS_INIT})
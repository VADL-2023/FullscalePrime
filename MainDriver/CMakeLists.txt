project(main_driver)
cmake_minimum_required(VERSION 3.18)
include_directories(include)

add_executable(displayTest src/integration_tests/display.cpp)

target_link_libraries(displayTest HFSM pigpio)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(displayTest ${CMAKE_THREAD_LIBS_INIT})
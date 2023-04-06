project(multithread_test)
cmake_minimum_required(VERSION 3.18)
include_directories(include)

add_executable(multithreadTest src/main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(multithreadTest ${CMAKE_THREAD_LIBS_INIT})

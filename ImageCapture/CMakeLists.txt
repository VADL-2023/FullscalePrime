project(image_capture)
cmake_minimum_required(VERSION 3.18)
include_directories(include)

add_executable(imageCapTest src/image_capture_test.cpp)

target_link_libraries(imageCapTest SerialObject)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(imageCapTest ${CMAKE_THREAD_LIBS_INIT})

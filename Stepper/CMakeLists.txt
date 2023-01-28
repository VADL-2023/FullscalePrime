cmake_minimum_required(VERSION 2.8)
project( stepper )

include_directories( include )

add_library( Stepper 
                src/Stepper.cpp )

add_executable(stepperTest src/Stepper_runner.cpp)
target_link_libraries(stepperTest Stepper pigpio)
target_include_directories( Stepper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(stepperTest ${CMAKE_THREAD_LIBS_INIT})
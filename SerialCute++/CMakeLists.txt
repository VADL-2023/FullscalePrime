cmake_minimum_required(VERSION 2.8)
project( serial_test )

include_directories( include )

add_library( SerialObject 
                src/SerialObject.cpp )

add_executable( SerialTest src/SerialTest.cpp)


target_link_libraries( SerialTest SerialObject )
target_include_directories( SerialObject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )
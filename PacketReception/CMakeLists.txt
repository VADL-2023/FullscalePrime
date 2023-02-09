cmake_minimum_required( VERSION 2.8 )
project( packet_reception_test )

SET(CMAKE_CXX_FLAGS -pthread)

add_library( PacketReceiver 
                src/PacketReceiver.cpp )

add_executable( SqulechTest SquelchTest.cpp )

target_link_libraries( SqulechTest PacketReceiver )
target_include_directories( PacketReceiver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )
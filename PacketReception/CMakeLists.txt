cmake_minimum_required( VERSION 2.8 )
project( packet_reception_test )

SET(CMAKE_CXX_FLAGS -pthread)

add_library( PacketReceiver 
                src/PacketReceiver.cpp )

target_link_libraries(PacketReceiver Log)
add_executable( PacketTest PacketTest.cpp )
add_executable( TCPTest TCPTest.cpp )

target_link_libraries( PacketTest PacketReceiver Log)
target_link_libraries( TCPTest PacketReceiver Log)
target_include_directories( PacketReceiver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )
include_directories(include)
add_library(VN
        src/attitude.cpp 
        src/compositedata.cpp
        src/conversions.cpp
        src/criticalsection.cpp
        src/dllvalidator.cpp
        src/event.cpp
        src/error_detection.cpp
        src/ezasyncdata.cpp
        src/memoryport.cpp
        src/packet.cpp
        src/packetfinder.cpp
        src/port.cpp
        src/position.cpp
        src/searcher.cpp
        src/sensors.cpp
        src/serialport.cpp
        src/thread.cpp
        src/types.cpp
        src/util.cpp
        src/utilities.cpp
        src/vntime.cpp)

target_include_directories(VN PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/vn)

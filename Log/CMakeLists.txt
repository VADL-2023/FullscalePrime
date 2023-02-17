include_directories(include)

add_library(Log
        src/Log.cpp
        )

target_link_libraries(Log VN)
target_include_directories(Log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

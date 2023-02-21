include_directories(include)

add_library(IMU src/IMU.cpp src/common.cpp src/commonOutMutex)
target_link_libraries(IMU VN pigpio)
target_include_directories(IMU PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(IMU ${PYTHON_LIBRARIES})

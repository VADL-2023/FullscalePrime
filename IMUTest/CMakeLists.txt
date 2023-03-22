project(imu_test)

add_executable(imuTest src/IMU_test.cpp)
target_link_libraries(imuTest VN IMU pigpio Log SerialObject)
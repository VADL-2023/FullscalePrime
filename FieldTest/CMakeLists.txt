project(lift_servo_test)
cmake_minimum_required(VERSION 3.18)
include_directories(include)

# Compiles cpp file so that it can be an executable
add_executable(liftServoTest src/LiftServoTest.cpp)
add_executable(levelServoTest src/LevelServoTest.cpp)
add_executable(levelLockTest src/LevelLockTest.cpp)

add_executable(nacelleServoTest src/NacelleLockTest.cpp)
add_executable(liftArrowTest src/LiftArrowTest.cpp)
add_executable(rcbArrowTest src/RCBArrowTest.cpp)
add_executable(swivel1Test src/Swivel1Test.cpp)
add_executable(swivel2Test src/Swivel2Test.cpp)
add_executable(swivel3Test src/Swivel3Test.cpp)

# Ensures that cpp file can access specific libraries
target_link_libraries(liftServoTest pigpio)
target_link_libraries(levelServoTest pigpio)
target_link_libraries(levelLockTest pigpio)

target_link_libraries(nacelleServoTest pigpio)
target_link_libraries(liftArrowTest pigpio)
target_link_libraries(rcbArrowTest pigpio VN IMU)
target_link_libraries(swivel1Test pigpio Stepper)
target_link_libraries(swivel2Test pigpio Stepper)
target_link_libraries(swivel3Test pigpio Stepper)

# Allows cpp files to do multithreading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(liftServoTest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(levelServoTest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(levelLockTest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(liftArrowTest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(rcbArrowTest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(swivel1Test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(swivel2Test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(swivel3Test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(nacelleServoTest ${CMAKE_THREAD_LIBS_INIT})




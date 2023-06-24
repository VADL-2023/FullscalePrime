# For every CMakeLists, need project and cmake minimum defined
project(prime_project)
cmake_minimum_required(VERSION 3.18)

# add_subdirectory will run the CMakeLists for the specified folder
add_subdirectory(pigpio-master)
add_subdirectory(IMU)
add_subdirectory(Log)
add_subdirectory(VectorNav)
add_subdirectory(SerialCute++)
add_subdirectory(ImageCapture)
add_subdirectory(Stepper)
add_subdirectory(PacketReception)
add_subdirectory(HFSM)
add_subdirectory(ServoTest)
add_subdirectory(FieldTest)
add_subdirectory(MainDriver)

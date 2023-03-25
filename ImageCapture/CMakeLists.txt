project(image_capture)
cmake_minimum_required(VERSION 3.18)
include_directories(include)
set (CMAKE_PREFIX_PATH "/home/vadl/opencv/build")
find_package( OpenCV REQUIRED )

add_executable(imageCapTest src/image_capture_test.cpp)
add_executable(usbDisplayTest src/usb_display_test.cpp)
add_executable(usbSavePNGTest src/usb_save_png_test.cpp)


target_link_libraries(imageCapTest SerialObject ${OpenCV_LIBS})
target_link_libraries(usbDisplayTest ${OpenCV_LIBS})
target_link_libraries(usbSavePNGTest ${OpenCV_LIBS})



set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(imageCapTest ${CMAKE_THREAD_LIBS_INIT})

cmake_minimum_required(VERSION 2.8)
project( main )
find_package( OpenCV 4.0.0 REQUIRED )


add_executable( main main.cpp )
target_link_libraries( main ${OpenCV_LIBS} zbar)

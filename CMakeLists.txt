cmake_minimum_required(VERSION 3.10)
project(VideoToAscii)

set(CMAKE_CXX_STANDARD 14)

include_directories(incl)
file(GLOB SOURCES "src/*.cpp")

find_package( OpenCV REQUIRED )
include_directories( ${OPENCV_INCLUDE_DIRS} )

add_executable(VideoToAscii ${SOURCES})

target_link_libraries(VideoToAscii ${OpenCV_LIBS} )

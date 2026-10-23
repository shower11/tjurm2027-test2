cmake_minimum_required(VERSION 3.5)
project(tjurm-test2)

set(CMAKE_CXX_STANDARD 14)
set(OpenCV_DIR "/usr/local/opencv4.5.4/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)
message(STATUS "找到 OpenCV 版本: ${OpenCV_VERSION}")

add_executable(tjurm-test2 src/main.cpp)

target_include_directories(tjurm-test2 PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(tjurm-test2 PRIVATE ${OpenCV_LIBS})
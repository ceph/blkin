cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")  
find_package(Boost COMPONENTS thread system REQUIRED)
find_package(lttng-ust REQUIRED)

# make && make test
enable_testing()

add_subdirectory(blkin-lib)

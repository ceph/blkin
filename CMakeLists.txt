cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")  
find_package(Boost COMPONENTS thread system REQUIRED)
find_package(lttng-ust REQUIRED)

include_directories(.)

#blkin
set(blkin_srcs
    zipkin_c.c
    ztracer.cc
    tp.c
)
add_library(blkin SHARED ${blkin_srcs})
target_link_libraries(blkin dl lttng-ust)

#test
set(test_srcs
    test.c
)
add_executable(test ${test_srcs})
target_link_libraries(test blkin boost_system boost_thread)

#testpp
set(testpp_srcs
    test.cc
)
add_executable(testpp ${testpp_srcs})
target_link_libraries(testpp blkin boost_system boost_thread)

#testppp
set(testppp_srcs
    test_p.cc
)
add_executable(testppp ${test_srcs})
target_link_libraries(testppp blkin boost_system boost_thread)

install(TARGETS blkin DESTINATION lib)

set(blkin_headers
  zipkin_c.h
  zipkin_trace.h
  ztracer.hpp
)
install(FILES ${blkin_headers} DESTINATION include/blkin)


include_directories(.)

#blkin
set(blkin_srcs
    zipkin_c.c
    tp.c
)
add_library(blkin SHARED ${blkin_srcs})
target_link_libraries(blkin dl lttng-ust)

install(TARGETS blkin DESTINATION lib)

set(blkin_headers
  zipkin_c.h
  zipkin_trace.h
  ztracer.hpp
)
install(FILES ${blkin_headers} DESTINATION include/blkin)

add_subdirectory(tests)

#test
add_executable(testc test.c)
target_link_libraries(testc blkin lttng-ust boost_system boost_thread)
add_test(testc testc)

#testpp
add_executable(testpp test.cc)
target_link_libraries(testpp blkin lttng-ust boost_system boost_thread)
add_test(testpp testpp)

#testppp
add_executable(testppp test_p.cc)
target_link_libraries(testppp blkin lttng-ust boost_system boost_thread)
add_test(testppp testppp)

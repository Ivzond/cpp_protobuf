find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS})

add_executable(test_helpers Test_Helpers.cpp)
target_link_libraries(test_helpers ${Protobuf_LIBRARIES} proto GTest::gtest_main)
target_include_directories(test_helpers PUBLIC ${PROJECT_SOURCE_DIR}/src ${CMAKE_BINARY_DIR}/src)

add_executable(test_stream_parser Test_DelimitedMessagesStreamParser.cpp)
target_link_libraries(test_stream_parser ${Protobuf_LIBRARIES} proto GTest::gtest_main)
target_include_directories(test_stream_parser PUBLIC ${PROJECT_SOURCE_DIR}/src ${CMAKE_BINARY_DIR}/src)

include(GoogleTest)
gtest_discover_tests(test_helpers)
gtest_discover_tests(test_stream_parser)

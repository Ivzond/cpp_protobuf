cmake_minimum_required(VERSION 3.10)
project(protobuf_parser)

set(CMAKE_CXX_STANDARD 14)

# Пути к исходникам
include_directories(src)
include_directories(${CMAKE_BINARY_DIR}) # для сгенерированных файлов protobuf

# Источники
set(SOURCES
        src/protobuf_parser/DelimitedMessagesStreamParser.cpp
        src/protobuf_parser/helpers.cpp
        src/protobuf/message.pb.cc
)

# Библиотека
add_library(protobuf_parser STATIC ${SOURCES})

# Протобуф
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

# gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Тесты
set(TEST_SOURCES
        test/Test_DelimitedMessagesStreamParser.cpp
        test/Test_Helpers.cpp
)

add_executable(runTests ${TEST_SOURCES})
target_link_libraries(runTests protobuf_parser ${PROTOBUF_LIBRARIES} ${GTEST_LIBRARIES} GTest::gtest_main)

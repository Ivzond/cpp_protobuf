cmake_minimum_required(VERSION 3.15.0)
project(protobuf_parser)

set(CMAKE_CXX_STANDARD 14)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS protobuf/message.proto)

add_library(proto ${PROTO_SRCS} ${PROTO_HDRS})

add_library(protobuf_parser
        protobuf_parser/DelimitedMessagesStreamParser.hpp
        protobuf_parser/helpers.hpp
)

target_link_libraries(protobuf_parser proto ${Protobuf_LIBRARIES})

cmake_minimum_required(VERSION 3.8)
project(server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package (Boost REQUIRED system thread)

set(SOURCE_FILES main.cpp core/server.hpp core/server.cpp core/connection.hpp core/connection.cpp request/request.hpp
    request/request_handler.cpp request/request_handler.hpp request/request_parser.hpp request/request_parser.cpp
    reply/mime_types.hpp reply/mime_types.cpp reply/reply.cpp reply/reply.hpp request/header.hpp
)

find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(server ${SOURCE_FILES})
add_executable(boosting ${SOURCE_FILES})
target_link_libraries (server ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})

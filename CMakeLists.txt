cmake_minimum_required(VERSION 3.0.0)

project(StreamBase VERSION 0.1.0)

set(SERVER_SOURCE server/main.cpp
                  server/named_pipe_server.cpp)

set(SERVER_INCLUDE server/named_pipe_server.h)

set(CLIENT_SOURCE client/main.cpp)

set(SHARED_SOURCE shared/windows_helper.cpp)
set(SHARED_INCLUDE shared/windows_helper.h)

add_library(Shared STATIC ${SHARED_SOURCE} ${SHARED_INCLUDE})
add_executable(Client ${CLIENT_SOURCE})
add_executable(Server ${SERVER_SOURCE} ${SERVER_INCLUDE})

target_include_directories(Client PUBLIC shared)
target_include_directories(Server PUBLIC shared)

target_link_libraries(Client Shared)
target_link_libraries(Server Shared)
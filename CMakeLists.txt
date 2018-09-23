cmake_minimum_required(VERSION 3.0.0)

project(StreamBase VERSION 0.1.0)

set(SERVER_SOURCE server/main.cpp
                  server/server.cpp
                  server/named_pipe_server.cpp)

set(SERVER_INCLUDE server/server.h
                   server/named_pipe_server.h)

set(CLIENT_SOURCE client/main.cpp
                  client/screens/screen_factory.cpp
                  client/screens/connect_screen.cpp
                  client/screens/screen_data.cpp)

set(CLIENT_INCLUDE client/screens/screen_factory.h
                   client/screens/connect_screen.h
                   client/screens/screen_interface.h
                   client/screens/connect_screen.h)

set(SHARED_SOURCE shared/windows_helper.cpp
                  shared/named_pipe_io.cpp)
set(SHARED_INCLUDE shared/windows_helper.h
                   shared/named_pipe_io.h
                   shared/comm.h)

add_library(Shared STATIC ${SHARED_SOURCE} ${SHARED_INCLUDE})
add_executable(Client ${CLIENT_SOURCE} ${CLIENT_INCLUDE})
add_executable(Server ${SERVER_SOURCE} ${SERVER_INCLUDE})

target_include_directories(Client PUBLIC shared)
target_include_directories(Server PUBLIC shared)

target_link_libraries(Client Shared)
target_link_libraries(Server Shared)
cmake_minimum_required(VERSION 3.0.0)

project(StreamBase VERSION 0.1.0)

set(SERVER_SOURCE server/main.cpp)
set(CLIENT_SOURCE client/main.cpp)

add_executable(Client ${CLIENT_SOURCE})
add_executable(Server ${SERVER_SOURCE})
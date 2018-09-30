cmake_minimum_required(VERSION 3.0.0)

project(StreamBase VERSION 0.1.0)

set(SERVER_SOURCE server/main.cpp
                  server/server.cpp
                  server/named_pipe_server.cpp
                  server/commands/command_factory.cpp
                  server/commands/store_command.cpp
                  server/commands/read_stored_command.cpp
                  server/commands/list_classes_command.cpp
                  server/commands/store_class_command.cpp
                  server/commands/list_params_command.cpp
                  server/commands/execute_method_command.cpp
                  server/data_objects/storage_data.cpp
                  server/data_objects/stored_class_data.cpp
                  server/custom_classes/custom_class_factory.cpp
                  server/custom_classes/custom_class_interface.cpp
                  server/custom_classes/custom_class_runner.cpp)

set(SERVER_INCLUDE server/server.h
                   server/named_pipe_server.h
                   server/commands/command_factory.h
                   server/commands/command_interface.h
                   server/commands/store_command.h
                   server/commands/read_stored_command.h
                   server/commands/list_classes_command.h
                   server/commands/store_class_command.h
                   server/commands/list_params_command.h
                   server/commands/execute_method_command.h
                   server/data_objects/data_object_interface.h
                   server/data_objects/storage_data.h
                   server/data_objects/stored_class_data.h
                   server/custom_classes/custom_class_factory.h
                   server/custom_classes/custom_class_interface.h
                   server/custom_classes/custom_class_runner.h)

set(CLIENT_SOURCE client/main.cpp
                  client/screens/screen_factory.cpp
                  client/screens/connect_screen.cpp
                  client/screens/home_screen.cpp
                  client/screens/store_data_screen.cpp
                  client/screens/read_stored_data_screen.cpp
                  client/screens/store_classes_screen.cpp
                  client/screens/screen_data.cpp)

set(CLIENT_INCLUDE client/screens/screen_factory.h
                   client/screens/connect_screen.h
                   client/screens/home_screen.h
                   client/screens/store_data_screen.h
                   client/screens/read_stored_data_screen.h
                   client/screens/store_classes_screen.h
                   client/screens/screen_interface.h
                   client/screens/connect_screen.h)

set(SHARED_SOURCE shared/windows_helper.cpp
                  shared/config.cpp
                  shared/named_pipe_io.cpp)

set(SHARED_INCLUDE shared/windows_helper.h
                   shared/config.h
                   shared/named_pipe_io.h
                   shared/comm.h)

add_library(Shared STATIC ${SHARED_SOURCE} ${SHARED_INCLUDE})
add_executable(Client ${CLIENT_SOURCE} ${CLIENT_INCLUDE})
add_executable(Server ${SERVER_SOURCE} ${SERVER_INCLUDE})

target_include_directories(Client PUBLIC shared client)
target_include_directories(Server PUBLIC shared server)

target_link_libraries(Client Shared)
target_link_libraries(Server Shared)
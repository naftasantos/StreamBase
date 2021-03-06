# StreamBase
This project is intended to develop a client/server architecture with the following requirements

Two console applications using C++: 1. client 2. server; with the following features:

* [x] The client should be able to connect to the server through a NamedPipe
* [x] The client should be able to make both sync and async calls to the server
* [x] The client should be able to send trivial data (strings, numbers) to the server
* [x] The client should be able to create objects on the server (based on req-7 below), retrieve them, their attributes and call methods on them
* [x] The server should be able to receive both sync/async connection requests from clients
* [x] The server should be able to store data provided by the client via NamedPipe in a reasonable data structure
* [x] The server should be able to register a custom class (w/ related functions, attributes) which can be used by the client (see req-4)
* [x] The server should be able to store the custom objects created by the client for the custom class created in req-7

## Assumptions
* NamedPipes API refers to Microsoft Windows' NamedPipes API
* Custom class on req-7 means an instance of a class with custom values to its properties associated with a given client.

## Decisions
* Since there is no requirement for multiple simultaneous connections, I decided to have single connection implemented to speed up the development

## How to use
> Note:
> The binaries are located under the `dist` directory. These binaries were compiled using MSVC2017 compilers, runtimes for this environment must be installed prior to running them. Only Windows API and C++ standard libraries were used.

Call `server.exe` and then `client.exe`. Once the client is shut down the server will as well.

The client will show a menu where you can select options from which you will interact with the server through the clients interface.

Both the server and client accepts the parameter `-async`, when this parameter is passed, they will behave asynchronously.
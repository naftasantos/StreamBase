# StreamBase
This project is intended to develop a client/server architecture with the following requirements

Two console applications using C++: 1. client 2. server; with the following features:

1. The client should be able to connect to the server through a NamedPipe
1. The client should be able to make both sync and async calls to the server
1. The client should be able to send trivial data (strings, numbers) to the server
1. The client should be able to create objects on the server (based on req-7 below), retrieve them, their attributes and call methods on them
1. The server should be able to receive both sync/async connection requests from clients
1. The server should be able to store data provided by the client via NamedPipe in a reasonable data structure
1. The server should be able to register a custom class (w/ related functions, attributes) which can be used by the client (see req-4)
1. The server should be able to store the custom objects created by the client for the custom class created in req-7

# Assumptions:
* NamedPipes API refers to Microsoft Windows' NamedPipes API
* Custom class on req-7 means an instance of a class with custom values to its properties associated with a given client.

# Decisions
* Since there is no requirement for multiple simultaneous connections, I decided to have single connection implemented to speed up the development

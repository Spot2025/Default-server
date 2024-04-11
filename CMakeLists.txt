cmake_minimum_required(VERSION 3.5)

project(Server)

set(HEADERS
		cool_sockets.h
        server.h)

set(SOURCES
		cool_sockets.cpp
        server.cpp
        main.cpp
)

add_executable(server ${HEADERS} ${SOURCES})

set_target_properties(server PROPERTIES
		CXX_STANDARD 14
		CXX_STANDARD_REQUIRED ON
)
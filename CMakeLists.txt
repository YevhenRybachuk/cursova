cmake_minimum_required(VERSION 3.30)
project(cursova)

set(CMAKE_CXX_STANDARD 20)

add_executable(cursova main.cpp
        ISerializable.h
        Entity.h
        Team.cpp
        Team.h
        User.cpp
        User.h
        DatabaseManager.cpp
        DatabaseManager.h)

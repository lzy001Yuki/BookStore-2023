cmake_minimum_required(VERSION 3.22)
project(BookStorefinal)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(code
        src/main.cpp
        src/Book.cpp
        src/Finance.cpp
        src/User.cpp
        src/TokenScanner.cpp
        src/error.cpp
        include/Block.h
        include/Book.h
        include/error.h
        include/User.h
        include/TokenScanner.h
        include/Finance.h
)

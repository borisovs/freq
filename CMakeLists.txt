cmake_minimum_required(VERSION 3.16)

project(freq LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
        src/exceptions.h
        src/freq.h
)

set(SOURCES
        src/freq.cpp
        src/main.cpp
)
find_package(Threads REQUIRED)

add_executable(freq ${HEADERS} ${SOURCES})

include(GNUInstallDirs)
install(TARGETS freq
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

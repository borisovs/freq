cmake_minimum_required(VERSION 3.16)

project(freq LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
        src/freq.h
        src/exceptions.h
)

set(SOURCES
        src/main.cpp
        src/freq.cpp
)
find_package(Threads REQUIRED)

add_executable(freq ${HEADERS} ${SOURCES})
target_link_libraries(freq PRIVATE Threads::Threads)

include(GNUInstallDirs)
install(TARGETS freq
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

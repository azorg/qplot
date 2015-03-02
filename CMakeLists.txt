project(qplot)

cmake_minimum_required(VERSION 2.6)

if(CMAKE_GENERATOR MATCHES "Visual Studio 10")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/runtime)
endif(CMAKE_GENERATOR MATCHES "Visual Studio 10")

if(CMAKE_GENERATOR MATCHES "Visual Studio 9 2008")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/runtime)
endif(CMAKE_GENERATOR MATCHES "Visual Studio 9 2008")

if(CMAKE_GENERATOR MATCHES "MinGW Makefiles")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/runtime/mingw)
endif(CMAKE_GENERATOR MATCHES "MinGW Makefiles")

if(CMAKE_GENERATOR MATCHES "Unix Makefiles")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/runtime/unix)
endif(CMAKE_GENERATOR MATCHES "Unix Makefiles")


find_package(Qt4)
if(QT4_FOUND)

add_subdirectory(./clib)
add_subdirectory(./qwt-src)
add_subdirectory(./src)

endif(QT4_FOUND)

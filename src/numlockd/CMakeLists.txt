cmake_minimum_required(VERSION 3.16)

project(numlockd VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)
find_package(Dtk6 REQUIRED COMPONENTS Core)

include(CheckIncludeFileCXX)
check_include_file_cxx(linux/uinput.h HAVE_LINUX_UINPUT_H)
if(NOT HAVE_LINUX_UINPUT_H)
    message(FATAL_ERROR "linux/uinput.h not found")
endif()

add_executable(numlockd)

set_target_properties(numlockd PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_sources(numlockd PRIVATE
    main.cpp
    numlock_simulator.cpp
    numlock_simulator.h
    ipc_server.cpp
    ipc_server.h
)

target_link_libraries(numlockd PRIVATE
    Qt6::Core
    Qt6::Network
    Dtk6::Core
)

install(TARGETS numlockd
    RUNTIME DESTINATION bin
)

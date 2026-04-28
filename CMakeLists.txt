cmake_minimum_required(VERSION 3.16)

project(candle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Find DTK6 packages
find_package(Dtk6 REQUIRED COMPONENTS Widget Core Gui)

add_executable(${PROJECT_NAME} "main.cpp" resources.qrc)

target_link_libraries(candle
    Dtk6::Widget
    Dtk6::Core
    Dtk6::Gui
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

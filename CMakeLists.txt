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

# 尝试查找 DTK6，如果找不到则使用 Qt6 纯版本
find_package(Dtk6 COMPONENTS Widget Core Gui QUIET)

if(Dtk6_FOUND)
    message(STATUS "DTK6 found, building with DTK support")
    add_executable(${PROJECT_NAME} "main.cpp" resources.qrc)
    target_link_libraries(candle
        Dtk6::Widget
        Dtk6::Core
        Dtk6::Gui
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
    )
    target_compile_definitions(candle PRIVATE USE_DTK=1)
else()
    message(STATUS "DTK6 not found, building with Qt6 only")
    add_executable(${PROJECT_NAME} "main_qt6.cpp" resources.qrc)
    target_link_libraries(candle
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
    )
endif()

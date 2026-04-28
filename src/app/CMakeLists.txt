cmake_minimum_required(VERSION 3.16)

# GUI 应用程序
set(APP_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    resources.qrc
)

add_executable(candle ${APP_SOURCES})

set_target_properties(candle PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_link_libraries(candle
    Dtk6::Widget
    Dtk6::Core
    Dtk6::Gui
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

install(TARGETS candle
    RUNTIME DESTINATION bin
)

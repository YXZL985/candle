cmake_minimum_required(VERSION 3.16)

# 项目名称
project(candle)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# 设置安装前缀为 /opt/apps/org.yxzl.candle
set(CMAKE_INSTALL_PREFIX "/opt/apps/org.yxzl.candle" CACHE PATH "Installation prefix" FORCE)

# 查找 Qt6 包
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# 查找 DTK6 包
find_package(Dtk6 REQUIRED COMPONENTS Widget Core Gui)

# 源文件列表
set(SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    resources.qrc
)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接库
target_link_libraries(candle
    Dtk6::Widget
    Dtk6::Core
    Dtk6::Gui
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

# 安装目标
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# 安装图标
install(FILES images/logo.svg
    DESTINATION share/icons/hicolor/scalable/apps
    RENAME org.yxzl.candle.svg
)

# 添加 numlockd 守护进程子目录
add_subdirectory(numlockd)

# 安装 numlockd 守护进程（必须在 add_subdirectory 之后）
install(TARGETS numlockd
    RUNTIME DESTINATION bin
)

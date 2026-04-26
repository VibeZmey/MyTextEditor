cmake_minimum_required(VERSION 3.19)
project(MyTextEditor LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(MyTextEditor
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/modules/editor/editor.cpp
    src/modules/editor/editor.h
    src/modules/command/command.cpp
    src/modules/command/command.h
    src/modules/command/commandhistory.cpp
    src/modules/command/commandhistory.h
    src/modules/syntaxhighlighter/syntaxhighlighter.cpp
    src/modules/syntaxhighlighter/syntaxhighlighter.h
    src/modules/filemanager/filemanager.cpp
    src/modules/filemanager/filemanager.h
    src/modules/searchmanager/searchmanager.cpp
    src/modules/searchmanager/searchmanager.h
    src/modules/mainwindow/mainwindow.cpp
    src/modules/mainwindow/mainwindow.h
    src/modules/mainwindow/mainwindow.ui
    resources/resources.qrc
)

target_include_directories(MyTextEditor PRIVATE 
    src/modules/editor
    src/modules/command
    src/modules/syntaxhighlighter
    src/modules/filemanager
    src/modules/searchmanager
    src/modules/mainwindow
)

target_link_libraries(MyTextEditor
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

include(GNUInstallDirs)

install(TARGETS MyTextEditor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MyTextEditor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
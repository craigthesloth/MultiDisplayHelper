cmake_minimum_required(VERSION 3.28+)

project(MultiDisplayHelper VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets
                                     Gui
                                     Multimedia
                                 )

# required
# sudo apt install qt6-base-dev libqt6widgets6 libxtst-dev
# sudo apt install qt6-multimedia-dev libglib2.0-dev

set(PROJECT_SOURCES
    main.cpp

    src/mainwindow/mainwindow.cpp
    src/mainwindow/mainwindow.h
    src/mainwindow/mainwindow.ui

    src/screen_widget/screen_widget.h
    src/screen_widget/screen_widget.cpp

    src/mouse_controller/mouse_controller.h
    src/mouse_controller/mouse_controller.cpp

    src/screen_capturer/screen_capturer.h
    src/screen_capturer/screen_capturer.cpp

    src/popout_window/popout_window.h
    src/popout_window/popout_window.cpp

    src/interfaces/IMouseController.h
    src/interfaces/IScreenCapturer.h
    src/interfaces/IMouseHandler.h
    src/interfaces/IWindowWidget.h
    src/interfaces/IPopOutWindow.h



)


if(WIN32)
    list(APPEND PROJECT_SOURCES
        src/mouse_controller/win/MouseHandlerWin.h
        src/mouse_controller/win/MouseHandlerWin.cpp
    )
else()
    list(APPEND PROJECT_SOURCES
        src/mouse_controller/linux/MouseHandlerLinux.h
        src/mouse_controller/linux/MouseHandlerLinux.cpp
    )
endif()


qt_add_executable(MultiDisplayHelper ${PROJECT_SOURCES})

target_include_directories(MultiDisplayHelper PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/popout_window
    ${CMAKE_CURRENT_SOURCE_DIR}/src/screen_widget
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mouse_controller
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mouse_controller/win
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mouse_controller/linux
    ${CMAKE_CURRENT_SOURCE_DIR}/src/screen_capturer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces
)

target_link_libraries(MultiDisplayHelper PRIVATE Qt6::Widgets
                                                 Qt6::Gui
                                                 Qt6::Multimedia
)

if(WIN32)
    target_link_libraries(MultiDisplayHelper PRIVATE user32 gdi32)
else()
    find_package(X11 REQUIRED)
    target_link_libraries(MultiDisplayHelper PRIVATE X11::X11 X11::Xtst)
endif()

set_target_properties(MultiDisplayHelper PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MultiDisplayHelper
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

cmake_minimum_required(VERSION 3.16)

project(MultiDisplayHelper VERSION 0.2 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

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
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MultiDisplayHelper
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${QRC_FILES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MultiDisplayHelper APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MultiDisplayHelper SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MultiDisplayHelper
            ${PROJECT_SOURCES}
            src/popout_window/popout_window.h src/popout_window/popout_window.cpp
            src/interfaces/IMouseController.h
            src/interfaces/IScreenCapturer.h
            src/interfaces/IPopOutWindow.h

        )
    endif()
endif()

target_include_directories(MultiDisplayHelper PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/popout_window
    ${CMAKE_CURRENT_SOURCE_DIR}/src/screen_widget
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow
    ${CMAKE_CURRENT_SOURCE_DIR}/src/screen_widget
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mouse_controller
    ${CMAKE_CURRENT_SOURCE_DIR}/src/screen_capturer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces
)


target_link_libraries(MultiDisplayHelper PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MultiDisplayHelper)
endif()
set_target_properties(MultiDisplayHelper PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MultiDisplayHelper
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MultiDisplayHelper)
endif()

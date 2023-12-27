cmake_minimum_required(VERSION 3.5)

project(TPGUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)


# ****添加vitip************
include(vitip)
include(${CMAKE_MODULE_PATH}/Qt.cmake)

# ****添加OpenCV************
set(OpenCV_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/opencv/cmake/x64/vc16/lib)
find_package(OpenCV 4.5.3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# ****添加tempmatch************
set(tempmatch_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TPGUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TPGUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TPGUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TPGUI
            ${PROJECT_SOURCES}
            lib/tempmatch.cpp
            lib/tempmatch.h
            DrawingWidget.cpp
            DrawingWidget.h
            ImageDisplayWidget.h
            ImageDisplayWidget.cpp
        )
    endif()
endif()

target_link_libraries(TPGUI PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    vitip::vitip3
    ${OpenCV_LIBS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
                           ${Vitip_INCLUDE_DIRS}
                           ${OpenCV_INCLUDE_DIRS}
                           ${tempmatch_DIR}

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TPGUI)
endif()
set_target_properties(TPGUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TPGUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    # RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)



add_subdirectory(lib)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TPGUI)
endif()

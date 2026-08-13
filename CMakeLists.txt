cmake_minimum_required(VERSION 3.16)

project(SensorMonitor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS SerialPort)

set(TS_FILES #SensorMonitor_en_US.ts
)

set(PROJECT_SOURCES
        main.cpp
        Back/Objects/SerialSensor.hpp Back/Objects/SerialSensor.cpp
        Back/Objects/SensorsManager.hpp Back/Objects/SensorsManager.cpp
        Front/Tables/Model/BaseDataTableModel.hpp Front/Tables/Model/BaseDataTableModel.cpp
        Front/Tables/View/SensorsTableView.hpp Front/Tables/View/SensorsTableView.cpp
        Front/Tables/View/BaseDataTableView.hpp Front/Tables/View/BaseDataTableView.cpp
        Front/Tables/Model/SensorsTableModel.hpp Front/Tables/Model/SensorsTableModel.cpp
        Front/Tables/Delegate/SensorsItemDelegate.hpp Front/Tables/Delegate/SensorsItemDelegate.cpp
        Front/Dialogs/BaseDialog.hpp Front/Dialogs/BaseDialog.cpp
        Front/Windows/MainWindow.hpp Front/Windows/MainWindow.cpp
        Front/Widgets/ConsoleWidget.hpp Front/Widgets/ConsoleWidget.cpp
        Back/Structs/SensorData.hpp
        Back/Utility/Utility.hpp
        Back/Utility/Application.hpp
        Back/Utility/ApplicationLogger.hpp Back/Utility/ApplicationLogger.cpp
        Front/Dialogs/SensorsEditorDialog.hpp  Front/Dialogs/SensorsEditorDialog.cpp
        Front/Widgets/SensorsEditorWidget.hpp Front/Widgets/SensorsEditorWidget.cpp Front/Widgets/SensorsEditorWidget.ui
        Front/Tables/Widgets/SerialPortCombobox.hpp Front/Tables/Widgets/SerialPortCombobox.cpp
        Back/Utility/Sensors.hpp
        Back/Utility/SensorUtility.hpp Back/Utility/SensorUtility.cpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SensorMonitor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Resource.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SensorMonitor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

target_include_directories(SensorMonitor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Tables/View
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Tables/Model
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Tables/Delegate
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Tables/Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/Back/Structs
    ${CMAKE_CURRENT_SOURCE_DIR}/Back/Objects
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Dialogs
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Windows
    ${CMAKE_CURRENT_SOURCE_DIR}/Back/Utility
)

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(SensorMonitor SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SensorMonitor
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(SensorMonitor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SensorMonitor PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SensorMonitor)
endif()
set_target_properties(SensorMonitor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SensorMonitor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SensorMonitor)
endif()

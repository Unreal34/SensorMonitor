cmake_minimum_required(VERSION 3.16)

project(SensorMonitor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    SerialPort
    LinguistTools
    Test
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Translations/SensorMonitor_fr_FR.ts
)

# Add ts file in the resource file
qt_add_translations(SensorMonitor
    TS_FILES
        ${TS_FILES}
    RESOURCE_PREFIX "/i18n"
)

set(PROJECT_SOURCES
    main.cpp

    Back/Objects/SerialSensor.hpp
    Back/Objects/SerialSensor.cpp

    Back/Objects/SensorsManager.hpp
    Back/Objects/SensorsManager.cpp

    Back/Structs/SensorData.hpp

    Back/Utility/Utility.hpp
    Back/Utility/Application.hpp
    Back/Utility/ApplicationLogger.hpp
    Back/Utility/ApplicationLogger.cpp
    Back/Utility/Sensors.hpp
    Back/Utility/SensorUtility.hpp
    Back/Utility/SensorUtility.cpp

    Front/Tables/Model/BaseDataTableModel.hpp
    Front/Tables/Model/BaseDataTableModel.cpp
    Front/Tables/Model/SensorsTableModel.hpp
    Front/Tables/Model/SensorsTableModel.cpp

    Front/Tables/View/BaseDataTableView.hpp
    Front/Tables/View/BaseDataTableView.cpp
    Front/Tables/View/SensorsTableView.hpp
    Front/Tables/View/SensorsTableView.cpp

    Front/Tables/Delegate/SensorsItemDelegate.hpp
    Front/Tables/Delegate/SensorsItemDelegate.cpp

    Front/Tables/Widgets/SerialPortCombobox.hpp
    Front/Tables/Widgets/SerialPortCombobox.cpp

    Front/Dialogs/BaseDialog.hpp
    Front/Dialogs/BaseDialog.cpp
    Front/Dialogs/SensorsEditorDialog.hpp
    Front/Dialogs/SensorsEditorDialog.cpp

    Front/Widgets/ConsoleWidget.hpp
    Front/Widgets/ConsoleWidget.cpp
    Front/Widgets/SensorsEditorWidget.hpp
    Front/Widgets/SensorsEditorWidget.cpp
    Front/Widgets/SensorsEditorWidget.ui

    Front/Windows/MainWindow.hpp
    Front/Windows/MainWindow.cpp

    Back/Objects/TranslationManager.hpp Back/Objects/TranslationManager.cpp

    Resource.qrc
)

qt_add_executable(SensorMonitor
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    Back/Objects/SimulatedSerialDevice.hpp Back/Objects/SimulatedSerialDevice.cpp
)

target_include_directories(SensorMonitor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Tables/View
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Tables/Model
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Tables/Delegate
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Tables/Widgets

    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Dialogs
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/Front/Windows

    ${CMAKE_CURRENT_SOURCE_DIR}/Back/Structs
    ${CMAKE_CURRENT_SOURCE_DIR}/Back/Objects
    ${CMAKE_CURRENT_SOURCE_DIR}/Back/Utility
)

target_link_libraries(SensorMonitor PRIVATE
    Qt6::Widgets
    Qt6::SerialPort
)

qt_add_translations(SensorMonitor
    TS_FILES
        ${TS_FILES}
)

if(Qt6_VERSION VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SensorMonitor
    )
endif()

set_target_properties(SensorMonitor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}

    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS SensorMonitor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

function(add_qt_test TEST_NAME)
    qt_add_executable(${TEST_NAME}
        ${ARGN}
    )

    target_link_libraries(${TEST_NAME} PRIVATE
        Qt6::Core
        Qt6::Test
        Qt6::SerialPort
    )

    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
    )

    target_include_directories(${TEST_NAME} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Back/Structs
            ${CMAKE_CURRENT_SOURCE_DIR}/Back/Objects
            ${CMAKE_CURRENT_SOURCE_DIR}/Back/Utility
            ${CMAKE_CURRENT_SOURCE_DIR}/Tests
        )
endfunction()

# Handle unit tests
add_qt_test(SensorsManagerTest
    Tests/SensorsManagerTest.cpp
    Back/Objects/SensorsManager.cpp
    Back/Objects/SensorsManager.hpp
    Back/Objects/SerialSensor.cpp
    Back/Objects/SerialSensor.hpp
    Back/Structs/SensorData.hpp
    Back/Objects/SimulatedSerialDevice.hpp
    Back/Objects/SimulatedSerialDevice.cpp
)

add_qt_test(SensorUtilityTest
    Tests/SensorUtilityTest.cpp
    Back/Utility/SensorUtility.hpp
    Back/Utility/SensorUtility.cpp
    Back/Structs/SensorData.hpp
)

add_qt_test(SerialSensorTest
    Tests/SerialSensorTest.cpp
    Back/Objects/SimulatedSerialDevice.hpp
    Back/Objects/SimulatedSerialDevice.cpp
    Back/Objects/SerialSensor.hpp
    Back/Objects/SerialSensor.cpp
)

qt_finalize_executable(SensorMonitor)

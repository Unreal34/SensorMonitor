cmake_minimum_required(VERSION 3.16)

project(SensorMonitor VERSION 1.0 LANGUAGES CXX)

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

# Handle translation files here.
set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Translations/SensorMonitor_fr_FR.ts
)

# Translation files are dynamically added to the ressource prefix during compilation.
qt_add_translations(SensorMonitor
    TS_FILES
        ${TS_FILES}
    RESOURCE_PREFIX "/i18n"
)

set(PROJECT_SOURCES
    main.cpp

    Back/Objects/Sensor.hpp
    Back/Objects/Sensor.cpp
    Back/Objects/SerialSensor.hpp
    Back/Objects/SerialSensor.cpp
    Back/Objects/SensorsManager.hpp
    Back/Objects/SensorsManager.cpp
    Back/Structs/SerialSensorData.hpp
    Back/Utility/Utility.hpp
    Back/Utility/Application.hpp
    Back/Utility/ApplicationLogger.hpp
    Back/Utility/ApplicationLogger.cpp

    Back/Utility/SensorUtility.hpp
    Back/Utility/SensorUtility.cpp
    Front/Tables/Model/BaseDataTableModel.hpp
    Front/Tables/Model/BaseDataTableModel.cpp
    Front/Tables/Model/SerialSensorsTableModel.hpp
    Front/Tables/Model/SerialSensorsTableModel.cpp
    Front/Tables/View/BaseDataTableView.hpp
    Front/Tables/View/BaseDataTableView.cpp
    Front/Tables/View/SerialSensorsTableView.hpp
    Front/Tables/View/SerialSensorsTableView.cpp
    Front/Tables/Delegate/SerialSensorsItemDelegate.hpp
    Front/Tables/Delegate/SerialSensorsItemDelegate.cpp
    Front/Tables/Widgets/SerialPortCombobox.hpp
    Front/Tables/Widgets/SerialPortCombobox.cpp
    Front/Dialogs/BaseDialog.hpp
    Front/Dialogs/BaseDialog.cpp
    Front/Dialogs/SensorsEditorDialog.hpp
    Front/Dialogs/SensorsEditorDialog.cpp
    Front/Widgets/ConsoleWidget.hpp
    Front/Widgets/ConsoleWidget.cpp
    Front/Widgets/SerialSensorsEditorWidget.hpp
    Front/Widgets/SerialSensorsEditorWidget.cpp
    Front/Widgets/SerialSensorsEditorWidget.ui
    Front/Windows/MainWindow.hpp
    Front/Windows/MainWindow.cpp
    Back/Objects/TranslationManager.hpp Back/Objects/TranslationManager.cpp
    Resource.qrc
)

qt_add_executable(SensorMonitor
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    Back/Objects/SimulatedDevice.hpp Back/Objects/SimulatedDevice.cpp
    Back/Objects/Sensor.hpp Back/Objects/Sensor.cpp
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

# Handle tests
add_qt_test(SensorsManagerTest
    Back/Objects/Sensor.hpp
    Back/Objects/Sensor.cpp
    Tests/SensorsManagerTest.cpp
    Back/Objects/SensorsManager.cpp
    Back/Objects/SensorsManager.hpp
    Back/Objects/SerialSensor.cpp
    Back/Objects/SerialSensor.hpp
    Back/Structs/SerialSensorData.hpp
    Back/Objects/SimulatedDevice.hpp
    Back/Objects/SimulatedDevice.cpp
    Back/Utility/SensorUtility.hpp
    Back/Utility/SensorUtility.cpp
)

add_qt_test(SensorUtilityTest
    Tests/SensorUtilityTest.cpp
    Back/Utility/SensorUtility.hpp
    Back/Utility/SensorUtility.cpp
    Back/Structs/SerialSensorData.hpp
)

add_qt_test(SerialSensorTest
    Tests/SerialSensorTest.cpp
    Back/Objects/SimulatedDevice.hpp
    Back/Objects/SimulatedDevice.cpp
    Back/Objects/Sensor.hpp
    Back/Objects/Sensor.cpp
    Back/Objects/SerialSensor.hpp
    Back/Objects/SerialSensor.cpp
    Back/Utility/SensorUtility.hpp
    Back/Utility/SensorUtility.cpp
)

qt_finalize_executable(SensorMonitor)

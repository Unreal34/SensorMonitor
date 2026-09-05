cmake_minimum_required(VERSION 3.16)

project(SensorMonitor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    SerialPort
    LinguistTools
    Test
)
find_package(Qt6 REQUIRED COMPONENTS Network)

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
    Source/main.cpp
    Source/Back/Objects/Sensor.hpp
    Source/Back/Objects/Sensor.cpp
    Source/Back/Objects/SerialSensor.hpp
    Source/Back/Objects/SerialSensor.cpp
    Source/Back/Objects/SensorsManager.hpp
    Source/Back/Objects/SensorsManager.cpp
    Source/Back/Structs/SerialSensorData.hpp
    Source/Back/Utility/Utility.hpp
    Source/Back/Utility/Application.hpp
    Source/Back/Utility/ApplicationLogger.hpp
    Source/Back/Utility/ApplicationLogger.cpp
    Source/Back/Utility/SensorUtility.hpp
    Source/Back/Utility/SensorUtility.cpp
    Source/Front/Tables/Model/BaseDataTableModel.hpp
    Source/Front/Tables/Model/BaseDataTableModel.cpp
    Source/Front/Tables/Model/SerialSensorsTableModel.hpp
    Source/Front/Tables/Model/SerialSensorsTableModel.cpp
    Source/Front/Tables/View/BaseDataTableView.hpp
    Source/Front/Tables/View/BaseDataTableView.cpp
    Source/Front/Tables/View/SerialSensorsTableView.hpp
    Source/Front/Tables/View/SerialSensorsTableView.cpp
    Source/Front/Tables/Delegate/SerialSensorsItemDelegate.hpp
    Source/Front/Tables/Delegate/SerialSensorsItemDelegate.cpp
    Source/Front/Tables/Widgets/SerialPortCombobox.hpp
    Source/Front/Tables/Widgets/SerialPortCombobox.cpp
    Source/Front/Dialogs/BaseDialog.hpp
    Source/Front/Dialogs/BaseDialog.cpp
    Source/Front/Dialogs/SerialSensorsEditorDialog.hpp
    Source/Front/Dialogs/SerialSensorsEditorDialog.cpp
    Source/Front/Widgets/ConsoleWidget.hpp
    Source/Front/Widgets/ConsoleWidget.cpp
    Source/Front/Widgets/SerialSensorsEditorWidget.hpp
    Source/Front/Widgets/SerialSensorsEditorWidget.cpp
    Source/Front/Widgets/BaseSensorsEditorWidget.ui
    Source/Front/Windows/MainWindow.hpp
    Source/Front/Windows/MainWindow.cpp
    Source/Back/Objects/TranslationManager.hpp
    Source/Back/Objects/TranslationManager.cpp
    Resource.qrc
)

qt_add_executable(SensorMonitor
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    Source/Back/Objects/SimulatedDevice.hpp
    Source/Back/Objects/SimulatedDevice.cpp
    Source/Back/Objects/Sensor.hpp
    Source/Back/Objects/Sensor.cpp
    Source/Back/Objects/Serial_OV7670Camera.hpp
    Source/Back/Objects/Serial_OV7670Camera.cpp
    Source/Front/Windows/ImageViewerSubWindow.hpp
    Source/Front/Windows/ImageViewerSubWindow.cpp
    Source/Back/Objects/Serial_ESP32Camera.hpp
    Source/Back/Objects/Serial_ESP32Camera.cpp
    Source/Back/Objects/SerialCamera.hpp
    Source/Back/Objects/SerialCamera.cpp
    Source/Back/Objects/UdpSensor.hpp
    Source/Back/Objects/UdpSensor.cpp
    Source/Back/Objects/Udp_ESP32Camera.hpp
    Source/Back/Objects/Udp_ESP32Camera.cpp
    Source/Back/Structs/ESP32_PacketHeader.hpp
    Source/Front/Widgets/UdpSensorsEditorWidget.hpp
    Source/Front/Widgets/UdpSensorsEditorWidget.cpp
    Source/Front/Tables/Model/UdpSensorsTableModel.hpp
    Source/Front/Tables/Model/UdpSensorsTableModel.cpp
    Source/Front/Tables/View/UdpSensorsTableView.hpp
    Source/Front/Tables/View/UdpSensorsTableView.cpp
    Source/Front/Tables/Delegate/UdpSensorsItemDelegate.hpp
    Source/Front/Tables/Delegate/UdpSensorsItemDelegate.cpp
    Source/Back/Structs/UdpSensorData.hpp
    Source/Back/Structs/SensorData.hpp
    Source/Front/Widgets/BaseSensorsEditorWidget.hpp
    Source/Front/Widgets/BaseSensorsEditorWidget.cpp
    Source/Front/Dialogs/UdpSensorsEditorDialog.hpp
    Source/Front/Dialogs/UdpSensorsEditorDialog.cpp
    Source/Back/Utility/Localization.hpp
)

target_include_directories(SensorMonitor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_link_libraries(SensorMonitor PRIVATE
    Qt6::Widgets
    Qt6::SerialPort
    Qt6::Gui
    Qt6::Network
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
        Qt6::Network
        Qt6::Gui
    )

    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
    )

    target_include_directories(${TEST_NAME} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Source
        )
endfunction()

# Handle tests
add_qt_test(SensorsManagerTest
    Source/Back/Objects/Sensor.hpp
    Source/Back/Objects/Sensor.cpp
    Tests/SensorsManagerTest.cpp
    Source/Back/Objects/SensorsManager.cpp
    Source/Back/Objects/SensorsManager.hpp
    Source/Back/Objects/SerialSensor.cpp
    Source/Back/Objects/SerialSensor.hpp
    Source/Back/Structs/SerialSensorData.hpp
    Source/Back/Objects/SimulatedDevice.hpp
    Source/Back/Objects/SimulatedDevice.cpp
    Source/Back/Utility/SensorUtility.hpp
    Source/Back/Utility/SensorUtility.cpp
    Source/Back/Objects/Serial_ESP32Camera.hpp
    Source/Back/Objects/Serial_ESP32Camera.cpp
    Source/Back/Objects/Serial_OV7670Camera.hpp
    Source/Back/Objects/Serial_OV7670Camera.cpp
    Source/Back/Objects/SerialCamera.hpp
    Source/Back/Objects/SerialCamera.cpp
    Source/Back/Objects/UdpSensor.hpp
    Source/Back/Objects/UdpSensor.cpp
    Source/Back/Objects/Udp_ESP32Camera.hpp
    Source/Back/Objects/Udp_ESP32Camera.cpp
)

add_qt_test(SensorUtilityTest
    Tests/SensorUtilityTest.cpp
    Source/Back/Utility/SensorUtility.hpp
    Source/Back/Utility/SensorUtility.cpp
    Source/Back/Structs/SerialSensorData.hpp
)

add_qt_test(SerialSensorTest
    Tests/SerialSensorTest.cpp
    Source/Back/Objects/SimulatedDevice.hpp
    Source/Back/Objects/SimulatedDevice.cpp
    Source/Back/Objects/Sensor.hpp
    Source/Back/Objects/Sensor.cpp
    Source/Back/Objects/SerialSensor.hpp
    Source/Back/Objects/SerialSensor.cpp
    Source/Back/Utility/SensorUtility.hpp
    Source/Back/Utility/SensorUtility.cpp
)

qt_finalize_executable(SensorMonitor)

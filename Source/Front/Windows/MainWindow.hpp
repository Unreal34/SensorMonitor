#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMdiArea>

#include "Back/Objects/SensorsManager.hpp"
#include "Front/Widgets/ConsoleWidget.hpp"
#include "Front/Windows/ImageViewerSubWindow.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

public:
    SensorsManager* sensorsManager() const { return mSensorsManager; }

private:
    /**
     * @brief Initializes the toolbar, menus, and required actions.
     */
    void initializeActions();

private slots:
    /**
     * @brief Handles all data forwarded by the sensors.
     * @note This is the main router where all incoming data is received and processed.
     * @note Sensor name is used to handle processing for each sensor.
     * @param sensor
     * @param data
     */
    void onDataReceived(const QString& sensor, const QByteArray& data);

    /**
     * @brief Called when the user wants to edit the serial sensors and their associated serial ports.
     */
    void openSerialSensorsEditorDialog();

    /**
     * @brief Starts or stops data acquisition using the sensor data list saved in the main manager.
     * @note Only serial sensor are supported yet.
     */
    void toggleDataAcquisition();

    /**
     * @brief Handle error received by the sensors manager.
     * @param sensor
     * @param error
     */
    void onErrorReceived(const QString& sensor, const QString& message, SensorsManager::ESensorsManagerError error);

private:
    /**
     * @brief Main widget to handle logging.
     */
    ConsoleWidget* mConsole = nullptr;

    /**
     * @brief Main manager used to handle sensors.
     */
    SensorsManager* mSensorsManager = nullptr;

    /**
     * @brief Indicates whether a data acquisition is currently in progress or not.
     */
    bool mAcquisitionStarted = false;

    /**
     * @brief The centrala area where sub windows will be displayed.
     */
    QMdiArea* mMdiArea = nullptr;

    /**
     * @brief Sub window used to display frames from camera streaming.
     */
    ImageViewerSubWindow* mImageViewer = nullptr;

    /**
     * @brief Action visible in the toolbar to start and stop sensor acquisition.
     */
    QAction* mActionPlayStopAcquisition = nullptr;
};
#endif // MAINWINDOW_HPP

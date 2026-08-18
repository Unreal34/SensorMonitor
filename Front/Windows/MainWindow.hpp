#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "ConsoleWidget.hpp"
#include "SensorsManager.hpp"

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
     * @param sensor
     * @param data
     */
    void onDataReceived(const QString& sensor, const QByteArray& data);

    /**
     * @brief Called when the user wants to edit the sensors and their associated serial ports.
     */
    void openSensorsEditorDialog();

    /**
     * @brief Starts or stops data acquisition using the sensor data list saved in the main manager.
     */
    void toggleDataAcquisition();

    /**
     * @brief Handle error received by sensors manager.
     * @param sensor
     * @param error
     */
    void onErrorReceived(const QString& sensor, const QString& port, SensorsManager::ESensorsManagerError error);

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
     * @brief Indicates whether a data acquisition is currently in progress.
     */
    bool mAcquisitionStarted = false;
};
#endif // MAINWINDOW_HPP

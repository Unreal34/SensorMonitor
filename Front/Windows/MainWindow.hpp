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
    void initializeActions();

private slots:
    void onDataReceived(const QString& sensor, const QByteArray& data);
    void openSensorsEditorDialog();
    void toggleDataAcquisition();

private:
    ConsoleWidget* mConsole = nullptr;
    SensorsManager* mSensorsManager = nullptr;
    bool mAcquisitionStarted = false;
};
#endif // MAINWINDOW_HPP

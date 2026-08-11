#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "Console.hpp"
#include "SensorsManager.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void initializeSerialPortsMenu();

private slots:
    void onSerialSelected(QAction* action);
    void onDataReceived(const QString& sensor, const QByteArray& data);

private:
    Console* mConsole = nullptr;
    QMenu* mSerialPortsMenu = nullptr;
    SensorsManager* mSensorsManager;
};
#endif // MAINWINDOW_HPP

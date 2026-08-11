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
    void initializeTools();

private slots:
    void onSerialSelected(QAction* action);
    void onDataReceived(const QString& sensor, const QByteArray& data);
    void openSensorsEditorDialog();

private:
    Console* mConsole = nullptr;
    QMenu* mToolsMenu = nullptr;
    SensorsManager* mSensorsManager;
};
#endif // MAINWINDOW_HPP

#include "MainWindow.hpp"
#include "Application.hpp"
#include "Utility.hpp"
#include "SensorsEditorDialog.hpp"
#include "ApplicationLogger.hpp"

#include <QDockWidget>
#include <QSerialPortInfo>
#include <QMenuBar>
#include <QActionGroup>
#include <QToolBar>
#include <QMessageBox>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
, mConsole(new ConsoleWidget(this))
, mSensorsManager(new SensorsManager(this))
{
    setWindowIcon(QIcon(APPLICATION_ICON));
    setWindowTitle(APPLICATION_NAME_VERSION);
    setMinimumSize(QSize(1280, 720));

    QDockWidget* consoleDockWidget = new QDockWidget(tr("Console"), this);
    consoleDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    consoleDockWidget->setWidget(mConsole);
    consoleDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDockWidget);

    initializeActions();

    connect(mSensorsManager, &SensorsManager::dataReceived, this, &MainWindow::onDataReceived);
    connect(mSensorsManager, &SensorsManager::errorHandled, this, &MainWindow::onErrorReceived);

    setCentralWidget(new QWidget(this));
}

void MainWindow::initializeActions()
{
    QToolBar* mainToolBar = new QToolBar("Plugin toolbar", this);
    addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
    mainToolBar->setIconSize(QSize(32, 32));

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu* toolsMenu = menuBar()->addMenu(tr("&Tools"));

    QAction* actionEditSensors = new QAction(QIcon("://Icons/RS232.png"), tr("Manage sensors and ports"), this);
    QAction* actionPlayStopAcquisition = new QAction(QIcon("://Icons/Play.png"), tr("Start data acquisition"), this);
    QAction* actionExit = new QAction(QIcon::fromTheme("application-exit"), tr("Exit"), this);

    toolsMenu->addAction(actionPlayStopAcquisition);
    toolsMenu->addAction(actionEditSensors);
    mainToolBar->addAction(actionPlayStopAcquisition);
    fileMenu->addAction(actionExit);

    mainToolBar->addAction(actionEditSensors);

    connect(actionPlayStopAcquisition, &QAction::triggered, this, &MainWindow::toggleDataAcquisition);
    connect(actionEditSensors, &QAction::triggered, this, &MainWindow::openSerialSensorsEditorDialog);
    connect(actionExit, &QAction::triggered, qApp, &QApplication::quit);
}

void MainWindow::onDataReceived(const QString &sensor, const QByteArray &data)
{
    if(sensor == GEIGER_SENSOR)
    {
        bool bSuccess = false;
        uint cpm = data.toUInt(&bSuccess);

        if(bSuccess)
        {
            QString message = QString("CPM: %1").arg(cpm);
            mConsole->appendLog(message, ConsoleWidget::ELogType::Information);
        }
    }
    else if(sensor == HTU21D_SENSOR)
    {
        QList<QByteArray> split = data.split(';');

        if(split.size() == 2)
        {
            bool bSuccess = false;
            float temperature = split[0].toFloat(&bSuccess);
            Q_ASSERT(bSuccess);

            float humidity = split[1].toFloat(&bSuccess);
            Q_ASSERT(bSuccess);

            QString message = QString("Temperature: %1°c - Humidity: %2%").arg(temperature).arg(humidity);
            mConsole->appendLog(message, ConsoleWidget::ELogType::Information);
        }
    }
}

void MainWindow::openSerialSensorsEditorDialog()
{
    Q_ASSERT(mSensorsManager);

    bool bOk;
    SerialSensorsEditorDialog dialog(mSensorsManager->savedSerialSensorData(), &bOk, this);
    dialog.exec();

    if(bOk)
    {
        // Cache sensor data in the appropriate manager for later retrieval.
        sensorsManager()->setSavedSerialSensorsData(dialog.sensorDataList());
    }
}

void MainWindow::toggleDataAcquisition()
{
    QAction* action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);

    bool bValue = !mAcquisitionStarted;

    if(bValue)
    {
        if(sensorsManager()->savedSerialSensorData().size() <= 0)
        {
            QMessageBox::critical(this, APPLICATION_NAME, tr("No sensor available!\nPlase configure at least one sensor in the sensor editor tool."), QMessageBox::Ok);
            return;
        }

        Q_FOREACH(const SerialSensorData& current, sensorsManager()->savedSerialSensorData())
        {
            sensorsManager()->registerNewSerialSensor(current.sensor_portName, current.sensor_name);
            sensorsManager()->openSensor(current.sensor_name);
        }
    }
    else
    {
        sensorsManager()->clear();
    }

    mAcquisitionStarted = !mAcquisitionStarted;
    action->setIcon(mAcquisitionStarted ? QIcon("://Icons/Stop.png") : QIcon("://Icons/Play.png"));
    action->setText(mAcquisitionStarted ? tr("Stop data acquisition") : tr("Start data acquisition"));
}

void MainWindow::onErrorReceived(const QString &sensor, const QString& message, SensorsManager::ESensorsManagerError error)
{
    mConsole->appendLog(message, error == SensorsManager::Success ? ConsoleWidget::ELogType::Success : ConsoleWidget::ELogType::Error);

    // log message to Logs.txt file.
    ApplicationLogger::log(message, error == SensorsManager::Success ? ApplicationLogger::ELogType::Success : ApplicationLogger::ELogType::Error);
}

#include "MainWindow.hpp"
#include "Application.hpp"
#include "Sensors.hpp"
#include "SensorsEditorDialog.hpp"

#include <QDockWidget>
#include <QSerialPortInfo>
#include <QMenuBar>
#include <QActionGroup>
#include <QToolBar>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
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
    connect(actionEditSensors, &QAction::triggered, this, &MainWindow::openSensorsEditorDialog);
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

void MainWindow::openSensorsEditorDialog()
{
    Q_ASSERT(mSensorsManager);

    bool bOk;
    SensorsEditorDialog dialog(mSensorsManager->savedSensorData(), &bOk, this);
    dialog.exec();

    if(bOk)
    {
        // Cache sensor data in the appropriate manager for later retrieval.
        sensorsManager()->setSavedSensorsData(dialog.sensorDataList());
    }
}

void MainWindow::toggleDataAcquisition()
{
    QAction* action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);

    bool bValue = !mAcquisitionStarted;

    if(bValue)
    {
        if(sensorsManager()->savedSensorData().size() <= 0)
        {
            QMessageBox::critical(this, APPLICATION_NAME, tr("No sensor available!\nPlase configure at least one sensor in the sensor editor tool."), QMessageBox::Ok);
            return;
        }

        Q_FOREACH(const SensorData& current, sensorsManager()->savedSensorData())
        {
            SerialSensor* sensor = sensorsManager()->registerNewSensor(current.sensor_portName, current.sensor_name);
            Q_ASSERT(sensor);

            if (sensor->isAvailable())
            {
                QString message = QString("Using serial port %1 for sensor %2").arg(sensor->serialPortName(), sensor->name());
                mConsole->appendLog(message, ConsoleWidget::ELogType::Success);
            }
            else
            {
                QString message = QString("Unable to open serial port %1 for sensor %2").arg(sensor->serialPortName(), sensor->name());
                mConsole->appendLog(message, ConsoleWidget::ELogType::Error);
            }
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

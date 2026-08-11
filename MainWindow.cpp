#include "MainWindow.hpp"

#include <QDockWidget>
#include <QSerialPortInfo>
#include <QMenuBar>
#include <QActionGroup>

#define GEIGER_SENSOR "Geiger"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mConsole(new Console(this))
    , mSensorsManager(new SensorsManager(this))
{
    setWindowIcon(QIcon(":/Icons/RS232.png"));
    setMinimumSize(QSize(1280, 720));

    QDockWidget* consoleDockWidget = new QDockWidget(tr("Console"), this);
    consoleDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    consoleDockWidget->setWidget(mConsole);
    consoleDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDockWidget);

    initializeSerialPortsMenu();

    connect(mSensorsManager, &SensorsManager::dataReceived, this, &MainWindow::onDataReceived);
}

void MainWindow::initializeSerialPortsMenu()
{
    mSerialPortsMenu = menuBar()->addMenu(tr("&Serial Ports"));

    QActionGroup* group = new QActionGroup(this);
    group->setExclusive(true);

    for (const QSerialPortInfo &portInfo : QSerialPortInfo::availablePorts())
    {
        QAction* actionPort = new QAction(portInfo.portName(), this);
        actionPort->setIcon(QIcon("://Icons/RS232.png"));
        actionPort->setCheckable(true);
        mSerialPortsMenu->addAction(actionPort);
        group->addAction(actionPort);
    }

    connect(group, &QActionGroup::triggered, this, &MainWindow::onSerialSelected);
}

void MainWindow::onSerialSelected(QAction* action)
{
    Q_ASSERT(action);

    if(action->isChecked())
    {
        if(mSensorsManager->exists(GEIGER_SENSOR))
        {
            mSensorsManager->deleteSensorByTag(GEIGER_SENSOR);
        }

        SerialSensor* sensor = mSensorsManager->addNewSensor(action->text(), GEIGER_SENSOR);
        Q_ASSERT(sensor);

        if (sensor->isOnline())
        {
            QString message = QString("Using serial port: %1").arg(sensor->serialPortName());
            mConsole->appendLog(message, Console::ELogType::Success);
        }
        else
        {
            QString message = QString("Unable to open serial port: %1").arg(sensor->serialPortName());
            mConsole->appendLog(message, Console::ELogType::Error);
            action->setChecked(false);
        }
    }
}

void MainWindow::onDataReceived(const QString &sensor, const QByteArray &data)
{
    if(sensor == GEIGER_SENSOR)
    {
        bool bSuccess = false;
        uint cpm = data.toUInt(&bSuccess);
        Q_ASSERT(bSuccess);

        QString message = QString("CPM: %1").arg(cpm);
        mConsole->appendLog(message, Console::ELogType::Information);
    }
}

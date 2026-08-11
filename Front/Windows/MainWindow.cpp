#include "MainWindow.hpp"
#include "Application.hpp"
#include "SensorsEditorDialog.hpp"

#include <QDockWidget>
#include <QSerialPortInfo>
#include <QMenuBar>
#include <QActionGroup>

#define GEIGER_SENSOR "Geiger"
#define TEMP_SENSOR "Temperature"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mConsole(new Console(this))
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

    initializeTools();

    connect(mSensorsManager, &SensorsManager::dataReceived, this, &MainWindow::onDataReceived);
}

void MainWindow::initializeTools()
{
    mToolsMenu = menuBar()->addMenu(tr("&Tools"));

    QAction* actionEditSensors = new QAction(QIcon("://Icons/RS232.png"), tr("Manage sensors and port"), this);
    connect(actionEditSensors, &QAction::triggered, this, &MainWindow::openSensorsEditorDialog);
    mToolsMenu->addAction(actionEditSensors);

    /*QActionGroup* group = new QActionGroup(this);
    group->setExclusive(true);

    for (const QSerialPortInfo &portInfo : QSerialPortInfo::availablePorts())
    {
        QAction* actionPort = new QAction(portInfo.portName(), this);
        actionPort->setIcon(QIcon("://Icons/RS232.png"));
        actionPort->setCheckable(true);
        mSerialPortsMenu->addAction(actionPort);
        group->addAction(actionPort);
    }

    connect(group, &QActionGroup::triggered, this, &MainWindow::onSerialSelected);*/
}

void MainWindow::onSerialSelected(QAction* action)
{
    Q_ASSERT(action);

    if(action->isChecked())
    {
        if(mSensorsManager->exists(GEIGER_SENSOR))
        {
            mSensorsManager->deleteSensorByName(GEIGER_SENSOR);
        }

        SerialSensor* sensor = mSensorsManager->addNewSensor(action->text(), GEIGER_SENSOR);
        Q_ASSERT(sensor);

        if (sensor->isAvailable())
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

void MainWindow::openSensorsEditorDialog()
{
    bool bOk;
    SensorsEditorDialog dialog(&bOk, this);
    dialog.exec();

    /*if(bOk && dialog.getLicensesToBulkImport().size() > 0)
    {
        emit bulkLicensesImportRequest(dialog.getLicensesToBulkImport());
    }*/
}

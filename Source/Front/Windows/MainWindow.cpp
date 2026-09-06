#include "MainWindow.hpp"
#include "Back/Utility/Application.hpp"
#include "Back/Utility/Utility.hpp"
#include "Front/Dialogs/SerialSensorsEditorDialog.hpp"
#include "Back/Utility/ApplicationLogger.hpp"
#include "Front/Dialogs/UdpSensorsEditorDialog.hpp"
#include <Back/Utility/SensorUtility.hpp>
#include <Back/Objects/Udp_ESP32Camera.hpp>

#include <QDockWidget>
#include <QSerialPortInfo>
#include <QMenuBar>
#include <QActionGroup>
#include <QToolBar>
#include <QMessageBox>
#include <QApplication>
#include <QLabel>
#include <QImage>
#include <QDesktopServices>
#include <qtextedit.h>
#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
, mConsole(new ConsoleWidget(this))
, mSensorsManager(new SensorsManager(this))
, mMdiArea(new QMdiArea(this))
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

    setCentralWidget(mMdiArea);
}

void MainWindow::initializeActions()
{
    QToolBar* mainToolBar = new QToolBar("Plugin toolbar", this);
    addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
    mainToolBar->setIconSize(QSize(32, 32));

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu* toolsMenu = menuBar()->addMenu(tr("&Tools"));

    QAction* actionEditSerialSensors = new QAction(QIcon("://Icons/RS232.png"), tr("Manage serial sensors"), this);
    QAction* actionEditUdpSensors = new QAction(QIcon("://Icons/Ethernet.png"), tr("Manage udp sensors"), this);
    mActionPlayStopAcquisition = new QAction(QIcon("://Icons/Play.png"), tr("Start data acquisition"), this);
    QAction* actionExit = new QAction(QIcon::fromTheme("application-exit"), tr("Exit"), this);

    toolsMenu->addAction(mActionPlayStopAcquisition);
    toolsMenu->addAction(actionEditSerialSensors);
    toolsMenu->addAction(actionEditUdpSensors);
    mainToolBar->addAction(mActionPlayStopAcquisition);
    fileMenu->addAction(actionExit);

    mainToolBar->addAction(actionEditSerialSensors);
    mainToolBar->addAction(actionEditUdpSensors);

    connect(mActionPlayStopAcquisition, &QAction::triggered, this, &MainWindow::toggleDataAcquisition);
    connect(actionEditSerialSensors, &QAction::triggered, this, &MainWindow::openSerialSensorsEditorDialog);
    connect(actionEditUdpSensors, &QAction::triggered, this, &MainWindow::openUdpSensorsEditorDialog);
    connect(actionExit, &QAction::triggered, qApp, &QApplication::quit);
}

void MainWindow::onDataReceived(const QString &sensor, const QByteArray &data)
{
    if(sensor.contains(GEIGER_SENSOR))
    {
        bool bSuccess = false;
        uint cpm = data.toUInt(&bSuccess);

        if(bSuccess)
        {
            QString message = QString("CPM: %1").arg(cpm);
            mConsole->appendLog(message, ConsoleWidget::ELogType::Information);
        }
    }
    else if(sensor.contains(HTU21D_SENSOR))
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
    else if(sensor.contains(OV7670_CAMERA) || sensor.contains(ESP32_CAMERA))
    {
        QImage frame;

        if(sensor.contains(OV7670_CAMERA))
        {
            frame = SensorUtility::createGrayscaleImage(data, 80, 60);
        }
        else if(sensor.contains(ESP32_CAMERA))
        {
            bool bSuccess = frame.loadFromData(data, "JPG");
            Q_ASSERT(bSuccess);
        }

        if(!mImageViewer)
        {
            mImageViewer = new ImageViewerSubWindow(this);
            mImageViewer->setWindowTitle(sensor);
            mMdiArea->addSubWindow(mImageViewer);
            mImageViewer->show();

            // reset the pointer to nullptr if the sub window is deleted during closing.
            // end the acquisition too.
            connect(mImageViewer, &ImageViewerSubWindow::imageViewerCloseRequest, this, [this]()
            {
                mImageViewer = nullptr;

                if(mAcquisitionStarted)
                {
                    toggleDataAcquisition();
                }
            });
        }

        mConsole->appendLog(tr("New frame received from camera."), ConsoleWidget::ELogType::Information);
        mImageViewer->setImage(frame);
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
        // Cache serial sensor data in the appropriate manager for later retrieval.
        sensorsManager()->setSavedSerialSensorsData(dialog.sensorDataList());
    }
}

void MainWindow::openUdpSensorsEditorDialog()
{
    Q_ASSERT(mSensorsManager);

    bool bOk;
    UdpSensorsEditorDialog dialog(mSensorsManager->savedUdpSensorData(), &bOk, this);
    dialog.exec();

    if(bOk)
    {
        // Cache udp sensor data in the appropriate manager for later retrieval.
        sensorsManager()->setSavedUdpSensorsData(dialog.sensorDataList());
    }
}

void MainWindow::toggleDataAcquisition()
{
    Q_ASSERT(mActionPlayStopAcquisition);

    bool bValue = !mAcquisitionStarted;

    if(bValue)
    {
        SensorsManager::ESensorsManagerError error = sensorsManager()->registerAndOpenSensorsFromSavedData();

        if(error == SensorsManager::ESensorsManagerError::EmptySavedBuffer)
        {
            QMessageBox::critical(this, APPLICATION_NAME, tr("No sensor available!\nPlease configure at least one sensor in one of the sensor editor tool."), QMessageBox::Ok);
        }

        if(error == SensorsManager::ESensorsManagerError::SensorRegistrationError)
        {
            QMessageBox::critical(this, APPLICATION_NAME, tr("At least one sensor triggered an error during sensor registration step.\nAborting..."), QMessageBox::Ok);
        }

        if(error == SensorsManager::ESensorsManagerError::SensorOpeningError)
        {
            QMessageBox::critical(this, APPLICATION_NAME, tr("At least one sensor triggered an error during sensor opening step.\nAborting..."), QMessageBox::Ok);
        }

        if(error != SensorsManager::ESensorsManagerError::Success)
        {
            return;
        }
    }

    mAcquisitionStarted = !mAcquisitionStarted;
    mActionPlayStopAcquisition->setIcon(mAcquisitionStarted ? QIcon("://Icons/Stop.png") : QIcon("://Icons/Play.png"));
    mActionPlayStopAcquisition->setText(mAcquisitionStarted ? tr("Stop data acquisition") : tr("Start data acquisition"));

    // mAcquisitionStarted state must be updated before mImageViewer destruction.
    if(!mAcquisitionStarted)
    {
        sensorsManager()->clear();

        if(mImageViewer)
        {
            delete mImageViewer;
            mImageViewer = nullptr;
        }
    }
}

void MainWindow::onErrorReceived(const QString &sensor, const QString& message, SensorsManager::ESensorsManagerError error)
{
    mConsole->appendLog(message, error == SensorsManager::Success ? ConsoleWidget::ELogType::Success : ConsoleWidget::ELogType::Error);

    // log message to Logs.txt file.
    ApplicationLogger::log(message, error == SensorsManager::Success ? ApplicationLogger::ELogType::Success : ApplicationLogger::ELogType::Error);
}

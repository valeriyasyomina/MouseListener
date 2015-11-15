#include "mainwindow.h"
#include "ui_mainwindow.h"

static const int portNumber = 8000;
static std::string deviceFileName = "/sys/devices/platform/vms/coordinates";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionStop_mouse_listening->setEnabled(false);
    try
    {
        QDesktopWidget desktop;
        QSize screenSize = desktop.geometry().size();
        Singleton::Instance().GetFacade()->GetMouseListener()->SetScreenSize(screenSize);

        connect(Singleton::Instance().GetFacade(),
                SIGNAL(ServerStartedSignal()), this, SLOT(ServerStarted()));
        connect(Singleton::Instance().GetFacade(),
                SIGNAL(ServerStoppedSignal()), this, SLOT(ServerStopped()));
        connect(Singleton::Instance().GetFacade(),
                SIGNAL(ErrorSendCommandToDeviceSignal()), this, SLOT(ErrorSendCommandToDevice()));
        connect(Singleton::Instance().GetFacade(),
                SIGNAL(ClientConnectedSignal(QString,int)), this, SLOT(ClientConnected(QString,int)));
        connect(Singleton::Instance().GetFacade(),
                SIGNAL(ClientDisconnectedSignal(QString,int)), this, SLOT(ClientDisconnected(QString,int)));
    }
    catch (Exception& exception)
    {}
}

MainWindow::~MainWindow()
{
    try
    {
        Singleton::Instance().GetFacade()->GetDeviceManager()->RemoveDeviceModule();
        Singleton::Instance().GetFacade()->GetDeviceManager()->CloseDeviceFile();
    }
    catch (Exception& exception)
    {}
    delete ui;
}

void MainWindow::on_actionRemove_device_module_triggered()
{
    try
    {
        Singleton::Instance().GetFacade()->GetDeviceManager()->RemoveDeviceModule();
        Singleton::Instance().GetFacade()->GetDeviceManager()->CloseDeviceFile();
        QMessageBox::information(this, "Remove module", "Kernel module was successfully removed!", QMessageBox::Ok);
    }
    catch (Exception& exception)
    {
        QMessageBox::information(this, "Remove module", exception.GetMessage(), QMessageBox::Ok);
    }
}

void MainWindow::on_actionStart_mouse_listening_triggered()
{
    try
    {
        Singleton::Instance().GetFacade()->GetMouseListener()->SetPortNumber(portNumber);
        Singleton::Instance().GetFacade()->GetMouseListener()->StartListen();
        ui->actionRemove_device_module->setEnabled(false);
        ui->actionLoad_device_module->setEnabled(false);
        ui->actionStart_mouse_listening->setEnabled(false);
        ui->actionStop_mouse_listening->setEnabled(true);
    }
    catch (Exception& exception)
    {
        QMessageBox::information(this, "Start listen", exception.GetMessage(), QMessageBox::Ok);
    }
}

void MainWindow::on_actionLoad_device_module_triggered()
{
    try
    {
        QString fileName = QFileDialog::getOpenFileName(this,tr("Open kernel module"), "../KernelModule", tr("Kernel module files (*.ko)"));
        Singleton::Instance().GetFacade()->GetDeviceManager()->InsertDeviceModule(fileName.toStdString());
        Singleton::Instance().GetFacade()->GetDeviceManager()->OpenDeviceFile(deviceFileName);
        QMessageBox::information(this, "Insert module", "Kernel module was successfully inserted!", QMessageBox::Ok);
    }
    catch (Exception& exception)
    {
        QMessageBox::information(this, "Insert module", exception.GetMessage(), QMessageBox::Ok);
    }
}


void MainWindow::on_actionHelp_triggered()
{
    QMessageBox::information(this, "Help", QString("1. To load device kernel module choose Service->Load device kernal module ") +
                             QString("and than choose your module *.ko\n") +
                             QString("2. To remove device module from kernel choose Service->Remove device kernal module\n") +
                             QString("3. To run server choose Service->Start mouse listening\n") +
                             QString("4. To stop server choose Service->Stop mouse listening\n"), QMessageBox::Ok);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About program", QString("This program represents a server for mobile device.\n") +
                             QString("Mobile device connected to server can control PC mouse movement and left button press\n"),
                             QMessageBox::Ok);}


void MainWindow::on_actionStop_mouse_listening_triggered()
{
    Singleton::Instance().GetFacade()->GetMouseListener()->StopListen();    
    ui->actionLoad_device_module->setEnabled(true);
    ui->actionRemove_device_module->setEnabled(true);
    ui->actionStart_mouse_listening->setEnabled(true);
    ui->actionStop_mouse_listening->setEnabled(false);
}

void MainWindow::ServerStarted()
{   
    QMessageBox::information(this, "Start listen", "Server was successfully started!", QMessageBox::Ok);
}

void MainWindow::ServerStopped()
{
    QMessageBox::information(this, "Stop listen", "Server was successfully stopped!", QMessageBox::Ok);
}

void MainWindow::ErrorSendCommandToDevice()
{
    QMessageBox::information(this, "Error send command to device",
                             "Maybe kernel module was not loaded! Try to do it", QMessageBox::Ok);
}

void MainWindow::ClientConnected(QString clientAddress, int clientPort)
{
    ui->lstClients->addItem(QString("Client ") + clientAddress + QString(":") +
                            QString::number(clientPort) + QString(" connected\n"));
}

void MainWindow::ClientDisconnected(QString clientAddress, int clientPort)
{
    ui->lstClients->addItem(QString("Client ") + clientAddress + QString(":") +
                            QString::number(clientPort) + QString(" disconnected\n"));
}

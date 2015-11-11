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
        QMessageBox::information(this, "Start listen", "Server was successfully started!", QMessageBox::Ok);
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

void MainWindow::on_pushButton_clicked()
{
    try
    {
        Singleton::Instance().GetFacade()->GetDeviceManager()->SendCommandToDevice("7 480 730");

    }
    catch (Exception& exception)
    {
        QMessageBox::information(this, "send command", exception.GetMessage(), QMessageBox::Ok);
    }
}


void MainWindow::on_actionHelp_triggered()
{
    QMessageBox::information(this, "Help", QString("1. To load device kernel module choose File->load device kernal module ") +
                             QString("and than choose your module *.ko\n") +
                             QString("2. To remove device module from kernel choose File->remove device kernal module\n") +
                             QString("3. To run server choose File->start mouse listening\n"), QMessageBox::Ok);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About program", QString("This program represents a server for mobile device.\n") +
                             QString("Mobile device connected to server can control PC mouse movement and left button press\n"),
                             QMessageBox::Ok);}


void MainWindow::on_actionStop_mouse_listening_triggered()
{
    Singleton::Instance().GetFacade()->GetMouseListener()->StopListen();
    QMessageBox::information(this, "Stop listen", "Server was successfully stopped!", QMessageBox::Ok);
    ui->actionLoad_device_module->setEnabled(true);
    ui->actionRemove_device_module->setEnabled(true);
    ui->actionStart_mouse_listening->setEnabled(true);
    ui->actionStop_mouse_listening->setEnabled(false);
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

static const int portNumber = 8000;
static std::string deviceFileName = "/sys/devices/platform/vms/coordinates";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
     //   QCursor::setPos(1440, 900);
     //   sleep(2);
       // Singleton::Instance().GetFacade()->GetDeviceManager()->SendCommandToDevice("0 50 100");
        //sleep(2);
        //Singleton::Instance().GetFacade()->GetDeviceManager()->SendCommandToDevice("0 100 30");
        //sleep(2);
    }
    catch (Exception& exception)
    {
        QMessageBox::information(this, "send command", exception.GetMessage(), QMessageBox::Ok);
    }
}


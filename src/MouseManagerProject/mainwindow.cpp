#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    Singleton::Instance().GetFacade()->GetMouseListener()->SetPortNumber(8000);
    Singleton::Instance().GetFacade()->GetMouseListener()->StartListen();
}

void MainWindow::on_actionLoad_device_module_triggered()
{
    try
    {
        QString fileName = QFileDialog::getOpenFileName(this,tr("Open kernel module"), "../KernelModule", tr("Kernel module files (*.ko)"));
        Singleton::Instance().GetFacade()->GetDeviceManager()->InsertDeviceModule(fileName.toStdString());
        Singleton::Instance().GetFacade()->GetDeviceManager()->OpenDeviceFile("/sys/devices/platform/vms/coordinates");
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
    Singleton::Instance().GetFacade()->GetDeviceManager()->SendCommandToDevice("10 30");
    }
    catch (Exception& exception)
    {
        QMessageBox::information(this, "send command", exception.GetMessage(), QMessageBox::Ok);
    }
}

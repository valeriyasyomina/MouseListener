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
    delete ui;
}

void MainWindow::on_actionRemove_device_module_triggered()
{

}

void MainWindow::on_actionStart_mouse_listening_triggered()
{
    Singleton::Instance().GetFacade()->GetMouseListener()->SetPortNumber(8000);
    Singleton::Instance().GetFacade()->GetMouseListener()->StartListen();
}

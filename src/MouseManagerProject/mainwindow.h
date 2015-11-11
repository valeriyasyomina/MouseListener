#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QObject>
#include "Singleton/Singleton.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionRemove_device_module_triggered();

    void on_actionStart_mouse_listening_triggered();

    void on_actionLoad_device_module_triggered();  

    void on_actionHelp_triggered();

    void on_actionAbout_triggered();

    void on_actionStop_mouse_listening_triggered();

private:
    Ui::MainWindow *ui;
public slots:
    void ServerStarted(QString serverAddress, int serverPort);
    void ServerStopped();
    void ErrorSendCommandToDevice();
};

#endif // MAINWINDOW_H

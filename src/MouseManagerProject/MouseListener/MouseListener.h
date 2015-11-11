#ifndef MOUSELISTENER_H
#define MOUSELISTENER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

static const int MAX_BUFFER_SIZE = 64;
static const int DEFAULT_SCREEN_HEIGHT = 600;
static const int DEFAULT_SCREEN_WIDTH = 800;

class MouseListener: public QObject
{
    Q_OBJECT
private:
     QTcpServer* server;
     QTcpSocket* socket;
     int portNumber;
     int screenWidth;
     int screenHeight;
     bool deviceKernelModuleLoaded;
public:
    MouseListener();
    ~MouseListener();

    void StartListen();
    void StopListen();
    void SetPortNumber(int port);
    void SetScreenSize(const QSize& screenSize);
public slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

    void DeviceKernelModuleInserted();
    void DeviceKernelModuleRemoved();
signals:
    void MessageReceived(const char* message);
    void ServerStartedSignal(QString serverAddress, int serverPort);
};

#endif // MOUSELISTENER_H

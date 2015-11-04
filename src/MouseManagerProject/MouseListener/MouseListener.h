#ifndef MOUSELISTENER_H
#define MOUSELISTENER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "Exception/ServerListenException.h"
#include "Exception/SocketReadDataException.h"

static const int MAX_BUFFER_SIZE = 64;

class MouseListener: public QObject
{
    Q_OBJECT
private:
     QTcpServer* server;
     QTcpSocket* socket;
     int portNumber;
public:
    MouseListener();
    ~MouseListener();

    void StartListen();
    void SetPortNumber(int port);
public slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
signals:
    void MessageReceived(const char* message);
};

#endif // MOUSELISTENER_H

#include "MouseListener.h"
#include "Exception/AllocMemoryException.h"
#include "Exception/ServerListenException.h"
#include "Exception/SocketReadDataException.h"
#include "Exception/ErrorInputDataException.h"
#include <QSize>
#include <new>

MouseListener::MouseListener()
{
    try
    {
        server = new QTcpServer(this);
        connect(server, SIGNAL(newConnection()),this, SLOT(onNewConnection()));
        portNumber = 0;
        screenHeight = DEFAULT_SCREEN_HEIGHT;
        screenWidth = DEFAULT_SCREEN_WIDTH;
        qDebug()<< "ML ctor";
    }
    catch (std::bad_alloc& exception)
    {
        throw new AllocMemoryException("Error memory alloc MouseListener::MouseListener()");
    }
}

MouseListener::~MouseListener()
{
    if (server)
    {
        delete server;
        server = NULL;
    }
}

///
/// \brief MouseListener::SetScreenSize Устанавливает размер экрана
/// \param screenSize Размер экрана
///
void MouseListener::SetScreenSize(const QSize& screenSize)
{
    if (screenSize.width() <= 0 || screenSize.height() <= 0)
        throw ErrorInputDataException("Error screen width or height in SetScreenSize!");
    screenHeight = screenSize.height();
    screenWidth = screenSize.width();
}

///
/// \brief MouseListener::StartListen   Начинает прослушивать соединения по установленному адресу
///
void MouseListener::StartListen()
{
    if (!server->isListening())
    {
        if (!server->listen(QHostAddress::Any, portNumber))
            throw ServerListenException("Error server start listen!");
        qDebug()<< "Started";
    }
}

///
/// \brief MouseListener::SetPortNumber  Устанавливает номер порта
/// \param port Номер порта
///
void MouseListener::SetPortNumber(int port)
{
    portNumber = port;
}

///
/// \brief MouseListener::onNewConnection  Вызывается при появлении нового подключенного клиента
///
void MouseListener::onNewConnection()
{
    qDebug()<< "server: New connection";
    socket = server->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    if(socket->state() == QTcpSocket::ConnectedState)
    {
        qDebug()<<socket->peerPort();
    }
   // char msg[] = "Hello from server";
    //socket->write(msg, strlen(msg));
}

///
/// \brief MouseListener::onReadyRead   Вызывается, когда на сокете появляются данные дл чтения
///
void MouseListener::onReadyRead()
{
    char socketData[MAX_BUFFER_SIZE];
    qDebug()<< "server: read";
    int numberOfBytes = socket->read(socketData, MAX_BUFFER_SIZE);
    if (numberOfBytes <= 0)
        throw SocketReadDataException("Error read data from socket");
    socketData[numberOfBytes] = '\0';
    qDebug() << "Client sended to me:";
    qDebug() << socketData;
    qDebug() << "\n";
    emit MessageReceived(socketData);
}


///
/// \brief MouseListener::onDisconnected    Вызывается, когда клиент завершает соединение
///
void MouseListener::onDisconnected()
{
    qDebug()<< "disconnct";
    disconnect(socket, SIGNAL(disconnected()));
    disconnect(socket, SIGNAL(readyRead()));
    socket->deleteLater();
}

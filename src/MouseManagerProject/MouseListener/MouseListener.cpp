#include "MouseListener.h"
#include "Exception/AllocMemoryException.h"
#include <new>

MouseListener::MouseListener()
{
    try
    {
        server = new QTcpServer(this);
        connect(server, SIGNAL(newConnection()),this, SLOT(onNewConnection()));
        portNumber = 0;
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

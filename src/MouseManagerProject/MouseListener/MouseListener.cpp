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
        TCPServer = new QTcpServer(this);
        connect(TCPServer, SIGNAL(newConnection()),this, SLOT(onNewConnection()));
        TCPPortNumber = 0;
        UDPPortNumber = 0;
        screenHeight = DEFAULT_SCREEN_HEIGHT;
        screenWidth = DEFAULT_SCREEN_WIDTH;
        deviceKernelModuleLoaded = false;

        UDPServer = new QUdpSocket(this);
        connect(UDPServer, SIGNAL(readyRead()),this, SLOT(onReadyReadDatagram()));
    }
    catch (std::bad_alloc& exception)
    {
        throw new AllocMemoryException("Error memory alloc MouseListener::MouseListener()");
    }
}

MouseListener::~MouseListener()
{
    if (TCPServer)
    {
        TCPServer->close();
        delete TCPServer;
        TCPServer = NULL;
    }
    if (UDPServer)
    {
        UDPServer->close();
        delete UDPServer;
        UDPServer = NULL;
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
    if (!TCPServer->isListening())
    {
        if (!deviceKernelModuleLoaded)
            throw ServerListenException("You must load kernel module before start listening!");
        if (!TCPServer->listen(QHostAddress::Any, TCPPortNumber))
            throw ServerListenException("Error server start listening!");
        if (!UDPServer->bind(QHostAddress::Any, UDPPortNumber))
            throw ServerListenException("Error bind server!");

        emit ServerStartedSignal();
    }
}

///
/// \brief MouseListener::StopListen Останавливает прослушивать по заданному ранее адресу
///
void MouseListener::StopListen()
{
    TCPServer->close();
    UDPServer->close();
    emit ServerStoppedSignal();
}

///
/// \brief MouseListener::SetPortNumber  Устанавливает номер порта для TCP подключения
/// \param port Номер порта
///
void MouseListener::SetTCPPortNumber(int port)
{
    TCPPortNumber = port;
}

///
/// \brief MouseListener::SetUDPPortNumber Устанавливает номер порта для UDP подключения
/// \param port Номер порта
///
void MouseListener::SetUDPPortNumber(int port)
{
    UDPPortNumber = port;
}

///
/// \brief MouseListener::onNewConnection  Вызывается при появлении нового подключенного клиента
///
void MouseListener::onNewConnection()
{
    TCPClient = TCPServer->nextPendingConnection();

    connect(TCPClient, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    QString serverData = QString::number(UDPPortNumber) + " " + QString::number(screenWidth) + " " + QString::number(screenHeight);
    TCPClient->write(serverData.toUtf8().data(), strlen(serverData.toUtf8().data()));

    emit ClientConnectedSignal(TCPClient->peerAddress().toString(), TCPClient->peerPort());
}

///
/// \brief MouseListener::onReadyRead   Вызывается, когда на UDP сокете появляются данные дл чтения
///
void MouseListener::onReadyReadDatagram()
{
    QByteArray socketData;
    socketData.resize(UDPServer->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    int numberOfBytes = UDPServer->readDatagram(socketData.data(), socketData.size(),
                            &sender, &senderPort);
    if (numberOfBytes <= 0)
        throw SocketReadDataException("Error read data from socket");

    emit MessageReceivedSignal(socketData);
}

///
/// \brief MouseListener::onDisconnected    Вызывается, когда клиент завершает соединение
///
void MouseListener::onDisconnected()
{   
    disconnect(TCPClient, SIGNAL(disconnected()));

    emit ClientDisconnectedSignal(TCPClient->peerAddress().toString(), TCPClient->peerPort());
    TCPClient->deleteLater();
}

///
/// \brief MouseListener::DeviceKernelModuleInserted Вызывается, когда пользователь загружает модуль ядра
///
void MouseListener::DeviceKernelModuleInserted()
{
    deviceKernelModuleLoaded = true;
}

///
/// \brief MouseListener::DeviceKernelModuleRemoved Вызывается, когда пользователь выгружает модуль ядра
///
void MouseListener::DeviceKernelModuleRemoved()
{
    deviceKernelModuleLoaded = false;
}

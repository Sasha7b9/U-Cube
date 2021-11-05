#pragma once

#include "cdefines.h"

#include <cstdint>
#include <functional>
#include <thread>
#include <list>

#ifdef _WIN32

#include <WinSock2.h>

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#endif


//����� ��� ����� ������ �� �������
static constexpr uint16 buffer_size = 4096;


struct TcpServer
{
    class Client;
    
    //��� Callback-������� ����������� �������
    typedef std::function<void(Client)> FuncCallback;
    
    //������ �������
    enum class status : uint8
    {
        up                   = 0,
        err_socket_init      = 1,
        err_socket_bind      = 2,
        err_socket_listening = 3,
        close                = 4
    };

private:
    uint16       port;
    status       _status = status::close;
    FuncCallback handler;

    std::thread                handlerThread;
    std::list<std::thread>     clientHandlerThreads;
    std::list<std::thread::id> clientHandlingEnd;

#ifdef _WIN32
    SOCKET  servSocket = INVALID_SOCKET;
    WSAData wData;
#else
    int servSocket;
#endif

    // ������� ��������� ����������
    void HandlingLoop();

public:
    //����������� ���������:
    //port - ���� �� ������� ����� ��������� ������
    //handler - callback-�������, ����������� ��� ����������� �������, ������ �������� � �������� ������ ���������� � callback (������ ������-�������: [](TcpServer::Client){...do something...})
    TcpServer(const uint16 port, FuncCallback handler) : port(port), handler(handler) {};
    ~TcpServer();

    //����� callback-������� ����������� ��� ����������� �������
    void SetHandler(FuncCallback h)             { handler = h; };

    uint16_t GetPort() const                    { return port; };
    uint16_t SetPort(const uint16_t port);

    status GetStatus() const { return _status; }

    //���������� �������
    status Restart();

    //������ �������
    status Start();

    //��������� �������
    void   Stop();

    // ���� � ����� ��������� ����������
    void   JoinLoop()                           {  handlerThread.join(); };
};


class TcpServer::Client
{
#ifdef _WIN32

    SOCKET      socket;
    SOCKADDR_IN address;
    char        buffer[buffer_size];

public:

    Client(SOCKET socket, SOCKADDR_IN address) : socket(socket), address(address) {}
#else

    int    socket;
    struct sockaddr_in address;
    char   buffer[buffer_size];

public:
    Client(int socket, struct sockaddr_in address) : socket(socket), address(address) {}

#endif

public:
    Client(const Client &other) : socket(other.socket), address(other.address) {}
    ~Client();

    //��������� � ����� ������ �� ������� � ���������� �� ������
    int   LoadData()    { return recv(socket, buffer, buffer_size, 0); };

    //���������� ��������� �� ����� � ������� �� �������
    char *GetData()     { return buffer; };

    //���������� ������ �������
    bool  SendData(const char *buffer, const size_t size) const;

#ifdef _WIN32

    uint   GetHost() const { return address.sin_addr.S_un.S_addr; };
    uint16 GetPort() const { return address.sin_port; };

#else

    uint   GetHost() const { return address.sin_addr.s_addr; }
    uint16 GetPort() const { return address.sin_port; }

#endif
};

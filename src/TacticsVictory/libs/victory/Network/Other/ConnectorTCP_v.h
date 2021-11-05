// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Network/Other/NetworkTypes_v.h"
#include <map>
#include <mutex>
#include <thread>
#include <queue>


// ������ �������
class BaseConnectorTCP
{
public:

    virtual ~BaseConnectorTCP();

    void Release();

    bool Connect(const std::string &host, uint16 port);

    void SetReadTimeOut(uint timeout);

    void SetWriteTimeOut(uint timeout);

    void Disconnect();

    bool IsConnected() const;

    // ������� size ���� �� ������� data
    void Transmit(const void *data, uint size);

    // ������ ������� ����� size ���� � ������ data. ���������� ���������� ������� �������� ����.
    // ���� size == 0, �� ������ �� ������, � ���������� ���������� ��������� ��� ������ ����
    ssize_t Receive(void *data, uint size);

    static void ThreadConnect(BaseConnectorTCP *conn_out, pchar host, uint16 port, std::mutex *mutex, uint8 *state);

private:

    std::unique_ptr<sockpp::tcp_connector>  connection;
};


/*
 *   ����� ����� ConnectorTCP �������������� �������������� � �������� �� ������� �������
 */

class ConnectorTCP
{
public:

    void Init(pchar _host, uint16 _port)
    {
        host = _host;
        port = _port;
    };

    void SetCallbacks(pFuncVV connection, pFuncVV disconnection, pFuncVV fail);

    void RunCycle();

    void Connect();

    void Disconnect();

    void Update();

    void Destroy();

    // ������� �������. ������ ������ ���������� ���������� 32-� ������ ���������������, ������� ��������� ��������
    // ������� � ������ �������.
    uint SendRequest(pchar request, const void *data = nullptr, uint size = 0);
    uint SendRequest(pchar request, pchar data);

    bool IsConnected();

    // ��������� ������ ����� �������� ���������� �������
    void SetTask(ServerTask *task);

    // �������� ��������� ������
    void RunTask(ServerTask *task);

    struct State { enum E {
        Idle,                   // �������
        NeedConnection,         // ����� ����������
        AttemptConnection,      // � �������� ����������
        EventConnection,        // ��������� ����������, ����� �������� ������� funcConnection
        EventFailConnection,    // ��������� ����������, ����� �������� ������� funcFailConnection
        InConnection,           // ����������
        EventDisconnect,        // ������� ������ ����� � ������-��������
        GetPing                 // ������� ����
    }; };

    std::vector<uint8> data;                        // ����� �������� �������� ������

    bool thread_need_stopped = false;               // true, ���� ����� ��������� � �������
    bool thread_is_stopped = true;                  // true, ���� ����� ����������

    static void ThreadUpdate(ConnectorTCP *connector);

    std::string GetFullAddress() const;

private:

    BaseConnectorTCP connector;                     // ����� �������� ������ � ������

    std::string host;
    uint16 port;                                    // ���� ������� � ������. ���� ������ �� 1 ������

    std::map<uint, ServerTask *> wait_tasks;        // ������, ��������� ������ (�����������)
    std::vector<ServerTask *>    new_tasks;         // ������, ��������� ���������� (������������ � ������� ���
                                                    // ����������

    bool destroy = false;
    std::mutex  mutex;                      // ������ mutex ����� ��������, ���� ������ ��������� � �������� ����������

    pFuncVV funcFailConnection = nullptr;   // ���������� � ������ ���������� ������� ����������
    pFuncVV funcConnection     = nullptr;   // ���������� � ������ �������� ������� ����������
    pFuncVV funcDisconnection  = nullptr;   // ���������� ��� ������ ����� � ��������

    std::unique_ptr<std::thread> thread_update = nullptr;

    State::E state = State::Idle;

    uint last_request_id = 0;

    // ��������� ��������� �������
    void ExecuteTasks();

    // ������� ��������� ������
    void ReceiveData();

    // ���������� �������� ������
    void ProcessData();

    // ���������� false, ���� ���������� � �������� ��������
    bool ExistConnection();
};

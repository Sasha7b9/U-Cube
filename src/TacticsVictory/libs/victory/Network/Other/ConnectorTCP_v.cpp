// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Network/Other/NetworkTypes_v.h"
#include "Network/Other/ConnectorTCP_v.h"
#include "Utils/GlobalFunctions_.h"
#include <climits>
#include <thread>


BaseConnectorTCP::~BaseConnectorTCP()
{
    Release();
}


bool BaseConnectorTCP::Connect(const std::string &host, uint16 port)
{
    static sockpp::socket_initializer sock_init;

    if (!connection)
    {
        connection = std::make_unique<sockpp::tcp_connector>();
    }

    try
    {
        volatile in_addr_t addr = sockpp::inet_address().resolve_name(host);
        addr = addr;
    }
    catch (std::runtime_error &error)
    {
        LOGERRORF("%s:%d %s", host.c_str(), port, error.what());

        return false;
    }

    connection->connect({ host, port });

    if (connection->is_connected())
    {
        SetReadTimeOut(1);
        SetWriteTimeOut(1);

//        LOGWRITEF("Connect to %s:%d success", host.c_str(), port);
    }
    else
    {
        //        LOGERRORF("%d - Connect to %s:%d failed. Error : %s", counter++, host.c_str(), port,
        //            connection->last_error_str().c_str());
    }

    return connection->is_connected();
}


void BaseConnectorTCP::SetReadTimeOut(uint timeout)
{
    connection->read_timeout((std::chrono::milliseconds)timeout);
}


void BaseConnectorTCP::SetWriteTimeOut(uint timeout)
{
    connection->write_timeout((std::chrono::milliseconds)timeout);
}


void BaseConnectorTCP::Release()
{
    Disconnect();
    connection.reset();
}


void BaseConnectorTCP::Disconnect()
{
    if (connection)
    {
        LOGWRITE("Disconnect");

        connection->close();
    }
}


void BaseConnectorTCP::Transmit(const void *data, uint size)
{
    connection->write_n(data, size);
}


ssize_t BaseConnectorTCP::Receive(void *data, uint size)
{
    fd_set set = { 1, { connection->handle() } };

#ifdef WIN32
    TIMEVAL time = { 0, 1000 };
#else
    struct timeval time = { 1, 0 };
#endif

    int ready = ::select(1, &set, 0, 0, &time);

    if (ready == 1)
    {
        ssize_t num_bytes = connection->read(data, size);

        return num_bytes;
    }

    return 0;
}


bool BaseConnectorTCP::IsConnected() const
{
    if (!connection)
    {
        return false;
    }

    if (!connection->is_open())
    {
        return false;
    }

    if (!connection->is_connected())
    {
        return false;
    }

    return true;
}


void BaseConnectorTCP::ThreadConnect(BaseConnectorTCP *conn_out, pchar host, uint16 port, std::mutex *mutex, uint8 *state)
{
    mutex->lock();

    if (conn_out->Connect(host, port))
    {
        *state = ConnectorTCP::State::EventConnection;
    }
    else
    {
        *state = ConnectorTCP::State::EventFailConnection;
    }

    mutex->unlock();
}


void ConnectorTCP::ThreadUpdate(ConnectorTCP *connector)
{
    connector->thread_is_stopped = false;

    while (!connector->thread_need_stopped)
    {
        connector->Update();
    }

    connector->thread_is_stopped = true;
}


void ConnectorTCP::Destroy()
{
    destroy = true;

    mutex.lock();           // ������� ���������� ThreadConnect
    mutex.unlock();

    thread_need_stopped = true;

    while (!thread_is_stopped) { }

    connector.Release();
}


bool ConnectorTCP::IsConnected()
{
    return (state == State::InConnection);
}


void ConnectorTCP::SetCallbacks(pFuncVV connection, pFuncVV disconnection, pFuncVV fail)
{
    funcFailConnection = fail;
    funcConnection = connection;
    funcDisconnection = disconnection;

    RunCycle();
}


void ConnectorTCP::RunCycle()
{
    if (thread_update == nullptr)
    {
        thread_update = std::make_unique<std::thread>(ConnectorTCP::ThreadUpdate, this);
    }

    thread_need_stopped = true;

    while (!thread_is_stopped)  { }

    thread_need_stopped = false;

    thread_update->detach();
}


void ConnectorTCP::Connect()
{
    if (!funcFailConnection || !funcConnection || !funcDisconnection)
    {
        LOGERROR("Callbacks not defined");
        return;
    }

    if (host.empty())
    {
        LOGERRORF("Not specified address master server");

        return;
    }

    if (state == State::Idle)
    {
        state = State::NeedConnection;
    }
}


void ConnectorTCP::Disconnect()
{
    if (IsConnected())
    {
        mutex.lock();
        connector.Disconnect();
        mutex.unlock();

        state = State::EventDisconnect;
    }
}


uint ConnectorTCP::SendRequest(pchar request, const void *buffer, uint size_buffer)
{
    mutex.lock();

    last_request_id++;

    connector.Transmit(&last_request_id, 4);

    uint size_full = (uint)std::strlen(request) + size_buffer + 1;

    connector.Transmit(&size_full, 4);

    connector.Transmit(request, (uint)std::strlen(request) + 1);

    if (buffer)
    {
        connector.Transmit(buffer, size_buffer);
    }

    mutex.unlock();

    return last_request_id;
}


uint ConnectorTCP::SendRequest(pchar request, pchar _data)
{
    return SendRequest(request, _data, (uint)std::strlen(_data) + 1);
}


void ConnectorTCP::Update()
{
    ReceiveData();

    ProcessData();

    if (!ExistConnection())
    {
        Disconnect();
    }

    switch (state)
    {
    case State::Idle:
        break;

    case State::NeedConnection:
        {
        static int64 prev_connection = 0;

            if (!destroy)
            {
                if ((GF::Timer::TimeMS() - prev_connection > 2000) &&
                    mutex.try_lock())
                {
                    mutex.unlock();
                    state = State::AttemptConnection;
                    prev_connection = GF::Timer::TimeMS();
                    std::thread thread(BaseConnectorTCP::ThreadConnect, &connector, std::move(host.c_str()), port, &mutex, (uint8 *)&state);
                    thread.detach();
                }
            }
        }
        break;

    case State::AttemptConnection:
        break;

    case State::EventConnection:
        state = State::InConnection;
        funcConnection();
        break;

    case State::EventFailConnection:
        state = State::Idle;
        funcFailConnection();
        break;

    case State::InConnection:
        ExecuteTasks();
        break;

    case State::EventDisconnect:
        state = State::Idle;
        wait_tasks.clear();
        new_tasks.clear();
        funcDisconnection();
        break;

    case State::GetPing:
        state = State::InConnection;
        break;
    }
}



void ConnectorTCP::ReceiveData()
{
    if (!IsConnected())
    {
        return;
    }

    mutex.lock();

    static const int SIZE_CHUNK = 1024;

    uint8 buffer[SIZE_CHUNK];

    ssize_t received = connector.Receive(buffer, SIZE_CHUNK);

    while (received > 0)
    {
        data.insert(data.end(), buffer, buffer + received);

        received = connector.Receive(buffer, SIZE_CHUNK);
    }

    mutex.unlock();
}


void ConnectorTCP::ProcessData()
{
    while (data.size() > 4 + 4)        // ���� ������� ������ ������ ��� id � ���������� ���� � ���������
    {
        uint id = *((uint *)data.data());

        uint size_answer = *((uint *)(data.data() + 4));

        if (data.size() >= 4 + 4 + size_answer)
        {
            auto it = wait_tasks.find(id);

            if (it != wait_tasks.end())
            {
                pchar answer = (pchar)data.data() + 4 + 4;

                void *buffer = nullptr;

                uint size_buffer = 0;

                if (std::strlen(answer) + 1 < size_answer)
                {
                    buffer = data.data() + 4 + 4 + std::strlen(answer) + 1;

                    size_buffer = size_answer - (uint)std::strlen(answer) - 1;
                }

                ServerTask *task = it->second;

                task->handler_answer(answer, buffer, size_buffer);

                task->last_tive_receive = GF::Timer::TimeMS();

//                wait_tasks.erase(it);
            }
            else
            {
                LOGERRORF("Handler for id %d not found", id);
            }

            data.erase(data.begin(), data.begin() + 4 + 4 + size_answer);
        }
        else
        {
            break;          // ���� �� ������� ��� ����� ���������
        }
    }
}


void ConnectorTCP::ExecuteTasks()
{
    int64 now = GF::Timer::TimeMS();

    for (ServerTask *task : new_tasks)
    {
        if (now >= task->prev_time + task->delta_time)
        {
            uint id = task->request();

            task->prev_time = now;

            task->counter--;

            wait_tasks[id] = task;
        }
    }

    while (new_tasks.size() && ServerTask::ExistCompleted(new_tasks))
    {
        for (auto it = new_tasks.begin(); it < new_tasks.end(); it++)
        {
            if ((*it)->counter == 0)
            {
                new_tasks.erase(it);
                break;
            }
        }
    }
}


void ConnectorTCP::SetTask(ServerTask *task)
{
    task->last_tive_receive = LLONG_MAX;

    new_tasks.push_back(task);
}


void ConnectorTCP::RunTask(ServerTask *task)
{
    task->delta_time = -1;

    task->last_tive_receive = LLONG_MAX;

    task->counter = 1;

    new_tasks.push_back(task);
}


bool ConnectorTCP::ExistConnection()
{
    int64 now = GF::Timer::TimeMS();

    if (wait_tasks.size() == 0)
    {
        return true;
    }

    for (auto &it : wait_tasks)
    {
        ServerTask *task = it.second;

        if (now - task->last_tive_receive < 1500)
        {
            return true;
        }
    }

    return false;
}


std::string ConnectorTCP::GetFullAddress() const
{
    return host + ":" + std::to_string(port);
}

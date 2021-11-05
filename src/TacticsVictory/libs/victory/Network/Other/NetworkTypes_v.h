// 2021/04/07 20:38:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// �������������� ������-�������� �������
#define MSG_NTW_PING                "ping"                  // ������ �����
#define MSG_NTW_INFO_LIVINGROOM     "info_livingroom"       // ���� ���������� �������� �������� ������-������� �
                                                            // ���� ���������
#define MSG_NTW_SET_NAME_LIVINGROOM "set_name_livingrom"    // ��������� ����� ��������
                                                            // ������ �������� ����� �������� ������� ������������

#define MSG_NTW_GET                 "get"
#define MSG_NTW_GET_PORT_LIVINGROOM_BROADCAST_UDP MSG_NTW_GET " port livingroom_broadcast_udp"   // ����� ������-�������


struct ServerTask
{
    ServerTask(pFuncUV _request, pFuncpCpVU _handler_answer, int64 _delta_time = 1000, uint _counter = 0xFFFFFFFF)
        : request(_request), handler_answer(_handler_answer), delta_time(_delta_time),
        prev_time(0), last_tive_receive(0),  counter(_counter)
    { }

    pFuncUV    request = nullptr;           // ���������� �������. ������ ���������� id �������
    pFuncpCpVU handler_answer = nullptr;    // ���������� ������
    int64      delta_time = 0;              // ����� ����� ���������� ������� ��������� �������
    int64      prev_time = 0;
    int64      last_tive_receive = 0;
    uint       counter = 0xFFFFFFFF;        // ������� ��� ��������� ������

    // ���������� true, ���� ���������� ����������� ������� (��, � ������� ������� == 0)
    static bool ExistCompleted(std::vector<ServerTask *> &tasks);
};


struct SockAddrIn
{
    void Init(uint16 family, pchar ip, uint16 port);

    // ���������� ���������� �������� ����
    int RecvFrom(evutil_socket_t socket, char *buffer, int size_buffer);

    int Bind(evutil_socket_t socket);

    sockaddr *GetSockAddr();
    pchar GetIP();
    uint16 GetPort();

    void SetHostIP(void *ip);

    // ���������� ������ ����� ������� � ���� "host:port"
    std::string ToStringFull() const;

    // ���������� ������ ip-����� �������
    std::string ToStringHost() const;

    const sockaddr_in &GetSockAddrInConst() const { return addr; };

    sockaddr_in &GetSockAddrIn() {  return addr;  };

private:

    sockaddr_in addr = { 0 };
};


struct ClientMessage
{
    std::vector<uint8> raw;         // ����� �������� �������� ��������� - ������� ������, � ����� ��������������
                                    // ������, ���� ����. ����� ��������� � ������ �� �����������.
                                    // ����� � ������ �������������� � ����������� � ����������� ���������� �
                                    // MoveRawFrom()
    uint number = 0;
    uint size = 0;

    std::vector<uint8> data;        // ������ ���������, ���� ������� �������.
    std::vector<std::string> words; // �������� �� ����� ��������� ����� ���������

    // ���������� ����� ������� �� received � raw. ��� ���� �� ����������� ������� ������������ ������ ���������
    void MoveRawFrom(std::vector<uint8> &received);

    void *GetRawData();             // ���������� ��������� �� ������, ���� ������� ������� � ���������

    static uint GetID(std::vector<uint8> &received);

    static uint GetSize(std::vector<uint8> &received);
};


struct ClientInfo
{
    uint          id = 0;
    SockAddrIn    address;          // ����� �������
    void         *bevnt = nullptr;  // ����� ������� libevent
    ClientMessage message;
};

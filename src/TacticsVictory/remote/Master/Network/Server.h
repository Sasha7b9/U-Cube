// 2021/05/02 11:16:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Network/Other/ServerTCP_v.h"


struct ClientServerInfo : public ClientInfo
{
    ClientServerInfo() {}
    ClientServerInfo(const ClientInfo &);

    bool IsLivingRoom() const { return !name.empty(); }

    std::string     name;      // ��� ��������, ��� ��� ����� ������������ � ���� ������ ������� � �������

    static ClientServerInfo empty;
};


class Server : public ServerTCP
{
public:

    typedef void (*handlerClient) (uint, ClientServerInfo &);

    Server();

    virtual ~Server() {};

    std::map<uint, ClientServerInfo> clients;   // ������� ����� �������� �� ����� ���������� ID, ���������� �� ��������

    void AppendHandler(pchar command, handlerClient handler);

private:

    virtual void                HandlerOnAccepted(ClientInfo &info) override;
    virtual std::vector<uint8>& HandlerOnRead1(void *bevnt) override;
    virtual ClientInfo&         HandlerOnRead2(void *bevnt) override;
    virtual void                HandlerOnError(void *bevnt) override;

    // ���������� ������ � ������� bevnt
    ClientServerInfo &GetClient(void *bevnt);
};

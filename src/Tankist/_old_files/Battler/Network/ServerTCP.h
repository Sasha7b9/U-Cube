#pragma once
#include "SocketServerTCP.h"



class ServerTCP;

struct ServerParam
{
    pFuncVSpCU16    funcOnConnect;
    pFuncVS         funcOnDisconnect;
    pFuncVSU8pVI    funcOnRecieve;      // client ID, type message, data, size data
    uint16          port;
};



class ServerTCP
{
public:
    ServerTCP();

    bool Init(const ServerParam &servParam);
    void SendMessage(SOCKET numClient, uint8 typeMessage, void* data, uint size);
    void Close();

    ServerParam     param;

private:
    SocketServerTCP *socket;
    SocketParam     socketParam;
};

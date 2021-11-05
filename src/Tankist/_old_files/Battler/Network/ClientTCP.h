#pragma once
#include "SocketClientTCP.h"



class ClientTCP
{
public:
    ClientTCP() = default;

    bool Init(pFuncVU8pVI funcOnRecieve);
    bool Connect(const char *address, uint16 port);
    void SendMessage(uint8 numMessage, void* data, uint size);
    void Close();

    pFuncVU8pVI FuncOnRecieve;

private:
    SocketClientTCP socket;
};

// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*  ����� ��� ������������ � ��������
*/

class ServerC : public Object
{
    URHO3D_OBJECT(ServerC, Object);

public:

    ServerC(ServerC **self);

    void Connect(const String &address, uint16 port);

private:

    void HandleMessage(StringHash, VariantMap &);

    void HandleServerConnected(StringHash, VariantMap &);

    void HandleServerDisconnected(StringHash, VariantMap &);

    void HandleConnectFailed(StringHash, VariantMap &);

    void Connect();

    String address;
    uint16 port = 0;
};

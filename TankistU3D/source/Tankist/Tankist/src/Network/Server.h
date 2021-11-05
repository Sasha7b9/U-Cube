#pragma once
#include "Objects/Vehicle.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
URHO3D_EVENT(E_CLOSECONNECTION, CloseConnection)
{
    URHO3D_PARAM(P_CONNECT, Connect);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Server : public Object
{
    URHO3D_OBJECT(Server, Object)

public:
    Server(Context *context);

    void Start(unsigned short port);

    void SendStringMessage(Vehicle *tank, const String &var, const String &value);

private:
    void HandleCloseConnection(StringHash, VariantMap&);
    void HandleServerConnected(StringHash, VariantMap&);
    void HandleServerDisconnected(StringHash, VariantMap&);
    void HandleConnectFailed(StringHash, VariantMap&);
    void HandleNetworkMessage(StringHash, VariantMap&);
    void HandleClientConnected(StringHash, VariantMap&);
    void HandleClientDisconnected(StringHash, VariantMap&);
    void HandleClientIdentity(StringHash, VariantMap&);
    void HandleClientSceneLoaded(StringHash, VariantMap&);
    void HandleNetworkUpdate(StringHash, VariantMap&);
    void HandleNetworkUpdateSent(StringHash, VariantMap&);
    void HandleNetworkSceneLoadFailed(StringHash, VariantMap&);
    void HandleRemoteEventData(StringHash, VariantMap&);
    void HandleShoot(StringHash, VariantMap&);
    void HandlePhysicsPostStep(StringHash, VariantMap&);
    void HandleControlMessage(StringHash, VariantMap&);

    int numClients = 0;

    Vector<Vehicle*>       tanks;          // ��� ����� - �� ������ �� ������� �������
    Vector<Connection*> connections;    // � ��� ��������������� �� ���������� � ��������

    Server(Server const&) : Object(nullptr) {};
    Server operator=(Server const&) {};
};

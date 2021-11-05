#pragma once
#include "Network/ClientTCP.h"
#include "Network/ServerTCP.h"



class Chat : public Object
{
    URHO3D_OBJECT(Chat, Object)

public:

    enum Type
    {
        Chat_Client,
        Chat_Server
    };

    Chat(Context *context, Type type);

    // *** ������� ������� ***
    bool Listen(uint16 port);
    void SendToAll(uint8 typeMessage, const String &message);
    void SendToAllExcept(uint8 typeMessage, const String &message, SOCKET except);

    // *** ������� ������� ***
    bool Connect(const char *address, uint16 port);
    bool IsActive();
    void SetActive(bool active);
    void PressEnter();
    void AddMessage(const String &message);     // Show message on display
    void SendToServer(const String &message);
    void UpdateChat();
    void SendAudioData(void *buffer, uint sizeBuffer);

private:
    Type type;

    // *** ��� ������� ***
    ServerTCP server;

    // *** ��� ������� ***
    ClientTCP client;

    SharedPtr<Text> historyText;

    SharedPtr<LineEdit> messageEdit;

    Vector<String> messages;

    Chat(Chat const&) : Object(nullptr) {};
    Chat operator=(Chat const&) { return *this; };
};

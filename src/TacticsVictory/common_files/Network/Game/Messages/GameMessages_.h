// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Network/Game/Connection_.h"
#include "Network/Game/Messages/GameMessagesID_.h"


namespace Message
{

    struct Message
    {
        Message(int _id) : id(_id) {}
        void Handle(VariantMap &map);
        int id;
        VectorBuffer buffer;

        void Send(bool reliable);   // ����� ������������ ������ ��� �������. ������� ����� ��� �����, � ����� ���-
                                    // ����� ��������
    };


    struct RequestForLevel : public Message
    {
        RequestForLevel() : Message(SERV_REQUEST_FOR_LEVEL) {}

        void Handle(const ConnectionT &connection);
    };


    struct ReturnLevel : public Message
    {
        ReturnLevel();

        void Handle(MemoryBuffer &);
    };


    // �������� ��������� ������
    struct TextString : public Message
    {
        TextString(const String &message) : Message(SERV_TEXTSTRING)
        {
            buffer.WriteString(message);
        }

        void Handle(MemoryBuffer &msg)
        {
            URHO3D_LOGINFO(msg.ReadString());
        }
    };


    // �������/���������� ������
    struct KeyEvent : public Message
    {
        KeyEvent(Key key,                                       // ��� �������
            bool press) : Message(SERV_KEY_EVENT)               // true - �������, false, ����������
        {
            buffer.WriteInt(key);
            buffer.WriteBool(press);
        }

        void Handle(MemoryBuffer & /*msg*/) {};
    };


    struct CreateGameObject : public Message
    {
        CreateGameObject(uint idNode, const StringHash &hashTypeObject, const Vector3 &position,
            const Quaternion &rotation) :
            Message(CLNT_CREATE_GAME_OBJECT)
        {
            buffer.WriteUInt(idNode);
            buffer.WriteStringHash(hashTypeObject);
            buffer.WriteVector3(position);
            buffer.WriteQuaternion(rotation);
        }

        void Handle(MemoryBuffer &);
    };


    struct SendTankPosition : public Message
    {
        SendTankPosition(const String &name, const Vector3 &position) : Message(CLNT_SEND_TANK_POSITION)
        {
            buffer.WriteString(name);
            buffer.WriteVector3(position);
        }

        void Handle(MemoryBuffer &);
    };

    struct SendScene : public Message
    {
        SendScene();

        void Handle(MemoryBuffer &);
    };
}

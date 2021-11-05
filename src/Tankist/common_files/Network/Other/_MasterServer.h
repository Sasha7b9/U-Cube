// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Network/Other/_SocketsTCP.h"


/*
*   ����� MasterServer ������ ������������ ��� ����������, ������� ����� ������ � ������-�������
*/


#define MASTER_GET_ADDRESS_UPLOADER "get address uploader"      // ������� ����� uploader
#define MASTER_GET_ADDRESS_HANGAR   "get address hangar"        // ������� ����� hangar
#define MASTER_GET_ADDRESS_DATABASE "get address database"      // ������� ����� database
#define MASTER_GET_ADDRESS_BATTLE   "get address battle"        // ������� ����� ���
#define MASTER_CLOSE_CONNECTION     "close connection"          // �������������� �� master
#define MASTER_TERMINCATE           "terminate"                 // ��������� ���������� master


class MasterServer
{
public:

    // ������ ������� ������������� � �������
    void Connnect();

    void Destroy();

    std::string GetValue(pchar key);

private:

    ConnectorTCP connector;             // ����������� ��� ����� � �������� ������-��������

    bool destroy = false;
    std::mutex mutex;                   // ������ mutex ����� ��������, ���� ������ ��������� � �������� ����������
};

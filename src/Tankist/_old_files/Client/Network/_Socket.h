#pragma once
#include "cdefines.h"
#include "FileSystem.h"


class Socket
{
public:

    static const uint16 SERVER_PORT = 30001;
    static const char * const SERVER_ADDRESS;

    // �������������� � ��������� �������
    bool ConnectToServer();

    // ������������� �� �������
    void DisconnectFromServer();

    // ������� ������ �������
    void SendRequest(const char *request);

    // ������ ������ �����
    int RequestFileSize(const char *nameFile);

    // ��������� ����
    void RequestFile(const char *nameFile, uint8 *buffer, int size);

    // ������� ������ � ������ �� ��������� �������
    void ReceiveInfoAboutFiles(std::vector<FileInfo> &infos, const char *fileName);

private:

    uint64 my_sock = 0;

    // ������� ������ �� �������
    int GetSize();

    // ������� ���������� ���������� ����� �� ������
    void SplitFileForStrings(const uint8 *buffer, int sizeBuffer, std::vector<std::string> &strings);
};

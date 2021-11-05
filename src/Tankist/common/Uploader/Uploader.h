#pragma once
#include "Network/Other/_SocketsTCP.h"

#include <vector>
#include <string>


class Uploader
{
public:

    int Run();

private:

    // ����������� ������ ������ ��� ����������
    void PrepareListFiles();

    int RunServer();

    // �������� ������ ������������ ������
    void WriteListFiles(pchar name, std::vector<std::string> &ignored);
};

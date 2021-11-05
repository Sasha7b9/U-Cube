// 2021/04/08 22:06:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <string>


namespace GF
{
    namespace Timer
    {
        // �������� ��������� ������ ���������
        void TimeStart();

        // ���������� ����� � �������������, ��������� � ������� ������ TimeStart();
        int64 DeltaMS();

        // ������� ����� � �������������
        int64 TimeMS();
    };

    void DelayMS(uint delay);

    std::string IntToString(int value, uint length);

#ifdef U3D

    // ���������� � address a����, ���� � ������� ���� ������ ����
    // "-address:xxxx", � port - ����� �����, ���� � ������� ���� ������ ���� "-port:xx"
    bool GetAddressPort(const Vector<std::string> &words, std::string &address, uint16 &port);


    // Return the file's last modified time. File path relative from TVData
    unsigned GetLastModifiedTime(char *name);

    std::string GetNameFile(pchar name);

#ifdef GRAPHICS

    void SetWindowInCenterScreen(Window *window);

    void OpenFileSelector(pchar title, pchar textOk, pchar textCancel, const Vector<std::string> &filters);

#endif

#endif
}

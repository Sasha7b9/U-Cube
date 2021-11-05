/* (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by */
#pragma once


/*
*   ���������� ��������� �� ���������� ������
*/


extern Context          *TheContext;        // ���������� ��������
extern FileSystem       *TheFileSystem;     // ���������� ��� ������ � �������� �������
extern Log              *TheLog;            // ���������� ������������
extern Network          *TheNetwork;        // ������� �������
extern ResourceCache    *TheCache;          // ���������� �������� � �����������
extern Time             *TheTime;           // ��������� �������

extern Audio            *TheAudio;          // �������� ����������
extern Console          *TheConsole;        // �������
extern DebugHud         *TheDebugHud;       // ���������� ���������
extern DebugRenderer    *TheDebugRenderer;  // ���������� ����������� �������
extern Graphics         *TheGraphics;       // ��������� �� ����������� ����������
extern Localization     *TheLocale;         // ���������� �����������
extern PhysicsWorld     *ThePhysicsWorld;   // ���������� ���������� ���������
extern Renderer         *TheRenderer;       // ��������� �� ������
extern UI               *TheUI;             // ��������� �� ����������� ����������
extern UIElement        *TheUIRoot;         // �������� ������� ��� (����������� ��������� ������������ - GUI)

extern String           TheIPAddress;
extern unsigned short   TheNumPort;

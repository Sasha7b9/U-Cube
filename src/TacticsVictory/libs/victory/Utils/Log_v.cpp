// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "FileSystem/FileSystem_v.h"
#include "Utils/StringUtils_v.h"
#include "Utils/UtilsOS_v.h"
#include <cstdarg>
#include <iostream>
#include <mutex>


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable: 4996)  // warning C4996: 'strcat': This function or variable may be unsafe. Consider using strcat_s instead.
#endif


static const int num_symbols_for_info = 55; // ���������� ������� ��� ���������� �� ������ (����, ������ � �.�)

pchar LogRAW::STR_ERROR   =   " ... ERROR ... ";
pchar LogRAW::STR_WARNING = " ... WARNING ... ";

static FS::File *outFile;

#ifdef WIN32
HANDLE ConsoleLog::handle = nullptr;
#define END_LINE "\x0d\x0a"
#else
#define END_LINE "\n"
#endif



static std::string CurrentTime()
{
    time_t now = time(0);

    char buf[80];

#ifdef WIN32
#pragma warning(push, 0)
#endif

    struct tm tstruct = *localtime(&now);

#ifdef WIN32
#pragma warning(pop)
#endif

    strftime(buf, sizeof(buf), "%X", &tstruct);

    return buf;
}


// ��������� �� ����� ����� �� ����� ������������ ���������� ��������
static pchar ExtractName(pchar fullName)
{
    const int max = num_symbols_for_info - 12;

    pchar pointer = fullName + SU::Length(fullName) - 1;

    int numSlash = 0;

    int length = 1;

    while (numSlash < 2 && length < max)
    {
        if (*pointer == '\\' || *pointer == '/')
        {
            numSlash++;
        }

        pointer--;
        length++;

        if (length == max)
        {
            break;
        }
    }

    return pointer + 2;
}


void LogRAW::Create(pchar nameLog, bool new_console)
{
    outFile = new FS::File();

    if (new_console)
    {
        ConsoleLog::Create();
    }

    outFile->Create((std::string("log") + FS::delimiter + nameLog).c_str());
}

void LogRAW::Destroy()
{
    ConsoleLog::Destroy();

    delete outFile;
}


void LogRAW::WriteF(pchar file, int line, pchar warn_err, pchar format, ...)
{
    std::string v;
    v.reserve(1024);

    CommonWriteF(file, line, v, warn_err);

    std::va_list args;
    va_start(args, format);

    char buffer[1024];

    std::vsnprintf(buffer, 1024, format, args);

    v.append(buffer);

    va_end(args);

    Write(v.data());
}


void LogRAW::Write(pchar file, int line, pchar text)
{
    CommonWrite(file, line, text, "");
}


void LogRAW::Warning(pchar file, int line, pchar text)
{
    CommonWrite(file, line, text, STR_WARNING);
}


void LogRAW::Error(pchar file, int line, pchar text)
{
    CommonWrite(file, line, text, STR_ERROR);
}


void LogRAW::CommonWrite(pchar file, int line, pchar text, pchar warn_err)
{
    std::string v;
    v.reserve(1024);

    v.append(CurrentTime().c_str());
    v.append(" | ");
    v.append(NameApplication());
    v.append(" | ");
    v.append(ExtractName(file));
    v.append(":");
    v.append(std::to_string(line));

    if (warn_err[0])
    {
        v.append(warn_err);
        v.append(text);
    }
    else
    {
        while (v.length() < num_symbols_for_info)
        {
            v.append(" ");
        }

        v.append("| ");
        v.append(text);
    }

    Write(v.c_str());
}


void LogRAW::CommonWriteF(pchar file, int line, std::string &v, pchar warn_err)
{
    v.append(CurrentTime().c_str());
    v.append(" | ");
    v.append(NameApplication());
    v.append(" | ");
    v.append(ExtractName(file));
    v.append(":");
    v.append(std::to_string(line));

    if (warn_err[0])
    {
        v.append(warn_err);
    }
    else
    {
        while (v.length() < num_symbols_for_info)
        {
            v.append(" ");
        }

        v.append("| ");
    }
}


void LogRAW::Write(pchar text)
{
    static std::mutex mutex;

    mutex.lock();

    if (outFile->IsOpened())
    {
        *outFile << text << END_LINE;
    }

    ConsoleLog::Write(text);

    mutex.unlock();
}


LogRAW::~LogRAW()
{
    LOGWRITE("Close application");
}

#ifdef WIN32
#pragma warning(pop)
#endif


void ConsoleLog::Create()
{
#ifdef WIN32

    FreeConsole();

    if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
    {
        /*
        *  ����� � ������� �� ���������� ���������� ������ ������� ����, ����� ������������� ����� Lucida Console
        */

        //        win::SetConsoleCP(1251);

        //        if (!SetConsoleOutputCP(1251))
        //        {
        //            DISPLAY_LAST_ERROR();
        //        }

        if (!AllocConsole())
        {
            DISPLAY_LAST_ERROR();
        }

        win::SetConsoleCP(1251);

        if (!SetConsoleOutputCP(1251))
        {
            DISPLAY_LAST_ERROR();
        }

        handle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (handle == INVALID_HANDLE_VALUE)
        {
            DISPLAY_LAST_ERROR();
        }

        RECT rect;

        HWND hwnd = GetConsoleWindow();

        if (!GetWindowRect(hwnd, &rect))
        {
            DISPLAY_LAST_ERROR();
        }

        if (!MoveWindow(hwnd, 0, 0, rect.right - rect.left, rect.bottom - rect.top, true))
        {
            DISPLAY_LAST_ERROR();
        }
    }

#endif
}


void ConsoleLog::Destroy()
{
#ifdef WIN32
    FreeConsole();
#endif
}


void ConsoleLog::Write(pchar message)
{
#ifdef WIN32

    if (handle)
    {
        if (!WriteConsoleA(handle, (void *)message, (DWORD)std::strlen(message), NULL, NULL))
        {
            DISPLAY_LAST_ERROR();
        }

        if (!WriteConsoleA(handle, "\n", 1, NULL, NULL))
        {
            DISPLAY_LAST_ERROR();
        }
    }
    else

#endif

    {
        std::cout << message << "\n";
    }
}

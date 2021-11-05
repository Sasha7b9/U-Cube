#pragma once
#include <vector>
#include <string>


// �������������� ���������.
#define LOGWRITE(x)      LogRAW::Write(__FILE__, __LINE__, x)
#define LOGWRITEF(...)   LogRAW::WriteF(__FILE__, __LINE__, "", __VA_ARGS__)

// ����������� ������
#define LOGWARNING(x)    LogRAW::Warning(__FILE__, __LINE__, x);
#define LOGWARNINGF(...) LogRAW::WriteF(__FILE__, __LINE__, LogRAW::STR_WARNING, __VA_ARGS__);

// ��������� ������.
#define LOGERROR(x)      LogRAW::Error(__FILE__, __LINE__, x)
#define LOGERRORF(...)   LogRAW::WriteF(__FILE__, __LINE__, LogRAW::STR_ERROR,  __VA_ARGS__)

#define LOG_FUNC_ENTER() LogRAW::WriteF(__FILE__, __LINE__, ">>>>>>>>> %s", __FUNCTION__)


class LogRAW
{
public:

    // ���� create_console == true, �� ����� ��������� ����� �������
    static void Create(pchar nameLog, bool create_console);

    static void Destroy();

    //+V576,class:Log,function:Write,format_arg:3,ellipsis_arg:4
    static void WriteF(pchar file, int line, pchar warn_err, pchar format, ...);
    static void Write(pchar file, int line, pchar text);
    static void Write(pchar text);

    //+V576,class:Log,function:Warning,format_arg:3,ellipsis_arg:4
    static void Warning(pchar file, int line, pchar text);

    //+V576,class:Log,function:Error,format_arg:3,ellipsis_arg:4
    static void Error(pchar file, int line, pchar text);

    static pchar STR_ERROR;
    static pchar STR_WARNING;

private:
    virtual ~LogRAW();

    // � ������, ���� ����� ��������� �������� ������ (ERROR, WARNING, etc..) ����� ��� ������ ���������
    static void CommonWrite(pchar file, int line, pchar text, pchar warn_err);
    static void CommonWriteF(pchar file, int line, std::string &v, pchar warn_err);

    static std::string NameApplication();
};


struct ConsoleLog
{
    static void Create();

    static void Write(pchar message);

    static void Destroy();

#ifdef WIN32
    static HANDLE handle;
#endif
};

#pragma once
#include "defines.h"
#include <string>
#include <list>
#include <chrono>


struct FileInfo
{
    std::string name;
    size_t      size; //-V122
    uint        crc;
    bool        processed;  // ���� false - ���� �� ���������

    FileInfo() : name(), size(0), crc(0), processed(false) {};
    FileInfo(pchar _name) : FileInfo() { name = _name; };
    FileInfo(const FileInfo &);
    bool operator==(const FileInfo &info);
    FileInfo &operator=(const FileInfo &);
    virtual ~FileInfo();
};


struct ListFiles
{
    ListFiles()
    {
        for (int i = 0; i < maxThreads; i++)
        {
            runThread[i] = false;
        }
    }

    ListFiles(pchar directory, std::vector<std::string> *ignoredFiles = nullptr, std::vector<std::string> *ignoredExtensions = nullptr)
    {
        for (int i = 0; i < maxThreads; i++)
        {
            runThread[i] = false;
        }

        Build(directory, ignoredFiles, ignoredExtensions);
    }

    std::list<FileInfo> files;

    // ��������� ������ ������� ������ �� �������� directory
    void Build(pchar directory, std::vector<std::string> *ignoredFiles = nullptr, std::vector<std::string> *ignoredExtensions = nullptr);

    // ��������� ������ ������� �� �����
    bool ExtractFiles(pchar fileName);

    // ���������� true, ���� � ������ ���������� ���� � ������ file
    bool ExistFile(const std::string &file) const;

    // ���������� true, ���� � ������ ���������� ���� �� ����������, ���������������� info
    bool ExistEqualFile(const FileInfo &info) const;

    // ������ ������ ������ � ����
    void Write(pchar fileName);

private:

    void AppendSubDirectory(pchar directory, std::vector<std::string> *ignoredFiles = nullptr, std::vector<std::string> *ignoredExtensions = nullptr);

    void AppendFile(std::string &fullName, const std::vector<std::string> *ignoredFiles = nullptr, const std::vector<std::string> *ignoredExtensions = nullptr);

    // ���������� true, ���� fullName ������������� �� ���� �� ���������� �� ������ ignoredExtensions
    bool ExtensionIs(std::string &fullName, const std::vector<std::string> *ignoredExtensions);

    // ���������� true, ���� fullName ���� ���� �� ��������, ������������� � ignoredFiles
    bool FileIs(std::string &fullName, const std::vector<std::string> *ignoredFiles);

    // ���������� ���������� ����������� � ������ ������ �������
    int NumThreads();

    static const int maxThreads = 8;        // ������������ ����� �������

    bool runThread[maxThreads];             // ������������� � true �������� ��������, ��� ������ ����� ��������� � ������

    std::chrono::steady_clock::time_point startBuild;                 // ����� ������ ���������� ������ ������

    std::chrono::seconds prevSeconds = std::chrono::seconds::zero();
};


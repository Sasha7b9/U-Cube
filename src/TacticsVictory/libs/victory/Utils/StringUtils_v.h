#pragma once


namespace SU
{
    char *Strcat(char *dest, pchar source);

    void SplitToWords(pchar text, std::vector<std::string> &out);

    // ������� text �� �����, ���������� ��������� �� ������� symbols
    void SplitToWords(pchar text, std::vector<std::string> &out, pchar symbols);

    int Length(pchar str);

    int Compare(pchar str1, pchar str2);

    void ReplaceSymbols(std::string &string, char old, char young);

    // ���������� true, ���� ������ string ���������� � ������������������ �������� symbols
    bool BeginWith(pchar string, pchar symbols);

    bool VerifyOnMask(pchar string, pchar mask);


}

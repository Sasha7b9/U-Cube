// 2021/03/16 21:50:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Menu/MenuPage_.h"


class PagePlay : public MenuPage
{
    URHO3D_OBJECT(PagePlay, MenuPage);

public:
    PagePlay();

private:

    SharedPtr<ButtonT> buttonFindServer;    // "����� ������"
    SharedPtr<ButtonT> buttonCreateServer;  // "������� ������"
    SharedPtr<ButtonT> buttonBack;          // ��������� � ���������� ����

    void HandleButtonRelease(StringHash, VariantMap &);
};

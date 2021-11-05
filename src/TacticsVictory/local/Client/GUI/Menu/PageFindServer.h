// 2021/03/26 23:38:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/OrderedTable_.h"
#include "GUI/Controls/Buttons/Button_.h"
#include "GUI/Menu/MenuPage_.h"


class PageFindServer : public MenuPage
{
    URHO3D_OBJECT(PageFindServer, MenuPage);

public:

    PageFindServer();

    // ���� ��������� ������ ������, ���������� �������
    void SetServersInfo(const std::string &data);

private:

    SharedPtr<OrderedTable> tableServers;
    SharedPtr<ButtonT>      buttonConnect;
    SharedPtr<ButtonT>      buttonBack;

    void HandleButtonRelease(StringHash, VariantMap &);
};

// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/Control_.h"


/*
    ������ ������ - �� ��� ����������� ������
*/
class PanelBottom : public Control
{
    URHO3D_OBJECT(PanelBottom, Control);

public:
    PanelBottom(Context *);
    static void RegisterObject();
};

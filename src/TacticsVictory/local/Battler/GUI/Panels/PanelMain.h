// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/Control_.h"


class Tab;


/*
    �������� ������� ������ � ���������
*/
class PanelMain : public Control
{
    URHO3D_OBJECT(PanelMain, Control);

public:
    PanelMain(Context *);
    static void RegisterObject();

    virtual void Update(float dT);
    void AddTab(SharedPtr<Tab> tab);

private:
    Vector<SharedPtr<Tab>> tabs;
    SharedPtr<Tab> currentTab;

    void HandleToggedTitle(StringHash eventType, VariantMap &eventData);
};

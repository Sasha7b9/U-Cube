// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "GUI/Controls/Buttons/ButtonSwitch_.h"
#include "GUI/Menu/MenuConfirmExit_.h"
#include "GUI/Menu/MenuGame.h"


MenuConfirmExit::MenuConfirmExit() : MenuPage()
{
    SetLayout(LM_VERTICAL, 0, IntRect(6, 6, 6, 6));
    SharedPtr<Label> label(Label::Create("Exit in OS?"));
    AddChild(label);

    SharedPtr<UIElement> layer(CreateChild<UIElement>());
    layer->SetLayout(LM_HORIZONTAL, 6, IntRect(6, 6, 6, 6));
    
    buttonOk = new ButtonT(layer, "Ok");
    buttonCancel = new ButtonT(layer, "Cancel");

    SubscribeToEvent(buttonOk, E_RELEASED, URHO3D_HANDLER(MenuConfirmExit, HandleButtonRelease));
    SubscribeToEvent(buttonCancel, E_RELEASED, URHO3D_HANDLER(MenuConfirmExit, HandleButtonRelease));

    AddChild(layer);
}


void MenuConfirmExit::HandleButtonRelease(StringHash, VariantMap& eventData)
{
    Button *button = (Button*)eventData[Released::P_ELEMENT].GetPtr();

    if (button == buttonOk)
    {
        OnPressButtonOk();
    }
    else if (button == buttonCancel)
    {
        OnPressButtonCancel();
    }
}


void MenuConfirmExit::OnPressButtonOk()
{
    VariantMap eventData = GetEventDataMap();
    eventData[MenuEvent::P_TYPE] = ME_EXIT_IN_OS;
    SendEvent(E_MENU, eventData);
}


void MenuConfirmExit::OnPressButtonCancel()
{
    TheMenuConfirmExit->SetVisible(false);
    TheMenuMain->SetVisible(true);
}

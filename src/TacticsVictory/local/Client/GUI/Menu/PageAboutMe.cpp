// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "GUI/Controls/Buttons/ButtonSwitch_.h"
#include "GUI/Menu/PageAboutMe.h"


PageAboutMe::PageAboutMe() : MenuPage()
{
    const int width = 216;
    const int height = 130;
    const int widthButton = 100;
    const int heightButton = 20;

    SetSize(width, height);

    AddLabel("Alexandr Shevchenko", 20, 20, 200, 40);
    AddLabel("e-mail Sasha7b9@tut.by", 20, 50, 200, 40);

    int x = width / 2 - widthButton / 2;
    int y = height - 2 * heightButton;

    buttonOk = AddButton("Ok", x, y, widthButton, heightButton);

    buttons.Push(buttonOk);

    SubscribeToEvent(buttonOk, E_RELEASED, URHO3D_HANDLER(PageAboutMe, HandleButtonRelease));

    SetMovable(false);
}


void PageAboutMe::HandleButtonRelease(StringHash, VariantMap&)
{
    SendEventClose();
}

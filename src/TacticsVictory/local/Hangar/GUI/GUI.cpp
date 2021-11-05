// 2021/04/12 22:58:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"


GUI::GUI(GUI **self) : GUIT((GUIT **)self)
{
    Create();

    *self = this;
}


void GUI::Create()
{
    TheConsole = new ConsoleT();
    TheUIRoot->AddChild(TheConsole);

    TheChat = new Chat();
    TheUIRoot->AddChild(TheChat);

    TheCursor = TheUIRoot->CreateChild<CursorT>("CursorT");

    TheLocalization->SetLanguage(TheSettings.GetInt("language") == 0 ? "en" : "ru");
}

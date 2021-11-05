/* (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by */
#include <stdafx.h>
#include "GameGUI.h"
 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GameGUI::GameGUI(Context *context) : Object(context)
{
    window = gUIRoot->CreateChild<Text>();
    window->SetStyleAuto();
    window->SetFontSize(20);
    window->SetPosition(0, gUIRoot->GetHeight() - 200);
    window->SetColor(Color::BLACK);

    SubscribeToEvent(E_NEW_VAR, URHO3D_HANDLER(GameGUI, HandleNewVar));

    window->SetText(gLocale->Get("tSpeed"));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void GameGUI::HandleNewVar(StringHash, VariantMap&)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void GameGUI::SetVar(const String &var, const String &value)
{
    window->SetText(var + " : " + value);
}

// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "FileSystem/ConfigurationFile_v.h"
#include "GUI/Cursor_.h"
#include "GUI/GUIEvents_.h"
#include "GUI/Controls/Hint_.h"
#include "GUI/Controls/Label_.h"
#include "GUI/Controls/Buttons/ButtonSwitch_.h"


ButtonT::ButtonT(Context *context) :
    Button(context)
{
    SetStyle("MainMenuButton");

    label = Label::Create("", TheSettings.GetInt("menu", "font", "size", "item"));
    AddChild(label);
}


ButtonT::ButtonT(UIElement *uielement, pchar text, int width /* = -1 */, int height /* = -1 */) :
    Button(TheContext)
{
    SetStyleAuto(TheUIRoot->GetDefaultStyle());
    SetStyle("MainMenuButton");
    label = Label::Create(text, TheSettings.GetInt("menu", "font", "size", "item"));
    label->SetAlignment(HA_CENTER, VA_CENTER);
    AddChild(label);

    if (uielement)
    {
        uielement->AddChild(this);
    }

    if (width == -1 && height == -1)
    {

    }
    else if (width == -1)
    {
        SetFixedHeight(height);
    }
    else if (height == -1)
    {
        SetFixedWidth(width);
    }
    else
    {
        SetFixedSize(width, height);
    }

    SubscribeToEvent(this, E_HOVERBEGIN, URHO3D_HANDLER(ButtonT, HandlerHoverBegin));
    SubscribeToEvent(this, E_HOVEREND, URHO3D_HANDLER(ButtonT, HandlerHoverEnd));
}


void ButtonT::HandlerHoverBegin(StringHash, VariantMap &)
{
    SendEvent(E_HOVER_BEGIN_ELEMENT_GUI);
}


void ButtonT::HandlerHoverEnd(StringHash, VariantMap &)
{
    SendEvent(E_HOVER_END_ELEMENT_GUI);
}


void ButtonT::RegisterObject()
{
    TheContext->RegisterFactory<ButtonT>("UI");

    Context *context = TheContext;

    URHO3D_COPY_BASE_ATTRIBUTES(Button);
}


void ButtonT::SetText(pchar text)
{
    label->SetText(text);
}


void ButtonT::SetHint(pchar text)
{
    hint = new Hint(text);
}


void ButtonT::OnClickBegin(const IntVector2& position, const IntVector2& screenPosition, MouseButton button,
    MouseButtonFlags buttons, QualifierFlags qualifiers, Cursor* cursor) //-V813
{
    Button::OnClickBegin(position, screenPosition, (MouseButton)button, (MouseButtonFlags)buttons, (QualifierFlags)qualifiers, cursor);

    if(buttons == MOUSEB_RIGHT)
    {
        if(hint)
        {
            if(TheHint)
            {
                TheUIRoot->RemoveChild(TheHint);
            }
            hint->SetPosition(screenPosition.x_, screenPosition.y_ - hint->GetHeight());
            TheUIRoot->AddChild(hint);
            hint->BringToFront();
            TheHint = hint;
        }
    }
}

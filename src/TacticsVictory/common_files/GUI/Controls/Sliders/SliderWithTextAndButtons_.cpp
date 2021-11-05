// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "FileSystem/ConfigurationFile_v.h"
#include "GUI/Cursor_.h"
#include "GUI/GUIEvents_.h"
#include "GUI/Controls/Hint_.h"
#include "GUI/Controls/Label_.h"
#include "GUI/Controls/Sliders/SliderInt_.h"
#include "GUI/Controls/Sliders/SliderWithTextAndButtons_.h"


SliderWithTextAndButtons::SliderWithTextAndButtons(Context *context) :
    UIElement(context)
{

}


SliderWithTextAndButtons::SliderWithTextAndButtons(UIElement *uielement, pchar text_, int min, int max, int step,
    int widthText, int widthRoller) :
    UIElement(TheContext)
{
    SharedPtr<Window> window(new Window(TheContext));
    window->SetDefaultStyle(TheCache->GetResource<XMLFile>("UI/MainStyle.xml"));
    window->SetStyle(TheSettings.GetString("menu", "elem", "window", "style"));
    AddChild(window);
 
    window->SetLayout(LM_HORIZONTAL, 3, IntRect(3, 3, 3, 3));

    SharedPtr<Label> text(Label::Create(text_, 15, widthText == -1 ? 
        TheSettings.GetInt("menu", "text", "width") : widthText, -1));

    text->SetAlignment(HA_CENTER, VA_CENTER);

    window->AddChild(text);
    
    auto sizeSlider = TheSettings.GetIntVector2("menu", "slider", "size");
    if (widthRoller != -1)
    {
        sizeSlider.first = widthRoller;
    }
    slider = SliderInt::Create(window, sizeSlider);
    SubscribeToEvent(slider, E_SLIDERINTCHANGED, URHO3D_HANDLER(SliderWithTextAndButtons, HandlerSliderIntChanged));
    SubscribeToEvent(slider, E_HOVERBEGIN, URHO3D_HANDLER(SliderWithTextAndButtons, HandlerHoverBegin));
    SubscribeToEvent(slider, E_HOVEREND, URHO3D_HANDLER(SliderWithTextAndButtons, HandlerHoverEnd));

    buttonLeft = new Button(TheContext);
    buttonLeft->SetRepeat(0.25f, 20.0f);
    buttonLeft->SetStyle("SliderButtonLeft");
    window->AddChild(buttonLeft);
    SubscribeToEvent(buttonLeft, E_PRESSED, URHO3D_HANDLER(SliderWithTextAndButtons, HandlerButtonDown));
    SubscribeToEvent(buttonLeft, E_HOVERBEGIN, URHO3D_HANDLER(SliderWithTextAndButtons, HandlerHoverBegin));
    SubscribeToEvent(buttonLeft, E_HOVEREND, URHO3D_HANDLER(SliderWithTextAndButtons, HandlerHoverEnd));

    buttonRight = new Button(TheContext);
    buttonRight->SetRepeat(0.25f, 20.0f);
    buttonRight->SetStyle("SliderButtonRight");
    window->AddChild(buttonRight);
    SubscribeToEvent(buttonRight, E_PRESSED, URHO3D_HANDLER(SliderWithTextAndButtons, HandlerButtonDown));
    SubscribeToEvent(buttonRight, E_HOVERBEGIN, URHO3D_HANDLER(SliderWithTextAndButtons, HandlerHoverBegin));
    SubscribeToEvent(buttonRight, E_HOVEREND, URHO3D_HANDLER(SliderWithTextAndButtons, HandlerHoverEnd));

    textValue = new Text(TheContext);
    textValue->SetFixedWidth(35);
    textValue->SetFont(TheFont, 15);
    textValue->SetText(String(slider->GetValue()));
    window->AddChild(textValue);

    SetMinSize(window->GetWidth(), window->GetHeight());
    
    SetRange(min, max, step);

    if(uielement)
    {
        uielement->AddChild(this);
    }

    SubscribeToEvent(E_UIMOUSECLICK, URHO3D_HANDLER(SliderWithTextAndButtons, HandlerMouseClick));
}


void SliderWithTextAndButtons::RegisterObject()
{
    TheContext->RegisterFactory<SliderWithTextAndButtons>("UI");

    Context *context = TheContext;

    URHO3D_COPY_BASE_ATTRIBUTES(UIElement);
}


void SliderWithTextAndButtons::SetHint(pchar text)
{
    hint = new Hint(text);
}


void SliderWithTextAndButtons::SetRange(int min, int max, int step)
{
    slider->SetRange(min, max, step);
}


void SliderWithTextAndButtons::SetValue(int value)
{
    slider->SetValueInt(value);
}


void SliderWithTextAndButtons::HandlerSliderIntChanged(StringHash, VariantMap& eventData_)
{
    int value = slider->GetValueInt();
    textValue->SetText(String(value));

    VariantMap &eventData = GetEventDataMap();
    eventData[SliderIntChanged::P_ELEMENT] = this;
    eventData[SliderIntChanged::P_VALUE] = eventData_[SliderIntChanged::P_VALUE];
    SendEvent(E_SLIDERINTCHANGED, eventData);
}


void SliderWithTextAndButtons::HandlerButtonDown(StringHash, VariantMap& eventData)
{
    Button *button = (Button*)eventData[Pressed::P_ELEMENT].GetPtr();

    if(button == buttonLeft)
    {
        slider->Decrease();
    }
    else if(button == buttonRight)
    {
        slider->Increase();
    }
}


void SliderWithTextAndButtons::HandlerHoverBegin(StringHash, VariantMap&)
{
    SendEvent(E_HOVER_BEGIN_ELEMENT_GUI);
}


void SliderWithTextAndButtons::HandlerHoverEnd(StringHash, VariantMap&)
{
    SendEvent(E_HOVER_END_ELEMENT_GUI);
}


int SliderWithTextAndButtons::GetValue()
{
    return slider->GetValueInt();
}


int SliderWithTextAndButtons::GetValueMin()
{
    return slider->GetValueMin();
}


int SliderWithTextAndButtons::GetValueMax()
{
    return slider->GetValueMax();
}


void SliderWithTextAndButtons::HandlerMouseClick(StringHash, VariantMap& eventData)
{
    UIElement *pointer = (UIElement*)eventData[UIMouseClick::P_ELEMENT].GetPtr();

    PODVector<UIElement*> childrens;

    GetChildren(childrens, true);

    if (childrens.Contains(pointer))
    {
        int buttons = (int)eventData[UIMouseClick::P_BUTTONS].GetInt();

        if (buttons == MOUSEB_RIGHT)
        {
            if (hint)
            {
                if (TheHint)
                {
                    TheUIRoot->RemoveChild(TheHint);
                }
                int x = (int)eventData[UIMouseClick::P_X].GetInt();
                int y = (int)eventData[UIMouseClick::P_Y].GetInt();
                hint->SetPosition(x, y - hint->GetHeight());
                TheUIRoot->AddChild(hint);
                hint->BringToFront();
                TheHint = hint;
            }
        }
    }
}

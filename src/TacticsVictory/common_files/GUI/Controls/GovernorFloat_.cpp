﻿// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "FileSystem/ConfigurationFile_v.h"
#include "GUI/Cursor_.h"
#include "GUI/GUIEvents_.h"
#include "GUI/Controls/GovernorFloat_.h"
#include "GUI/Controls/Label_.h"


GovernorCell::GovernorCell(Context *context) :
    Control(context)
{
    SetMovable(false);
    SetStyleAuto();
    SetStyle("WindowGovernorCell");
    //ApplyAttributes();

    auto size = TheSettings.GetIntVector2("menu", "governor", "cell", "size");
    SetFixedSize({ size.first, size.second });

    label = new Text(TheContext);
    label->SetFont(TheFont, 8);
    label->SetAlignment(HA_CENTER, VA_CENTER);

    label->SetStyle("WindowNoBorder");
    label->SetAlignment(HA_LEFT, VA_TOP);
    label->SetPosition(1, -1);
    AddChild(label);

    SubscribeToEvent(E_UIMOUSECLICK, URHO3D_HANDLER(GovernorCell, HandlerMouseDown));
    SubscribeToEvent(E_MOUSEBUTTONUP, URHO3D_HANDLER(GovernorCell, HandlerMouseUp));
    SubscribeToEvent(E_MOUSEMOVE, URHO3D_HANDLER(GovernorCell, HandlerMouseMove));
    SubscribeToEvent(this, E_HOVERBEGIN, URHO3D_HANDLER(GovernorCell, HandlerHoverBegin));
    SubscribeToEvent(this, E_HOVEREND, URHO3D_HANDLER(GovernorCell, HandlerHoverEnd));
}


void GovernorCell::SetSelected()
{
    selected = true;
    SetSymbol(symbol);
}


void GovernorCell::SetNormal()
{
    selected = false;
    SetSymbol(symbol);
}


void GovernorCell::SetSymbol(char symbol_)
{
    symbol = symbol_;
    label->SetText(String(symbol));
    label->SetPosition(symbol == '0' ? 0 : 1, -1);
    label->SetColor(selected ? Color::YELLOW : Color::WHITE);
}


void GovernorCell::SetSymbolWithEvent(char symbol_)
{
    symbol = symbol_;
    label->SetText(String(symbol));
    label->SetPosition(symbol == '0' ? 0 : 1, -1);
    label->SetColor(selected ? Color::YELLOW : Color::WHITE);

    VariantMap eventData = GetEventDataMap();
    eventData[GovernorCellChanged::P_ELEMENT] = this;
    eventData[GovernorCellChanged::P_SYMBOL] = symbol;
    SendEvent(E_GOVERNORCELLCHANGED, eventData);
}


char GovernorCell::GetSymbol()
{
    return symbol;
}


void GovernorCell::HandlerMouseDown(StringHash, VariantMap&)
{
    if(type == Type::Static)
    {
        return;
    }
    if(mouseOver)
    {
        delta = 0;
        mouseIsDown = true;
        TheCursor->Hide();
        posCursor = TheCursor->GetCursor()->GetPosition();
        SetSelected();
    }
}


void GovernorCell::HandlerMouseMove(StringHash, VariantMap& eventData)
{
    if(type == Type::Static)
    {
        return;
    }

    if(!mouseIsDown)
    {
        return;
    }

    TheCursor->GetCursor()->SetPosition(posCursor);

    delta -= (int)eventData[MouseMove::P_DY].GetInt();

    if(delta > 20 || delta < -20)
    {
        ChangeValue(delta);
        delta = 0;
    }
}


void GovernorCell::ChangeValue(int d)
{
    if(type == Type::Sign)
    {
        SetSymbolWithEvent(symbol == '-' ? '+' : '-');
    }
    else if(type == Type::Digit)
    {
        if(d > 0)
        {
            if(symbol == '9')
            {
                SetSymbolWithEvent('0');
            }
            else
            {
                SetSymbolWithEvent(symbol + 1);
            }
        }
        else
        {
            if(symbol == '0')
            {
                SetSymbolWithEvent('9');
            }
            else
            {
                SetSymbolWithEvent(symbol - 1);
            }
        }
    }
}


void GovernorCell::HandlerMouseUp(StringHash, VariantMap&)
{
    if(type == Type::Static)
    {
        return;
    }
    TheCursor->Show();
    mouseIsDown = false;
    SetNormal();
}


void GovernorCell::HandlerHoverBegin(StringHash, VariantMap& eventData)
{
    if(type == Type::Static)
    {
        return;
    }
    GovernorCell *cell = (GovernorCell*)eventData[HoverBegin::P_ELEMENT].GetPtr();
    if(cell == this)
    {
        mouseOver = true;
        SendEvent(E_HOVER_BEGIN_ELEMENT_GUI);
    }
}


void GovernorCell::HandlerHoverEnd(StringHash, VariantMap &)
{
    if (type == Type::Static)
    {
        return;
    }
    mouseOver = false;
    mouseIsDown = false;
    SendEvent(E_HOVER_END_ELEMENT_GUI);
}


void GovernorCell::RegisterObject()
{
    TheContext->RegisterFactory<GovernorCell>("UI");

    Context *context = TheContext;

    URHO3D_COPY_BASE_ATTRIBUTES(Control);
}


GovernorFloat::GovernorFloat(Context *context) :
    Control(context)
{
    SetMovable(false);
    SetStyle("WindowNoBorder");

    int widthLabel = 168;

    label = Label::Create("Camera position Y", 8, widthLabel, 13);
    label->SetAlignment(HA_LEFT, VA_CENTER);
    AddChild(label);

    for(int i = 0; i < numCells; i++)
    {
        SharedPtr<GovernorCell> cell(new GovernorCell(context));
        cell->SetPosition(i * (TheSettings.GetInt("menu", "governor", "cell", "width") - 1) + widthLabel +
            (i > 1 ? TheSettings.GetInt("menu", "governor", "cell", "width") : 0), 0);

        cell->SetSymbol((char)(0x30 + i));
        AddChild(cell);
        cells.Push(cell);

        if(i == 0 || i == 7)
        {
            cell->SetType(GovernorCell::Type::Sign);
            cell->SetSymbol('+');
        }
        else
        {
            cell->SetType(GovernorCell::Type::Digit);
            cell->SetSymbol('0');
        }
        SubscribeToEvent(E_GOVERNORCELLCHANGED, URHO3D_HANDLER(GovernorFloat, HandlerGovernorCellChanged));
    }

    SharedPtr<GovernorCell> cell(new GovernorCell(context));
    cell->SetType(GovernorCell::Type::Static);
    cell->SetPosition(2 * (TheSettings.GetInt("menu", "governor", "cell", "width") - 1) + widthLabel, 0);
    cell->SetSymbol('.');
    AddChild(cell);

    SetFixedSize((TheSettings.GetInt("menu", "governor", "cell", "width") - 1) * (numCells + 1) + 17 + widthLabel,
        TheSettings.GetInt("menu", "governor", "cell", "height"));

    buttonDown = new Button(TheContext);
    buttonDown->SetStyle("DropDownButtonDown");
    AddChild(buttonDown);
    buttonDown->SetPosition((numCells + 1) * (TheSettings.GetInt("menu", "governor", "cell", "width") - 1)
        + 4 + widthLabel, 0);

    SubscribeToEvent(buttonDown, E_HOVERBEGIN, URHO3D_HANDLER(GovernorFloat, HandlerHoverButtonBegin));
    SubscribeToEvent(buttonDown, E_HOVEREND, URHO3D_HANDLER(GovernorFloat, HandlerHoverButtonEnd));
}


void GovernorFloat::RegisterObject()
{
    TheContext->RegisterFactory<GovernorFloat>("UI");

    Context *context = TheContext;

    URHO3D_COPY_BASE_ATTRIBUTES(Control);
}


void GovernorFloat::SetFunctionFloat(pchar title_, pFuncFV funcRead_, pFuncVF funcWrite_)
{
    funcRead = funcRead_;
    funcWrite = funcWrite_;
    title = title_;
    label->SetText(title);
}


void GovernorFloat::Update(float timeStep)
{
    if (!TheCamera)
    {
        return;
    }

    UIElement::Update(timeStep);

    if(funcWrite && valueChanged)
    {
        float value = GetValue();
        funcWrite(value);
        valueChanged = false;
    }
    else if(funcRead)
    {
        WriteValue(funcRead());
    }
}


void GovernorFloat::WriteValue(float value)
{
    int order = 0;
    int mantiss = 0;
    GetOrderAndMantiss(value, &order, &mantiss);

    WriteMantiss(mantiss);
    WriteOrder(order);
}


static char NextDigit(int *mantiss)
{
    char retValue = (char)((*mantiss - (*mantiss / 10) * 10) | 0x30);
    *mantiss /= 10;
    return retValue;
}


void GovernorFloat::WriteMantiss(int mantiss)
{
    cells[0]->SetSymbol(mantiss >= 0 ? '+' : '-');

    if(mantiss < 0)
    {
        mantiss = -mantiss;
    }
    
    for(uint i = 6; i >= 1; i--)
    {
        cells[i]->SetSymbol(NextDigit(&mantiss));
    }
}


void GovernorFloat::WriteOrder(int order)
{
    cells[7]->SetSymbol(order >= 0 ? '+' : '-');

    if(order < 0)
    {
        order = -order;
    }

    for(uint i = 10; i >= 8; i--)
    {
        cells[i]->SetSymbol(NextDigit(&order));
    }
}


void GovernorFloat::GetOrderAndMantiss(float value, int *order, int *mantiss)
{
    static const int SIZE_BUFFER = 30;
    char buffer[SIZE_BUFFER];
#ifdef WIN32
    sprintf_s(buffer, SIZE_BUFFER, "%e", value);
#else
    sprintf(buffer, "%e", value);
#endif
    int first = 0;
    for(int i = 0; i < SIZE_BUFFER; i++)
    {
        if(buffer[i] == 'e')
        {
            first = i + 1;
            break;
        }
    }

    int sign = buffer[first] == '-' ? -1 : 1; //-V557

    int startOrder = first + 1;                 // Индекс первого символа порядка
    int lastOrder = startOrder;
    while(buffer[lastOrder + 1])
    {
        lastOrder++;                            // Находим индекс последнего символа порядка
    }

    int units = buffer[lastOrder--] ^ 0x30;                                     // Количество единиц
    int tens = (lastOrder < startOrder) ? 0 : (buffer[lastOrder--] ^ 0x30);     // Количество десятков
    int hundrend = (lastOrder < startOrder) ? 0 : (buffer[lastOrder] ^ 0x30);   // Количество сотен

    *order = sign *(units + tens * 10 + hundrend * 100);

    sign = buffer[0] == '-' ? -1 : 1;
    first = sign < 0 ? 1 : 0;

    int last = first + 6;

    int buf[6];

    for(int index = first; index <= last; index++)
    {
        if(index != first + 1)
        {
            int iBuf = index == first ? 0 : index - 1 - first;
            buf[iBuf] = buffer[index] ^ 0x30;
        }
    }

    int pow = 1;

    *mantiss = 0;

    for(int i = 5; i >= 0; i--)
    {
        *mantiss += buf[i] * pow;
        pow *= 10;
    }

    *mantiss *= sign;
}


float GovernorFloat::GetValue()
{
    int pow = 1;
    int mantiss = 0;
    for(uint i = 6; i >= 1; i--)
    {
        int value = cells[i]->GetSymbol() ^ 0x30;
        mantiss += value * pow;
        pow *= 10;
    }

    pow = 1;
    int order = 0;

    for(uint i = 10; i >= 8; i--)
    {
        int value = cells[i]->GetSymbol() ^ 0x30;
        order += value * pow;
        pow *= 10;
    }

    float mul = cells[7]->GetSymbol() == '-' ? -1e1f : 1e1f;
    float value = (float)mantiss;

    for(int i = 0; i < order; i++)
    {
        value *= mul;
    }
    value /= 1e5f;

    if(cells[0]->GetSymbol() == '-')
    {
        value = -value;
    }

    return value;
}


void GovernorFloat::HandlerGovernorCellChanged(StringHash, VariantMap&)
{
    valueChanged = true;
}


void GovernorFloat::HandlerHoverButtonBegin(StringHash, VariantMap&)
{
    SendEvent(E_HOVER_BEGIN_ELEMENT_GUI);
}


void GovernorFloat::HandlerHoverButtonEnd(StringHash, VariantMap&)
{
    SendEvent(E_HOVER_END_ELEMENT_GUI);
}

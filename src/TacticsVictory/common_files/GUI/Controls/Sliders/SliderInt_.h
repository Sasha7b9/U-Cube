// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/Sliders/Slider_.h"


URHO3D_EVENT(E_SLIDERINTCHANGED, SliderIntChanged)
{
    URHO3D_PARAM(P_ELEMENT, Element);
    URHO3D_PARAM(P_VALUE, Value);
}


class SliderInt : public Slider
{
    URHO3D_OBJECT(SliderInt, Slider);
public:
    SliderInt(Context *);
    static void RegisterObject();

    static SharedPtr<SliderInt> Create(UIElement *uielement, const IntVector2 &size);
    static SharedPtr<SliderInt> Create(UIElement *uielement, const std::pair<int, int> &size);

    void SetRange(int min, int max, int step = 1);
    virtual void OnDragMove(const IntVector2 &position, const IntVector2 &screenPosition, const IntVector2 &deltaPos,
        MouseButtonFlags buttons, QualifierFlags qualifiers, Cursor *cursor) override;
    int GetValueInt();
    int GetValueMax();
    int GetValueMin();
    void SetValueInt(int value);
    void Increase();
    void Decrease();

private:
    int min = 0;
    int max = 100;
    int value = 0;
    int step = 1;
    float delta = 0.0f;     // ������� ����� ����������� ��������� � ����(����������� ��������� ������������� Slider)
};

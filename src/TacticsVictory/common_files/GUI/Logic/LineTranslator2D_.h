// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Logic/LineTranslator1D_.h"


class LineTranslator2D : public Object
{
    URHO3D_OBJECT(LineTranslator2D, Object);

public:

    enum State
    {
        State_PointStart,
        State_PointFinish,
        State_MoveToFinish,
        State_MoveToStart
    };

    LineTranslator2D(const IntVector2 &start, const IntVector2 &finish, float speed, State startPosition);
    IntVector2 Update(float dT);
    void Toggle();
    State GetState();

private:
    SharedPtr<LineTranslator1D> translatorX;
    SharedPtr<LineTranslator1D> translatorY;
};

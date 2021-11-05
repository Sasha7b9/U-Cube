// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Scene/Terrain/PlaneCube_.h"


class SideCube : public Object
{
    URHO3D_OBJECT(SideCube, Object);

public:
    SideCube();
    PlaneCube plane;
};

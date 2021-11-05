// 2021/03/10 15:31:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Scene/Objects/Units/UnitC.h"
#include "Scene/Objects/Units/Air/AirPlane/AirPlane_.h"


class AirPlaneSpecificC : public UnitSpecificC
{
    URHO3D_OBJECT(AirPlaneSpecificC, UnitSpecificC);

public:

    AirPlaneSpecificC(AirPlane *airPlane) : UnitSpecificC(airPlane) {}

    virtual void Update(float dT) override;
};

// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Scene/Primitives/Line_.h"


Line Line::ZERO = Line();


Line::Line(const Vector3 &start_, const Vector3 &end_) : start(start_), end(end_)
{
}


bool Line::IsEquals(const Line &line)
{
    return start == line.start && end == line.end;
}


bool Line::IsZero()
{
    return start == Vector3::ZERO && end == Vector3::ZERO;
}

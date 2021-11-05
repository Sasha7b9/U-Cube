// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Core/Math_v.h"
#include "Scene/Primitives/Plane_.h"


PlaneT PlaneT::ZERO = PlaneT(Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, Vector3::ZERO);


PlaneT::PlaneT(const Vector3 &v0_, const Vector3 &v1_, const Vector3 &v2_, const Vector3 &v3_) :
    v0(v0_), v1(v1_), v2(v2_), v3(v3_)
{
}


bool PlaneT::IsEquals(const PlaneT &plane)
{
    return v0 == plane.v0 && v1 == plane.v1 && v2 == plane.v2 && v3 == plane.v3;
}


bool PlaneT::IsZero()
{
    return v0 == Vector3::ZERO && v1 == Vector3::ZERO && v2 == Vector3::ZERO && v3 == Vector3::ZERO;
}


void PlaneT::CalculateRowCol()
{
    float xMin = Math::Min(v0.x_, v1.x_, v2.x_, v3.x_);
    float xMax = Math::Max(v0.x_, v1.x_, v2.x_, v3.x_);
    float zMin = Math::Min(-v0.z_, -v1.z_, -v2.z_, -v3.z_);
    float zMax = Math::Max(-v0.z_, -v1.z_, -v2.z_, -v3.z_);
    
    col = (uint)((xMax + xMin) / 2.0f);
    row = (uint)((zMin + zMax) / 2.0f);
}


void PlaneT::SetY(float y)
{
    v0.y_ = v1.y_ = v2.y_ = v3.y_ = y;
}


Line PlaneT::NearEdge(const Ray &ray)
{
    Line lines[] =
    {
        Line(v0, v1),
        Line(v1, v2),
        Line(v2, v3),
        Line(v3, v0)
    };

    int index = -1;

    float distance = 1e3f;

    for (int i = 0; i < 4; i++)
    {
        Ray rayEdge(lines[i].start, lines[i].end - lines[i].start);
        Vector3 closestPoint = ray.ClosestPoint(rayEdge);
        float dist = rayEdge.Distance(closestPoint);
        if (dist < distance)
        {
            distance = dist;
            index = i;
        }
    }

    return Line(lines[index].start, lines[index].end);
}

// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
//#include "Scene/Objects/Object_.h"
#include "Scene/Primitives/Plane_.h"


class CubeTerrain;
class Level;
class ObjectT;
class SegmentTerrain;


struct DIR { enum E {
    LEFT,
    TOPLEFT,
    TOP,
    TOPRIGHT,
    RIGHT,
    DOWNRIGHT,
    DOWN,
    DOWNLEFT
}; };


/*
   0 1 2 3 ...........................N
 0 +--------------------------------------> colZ
   |
 2 |
   |
 3 |
   |
 4 |
 . |
 . |
 . |
 . |
 . |
 . |
   |
   | rowX

*/



class TerrainT : public Object
{
    URHO3D_OBJECT(TerrainT, Object);

public:

    TerrainT();

    ~TerrainT();

    void CreateFromVector(const Vector<Vector<float>> &level);

    float GetHeight(uint rowX, uint colZ) const;
    float GetHeight(float rowX, float colZ) const;
    float GetHeight(const Vector2 &coord) const;

    uint HeightX() const;
    uint WidthZ() const;

    bool IsEmpty() const;

    // "��������" ������� ������ � ����� {colZ, rowX}. ������ ����� �� ����������� ��������. ���� ������ ����� ��������-
    // �� ��� ������������, ��� ���������� Y �� ���������, ���� �� ��������� ��� �����. ����� ��� �� ��� � ���
    // ���������
    void PutIn(ObjectT *object, uint rowX, uint colZ);

    PlaneT GetIntersectionPlane(Ray &ray);

    Line GetIntersectionEdge(Ray &ray);

    PlaneT GetPlane(uint row, uint col);

    Vector<Vector<float>> GetHeightMap();

    PODVector<CubeTerrain*>* GetColumnCubes(const CubeTerrain *cube, DIR::E dir);

    static Vector<Vector<PODVector<CubeTerrain*>>> columnsCubes;

    SharedPtr<Level> level;

private:
   
    Vector<Vector<SharedPtr<SegmentTerrain>>> segments;

    SegmentTerrain *GetSegmentForCoord(uint row, uint col);
};

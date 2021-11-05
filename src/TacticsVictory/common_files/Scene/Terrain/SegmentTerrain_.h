// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Scene/Terrain/Level_.h"


class CubeTerrain;
class LayerTerrain;


class SegmentTerrain : public Object
{
    URHO3D_OBJECT(SegmentTerrain, Object);

public:

    static const uint HEIGHT_X = 10;
    static const uint WIDTH_Z = 10;

    enum
    {
        LEFT,
        TOP,
        RIGHT,
        BOTTOM
    };

    SegmentTerrain();
    ~SegmentTerrain();
    void CreateFromVector(const Vector<Vector<Level::LogicCell>> &level, uint row0, uint col0, uint numRows, uint numCols);
    void Build();
    void GetColumnCubes(uint row, uint col, PODVector<CubeTerrain*> &column);

    SegmentTerrain* neighbours[4];      // ������ ������������� � ������ ������

private:
    Vector<SharedPtr<LayerTerrain>> ground;          // Height (0, 1, 2, 3, ...)
    Vector<SharedPtr<LayerTerrain>> underGround;     // Height [0, -1, -2, ...)


    void AddCube(const SharedPtr<CubeTerrain> &cube);
    void CreateLayers();
};

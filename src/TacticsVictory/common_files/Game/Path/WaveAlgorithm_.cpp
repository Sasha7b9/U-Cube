﻿// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Path/WaveAlgorithm_.h"


#define TERRAIN_HEIGHT_EQUAL(x,z) (fabs(TheTerrain->GetHeight(x, z) - heightStart) < M_EPSILON)


WaveAlgorithm::WaveAlgorithm(Context *context) : Thread(), Object(context)
{

}


WaveAlgorithm::~WaveAlgorithm()
{
    Stop();
}


//static CScriptArray* GetTestStringsToArray()
//{
//    static Vector<String> args;
//    args.Clear();
//    args.Push("Test");
//    args.Push("array");
//    args.Push("Angelscript");
//
//    return VectorToArray<String>(args, "Array<String>");
//}


void WaveAlgorithm::RegisterInAS()
{
//    asIScriptEngine *engine = TheScript->GetScriptEngine();
//    engine->RegisterObjectType("WaveAlgorithm", 0, asOBJ_REF);
////    RegisterObject<WaveAlgorithm>(engine, "WaveAlgorithm");
//#ifdef WIN32
//#pragma warning(push)
//#pragma warning(disable:4191)
//#endif
//    engine->RegisterObjectMethod("WaveAlgorithm", "bool PathIsFound()", asMETHOD(WaveAlgorithm, PathIsFound), asCALL_THISCALL);
//    //engine->RegisterObjectMethod("WaveAlgorithm", "Array<uint> GetPathUINT()", asMETHOD(WaveAlgorithm, GetPathUINT), asCALL_THISCALL);
//    //engine->RegisterObjectMethod("WaveAlgorithm", "Array<String>@ GetPathString()", asMETHOD(WaveAlgorithm, GetPathString), asCALL_THISCALL);
//    engine->RegisterObjectMethod("WaveAlgorithm", "uint GetUINT()", asMETHOD(WaveAlgorithm, GetUINT), asCALL_THISCALL);
//
//    engine->RegisterGlobalFunction("Array<String>@ GetTestStrings()", asFUNCTION(GetTestStringsToArray), asCALL_CDECL);
//#ifdef WIN32
//#pragma warning(pop)
//#endif
}


void WaveAlgorithm::SetSize(uint rows, uint cols)
{
    numRows = rows;
    numCols = cols;

    cells.Resize(numRows);
    for (auto &row : cells)
    {
        row.Resize(numCols);
    }
}


void WaveAlgorithm::StartFind(Coord start_, Coord end_)
{
    start = start_;
    end = end_;
    Run();
}


bool WaveAlgorithm::PathIsFound()
{
    return pathIsFound;
}


PODVector<Coord> WaveAlgorithm::GetPath()
{
    Stop();
    return path;
}


void WaveAlgorithm::ThreadFunction()
{
    pathIsFound = false;
    path.Clear();
    FindPath();
    pathIsFound = true;
}


void WaveAlgorithm::FindPath()
{
    if (fabs(TheTerrain->GetHeight(start.rowX, start.colZ) - TheTerrain->GetHeight(end.rowX, end.colZ)) > M_EPSILON)
    {
        return;
    }

    if (start == end)
    {
        path.Push(start);
        return;
    }

    for (auto &row : cells)
    {
        for (auto &cell : row)
        {
            cell = -1;
        }
    }

    heightStart = TheTerrain->GetHeight(start.rowX, start.colZ);

    Vector<Wave> waves;

    Wave wave;
    SetCell(wave, start.rowX, start.colZ, 0);

    waves.Push(wave);

    do
    {
        NextWave(waves);
    } while(waves[waves.Size() - 1].Size() && !Contain(waves[waves.Size() - 1], end));

    if(Contain(waves[waves.Size() - 1], end))
    {
        path.Push(end);
        while(!(path[0] == start))
        {
            AddPrevWave(path);
        }
    }
    else
    {
        path.Push(start);
    }
}


bool WaveAlgorithm::Contain(const Wave &wave, const Coord &coord)
{
    for (auto &crd : wave)
    {
        if (crd == coord)
        {
            return true;
        }
    }

    return false;
}


static int dRow[] = {0, -1, 0, 1, -1, -1, 1, 1};
static int dCol[] = {-1, 0, 1, 0, -1, 1, 1, -1};


void WaveAlgorithm::NextWave(Vector<Wave> &waves)
{
    int numWave = static_cast<int>(waves.Size());
    Wave &prevWave = waves[static_cast<uint>(numWave - 1)];
    Wave wave;

    for (auto coord : prevWave)
    {
        for (int i = 0; i < 8; i++)
        {
            uint row = coord.rowX;
            uint col = coord.colZ;

            int dR = dRow[i];
            int dC = dCol[i];

            int iRow = static_cast<int>(row) + dR;
            int iCol = static_cast<int>(col) + dC;

            uint newRow = static_cast<uint>(iRow);
            uint newCol = static_cast<uint>(iCol);

            if (newRow < numRows &&
                newCol < numCols &&
                cells[newRow][newCol] == -1 &&
                fabs(TheTerrain->GetHeight(newRow, newCol) - heightStart) < M_EPSILON)
            {
                if (i == 4 && (!TERRAIN_HEIGHT_EQUAL(row, col - 1) || !TERRAIN_HEIGHT_EQUAL(row - 1, col)))
                {
                    continue;
                }
                else if (i == 5 && (!TERRAIN_HEIGHT_EQUAL(row - 1, col) || !TERRAIN_HEIGHT_EQUAL(row, col)))
                {
                    continue;
                }
                else if (i == 6 && (!TERRAIN_HEIGHT_EQUAL(row, col + 1) || !TERRAIN_HEIGHT_EQUAL(row + 1, col)))
                {
                    continue;
                }
                else if (i == 7 && (!TERRAIN_HEIGHT_EQUAL(row, col - 1) || !TERRAIN_HEIGHT_EQUAL(row + 1, col)))
                {
                    continue;
                }

                SetCell(wave, newRow, newCol, numWave);
            }
        }
    }

    waves.Push(wave);
}


void WaveAlgorithm::SetCell(Wave &wave, uint row, uint col, int numWave)
{
    wave.Push(Coord(row, col));
    cells[row][col] = numWave;
}


void WaveAlgorithm::AddPrevWave(PODVector<Coord> &path_)
{
    Coord coord = path_[0];
    uint rowX = coord.rowX;
    uint colZ = coord.colZ;
    int numWave = cells[rowX][colZ];

    for (int i = 0; i < 8; i++)
    {
        int iRow = static_cast<int>(rowX) + dRow[i];
        int iCol = static_cast<int>(colZ) + dCol[i];

        uint newRow = static_cast<uint>(iRow);
        uint newCol = static_cast<uint>(iCol);

        if (newRow < TheTerrain->HeightX() && newCol < TheTerrain->WidthZ() && cells[newRow][newCol] == numWave - 1)
        {
            path_.Insert(0, Coord(newRow, newCol));
            return;
        }
    }
}


void WaveAlgorithm::RegisterObject()
{
    RegisterInAS();
}

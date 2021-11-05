// 2021/02/26 17:28:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Scene/Objects/ObjectS.h"
#include "Scene/Objects/Units/Ground/Tank/Tank_.h"
#include "Scene/Objects/Units/UnitS.h"
#include "Scene/Objects/Units/Accessories/Engine/Engine_.h"


void UnitSpecificS::Update(float timeStep)
{
    EngineT *engine = unit->engine;

    unit->StoreState();                                         // ��������� ������� ��������� �����

    engine->Update(timeStep);                                   // ��������� ��� ���������

    if (unit->TooCloseToAnoterUnit())                           // ���� ������ ���� ������� ������ � ������� �����
    {
        engine->algorithm.Clear();                              // �� ��������� ���������� ���������

        unit->RestoreState();                                   // � ��������������� ��������� �����
    }

    if (engine->algorithm.IsFinished())
    {
        int direct = Rand() % CommandEngine::MoveToWestNorth;

        engine->GiveCommand((CommandEngine::E)(direct + CommandEngine::None + 1), (Rand() % 10) + 1);
    }

    ObjectSpecificS::Update(timeStep);
}

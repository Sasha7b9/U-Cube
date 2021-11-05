// 2021/03/03 21:55:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Scene/Objects/Object_.h"


class ObjectSpecificC : public ObjectSpecific
{
    URHO3D_OBJECT(ObjectSpecificC, ObjectSpecific);

public:

    ObjectSpecificC(ObjectT *object);

    static ObjectT *GetFromID(uint id);

    virtual void OnPostRenderUpdate() override;

    static HashMap<uint, ObjectT *> remoteStorage;  // ����� �������� ������� ������� ����� �� id ����� �� �������
};

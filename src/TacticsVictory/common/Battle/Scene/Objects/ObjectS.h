// 2021/03/03 21:08:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Scene/Objects/Object_.h"


class ObjectT;


class ObjectSpecificS : public ObjectSpecific
{
public:

    ObjectSpecificS(ObjectT *object);

    virtual void Update(float timeStep) override;

    float timeNextTimeSend = 0.0f;        // ����� ��������� ������� ��������� �������
};

// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Tank;


class SceneT : public Scene
{
public:

    SceneT();
    virtual ~SceneT();

    virtual void Create();

    // ��������� ��������� ����� � buffer ��� �������� �� ����
    void Comporess(VectorBuffer &buffer);

    // ����������� ��������� ����� �� buffer-�, ��������� �� ����
    void Decompress(MemoryBuffer &buffer);
};

// 2021/03/05 20:39:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ObjectT;


//----------------------------------------------------------------------------------------------------------------------
struct MovementPP
{
    MovementPP(ObjectT *_object) : object(_object) {}

    float GetSpeed() const { return speedMove; };

private:

    ObjectT *object = nullptr;

    float speedMove = 0.0f;      // �������������� �������� ��������
};


//----------------------------------------------------------------------------------------------------------------------
struct RotationPP
{
    RotationPP(ObjectT *_object) : object(_object) {}

    float GetSpeed() const { return speedRotate; };

    Quaternion GetWorld() const;

    void SetWorld(const Quaternion &rotation);

    void ChangeWorld(const Quaternion &delta);

private:

    ObjectT *object = nullptr;

    float speedRotate = 0.0f;    // �������������� �������� ��������
};


//----------------------------------------------------------------------------------------------------------------------
struct DirectionPP
{
    DirectionPP(ObjectT *_object) : object(_object) { }

    Vector3 GetWorldDir() const;
    Vector3 GetWorldUp() const;
    Vector3 GetWorldRight() const;

private:

    ObjectT *object = nullptr;
};


//----------------------------------------------------------------------------------------------------------------------
struct PositionPP
{
    PositionPP(ObjectT *_object) : object(_object) {}

    // ���������� �������� ������� � ����
    Vector3 GetWorld() const;

    // ������������� ������� � ���� � ������ ��������
    void SetWorld(const Vector3 &position);

    // ���������� ������ ������� (���������� y)
    float GetAltitude() const { return GetWorld().y_; }

    // ���������� ���������� ������� � ������������ �����
    Vector2 GetCoord() const;

    // ��� ������� ������������ ��������� �� ������
    void CalculateDistanceFromCenter() { distanceFromCenter = GetWorld().Length(); }

    float distanceFromCenter = 0.0f;        // ��������� �� ������

private:

    ObjectT *object = nullptr;
};


//----------------------------------------------------------------------------------------------------------------------
struct MaxPP                               // � ���� ��������� ����� ��������� ����������� ��������� �������� ����������
{
    const float speedMove = 1.0f;                 // ������������ �������� ��������
    const float speedRotate = 120.0f;             // ������������ �������� ��������
    const float altitude = 10.0f;                 // ������������ ������ ��� ������������
};


//----------------------------------------------------------------------------------------------------------------------
struct MinPP                                // � ���� ��������� ����� ��������� ���������� ��������� �������� ����������
{
    const float altitude = 0.0f;            // ����������� ������ ��� ������������
};


//----------------------------------------------------------------------------------------------------------------------
class PhysicsParameters : public Object
{
    URHO3D_OBJECT(PhysicsParameters, Object);

public:

#undef min
#undef max

    PhysicsParameters(ObjectT *_object, const MinPP &_min, const MaxPP &_max) : Object(TheContext),
        min(_min), max(_max), pos(_object), dir(_object), mov(_object), rot(_object), object(_object) {}

    MinPP       min;
    MaxPP       max;
    PositionPP  pos;
    DirectionPP dir;
    MovementPP  mov;
    RotationPP  rot;

private:

    ObjectT *object = nullptr;
};

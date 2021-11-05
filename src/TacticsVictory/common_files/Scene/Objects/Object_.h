// 2021/02/18 22:31:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Scene/Objects/PhysicsParameters_.h"


class ObjectT;
class PhysicsParameters;
struct MaxPP;
struct MinPP;


//----------------------------------------------------------------------------------------------------------------------
// ����� �������� �������� ��� ���������� ������, ������� ����� ��������� � ��� ��� ���������� ����������.
// ������� �������� ���������� � node_
class ShiftParameters : public Object
{
    URHO3D_OBJECT(ShiftParameters, Object);

public:

    ShiftParameters() : Object(TheContext) {}

    float rotateY = 0.0f;               // ������� ������ ������������ ����������� ����
//    Vector3 position = Vector3::ZERO;  // ���� ������ �� ��������� ������������ ������ ���������, ����� ��������
};


//----------------------------------------------------------------------------------------------------------------------
// ����������� ��� �����/��������� �������� ���������
class ObjectSpecific : public Object
{
    URHO3D_OBJECT(ObjectSpecific, Object);

public:

    static ObjectSpecific *Create(ObjectT *object);

    virtual void Update(float dT);

    virtual void OnPostRenderUpdate() {};

protected:

    ObjectSpecific(ObjectT *_object);

    ObjectT *object       = nullptr;
};


namespace ObjectCreator
{
    // ������ ������ ��� ��������� ��� �� �����
    template<class T> T *Create();

    // ������ ������ � �������� ��� �� �����
    template<class T> T *Create(uint rowX, uint colZ);
}


//----------------------------------------------------------------------------------------------------------------------
class ObjectT : public LogicComponent
{
    URHO3D_OBJECT(ObjectT, LogicComponent);

public:

    // ��������� ��������� ������� ��� �������� �� ����
    virtual void Compress(VectorBuffer &buffer);

    // ����������� ��������� �������, ��������� �� ����
    virtual void Decompress(MemoryBuffer &buffer);

    virtual void OnPostRenderUpdate();

    Node *GetObjectNode() const { return GetNode(); }

    virtual bool IsFlying() const   { return node_->GetVar(VAR_NODE_IS_FLYING).GetBool(); }
    virtual bool IsUnit() const     { return node_->GetVar(VAR_NODE_IS_UNIT).GetBool(); }
    virtual bool IsBuilding() const { return node_->GetVar(VAR_NODE_IS_BUILDING).GetBool(); }

    // ��������� ��������� �������
    void StoreState();

    // ������������ ����������� �������
    void RestoreState();

    // ���������� true, ���� ������ ������������ � ������ ��� �������
    bool IsIntersectionWithUnitOrBuilding() const;

    BoundingBox GetBoundingBox() const;

    SharedPtr<ShiftParameters>   shift;     // ������������ ��� ���������� ���������� ������ � ������� �����
    static Vector<ObjectT *>     storage;   // ����� �������� ��� ������� ���� ObjectT (� �� ���������)
    SharedPtr<PhysicsParameters> physics;   // ��������� � ���������� ����. ����� ��� ����������
    static ObjectT              *empty;     // ���� ������ ������������ � �������� null-�������

protected:

    ObjectT(Context *context, const MinPP &min, const MaxPP &max);

    bool LoadFromJSON(pchar name);

    void Normalize(float k = 1.0f);

    virtual void OnNodeSet(Node *node) override;

    virtual void Start() override;

    virtual void DelayedStart() override;

    virtual void CreateSpecific() = 0;

    virtual void Update(float time) override;

    // ���������� ������� ����������� node_ (������������ �������� ���� �����)
    int NestingDepth() const;

    SharedPtr<ObjectSpecific> specific;         // ������/������ ����������� ���������

private:

    // ���������������� ������ � ���� �������� � ����������� �������. ���, ��� ��� ����������, ����� ������������
    // GetObjectNode.
    Node *GetNode() const { return Component::GetNode(); }

    SharedPtr<StaticModel> staticModel;

    VectorBuffer state;                     // ����� ������������ ��� ���������� ���������
};

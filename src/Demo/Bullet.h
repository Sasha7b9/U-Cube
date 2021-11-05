#pragma once

#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Graphics/Material.h>

using namespace Urho3D;

URHO3D_EVENT(E_SHOT, Shot)
{
    //URHO3D_PARAM(P_NODE, Node);
}


class Bullet : public LogicComponent
{
    URHO3D_OBJECT(Bullet, LogicComponent);

public:
    /// Construct.
    Bullet(Context* context);

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);

    /// Handle startup. Called by LogicComponent base class.
    virtual void Start();

    /// Handle update. Called by LogicComponent base class.
    virtual void Update(float timeStep);

    ///
    void Shot(const Vector3& start, const Vector3& direction, float distance);

private:
    float timeShot = 0.0f;
    static float timeStartCalc;
    static float timeForBuild;
    SharedPtr<Material> material;
    float timeLive = 0.0f;
};

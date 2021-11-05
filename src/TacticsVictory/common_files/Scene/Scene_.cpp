// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Core/Math_v.h"
#include "Game/Logic/SunEngine_.h"
#include "Scene/Scene_.h"
#include "Scene/Objects/Object_.h"
#include "Scene/Terrain/Terrain_.h"


SceneT::SceneT() : Scene(TheContext)
{

}


SceneT::~SceneT()
{
    delete TheTerrain;
}


void SceneT::Create()
{
    CreateComponent<Octree>(LOCAL);

    ThePhysicsWorld = CreateComponent<PhysicsWorld>(LOCAL);

    ThePhysicsWorld->SetGravity(Vector3::ZERO);

    ThePhysicsWorld->SetFps(5);

    // Create a Zone component into a child scene node. The Zone controls ambient lighting and fog settings. Like the Octree,
    // it also defines its volume with a bounding box, but can be rotated (so it does not need to be aligned to the world X, Y
    // and Z axes.) Drawable objects "pick up" the zone they belong to and use it when rendering; several zones can exist

    Node* zoneNode = CreateChild("Zone");
    Zone* zone = zoneNode->CreateComponent<Zone>(LOCAL);
    // Set same volume as the Octree, set a close bluish fog and some ambient light
    zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
    zone->SetFogColor(Color::GRAY);
    zone->SetFogHeightScale(100000.0f);
    zone->SetFogStart(0.0f);
    zone->SetFogEnd(10000.0f);
    float dColor = 0.1f;
    zone->SetAmbientColor(Color(dColor, dColor, dColor));
}


void SceneT::Comporess(VectorBuffer &buffer)
{
    for (uint i = 0; i < ObjectT::storage.Size(); i++)
    {
        ObjectT::storage[i]->Compress(buffer);
    }
}

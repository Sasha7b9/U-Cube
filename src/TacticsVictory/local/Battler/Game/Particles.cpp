// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Particles.h"
#include "Scene/SceneC.h"


Vector<Vector<Node*>> Particles::nodesParticles;


void Particles::Init()
{
    for (uint i = 0; i < NumParticles; i++)
    {
        nodesParticles.Push(Vector<Node*>());
    }

    // Explosion_Tank
    Node *node = TheScene->CreateChild("Emitter");
    ParticleEmitter *emitter = node->CreateComponent<ParticleEmitter>(LOCAL);
    emitter->SetViewMask(VIEW_MASK_FOR_EFFECTS);
    emitter->SetEffect(TheCache->GetResource<ParticleEffect>("Models/Units/Tank/FireTank.xml"));
    emitter->SetEmitting(false);
    Vector<Node*> &emitters = nodesParticles.At(Explosion_Tank);
    emitters.Push(node);
}


void Particles::EmittingStatic(TypeParticles type, const Vector3 &position)
{
    if (type == Explosion_Terrain)
    {
        Node *node = TheScene->CreateChild();
        node->SetScale(1.0f);
        node->SetPosition(position);
        ParticleEmitter *emitter = node->CreateComponent<ParticleEmitter>(LOCAL);
        emitter->SetViewMask(VIEW_MASK_FOR_EFFECTS);
        emitter->SetEffect(TheCache->GetResource<ParticleEffect>("Particle/Disco.xml"));
        emitter->SetAutoRemoveMode(REMOVE_COMPONENT);
        emitter->SetEmitting(true);
    }

    /*
    ParticleEmitter* emitter = nullptr;
    
    Vector<ParticleEmitter*> &emitters = particles.At(type);

    // ������� ������������ �������
    for (auto em : emitters)
    {
        if (!em->IsEmitting())
        {
            emitter = em;
            break;
        }
    }

    // ���� �� ����� �������� - ������ �����
    if (!emitter)
    {
        Node *node = TheScene->CreateChild("Emitter");
        emitter = (ParticleEmitter*)node->CloneComponent(emitters[0]);
        emitter->SetEffect(emitters[0]->GetEffect());
        emitters.Push(emitter);
    }

    emitter->GetNode()->SetPosition(position);
    emitter->SetEmitting(true);
    */
}



void Particles::EmittingDinamic(TypeParticles type, Node *node_)
{
    String name = String((uint)type);
    if (NumEmitters(node_, name) > 1)
    {
        return;
    }
    if (type == Explosion_Tank)
    {
        Node *node = node_->CreateChild(name);
        node->SetScale(50.0f);
        ParticleEmitter *emitter = node->CreateComponent<ParticleEmitter>(LOCAL);
        emitter->SetViewMask(VIEW_MASK_FOR_EFFECTS);
        emitter->SetEffect(TheCache->GetResource<ParticleEffect>("Models/Units/Tank/FireTank.xml"));
        emitter->SetAutoRemoveMode(REMOVE_COMPONENT);
        emitter->SetEmitting(true);
    }
    else if (type == Fire_Tank)
    {
        Node *node = node_->CreateChild(name);
        node->SetScale(25.0f);
        node->SetPosition({0.0f, 15.5f, 0.0f});
        ParticleEmitter *emitter = node->CreateComponent<ParticleEmitter>(LOCAL);
        emitter->SetViewMask(VIEW_MASK_FOR_EFFECTS);
        emitter->SetEffect(TheCache->GetResource<ParticleEffect>("Particle/Disco.xml"));
        emitter->SetAutoRemoveMode(REMOVE_COMPONENT);
        emitter->SetEmitting(true);
    }
}


int Particles::NumEmitters(Node *node, const String &type)
{
    PODVector<Component*> components;
    node->GetComponents(components, ParticleEmitter::GetTypeStatic());

    int num = 0;
    for (uint i = 0; i < components.Size(); i++)
    {
        if (components[0]->GetNode()->GetName() == type) //-V767
        {
            num++;
        }
    }

    return num;
}

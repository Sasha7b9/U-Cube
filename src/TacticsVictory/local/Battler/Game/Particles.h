// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


enum TypeParticles
{
    Explosion_Tank,
    Explosion_Terrain,
    Fire_Tank,
    NumParticles
};


class Particles
{
public:
    static void Init();
    static void EmittingStatic(TypeParticles type, const Vector3 &position);
    static void EmittingDinamic(TypeParticles type, Node *node);

private:
    static Vector<Vector<Node*>> nodesParticles;
    static int NumEmitters(Node *node, const String &type);
};

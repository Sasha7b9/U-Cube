// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/Control_.h"


class ObjectT;
class Label;


class ContextMenuUnit : public Control
{
    URHO3D_OBJECT(ContextMenuUnit, Control);

public:
    ContextMenuUnit();
    void Create(ObjectT *object);

private:
    SharedPtr<Label> title;
    ObjectT *object = nullptr;

    void CreateForUnit();
    void HandleToggledFiedView(StringHash, VariantMap&);
};

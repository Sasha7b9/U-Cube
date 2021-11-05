#pragma once


enum CameraMode
{
    ModeNone,
    ModeCommander,
    ModeShooter,
    ModeDriver,
    ModeOverview
};


class Sight;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CameraUni : public Object
{
    URHO3D_OBJECT(CameraUni, Object)

public:
    CameraUni(Context *context);

    void MoveFromMouse();
    bool SetCameraMode(CameraMode mode, Node *node = nullptr);  // For client
    void SetupViewport();                       // For server
    void IncFov()
    {
        camera->SetFov(camera->GetFov() / 1.1f);
    }
    void DecFov()
    {
        camera->SetFov(camera->GetFov() * 1.1f);
    }
    void DefaultFov()
    {
        camera->SetFov(45.0f);
    }
    CameraMode GetMode()
    {
        return mode;
    }

    Node *GetNode()
    {
        return node;
    }

private:
    float yaw = 0.0f;
    float pitch = 0.0f;
    SharedPtr<Node> node;
    Camera *camera = nullptr;
    CameraMode mode = ModeNone;
    Quaternion rotationCommaner;
    Sight *sight = nullptr;

    CameraUni(CameraUni const&) : Object(nullptr) {};
    CameraUni operator=(CameraUni const&) {};
};

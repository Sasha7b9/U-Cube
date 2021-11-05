#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum Control
{
    CTRL_FORWARD            = 1 << 0,   ///< �������� �����
    CTRL_BACK               = 1 << 1,   ///< �������� �����
    CTRL_LEFT               = 1 << 2,   ///< ������� �����
    CTRL_RIGHT              = 1 << 3,   ///< ������� ������
    CTRL_TOWER_LEFT         = 1 << 4,   ///< ������� ����� �����
    CTRL_TOWER_RIGHT        = 1 << 5,   ///< ������� ����� ������
    CTRL_TRUNK_UP           = 1 << 6,   ///< ����� �����
    CTRL_TRUNK_DOWN         = 1 << 7,   ///< ����� ����
    CTRL_TOWER_LEFT_FAST    = 1 << 8,   ///< ������� ������� ����� �����
    CTRL_TOWER_RIGHT_FAST   = 1 << 9,   ///< ������� ������� ����� ������
    CTRL_TRUNK_UP_FAST      = 1 << 10,  ///< ������ ����� �����
    CTRL_TRUNK_DOWN_FAST    = 1 << 11   ///< ������ ����� ����
};

enum TypeControl
{
    CTRL_ON,    ///< ����� ������� ����������
    CTRL_OFF    ///< ����� ������� ����������
};


const float ENGINE_POWER = 500.0f;
const float DOWN_FORCE = 1.0f;
const float MAX_WHEEL_ANGLE = 22.5f;
const float SPEED_TOWER_ROTATION = 2.0f;
const float SPEED_TRUNK_ROTATION = 2.5f;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tank component, responsible for physical movement according to controls.
class Tank : public LogicComponent
{
    URHO3D_OBJECT(Tank, LogicComponent);

public:
    /// Construct.
    Tank(Context* context);
    
    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    
    /// Handle physics world update. Called by LogicComponent base class.
    virtual void FixedUpdate(float timeStep);
    
    /// Initialize the tank. Create rendering and physics components. Called by the application.
    void Init();

    /// Movement controls.
    Controls controls;

    void Delete();

    unsigned towerID;
    unsigned trunkID;

    void DrawDebugGeometry_();
    void Logging();

    //
    void SetControl(Control control, TypeControl type);

    int speed = 0;
    
private:
    void InitTower();

    void InitTrunk();

    void RotateTrunk(float delta);

    // Hull and wheel rigid bodies.
    WeakPtr<RigidBody> hullBody;
    bool created = false;

    WeakPtr<Node> nodeTower;
    WeakPtr<Node> nodeTrunk;

    float pitchTower = 0.0f;

    float yawTrunkMin = -110.0f;
    float yawTrunkMax = -35.0f;
    float yawTrunk = 0.0f;
    
    /// Current left/right steering amount (-1 to 1.)
    float steering;

    Tank(Tank const&) : LogicComponent(nullptr) {};
    Tank operator=(Tank const&){};

    float scaleNode = 0.5f;

    const int maxSpeed = 20;
    const int minSpeed = -5;
};

/* (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by */
#include <stdafx.h>
#include "Objects/Vehicle.h"
#include "Tankist.h"
#include "GlobalVars.h"
#include "CommonFunctions.h"
#include "Network/Chat.h"
#include "Graphics/ImageT.h"


#pragma warning(disable:4100)
URHO3D_DEFINE_APPLICATION_MAIN(Tankist)
#pragma warning(default:4100)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Tankist::Tankist(Context* context) :
    Application(context)
{
    Vehicle::RegisterObject(context);

    gContext = context_;

    gSet = new Settings();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::Setup()
{
    Vector<String> argumensts = GetArguments();

    if (!ParseArguments(argumensts, gTypeApplication, gIPAddress, gNumPort))
    {
#ifdef WIN32
        URHO3D_LOGERROR("To rum application type tankist.exe -{client|server} [-ip:xxx.xxx.xxx.xxx] -port:xxx");
#else
        URHO3D_LOGERROR("To rum application type ./tankist -{client|server} [-ip:xxx.xxx.xxx.xxx] -port:xxx");
#endif
        exit = true;
    }

    gLog = new Log(context_);
    gLog->SetLevel(LOG_INFO);

    if (gTypeApplication == Type_Client)
    {
        gLog->Open("client.log");
    }
    else
    {
        gLog->Open("server.log");
    }

    gAudio = GetSubsystem<Audio>();

#ifdef WIN32
    gAudioCapturer = new AudioCapturer();
    gAudioCapturer->Start();
#endif

    gNetwork = GetSubsystem<Network>();
    gFileSystem = GetSubsystem<FileSystem>();
    gCache = GetSubsystem<ResourceCache>();

    gCache->AddResourceDir(gFileSystem->GetProgramDir() + "TankistData");
    //gCache->AddResourceDir(gFileSystem->GetProgramDir() + "distr\\TankistData");
    //gCache->AddResourceDir(gFileSystem->GetProgramDir() + "..\\..\\out\\distr\\TankistData");

    gTime = GetSubsystem<Time>();

    if (gTypeApplication == Type_Client)
    {
        gUI = GetSubsystem<UI>();
        gUIRoot = gUI->GetRoot();
        gInput = GetSubsystem<Input>();
    }

    engineParameters_["Headless"] = gTypeApplication == Type_Server;
    engineParameters_["WindowTitle"] = "Tankist WaT";
    engineParameters_["LogName"] = gFileSystem->GetAppPreferencesDir("urho3d", "logs") + GetTypeName() + ".log";

    engineParameters_["FrameLimiter"] = false;
    engineParameters_["FullScreen"] = gSet->Get(FULLSCREEN);
    engineParameters_["Sound"] = true;
    engineParameters_["WindowWidth"] = gSet->Get(WINDOW_WIDTH);
    engineParameters_["WindowHeight"] = gSet->Get(WINDOW_HEIGHT);
    //engineParameters_["WindowPositionY"] = 20;
    //engineParameters_["WindowPositionX"] = gTypeConnection == Connection_Server ? 20 : 700;

    // Construct a search path to find the resource prefix with two entries:
    // The first entry is an empty path which will be substituted with program/bin directory -- this entry is for binary when it is still in build tree
    // The second and third entries are possible relative paths from the installed program/bin directory to the asset directory -- these entries are for binary when it is in the Urho3D SDK installation location
    if (!engineParameters_.Contains("ResourcePrefixPaths"))
    {
        engineParameters_["ResourcePrefixPaths"] = ";../share/Resources;../share/Urho3D/Resources";
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::Start()
{
    if (gTypeApplication == Type_Client)
    {
        gLocale = GetSubsystem<Localization>();
        gLocale->LoadJSONFile("Strings.json");
        gLocale->SetLanguage(gSet->Get(LANGUAGE) == 0 ? "ru" : "en");
        gLocale->SetLanguage(gSet->Get(LANGUAGE) == 0 ? "en" : "ru");

        gGraphics = GetSubsystem<Graphics>();
        gRenderer = GetSubsystem<Renderer>();

        if(gSet->FirstStart())
        {
            /*
            IntVector2 res = gGraphics->GetDesktopResolution();
            gSet->Set(WINDOW_WIDTH, res.x_);
            gSet->Set(WINDOW_HEIGHT, res.y_);
            gGraphics->SetMode(gSet->Get(WINDOW_WIDTH), gSet->Get(WINDOW_HEIGHT));
            */
        }

        SetWindowTitleAndIcon();

        CreateConsoleAndDebugHud();

        CreateUI();
    }

    CreateScene();

    SubscribeToEvents();

    gTankist = this;

    gCounters = new Counters();

    if (gTypeApplication == Type_Server)
    {
        gServer = new Server(context_);
        gServer->Start(gNumPort);

        CreateListFiles();

        gChatLog = new ChatLog(context_);

        gChat = new Chat(gContext, Chat::Chat_Server);
        gChat->Listen(PORT_CHAT);

    }
    
    if (gTypeApplication == Type_Client)
    {
        gCamera = new CameraUni(context_);
        gCamera->SetupViewport();

        gClient = new Client(context_);
        gClient->ConnectToServer();

        CreateInstructions();

        gChat = new Chat(gContext, Chat::Chat_Client);
        gChat->Connect(gIPAddress.CString(), PORT_CHAT);
    }

    gGame = new Game(gContext);
    gGame->Start();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::Stop()
{
    if (gClient)
    {
        gClient->Disconnect();
        gSet->Save();
    }

#ifdef WIN32
    gAudioCapturer->Stop();
#endif

    //engine_->DumpResources(true);
    //engine_->DumpProfiler();
    //engine_->DumpMemory();
    gLog->Write(__FILE__, __LINE__, 0, "out");
    gLog->Close();

    SAFE_DELETE(gCounters);
    SAFE_DELETE(gGame);
    SAFE_DELETE(gCamera);
    SAFE_DELETE(gLog);
    SAFE_DELETE(gClient);
    SAFE_DELETE(gServer);
    SAFE_DELETE(gScene);
    SAFE_DELETE(gChat);
    SAFE_DELETE(gChatLog);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::CreateScene()
{
    gScene = new Scene(context_);

    gScene->CreateComponent<Octree>(LOCAL);
    gPhysicsWorld = gScene->CreateComponent<PhysicsWorld>(LOCAL);

    if(gTypeApplication == Type_Client)
    {
        gDebugRenderer = gScene->CreateComponent<DebugRenderer>(LOCAL);
    }

    Node* zoneNode = gScene->CreateChild("Zone", LOCAL);
    Zone* zone = zoneNode->CreateComponent<Zone>();
    zone->SetAmbientColor(Color(0.15f, 0.15f, 0.15f));
    zone->SetFogColor(Color(0.5f, 0.5f, 0.7f));
    zone->SetFogStart(4000.0f);
    zone->SetFogEnd(5000.0f);
    zone->SetBoundingBox(BoundingBox(-2000.0f, 2000.0f));

    // Create a directional light with cascaded shadow mapping
    Node* lightNode = gScene->CreateChild("DirectionalLight", LOCAL);
    lightNode->SetDirection(Vector3(0.3f, -0.5f, 0.425f));
    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetCastShadows(true);
    light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
    light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));
    light->SetSpecularIntensity(0.5f);

    // Create heightmap terrain with collision
    Node* terrainNode = gScene->CreateChild("Terrain");
    terrainNode->SetPosition(Vector3::ZERO);
    Terrain* terrain = terrainNode->CreateComponent<Terrain>();
    terrain->SetViewMask(VIEW_MASK_TERRAIN);
    terrain->SetPatchSize(64);
    terrain->SetSpacing(Vector3(2.0f, 1.0f, 2.0f)); // Spacing between vertices and vertical resolution of the height map
    terrain->SetSmoothing(true);
    terrain->SetHeightMap(gCache->GetResource<Image>("Landscape/land1.png"));
    terrain->SetMaterial(gCache->GetResource<Material>("Landscape/Terrain.xml"));
    // The terrain consists of large triangles, which fits well for occlusion rendering, as a hill can occlude all
    // terrain patches and other objects behind it
    terrain->SetOccluder(true);

    RigidBody* body = terrainNode->CreateComponent<RigidBody>();
    body->SetCollisionLayer(1); // Use layer bitmask 2 for static geometry
    body->SetFriction(1.0f);
    CollisionShape* shape = terrainNode->CreateComponent<CollisionShape>();
    shape->SetTerrain();

    //gScene->GetComponent<PhysicsWorld>()->SetFps(300);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::SubscribeToEvents()
{
    if (gTypeApplication == Type_Client)
    {
        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(Tankist, HandleKeyDown));
        SubscribeToEvent(E_KEYUP, URHO3D_HANDLER(Tankist, HandleKeyUp));
        SubscribeToEvent(E_CHANGELANGUAGE, URHO3D_HANDLER(Tankist, HandleLanguageChanged));
        SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(Tankist, HandlePostRenderUpdate));
    }

    SubscribeToEvent(E_POSTUPDATE, URHO3D_HANDLER(Tankist, HandlePostUpdate));
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::CreateUI()
{
#ifdef WIN32
    XMLFile* uiStyle = gCache->GetResource<XMLFile>("UI/TankistStyle.xml");
    // Set style to the UI root so that elements will inherit it
    gUIRoot->SetDefaultStyle(uiStyle);

    // Create a Cursor UI element because we want to be able to hide and show it at will. When hidden, the mouse cursor will
    // control the camera, and when visible, it can interact with the login UI
    SharedPtr<Cursor> cursor(new Cursor(context_));
    cursor->SetStyleAuto(uiStyle);
    gUI->SetCursor(cursor);
    // Set starting position of the cursor at the rendering window center
    cursor->SetPosition(gGraphics->GetWidth() / 2, gGraphics->GetHeight() / 2);

    SharedPtr<UIElement> container(gUIRoot->CreateChild<UIElement>());
    container->SetFixedSize(200, 300);
    container->SetPosition(0, 100);
    container->SetLayoutMode(LM_VERTICAL);
    container->SetStyleAuto();

    statisticsWindow = gUIRoot->CreateChild<Text>();
    statisticsWindow->SetStyleAuto();
    statisticsWindow->SetPosition(gUIRoot->GetWidth() - 200, 0);
    statisticsWindow->SetColor(Color::BLACK);

    gWindowGameESC = new WindowGameESC(context_);

    gWindowSettings = new WindowSettings(context_);

    gGameGUI = new GameGUI(context_);
#endif
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::UpdateCamera()
{
    if (gTypeApplication != Type_Server)
    gUI->GetCursor()->SetVisible(!gInput->GetMouseButtonDown(MOUSEB_RIGHT));

    gCamera->MoveFromMouse();

    if (gTypeApplication == Type_Client)
    {
        static bool cameraIsAttached = false;

        if (!cameraIsAttached)
        {
            cameraIsAttached = gClient->AttachCameraToNode();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::SetWindowTitleAndIcon()
{
    Image* icon = gCache->GetResource<Image>("Textures/UrhoIcon.png");
    gGraphics->SetWindowIcon(icon);
    //gGraphics->SetWindowTitle("Tankist WaT");
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::CreateConsoleAndDebugHud()
{
    XMLFile* xmlFile = gCache->GetResource<XMLFile>("UI/TankistStyle.xml");

    Console* console = engine_->CreateConsole();
    gConsole = GetSubsystem<Console>();
    console->SetDefaultStyle(xmlFile);
    console->GetBackground()->SetOpacity(0.8f);

    gDebugHud = engine_->CreateDebugHud();
    gDebugHud->SetDefaultStyle(xmlFile);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
bool Tankist::ParseArguments(Vector<String> &arguments, TypeApplication &type, String &address, unsigned short &port)
{
    if (arguments.Size() == 2 && arguments[0] == "-server")
    {
        if (arguments[1].Length() > 6 && GetNumPort(arguments[1], port))     // "-port:"
        {
            type = Type_Server;
            return true;
        }
    }

    if (arguments.Size() == 3 && arguments[0] == "-client")
    {
        if (arguments[1].Length() > 9)   // "-address:"
        {
            address = arguments[1].Substring(9);

            if (GetNumPort(arguments[2], port))
            {
                type = Type_Client;
                return true;
            }
        }
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
bool Tankist::GetNumPort(String &str, unsigned short &port)
{
    String strPort = str.Substring(6);

    long int numPort = strtol(strPort.CString(), nullptr, 10);

    if (numPort == 0L)
    {
        return false;
    }

    port = (unsigned short)numPort;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::CreateInstructions()
{
    Text *instructionText = gUIRoot->CreateChild<Text>();
    instructionText->SetText("Press F12 to help");
    instructionText->SetFont(gCache->GetResource<Font>("Fonts/CRL.ttf"), 10);

    instructionText->SetHorizontalAlignment(HA_LEFT);
    instructionText->SetVerticalAlignment(VA_TOP);
    instructionText->SetPosition(0, 0);

    instructionText = gUIRoot->CreateChild<Text>(INSTRUCTION);

    instructionText->SetFont(gCache->GetResource<Font>("Fonts/CRL.ttf"), 15);
    instructionText->SetTextAlignment(HA_CENTER);
    instructionText->SetHorizontalAlignment(HA_CENTER);
    instructionText->SetVerticalAlignment(VA_CENTER);
    instructionText->SetPosition(0, gUI->GetRoot()->GetHeight() / 4);
    instructionText->SetVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::UpdateStatisticWindow()
{
    statisticsWindow->SetText(ToString("%d users online\n\n", numClients) +
                              String("speed in = ") + String((int)(bytesInPerSec / 1024.0f + 0.5f)) + String(" kB/s\n") +
                              String("speed out = ") + String((int)(bytesOutPerSec / 1024.0f + 0.5f)) + String(" kB/s\n") +
                              String("ping = ") + String((int)(ping * 1000.0f)) + String(" ms\n\n") +
                              String("load server CPU = ") + String(loadCPU * 100.0f) + " %\n" +
                              String("server in = ") + String((int)(bytesInPerSecServer / 1024.0f + 0.5f)) + String(" kB/s\n") +
                              String("server out = ") + String((int)(bytesOutPerSecServer / 1024.0f + 0.5f)) + String(" kB/s\n")
                              );
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::SetPing(float pingMS)
{
    this->ping = pingMS;
    UpdateStatisticWindow();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::SetLoadCPU(float loadCPU_)
{
    loadCPU = loadCPU_;
    UpdateStatisticWindow();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::SetNumClients(int numClients_)
{
    numClients = numClients_;
    UpdateStatisticWindow();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::SetBytesInPerSec(float bytesInPerSec_)
{
    bytesInPerSec = bytesInPerSec_;
    UpdateStatisticWindow();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::SetBytesOutPerSec(float bytesOutPerSec_)
{
    bytesOutPerSec = bytesOutPerSec_;
    UpdateStatisticWindow();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::SetBytesInPerSecServer(float bytes)
{
    bytesInPerSecServer = bytes;
    UpdateStatisticWindow();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::SetBytesOutPerSecServer(float bytes)
{
    bytesOutPerSecServer = bytes;
    UpdateStatisticWindow();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Tankist::Exit()
{
    engine_->Exit();
}

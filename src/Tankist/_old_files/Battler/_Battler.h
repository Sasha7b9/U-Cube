#pragma once



#define VIEW_MASK_TERRAIN 1 << 10       ///< ����� ��������� ��� ���������


enum TypeApplication
{
    Type_None,      ///< ����� ��������������
    Type_Server,    ///< ��� ���������� - ������
    Type_Client     ///< ��� ���������� - ������
};



class Tank;


class Battler : public Application
{
    URHO3D_OBJECT(Battler, Application);

    friend class Client;

public:
    Battler(Context* context);
    
    // ������� ���������� ������� ����� �� ����� �������������
    virtual void Setup();
    // ������� ���������� �������
    virtual void Start();
    // ������� ���������� �������
    virtual void Stop();
    // ������� ���������� ��� �������� ����
    void Exit();
    
private:

    // ������� ���������� ������� Client ��� ���������� ���������� � ������� �����
    void SetPing(float pingMS);
    // ������� ���������� ������� Client ��� ���������� ���������� � �������� ���
    void SetLoadCPU(float loadCPU);
    // ������� ���������� ������� Client ��� ���������� ���������� � ���������� ������������ � ������� ��������
    void SetNumClients(int numClients);
    // ������� ���������� ������� Client ��� ���������� ���������� � �������� ��������� ����������
    void SetBytesInPerSec(float bytesInPerSec);
    // ������� ���������� ������� Client ��� ���������� ���������� � �������� ���������� ����������
    void SetBytesOutPerSec(float bytesOutPerSec);
    // ������� ���������� ������� Client ��� ���������� ���������� � ��������� �������� �������� �������
    void SetBytesInPerSecServer(float bytes);
    // ������� ���������� ������� Client ��� ���������� ���������� � ��������� ��������� �������� �������
    void SetBytesOutPerSecServer(float bytes);
    // �������� �����
    void CreateScene();

    void CreateInstructions();

    void CreateUI();
    // ������� ���������� ������ ����
    void UpdateCamera();

    void SetWindowTitleAndIcon();

    void CreateConsoleAndDebugHud();
    // ������� ������� ���������� ��������� ������
    bool ParseArguments(Vector<String> &arguments, TypeApplication &type, String &address, unsigned short &port);

    bool GetNumPort(String &str, unsigned short &port);

    void HandleKeyDown(StringHash, VariantMap&);

    void HandleKeyUp(StringHash, VariantMap&);

    void HandlePostUpdate(StringHash, VariantMap&);

    void HandleLanguageChanged(StringHash, VariantMap&);

    void HandlePostRenderUpdate(StringHash, VariantMap&);

    void SubscribeToEvents();

    Battler(Battler const&) : Application(nullptr) {};
    Battler operator=(Battler const &) { return *this; };

    bool exit = false;

    SharedPtr<Text> statisticsWindow;

    float ping = 0.0f;

    float loadCPU = 0.0f;

    int numClients = 0;

    float bytesInPerSec = 0.0f;

    float bytesOutPerSec = 0.0f;

    float bytesInPerSecServer = 0.0f;

    float bytesOutPerSecServer = 0.0f;

    void UpdateStatisticWindow();
};

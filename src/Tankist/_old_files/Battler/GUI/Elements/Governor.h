#pragma once



class Governor : public Object
{
    URHO3D_OBJECT(Governor, Object)

public:
    Governor(Context *context, UIElement *parent, const char *nameGovernor, int min = 0, int max = 100);

    int GetValue();

    void SetValue(int value);

    void SaveValue()
    {
        savedValue = value;
    };
    void RestoreValue()
    {
        SetValue(savedValue);
    };

private:
    Governor(Governor const&) : Object(nullptr) {};
    Governor operator=(Governor const&) { return *this; };


    void HandleButtonLess(StringHash, VariantMap&);

    void HandleButtonMore(StringHash, VariantMap&);

    void HandleMouseDown(StringHash, VariantMap&);

    void HandleMouseMove(StringHash, VariantMap&);

    void HandleMouseUp(StringHash, VariantMap&);

    void Inc();

    void Dec();

    void UpdateSlider();

    void UpdateText();

    Button *btnDirect = nullptr;        // Directing for the movement of the slider

    Button *slider = nullptr;

    Text *tValue = nullptr;             // Text control for display value

    int value = 0;

    int min = 0;

    int max = 0;

    int savedValue = 0;

    bool mousePressed = false;

    float scale = 1.0f;
};

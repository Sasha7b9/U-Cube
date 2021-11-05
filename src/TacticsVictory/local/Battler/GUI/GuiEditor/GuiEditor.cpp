// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Editor/Editor.h"
#include "FileSystem/ConfigurationFile_v.h"
#include "GUI/GUI.h"
#include "GUI/Controls/DropDownListWithTextAndButton_.h"
#include "GUI/Controls/Label_.h"
#include "GUI/Controls/Tab_.h"
#include "GUI/Controls/Buttons/ButtonSwitch_.h"
#include "GUI/Controls/Sliders/SliderWithTextAndButtons_.h"
#include "GUI/GuiEditor/GuiEditor.h"
#include "GUI/Logic/LineTranslator2D_.h"
#include "GUI/Panels/PanelBottom.h"
#include "GUI/Panels/PanelMain.h"
#include "GUI/Panels/PanelMap.h"
#include "GUI/Windows/Console_.h"
#include "Scene/SceneC.h"
//#include "Scene/Cameras/Camera.h"
#include "Scene/Terrain/Level_.h"
#include "Utils/GlobalFunctions_.h"


GuiEditor::GuiEditor(Context* context) : Object(context)
{
    CreatePanels();

    CreateTabs();

    CreateWindows();

    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(GuiEditor, HandleKeyDown));
}


void GuiEditor::CreatePanels()
{
    panelMap = new PanelMap(TheContext);
    TheUIRoot->AddChild(panelMap);

    panelMain = new PanelMain(TheContext);
    TheUIRoot->AddChild(panelMain);

    // Panel bottom
    panelBottom = new PanelBottom(TheContext);
    panelBottom->SetPosition(0, TheSettings.GetInt("screen", "height") -
        TheSettings.GetInt("panel", "bottom", "height"));

    width = TheSettings.GetInt("panel", "bottom", "button_width");
    height = TheSettings.GetInt("panel", "bottom", "button_height");

    x = TheSettings.GetInt("panel_map", "width") / 2 - width / 2;
    y = TheSettings.GetInt("panel", "bottom", "y");

    buttonInterface = panelBottom->AddButton("Interface", x, y, width, height);
    SubscribeToEvent(buttonInterface, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleButtonRelease));

    x = TheSettings.GetInt("screen", "width") - 2 * width;
    buttonMenu = panelBottom->AddButton("Menu", x, y, width, height);
    SubscribeToEvent(buttonMenu, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleButtonRelease));

    TheUIRoot->AddChild(panelBottom);
    panelBottom->BringToFront();
}


void GuiEditor::CreateTabs()
{
    CreateTabFile();
    CreateTabEdit();
    CreateTabTerrain();
    CreateTabObjects();
}


void GuiEditor::CreateTabFile()
{
    SharedPtr<Tab> tabFile(Tab::Create("File"));
    panelMain->AddTab(tabFile);

    SharedPtr<ButtonT> btnFileOpen = tabFile->AddButton("Load", 10, y, width, height);
    btnFileOpen->SetHint("loadMapFromFile");
    SubscribeToEvent(btnFileOpen, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleFileLoad));

    SharedPtr<ButtonT> btnFileSave = tabFile->AddButton("Save", 10, y += dY, width, height);
    btnFileSave->SetHint("saveMapToFile");
    SubscribeToEvent(btnFileSave, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleFileSave));
}


void GuiEditor::CreateTabEdit()
{
    SharedPtr<Tab> tabEdit(Tab::Create("Edit"));
    panelMain->AddTab(tabEdit);

    y = 10;

    SharedPtr<ButtonT> btnEditUndo = tabEdit->AddButton("Undo", x, y, width, height);
    btnEditUndo->SetHint("hintEditUndo");
    SubscribeToEvent(btnEditUndo, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleEditUndo));

    SharedPtr<ButtonT>  btnEditRedo = tabEdit->AddButton("Redo", x, y += dY, width, height);
    btnEditRedo->SetHint("hintEditRedo");
    SubscribeToEvent(btnEditRedo, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleEditRedo));
}


void GuiEditor::CreateTabTerrain()
{
    SharedPtr<Tab> tabTerrain(Tab::Create("Terrain"));
    panelMain->AddTab(tabTerrain);

    btnNewMap = tabTerrain->AddButton("New map", 10, y = 10, width, height);
    btnNewMap->SetHint("createNewMap");
    SubscribeToEvent(btnNewMap, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleButtonRelease));

    SharedPtr<ButtonT> btnClearTerrain = tabTerrain->AddButton("Clear", 10, y += dY, width, height);
    btnClearTerrain->SetHint("clearTerrain");
    SubscribeToEvent(btnClearTerrain, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleTerrainClearTerrain));

    SharedPtr<SliderWithTextAndButtons> sliderHeightDefault = tabTerrain->AddSlider("", -100, 100, 1,
        btnClearTerrain->GetPosition().x_ + btnClearTerrain->GetWidth() + 5, y - 2, 0, 20);
    sliderHeightDefault->SetValue(0);
    sliderHeightDefault->SetHint("hintSliderDefaultHeight");

    SharedPtr<SliderWithTextAndButtons> sliderSizeBrushX =
        tabTerrain->AddSlider("Brush X", 1, 10, 1, 10, y += dY, 75, 50);
    sliderSizeBrushX->SetValue(1);
    sliderSizeBrushX->SetHint("hintSliderSizeBrushX");

    SharedPtr<SliderWithTextAndButtons> sliderSizeBrushY =
        tabTerrain->AddSlider("Brush Y", 1, 10, 1, 10, y += dY, 75, 50);
    sliderSizeBrushY->SetValue(1);
    sliderSizeBrushY->SetHint("hintSliderSizeBrushY");

    pchar items[] = {"TPlane", "Edge"};
    SharedPtr<DropDownListWithTextAndButton> ddListModeSelect =
        tabTerrain->AddDDList("Mode select", 100, 80, 2, items, 10, y += dY);
    SubscribeToEvent(ddListModeSelect, E_ITEMSELECTED, URHO3D_HANDLER(GuiEditor, HandleTerrainModeSelectChanged));
}


void GuiEditor::CreateTabObjects()
{
    SharedPtr<Tab> tabObjects(Tab::Create("Objects"));
    panelMain->AddTab(tabObjects);

    y = 10;

    SharedPtr<ButtonT> btnObjectsAdd = tabObjects->AddButton("Add", x, y, width, height);
    btnObjectsAdd->SetHint("hintObjectsAdd");
    SubscribeToEvent(btnObjectsAdd, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleObjectsAdd));
}


void GuiEditor::ToggleInterfacePanels()
{
    LineTranslator2D::State stateMap = panelMap->GetTranslator()->GetState();
    LineTranslator2D::State stateMain = panelMain->GetTranslator()->GetState();

    if(stateMap == LineTranslator2D::State_PointStart && stateMain == LineTranslator2D::State_PointStart)
    {
        panelMain->Toggle();
    }
    else if(stateMap == LineTranslator2D::State_PointStart && stateMain == LineTranslator2D::State_PointFinish)
    {
        panelMap->Toggle();
    }
    else if(stateMap == LineTranslator2D::State_PointFinish && stateMain == LineTranslator2D::State_PointFinish)
    {
        panelMap->Toggle();
        panelMain->Toggle();
    }
}


void GuiEditor::HandleButtonRelease(StringHash, VariantMap &eventData)
{
    ButtonT *button = (ButtonT*)eventData[Released::P_ELEMENT].GetPtr();

    if (button == buttonInterface)
    {
        ToggleInterfacePanels();
    }
    else if (button == btnNewMap)
    {
        windowNewMap->SetVisible(!windowNewMap->IsVisible());
        if(windowNewMap->IsVisible())
        {
            TheOpenedWindow.Push(windowNewMap);
            windowNewMap->BringToFront();
        }
    }
    else if(button == buttonMenu)
    {
        windowMenu->SetVisible(!windowMenu->IsVisible());
        if(windowMenu->IsVisible())
        {
            TheOpenedWindow.Push(windowMenu);
            windowMenu->SetPosition(
                buttonMenu->GetPosition().x_,
                panelBottom->GetPosition().y_ - windowMenu->GetHeight());
            windowMenu->BringToFront();
        }
    }
}


bool GuiEditor::IntersectionX(const ButtonT *button, int x_)
{
    return x_ >= button->GetPosition().x_ && x_ <= button->GetPosition().x_ + button->GetWidth();
}


bool GuiEditor::CheckOnDeadZoneForCursorBottomScreen(int x_)
{
    return IntersectionX(buttonInterface, x_) || IntersectionX(buttonMenu, x_);
}


bool GuiEditor::IsInside(const IntVector2 &position)
{
    return IsVisible() && 
        (
            panelBottom->IsInside(position, true) || 
            panelMain->IsInside(position, true) || 
            panelMap->IsInside(position, true) ||
            windowNewMap->IsInside(position, true)
        ) && 
        position.x_ > 0 && 
        position.x_ < TheSettings.GetInt("screen", "width") - 1 &&
        position.y_ < TheSettings.GetInt("screen", "height") - 1;
}


void GuiEditor::CreateWindows()
{
    // window new map
    windowNewMap = new Control(TheContext);
    windowNewMap->SetLayout(LM_VERTICAL, 0, IntRect(6, 6, 6, 6));

    windowNewMap->AddLabel("CreateNewMap");
    sliderSizeNewMapX = windowNewMap->AddSlider("Dimension X", 50, 250, 50);
    sliderSizeNewMapX->SetValue(150);
    sliderSizeNewMapY = windowNewMap->AddSlider("Dimension Y", 50, 250, 50);
    sliderSizeNewMapY->SetValue(150);
    SharedPtr<ButtonT> buttonCreateMap = windowNewMap->AddButton("Create");
    SubscribeToEvent(buttonCreateMap, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleTerrainCreateNewMap));

    windowNewMap->SetFixedSize(windowNewMap->GetSize());
    GF::SetWindowInCenterScreen(windowNewMap);
    TheUIRoot->AddChild(windowNewMap);
    windowNewMap->SetVisible(false);

    // window menu
    windowMenu = new Control(TheContext);
    windowMenu->SetLayout(LM_VERTICAL, 0, IntRect(0, 0, 0, 0));

    SharedPtr<ButtonT> buttonOptions = windowMenu->AddButton("Options");
    SubscribeToEvent(buttonOptions, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleOptions));
    SharedPtr<ButtonT> buttonExit = windowMenu->AddButton("Exit");
    SubscribeToEvent(buttonExit, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleExit));

    windowMenu->SetFixedWidth(buttonMenu->GetWidth());
    windowMenu->SetFixedSize(windowMenu->GetSize());
    TheUIRoot->AddChild(windowMenu);
    windowMenu->SetVisible(false);

    // window confirm exit
    windowConfirmExit = new Control(TheContext);
    windowConfirmExit->SetLayout(LM_VERTICAL, 0, IntRect(6, 6, 6, 6));

    SharedPtr<Label> label(Label::Create("Exit in main menu?"));
    windowConfirmExit->AddChild(label);

    SharedPtr<UIElement> layer(windowConfirmExit->CreateChild<UIElement>());
    layer->SetLayout(LM_HORIZONTAL, 6, IntRect(6, 6, 6, 6));

    SharedPtr<ButtonT> buttonOk(new ButtonT(layer, "Ok"));
    SubscribeToEvent(buttonOk, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleExitOk));

    SharedPtr<ButtonT> buttonCancel(new ButtonT(layer, "Cancel"));
    SubscribeToEvent(buttonCancel, E_RELEASED, URHO3D_HANDLER(GuiEditor, HandleExitCancel));

    windowConfirmExit->AddChild(layer);

    TheUIRoot->AddChild(windowConfirmExit);
    windowConfirmExit->SetVisible(false);
    GF::SetWindowInCenterScreen(windowConfirmExit);

    SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(GuiEditor, HandleMouseDown));
}


void GuiEditor::HandleTerrainCreateNewMap(StringHash, VariantMap&)
{
    Vector<Vector<float>> map = TheTerrain->level->CreateRandom(
        (uint)sliderSizeNewMapY->GetValue(),
        (uint)sliderSizeNewMapX->GetValue());

    delete TheTerrain;
    TheTerrain = new TerrainT();
    TheTerrain->CreateFromVector(map);
    windowNewMap->SetVisible(false);

    TheCamera->SetPosition(
        {TheTerrain->WidthZ() / 2.0f, 20.0f, -(float)TheTerrain->HeightX()},
        {TheTerrain->WidthZ() / 2.0f, 0.0f, -(TheTerrain->HeightX() / 2.0f)});
}


void GuiEditor::HandleTerrainClearTerrain(StringHash, VariantMap&)
{
    for (uint row = 0; row < TheTerrain->HeightX(); row++)
    {
        for (uint col = 0; col < TheTerrain->WidthZ(); col++)
        {
            TheTerrain->level->SetHeight(row, col, 0.0f);
        }
    }
}


void GuiEditor::HandleKeyDown(StringHash, VariantMap& eventData)
{
    if (!IsVisible() || TheConsole->IsActive())
    {
        return;
    }

    int key = eventData[KeyDown::P_KEY].GetInt();

    if (TheFileSelector->GetWindow()->IsVisible())
    {
        if (KEY_IS_ESC)
        {
            UnsubscribeFromEvent(TheFileSelector, E_FILESELECTED);
            TheFileSelector->GetWindow()->SetVisible(false);
            TheCamera->SetEnabled(true);
        }
        return;
    }

    if (KEY_IS_ESC)
    {
        if(!TheOpenedWindow.Empty())
        {
            Control *window = TheOpenedWindow.Back();
            window->SetVisible(false);
            TheOpenedWindow.Remove(window);
        }
    }
    else if (KEY_IS_I)
    {
        ToggleInterfacePanels();
    }
}


void GuiEditor::HandleFileLoad(StringHash, VariantMap&)
{
    TheCamera->SetEnabled(false);

    Vector<std::string> filters;

    filters.Push("*.map");

    GF::OpenFileSelector("Load landscape", "Load", "Cancel", filters);

    SubscribeToEvent(TheFileSelector, E_FILESELECTED, URHO3D_HANDLER(GuiEditor, HandleFileSelectorLoadTerrain));
}


void GuiEditor::HandleFileSave(StringHash, VariantMap&)
{
    TheCamera->SetEnabled(false);

    Vector<std::string> filters;

    filters.Push("*.map");

    GF::OpenFileSelector("Save landscape", "Save", "Cancel", filters);

    SubscribeToEvent(TheFileSelector, E_FILESELECTED, URHO3D_HANDLER(GuiEditor, HandleFileSelectorSaveTerrain));
}


void GuiEditor::HandleFileSelectorLoadTerrain(StringHash, VariantMap& eventData)
{
    UnsubscribeFromEvent(TheFileSelector, E_FILESELECTED);

    bool ok = (bool)eventData[FileSelected::P_OK].GetBool();

    if(ok)
    {
        TheTerrain->level->Load((char*)((String)eventData[FileSelected::P_FILENAME].GetString()).CString());

        delete TheTerrain;
        TheTerrain = new TerrainT();
        TheTerrain->CreateFromVector(TheTerrain->level->map);
        TheCamera->SetPosition(
            {TheTerrain->WidthZ() / 2.0f, 20.0f, -(float)TheTerrain->HeightX()},
            {TheTerrain->WidthZ() / 2.0f, 0.0f, -(TheTerrain->HeightX() / 2.0f)});
    }
    
    TheFileSelector->GetWindow()->SetVisible(false);

    TheCamera->SetEnabled(true);
}


void GuiEditor::HandleFileSelectorSaveTerrain(StringHash, VariantMap& eventData)
{
    UnsubscribeFromEvent(TheFileSelector, E_FILESELECTED);

    bool ok = (bool)eventData[FileSelected::P_OK].GetBool();

    if (ok)
    {
        String fileName = (String)eventData[FileSelected::P_FILENAME].GetString();
        fileName = ReplaceExtension(fileName, ".map");
        TheTerrain->level->map = TheTerrain->GetHeightMap();
        TheTerrain->level->Save(fileName);
    }

    TheFileSelector->GetWindow()->SetVisible(false);

    TheCamera->SetEnabled(true);
}


void GuiEditor::HandleExit(StringHash, VariantMap&)
{
    TheGUI->SetVisibleControl(windowConfirmExit, true);
}


void GuiEditor::HandleExitOk(StringHash, VariantMap&)
{
    TheGuiEditor->SetVisible(false);
    TheCamera->SetEnabled(false);
    TheEditor->ClearScene();
    TheGUI->SetVisibleControl(windowConfirmExit, false);
}


void GuiEditor::HandleExitCancel(StringHash, VariantMap&)
{
    TheGUI->SetVisibleControl(windowConfirmExit, false);
}


void GuiEditor::HandleOptions(StringHash, VariantMap&)
{

}


void GuiEditor::HandleMouseDown(StringHash, VariantMap&)
{
    
}


void GuiEditor::HandleTerrainModeSelectChanged(StringHash, VariantMap& eventData)
{
    int index = eventData[ItemSelected::P_SELECTION].GetInt();

    modeSelect = (ModeSelect)index;
}


void GuiEditor::HandleEditUndo(StringHash, VariantMap&)
{

}


void GuiEditor::HandleEditRedo(StringHash, VariantMap&)
{

}


void GuiEditor::HandleObjectsAdd(StringHash, VariantMap&)
{

}


bool GuiEditor::IsVisible()
{
    return panelMap->IsVisible();
}


void GuiEditor::SetVisible(bool visible)
{
    panelMap->SetVisible(visible);
    panelBottom->SetVisible(visible);
    panelMain->SetVisible(visible);
}

// 2021/03/26 23:38:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "GUI/Menu/PageFindServer.h"


PageFindServer::PageFindServer() : MenuPage()
{
    SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
    SetName("Find server");

    SharedPtr<Label> text(Label::Create("Find server", 20, 120, -1));
    text->SetAlignment(HA_CENTER, VA_CENTER);
    text->SetTextAlignment(HA_CENTER);
    AddChild(text);

    tableServers = new OrderedTable(this, "List of available servers");

    buttonConnect = new ButtonT(this, "Connect");
    SubscribeToEvent(buttonConnect, E_RELEASED, URHO3D_HANDLER(PageFindServer, HandleButtonRelease));

    buttonBack = new ButtonT(this, "Back");
    SubscribeToEvent(buttonBack, E_RELEASED, URHO3D_HANDLER(PageFindServer, HandleButtonRelease));
}


void PageFindServer::HandleButtonRelease(StringHash, VariantMap &eventData)
{
    using namespace Released;

    Button *button = (Button *)(eventData[P_ELEMENT].GetPtr());

    if (button == buttonBack)
    {
        SendEventClose();
    }
}


void PageFindServer::SetServersInfo(const std::string &data)
{
    std::string info(data);

    tableServers->SetServersInfo(info);
}

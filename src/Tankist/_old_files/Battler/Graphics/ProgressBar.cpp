/* (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by */
#include <stdafx.h>
#include <Urho3D/UI/Sprite.h>
#include "ProgressBar.h"



ProgressBar::ProgressBar(Context *context) : UIElement(context)
{
    sprite = new SpriteT(context);
    sprite->SetSize((int)width, (int)height);
    AddChild(sprite);

    Font *font = TheCache->GetResource<Font>("Fonts/Anonymous Pro.ttf");

    textPercents = new Text(context);
    textPercents->SetFont(font);
    textPercents->SetFixedSize((int)height, (int)height);
    textPercents->SetPosition((int)width + 3, 2);
    AddChild(textPercents);

    text = new Text(context);
    text->SetFont(font);
    text->SetPosition(-100, -60);
    AddChild(text);

    SetWidth(sprite->GetWidth() + textPercents->GetWidth());

    DrawProgress();
}


void ProgressBar::SetParameters(float progress_, float timePassed_, float timeElapsed_, float speed_, String currentFile_)
{
    this->currentFile = currentFile_;
    this->progress = progress_;
    this->timePassed = timePassed_;
    this->timeElapsed = timeElapsed_;
    this->speed = speed_;

    DrawProgress();
}


void ProgressBar::SetBytes(int all, int recieved)
{
    bytesAll = all;
    bytesRecieved = recieved;

    DrawProgress();
}


void ProgressBar::DrawProgress()
{
    sprite->Clear(Color::GRAY);
    sprite->FillRectangle(0, 0, (int)(width * progress), (int)height, Color::BLUE);

    char buffer[100];
    sprintf(buffer, "%5.1f%%", progress * 100.0f);

    textPercents->SetText(String(buffer));

    sprintf(buffer, "Size: all %5.1fMB, recieved %5.1fMB\n", bytesAll / 1024.0f / 1024.0f, bytesRecieved / 1024.0f / 1024.0f);
    String str1(buffer);

    sprintf(buffer, "Speed %5.1fkB/s, time: passed %5.1fs, elapsed %5.1fs", speed / 1024.0f, timePassed, timeElapsed);
    String str2(buffer);

    text->SetText(currentFile + String("\n") + str1 + str2);
}

#include <Urho3D/Graphics/Texture2D.h>

#include "lSprite.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
lSprite::lSprite(Context *context) : Sprite(context)
{
    image = new lImage(context);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
lSprite::~lSprite()
{

}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void lSprite::SetSize(int width, int height)
{
    SetFixedSize(width, height);
    image->SetSize(width, height);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void lSprite::Clear(const Color &color)
{
    FillRectangle(0, 0, GetWidth(), GetHeight(), color);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void lSprite::FillRectangle(int x, int y, int width, int height, const Color &color)
{
    image->FillRectangle(x, y, width, height, color);

    SharedPtr<Texture2D> texture(new Texture2D(context_));
    texture->SetSize(image->GetWidth(), image->GetHeight(), /*D3DFMT_X8R8G8B8*/ 22);
    texture->SetData((SharedPtr<Image>)image);

    SetTexture(texture);
    SetFullImageRect();
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
Texture* lSprite::GetTexture()
{
    return Sprite::GetTexture();
}

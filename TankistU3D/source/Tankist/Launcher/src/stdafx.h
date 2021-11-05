#pragma once


#pragma warning(push)
#pragma warning(disable:4100 4263 4264 4265 4266 4312 4365 4514 4625 4626 4640 4668 4710 4571)


#define URHO3D_LOGGING


#include <Urho3D/Container/Str.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Core/Thread.h>
#include <Urho3D/Core/Timer.h>

#include <Urho3D/Engine/Application.h>

#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>

#include <Urho3D/Input/Input.h>

#include <Urho3D/IO/File.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/Log.h>

#include <Urho3D/Resource/Resource.h>
#include <Urho3D/Resource/ResourceCache.h>

#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>

#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Window.h>


#include <d3d9types.h>

#include <thread>
#include <cstdio>


#pragma warning(pop)


using Urho3D::Application;

using Urho3D::BoundingBox;
using Urho3D::Button;

using Urho3D::Color;
using Urho3D::Context;
using Urho3D::Cursor;

using Urho3D::File;
using Urho3D::FileSystem;
using Urho3D::Font;

using Urho3D::Graphics;

using Urho3D::Image;
using Urho3D::Input;
using Urho3D::IntVector2;

using Urho3D::HashMap;

using Urho3D::Log;

using Urho3D::Model;

using Urho3D::Node;

using Urho3D::ResourceCache;

using Urho3D::Scene;
using Urho3D::SharedPtr;
using Urho3D::Sprite;
using Urho3D::StaticModel;
using Urho3D::String;
using Urho3D::StringHash;

using Urho3D::Text;
using Urho3D::Texture2D;
using Urho3D::Time;
using Urho3D::Thread;

using Urho3D::UI;
using Urho3D::UIElement;

using Urho3D::XMLFile;

using Urho3D::VariantMap;
using Urho3D::Vector;
using Urho3D::Vector3;

using Urho3D::Window;


using Urho3D::Sin;
using Urho3D::Cos;


using Urho3D::E_POSTUPDATE;


#include "GlobalVars.h"

#include "defines.h"


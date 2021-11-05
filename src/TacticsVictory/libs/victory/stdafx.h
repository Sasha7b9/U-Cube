// 2021/04/22 11:56:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#ifdef WIN32
#define _WINSOCKAPI_
#define WIN32_LEAN_AND_MEAN     // ��� ���������� �������������� warning C4668: '_WIN32_WINNT_WIN10_TH2' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#pragma warning(push, 0)
#include <windows.h>
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif


#include <Urho3D/Container/Str.h>
#include <Urho3D/Core/StringUtils.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Math/Ray.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/Cursor.h>
#include <Urho3D/UI/FileSelector.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>

using namespace Urho3D;


#undef Bool
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>

#include <sockpp/tcp_acceptor.h>
#include <sockpp/tcp_connector.h>
#include <sockpp/udp_socket.h>


#ifdef WIN32
#pragma warning(pop)
#else
#endif


#include "defines.h"
#include "victory.h"
#include "Utils/Log_v.h"

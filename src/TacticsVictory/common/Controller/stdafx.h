// 2021/04/02 17:15:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#ifdef WIN32
#pragma warning(push, 0)
#pragma warning(disable: 5039)
#define WIN32_LEAN_AND_MEAN             // ��� ���������� �������������� warning C4668: '_WIN32_WINNT_WIN10_TH2' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#ifdef WIN32
#define _WINSOCKAPI_
#include <windows.h>
#endif

#include <Urho3D/Math/Vector2.h>

using namespace Urho3D;

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>

#include <sockpp/tcp_acceptor.h>
#include <sockpp/tcp_connector.h>

#ifdef WIN32
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif

#include "defines.h"
#include "globals.h"
#include "Utils/Log_v.h"


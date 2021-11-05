// 2021/04/02 17:53:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Cursor_.h"
#include "Scene/Cameras/Camera_.h"
#include "Scene/Terrain/Terrain_.h"


class Client;
class ConfigurationFile;
class ConsoleT;
class GUI;
class Hint;
class LogRAW;
class ConnectorTCP;
class ConnectorUDP;
class Menus;
class Mouse;
class PageStart;
class WindowVariables;

extern ConfigurationFile TheSettings;
extern ConnectorTCP      TheConnMaster;

extern CameraT         *TheCamera;
extern Client          *TheClient;
extern ConsoleT        *TheConsole;
extern CursorT         *TheCursor;
extern GUI             *TheGUI;
extern Hint            *TheHint;
extern LogRAW          *TheLog;
extern Menus           *TheMenu;
extern Mouse           *TheMouse;
extern TerrainT        *TheTerrain;
extern WindowVariables *TheWindowVars;

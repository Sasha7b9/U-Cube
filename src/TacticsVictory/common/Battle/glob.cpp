﻿// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "FileSystem/ConfigurationFile_v.h"
#include "Network/Other/ConnectorTCP_v.h"


Context           *TheContext = nullptr;
Engine            *TheEngine = nullptr;
FileSystem        *TheFileSystem = nullptr;
Localization      *TheLocalization = nullptr;
Log               *TheLog = nullptr;
Network           *TheNetwork = nullptr;
PhysicsWorld      *ThePhysicsWorld = nullptr;
Profiler          *TheProfiler = nullptr;
ResourceCache     *TheCache = nullptr;
//Script            *TheScript = nullptr;
Time              *TheTime = nullptr;


ConfigurationFile TheConfig;
ConfigurationFile TheSettings;


Battle            *TheBattle = nullptr;
SceneS            *TheScene = nullptr;
ServerS           *TheServer = nullptr;
TerrainT          *TheTerrain = nullptr;

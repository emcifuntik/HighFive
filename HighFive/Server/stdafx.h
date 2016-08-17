#pragma once

#define WIN32_LEAN_AND_MEAN

// Windows Header Files:
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <intrin.h>
#include <string>
#include <vector>
#include <sstream>
#include <Psapi.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <direct.h>
#include <thread>
#include <iomanip>
#pragma comment(lib, "winmm.lib")

// XML
#include <tinyxml2.h>

// Config
#include "CConfig.h"

// Logging
#include <Console/CConsole.h>
#include <Console/CLog.h>

// RakNet
#include <MessageIdentifiers.h>
#include <RakPeerInterface.h>
#include <RakNetStatistics.h>
#include <RakNetTypes.h>
#include <BitStream.h>
#include <RakSleep.h>
#include <PacketLogger.h>
#include <Gets.h>
#include <WindowsIncludes.h>
#include <GetTime.h>
#include <RPC4Plugin.h>
using namespace RakNet;

// Scripthook types
#include <../ScriptHookV/Types.h>

// Console coloring
#include "Console/CConsole.h"

// Network manager
#include "CNetworkConnection.h"

// Shared stuff from MTA
#include "../../Shared/CMath.h"
#include "../../Shared/CVector3.h"
#include "../../Shared/NetworkTypes.h"

// RPC
#include "CRPCPlugin.h"

// Squirrel
#include <squirrel.h>
#include <sqstdblob.h>
#include <sqstdsystem.h>
#include <sqstdio.h>
#include <sqstdmath.h>
#include <sqstdstring.h>
#include <sqstdaux.h>
#include <sqstdstring.h>
#include "SQ.h"
#include "Natives.h"

// Network objects
#include "CNetworkPlayer.h"
#include "CNetworkVehicle.h"


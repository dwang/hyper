#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <cstdint>
#include <utility>
#include <thread>
#include <inttypes.h>
#include <iostream>
/*
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include "menu/m_menu.hpp"
#include "menu/m_overlay.hpp"
#include "menu/m_renderer.hpp"
*/
#include "features/f_aim.hpp"

#include "sdk/cs_convar.hpp"
#include "sdk/cs_engine.hpp"
#include "sdk/cs_inputsystem.hpp"
#include "sdk/cs_interfaces.hpp"
#include "sdk/cs_netvars.hpp"
#include "sdk/cs_player.hpp"
#include "sdk/cs_types.hpp"

#include "config/c_config.hpp"
#include "config/c_files.hpp"

#include "utils/u_math.hpp"
#include "utils/u_process.hpp"
#include "utils/u_static.hpp"

#pragma once

#include "common.h"

#ifdef __linux__ 
#include <dlfcn.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include "api/Kinova.API.CommLayerUbuntu.h"
#include "api/Kinova.API.EthCommandLayerUbuntu.h"
#elif _WIN32
#include <time.h>
#include <fstream>
#include <Windows.h>
#include "api/CommunicationLayer.h"
#include "api/CommandLayer.h"
#include <conio.h>
#endif

//A handle to the API.
#ifdef __linux__ 
#define LoadAPIDef() void * commandLayer_handle
#elif _WIN32
#define LoadAPIDef() HINSTANCE commandLayer_handle
#endif

#define KinovaFuncDef(ret, name, ...) ret (*CONCAT(My, name))(__VA_ARGS__)

#ifdef __linux__
#define LoadAPI() commandLayer_handle = dlopen("api/Kinova.API.EthCommandLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL)
#define KinovaFunc(ret, name, ...) CONCAT(My, name) = (ret (*)(__VA_ARGS__)) dlsym(commandLayer_handle, TOKENIZE(CONCAT(Ethernet_, name)))
#elif _WIN32
#define commandLayer_handle = LoadLibrary(L"api\\CommandLayerWindows.dll")
#define KinovaFunc(ret, name, ...) CONCAT(My, name) = (ret (*)(__VA_ARGS__)) GetProcAddress(commandLayer_handle, TOKENIZE(CONCAT(Ethernet_, name)))
#endif

#ifdef __linux__ 
#define UnloadAPI() dlclose(commandLayer_handle);
#elif _WIN32
#define UnloadAPI() FreeLibrary(commandLayer_handle);
#endif
#pragma once

#include "common.h"

#include <dlfcn.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
// #include "api/Kinova.API.CommLayerUbuntu.h"
#include "api/Kinova.API.EthCommLayerUbuntu.h"
#include "api/Kinova.API.EthCommandLayerUbuntu.h"
// #include "api/Kinova.API.EthCommLayerUbuntu.h"
// #include "api/Kinova.API.UsbCommandLayerUbuntu.h"

#define PREFIX My

#define COMM_PREFIX 
#define ETH_PREFIX 
#define USB_PREFIX 

#define ETH_COMM_PREFIX CONCAT(ETH_PREFIX, COMM_PREFIX)
#define USB_COMM_PREFIX CONCAT(USB_PREFIX, COMM_PREFIX)

#define FUNC_DEF(name) CONCAT(PREFIX, name)

#define COMM_VAR(name) CONCAT(COMM_PREFIX, name)
#define COMM_FUNC(name) FUNC_DEF(COMM_VAR(name))

#define ETH_VAR(name) CONCAT(ETH_PREFIX, name)
#define ETH_FUNC(name) FUNC_DEF(ETH_VAR(name))

#define ETH_COMM_VAR(name) CONCAT(ETH_COMM_PREFIX, name)
#define ETH_COMM_FUNC(name) FUNC_DEF(ETH_COMM_VAR(name))

#define USB_VAR(name) CONCAT(USB_PREFIX, name)
#define USB_FUNC(name) FUNC_DEF(USB_VAR(name))

#define USB_COMM_VAR(name) CONCAT(USB_COMM_PREFIX, name)
#define USB_COMM_FUNC(name) FUNC_DEF(USB_COMM_VAR(name))

#define COMM_HANDLE COMM_VAR(Layer_handle)
#define ETH_HANDLE ETH_VAR(Layer_handle)
#define ETH_COMM_HANDLE ETH_COMM_VAR(Layer_handle)
#define USB_HANDLE USB_VAR(Layer_handle)

// API handles
#define LoadAPIDef() void * Layer_handle
#define CommLoadAPIDef() void * COMM_HANDLE
#define EthCommandLoadAPIDef() void * ETH_HANDLE
#define EthCommLoadAPIDef() void * ETH_COMM_HANDLE
#define UsbCommandLoadAPIDef() void * USB_HANDLE

// Function handles
#define KinovaFuncDef(ret, name, ...) ret (*FUNC_DEF(name))(__VA_ARGS__)
#define CommKinovaFuncDef(ret, name, ...) ret (*COMM_FUNC(name))(__VA_ARGS__)
#define EthCommandKinovaFuncDef(ret, name, ...) ret (*ETH_FUNC(name))(__VA_ARGS__)
#define EthCommKinovaFuncDef(ret, name, ...) ret (*ETH_COMM_FUNC(name))(__VA_ARGS__)
#define UsbCommandKinovaFuncDef(ret, name, ...) ret (*USB_FUNC(name))(__VA_ARGS__)

// Check API
#define Loaded res
#define CommLoaded COMM_VAR(res)
#define EthCommandLoaded ETH_VAR(res)
#define EthCommLoaded ETH_COMM_VAR(res)
#define UsbCommandLoaded USB_VAR(res)

// Load API
#define CommLoadAPI() bool CommLoaded = true; COMM_HANDLE = dlopen("../includes/api/Kinova.API.CommLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL)
#define EthCommandLoadAPI() bool EthCommandLoaded = true; ETH_HANDLE = dlopen("../includes/api/Kinova.API.EthCommandLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL)
#define EthCommLoadAPI() bool EthCommLoaded = true; ETH_COMM_HANDLE = dlopen("../includes/api/Kinova.API.EthCommLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL)
#define UsbCommandLoadAPI() bool UsbCommandLoaded = true; USB_HANDLE = dlopen("../includes/api/Kinova.API.USBCommandLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL)

#define GCommLoadAPI() bool Loaded = true; Layer_handle = dlopen("../includes/api/Kinova.API.CommLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL)
#define GEthCommandLoadAPI() bool Loaded = true; Layer_handle = dlopen("../includes/api/Kinova.API.EthCommandLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL)
#define GEthCommLoadAPI() bool Loaded = true; Layer_handle = dlopen("../includes/api/Kinova.API.EthCommLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL)
#define GUsbCommandLoadAPI() bool Loaded = true; Layer_handle = dlopen("../includes/api/Kinova.API.USBCommandLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL)

#define Error() if (!Loaded) std::cout << "error: " << dlerror() << std::endl
#define CommError() if (!CommLoaded) std::cout << "comm error: " << dlerror() << std::endl
#define EthCommandError() if (!EthCommandLoaded) std::cout << "ethCommand error: " << dlerror() << std::endl
#define EthCommError() if (!EthCommLoaded) std::cout << "ethComm error: " << dlerror() << std::endl
#define UsbCommandError() if (!UsbCommandLoaded) std::cout << "usbCommand error: " << dlerror() << std::endl

#define PRE_ Ethernet_

// Load funcs
#define KinovaFunc(ret, name, ...) FUNC_DEF(name) = (ret (*)(__VA_ARGS__)) dlsym(Layer_handle, TOKENIZE(CONCAT(PRE_, name))); Loaded &= (FUNC_DEF(name) != nullptr)
#define CommKinovaFunc(ret, name, ...) COMM_FUNC(name) = (ret (*)(__VA_ARGS__)) dlsym(COMM_HANDLE, TOKENIZE(name)); CommLoaded &= (COMM_FUNC(name) != nullptr)
#define EthCommandKinovaFunc(ret, name, ...) ETH_FUNC(name) = (ret (*)(__VA_ARGS__)) dlsym(ETH_HANDLE, TOKENIZE(CONCAT(Ethernet_, name))); EthCommandLoaded &= (ETH_FUNC(name) != nullptr)
#define EthCommKinovaFunc(ret, name, ...) ETH_COMM_FUNC(name) = (ret (*)(__VA_ARGS__)) dlsym(ETH_COMM_HANDLE, TOKENIZE(CONCAT(Ethernet_Communication_, name))); EthCommLoaded &= (ETH_COMM_FUNC(name) != nullptr)
#define UsbCommandKinovaFunc(ret, name, ...) USB_FUNC(name) = (ret (*)(__VA_ARGS__)) dlsym(USB_HANDLE, TOKENIZE(CONCAT(Ethernet_, name))); UsbCommandLoaded &= (USB_FUNC(name) != nullptr)

// Unload API
#define UnloadAPI() dlclose(Layer_handle)
#define CommUnloadAPI() dlclose(COMM_HANDLE)
#define EthCommandUnloadAPI() dlclose(ETH_HANDLE)
#define EthCommUnloadAPI() dlclose(ETH_COMM_HANDLE)
#define UsbCommandUnloadAPI() dlclose(USB_HANDLE)
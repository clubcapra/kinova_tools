#include "api.h"
#include <iostream>

LoadAPIDef();

KinovaFuncDef(int, InitAPI,);
KinovaFuncDef(int, CloseAPI,);
KinovaFuncDef(int, StartForceControl,);
KinovaFuncDef(int, GetDevices, KinovaDevice devices[MAX_KINOVA_DEVICE], int &result);
KinovaFuncDef(int, SetActiveDevice, KinovaDevice device);

typedef int (*commandCB_t)();

struct command
{
    const char* name;
    commandCB_t cb;
};
#define Command(func) command{TOKENIZE(func), &func}

int printDevs();
int setID();

command commands[] = {
    Command(printDevs),
    Command(setID),
};
const int commandCount = sizeof(commands) / sizeof(command);

int run()
{
    while (true)
    {
        std::cout << "Select command: " << std::endl << "0: exit" << std::endl;
        for (int i = 0; i < commandCount; ++i)
        {
            std::cout << i+1 << ": " << commands[i].name << std::endl;
        }
        std::cout << std::endl << "Input: ";

        int choice = 0;
        std::cin >> choice;

        if (0 < choice && choice < commandCount)
        {
            int res = commands[choice-1].cb();
            std::cout << "exitted with code " << res << std::endl;
        }
        if (choice == 0)
        {
            break;
        }
    }

    return 0;
}

int main(int argc, char** argv)
{
    std::cout << "Started" << std::endl;

    int retCode = 0;
    LoadAPI();
    KinovaFunc(int, EthernetInitAPI);
    KinovaFunc(int, CloseAPI);
    KinovaFunc(int, StartForceControl);
    KinovaFunc(int, GetDevices, KinovaDevice devices[MAX_KINOVA_DEVICE], int &result);
    KinovaFunc(int, SetActiveDevice, KinovaDevice device);
    
    if(!((MyInitAPI == NULL) || (MyCloseAPI == NULL) || (MyGetDevices == NULL)
	|| (MySetActiveDevice == NULL) || (MyStartForceControl == NULL)))
    {
        int result = (*MyInitAPI)();
		std::cout << "Initialization's result :" << result << std::endl;

        retCode = run();

        result = (*MyCloseAPI)();
        std::cout << "Close result :" << result << std::endl;
    }
    else
    {
        std::cout << "Initialization failed!" << std::endl;
    }

    UnloadAPI();
    return retCode;
}

int printDevs()
{
    KinovaDevice list[MAX_KINOVA_DEVICE];

    int result;
    int devicesCount = MyGetDevices(list, result);

    for (int i = 0; i < devicesCount; i++)
    {
        std::cout << "Found a robot on the Ethernet bus (" << list[i].SerialNumber << ")" << std::endl;

        //Setting the current device as the active device.
        MySetActiveDevice(list[i]);
        std::cout << "SN: " << list[i].SerialNumber << " ID: " << list[i].DeviceID <<
            " Type: " << list[i].DeviceType << " Model: " << list[i].Model <<
            "Version: " << list[i].VersionMajor << '.' << list[i].VersionMinor << '.' <<
            list[i].VersionRelease << std::endl;

    }
    return 0;
}

int setID()
{
    return 0;
}

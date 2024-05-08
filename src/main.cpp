#include "api.h"
#include <iostream>
#include <arpa/inet.h>

// Definitions
LoadAPIDef();
// EthCommLoadAPIDef();

KinovaFuncDef(int, InitAPI,);
KinovaFuncDef(int, InitEthernetAPI, EthernetCommConfig& config);
KinovaFuncDef(int, GetDevices, KinovaDevice devices[MAX_KINOVA_DEVICE], int &result);
KinovaFuncDef(int, SetActiveDevice, KinovaDevice device);
KinovaFuncDef(int, GetSystemErrorCount, unsigned int &count);
KinovaFuncDef(int, GetSystemError, unsigned int indexError, SystemError &response);
KinovaFuncDef(int, CloseAPI);
KinovaFuncDef(int, GetGeneralInformations, GeneralInformations &response);
KinovaFuncDef(int, SetActuatorAddress, int actuatorAddress, int newAddress);
// KinovaFuncDef(int, RefresDevicesList,);
KinovaFuncDef(int, ClearErrorLog,);
KinovaFuncDef(int, GetAngularCommand, AngularPosition &position);
// KinovaFuncDef(int, GetActuatorsPosition, float* positions);
KinovaFuncDef(int, GetPositionCurrentActuators, float positions[POSITION_CURRENT_COUNT]);

// Forward declarations
void printErrors();
void printError(errorLoggerType err);
void printAPIError(int err);

// Commands
typedef int (*commandCB_t)();

struct command
{
    const char* name;
    commandCB_t cb;
};
#define Command(func) command{TOKENIZE(func), &func}

int printDevs();
int move();
int getID();
int setID();

command commands[] = {
    Command(printDevs),
    Command(move),
    Command(getID),
    Command(setID),
};
const int commandCount = sizeof(commands) / sizeof(command);

// Command runner
int run()
{
    while (true)
    {
        // Pront for a command
        std::cout << "Select command: " << std::endl << "0: exit" << std::endl;
        for (int i = 0; i < commandCount; ++i)
        {
            std::cout << i+1 << ": " << commands[i].name << std::endl;
        }
        std::cout << std::endl << "Input: ";

        int choice = 0;
        std::cin >> choice;

        if (0 < choice && choice <= commandCount)
        {
            // Run selected command
            int res = commands[choice-1].cb();
            std::cout << "exitted with code " << res << std::endl;
        }
        if (choice == 0)
        {
            // If exit selected
            break;
        }
    }

    return 0;
}

int main(int argc, char** argv)
{
    std::cout << "Started" << std::endl;

    // Init API
    int retCode = 0;
    // GUsbCommandLoadAPI();
    GEthCommandLoadAPI();
    // EthCommLoadAPI();
    // if (ETH_HANDLE == nullptr) std::cout << "API error: " << dlerror() << std::endl;
    if (Layer_handle == nullptr) std::cout << "API error: " << dlerror() << std::endl;

    KinovaFunc(int, InitAPI,);
    Error();
    KinovaFunc(int, InitEthernetAPI, EthernetCommConfig &config);
    Error();
    KinovaFunc(int, CloseAPI);
    Error();
    KinovaFunc(int, GetDevices, KinovaDevice devices[MAX_KINOVA_DEVICE], int &result);
    Error();
    KinovaFunc(int, SetActiveDevice, KinovaDevice device);
    Error();
    KinovaFunc(int, GetSystemErrorCount, unsigned int &count);
    Error();
    KinovaFunc(int, GetSystemError, unsigned int indexError, SystemError &response);
    Error();
    KinovaFunc(int, GetGeneralInformations, GeneralInformations &response);
    Error();
    KinovaFunc(int, SetActuatorAddress, int actuatorAddress, int newAddress);
    Error();
    // KinovaFunc(int, RefresDevicesList,);
    // Error();
    KinovaFunc(int, ClearErrorLog,);
    Error();
    KinovaFunc(int, GetAngularCommand, AngularPosition &position);
    Error();
    // KinovaFunc(int, GetActuatorsPosition, float* positions);
    // Error();
    KinovaFunc(int, GetPositionCurrentActuators, float positions[POSITION_CURRENT_COUNT]);
    Error();

    // Create ethernet config
    EthernetCommConfig config;

	config.localBcastPort = 25015;
	config.localCmdport   = 25025;
	config.robotPort      = 55000;
	config.localIpAddress = inet_addr("192.168.84.15");
	config.robotIpAddress = inet_addr("192.168.84.160");
	config.subnetMask     = inet_addr("255.255.255.0");
	config.rxTimeOutInMs  = 1000;

    
    if(Loaded)
    {
        std::cout << "API loaded successfully!" << std::endl;
        int result;
        // Wait for connection
        while (true)
        {
            // result = (*MyInitAPI)();
            result = MyInitEthernetAPI(config);
            
            if (result == NO_ERROR_KINOVA) break;
            // result = (*MyInitEthernetAPI)(config);
            // if (result == NO_ERROR_KINOVA) break;
            std::cout << "Initialization's result :" << result << std::endl;

            result = MyCloseAPI();
            std::cout << "Close's result :" << result << std::endl;
            sleep(5);
            // (*MyEthCloseAPI)();
        }
        std::cout << "Connected!" << std::endl;
        printErrors();

        // Run the command runner
        retCode = run();

        // Close the connection
        result = MyCloseAPI();
        std::cout << "Close result :" << result << std::endl;
    }
    else
    {
        // Manage errors
        std::cout << "Initialization failed!" << std::endl;
        Error();
        // EthCommError();
    }
    
    // Unload API
    UnloadAPI();
    // EthCommUnloadAPI();
    return retCode;
}

int printDevs()
{
    // Get devices
    KinovaDevice list[MAX_KINOVA_DEVICE]{};
    int result;
    
    // printErrors();
    int devicesCount = MyGetDevices(list, result);
    // printErrors();
    
    // Print info for each device
    for (int i = 0; i < devicesCount; i++)
    {
        std::cout << "Found a robot (" << list[i].SerialNumber << ")" << std::endl;

        //Setting the current device as the active device.
        MySetActiveDevice(list[i]);
        std::cout << "SN: " << list[i].SerialNumber << " ID: " << list[i].DeviceID <<
            " Type: " << list[i].DeviceType << " Model: " << list[i].Model <<
            "Version: " << list[i].VersionMajor << '.' << list[i].VersionMinor << '.' <<
            list[i].VersionRelease << std::endl;

    }
    return 0;
}

int move()
{
    AngularPosition DataCommand;
    int res = MyGetAngularCommand(DataCommand);
    std::cout << "current angular data: " << DataCommand.Actuators.Actuator1 << ", " << 
        DataCommand.Actuators.Actuator2 << ", " << 
        DataCommand.Actuators.Actuator3 << ", " << 
        DataCommand.Actuators.Actuator4 << ", " << 
        DataCommand.Actuators.Actuator5 << ", " << 
        DataCommand.Actuators.Actuator6 << std::endl;
    printAPIError(res);

    // float positions[POSITION_CURRENT_COUNT] = {0.1};
    // res = MyGetActuatorsPosition(positions);
    // for (int i = 0; i < POSITION_CURRENT_COUNT/2; ++i)
    // {
    //     std::cout << i << ": (" << positions[i*2] << ',' << positions[i*2 + 1] << ") ";
    // }
    // std::cout << std::endl;
    // printAPIError(res);

    float positionsActual[POSITION_CURRENT_COUNT] = {0.1};
    res = MyGetPositionCurrentActuators(positionsActual);
    for (int i = 0; i < POSITION_CURRENT_COUNT/2; ++i)
    {
        std::cout << i << ": (" << positionsActual[i*2] << ',' << positionsActual[i*2 + 1] << ") ";
    }
    std::cout << std::endl;
    printAPIError(res);

    return res;
}

int getID()
{
    GeneralInformations result;
    MyGetGeneralInformations(result);
    std::cout << "Power: " << result.AveragePower << 'W' <<
        " Actuator count: " << result.ConnectedActuatorCount <<
        " Supply voltage: " << result.SupplyVoltage << 'V' <<
        " Current: " << result.TotalCurrent << 'A' << std::endl;
    for (int i = 0; i < MAX_ACTUATORS; ++i)
    {
        std::cout << "Actuator " << i << ": " << result.ActuatorsCommErrors[i] << std::endl;
    }
    return 0;
}

int inSrc(int (&bindings)[MAX_ACTUATORS][2], int id)
{
    for (int i = 0; i < MAX_ACTUATORS; ++i)
    {
        if (id == bindings[i][0]) return i;
    }
    return -1;
}

int inDst(int (&bindings)[MAX_ACTUATORS][2], int id)
{
    for (int i = 0; i < MAX_ACTUATORS; ++i)
    {
        if (id == bindings[i][1]) return i;
    }
    return -1;
}

void swap(int (&mem)[MAX_ACTUATORS], int src, int dst, int actualDst)
{
    if (src == dst) return;
    mem[src] = -1;
    MySetActuatorAddress(src, dst);
    mem[dst] = actualDst;
}

int reassignRecur(int (&bindings)[MAX_ACTUATORS][2], int (&mem)[MAX_ACTUATORS], int src, int dst, int count)
{
    // Make way
    swap(mem, dst, count, bindings[inSrc(bindings, dst)][1]);
    swap(mem, src, dst, dst);
    swap(mem, count, src, mem[count]);
}

int reassign(int (&bindings)[MAX_ACTUATORS][2], int count)
{
    // Find size
    int mem[MAX_ACTUATORS];

    for (int i = 0; i < MAX_ACTUATORS; ++i)
    {
        mem[i] = -1;
    }

    for (int i = 0; i < count; ++i)
    {
        if (bindings[i][0] == bindings[i][1])
        {
            if (mem[bindings[i][0]] != -1) return 1;
            mem[bindings[i][0]] = bindings[i][0];
        }
    }

    while (true)
    {
        bool allDone = true;
        for (int i = 0; i < count; ++i)
        {
            if (bindings[i][0] == bindings[i][1]) continue;
            if (mem[bindings[i][1]] != -1) continue;
            allDone = false;
            reassignRecur(bindings, mem, bindings[i][0], bindings[i][1], count);
        }
        if (allDone) break;
    }
    return 1;
}

int setID()
{
    // GeneralInformations result;
    // MyGetGeneralInformations(result);
    // int count = result.ConnectedActuatorCount;
    int address;
    // int bindings[MAX_ACTUATORS][2] = {-1};
        
    // for (int i = 0; i < count; ++i)
    // {
    //     std::cout << "Source ID: ";
    //     std::cin >> address;
    //     bindings[i][0] = address;

    //     std::cout << "Destination ID: ";
    //     std::cin >> address;
    //     bindings[i][1] = address;
    // }

    // reassign(bindings, count);
    
    std::cout << "Destination ID: ";
    std::cin >> address;

    for (int i = 0; i < MAX_ACTUATORS; ++i)
    {
        int res = MySetActuatorAddress(i, address);
        printAPIError(res);
    }

    return 0;
}

void printAPIError(int err)
{
    if (err == NO_ERROR_KINOVA) return;
    std::cout << '{' << err << "} "; 
    switch (err)
    {
        case NO_ERROR_KINOVA:
            // std::cout << "No error, everything is fine.";
            break;
        case UNKNOWN_ERROR:
            std::cout << "We know that an error has occured but we don't know where it comes from.";
            break;
        case ERROR_LOAD_USB_LIBRARY:
            std::cout << "Unable to load the USB library.";
            break;
        case ERROR_OPEN_METHOD:
            std::cout << "Unable to access the Open method from the USB library.";
            break;
        case ERROR_WRITE_METHOD:
            std::cout << "Unable to access the Write method from the USB library.";
            break;
        case ERROR_READ_METHOD:
            std::cout << "Unable to access the Read method from the USB library.";
            break;
        case ERROR_READ_INT_METHOD:
            std::cout << "Unable to access the Read Int method from the USB library.";
            break;
        case ERROR_FREE_LIBRARY:
            std::cout << "Unable to access the Free Library method from the USB library.";
            break;
        case ERROR_JACO_CONNECTION:
            std::cout << "There is a problem with the USB connection between the device and the computer.";
            break;
        case ERROR_CLAIM_INTERFACE:
            std::cout << "Unable to claim the USB interface.";
            break;
        case ERROR_UNKNOWN_DEVICE:
            std::cout << "Unknown type of device.";
            break;
        case ERROR_NOT_INITIALIZED:
            std::cout << "The functionality you are trying to use has not been initialized.";
            break;
        case ERROR_LIBUSB_NO_DEVICE:
            std::cout << "The USB library cannot find the device.";
            break;
        case ERROR_LIBUSB_BUSY:
            std::cout << "The USB Library is bussy and could not perform the action.";
            break;
        case ERROR_LIBUSB_NOT_SUPPORTED:
            std::cout << "The functionality you are trying to perform is not supported by the version installed.";
            break;
        case ERROR_SENDPACKET_UNKNOWN:
            std::cout << "Unknown error while sending a packet.";
            break;
        case ERROR_NO_DEVICE_FOUND:
            std::cout << "Cannot find the requested device.";
            break;
        case ERROR_OPERATION_INCOMPLETED:
            std::cout << "The operation was not entirely completed :)";
            break;
        case ERROR_RS485_INVALID_HANDLE:
            std::cout << "Handle used is not valid.";
            break;
        case ERROR_RS485_IO_PENDING:
            std::cout << "An overlapped I/O operation is in progress but has not completed.";
            break;
        case ERROR_RS485_NOT_ENOUGH_MEMORY:
            std::cout << "Not enough memory to complete the opreation.";
            break;
        case ERROR_RS485_TIMEOUT:
            std::cout << "The operation has timed out.";
            break;
        case ERROR_FUNCTION_NOT_ACCESSIBLE:
            std::cout << "You are trying to call a USB function that is not available in the current context.";
            break;
        case ERROR_COMM_TIMEOUT:
            std::cout << "No response timeout reached ";
            break;
        case ERROR_NACK_RECEIVED:
            std::cout << "If the robot answered a NACK to our command";
            break;
        case ERROR_INIT_API:
            std::cout << " Error while initializing the API";
            break;
        case ERROR_LOAD_COMM_DLL:
            std::cout << " Error while loading the communication layer";
            break;
        case ERROR_INIT_COMM_METHOD:
            std::cout << "Unable to initialize the communication layer.";
            break;
        case ERROR_CLOSE_METHOD:
            std::cout << "Unable to load the Close() function from the communication layer.";
            break;
        case ERROR_GET_DEVICE_COUNT_METHOD:
            std::cout << "Unable to load the GetDeviceCount() function from the communication layer.";
            break;
        case ERROR_SEND_PACKET_METHOD:
            std::cout << "Unable to load the SendPacket() function from the communication layer.";
            break;
        case ERROR_SET_ACTIVE_DEVICE_METHOD:
            std::cout << "Unable to load the SetActiveDevice() function from the communication layer.";
            break;
        case ERROR_GET_DEVICES_LIST_METHOD:
            std::cout << "Unable to load the GetDeviceList() function from the communication layer.";
            break;
        case ERROR_SEMAPHORE_FAILED:
            std::cout << "Unable to initialized the system semaphore.";
            break;
        case ERROR_SCAN_FOR_NEW_DEVICE:
            std::cout << "Unable to load the ScanForNewDevice() function from the communication layer.";
            break;
        case ERROR_GET_ACTIVE_DEVICE_METHOD:
            std::cout << "Unable to load the GetActiveDevice function from the communication layer.";
            break;
        case ERROR_OPEN_RS485_ACTIVATE:
            std::cout << "Unable to load the OpenRS485_Activate() function from the communication layer.";
            break;
        case ERROR_INVALID_PARAM:
            std::cout << "A function's parameter is not valid.";
            break;
        case ERROR_API_NOT_INITIALIZED:
            std::cout << "The API is not initialized.";
            break;
        case ERROR_INIT_DATA_STRUCTURES_METHOD:
            std::cout << "Unable to load the InitDataStructure() function from the communication layer.";
            break;
        case JACO_NACK_FIRST:
            std::cout << "Internale error (JACO_NACK_FIRST)";
            break;
        case JACO_COMM_FAILED:
            std::cout << "Internale error (JACO_COMM_FAILED)";
            break;
        case JACO_NACK_NORMAL:
            std::cout << "Internale error (JACO_NACK_NORMAL)";
            break;
        default:
            std::cout << "Other error";
            break;
    }
    std::cout << std::endl;
}

void printError(errorLoggerType err)
{
    switch (err)
        {
            case ERROR_NOTINITIALIZED:
                // std::cout << "Default value." << std::endl;
                break;
            case keos_err1:
                std::cout << "An error from the system's first software layer. It is very low level stuff." << std::endl;
                break;
            case keos_err2:
                std::cout << "An error from the system's second software layer. It is very low level stuff." << std::endl;
                break;
            case keos_err3:
                std::cout << "An error from the system's third software layer. It is low level stuff." << std::endl;
                break;
            case User_err_start_marker:
                std::cout << "Not used for now." << std::endl;
                break;
            case errorlog_Actuator_Temperature:
                std::cout << "Indicates that one of the actuator's temperature has been over the temperature limit." << std::endl;
                break;
            case errorlog_Actuator_TemperatureOK:
                std::cout << "Indicates that the actuator that was in temperature error but is now ok." << std::endl;
                break;
            case errorlog_Finger_Temperature:
                std::cout << "Indicates that one of the finger's temperature has been over the temperature limit." << std::endl;
                break;
            case errorlog_Finger_TemperatureOK:
                std::cout << "Indicates that one of the finger's temperature was over the temperature limit but is now ok." << std::endl;
                break;
            case errorlog_voltage:
                std::cout << "Indicates that the voltage is below the minimum value." << std::endl;
                break;
            case errorlog_voltageOK:
                std::cout << "Indicate that the voltage was in error but is now ok." << std::endl;
                break;
            case errorlog_current_FingersClosing:
                std::cout << "That indicates the one of the finger's current has been over the current limit while closing." << std::endl;
                break;
            case errorlog_current_FingersOpening:
                std::cout << "That indicates the one of the finger's current has been over the current limit while opening." << std::endl;
                break;
            case errorlog_current_FingersOK:
                std::cout << "That indicates the one of the finger's current was in error but is now ok." << std::endl;
                break;
            case errorlog_current_Actuators:
                std::cout << "That indicates the one of the actuators's current has been over the current limit." << std::endl;
                break;
            case errorlog_current_ActuatorsOK:
                std::cout << "That indicates the one of the actuators was in current error but is now ok." << std::endl;
                break;
            case errorLog_RobotStatus_Build_Incomplete:
                std::cout << "The system did not detect enough hardware to virtually build a JACO or a MICO." << std::endl;
                break;
            case errorLogger_END:
                std::cout << "Not used for now" << std::endl;
                break;
        }
}

void printErrors()
{
    unsigned int count;
    SystemError err{};
    (*MyGetSystemErrorCount)(count);
    if (count > 10) return;
    for (unsigned int i = 0; i < count; ++i)
    {
        (*MyGetSystemError)(i, err);
        printError(err.ErrorType);
    }
    (*MyClearErrorLog)();
}

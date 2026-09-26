#pragma once
#include<iostream>
#include "../device/IAudioOutputDevice.hpp"
#include "../enums/DeviceType.hpp"
#include "../factories/DeviceFactory.hpp"

using namespace std;

class DeviceManager
{
private:
    static DeviceManager* instance;
    IAudioOutputDevice* currentOutputDevice;
    DeviceManager() {
        currentOutputDevice = nullptr;
    }
public:
    static DeviceManager* getInstance() {
        if(!instance) {
            instance = new DeviceManager();
        }
        return instance;
    }
    void connect(DeviceType deviceType){
        if(currentOutputDevice) {
            delete currentOutputDevice;
        }

        currentOutputDevice = DeviceFactory::createDevice(deviceType);

        switch (deviceType)
        {
        case DeviceType::BLUETOOTH:
            cout<<"Bluetooth device connected.\n";
            break;
        case DeviceType::WIRED:
            cout<<"Wired device connected.\n";
            break;
        case DeviceType::HEADPHONES:
            cout<<"Headphones connected.\n";
            break;
        default:
            break;
        }
    }

    IAudioOutputDevice* getOutputDevice() {
        if(!currentOutputDevice) {
            throw runtime_error("No output device is connected.\n");
        }
        return currentOutputDevice;
    }

    bool hasOutputDevice() {
        return currentOutputDevice!=nullptr;
    }

};
DeviceManager* DeviceManager::instance = nullptr;


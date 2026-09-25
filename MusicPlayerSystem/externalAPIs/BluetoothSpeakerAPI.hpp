#pragma once
#include <iostream>
#include <string>

using namespace std;

class BluetoothSpeakerAPI
{
public:
    void playSongViaBluetooth(const string& data){
        cout<<"[Bluetooth Speaker] Playing : "<<data<<endl;
    }
};


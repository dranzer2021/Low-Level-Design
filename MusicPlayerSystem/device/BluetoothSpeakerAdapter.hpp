#pragma once
#include <iostream>
#include <string>
#include "../models/Song.hpp"
#include "IAudioOutputDevice.hpp"
#include "../externalAPIs/BluetoothSpeakerAPI.hpp"

using namespace std;

class BluetoothSpeakerAdapter : public IAudioOutputDevice
{
    BluetoothSpeakerAPI* bluetoothApi;

public:
    BluetoothSpeakerAdapter(BluetoothSpeakerAPI* api) {
        bluetoothApi = api;
    }
    void playAudio(Song* song) override {
        string payload = song->getTitle() + " By " + song->getArtist();
        bluetoothApi->playSongViaBluetooth(payload);
    }
};




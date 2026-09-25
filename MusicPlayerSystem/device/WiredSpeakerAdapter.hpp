#pragma once
#include <iostream>
#include <string>
#include "../models/Song.hpp"
#include "IAudioOutputDevice.hpp"
#include "../externalAPIs/WiredSpeakerAPI.hpp"

using namespace std;

class WiredSpeakerAdapter : public IAudioOutputDevice
{
    WiredSpeakerAPI* wiredApi;

public:
    WiredSpeakerAdapter(WiredSpeakerAPI* api) {
        wiredApi = api;
    }
    void playAudio(Song* song) override {
        string payload = song->getTitle() + " By " + song->getArtist();
        wiredApi->playSongViaCable(payload);
    }
    
};
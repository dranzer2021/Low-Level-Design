#pragma once
#include <iostream>
#include <string>
#include "../models/Song.hpp"
#include "IAudioOutputDevice.hpp"
#include "../externalAPIs/HeadphonesAPI.hpp"

using namespace std;

class HeadphonesAdapter : public IAudioOutputDevice
{
    HeadphonesAPI* headphonesApi;

public:
    HeadphonesAdapter(HeadphonesAPI* api) {
        headphonesApi = api;
    }
    void playAudio(Song* song) override {
        string payload = song->getTitle() + " By " + song->getArtist();
        headphonesApi->playSongViaJack(payload);
    }
    
};
#pragma once
#include <iostream>
#include <string>
#include "../models/Song.hpp"
#include "IAudioOutputDevice.hpp"

using namespace std;

class AudioEngine
{
private:
    Song* currentSong;
    bool songIsPaused;
public:
    AudioEngine(){
        currentSong = nullptr;
        songIsPaused = false;
    }
    string getCurrentSongTitle() const {
        return currentSong->getTitle();
    }
    bool isPaused() const {
        return songIsPaused;
    }

    void play(IAudioOutputDevice* aod, Song* song){
        if(song == nullptr){
            throw runtime_error("Null Song cannot be played.");
        }
        if(song == currentSong && songIsPaused){
            songIsPaused = false;
            cout<<"Running Song : "<<currentSong<<endl;
            aod->playAudio(song);
            return;
        }

        currentSong = song;
        songIsPaused = false;
        cout<<"Playing Song : "<<currentSong<<endl;
        aod->playAudio(song);
    }

    void pause() {
        if(currentSong == nullptr){
            throw runtime_error("No song is playing currently.");
        }
        if(songIsPaused){
            throw runtime_error("Song is already paused.");
        }
        songIsPaused = true;
        cout<<"Pausing Song : "<<currentSong->getTitle()<<endl;
    }
};


#pragma once
#include "core/AudioEngine.hpp"
#include "models/Playlist.hpp"
#include "models/Song.hpp"
#include "strategies/PlayStrategy.hpp"
#include "enums/DeviceType.hpp"
#include "enums/PlayStrategyType.hpp"
#include "managers/DeviceManager.hpp"
#include "managers/PlaylistManager.hpp"
#include "managers/StrategyManager.hpp"

using namespace std;

class MusicPlayerFacade
{
private:
    static MusicPlayerFacade* instance;
    AudioEngine* audioEngine;
    Playlist* loadedPlaylist;
    PlayStrategy* playStrategy;
    MusicPlayerFacade(){
        loadedPlaylist = nullptr;
        playStrategy = nullptr;
        audioEngine = new AudioEngine();    
    }

public:

    static MusicPlayerFacade* getInstance(){
        if(!instance) {
            instance = new MusicPlayerFacade();
        }
        return instance;
    }

    void connectDevice(DeviceType deviceType) {
        DeviceManager::getInstance()->connect(deviceType);
    }

    void setPlayStrategy(PlayStrategyType strategyType) {
        playStrategy = StrategyManager::getInstance()->getStrategy(strategyType);
    }

    void loadPlaylist(const string& name){
        loadedPlaylist = PlaylistManager::getInstance()->getPlaylist(name);
        if (!playStrategy) {
            throw runtime_error("Play strategy not set before loading.");
        }
        playStrategy->setPlaylist(loadedPlaylist);
    }

    void playSong(Song* song) {
        if(!DeviceManager::getInstance()->hasOutputDevice()) {
            throw runtime_error("Output device not connected.\n");
        }
        IAudioOutputDevice* device = DeviceManager::getInstance()->getOutputDevice();
        audioEngine->play(device,song);
    }

    void pauseSong(Song* song) {
        if(audioEngine->getCurrentSongTitle() != song->getTitle()) {
            throw runtime_error("Cannot pause. Song Mismatch ! \n");
        }
        audioEngine->pause();
    }

    void playAllTracks() {
        if(!loadedPlaylist) {
            throw runtime_error("No Playlist Loaded ! \n");
        }
        while(playStrategy->hasNext()) {
            Song* nextSong = playStrategy->next();
            IAudioOutputDevice* device = DeviceManager::getInstance()->getOutputDevice();
            audioEngine->play(device,nextSong);  
        }
        cout << "Completed playlist: " << loadedPlaylist->getPlaylistName() << "\n";
    }

    void playNextTrack() {
        if(!loadedPlaylist) {
            throw runtime_error("No Playlist Loaded ! \n");
        }
        if(playStrategy->hasNext()) {
            Song* nextSong = playStrategy->next();
            IAudioOutputDevice* device = DeviceManager::getInstance()->getOutputDevice();
            audioEngine->play(device,nextSong);
            cout<<"Playing next song : "<<nextSong->getTitle()<<"\n";  
        }
    }

    void playPreviousTrack() {
        if(!loadedPlaylist) {
            throw runtime_error("No Playlist Loaded ! \n");
        }
        if(playStrategy->hasPrevious()) {
            Song* prevSong = playStrategy->previous();
            IAudioOutputDevice* device = DeviceManager::getInstance()->getOutputDevice();
            audioEngine->play(device,prevSong);
            cout<<"Playing previous song : "<<prevSong->getTitle()<<"\n";  
        }
    }

    void enqueueNext(Song* song){
        playStrategy->addToNext(song);
    }
};
MusicPlayerFacade* MusicPlayerFacade::instance = nullptr;



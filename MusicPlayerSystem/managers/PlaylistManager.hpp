#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../models/Song.hpp"
#include "../models/Playlist.hpp"

using namespace std;

class PlaylistManager
{
private:

    static PlaylistManager* instance;
    map<string,Playlist*> playlists; 
    PlaylistManager() {}

public:

    static PlaylistManager* getInstance(){
        if(!instance){
            instance = new PlaylistManager();
        }
        return instance;
    }

    void createPlaylist(const string& name){
        if(playlists[name]){
            throw runtime_error("Playlist \"" + name + "\" already exists.");
        }
        playlists[name] = new Playlist(name);
    }

    void addSongToPlaylist(const string& playListName,Song* song){
        if(!playlists.count(playListName)){
            throw runtime_error("Playlist \"" + playListName + "\" not found.");
        }
        playlists[playListName]->addSongToPlaylist(song);
    }
    Playlist* getPlaylist(const string& name){
        if(!playlists.count(name)){
            throw runtime_error("Playlist \"" + name + "\" not found.");
        }
        return playlists[name];
    }

    
};

PlaylistManager* PlaylistManager::instance = nullptr;


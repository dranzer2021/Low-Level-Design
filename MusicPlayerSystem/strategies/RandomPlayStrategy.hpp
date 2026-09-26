#pragma once
#include<iostream>
#include<stack>
#include<vector>
#include "../models/Playlist.hpp"
#include "../models/Song.hpp"
#include "PlayStrategy.hpp"

using namespace std;

class RandomPlayStrategy : public PlayStrategy
{
private:
    Playlist* playlist;
    vector<Song*> remainingSongs;
    stack<Song*> history;
public:
    RandomPlayStrategy() {
        playlist = nullptr;
        srand((unsigned)time(nullptr));
    }

    void setPlaylist(Playlist* p) override {
        playlist = p;
        if(!playlist || playlist->getSize()==0) 
            return;
        remainingSongs = playlist->getSongList();
        history = stack<Song*>();
    }

    bool hasNext() override {
        return (remainingSongs.size() && playlist);

    }

    Song* next() override {
        
        if(!playlist || playlist->getSize()==0)
            throw runtime_error("Playlist is empty or invalid.");
        if(remainingSongs.empty())
            throw runtime_error("No songs remaining.");
        
        int idx = rand()%remainingSongs.size();
        Song* selectedSong = remainingSongs[idx];
        
        swap(remainingSongs[idx],remainingSongs.back());
        remainingSongs.pop_back();

        history.push(selectedSong);

        return selectedSong;
    }

    bool hasPrevious() override {
        return history.size() > 0;
    }

    Song* previous() override {
        if (history.empty()) {
            throw std::runtime_error("No previous song available.");
        }

        Song* song = history.top();
        history.pop();
        return song;
    }
    
};
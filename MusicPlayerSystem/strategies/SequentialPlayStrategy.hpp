#pragma once
#include<iostream>
#include "../models/Playlist.hpp"
#include "PlayStrategy.hpp"

using namespace std;

class SequentialPlayStrategy : public PlayStrategy
{
private:
    Playlist* playlist;
    int currentIndex;
public:
    SequentialPlayStrategy() {
        playlist = nullptr;
        currentIndex = -1;
    }
    void setPlaylist(Playlist* p) override {
        currentIndex = -1;
        playlist = p;
    }
    bool hasNext() override {
        return ((currentIndex+1) < playlist->getSize());
    }
    Song* next() override {
        if(!playlist || playlist->getSize()==0)
            throw runtime_error("Playlist is empty or invalid.");
        currentIndex++;
        return playlist->getSongList()[currentIndex];
    }
    bool hasPrevious() override {
        return (currentIndex-1 > 0);
    }
    Song* previous() override {
        if(!playlist || playlist->getSize()==0)
            throw runtime_error("Playlist is empty or invalid.");
        currentIndex--;
        return playlist->getSongList()[currentIndex];
    }
};


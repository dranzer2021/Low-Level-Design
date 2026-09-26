#pragma once

#include<iostream>
#include<vector>
#include<string>
#include"../models/Playlist.hpp"
#include"../models/Song.hpp"

using namespace std;

class PlayStrategy
{
public:
    virtual void setPlaylist(Playlist* p) = 0;
    virtual bool hasNext() = 0;
    virtual Song* next() = 0;
    virtual bool hasPrevious() = 0;
    virtual Song* previous() = 0;
    virtual void addToNext(Song* song) {}
    virtual ~PlayStrategy() {}
};


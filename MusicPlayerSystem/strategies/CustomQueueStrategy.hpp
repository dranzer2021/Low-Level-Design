#pragma once
#include<iostream>
#include<stack>
#include<queue>
#include "../models/Playlist.hpp"
#include "PlayStrategy.hpp"

using namespace std;

class CustomQueueStrategy : public PlayStrategy
{
private:
    Playlist* playlist;
    int currentIndex;
    queue<Song*> nextQueue;
    stack<Song*> prevStack;

    Song* nextSequential() {
        if(playlist->getSize()==0)
            throw runtime_error("Playlist is empty.");
        currentIndex++;
        return playlist->getSongList()[currentIndex];
    }
    Song* prevSequential() {
        if(playlist->getSize()==0)
            throw runtime_error("Playlist is empty.");
        currentIndex--;
        return playlist->getSongList()[currentIndex];
    }
public:
    CustomQueueStrategy() {
        playlist = nullptr;
        currentIndex = -1;
    }

    void setPlaylist(Playlist* p) override {
        playlist = playlist;
        currentIndex = -1;
        while (!nextQueue.empty()) {
            nextQueue.pop();
        }
        while(!prevStack.empty()) {
            prevStack.pop();
        }
    }

    bool hasNext() override {
        return ((currentIndex + 1) < playlist->getSize());
    }

    Song* next() override {
        if(!playlist || playlist->getSize()==0)
            throw runtime_error("Playlist empty or invalid.");

        if(!nextQueue.empty()){
            Song* s = nextQueue.front();
            nextQueue.pop();
            prevStack.push(s);

            //Updating Current Index 
            auto& list = playlist->getSongList();
            for(int i=0;i<(int)list.size();i++){
                if(list[i]==s){
                    currentIndex = i;
                    break;
                }
            }
            return s;
        }
        return nextSequential();
    }

    bool hasPrevious() override {
        return (currentIndex - 1 > 0);
    }

    Song* previous() override {
        if(!playlist || playlist->getSize()==0)
            throw runtime_error("Playlist empty or invalid.");

        if(!prevStack.empty()){
            Song* s = prevStack.top();
            prevStack.pop();

            //Updating Current Index 
            auto& list = playlist->getSongList();
            for(int i=0;i<(int)list.size();i++){
                if(list[i]==s){
                    currentIndex = i;
                    break;
                }
            }
            return s;
        }
        return prevSequential();
    }

    void addToNext(Song* song) override {
        if (!song) {
            throw runtime_error("Cannot enqueue null song.");
        }
        nextQueue.push(song);
    }
};
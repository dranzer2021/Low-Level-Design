#pragma once
#include <iostream>
#include <string>

using namespace std;

class WiredSpeakerAPI
{
public:
    void playSongViaCable(const string& data){
        cout<<"[Wired Speaker] Playing : "<<data<<endl;
    }
};


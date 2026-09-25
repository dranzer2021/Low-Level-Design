#pragma once
#include <iostream>
#include <string>

using namespace std;

class HeadphonesAPI
{
public:
    void playSongViaJack(const string& data){
        cout<<"[Headphones] Playing : "<<data<<endl;
    }
};
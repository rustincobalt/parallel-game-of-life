#pragma once
#include "..\lib\raylib.h"

extern "C" {
    #include "raygui.h"
}

#include "AppState.hpp"
#include "AppUI.hpp"

#include <string>

using std::string;

// #define TIMEDEBUG

#ifdef TIMEDEBUG
#include <chrono>
#include <thread>
#include <iostream>
#endif


class App {

private:
    int windowY;
    int windowX;
    Color bgColor;
    int targetFPS;
    string windowName;
    

    // App State (controls Game Simulation)
    AppState state;

    // UI specific
    AppUI ui;
    void runAppLoop();



public:

    App(int y, int x, int targetFPS = 120, string windowName = "Game of Life",  Color bgColor = BLACK);
    void Launch();
    
};

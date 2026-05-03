#pragma once
#include "..\lib\raylib.h"

extern "C" {
    #include "raygui.h"
}
#include "AppState.hpp"

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
    int y;
    int x;
    float pixelSize;
    Color bgColor;
    int targetFPS;
    string windowName;

    
    void handleUI(AppState&);
    void drawUI(const AppState&);
    void drawGrid(const AppState&);
    void runAppLoop();



public:

    App(int y, int x, float pixelSize, int targetFPS = 120, string windowName = "Game of Life",  Color bgColor = BLACK);
    void Launch();
    
};

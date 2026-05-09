#pragma once
#include "..\lib\raylib.h"
extern "C" {
    #include "..\lib\raygui.h"
}

#include "AppState.hpp"
#include "AppUI.hpp"

#include "LifeSeq.hpp"
#pragma once
#include "LifeOMPBlocks.hpp"
#include "LifeOMPColumns.hpp"
#include "LifeOMPRows.hpp"

#include <string>
#include <memory>


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

    // Texture for grid
    Texture2D gridTexture;

    // Game of Life logic
    std::unique_ptr<Life> simulation;


    // Main Loop
    void runAppLoop();
    
    // Handle
    void handleEvents();

    // HELP FUNCTIONS
    
    void generateTextureFromImage(Texture2D& texture, int w, int h);
    
    void initSimulationBasedOnAlgorithm();
    void generateGridBasedOnMode();
    
    void recreateSimulation();

public:

    App(int y, int x, int targetFPS = 120, string windowName = "Game of Life",  Color bgColor = BLACK);
    void Launch();
    
};

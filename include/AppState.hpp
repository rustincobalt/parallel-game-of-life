#pragma once

// TODO: DELETE
// #include "..\lib\raylib.h"

// extern "C" {
//     #include "..\lib\raygui.h"
// }

#include <algorithm>


class AppState {
    public:

    enum DirtyFlags {
        NONE            = 0,
        WINDOW_RESIZED  = 1 << 0,
        LAYOUT_CHANGED  = 1 << 1, // Any Drawing Grid change: height or cell size in pixels
        GRID_GENERATE   = 1 << 2,
        MODE_CHANGED    = 1 << 3,
        FPS_CHANGED     = 1 << 4
        // SIMULATION_RESET = 1 << 2,
    };


private:

    // --- Simulation ---
    bool isInit;
    bool running;

    int mode;
    int selectedTestCase; // for TEST mode
    int algorithm;

    int randomFillPercent;
    int randomSeed;

    int cellsX;
    int cellsY;

    // --- Rendering ---
    int fps;
    double maxCellSizePX;
    double cellSizePX;

    float gridStartX;
    float gridStartY;

    double gridDrawWidth;
    double gridDrawHeight;

    // --- Layout ---
    int panelWidth;

    // --- Flags ---
    int dirtyFlags;

public:

    AppState();

    // --- Layout ---
    void updateGridLayout(int screenWidth, int screenHeight);
    

    // --- Flags ---
    void addFlag(DirtyFlags flag);

    bool consumeFlag(DirtyFlags flag);

    // --- Setters ---

    void requestGridGenerate(); // Triggered by Generate Button

    void setRunning(bool value);
    void setInitStatus(bool value);
    void toggleRunning();

    void setMode(int value);
    void setSelectedTestCase(int value);
    void setAlgorithm(int value);

    void setRandomFillPercent(int value);
    void setRandomSeed(int value);

    void setFPS(int value);
    void setCellsX(int value);
    void setCellsY(int value);

    void setMaxCellSizePX(int value);
    
    // --- Getters ---
    bool isRunning() const;

    int getMode() const;
    int getSelectedTestCase() const;
    int getAlgorithm() const;

    int getRandomFillPercent() const;
    int getRandomSeed() const;

    int getFPS() const;
    int getCellsX() const;
    int getCellsY() const;

    int getMaxCellSizePX() const;
    int getCellSizePX() const;

    float getGridStartX() const;
    float getGridStartY() const;

    int getGridDrawWidth() const;
    int getGridDrawHeight() const;

    int getPanelWidth() const;
    
    void incrementTestCase();
    void decrementTestCase();
};
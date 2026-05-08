#include "../include/AppState.hpp"

AppState::AppState()
    : isInit(false),
      running(false),
      mode(0),
      algorithm(0),
      randomFillPercent(30),
      randomSeed(1),
      cellsX(100),
      cellsY(100),
      fps(30),
      maxCellSizePX(20),
      cellSizePX(1),
      gridStartX(0.0f),
      gridStartY(0.0f),
      gridDrawWidth(0),
      gridDrawHeight(0),
      panelWidth(220),
      dirtyFlags(NONE)
{
    // updateGridLayout();
    addFlag(LAYOUT_CHANGED); // instead of updateGridLayout()

}


void AppState::updateGridLayout(int screenWidth, int screenHeight)
{
    int availableWidth =
        screenWidth - panelWidth;

    int availableHeight =
        screenHeight;

    cellSizePX = std::min(
        availableWidth / (double)cellsX,
        availableHeight / (double)cellsY
    );

    // cellSizePX = std::max(cellSizePX, 1.);

    cellSizePX = std::min(
        cellSizePX,
        maxCellSizePX
    );

    gridDrawWidth =
        cellsX * cellSizePX;

    gridDrawHeight =
        cellsY * cellSizePX;

    gridStartX =
        panelWidth +
        (availableWidth - gridDrawWidth) / 2.0f;

    gridStartY =
        (availableHeight - gridDrawHeight) / 2.0f;
}


void AppState::addFlag(DirtyFlags flag)
{
    dirtyFlags |= flag;
}

bool AppState::consumeFlag(DirtyFlags flag)
{
    bool hasFlag = dirtyFlags & flag;

    dirtyFlags &= ~flag;

    return hasFlag;
}

// Setters

void AppState::requestGridGenerate()
{
    isInit = false;
    running = false;

    addFlag(GRID_GENERATE);
}


void AppState::setCellsX(int value)
{
    cellsX = value;
    addFlag(LAYOUT_CHANGED);
    // addFlag(GRID_GENERATE);
    // updateGridLayout();
}

void AppState::setCellsY(int value)
{
    cellsY = value;
    addFlag(LAYOUT_CHANGED);
    // addFlag(GRID_GENERATE);
    // updateGridLayout();
}


void AppState::setMaxCellSizePX(int value)
{
    if (maxCellSizePX == value)
        return;

    maxCellSizePX = value;

    // updateGridLayout();

    addFlag(LAYOUT_CHANGED);
}

void AppState::setInitStatus(bool value){
    isInit = value;
}

void AppState::setRunning(bool value)
{
    if (isInit == false)
        return;
    
    if (running == value)
        return;

    running = value;

    // addFlag(RUN_STATE_CHANGED);
}

void AppState::toggleRunning(){
   setRunning(!running); // IMPORTANT: setRunning performs checks
    // addFlag(RUN_STATE_CHANGED);
}

void AppState::setMode(int value)
{
    if (mode == value)
        return;

    mode = value;

    isInit = false;
    running = false;

    addFlag(MODE_CHANGED);
}

void AppState::setAlgorithm(int value)
{
    if (algorithm == value)
        return;

    algorithm = value;

    // addFlag(SIMULATION_RESET); TODO: DELETE
}

void AppState::setRandomFillPercent(int value)
{
    randomFillPercent = value;
}

void AppState::setRandomSeed(int value)
{
    randomSeed = value;
}

// Getters

bool AppState::isRunning() const
{
    return running;
}

int AppState::getMode() const
{
    return mode;
}

int AppState::getAlgorithm() const
{
    return algorithm;
}

int AppState::getRandomFillPercent() const
{
    return randomFillPercent;
}

int AppState::getRandomSeed() const
{
    return randomSeed;
}

int AppState::getCellsX() const
{
    return cellsX;
}

int AppState::getCellsY() const
{
    return cellsY;
}

int AppState::getMaxCellSizePX() const
{
    return maxCellSizePX;
}

int AppState::getCellSizePX() const
{
    return cellSizePX;
}

float AppState::getGridStartX() const
{
    return gridStartX;
}

float AppState::getGridStartY() const
{
    return gridStartY;
}

int AppState::getGridDrawWidth() const
{
    return gridDrawWidth;
}

int AppState::getGridDrawHeight() const
{
    return gridDrawHeight;
}

int AppState::getPanelWidth() const
{
    return panelWidth;
}

void AppState::setFPS(int value)
{
    fps = value;
    addFlag(FPS_CHANGED);
}
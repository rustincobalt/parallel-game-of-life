#include "..\include\App.hpp"

App::App(int y, int x, float pixelSize, int targetFPS, string windowName, Color bgColor):
y(y), x(x), pixelSize(pixelSize), targetFPS(targetFPS), windowName(windowName), bgColor(bgColor)
{}

void App::handleUI(AppState& state) {

    int panelWidth = 125;

    // Layout positions
    float x = 10;
    float y = 10;
    float w = panelWidth - 20;
    float h = 25;
    int spacing = 35;

    // --- Grid size X ---
    GuiLabel({x, y, w, h}, "Size X");
    y += 20;

    static int editX = 0;
    GuiValueBox({x, y, w, h}, nullptr, &state.cellsX, 10, 500, editX);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) editX = 0;
    y += spacing;

    // --- Grid size Y ---
    GuiLabel({x, y, w, h}, "Size Y");
    y += 20;

    static int editY = 0;
    GuiValueBox({x, y, w, h}, nullptr, &state.cellsY, 10, 500, editY);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) editY = 0;
    y += spacing;

    // --- Mode dropdown ---
    GuiLabel({x, y, w, h}, "Mode");
    y += 20;

    static bool modeEdit = false;
    if (GuiDropdownBox({x, y, w, h},
        "Demo;Test;Measure",
        &state.mode,
        modeEdit)) {
        modeEdit = !modeEdit;
    }
    y += spacing;

    // --- Algorithm dropdown ---
    GuiLabel({x, y, w, h}, "Algorithm");
    y += 20;

    static bool algoEdit = false;
    if (GuiDropdownBox({x, y, w, h},
        "Sequential;Parallel",
        &state.algorithm,
        algoEdit)) {
        algoEdit = !algoEdit;
    }
    y += spacing;

    // --- Start button ---
    if (!state.running) {
        if (GuiButton({x, y, w, h}, "Start")) {
            state.running = true;
        }
    } else {
        if (GuiButton({x, y, w, h}, "Stop")) {
            state.running = false;
        }
    }
    y += spacing;

    // --- Hard reset / terminate ---
    if (GuiButton({x, y, w, h}, "Reset")) {
        state.running = false;
        // later: reset grid
    }
}

void App::drawUI(const AppState&){
    int panelWidth = 125;

    // DrawRectangle(0, 0, panelWidth, GetScreenHeight(), LIGHTGRAY);

};
void App::drawGrid(const AppState& state){

    int panelWidth = 125;

    int availableWidth = GetScreenWidth() - panelWidth;
    int availableHeight = GetScreenHeight();

    int cellSize = std::min(
        availableWidth / state.cellsX,
        availableHeight / state.cellsY
    );

    cellSize = std::min(cellSize, state.maxCellSize);
    
    
    float startX = panelWidth + (availableWidth - state.cellsX * cellSize) / 2.;
    float startY = (availableHeight - state.cellsY * cellSize) / 2.;

    for (int y = 0; y < state.cellsY; y++) {
        for (int x = 0; x < state.cellsX; x++) {

            DrawRectangleLines(
                startX + x * cellSize,
                startY + y * cellSize,
                cellSize,
                cellSize,
                DARKGRAY
            );
        }
    }
}


void App::runAppLoop(){
    
    AppState state;

    while (!WindowShouldClose()) {

        // --- 1. HANDLE UI ---
        handleUI(state);

        // --- 2. UPDATE SIMULATION ---
        if (state.running) {
            // currently empty
        }

        // --- 3. DRAW ---
        BeginDrawing();
        ClearBackground(bgColor);

            drawUI(state);
            drawGrid(state); // empty for now

        EndDrawing();
    }


}

void App::Launch(){

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(x*pixelSize, y*pixelSize, windowName.c_str());
    SetTargetFPS(targetFPS);
    
    this->runAppLoop();

    CloseWindow();

}
#include "..\include\App.hpp"

App::App(int y, int x, int targetFPS, string windowName, Color bgColor):
windowY(y), windowX(x), targetFPS(targetFPS), windowName(windowName), bgColor(bgColor)
{
    ui = AppUI(state);
}



void App::runAppLoop(){
    
    while (!WindowShouldClose()) {

        // --- 1. HANDLE UI ---

        // --- 2. UPDATE SIMULATION ---
        if (state.running) {
            // currently empty
        }

        // --- 3. DRAW ---
        BeginDrawing();
        ClearBackground(bgColor);
            
            ui.handleUI();
            ui.drawGrid(); // empty for now

        EndDrawing();
    }


}

void App::Launch(){

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   
    InitWindow(windowX, windowY, windowName.c_str());
    SetTargetFPS(targetFPS);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20); 
    
    this->runAppLoop();

    CloseWindow();

}
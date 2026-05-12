#include "..\include\App.hpp"

App::App(int y, int x, int targetFPS, string windowName, Color bgColor):
windowY(y), windowX(x), targetFPS(targetFPS), windowName(windowName), bgColor(bgColor)
{   
    ui = AppUI(state);
    simulation = nullptr;

    // IMPORTANT: can't generate Texture before InitWindow
    // generateTextureFromImage(gridTexture, state.getCellsX(), state.getCellsX());
}

void App::generateTextureFromImage(Texture2D& texture, int w, int h){
    if (IsTextureValid(gridTexture))
        UnloadTexture(gridTexture);
    
    Image image = GenImageColor(w, h, bgColor);
    
    texture = LoadTextureFromImage(image);

    UnloadImage(image);

    // SetTextureFilter(
    // gridTexture,
    // TEXTURE_FILTER_POINT);
}


// void App::recreateSimulation()
// {
//     generateTextureFromImage(gridTexture, state.getCellsX(), state.getCellsY());

//     UpdateTexture(
//         gridTexture,
//         simulation->getPixels().data());
// }

void App::generateGridBasedOnMode(){

    if(state.getMode() == 1){
        simulation->loadPrimitive(state.getSelectedTestCase());
        
        // Update AppState
        state.setCellsX( simulation->getRealWidth());
        state.setCellsY( simulation->getRealHeight());
    } 
    else{

        simulation->generateRandomGrid(
            state.getRandomSeed(),
            state.getRandomFillPercent());
    }


}

void App::initSimulationBasedOnAlgorithm(){
      switch (state.getAlgorithm())
    {
        case 1:
            simulation =
                std::make_unique<LifeOMPBlocks>(
                    state.getCellsY(),
                    state.getCellsX(),
                    0XFF000000,
                    0XFF00A5FF, 
                    6);
            break;
        

        case 2:
            simulation =
            std::make_unique<LifeOMPColumns>(
                state.getCellsY(),
                state.getCellsX(),
                0XFF000000,
                0XFFFF4500, 
                6);
            break;
        case 3:
            simulation =
                std::make_unique<LifeOMPRows>(
                    state.getCellsY(),
                    state.getCellsX(),
                    0XFF000000,
                    0XFFC4008A, 
                    6);
            break;

        default:
            simulation =
                std::make_unique<LifeSeq>(
                    state.getCellsY(),
                    state.getCellsX());
            break;
    }

    state.setInitStatus(true);

}

void App::handleEvents(){

    if (IsWindowResized())
        state.updateGridLayout(GetScreenWidth(), GetScreenHeight());
    
    if (state.consumeFlag(
            AppState::LAYOUT_CHANGED))    
        state.updateGridLayout(GetScreenWidth(), GetScreenHeight());

    if (state.consumeFlag(
            AppState::FPS_CHANGED))    
        SetTargetFPS(state.getFPS());
        
    if (state.consumeFlag(
            AppState::GRID_GENERATE))
    {
        state.setRunning(false);

        initSimulationBasedOnAlgorithm();

        generateGridBasedOnMode();

        simulation->calcPixels();

        generateTextureFromImage(gridTexture, state.getCellsX(), state.getCellsY()); // After init and generate

        UpdateTexture(
            gridTexture,
            simulation->getPixels().data());
    }



    if (state.consumeFlag(
            AppState::MODE_CHANGED))
    {
        state.setRunning(false);

        initSimulationBasedOnAlgorithm();
        generateGridBasedOnMode();
    }


    // if (ui.gridSizeChanged())
    // {
    //     recreateSimulation();

    //     state.updateGridLayout();
    // }
}

void App::runAppLoop(){
    
    while (!WindowShouldClose()) {

        handleEvents();

        // --- UPDATE SIMULATION ---
        if (state.isRunning()) {
            simulation->updateGrid();
            simulation->calcPixels(); 
            UpdateTexture(gridTexture, 
                simulation->getPixels().data());
        }

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(bgColor);
            
            // ui.drawGrid();

            // Defines the section of the original texture image to draw
            Rectangle source = {
                0.0f,
                0.0f,
                (float)gridTexture.width,
                -(float)gridTexture.height
            };

            // Defines where to draw the texture on the screen, using {x, y, width, height} in screen coordinates.
            Rectangle dest = {
                state.getGridStartX(),
                state.getGridStartY(),
                (float)state.getGridDrawWidth(),
                (float)state.getGridDrawHeight()
            };

           DrawTexturePro(
                gridTexture,
                source,
                dest,
                {0.0f, 0.0f},
                0.0f,
                WHITE); // Use WHITE to display the texture normally without color change.
            

            DrawRectangleLines(
                state.getGridStartX(),
                state.getGridStartY(),
                (float)state.getGridDrawWidth(),
                (float)state.getGridDrawHeight(),
                LIGHTGRAY
            );

            ui.handleUI();

        EndDrawing();
    }


}

void App::Launch(){

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   
    InitWindow(windowX, windowY, windowName.c_str());
    SetTargetFPS(targetFPS);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20); 
   
    state.updateGridLayout(GetScreenWidth(), GetScreenHeight());
    generateTextureFromImage(gridTexture, state.getCellsX(), state.getCellsX());

    this->runAppLoop();

    CloseWindow();

}
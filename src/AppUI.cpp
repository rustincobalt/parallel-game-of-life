#include "..\include\AppUI.hpp"
#include "..\include\AppState.hpp"

AppUI::AppUI(){
    panelWidth = 220;
    margin = 10;
    x = margin;
    y = margin;
    w = panelWidth - 2 * margin;
    h = 35;
    textSpacing = h - margin/2;
    blockSpacing = h + margin;

    fpsEditMode = false;
    cellsXEditMode = false;
    cellsYEditMode = false;
    dropDown00EditMode = false;
    dropDown01EditMode = false;

    dropDownDemoPositionY = margin;
    dropDownAlgoPositionY = margin;
}

AppUI::AppUI(AppState& state):AppUI(){
    this->state = &state;
}

void AppUI::drawUIBase() {

    // --- Mode label (dropdown drawn later) ---
    
    
    // --- Mode-specific UI ---
    if (state->getMode() == 0) 
        drawDemoUI();

    else if (state->getMode() == 1) { // TEST
        drawTestUI();
    }

    else if (state->getMode() == 2) { // MEASURE
        drawMeasureUI();
    }

    GuiLabel({x, GetScreenHeight()-margin-blockSpacing-textSpacing, w, h}, "FPS: ");
    static int fps = state->getFPS();
    if( GuiValueBox({x, GetScreenHeight()-margin-blockSpacing, w, h}, nullptr, &fps, 5, 120, fpsEditMode)){
        state->setFPS(fps);
        fpsEditMode = !fpsEditMode;
    }

}


void AppUI::drawUIDropdowns() {

    drawAlgorithmDropdown();
    drawModeDropdown();
}

void AppUI::drawDemoUI(){
    float stepY = 0; 
    setDropDownDemoPositionY(y);
    stepY += textSpacing + blockSpacing;
    
    // Size X
    GuiLabel({x, y+stepY, w, h}, "Size X");
    stepY += textSpacing;

    // static int editX = 0; TODO: DELETE
    static int cellsX = state->getCellsX();
    if( GuiValueBox({x, y+stepY, w, h}, nullptr, &cellsX, 4, 10'000, cellsXEditMode)){
        state->setCellsX(cellsX);
        cellsXEditMode = !cellsXEditMode;
    }
    stepY += blockSpacing;

    // Size Y
    GuiLabel({x, y+stepY, w, h}, "Size Y");
    stepY += textSpacing;

    // static int editY = 0;
    static int cellsY = state->getCellsY();
    if (GuiValueBox({x, y+stepY, w, h}, nullptr, &cellsY, 4, 10'000, cellsYEditMode)){
        state->setCellsY(cellsY);
        cellsYEditMode = !cellsYEditMode; 
    }
    stepY += blockSpacing;

    // Random fill %
    GuiLabel({x, y+stepY, w, h}, "Fill %");
    stepY += textSpacing;


    static float randomFillPercent = (float)state->getRandomFillPercent();
    if (GuiSlider({x + 2 * margin, y + stepY, w - 5 * margin, h}, 
        "0", "100", &randomFillPercent, 0, 100)) 
        {
      state->setRandomFillPercent((int)randomFillPercent);
    }
    GuiLabel({w/2, y+stepY, 60, h}, TextFormat("%d%%", (int)randomFillPercent));
    stepY += blockSpacing;

    // Seed
    GuiLabel({x, y+stepY, w, h}, "Seed");
    stepY += textSpacing;

    static bool editSeed = false;
    static int randomSeed = state->getRandomSeed();
    if (GuiValueBox({x, y+stepY, w, h}, nullptr, &randomSeed, 0, 100000, editSeed)){
        editSeed = !editSeed;
        state->setRandomSeed(randomSeed);
}
    stepY += blockSpacing;

    // Algorithm Drop Down Skip
    setDropDownAlgoPositionY(y+stepY);
    stepY += textSpacing + blockSpacing;


    // Start/Stop
    if (GuiButton({x, y+stepY, w, h},
        state->isRunning() ? "Stop" : "Start")) {
        state->toggleRunning();
    }
    stepY += blockSpacing;

    // Generate
    if (GuiButton({x, y+stepY, w, h}, "Generate")) {
        state->requestGridGenerate();
    }
    stepY += blockSpacing;


}

void AppUI::drawTestUI(){   
    float stepY = 0; 
    setDropDownDemoPositionY(y);
    stepY += textSpacing + blockSpacing;

    setDropDownAlgoPositionY(y+stepY);
    stepY += textSpacing + blockSpacing;
    
    GuiLabel({x, y+stepY, w, h}, TextFormat("Current Primitve: %d", state->getSelectedTestCase()+1));
    stepY += textSpacing;
    
    if (GuiButton({x, y+stepY, w, h}, "Next")) {
            state->incrementTestCase();
    }
    stepY += blockSpacing;

    if (GuiButton({x, y+stepY, w, h}, "Previous")) {
            state->decrementTestCase();
    }
    stepY += blockSpacing;

   

    if (GuiButton({x, y+stepY, w, h},
        state->isRunning() ? "Stop" : "Start")) {
        state->toggleRunning();
    }
    
}

void AppUI::drawMeasureUI()
{
    float stepY = 0; 
    setDropDownDemoPositionY(y);
    stepY += textSpacing + blockSpacing;

    GuiLabel({x, y+stepY, w, h}, "Output file");
    stepY += textSpacing;

    // Placeholder (raygui text box later)
    GuiLabel({x, y+stepY, w, h}, "results.txt");
    stepY += blockSpacing;

    setDropDownAlgoPositionY(y+stepY);
    stepY += textSpacing + blockSpacing;

    if (GuiButton({x, y+stepY, w, h}, "Start")) {
        state->setRunning(true);
    }
    stepY += blockSpacing;

    if (GuiButton({x, y+stepY, w, h}, "Abort")) {
        state->setRunning(false);
    }
    stepY += blockSpacing;
}

void AppUI::drawModeDropdown()
{
    // --- Mode dropdown ---
    GuiLabel({x, dropDownDemoPositionY, w, h}, "Mode");

    int mode = state->getMode();
    if (GuiDropdownBox({x, dropDownDemoPositionY+textSpacing, w, h},
        "Demo;Test;Measure",
        &mode,
        dropDown00EditMode)) {
        dropDown00EditMode = !dropDown00EditMode;
        state->setMode(mode);
    }
    
}

void AppUI::drawAlgorithmDropdown()
{

    // --- Algo dropdown ---
    int algorithm = state->getAlgorithm();
    GuiLabel({x, dropDownAlgoPositionY, w, h}, "Algorithm");
        if (GuiDropdownBox({x, dropDownAlgoPositionY+textSpacing, w, h},
            "Sequential;OpenMPBlocks;OpenMPColumns;OpenMPRows",
            &algorithm,
            dropDown01EditMode)) {
            dropDown01EditMode = !dropDown01EditMode;
            state->setAlgorithm(algorithm);
        }

}

void AppUI::setDropDownDemoPositionY(float posY)
{
    this->dropDownDemoPositionY = posY;
}

void AppUI::setDropDownAlgoPositionY(float posY)
{
    this->dropDownAlgoPositionY = posY;
}

void AppUI::handleUI()
{
    DrawRectangle(0, 0, this->panelWidth, GetScreenHeight(), LIGHTGRAY);
    drawUIBase();
    drawUIDropdowns();
}

// void AppUI::drawGrid(){

//     int availableWidth = GetScreenWidth() - this->panelWidth;
//     int availableHeight = GetScreenHeight();

//     int cellSize = std::min(
//         availableWidth / state->cellsX,
//         availableHeight / state->cellsY
//     );

//     cellSize = std::min(cellSize, state->maxCellSizePX);
    
    
//     float startX = this->panelWidth + (availableWidth - state->cellsX * cellSize) / 2.;
//     float startY = (availableHeight - state->cellsY * cellSize) / 2.;

//     for (int y = 0; y < state->cellsY; y++) {
//         for (int x = 0; x < state->cellsX; x++) {

//             DrawRectangleLines(
//                 startX + x * cellSize,
//                 startY + y * cellSize,
//                 cellSize,
//                 cellSize,
//                 DARKGRAY
//             );
//         }
//     }
// }

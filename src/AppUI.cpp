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
    if (state->mode == 0) 
        drawDemoUI();

    else if (state->mode == 1) { // TEST
        drawTestUI();
    }

    else if (state->mode == 2) { // MEASURE
        drawMeasureUI();
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

    static int editX = 0;
    GuiValueBox({x, y+stepY, w, h}, nullptr, &state->cellsX, 10, 500, editX);
    stepY += blockSpacing;

    // Size Y
    GuiLabel({x, y+stepY, w, h}, "Size Y");
    stepY += textSpacing;

    static int editY = 0;
    GuiValueBox({x, y+stepY, w, h}, nullptr, &state->cellsY, 10, 500, editY);
    stepY += blockSpacing;

    // Random fill %
    GuiLabel({x, y+stepY, w, h}, "Fill %");
    stepY += textSpacing;

    GuiSlider({x+2*margin, y+stepY, w-5*margin, h}, "0", "100", 
                (float*)&state->randomFillPercent, 0, 100);
    stepY += blockSpacing;

    // Seed
    GuiLabel({x, y+stepY, w, h}, "Seed");
    stepY += textSpacing;

    static int editSeed = 0;
    GuiValueBox({x, y+stepY, w, h}, nullptr, &state->randomSeed, 0, 100000, editSeed);
    stepY += blockSpacing;

    // Algorithm Drop Down Skip
    setDropDownAlgoPositionY(y+stepY);
    stepY += textSpacing + blockSpacing;


    // Start/Stop
    if (GuiButton({x, y+stepY, w, h},
        state->running ? "Stop" : "Start")) {
        state->running = !state->running;
    }
    stepY += blockSpacing;

    // Reset
    if (GuiButton({x, y+stepY, w, h}, "Reset")) {
        state->running = false;
    }
    stepY += blockSpacing;


}

void AppUI::drawTestUI(){   
    float stepY = 0; 
    setDropDownDemoPositionY(y);
    stepY += textSpacing + blockSpacing;

    setDropDownAlgoPositionY(y+stepY);
    stepY += textSpacing + blockSpacing;

    if (GuiButton({x, y+stepY, w, h}, "Previous")) {
            // TODO
    }
    stepY += blockSpacing;

    if (GuiButton({x, y+stepY, w, h}, "Next")) {
            // TODO
    }
    stepY += blockSpacing;

    if (GuiButton({x, y+stepY, w, h},
        state->running ? "Stop" : "Start")) {
        state->running = !state->running;
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
        state->running = true;
    }
    stepY += blockSpacing;

    if (GuiButton({x, y+stepY, w, h}, "Abort")) {
        state->running = false;
    }
    stepY += blockSpacing;
}

void AppUI::drawModeDropdown()
{
    // --- Mode dropdown ---
    GuiLabel({x, dropDownDemoPositionY, w, h}, "Mode");

    if (GuiDropdownBox({x, dropDownDemoPositionY+textSpacing, w, h},
        "Demo;Test;Measure",
        &state->mode,
        dropDown00EditMode)) {
        dropDown00EditMode = !dropDown00EditMode;
    }
    
}

void AppUI::drawAlgorithmDropdown()
{

    // --- Algo dropdown ---
    GuiLabel({x, dropDownAlgoPositionY, w, h}, "Algorithm");
        if (GuiDropdownBox({x, dropDownAlgoPositionY+textSpacing, w, h},
            "Sequential;Parallel",
            &state->algorithm,
            dropDown01EditMode)) {
            dropDown01EditMode = !dropDown01EditMode;
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

void AppUI::drawGrid(){

    int availableWidth = GetScreenWidth() - this->panelWidth;
    int availableHeight = GetScreenHeight();

    int cellSize = std::min(
        availableWidth / state->cellsX,
        availableHeight / state->cellsY
    );

    cellSize = std::min(cellSize, state->maxCellSize);
    
    
    float startX = this->panelWidth + (availableWidth - state->cellsX * cellSize) / 2.;
    float startY = (availableHeight - state->cellsY * cellSize) / 2.;

    for (int y = 0; y < state->cellsY; y++) {
        for (int x = 0; x < state->cellsX; x++) {

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

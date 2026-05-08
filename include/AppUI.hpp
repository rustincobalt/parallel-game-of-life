#pragma once


extern "C" {
    #include "..\lib\raygui.h"
}


struct AppState;

class AppUI{

    AppState* state;

    // --- Side Panel ---
    int panelWidth;
    float margin;
    float x;
    float y;
    float w;
    float h;
    float textSpacing;
    float blockSpacing;

    // --- EditModes ---
    bool fpsEditMode;
    bool cellsXEditMode;
    bool cellsYEditMode;


    // --- Drop Down ---
    bool dropDown00EditMode;
    bool dropDown01EditMode;
    
    float dropDownDemoPositionY;
    float dropDownAlgoPositionY;

    void setDropDownDemoPositionY(float);
    void setDropDownAlgoPositionY(float);

    void drawUIBase();
    void drawUIDropdowns();

    void drawDemoUI();
    void drawTestUI();
    void drawMeasureUI();

    void drawModeDropdown();
    void drawAlgorithmDropdown();

    

    public:
    AppUI();
    AppUI(AppState&);
    // void drawGrid(); TODO: DELETE
    void handleUI();

};
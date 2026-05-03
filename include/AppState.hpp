/*

Without it:

UI variables get scattered
simulation reads random fields from App
hard to scale

With it:

UI edits state
simulation reads state
rendering uses state

*/
#pragma once

struct AppState {
    int running = false;
    
    int mode = 0; // demo - 0 , test - 1, measure - 2
    int algorithm = 0; // 0=seq,1=parallel
    int randomFillPercent = 30;
    int randomSeed = 1;


    int cellsX = 10;
    int cellsY = 5;

    int maxCellSize = 20;
};
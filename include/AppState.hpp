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


struct AppState {
    int running = false;
    
    int mode = 0; // paused - 0, demo 1 , test - 2, measure - 3
    int algorithm = 0; // 0=seq,1=parallel

    int cellsX = 10;
    int cellsY = 5;

    int maxCellSize = 20;
};
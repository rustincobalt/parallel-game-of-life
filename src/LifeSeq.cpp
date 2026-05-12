#include "../include/LifeSeq.hpp"
#include <cstdint>

LifeSeq::LifeSeq(int h, int w): Life(h, w){}

LifeSeq::LifeSeq(int h, int w, uint32_t deadColor , uint32_t  aliveColor): 
Life(h, w, deadColor, aliveColor){}


void LifeSeq::updateGrid() {
    updateCornersPadding(currGrid);
    updateHorizontalPadding(currGrid, 1, rows-2);
    updateVerticalPadding(currGrid, 1, width-2);

    
    // updateRowRange(currGrid, nextGrid, 1, rows-2);
    updateBlock(currGrid, nextGrid, 1, rows-2, 1, width-2);
    currGrid.swap(nextGrid);
    // calcPixelsRowRange(currGrid, 1, rows-2);
    // calcPixelsBlock(currGrid, 1, rows-2, 1, width-2);
}


void LifeSeq::calcPixels(){
    calcPixelsRowRange(currGrid, 1, rows-2);
}

void LifeSeq::runUpdateIterations(int iterations){
    for (int i = 0; i < iterations; i++)
        this->updateGrid();
}
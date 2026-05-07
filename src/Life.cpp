#include "..\include\Life.hpp"
#include <cstdint>

Life::GridCorners::GridCorners(int rows, int width){
    {
        const int topRow = 0;
        const int bottomRow = rows - 1;

        const int realTopRow = 1;
        const int realBottomRow = rows - 2;

        const int left = 0;
        const int right = width - 1;

        const int realLeft = 1;
        const int realRight = width - 2;

        // Padding corners
        topLeftPad     = topRow * width + left;
        topRightPad    = topRow * width + right;
        bottomLeftPad  = bottomRow * width + left;
        bottomRightPad = bottomRow * width + right;

        // Real corners
        topLeftReal     = realTopRow * width + realLeft;
        topRightReal    = realTopRow * width + realRight;
        bottomLeftReal  = realBottomRow * width + realLeft;
        bottomRightReal = realBottomRow * width + realRight;
    }
}
  
Life::Life(int h, int w): rows(h+2), width(w+2), DEAD_COLOR(0xFF000000), ALIVE_COLOR(0xFF00FF00) 
{   
    
    CORNERS = GridCorners(rows, width);
    grid.assign(rows*width, 0);
    pixels.resize(h*w);

    PIXEL_LUT.resize(2);
    PIXEL_LUT[0] = DEAD_COLOR;
    PIXEL_LUT[1] = ALIVE_COLOR;
}


void Life::updateRowRange(const vector<uint8_t> &currGrid, vector<uint8_t> &nextGrid, int stRow, int enRow) {

    const uint8_t* currGridPtr = currGrid.data();
    uint8_t* nextGridPtr = nextGrid.data();

    for (int row = stRow; row <= enRow; row++) {
        const int rowStart = row * width;

        const uint8_t *topcell = currGridPtr + rowStart - width + 1;
        const uint8_t *midCell = currGridPtr + rowStart + 1;
        const uint8_t *botCell = currGridPtr + rowStart + width + 1;

        uint8_t *outCell = nextGridPtr + rowStart + 1;

        for (int cell = 1; cell < width - 1; cell++) {
            uint8_t aliveNeighbours = topcell[-1] + topcell[0] + topcell[1] + midCell[-1] + midCell[1] +
                                        botCell[-1] + botCell[0] + botCell[1];

            *outCell = STATUS_TABLE[aliveNeighbours][midCell[0]];

            ++topcell;
            ++midCell;
            ++botCell;
            ++outCell;
        }
    }
}

void Life::updateHorizontalPadding(vector<uint8_t> &grid, int stRow, int enRow) {


    for (int row = stRow; row <= enRow; row++) {
        const int rowStart = row * width;

        grid[rowStart] = grid[rowStart + width - 2];

        grid[rowStart + width - 1] = grid[rowStart + 1];
  }
}

void Life::updateVerticalPadding(vector<uint8_t> &grid, int stCell, int enCell) {
  

    const int paddLastRow = rows - 1;
    const int realLastRow = rows - 2;

    for (int cell = stCell; cell <= enCell; cell++) {
        // Top padding row
        grid[cell] = grid[realLastRow * width + cell];

        // Bottom padding row
        grid[paddLastRow * width + cell] = grid[width + cell];
    }
}

void Life::updateCornersPadding(vector<uint8_t>& grid)
{
    grid[CORNERS.topLeftPad] = grid[CORNERS.bottomRightReal];
    grid[CORNERS.topRightPad] = grid[CORNERS.bottomLeftReal];

    grid[CORNERS.bottomLeftPad] = grid[CORNERS.topRightReal];
    grid[CORNERS.bottomRightPad] = grid[CORNERS.topLeftReal];
}

void Life::calcPixelsRowRange(int stRow, int enRow)
{
    const int realWidth = width - 2;

    const uint8_t* srcPtr = grid.data();
    uint32_t* dstPtr = pixels.data();

    for (int row = stRow; row <= enRow; row++)
    {
        const uint8_t* cellRow =
            srcPtr + row * width + 1;

        uint32_t* pixelRow =
            dstPtr + (row - 1) * realWidth;

        for (int col = 0; col < realWidth; col++)
        {
            pixelRow[col] =
                PIXEL_LUT[cellRow[col]];
        }
    }
}

const vector<uint32_t>& Life::getPixels()
{
    return pixels;
}

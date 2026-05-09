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


Life::Life(int h, int w): Life(h, w, 0xFF000000, 0xFF00FF00) {}

Life::Life(int h, int w, uint32_t deadColor, uint32_t aliveColor)
    : rows(h+2), width(w+2),
      DEAD_COLOR(deadColor), ALIVE_COLOR(aliveColor), nextGrid(rows*width)
{
    CORNERS = GridCorners(rows, width);
    currGrid.assign(rows*width, 0);
    
    pixels.resize(h*w);

    PIXEL_LUT = { DEAD_COLOR, ALIVE_COLOR };
}


void Life::UpdateGridParallelInternal() {
    updateGrid();
}

void Life::updateRowRange(const vector<uint8_t> &currGrid, vector<uint8_t> &nextGrid, int stRow, int enRow) {

    const uint8_t* currGridPtr = currGrid.data();
    uint8_t* nextGridPtr = nextGrid.data();

    for (int row = stRow; row <= enRow; row++) {
        const int rowStart = row * width;

        const uint8_t *topCell = currGridPtr + rowStart - width + 1;
        const uint8_t *midCell = currGridPtr + rowStart + 1;
        const uint8_t *botCell = currGridPtr + rowStart + width + 1;

        uint8_t *outCell = nextGridPtr + rowStart + 1;

        for (int cell = 1; cell < width - 1; cell++) {
            uint8_t aliveNeighbours = topCell[-1] + topCell[0] + topCell[1] + midCell[-1] + midCell[1] +
                                        botCell[-1] + botCell[0] + botCell[1];

            *outCell = STATUS_TABLE[aliveNeighbours][midCell[0]];

            ++topCell;
            ++midCell;
            ++botCell;
            ++outCell;
        }
    }
}

void Life::updateBlock(const vector<uint8_t> &currGrid, vector<uint8_t> &nextGrid, 
                        int stRow, int enRow, 
                        int stCol, int enCol) 
                        {
    const uint8_t *currGridPtr = currGrid.data();
    uint8_t *nextGridPtr = nextGrid.data();

    for (int row = stRow; row <= enRow; row++) {
        const int rowStart = row * width;

        const uint8_t *topCell = currGridPtr + rowStart - width + stCol;
        const uint8_t *midCell = currGridPtr + rowStart + stCol;
        const uint8_t *botCell = currGridPtr + rowStart + width + stCol;

        uint8_t *outCell = nextGridPtr + rowStart + stCol;

        for (int col = stCol; col <= enCol; col++) {
            uint8_t aliveNeighbours = topCell[-1] + topCell[0] + topCell[1] +
                                        midCell[-1] + midCell[1] + botCell[-1] +
                                        botCell[0] + botCell[1];

            *outCell = STATUS_TABLE[aliveNeighbours][midCell[0]];

            ++topCell;
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

void Life::calcPixelsRowRange(const vector<uint8_t>& grid, int stRow, int enRow)
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

void Life::calcPixelsBlock(const vector<uint8_t>& grid, int stRow, int enRow, int stCol, int enCol)
{
    const int realWidth = width - 2;

    const uint8_t* srcPtr = grid.data();
    uint32_t* dstPtr = pixels.data();

    for (int row = stRow; row <= enRow; row++)
    {
        const uint8_t* cellBlockRow =
            srcPtr + row * width + stCol;

        uint32_t* pixelBlockRow =
            dstPtr + (row - 1) * realWidth + (stCol - 1);

        for (int col = stCol; col <= enCol; col++)
        {
            pixelBlockRow[0] =
                PIXEL_LUT[cellBlockRow[0]];
            
            ++pixelBlockRow;
            ++cellBlockRow;
        }
    }
}

const vector<uint32_t>& Life::getPixels()
{
    return pixels;
}


void Life::generateRandomGrid(int seed, int fillPercent)
{
    std::mt19937 rng(seed);

    std::uniform_int_distribution<int>
        dist(0, 99);

    const int realWidth = width - 2;
    const int realHeight = rows - 2;

    uint8_t* gridPtr = currGrid.data();

    for (int row = 1; row <= realHeight; row++)
    {
        uint8_t* cellsRow =
            gridPtr + row * width + 1;

        for (int col = 0; col < realWidth; col++)
        {
            cellsRow[col] =
                dist(rng) < fillPercent;
        }
    }

    updateHorizontalPadding(
        currGrid,
        1,
        realHeight);

    updateVerticalPadding(
        currGrid,
        1,
        realWidth);

    updateCornersPadding(currGrid);
}

void Life::loadPrimitive(int primitiveId){

    // primitiveId = primitiveId % TOTAL_PRIMITIVES;
    this->rows = PRIMITIVE_GRID_SIZES[primitiveId][0];
    this->width = PRIMITIVE_GRID_SIZES[primitiveId][1];

    CORNERS = GridCorners(rows, width);
    currGrid.assign(rows*width, 0);
    nextGrid.resize(rows*width);

    switch (primitiveId) {
    case 0:
      currGrid = vector<uint8_t>(MAX_STILL_LIFE, MAX_STILL_LIFE + rows * width);
      break;
    case 1:
      currGrid = vector<uint8_t>(BLOCK, BLOCK + rows * width);
      break;
    case 2:
      currGrid = vector<uint8_t>(BLINKER, BLINKER + rows * width);
      break;
    case 3:
      currGrid = vector<uint8_t>(PULSAR, PULSAR + rows * width);
      break;
    case 4:
      currGrid = vector<uint8_t>(BONE, BONE + rows * width);
      break;
    default:
      currGrid = vector<uint8_t>(MID_SPACESHIP, MID_SPACESHIP + rows * width);
      break;
    }

    pixels.resize((rows-2)*(width-2));
}


int Life::getRealHeight() const {
    return rows-2;
}
int Life::getRealWidth() const {
    return width-2;
}
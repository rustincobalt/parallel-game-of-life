#include "../include/LifeOMPBlocks.hpp"

LifeOMPBlocks::LifeOMPBlocks(
    int h,
    int w,
    int threads)
    :
    Life(h, w),
    threadsNumber(threads)
{
    calculateBlockSizes();
}

LifeOMPBlocks::LifeOMPBlocks(
    int h,
    int w,
    uint32_t deadColor,
    uint32_t aliveColor,
    int threads)
    :
    Life(h, w, deadColor, aliveColor),
    threadsNumber(threads)
{
    calculateBlockSizes();
}

void LifeOMPBlocks::calculateBlockSizes(){
    const int usableRows = rows - 2;
    const int usableCols = width - 2;

    int blockRows =
        static_cast<int>(std::sqrt(threadsNumber));

    if (blockRows < 1)
        blockRows = 1;

    int blockCols =
        (threadsNumber + blockRows - 1) / blockRows;

    blockHeight =
        std::max(1, usableRows / blockRows);

    blockWidth =
        std::max(1, usableCols / blockCols);
}

void LifeOMPBlocks::UpdateGridParallelInternal() {

    #pragma omp single nowait
    updateHorizontalPadding(currGrid, 1, rows - 2);
    
    #pragma omp single nowait
    updateVerticalPadding(currGrid, 1, width - 2);
    
    #pragma omp single // implicit barrier
    updateCornersPadding(currGrid);
    

    #pragma omp for collapse(2) schedule(dynamic)
    for (int row = 1; row < rows - 1; row+=blockHeight) {
        for (int col = 1; col < width - 1; col++) {
            const int endRow = std::min(row + blockHeight - 1, rows - 2);
            // const int endCol = std::min(col + blockWidth - 1, width - 2);

            updateBlock(currGrid, nextGrid, row, endRow, col, col);
            calcPixelsBlock(nextGrid, row, endRow, col, col);
        }
    }

    // Only one must perform
    #pragma omp single
    {
        currGrid.swap(nextGrid);
    }
}




void LifeOMPBlocks::updateGrid()
{   
    
    #pragma omp parallel num_threads(threadsNumber)
    {

        #pragma single nowait
            updateHorizontalPadding(currGrid, 1, rows - 2);
        

        #pragma single nowait
            updateVerticalPadding(currGrid, 1, width - 2);
        

        #pragma single // implicit barrier
            updateCornersPadding(currGrid);
        
        


        #pragma omp for collapse(2) schedule(dynamic)
        for (int row = 1; row < rows - 1; row+=blockHeight){
            for (int col = 1; col < width - 1; col++){
                const int endRow = std::min(row + blockHeight - 1, rows - 2);
                // const int endCol = std::min(col + blockWidth - 1, width - 2);

                updateBlock(currGrid,
                    nextGrid,
                    row,
                    endRow,
                    col,
                    col);

                calcPixelsBlock(nextGrid,
                    row,
                    endRow,
                    col,
                    col);
            }
        }     
    }
    currGrid.swap(nextGrid);
}


void LifeOMPBlocks::calcPixels()
{
    #pragma omp parallel for collapse(2) schedule(dynamic) num_threads(threadsNumber)
    for (int row = 1; row < rows - 1; row += blockHeight){
        for (int col = 1; col < width - 1; col += blockWidth){
            const int endRow =
                std::min(row + blockHeight - 1, rows - 2);

            const int endCol =
                std::min(col + blockWidth - 1, width - 2);

            calcPixelsBlock(currGrid,
                row,
                endRow,
                col,
                endCol);
        }
    }
}

void LifeOMPBlocks::runUpdateIterations(int iterations){
    #pragma omp parallel num_threads(threadsNumber)
    {
        for (int i = 0; i < iterations; i++) {
            UpdateGridParallelInternal();
        }
    }
}
#include "../include/LifeOMPRows.hpp"

void LifeOMPRows::calculateRowChunkSize() {
    const int realRows = rows - 2;
    
    if (realRows < 256) {
        rowChunkSize = 1;
        return;
    }

    const int targetChunks = threadsNumber * 1.8;


    int chunk = realRows / targetChunks;

    chunk = (chunk / 64) * 64;

    if (chunk < 64)
        chunk = 64;

    rowChunkSize = chunk;
}

LifeOMPRows::LifeOMPRows(
    int h,
    int w,
    int threads)
    :
    Life(h, w),
    threadsNumber(threads)
{
    calculateRowChunkSize();
}

LifeOMPRows::LifeOMPRows(
    int h,
    int w,
    uint32_t deadColor,
    uint32_t aliveColor,
    int threads)
    :
    Life(h, w, deadColor, aliveColor),
    threadsNumber(threads)
{
    calculateRowChunkSize();
}


void LifeOMPRows::UpdateGridParallelInternal()
{
    #pragma omp single nowait
    updateHorizontalPadding(currGrid, 1, rows - 2);

    #pragma omp single nowait
    updateVerticalPadding(currGrid, 1, width - 2);

    #pragma omp single
    updateCornersPadding(currGrid);

    #pragma omp for schedule(dynamic)
    for (int row = 1; row < rows - 1; row += rowChunkSize){
        
        const int enRow = std::min(row + rowChunkSize - 1, rows - 2);

        updateBlock(currGrid, nextGrid,
            row, enRow,
            1, width - 2);
    }

    #pragma omp single
    currGrid.swap(nextGrid);
}

void LifeOMPRows::updateGrid()
{   
    #pragma omp parallel num_threads(threadsNumber)
    {
        #pragma omp single nowait
        updateHorizontalPadding(currGrid, 1, rows - 2);
        

        #pragma omp single nowait
        updateVerticalPadding(currGrid, 1, width - 2);
        

        #pragma omp single // implicit barrier
        updateCornersPadding(currGrid);


        #pragma omp for schedule(dynamic)
        for (int row = 1; row < rows - 1; row += rowChunkSize){
        
            const int enRow = std::min(row + rowChunkSize - 1, rows - 2);

            updateBlock(currGrid, nextGrid,
                row, enRow,
                1, width - 2);
        }
    }
    currGrid.swap(nextGrid);
}

void LifeOMPRows::calcPixels()
{
    #pragma omp parallel for schedule(static) num_threads(threadsNumber)
    for (int row = 1; row < rows - 1; row += rowChunkSize){
        
        const int enRow = std::min(row + rowChunkSize - 1, rows - 2);

        calcPixelsBlock(currGrid,
            row,
            enRow,
            1,
            width-2);
    }
    
}

void LifeOMPRows::runUpdateIterations(int iterations){
    #pragma omp parallel num_threads(threadsNumber)
    {
        for (int i = 0; i < iterations; i++) {
            UpdateGridParallelInternal();
        }
    }
}
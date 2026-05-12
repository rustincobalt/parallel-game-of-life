#include "../include/LifeOMPColumns.hpp"

void LifeOMPColumns::calculateColumnChunkSize() {
    const int realRows = rows - 2;

    if (realRows < 256) {
        colChunkSize = 1;
        return;
    }

    const int targetChunks = threadsNumber * 1.8;

    int chunk = realRows / targetChunks;

    chunk = (chunk / 64) * 64;

    if (chunk < 64)
        chunk = 64;

    colChunkSize = chunk;
}

LifeOMPColumns::LifeOMPColumns(
    int h,
    int w,
    int threads)
    :
    Life(h, w),
    threadsNumber(threads)
{
    calculateColumnChunkSize();
}

LifeOMPColumns::LifeOMPColumns(
    int h,
    int w,
    uint32_t deadColor,
    uint32_t aliveColor,
    int threads)
    :
    Life(h, w, deadColor, aliveColor),
    threadsNumber(threads)
{
    calculateColumnChunkSize();
}


void LifeOMPColumns::UpdateGridParallelInternal()
{
    #pragma omp single nowait
    updateHorizontalPadding(currGrid, 1, rows - 2);

    #pragma omp single nowait
    updateVerticalPadding(currGrid, 1, width - 2);

    #pragma omp single
    updateCornersPadding(currGrid);

    #pragma omp for schedule(dynamic)
    for (int col = 1; col < width - 1; col+=colChunkSize)
    {
        const int enCol = std::min( col + colChunkSize - 1, width - 2);

        updateBlock(currGrid, nextGrid,
            1, rows - 2,
            col, enCol);
    }

    #pragma omp single
    currGrid.swap(nextGrid);
}


void LifeOMPColumns::updateGrid()
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
        for (int col = 1; col < width - 1; col+=colChunkSize)
        {
            const int enCol = std::min(col + colChunkSize - 1, width - 2);

            updateBlock( currGrid, nextGrid,
                1, rows -2,
                col, enCol);
        }
    }

    currGrid.swap(nextGrid);
}


void LifeOMPColumns::calcPixels()
{
    #pragma omp parallel for schedule(static) num_threads(threadsNumber)
    for (int col = 1; col < width - 1; col += colChunkSize){
        
        const int enCol = std::min(col + colChunkSize - 1, width - 2);

        calcPixelsBlock(currGrid,
            1,
            rows-2,
            col,
            enCol);
    }
    
}

void LifeOMPColumns::runUpdateIterations(int iterations){
    #pragma omp parallel num_threads(threadsNumber)
    {
        for (int i = 0; i < iterations; i++) {
            UpdateGridParallelInternal();
        }
    }
}
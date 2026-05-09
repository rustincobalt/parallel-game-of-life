#include "../include/LifeOMPColumns.hpp"

void LifeOMPColumns::calculateColumnChunkSize(){
    const int realColumns = width-2;
    colChunkSize = std::max(1, realColumns / threadsNumber);
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

    #pragma omp for schedule(static)
    for (int col = 1; col < width - 1; col += colChunkSize)
    {
        const int enCol = std::min( col + colChunkSize - 1, width - 2);

        updateBlock(currGrid, nextGrid,
            1, rows - 2,
            col, enCol);

        calcPixelsBlock(nextGrid,
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
        #pragma single nowait
        updateHorizontalPadding(currGrid, 1, rows - 2);
        

        #pragma single nowait
        updateVerticalPadding(currGrid, 1, width - 2);
        

        #pragma single // implicit barrier
        updateCornersPadding(currGrid);


        #pragma omp for schedule(static)
        for (int col = 1; col < width - 1; col += colChunkSize)
        {
            const int endCol = std::min(col + colChunkSize - 1, width - 2);

            updateBlock( currGrid, nextGrid,
                1, rows -2,
                col, endCol);


            calcPixelsBlock( nextGrid,
                1, rows - 2,
                col, endCol);
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
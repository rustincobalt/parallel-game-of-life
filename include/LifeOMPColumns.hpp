#pragma once

#include "Life.hpp"
#include <algorithm>

class LifeOMPColumns : public Life {

private:
    int threadsNumber;
    int colChunkSize;

    void calculateColumnChunkSize();

public:

    LifeOMPColumns(
        int h,
        int w,
        int threads);

    LifeOMPColumns(
        int h,
        int w,
        uint32_t deadColor,
        uint32_t aliveColor,
        int threads);

    virtual void updateGrid() override;

    virtual void calcPixels() override;

    virtual void UpdateGridParallelInternal() override;
};
#pragma once
#include "Life.hpp"
#include <cstdint>


class LifeOMPBlocks : public Life {

private:
    int threadsNumber;

    int blockHeight;
    int blockWidth;

    void calculateBlockSizes();

public:
    LifeOMPBlocks(
        int h,
        int w,
        int threads);

   LifeOMPBlocks(
        int h,
        int w,
        uint32_t deadColor,
        uint32_t aliveColor,
        int threads);

    virtual void updateGrid() override;
    virtual void calcPixels() override;

    void UpdateGridParallelInternal() override;
};
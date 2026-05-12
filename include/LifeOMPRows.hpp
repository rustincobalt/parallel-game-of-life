#pragma once

#include "Life.hpp"

class LifeOMPRows : public Life {

private:
    int threadsNumber;
    int rowChunkSize;

    void calculateRowChunkSize();
public:
    LifeOMPRows(int h, int w, int threads);

    LifeOMPRows(
        int h,
        int w,
        uint32_t deadColor,
        uint32_t aliveColor,
        int threads);

    virtual void updateGrid() override;
    virtual void calcPixels() override;
    virtual void runUpdateIterations(int iterations) override;
    virtual void UpdateGridParallelInternal() override;
};
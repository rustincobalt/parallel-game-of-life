#include "Life.hpp"
#include <cstdint>

// Sequential implementation of a class Life
class LifeSeq : public Life {

    public:
    LifeSeq(int h, int w);
    LifeSeq(int h, int w, uint32_t deadColor , uint32_t  aliveColor);


    virtual void updateGrid() override;
    virtual void calcPixels() override;
    virtual void runUpdateIterations(int iterations) override;
};
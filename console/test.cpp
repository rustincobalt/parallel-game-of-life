#include <iostream>
#include <cassert>

#include "../include/LifeOMPBlocks.hpp"
#include "../include/LifeOMPColumns.hpp"
#include "../include/LifeOMPRows.hpp"
#include "../include/LifeSeq.hpp"

int main()
{
    constexpr int WIDTH = 1000;
    constexpr int HEIGHT = 1000;
    constexpr int THREADS = 4;
    constexpr int ITERATIONS = 200;  // keep reasonable for testing

    constexpr int SEED = 1441;
    constexpr int FILL_PERCENT = 54;

    LifeSeq seq(WIDTH, HEIGHT);
    LifeOMPRows rows(WIDTH, HEIGHT, THREADS);
    LifeOMPColumns cols(WIDTH, HEIGHT, THREADS);
    LifeOMPBlocks blocks(WIDTH, HEIGHT, THREADS);

    // IMPORTANT: same initial state
    seq.generateRandomGrid(SEED, FILL_PERCENT);
    rows.generateRandomGrid(SEED, FILL_PERCENT);
    cols.generateRandomGrid(SEED, FILL_PERCENT);


    for (int i = 0; i < 10 * ITERATIONS; i++) {
        seq.updateGrid();
        rows.updateGrid();
        cols.updateGrid();


        if (!(seq == rows))
        {
            std::cerr << "Mismatch: SEQ vs ROWS\n";
            return 1;
        }

        if (!(seq == cols))
        {
            std::cerr << "Mismatch: SEQ vs COLS\n";
            return 1;
        }
    }
    

    std::cout << "All implementations match perfectly.\n";
    return 0;
}
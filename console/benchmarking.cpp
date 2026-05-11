#define CONSOLE_PRINT

#include <algorithm>
#include <chrono>
#include <cmath>
#include <conio.h>
#include <cstdio>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

// #include "../include/Life.hpp"
#include "../include/LifeOMPBlocks.hpp"
#include "../include/LifeOMPColumns.hpp"
#include "../include/LifeOMPRows.hpp"
#include "../include/LifeSeq.hpp"

// using Clock = std::chrono::high_resolution_clock;
using Clock = std::chrono::steady_clock;
using ms = std::chrono::duration<double, std::milli>;

constexpr int GENERATIONS = 250;
constexpr int WARMUP_RUNS = 5;
constexpr int MEASURE_RUNS = 25;

// ============================================================
// CONFIGURATION
// ============================================================

std::vector<int> threadCounts = {1, 2,3, 4, 5, 6, 7, 8, 
    12, 16, 20
};

// std::vector<std::pair<int, int>> gridSizes = {
//     {64, 64},
//     {128, 128},
//     {256, 256},
//     {512, 512},
//     {1024, 1024},
//     {2048, 2048}
// };


std::vector<std::pair<int, int>> gridSizes = {
    {100, 100},
    {500,500},
    {1000,1000},
    {1500,1500},
        {1750,1750},
    {2000,2000},
    {2250,2250},
    {2500,2500},
    {3000,3000},
    // {4000,4000},
    // {6000,6000},
    // {10'000, 10'000}
};

std::vector<int> seeds = {42};

std::vector<float> fillPercents = {0.4f};

// ============================================================

bool stopRequested = false;

// ============================================================
// ALGORITHM TYPE
// ============================================================

enum class AlgorithmType {
    Sequential,
    OMP_Rows,
    OMP_Columns,
    OMP_Blocks
};

// ============================================================
// OUTLIER FILTER
// ============================================================

std::vector<double> removeOutliersIterative(
    std::vector<double> data,
    double k = 2.0,
    int maxIters = 6
) {
    for (int iter = 0; iter < maxIters; iter++) {

        double mean =
            std::accumulate(data.begin(), data.end(), 0.0)
            / data.size();

        double variance = 0.0;

        for (double x : data) {
            variance += (x - mean) * (x - mean);
        }

        variance /= data.size();

        double stddev = std::sqrt(variance);

        std::vector<double> filtered;

        for (double x : data) {
            if (std::abs(x - mean) <= k * stddev) {
                filtered.push_back(x);
            }
        }

        if (filtered.size() == data.size()) {
            break;
        }

        data = filtered;
    }

    return data;
}

// ============================================================

double average(const std::vector<double>& data) {
    return std::accumulate(data.begin(), data.end(), 0.0)
           / data.size();
}

// ============================================================
// LIFE FACTORY
// ============================================================

std::unique_ptr<Life> createLife(
    AlgorithmType type,
    int height,
    int width,
    int threads
) {
    if (type == AlgorithmType::Sequential) {
        return std::make_unique<LifeSeq>(height, width);
    }

    if (type == AlgorithmType::OMP_Rows) {
        return std::make_unique<LifeOMPRows>(
            height,
            width,
            threads
        );
    }

    if (type == AlgorithmType::OMP_Columns) {
        return std::make_unique<LifeOMPColumns>(
            height,
            width,
            threads
        );
    }

    return std::make_unique<LifeOMPBlocks>(
        height,
        width,
        threads
    );
}

// ============================================================
// BENCHMARK
// ============================================================

double benchmarkLife(Life& life) {
    auto start = Clock::now();

    life.runUpdateIterations(GENERATIONS);

    auto end = Clock::now();

    return ms(end - start).count();
}

// ============================================================

void writeMeasurement(
    FILE* csv,
    const std::string& name,
    int width,
    int height,
    int threads,
    int seed,
    float fill,
    int run,
    double elapsed
) {
    fprintf(
        csv,
        "%s,%d,%d,%d,%d,%.2f,%d,%.6f\n",
        name.c_str(),
        width,
        height,
        threads,
        seed,
        fill,
        run,
        elapsed
    );
}

// ============================================================

void printBenchmarkInfo(
    const std::string& name,
    int width,
    int height,
    int threads,
    int seed,
    float fill
) {
    printf(
        "\n[%s] Grid=%dx%d Threads=%d Seed=%d Fill=%.2f\n",
        name.c_str(),
        width,
        height,
        threads,
        seed,
        fill
    );
}

// ============================================================

void performBenchmark(AlgorithmType type, const std::string& name, FILE* csv) {
    for (auto [height, width] : gridSizes) {

        for (int threads : threadCounts) {

            if (
                type == AlgorithmType::Sequential
                && threads != 1
            ) {
                continue;
            }

            if (
                type != AlgorithmType::Sequential
                && threads == 1
            ) {
                continue;
            }

            for (int seed : seeds) {

                for (float fill : fillPercents) {

                    // if (stopRequested)
                    //     return;
                    
                    printBenchmarkInfo(name, width, height, threads, seed, fill);


                    // === WARM UP ===
                    // auto warmupLife = createLife(type, height, width, threads);
                    auto localLife = createLife(type, height, width, threads);
                    localLife->generateRandomGrid(seed, (int)fill*100);

                    for (int i = 0; i < WARMUP_RUNS; i++) 
                        benchmarkLife(*localLife);
                    

                    // === MEASURE ===
                    std::vector<double> measurements;
                    measurements.reserve(MEASURE_RUNS);

                    for (int run = 0; run < MEASURE_RUNS; run++) {

                        // if (_kbhit()) {
                        //     stopRequested = true;
                        //     return;
                        // }

                        // auto localLife = createLife(type,height,width,threads);

                        localLife->generateRandomGrid(seed,fill);

                        double elapsed = benchmarkLife(*localLife);

                        measurements.push_back(elapsed);
#ifdef CONSOLE_PRINT
                        printf("Run %2d/%d : %.3f ms\n",run + 1,MEASURE_RUNS,elapsed);
#endif
                        writeMeasurement(csv,name,width,height,threads,seed,fill,run,elapsed);
                    }

                    auto filtered = removeOutliersIterative(measurements);

                    double avg = average(filtered);
#ifdef CONSOLE_PRINT
                    printf("Average: %.3f ms (filtered count=%zu)\n", avg, filtered.size());
#endif
                    fprintf(csv,"%s,%d,%d,%d,%d,%.2f,AVG,%.6f\n",
                        name.c_str(),width,height,threads,seed,fill,avg);

                    fflush(csv);
                }
            }
        }
    }
}

// ============================================================

int main() {
    // "ROW_Separate_Chunk_NoReInit.csv" +
    // "ROW_Together_Chunk_NoReInit.csv" 
    // "ROW_Together_Row_NoReInit.csv"
    FILE* csv = fopen("RowPure.csv", "w");

    if (!csv) {
        printf("Failed to open CSV.\n");
        return 1;
    }

    fprintf(
        csv,
        "Algorithm,"
        "Width,"
        "Height,"
        "Threads,"
        "Seed,"
        "FillPercent,"
        "Run,"
        "TimeMs\n"
    );

    printf("=====================================\n");
    printf("GAME OF LIFE BENCHMARK\n");
    printf("Press any key to stop.\n");
    printf("=====================================\n");

    // performBenchmark(
    //     AlgorithmType::Sequential,
    //     "Sequential",
    //     csv
    // );

    performBenchmark(
        AlgorithmType::OMP_Rows,
        "OMP_Rows",
        csv
    );

    // performBenchmark(
    //     AlgorithmType::OMP_Columns,
    //     "OMP_Columns",
    //     csv
    // );

    // performBenchmark(
    //     AlgorithmType::OMP_Blocks,
    //     "OMP_Blocks",
    //     csv
    // );

    fclose(csv);

    printf("\nBenchmark finished.\n");

    return 0;
}

// set OMP_PROC_BIND=true

// set OMP_PLACES=cores

// set OMP_DISPLAY_ENV=VERBOSE


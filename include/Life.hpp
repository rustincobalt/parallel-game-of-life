#include <cstdint>
#include <vector>
#include <cassert>
#include <random>

using std::vector;

class Life{
    private:

    protected:

      struct GridCorners {

        // Padding corners (destinations)
        int topLeftPad;
        int topRightPad;
        int bottomLeftPad;
        int bottomRightPad;

        // Real corners (sources)
        int topLeftReal;
        int topRightReal;
        int bottomLeftReal;
        int bottomRightReal;

        GridCorners() = default;
        GridCorners(int rows, int width);
      };

    static constexpr uint8_t STATUS_TABLE[9][2]{
    //IsAlive:   0  1            // Alive neighbours:
            {0, 0},     // 0 
            {0, 0},     // 1
            {0, 1},     // 2
            {1, 1},     // 3
            {0, 0},     // 4
            {0, 0},     // 5
            {0, 0},     // 6
            {0, 0},     // 7
            {0, 0}      // 8
};
    
    // --- FIELDS ---

    int rows;
    int width;
    GridCorners CORNERS;

    vector<uint8_t> currGrid;
    vector<uint8_t> nextGrid;

    const uint32_t DEAD_COLOR;
    const uint32_t ALIVE_COLOR;
    // uint32_t* PIXEL_LUT;
    vector<uint32_t> PIXEL_LUT;
    vector<uint32_t> pixels; // 0xAABBGGRR

    // ---  ---


    // --- CONSTRUCT ---

    Life(int h, int w, uint32_t deadColor, uint32_t aliveColor);
    Life(int h, int w);
    


    // --- CALC METHODS ---

    /// @brief [stRow; enRow]
    void updateRowRange(const vector<uint8_t>& currGrid, vector<uint8_t>& nextGrid,  int stRow, int enRow);
    
    /// @brief [stRow; enRow]
    void updateHorizontalPadding(vector<uint8_t>& grid, int stRow, int enRow);
    
    /// @brief [stCell; enCell]
    void updateVerticalPadding(vector<uint8_t>& grid, int stCell, int enCell);
    void updateCornersPadding(vector<uint8_t> &grid);

    /// @brief [stRow; enRow]
    void calcPixelsRowRange(int stRow, int enRow);

    public:

    // --- PUBLIC METHODS ---

    virtual ~Life() = default;
    virtual void updateGrid() = 0;
    virtual void calcPixels() = 0;
    const vector<uint32_t>& getPixels();
    void generateRandomGrid(int seed, int fillPercent);
};
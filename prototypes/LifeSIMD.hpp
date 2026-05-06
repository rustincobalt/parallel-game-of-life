#include <cstdint>
#include <vector>

using std::vector;


class LifeSIMD{

    struct rowPaddingWriteAtoB{
        // j
        int wordIndexA;
        int wordIndexB;

        uint64_t maskA;
        uint64_t maskB;
        int8_t shift;

        // rowPaddingWriteAtoB(int wordIdA, int wordIdB, uint64_t maskA, uint64_t maskB, int8_t shift);
        rowPaddingWriteAtoB(int wordIdA, int wordIdB, int wordAbyteId, int wordBbyteId);
    };

    struct cornerPaddingWriteAtoB{
        // i
        int rowIndexA;
        int rowIndexB;

        // j
        int wordIndexA;
        int wordIndexB;

        uint64_t maskA;
        uint64_t maskB;
        int8_t shift;

        cornerPaddingWriteAtoB(int rowIdA, int rowIdB, int wordIdA, int wordIdB, int wordAbyteId, int wordBbyteId);

    };

    protected:
    
    int h, w;
    float cellSize; // pixel
    
    int rowsNum;
    uint8_t rightPaddingByte;
    int wordsPerRow;
    
    int gridLength;

    vector<uint64_t> grid;
    
    vector<rowPaddingWriteAtoB> rowPaddingInstruct;
    vector<cornerPaddingWriteAtoB> cornerPaddingInstruct;
    

    void updateRowRange(const vector<uint64_t>& currGrid, vector<uint64_t>& nextGrid,  int stRow, int enRow);
    
    void updateHorizontalPadding(vector<uint64_t>& grid, int stRow, int enRow);
    void updateVerticalPadding(vector<uint64_t>& grid, int stWord, int enWord);
    void updateCornersPadding();

    LifeSIMD(int h, int w, float cellSize);

    private:
    static const uint16_t MASKS[16];

    void addMaskedAToMaskedB(uint64_t A, uint64_t& B, uint64_t maskA, uint64_t maskB, int8_t shift);
    void initPaddingData(int rowsNum, int wordsPerRow, int rightPaddingByte);

    public:

    virtual void nextGrid() = 0;
    virtual void drawGrid() = 0;
    
};
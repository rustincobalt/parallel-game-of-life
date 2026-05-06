#include "..\include\LifeSIMD.hpp"
#include "LifeSIMD.hpp"

// Masks for A (clear all except 1 byte)
const uint16_t LifeSIMD::MASKS[16]{
    0xF000'0000'0000'0000LL,
    0x0F00'0000'0000'0000LL,
    0x00F0'0000'0000'0000LL,
    0x000F'0000'0000'0000LL,
    0x0000'F000'0000'0000LL,
    0x0000'0F00'0000'0000LL,
    0x0000'00F0'0000'0000LL,
    0x0000'000F'0000'0000LL,
    0x0000'0000'F000'0000LL,
    0x0000'0000'0F00'0000LL,
    0x0000'0000'00F0'0000LL,
    0x0000'0000'000F'0000LL,
    0x0000'0000'0000'F000LL,
    0x0000'0000'0000'0F00LL,
    0x0000'0000'0000'00F0LL,
    0x0000'0000'0000'000FLL
};


// LifeSIMD::rowPaddingWriteAtoB::rowPaddingWriteAtoB(
//     int wordIdA, int wordIdB, 
//     uint64_t maskA, uint64_t maskB, 
//     int8_t shift): 
//             wordIndexA(wordIdA), wordIndexB(wordIdB), 
//             maskA(maskA), maskB(maskB), shift(shift)
//             {};

// LifeSIMD::cornerPaddingWriteAtoB::cornerPaddingWriteAtoB(
//     int rowIdA, int rowIdB, 
//     int wordIdA, int wordIdB, 
//     uint64_t maskA, uint64_t maskB, 
//     int8_t shift): 
//             rowIndexA(rowIdA), rowIndexB(rowIdB),
//             wordIndexA(wordIdA), wordIndexB(wordIdB), 
//             maskA(maskA), maskB(maskB), shift(shift)
//             {};



LifeSIMD::LifeSIMD(int h, int w, float cellSize): h(h), w(w), cellSize(cellSize) {
    
    // +2 for ghost padding
    rowsNum = h+2;
    rightPaddingByte = (w+2)%16;
    wordsPerRow = (rightPaddingByte > 0) ? (w+2)/16 : (w+2)/16 + 1;
    
    gridLength = rowsNum * wordsPerRow;
    
    grid.assign(gridLength, 0x0000000000000000LL);

    initPaddingData(rowsNum, wordsPerRow, rightPaddingByte);
}

  void LifeSIMD::updateRowRange(const vector<uint64_t>& currGrid, vector<uint64_t>& nextGrid,  int stRow, int enRow){
    
    // CAUTION: enRow not including
    for (int i = stRow*wordsPerRow; i < enRow*wordsPerRow; i+=wordsPerRow)
    {
        for (int j = 0; j < wordsPerRow; j++)
        {
            uint64_t prevWord = currGrid[i-wordsPerRow+j];
            uint64_t currWord = currGrid[i+j];
            uint64_t nextWord = currGrid[i+wordsPerRow+j];
            uint64_t count = 
                    (prevWord << 4) + prevWord + (prevWord >> 4) +  
                    (nextWord << 4) + nextWord + (nextWord >> 4) +
                    (currWord << 4) + (currWord >> 4);
            
            if (j > 0)
                count += (currGrid[i-wordsPerRow+j-1] + currGrid[i+j-1] + currGrid[i+wordsPerRow+j-1]) << 60;
            if (j+1 < wordsPerRow)
                count += (currGrid[i-wordsPerRow+j+1] + currGrid[i+j+1] + currGrid[i+wordsPerRow+j+1]) >> 60;

            uint64_t combinedScore = currWord | count;

            uint64_t isOddAndMoreThan2 = combinedScore & (combinedScore >> 1);
            uint64_t isLessThan4 = ~( (combinedScore >> 2) | (combinedScore >> 3));

            nextGrid[i+j] = isOddAndMoreThan2 & isLessThan4 & 0x1111111111111111LL;
        }
    }
    
}

void LifeSIMD::updateHorizontalPadding(vector<uint64_t> &grid, int stRow, int enRow)
{
    for (int i = stRow*wordsPerRow; i < enRow*wordsPerRow; i+=wordsPerRow)
    {   
        for (auto &&writingInstruct : rowPaddingInstruct)
        {   
            uint64_t  A = grid[i + writingInstruct.wordIndexA];
            uint64_t& B = grid[i + writingInstruct.wordIndexB];
            addMaskedAToMaskedB(A, B, writingInstruct.maskA, writingInstruct.maskB, writingInstruct.shift);
        }
    }
    
}

void LifeSIMD::updateVerticalPadding(vector<uint64_t> &grid, int stWord, int enWord)
{
    
}

void LifeSIMD::updateCornersPadding()
{
    for (auto &&writingInstruct : cornerPaddingInstruct)
    {   
        uint64_t  A = grid[writingInstruct.rowIndexA*wordsPerRow + writingInstruct.wordIndexA];
        uint64_t& B = grid[writingInstruct.rowIndexB*wordsPerRow + writingInstruct.wordIndexB];
        addMaskedAToMaskedB(A, B, writingInstruct.maskA, writingInstruct.maskB, writingInstruct.shift);
    }
}

void LifeSIMD::addMaskedAToMaskedB(uint64_t A, uint64_t& B, uint64_t maskA, uint64_t maskB, int8_t shift)
{
    A = A & maskA;
    B = B & maskB;
    B += (shift >= 0) ? A >> shift : A << shift*(-1); 
}

void LifeSIMD::initPaddingData(int rowsNum, int wordsPerRow, int rightPaddingByte)
{   
    int leftWordTake = 0;
    int leftWordPut = 0; 

    // always last
    int rightWordPut = wordsPerRow-1; 

    // if padding cell occupies first byte of the machine word, 
    // then we need to take byte from the previous machine word
    int rightWordTake = (rightPaddingByte == 1) ? wordsPerRow-2 : wordsPerRow-1; 

    int leftWordByteTake = 2;
    int leftWordBytePut = 1;


    int rightWordByteTake = rightPaddingByte;
    int rightWordBytePut = (rightPaddingByte == 1) ? 16 : rightPaddingByte-1;

    rowPaddingWriteAtoB LeftToRight = rowPaddingWriteAtoB(leftWordTake, rightWordPut, leftWordByteTake-1, rightWordBytePut-1);
    rowPaddingWriteAtoB RightToLeft = rowPaddingWriteAtoB(rightWordTake, leftWordPut, rightWordByteTake-1, leftWordBytePut-1);
    
    rowPaddingInstruct.push_back(LeftToRight);
    rowPaddingInstruct.push_back(RightToLeft);
    
    // TODO: delete comment below
    // Writing
    // 11 -> 22
    // 01 -> 10
    // 10 -> 01
    // 11 -> 00

    // Take
    int realCorner00Row = 1;
    int realCorner01Row = 1;
    int realCorner10Row = rowsNum-2;
    int realCorner11Row = rowsNum-2;


    int realCorner00Word = 0;
    int realCorner01Word = (rightPaddingByte == 1) ? wordsPerRow-2 : wordsPerRow-1;
    int realCorner10Word = 0;
    int realCorner11Word = (rightPaddingByte == 1) ? wordsPerRow-2 : wordsPerRow-1;


    int realCorner00Byte = 2;
    int realCorner01Byte = (rightPaddingByte == 1) ? 16 : rightPaddingByte-1;
    int realCorner10Byte = 2;
    int realCorner11Byte = (rightPaddingByte == 1) ? 16 : rightPaddingByte-1;

    // Put
    int paddCorner00Row = 0;
    int paddCorner01Row = 0;
    int paddCorner10Row = rowsNum-1;
    int paddCorner11Row = rowsNum-1;


    int paddCorner00Word = 0;
    int paddCorner01Word = wordsPerRow-1;
    int paddCorner10Word = 0;
    int paddCorner11Word = wordsPerRow-1;
    
    int paddCorner00Byte = 1;
    int paddCorner01Byte = rightPaddingByte;
    int paddCorner10Byte = 1;
    int paddCorner11Byte = rightPaddingByte;


    cornerPaddingWriteAtoB real_00_to_padd_11(
        realCorner00Row, paddCorner11Row, 
        realCorner00Word, paddCorner11Word,
        realCorner00Byte, paddCorner11Byte
    );
    cornerPaddingWriteAtoB real_01_to_padd_10(
        realCorner01Row, paddCorner10Row, 
        realCorner01Word, paddCorner10Word,
        realCorner01Byte, paddCorner10Byte
    );
    cornerPaddingWriteAtoB real_10_to_padd_01(
        realCorner10Row, paddCorner01Row, 
        realCorner10Word, paddCorner01Word,
        realCorner10Byte, paddCorner01Byte
    );
    cornerPaddingWriteAtoB real_11_to_padd_00(
        realCorner11Row, paddCorner00Row, 
        realCorner11Word, paddCorner00Word,
        realCorner11Byte, paddCorner00Byte
    );

    cornerPaddingInstruct.push_back(real_00_to_padd_11);
    cornerPaddingInstruct.push_back(real_01_to_padd_10);
    cornerPaddingInstruct.push_back(real_10_to_padd_01);
    cornerPaddingInstruct.push_back(real_11_to_padd_00);
}

LifeSIMD::rowPaddingWriteAtoB::rowPaddingWriteAtoB(int wordIdA, int wordIdB, int wordAbyteId, int wordBbyteId):
    wordIndexA(wordIdA), wordIndexB(wordIdB)
{
    maskA = MASKS[wordAbyteId];
    maskB = ~MASKS[wordBbyteId];        // always invert bytes
    shift = wordAbyteId - wordBbyteId;  // always A - B
}

LifeSIMD::cornerPaddingWriteAtoB::cornerPaddingWriteAtoB(int rowIdA, int rowIdB, int wordIdA, int wordIdB, int wordAbyteId, int wordBbyteId):
    rowIndexA(rowIdA), rowIndexB(rowIdB), 
    wordIndexA(wordIdA), wordIndexB(wordIdB)
{
    maskA = MASKS[wordAbyteId];
    maskB = ~MASKS[wordBbyteId];        // always invert bytes
    shift = wordAbyteId - wordBbyteId;  // always A - B
}

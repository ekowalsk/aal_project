#include <iostream>
#include "Raster.h"
#include "Table.h"
#define M 5  // wymiar tablicy
#define N 5   // wymiar tablicy
#define L 5 // wysokość najwyższego bloku

int main() {
    std::list<Block *> * blocks = createFromFile("block.txt", L);
    table myTable = table(M, N, L, blocks);
    //int leftVolume = myRaster.countVolume();
    //myRaster.print();
    //std::cout << "volume left: " << leftVolume << " empty fields: " << myRaster.getEmptyFields();
    myTable.setEmptyFields();
    std::pair<int, int> point (2,1);
    myTable.checkWaterLevel(point);
    myTable.print();


}
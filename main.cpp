#include <iostream>
#include "Raster.h"
#include "Table.h"
#define M 10 // wymiar tablicy
#define N 10 // wymiar tablicy
#define L 10 // wysokość najwyższego bloku

int main() {
    std::list<Block *> * blocks = createFromFile("block.txt", L);
    table myTable = table(M, N, L, blocks);
    //int leftVolume = myRaster.countVolume();
    //myRaster.print();
    //std::cout << "volume left: " << leftVolume << " empty fields: " << myRaster.getEmptyFields();
   myTable.setEmptyFields();
    //std::pair<int, int> point (3,6);
   // bool * setChecked;
    myTable.checkWater();
    myTable.print();
    //Raster myRaster = Raster ("block.txt", M, N, L);
    //std::cout << "volume left: " << myRaster.countVolume() << std::endl;
    //myRaster.print();


}
#include <iostream>
#include "Raster.h"
#include "Table.h"
#define M 5 // wymiar tablicy
#define N 5 // wymiar tablicy
#define L 5 // wysokość najwyższego bloku

int main() {
    srand(time(NULL));
    //std::list<Block *> * blocks = createFromFile("raster_table.txt", L);
    //table myTable = table(M, N, L, blocks);
    //int leftVolume = myRaster.countVolume();
    //myRaster.print();
    //std::cout << "volume left: " << leftVolume << " empty fields: " << myRaster.getEmptyFields();
   //myTable.setEmptyFields();
    //std::pair<int, int> point (3,6);
   // bool * setChecked;
    //myTable.checkWaterAround2(point);
  //myTable.checkWater2();
    //myTable.print();
    //myTable.printactivePoints();

    table myTable = table(M,N,L);
    myTable.generateRaster(67);
    myTable.writeToFile("raster.txt");
    myTable.writeToFile2("raster_table.txt");
    myTable.setEmptyFields();
    myTable.checkWater2();
    myTable.print();
    std::cout << "pozostala objetosc wody: " << myTable.getLeftWaterVolume() << std::endl;

    Raster myRaster = Raster ("raster_table.txt", M, N, L);
    std::cout << "volume left: " << myRaster.countVolume() << std::endl;
    myRaster.print();



}
#include <iostream>
#include "Raster.h"
#include "Table.h"
#include "Interface.h"

#define M 100 // wymiar tablicy
#define N 100 // wymiar tablicy
#define L 60 // wysokość najwyższego bloku

int main(int argc, char * argv[]) {
    Interface inOut = Interface();
    inOut.checkParameters(argc, argv);
    if (inOut.mode == "-m1"){
        table myTable = table(inOut.width, inOut.length, inOut.height);
        Raster myRaster = Raster(inOut.width, inOut.length, inOut.height);
        readFromFile(myTable, myRaster);
        myTable.setEmptyFields();
        myTable.checkWater();
        myTable.print();
        myRaster.print();
        std::cout << "raster volume left: " << myRaster.countVolume() << std::endl;
        std::cout << "table volume left: " << myTable.getLeftWaterVolume() << std::endl;
    }
    if (inOut.mode == "-m2"){
        srand(time(NULL));
        table myTable = table(inOut.width, inOut.length, inOut.height);
        Raster myRaster = Raster(inOut.width, inOut.length, inOut.height);
        generateRaster(90, myTable, myRaster);
        myTable.setEmptyFields();
        myTable.checkWater();
        myTable.print();
        myRaster.print();
        std::cout << "raster volume left: " << myRaster.countVolume() << std::endl;
        std::cout << "table volume left: " << myTable.getLeftWaterVolume() << std::endl;
    }
}
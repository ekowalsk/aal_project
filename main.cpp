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
    if (inOut.mode == "-m2"){
        srand(time(NULL));
        table myTable = table(inOut.width, inOut.length, inOut.height);
        myTable.generateRaster(inOut.density);
        myTable.writeToFile2("raster_table.txt");
        myTable.writeToFile("raster.txt");
        myTable.setEmptyFields();
        myTable.checkWater();
        myTable.print();

        Raster myRaster = Raster("raster_table.txt", inOut.width, inOut.length, inOut.height);
        myRaster.print();
        std::cout << "raster volume left: " << myRaster.countVolume() << std::endl;
        std::cout << "table volume left: " << myTable.getLeftWaterVolume() << std::endl;
    }

}
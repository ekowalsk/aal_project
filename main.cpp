#include <iostream>
#include "Cuboid.h"
#include "Table.h"
#include "Parameters.h"
#include "Test.h"

int main(int argc, char * argv[]) {
    Parameters inOut = Parameters();
    inOut.checkParameters(argc, argv);
    if (inOut.mode == "-m1"){
        Table myTable = Table(inOut.width, inOut.length, inOut.height);
        Cuboid myRaster = Cuboid(inOut.width, inOut.length, inOut.height);
        readFromFile(myTable, myRaster);
        std::cout << "raster volume left: " << myRaster.countVolume() << std::endl;
        std::cout << "Table volume left: " << myTable.countVolume() << std::endl;
        myRaster.print();
        myTable.print();

    }
    if (inOut.mode == "-m2"){
        srand(time(NULL));
        Table myTable = Table(inOut.width, inOut.length, inOut.height);
        Cuboid myRaster = Cuboid(inOut.width, inOut.length, inOut.height);
        generateRaster(inOut.density, myTable, myRaster);
        std::cout << "raster volume left: " << myRaster.countVolume() << std::endl;
        std::cout << "Table volume left: " << myTable.countVolume() << std::endl;
        myTable.print();
        myRaster.print();
    }

    if (inOut.mode == "-m3"){
        srand(time(NULL));
        makeTests(inOut);
    }
}
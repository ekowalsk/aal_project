//
// Created by ewa on 07.01.18.
//
#include <iostream>
#include <string>
#include "Interface.h"

Interface::Interface() {
    mode = " ";
    width = 0;
    length = 0;
    height = 0;
    density = 0;
}

bool Interface::checkParameters(int argc, char *argv[]) {
    if (argc == 5) {
        std::string strMode ("-m1");
        if (strMode.compare(argv[1]) != 0)
            return false;

        mode = strMode;
        width = std::stoi(argv[2], nullptr, 10);
        length = std::stoi(argv[3], nullptr, 10);
        height = std::stoi(argv[4], nullptr, 10);
        return true;
    }
    if (argc == 6) {
        std::string strMode ("-m2");
        if (strMode.compare(argv[1]) != 0)
            return false;

        mode = strMode;
        width = std::stoi(argv[2], nullptr, 10);
        length = std::stoi(argv[3], nullptr, 10);
        height = std::stoi(argv[4], nullptr, 10);
        density = std::stoi(argv[5], nullptr, 10);
        return true;
    }
    if (argc == 2) {
        std::string strMode ("-m1");
        if (strMode.compare(argv[1]) != 0)
            return false;

        mode = strMode;
        return true;
    } else {
        std::cerr << "Usage: " << std::endl;
        std::cerr << "-m1 - read data from file data.txt and write solution to solution.txt" << std::endl;
        std::cerr << argv[0] << " -m1 << data.txt >> solution.txt" << std::endl;
        std::cerr << "-m2 - generate data and write solution to file, m - width, n - length,"
                " h - maximum height, d - density percent" << std::endl;
        std::cerr << "example " << argv[0] << " -m2 100 120 60 80 >> solution.txt " << std::endl;
        std::cerr << "-m3 - test" << std::endl;
        std::cerr << argv[0] << "-m3" << std::endl;

        return false;
    }
}

/*void generateRaster(float percent, table myTable, Raster myRaster){
    int pos, x, y;
    int blocksAmount = int((percent * myTable.ge * N) / 100);
    for (int i = 0; i < blocksAmount; i++) {
        pos = rand() % (M * N);
        x = pos % M;
        y = pos / M;
        while (raster[x][y].getType() != Field::waterfield) {
            pos = (pos + 3) % (M * N);
            x = pos % M;
            y = pos / M;
        }
        int height = rand() % maxHeight + 1;
        raster[x][y].setType(cube::blockfield);
        raster[x][y].setHeight(height);
        Block *newBlock = new Block(x, y, height);
        blockslist[height].push_back(newBlock);
    }
}*/

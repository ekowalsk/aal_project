//
// Created by ewa on 08.01.18.
//

#include <iostream>
#include <cstddef>
#include <chrono>
#include <cmath>
#include <map>
#include "Test.h"

/**
 * generates structures of Table and Cuboid for the same data
 * @param percent percent of blocks according to the surface of raster
 * @param myTable
 * @param myRaster
 */
void generateRaster(float percent, Table myTable, Cuboid myCuboid) {
    int pos, x, y;
    int M = myTable.getWidth();
    int N = myTable.getLength();
    int maxHeight = myTable.getHeight();
    int blocksAmount = int((percent * M * N) / 100);
    for (int i = 0; i < blocksAmount; i++) {
        pos = rand() % (M * N);
        x = pos % M;
        y = pos / M;
        while (myTable.getField(x, y)->getType() != Field::water) {
            pos = (pos + 3) % (M * N);
            x = pos % M;
            y = pos / M;
        }
        int height = rand() % maxHeight + 1;
        myTable.getField(x, y)->setType(Field::block);
        myTable.getField(x, y)->setHeight(height);
        Block *newBlock = new Block(x, y, height);
        myTable.getBlocks()[height].push_back(newBlock);

        for (int z = 0; z < height; z++) {
            myCuboid.getRaster()[x][y][z].setType(cube::block);
            myCuboid.getBlocksPerLevel()[z]++;
        }
    }
}

/**
 * reads from standard input data and initializes table and cuboid
 * @param myTable
 * @param myRaster
 */
void readFromFile(Table myTable, Cuboid myCuboid) {
    int x, y, height;
    while (!std::cin.eof()) {
        std::cin >> x;
        // EOF mark just after the last character
        if (std::cin.eof())
            break;
        std::cin >> y;
        std::cin >> height;
        auto *newBlock = new Block(x, y, height);
        myTable.getBlocks()[height].push_back(newBlock);
        myTable.getField(x, y)->setType(cube::block);
        myTable.getField(x, y)->setHeight(height);

        for (int z = 0; z < height; z++) {
            myCuboid.getRaster()[x][y][z].setType(cube::block);
            myCuboid.getBlocksPerLevel()[z]++;
        }
    }
}

/**
 * makes test for all instances of one problem
 * @param parameters
 * @param tableDuration returns measured duration of counting volume for table
 * @param cuboidDuration returns measured duration of counting volume for cuboid
 * @param width width of table and cuboid
 * @param length length of table and cuboid
 * @param height height of maximum block of table and cuboid
 * @return number of blocks
 */
int makeOneTest(Parameters parameters, double * tableDuration,
                double * cuboidDuration, int width, int length, int height) {
    std::chrono::time_point<std::chrono::system_clock> start, stop;
    std::chrono::duration<double> duration;
    *tableDuration = 0;
    *cuboidDuration = 0;
    int nBlocks = (width * length * parameters.density)/100;
    for (int i = 0; i < parameters.nInstances; i++) {
        Table myTable = Table(width, length, height);
        Cuboid myRaster = Cuboid(width, length, height);
        generateRaster(parameters.density, myTable, myRaster);
        start = std::chrono::system_clock::now();
        int rasterVolume = myRaster.countVolume();
        stop = std::chrono::system_clock::now();
        duration = stop - start;
        *cuboidDuration += duration.count();
        start = std::chrono::system_clock::now();
        int tableVolume = myTable.countVolume();
        stop = std::chrono::system_clock::now();
        duration = stop - start;
        *tableDuration += duration.count();
        myTable.print();
        myRaster.print();
        std::cout << "Cuboid volume left: " << rasterVolume << std::endl;
        std::cout << "Table volume left: " << tableVolume << std::endl;
    }
    *tableDuration = *tableDuration / parameters.nInstances;
    *cuboidDuration = *cuboidDuration / parameters.nInstances;
    return nBlocks;
}

void makeTests(Parameters parameters) {
    double rasterDuration = 0;
    double tableDuration = 0;
    std::multimap<double, double> rasterTimes;
    std::multimap<double, double> tableTimes;
    int width = parameters.width;
    int length = parameters.length;
    int height = parameters.height;
    int step = parameters.step;
    int nBlocks = 0;

    for (int nProblems = 0; nProblems != parameters.nProblems; nProblems++) {
        nBlocks = makeOneTest(parameters, &tableDuration, &rasterDuration, width, length, height);
        double rasterTime = 4* width * length * height;
        rasterTimes.insert(std::make_pair(rasterTime, rasterDuration));
        double tableTime = 4 * length * width + nBlocks*nBlocks;
        tableTimes.insert(std::make_pair(tableTime, tableDuration));
        width += step;
        length += step;
        height += step;
    }

    std::cout << "Table results: " << std::endl;
    printResults(tableTimes, parameters.width, parameters.length, parameters.height, parameters.step);
    std::cout << "Cuboid results: " << std::endl;
    printResults(rasterTimes, parameters.width, parameters.length, parameters.height, parameters.step);
}

void printResults(std::multimap<double, double> durations, int width, int lenght, int height, int step) {
    auto iterator = durations.begin();
    int w = width;
    int l = lenght;
    int h = height;
    for (int i = 0; i < durations.size() / 2; i++) {
        iterator++;
    }
    double mediant = iterator->second;
    double medianT = iterator->first;

    for (auto &duration : durations) {
        double actualTime = (duration.second * medianT)/(duration.first*mediant);
        std::cout << "width: " << w << " lenght: " << l << " height: " << h  <<
                  " t(n) " << duration.second << " q(n) " << actualTime << std::endl;
        w += step;
        l += step;
        h += step;
    }
}
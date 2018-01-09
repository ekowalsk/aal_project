//
// Created by ewa on 08.01.18.
//

#include <iostream>
#include <cstddef>
#include <chrono>
#include <cmath>
#include <map>
#include "Test.h"

void generateRaster(float percent, Table myTable, Cuboid myRaster) {
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
            myRaster.getRaster()[x][y][z].setType(cube::block);
            myRaster.getBlocksPerLevel()[z]++;
        }
    }
}

void readFromFile(Table myTable, Cuboid myRaster) {
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
            myRaster.getRaster()[x][y][z].setType(cube::block);
            myRaster.getBlocksPerLevel()[z]++;
        }
    }
}

int makeOneTest(Parameters parameters, double * tableDuration,
                double * rasterDuration, int width, int length, int height) {
    std::chrono::time_point<std::chrono::system_clock> start, stop;
    std::chrono::duration<double> duration;
    *tableDuration = 0;
    *rasterDuration = 0;
    int nBlocks = (width * length * parameters.density)/100;
    for (int i = 0; i < parameters.nInstances; i++) {
        Table myTable = Table(width, length, height);
        Cuboid myRaster = Cuboid(width, length, height);
        generateRaster(parameters.density, myTable, myRaster);
        start = std::chrono::system_clock::now();
        int rasterVolume = myRaster.countVolume();
        stop = std::chrono::system_clock::now();
        duration = stop - start;
        *rasterDuration += duration.count();
        start = std::chrono::system_clock::now();
        int tableVolume = myTable.countVolume();
        stop = std::chrono::system_clock::now();
        duration = stop - start;
        *tableDuration += duration.count();
        myTable.print();
        myRaster.print();
        std::cout << "raster volume left: " << rasterVolume << std::endl;
        std::cout << "Table volume left: " << tableVolume << std::endl;
    }
    *tableDuration = *tableDuration / parameters.nInstances;
    *rasterDuration = *rasterDuration / parameters.nInstances;
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
        double tableTime = 4 * length * width + 0.5*nBlocks*nBlocks;
        tableTimes.insert(std::make_pair(tableTime, tableDuration));
        width += step;
        length += step;
        height += step;
    }

    std::cout << "Table results: " << std::endl;
    printResults(tableTimes);
    std::cout << "raster results: " << std::endl;
    printResults(rasterTimes);
}

void printResults(std::multimap<double, double> durations) {
    auto iterator = durations.begin();
    for (int i = 0; i < durations.size() / 2; i++) {
        iterator++;
    }
    double mediant = iterator->second;
    double medianT = iterator->first;
    for (auto &duration : durations) {
        double actualTime = (duration.second * medianT)/(duration.first*mediant);
        std::cout << actualTime << std::endl;
    }
}
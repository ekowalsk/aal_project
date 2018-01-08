//
// Created by ewa on 08.01.18.
//

#include <iostream>
#include <cstddef>
#include <chrono>
#include <cmath>
#include <map>
#include "Test.h"

void generateRaster(float percent, table myTable, Raster myRaster) {
    int pos, x, y;
    int M = myTable.getWidth();
    int N = myTable.getLength();
    int maxHeight = myTable.getHeight();
    int blocksAmount = int((percent * M * N) / 100);
    for (int i = 0; i < blocksAmount; i++) {
        pos = rand() % (M * N);
        x = pos % M;
        y = pos / M;
        while (myTable.getField(x, y)->getType() != Field::waterfield) {
            pos = (pos + 3) % (M * N);
            x = pos % M;
            y = pos / M;
        }
        int height = rand() % maxHeight + 1;
        myTable.getField(x, y)->setType(cube::blockfield);
        myTable.getField(x, y)->setHeight(height);
        Block *newBlock = new Block(x, y, height);
        myTable.getBlocks()[height].push_back(newBlock);

        for (int z = 0; z < height; z++) {
            myRaster.getRaster()[x][y][z].setType(cube::blockfield);
            myRaster.getBlocksPerLevel()[z]++;
        }
    }
}

void readFromFile(table myTable, Raster myRaster) {
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
        myTable.getField(x, y)->setType(cube::blockfield);
        myTable.getField(x, y)->setHeight(height);

        for (int z = 0; z < height; z++) {
            myRaster.getRaster()[x][y][z].setType(cube::blockfield);
            myRaster.getBlocksPerLevel()[z]++;
        }
    }
}

int makeOneTest(Parameters parameters, double * tableDuration,
                double * rasterDuration) {
    std::chrono::time_point<std::chrono::system_clock> start, stop;
    std::chrono::duration<double> duration;
    *tableDuration = 0;
    *rasterDuration = 0;
    int nBlocks = 0;
    for (int i = 0; i < parameters.nInstances; i++) {
        table myTable = table(parameters.width, parameters.length, parameters.height);
        Raster myRaster = Raster(parameters.width, parameters.length, parameters.height);
        generateRaster(parameters.density, myTable, myRaster);
        nBlocks = myTable.getBlocks()->size();
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
        std::cout << "table volume left: " << tableVolume << std::endl;
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
    int nBlocks = 0;

    for (int nProblems = 0; nProblems != parameters.nProblems; nProblems++) {
        nBlocks = makeOneTest(parameters, &tableDuration, &rasterDuration);
        double rasterTime = width * length * height;
        rasterTimes.insert(std::make_pair(rasterTime, rasterDuration));
        double tableTime = 4 * length * width + 1.5 * sqrt(nBlocks);
        tableTimes.insert(std::make_pair(tableTime, tableDuration));
    }

    std::cout << "table results: " << std::endl;
    printResults(tableTimes);
    std::cout << "raster results: " << std::endl;
    printResults(rasterTimes);
}

void printResults(std::multimap<double, double> durations) {
    double median;
    auto iterator = durations.begin();
    for (int i = 0; i < durations.size() / 2; i++) {
        iterator++;
    }
    median = iterator->second;
    for (auto &duration : durations) {
        double actualTime = (duration.second)/median;
        std::cout << actualTime << std::endl;
    }
}
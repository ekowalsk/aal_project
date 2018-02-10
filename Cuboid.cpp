//
// Created by ewa on 29.12.17.
//

#include <fstream>
#include <iostream>
#include "Cuboid.h"
/**
 * creates empty cuboid
 * @param width width of cuboid
 * @param length length of cuboid
 * @param height height of cuboid
 */
Cuboid::Cuboid(int width, int length, int height){
    this->width = width;
    this->length = length;
    this->maxHeight = height;
    emptyFields = 0;

    raster = new cube **[width];
    for (int i = 0; i < width; i++) {
        raster[i] = new cube *[length];

    }
    for (int x = 0; x < width; x++)
        for (int y = 0; y < length; y++)
            raster[x][y] = new cube[maxHeight];

    blocksPerLevel = new int[maxHeight];
    for (int i = 0; i < maxHeight; i++)
        blocksPerLevel[i] = 0;
}
/**
 * creates cuboid and initializes it with data from file
 * @param fname name of file
 * @param width width of cuboid
 * @param length length of cuboid
 * @param height height of cuboid
 */
Cuboid::Cuboid(std::string fname, int width, int length, int height){
    this->width = width;
    this->length = length;
    this->maxHeight = height;
    emptyFields = 0;

    raster = new cube **[width];
    for (int i = 0; i < width; i++) {
        raster[i] = new cube *[length];
    }
    for (int x = 0; x < width; x++)
        for (int y = 0; y < length; y++)
            raster[x][y] = new cube[maxHeight];

    blocksPerLevel = new int[maxHeight];
    for (int i = 0; i < maxHeight; i++)
        blocksPerLevel[i] = 0;

    createFromFile(fname);
};
/**
 * initializes structures with data from file
 * @param fname name of file
 */
void Cuboid::createFromFile(std::string fname) {
    std::fstream file;
    file.open(fname, std::ios::in);
    if (file.good()) {
        int x, y;
        float height;
        while (!file.eof()) {
            file >> x;
            // EOF mark just after the last character
            if (file.eof()) break;
            file >> y;
            file >> height;
            for (int z = 0; z < height; z++) {
                raster[x][y][z].setType(cube::block);
                blocksPerLevel[z]++;
            }
        }
        file.close();
    } else {
        std::cout << "can't open file" << std::endl;
    }
};

/**
 * seeks for fields on each level of cuboid that are on the edge of raster and are not blocks, to specify from which fields the water will escape
 * @return list of coordinates of fields
 * @param z level of cuboid
 *
 */
std::list<std::pair<int, int>> Cuboid::findEmptyField(int z) {
    std::list<std::pair<int, int>> emptyFields;
    int x, y;
    for (x = 0, y = 0; x < width; x++) {
        if (raster[x][y][z].getType() == cube::water) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }
// y = 1 aby uniknąć redundancji przy dodawaniu narożników
    for (y = 1, x = width - 1; y < length; y++) {
        if (raster[x][y][z].getType() == cube::water) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (x = 0, y = length - 1; x < width - 1; x++) {
        if (raster[x][y][z].getType() == cube::water) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (y = 1, x = 0; y < length - 1; y++) {
        if (raster[x][y][z].getType() == cube::water) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }


    for (auto &it : emptyFields) {
        raster[it.first][it.second][z].setType(cube::empty);
        this->emptyFields++;
    }
    return emptyFields;
};

/**
 * checks neighbours of given field to set them empty if they're not blocks
 * @param x coordinate of field
 * @param y coordinate of field
 * @param z coordinate of field (level)
 */
void Cuboid::setNeighboursEmpty(int x, int y, int z) {
    std::list<std::pair<int, int>> neighbours = getNeighbours(x, y);
    for (auto &it : neighbours) {
        int x = it.first;
        int y = it.second;
        if (raster[x][y][z].getType() == cube::water) {
            raster[x][y][z].setType(cube::empty);
            activePoints.emplace_back(std::pair<int, int>(x, y));
            emptyFields++;
        }
    }
};
/**
 * uses setNeighboursEmpty for all points on activePoints list
 * @param z level
 */
void Cuboid::setEmptyFields(int z) {
    activePoints = findEmptyField(z);
    std::pair<int, int> activePoint;
    while (!activePoints.empty()) {
        activePoint = activePoints.front();
        setNeighboursEmpty(activePoint.first, activePoint.second, z);
        activePoints.pop_front();
    }
};

/**
 * finds all of the neighbours of given point
 * @param x x coordinate
 * @param y y coordinate
 * @return list of coordinates of neighbours
 */
std::list<std::pair<int, int>> Cuboid::getNeighbours(int x, int y) {
    std::list<std::pair<int, int>> neighbours;
    int lx = x - 1;
    int rx = x + 1;
    int dy = y - 1;
    int uy = y + 1;

    if (lx >= 0)
        neighbours.emplace_back(std::pair<int, int>(lx, y));
    if (rx < width)
        neighbours.emplace_back(std::pair<int, int>(rx, y));
    if (dy >= 0)
        neighbours.emplace_back(std::pair<int, int>(x, dy));
    if (uy < length)
        neighbours.emplace_back(std::pair<int, int>(x, uy));
    return neighbours;
};

/**
 * prints cuboid, types: 0 - empty, 1 - water, 2 - block
 */
void Cuboid::print() {
    for (int z = maxHeight - 1; z >= 0; z--) {
        for (int y = 0; y < length; y++) {
            for (int x = 0; x < width; x++) {
                std::cout << raster[x][y][z].getType();
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};
/**
 * gathers all functions necessary to count volume left
 * @return volume
 */
int Cuboid::countVolume() {
    for (int z = 0; z < maxHeight; z++) {
        setEmptyFields(z);
    }
    int blocksVolume = 0;
    for (int i = 0; i < maxHeight; i++) {
        blocksVolume += blocksPerLevel[i];
    }
    return width * length * maxHeight - emptyFields - blocksVolume;
};

int Cuboid::getEmptyFields() {
    return emptyFields;
};

cube *** Cuboid::getRaster(){
    return raster;
}
int * Cuboid::getBlocksPerLevel(){
    return blocksPerLevel;
}



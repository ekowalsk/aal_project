//
// Created by ewa on 27.12.17.
//

#include <iostream>
#include <fstream>
#include "Table.h"
/**
 * creates empty raster with array of lists, indexes of the array corresponds to heights of blocks
 * @param width width of table
 * @param length length of table
 * @param maxHeight height of the highest block
 */
Table::Table(int width, int length, int maxHeight) {
    this->width = width;
    this->length = length;
    this->maxHeight = maxHeight;
    emptyFields = 0;
    leftWaterVolume = 0;

    raster = new Field *[width];
    for (int i = 0; i < width; i++) {
        raster[i] = new Field[length];
    }

    blockslist = new std::list<Block *>[maxHeight + 1];
}
/**
 * creates table and sets correspondent fields with parameters of blocks from the list
 * @param width width of table
 * @param length length of table
 * @param maxHeight height of the highest block
 * @param blockList list of blocks
 */

Table::Table(int width, int length, int maxHeight, std::list<Block *> *blockList){
    this->width = width;
    this->length = length;
    this->maxHeight = maxHeight;
    emptyFields = 0;
    leftWaterVolume = 0;

    raster = new Field *[width];
    for (int i = 0; i < width; i++) {
        raster[i] = new Field[length];
    }

    blockslist = new std::list<Block *>[maxHeight + 1];
    for (int i = 1; i <= maxHeight; i++) {
        blockslist[i] = blockList[i];
    }

    int x, y;
    for (int i = 1; i <= maxHeight; i++) {
        for (auto &it : blockslist[i]) {
            x = it->getX();
            y = it->getY();
            raster[x][y].setType(Field::block);
            raster[x][y].setHeight(it->getHeight());
        }
    }
};

/**
 * writes to a file coordinates and heights of blocks in a format that can be used to create Cuboid
 * @param fname name od file
 */
void Table::writeToFile2(std::string fname) {
    std::fstream file;
    file.open(fname, std::ios::out | std::ios::trunc);
    if (file.good()) {
        for (int y = 0; y < length; y++) {
            for (int x = 0; x < width; x++) {
                int height = raster[x][y].getHeight();
                if (height != 0)
                    file << x << " " << y << " " << height << std::endl;
            }
        }
        file.close();
    } else {
        std::cout << "can't open file" << std::endl;
    }
}

/**
 * prints table, h represents height of field, t represents type: 0 - empty, 1 - water, 2 - block
 */
void Table::print() {
    for (int y = 0; y < length; y++) {
        for (int x = 0; x < width; x++) {
            std::string height = std::to_string(raster[x][y].getHeight());
            if (height.length() == 1)
                height += "  ";
            if (height.length() == 2)
                height += " ";

            std::cout << "h: " << height << " t: " << raster[x][y].getType() << " | ";
        }
        std::cout << std::endl;
    }
}
/**
 * seeks for fields that are on the edge of raster and are not blocks, to specify from which fields the water will escape
 * @return list of coordinates of fields
 */
std::list<std::pair<int, int>> Table::findEmptyField() {
    std::list<std::pair<int, int>> emptyFields;
    int x, y;
    for (x = 0, y = 0; x < width; x++) {
        if (raster[x][y].getType() == Field::water) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }
// y = 1 aby uniknąć redundancji przy dodawaniu narożników
    for (y = 1, x = width - 1; y < length; y++) {
        if (raster[x][y].getType() == Field::water) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (x = 0, y = length - 1; x < width - 1; x++) {
        if (raster[x][y].getType() == Field::water) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (y = 1, x = 0; y < length - 1; y++) {
        if (raster[x][y].getType() == Field::water) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (auto &it : emptyFields) {
        raster[it.first][it.second].setType(cube::empty);
        raster[it.first][it.second].setChecked(true);
        this->emptyFields++;
    }
    return emptyFields;
};

/**
 * checks neighbours of given field to set them empty if they're not blocks
 * @param x x coordinate of field
 * @param y y coordinate od field
 */
void Table::setNeighboursEmpty(int x, int y) {
    std::list<std::pair<int, int>> neighbours = getNeighbours(x, y);
    for (auto &it : neighbours) {
        int x = it.first;
        int y = it.second;
        if (raster[x][y].getType() == Field::water) {
            raster[x][y].setType(Field::empty);
            raster[x][y].setChecked(true);
            activePoints.emplace_back(std::pair<int, int>(x, y));
            emptyFields++;
        }
    }
}

/**
 * uses setNeighboursEmpty for all points on activePoints list
 */
void Table::setEmptyFields() {
    activePoints = findEmptyField();
    std::pair<int, int> activePoint;
    while (!Table::activePoints.empty()) {
        activePoint = activePoints.front();
        setNeighboursEmpty(activePoint.first, activePoint.second);
        Table::activePoints.pop_front();
    }
}

/**
 * finds all of the neighbours of given point
 * @param x x coordinate
 * @param y y coordinate
 * @return list of coordinates of neighbours
 */
std::list<std::pair<int, int>> Table::getNeighbours(int x, int y) {
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
}

/**
 * finds the smallest block that has at the same time checked field and unchecked field as neighbours
 * @return list constructed from returned block
 */
std::list<std::pair<int, int>> Table::getBlock() {
    int x, y;
    bool hasCheckedField = false, hasUncheckedField = false;
    std::list<std::pair<int, int>> blocks;
    std::list<std::pair<int, int>> neighbours;
    for (int i = 1; i <= maxHeight; i++) {
        for (auto &it : blockslist[i]) {
            x = it->getX();
            y = it->getY();
            if (!raster[x][y].isChecked()) {
                if ( x == 0 || x == width - 1 || y == 0 || y == length - 1)
                    hasCheckedField = true;
                neighbours = getNeighbours(x, y);
                for (auto &iter : neighbours) {
                    int nX = iter.first;
                    int nY = iter.second;
                    if (raster[nX][nY].isChecked())
                        hasCheckedField = true;

                    if (!raster[nX][nY].isChecked())
                        hasUncheckedField = true;

                    if (hasCheckedField && hasUncheckedField) {
                        blocks.emplace_back(std::make_pair(x, y));
                        return blocks;
                    }
                }
            }
            hasCheckedField = false;
            hasUncheckedField = false;
        }
    }

    return blocks;
};

/**
 * defines the rules of checking neighbours, depending of the height and type of field
 * @param x
 * @param y
 */
void Table::setNeighboursChecked(int x, int y) {
    std::list<std::pair<int, int>> neighbours = getNeighbours(x, y);
    int height = raster[x][y].getHeight();
    bool hasEmptyNeigh = false;
    for (auto &it : neighbours) {
        int nX = it.first;
        int nY = it.second;
        if (!raster[nX][nY].isChecked()) {
            if (raster[nX][nY].getType() == Field::water) {
                raster[nX][nY].setChecked(true);
                raster[nX][nY].setHeight(height);
                leftWaterVolume += height;
                activePoints.emplace_back(std::pair<int, int>(nX, nY));
            }
            if (raster[nX][nY].getType() == Field::block && raster[nX][nY].getHeight() == height) {
                activePoints.emplace_back(std::make_pair(nX, nY));
                raster[nX][nY].setChecked(true);
            }
            if (raster[nX][nY].getType() == Field::block && raster[nX][nY].getHeight() < height) {
                if (!(nX == 0 || nX == width - 1 || nY == 0 || nY == length - 1)) {
                    std::list<std::pair<int, int>> neigh = getNeighbours(nX, nY);
                    // może ustawiać na water bloki, za którymi nic nie ma
                    for (auto &it : neigh) {
                        if (raster[it.first][it.second].getType() == Field::empty)
                            hasEmptyNeigh = true;
                    }
                    if (!hasEmptyNeigh) {
                        leftWaterVolume += height - raster[nX][nY].getHeight();
                        raster[nX][nY].setChecked(true);
                        raster[nX][nY].setHeight(height);
                        raster[nX][nY].setType(Field::water);
                        activePoints.emplace_back(std::pair<int, int>(nX, nY));
                    }
                }
            }
        }
    }
}

/**
 * simulates filling with water surface at the level of given block
 * @param block blocks which height states the level of water
 */
void Table::checkWaterAround(std::pair<int, int> block) {
    activePoints.push_back(block);
    std::pair<int, int> activePoint;

    while (!activePoints.empty()) {
        activePoint = activePoints.front();
        setNeighboursChecked(activePoint.first, activePoint.second);
        activePoints.pop_front();
    }
}
/**
 * simulates filling with water for all the blocks returned from getBlock() function
 */
void Table::checkWater() {
    std::list<std::pair<int, int>> block = getBlock();
    while (!block.empty()) {
        checkWaterAround(block.front());
        int x = block.front().first;
        int y = block.front().second;
        raster[x][y].setChecked(true);
        block = getBlock();
    }
}

/**
 * gathers all of procedures necessary to count amount of water left on the raster
 * @return left volume of water
 */
int Table::countVolume() {
    setEmptyFields();
    checkWater();
    return leftWaterVolume;
}

int Table::getWidth (){
    return width;
}
int Table::getLength(){
    return length;
}
int Table::getHeight(){
    return maxHeight;
}
Field * Table::getField(int x, int y){
    return &raster[x][y];
}
std::list<Block *> * Table::getBlocks(){
    return blockslist;
}

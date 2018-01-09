//
// Created by ewa on 27.12.17.
//

#include <iostream>
#include <fstream>
#include "Table.h"

Table::Table(int m, int n, int maxHeight) : width(m), length(n) {
    this->maxHeight = maxHeight;
    emptyFields = 0;
    leftWaterVolume = 0;

    raster = new Field *[width];
    for (int i = 0; i < width; i++) {
        raster[i] = new Field[length];
    }

    blockslist = new std::list<Block *>[maxHeight + 1];
}

Table::Table(int m, int n, int maxHeight, std::list<Block *> *blockList) : width(m), length(n) {
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
Table::~Table(){
    if (blockslist->size() != 0)
        delete[]blockslist;
}

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

void Table::writeToFile(std::string fname) {
    std::fstream file;
    file.open(fname, std::ios::out | std::ios::trunc);
    if (file.good()) {
        for (int y = 0; y < length; y++) {
            for (int x = 0; x < width; x++) {
                int height = raster[x][y].getHeight();
                if (height != 0)
                    file << height << "|";
                else
                    file << " " << "|";
            }
            file << std::endl;
        }
        file.close();
    } else {
        std::cout << "can't open file" << std::endl;
    }
}

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

// used while setting fields with no water
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

void Table::setEmptyFields() {
    activePoints = findEmptyField();
    std::pair<int, int> activePoint;
    while (!Table::activePoints.empty()) {
        activePoint = activePoints.front();
        setNeighboursEmpty(activePoint.first, activePoint.second);
        Table::activePoints.pop_front();
    }
}

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

std::list<std::pair<int, int>> Table::getBlock() {
    int x, y;
    bool hasCheckedField = false, hasUncheckedField = false;
    std::list<std::pair<int, int>> blocks;
    std::list<std::pair<int, int>> neighbours;
    for (int i = 1; i <= maxHeight; i++) {
        for (auto &it : blockslist[i]) {
            x = it->getX();
            y = it->getY();
            if ( x == 0 || x == width - 1 || y == 0 || y == length - 1)
                hasCheckedField = true;
            if (!raster[x][y].isChecked()) {
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
                activePoints.push_back(std::make_pair(nX, nY));
                raster[nX][nY].setChecked(true);
            }
            if (raster[nX][nY].getType() == Field::block && raster[nX][nY].getHeight() < height) {
                //raster[nX][nY].setChecked(true);

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

void Table::checkWaterAround(std::pair<int, int> block) {
    activePoints.push_back(block);
    std::pair<int, int> activePoint;

    while (!activePoints.empty()) {
        activePoint = activePoints.front();
        setNeighboursChecked(activePoint.first, activePoint.second);
        activePoints.pop_front();
    }
}

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

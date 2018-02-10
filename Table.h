//
// Created by ewa on 27.12.17.
//

#ifndef AAL_TABLE_H
#define AAL_TABLE_H

#include <cstdlib>
#include <map>
#include <list>
#include "Cube.h"
#include "Cuboid.h"
/**
 * represents raster and blocks in 2D array
 */
class Table
{
private:
    int width;
    int length;
    int maxHeight;
    int emptyFields;
    int leftWaterVolume;
    Field ** raster;
    std::list<Block *> * blockslist;
    std::list<std::pair<int, int>> activePoints;

    std::list<std::pair <int, int >> getNeighbours(int x, int y);
    std::list<std::pair<int, int>> getBlock();
    void setNeighboursEmpty(int x, int y);
    void setNeighboursChecked(int x, int y);
    std::list<std::pair<int, int>> findEmptyField();
    void setEmptyFields();
    void checkWaterAround(std::pair<int, int>);
    void checkWater();
public:

    Table(int w, int len, int maxHeight);
    Table(int width, int length, int maxHeight, std::list<Block*> * blockList);
    int countVolume();
    void print();

    int getWidth();
    int getLength();
    int getHeight();
    Field * getField(int x, int y);
    std::list<Block *> * getBlocks();
    void writeToFile2(std::string fname);
};

#endif //AAL_TABLE_H

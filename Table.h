//
// Created by ewa on 27.12.17.
//

#ifndef AAL_TABLE_H
#define AAL_TABLE_H

#include <cstdlib>
#include <map>
#include <list>
#include "Cube.h"

class table
{
private:
    int M;
    int N;
    int maxHeight;
    int emptyFields;
    int underWaterFields;
    int underWaterBlocks;
    Field ** raster;
    std::list<Block *> * blockslist;
    std::list<std::pair<int, int>> activePoints;

    std::list<std::pair <int, int >> getNeighbours(int x, int y);
    std::list<std::pair <int, int>> getAllNeighbours(int x, int y);
    void setNeighboursEmpty(int x, int y);
    std::list<std::pair<int, int>> findEmptyField();
    std::list<std::pair<int, int>>  checkWaterAround(int x, int y);
public:

    table(int m, int n, int maxHeight, std::list<Block*> * blockList);
    void print();
    void printBlocks();
    void printactivePoints();
    void setEmptyFields();
    void checkWaterLevel(std::pair<int, int> block);
    void checkWater();


    std::list<std::pair<int, int>> getWaterBlocks();
    void checkBlocks();

};
std::list<Block *> * createFromFile (std::string fname, int maxHeight);

#endif //AAL_TABLE_H

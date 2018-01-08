//
// Created by ewa on 27.12.17.
//

#ifndef AAL_TABLE_H
#define AAL_TABLE_H

#include <cstdlib>
#include <map>
#include <list>
#include "Cube.h"
#include "Raster.h"

class table
{
private:
    int M;
    int N;
    int maxHeight;
    int emptyFields;
    int leftWaterVolume;
    Field ** raster;
    std::list<Block *> * blockslist;
    std::list<std::pair<int, int>> activePoints;

    std::list<std::pair <int, int >> getNeighbours(int x, int y);
    void setNeighboursEmpty(int x, int y);
    void setNeighboursChecked(int x, int y);
    std::list<std::pair<int, int>> findEmptyField();
public:

    table(int m, int n, int maxHeight);
    table(int m, int n, int maxHeight, std::list<Block*> * blockList);
    void print();
    void printBlocks();
    void printactivePoints();
    void setEmptyFields();
    void checkWaterAround(std::pair<int, int>);
    void checkWater();
    int countVolume();
    void generateRaster(float percent);

    std::list<std::pair<int, int>> getBlock();

    int getLeftWaterVolume();
    int getEmptyFields();
    int getWidth();
    int getLength();
    int getHeight();
    Field * getField(int x, int y);
    std::list<Block *> * getBlocks();
    void writeToFile(std::string fname);
    void writeToFile2(std::string fname);

};
std::list<Block *> * createFromFile (std::string fname, int maxHeight);

#endif //AAL_TABLE_H

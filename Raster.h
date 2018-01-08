//
// Created by eva on 29.12.17.
//

#ifndef AAL_RASTER_H
#define AAL_RASTER_H

#include <utility>
#include <list>
#include <string>
#include "Cube.h"

class Raster
{
private:
    int M;
    int N;
    int H;
    int emptyFields;
    cube *** raster;
    std::list<std::pair<int, int>> activePoints;
    int * blocksPerLevel;

    std::list<std::pair<int, int>> findEmptyField(int z);
    std::list<std::pair <int, int >> getNeighbours(int x, int y);
    void setNeighboursEmpty(int x, int y, int Z);

public:

    Raster(std::string fname , int m, int n, int h);
    Raster(int m, int n, int h);

    void createFromFile (std::string fname);
    void setEmptyFields(int z);
    void print();
    int countVolume();
    int getEmptyFields();
    cube *** getRaster();
    int * getBlocksPerLevel();
};


#endif //AAL_RASTER_H

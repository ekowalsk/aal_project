//
// Created by Ewa Kowalska on 29.12.17.
//

#ifndef AAL_RASTER_H
#define AAL_RASTER_H

#include <utility>
#include <list>
#include <string>
#include "Cube.h"
/**
 * represents raster and blocks in 3D array
 */
class Cuboid
{
private:

    int width;
    int length;
    int maxHeight;
    int emptyFields;
    cube *** raster;
    std::list<std::pair<int, int>> activePoints;
    int * blocksPerLevel;

    std::list<std::pair<int, int>> findEmptyField(int z);
    std::list<std::pair <int, int >> getNeighbours(int x, int y);
    void setNeighboursEmpty(int x, int y, int Z);

public:

    Cuboid(std::string fname , int width, int length, int height);
    Cuboid(int width, int length, int height);

    void createFromFile (std::string fname);
    void setEmptyFields(int z);
    void print();
    int countVolume();
    int getEmptyFields();
    cube *** getRaster();
    int * getBlocksPerLevel();
};


#endif //AAL_RASTER_H

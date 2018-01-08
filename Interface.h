//
// Created by ewa on 07.01.18.
//

#ifndef AAL_INOUT_H
#define AAL_INOUT_H
#include "Table.h"
#include "Raster.h"


class Interface {
public:
    std::string mode;
    int width;
    int length;
    int height;
    int density;
    Interface();
    bool checkParameters(int argc, char * argv[]);
};

void generateRaster(float percent, table myTable, Raster myRaster);
void readFromFile (table myTable, Raster raster);



#endif //AAL_INOUT_H
//
// Created by ewa on 08.01.18.
//

#ifndef AAL_TEST_H
#define AAL_TEST_H
#include "Table.h"
#include "Cuboid.h"
#include "Parameters.h"
#include <chrono>

void generateRaster(float percent, Table myTable, Cuboid myRaster);
void readFromFile (Table myTable, Cuboid raster);
int makeOneTest (Parameters parameters, double * tableDuration, double * rasterDuration, int width, int length, int height);
void makeTests (Parameters parameters);
void printResults(std::multimap<double, double> durations);
#endif //AAL_TEST_H

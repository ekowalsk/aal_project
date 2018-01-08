//
// Created by ewa on 08.01.18.
//

#ifndef AAL_TEST_H
#define AAL_TEST_H
#include "Table.h"
#include "Raster.h"
#include "Parameters.h"
#include <chrono>

void generateRaster(float percent, table myTable, Raster myRaster);
void readFromFile (table myTable, Raster raster);
int makeOneTest (Parameters parameters, double * tableDuration, double * rasterDuration);
void makeTests (Parameters parameters);
void printResults(std::multimap<double, double> durations);
#endif //AAL_TEST_H

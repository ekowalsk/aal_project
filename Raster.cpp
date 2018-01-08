//
// Created by ewa on 29.12.17.
//

#include <fstream>
#include <iostream>
#include "Raster.h"
Raster::Raster(int m, int n, int h) : M(m), N(n), H(h) {
    emptyFields = 0;

    raster = new cube **[M];
    for (int i = 0; i < M; i++) {
        raster[i] = new cube *[N];
    }
    for (int x = 0; x < M; x++)
        for (int y = 0; y < N; y++)
            raster[x][y] = new cube[H];

    blocksPerLevel = new int[H];
    for (int i = 0; i < H; i++)
        blocksPerLevel[i] = 0;
}

Raster::Raster(std::string fname, int m, int n, int h) : M(m), N(n), H(h) {
    emptyFields = 0;

    raster = new cube **[M];
    for (int i = 0; i < M; i++) {
        raster[i] = new cube *[N];
    }
    for (int x = 0; x < M; x++)
        for (int y = 0; y < N; y++)
            raster[x][y] = new cube[H];

    blocksPerLevel = new int[H];
    for (int i = 0; i < H; i++)
        blocksPerLevel[i] = 0;

    createFromFile(fname);
};

void Raster::createFromFile(std::string fname) {
    std::fstream file;
    file.open(fname, std::ios::in);
    if (file.good()) {
        int x, y;
        float height;
        while (!file.eof()) {
            file >> x;
            // EOF mark just after the last character
            if (file.eof()) break;
            file >> y;
            file >> height;
            for (int z = 0; z < height; z++) {
                raster[x][y][z].setType(cube::blockfield);
                blocksPerLevel[z]++;
            }
        }
        file.close();
    } else {
        std::cout << "can't open file" << std::endl;
    }
};

std::list<std::pair<int, int>> Raster::findEmptyField(int z) {
    std::list<std::pair<int, int>> emptyFields;
    int x, y;
    for (x = 0, y = 0; x < M; x++) {
        if (raster[x][y][z].getType() == cube::waterfield) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }
// y = 1 aby uniknąć redundancji przy dodawaniu narożników
    for (y = 1, x = M - 1; y < N; y++) {
        if (raster[x][y][z].getType() == cube::waterfield) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (x = 0, y = N - 1; x < M - 1; x++) {
        if (raster[x][y][z].getType() == cube::waterfield) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (y = 1, x = 0; y < N - 1; y++) {
        if (raster[x][y][z].getType() == cube::waterfield) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }


    for (auto &it : emptyFields) {
        raster[it.first][it.second][z].setType(cube::emptyfield);
        this->emptyFields++;
    }
    return emptyFields;
};

// used while setting fields with no water
void Raster::setNeighboursEmpty(int x, int y, int z) {
    std::list<std::pair<int, int>> neighbours = getNeighbours(x, y);
    for (auto &it : neighbours) {
        int x = it.first;
        int y = it.second;
        if (raster[x][y][z].getType() == cube::waterfield) {
            raster[x][y][z].setType(cube::emptyfield);
            activePoints.emplace_back(std::pair<int, int>(x, y));
            emptyFields++;
        }
    }
};

void Raster::setEmptyFields(int z) {
    activePoints = findEmptyField(z);
    std::pair<int, int> activePoint;
    while (!activePoints.empty()) {
        activePoint = activePoints.front();
        setNeighboursEmpty(activePoint.first, activePoint.second, z);
        activePoints.pop_front();
    }
};

std::list<std::pair<int, int>> Raster::getNeighbours(int x, int y) {
    std::list<std::pair<int, int>> neighbours;
    int lx = x - 1;
    int rx = x + 1;
    int dy = y - 1;
    int uy = y + 1;

    if (lx >= 0)
        neighbours.emplace_back(std::pair<int, int>(lx, y));
    if (rx < M)
        neighbours.emplace_back(std::pair<int, int>(rx, y));
    if (dy >= 0)
        neighbours.emplace_back(std::pair<int, int>(x, dy));
    if (uy < N)
        neighbours.emplace_back(std::pair<int, int>(x, uy));
    return neighbours;
};

void Raster::print() {
    for (int z = H - 1; z >= 0; z--) {
        for (int y = 0; y < N; y++) {
            for (int x = 0; x < M; x++) {
                std::cout << raster[x][y][z].getType();
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

int Raster::countVolume() {
    for (int z = 0; z < H; z++) {
        setEmptyFields(z);
    }
    int blocksVolume = 0;
    for (int i = 0; i < H; i++) {
        blocksVolume += blocksPerLevel[i];
    }
    std::cout << "blockVolume: " << blocksVolume << std::endl;
    return M * N * H - emptyFields - blocksVolume;
};

int Raster::getEmptyFields() {
    return emptyFields;
};

cube *** Raster::getRaster(){
    return raster;
}
int * Raster::getBlocksPerLevel(){
    return blocksPerLevel;
}



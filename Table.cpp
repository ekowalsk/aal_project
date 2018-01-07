//
// Created by ewa on 27.12.17.
//

#include <iostream>
#include <fstream>
#include "Table.h"

table::table(int m, int n, int maxHeight) : M(m), N(n) {
    this->maxHeight = maxHeight;
    emptyFields = 0;
    leftWaterVolume = 0;

    raster = new Field *[M];
    for (int i = 0; i < M; i++) {
        raster[i] = new Field[N];
    }

    blockslist = new std::list<Block *>[maxHeight + 1];
}

table::table(int m, int n, int maxHeight, std::list<Block *> *blockList) : M(m), N(n) {
    this->maxHeight = maxHeight;
    emptyFields = 0;
    leftWaterVolume = 0;

    raster = new Field *[M];
    for (int i = 0; i < M; i++) {
        raster[i] = new Field[N];
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
            raster[x][y].setType(Field::blockfield);
            raster[x][y].setHeight(it->getHeight());
        }
    }
};

void table::generateRaster(float percent) {
    int pos, x, y;
    int blocksAmount = int((percent * M * N) / 100);
    for (int i = 0; i < blocksAmount; i++) {
        pos = rand() % (M * N);
        x = pos % M;
        y = pos / M;
        while (raster[x][y].getType() != Field::waterfield) {
            pos = (pos + 3) % (M * N);
            x = pos % M;
            y = pos / M;
        }
        int height = rand() % maxHeight + 1;
        raster[x][y].setType(cube::blockfield);
        raster[x][y].setHeight(height);
        Block *newBlock = new Block(x, y, height);
        blockslist[height].push_back(newBlock);
    }
}

void table::writeToFile2(std::string fname) {
    std::fstream file;
    file.open(fname, std::ios::out | std::ios::trunc);
    if (file.good()) {
        for (int y = 0; y < N; y++) {
            for (int x = 0; x < M; x++) {
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

void table::writeToFile(std::string fname) {
    std::fstream file;
    file.open(fname, std::ios::out | std::ios::trunc);
    if (file.good()) {
        for (int y = 0; y < N; y++) {
            for (int x = 0; x < M; x++) {
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

std::list<Block *> *createFromFile(std::string fname, int maxHeight) {
    std::fstream file;
    file.open(fname, std::ios::in);
    if (file.good()) {
        auto *blockList = new std::list<Block *>[maxHeight + 1];
        int x, y;
        int height;
        while (!file.eof()) {
            file >> x;
            // EOF mark just after the last character
            if (file.eof())
                break;
            file >> y;
            file >> height;
            auto *newBlock = new Block(x, y, height);
            blockList[height].push_back(newBlock);
        }
        file.close();
        return blockList;
    } else {
        std::cout << "can't open file" << std::endl;
    }
}

void table::print() {
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < M; x++) {
            std::string height = std::to_string(raster[x][y].getHeight());
            if (height.length() == 1)
                height += "  ";
            if (height.length() == 2)
                height += " ";

            std::cout << "h: " << height << " t: " << raster[x][y].getType()
                      << " chec: " << raster[x][y].isChecked() << " | ";
        }
        std::cout << std::endl;
    }
}


void table::printBlocks() {
    for (int i = 1; i <= maxHeight; i++)
        for (auto &it : blockslist[i]) {
            std::cout << "height: " << it->getHeight() << " ";
            std::cout << "x: " << it->getX() << " ";
            std::cout << "y: " << it->getY() << std::endl;
        }
};

std::list<std::pair<int, int>> table::findEmptyField() {
    std::list<std::pair<int, int>> emptyFields;
    int x, y;
    for (x = 0, y = 0; x < M; x++) {
        if (raster[x][y].getType() == Field::waterfield) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }
// y = 1 aby uniknąć redundancji przy dodawaniu narożników
    for (y = 1, x = M - 1; y < N; y++) {
        if (raster[x][y].getType() == Field::waterfield) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (x = 0, y = N - 1; x < M - 1; x++) {
        if (raster[x][y].getType() == Field::waterfield) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (y = 1, x = 0; y < N - 1; y++) {
        if (raster[x][y].getType() == Field::waterfield) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (auto &it : emptyFields) {
        raster[it.first][it.second].setType(cube::emptyfield);
        raster[it.first][it.second].setChecked(true);
        this->emptyFields++;
    }
    return emptyFields;
};

void table::printactivePoints() {
    for (auto &activePoint : table::activePoints) {
        std::cout << "x: " << activePoint.first << " y: " << activePoint.second << std::endl;
    }
}

// used while setting fields with no water
void table::setNeighboursEmpty(int x, int y) {
    std::list<std::pair<int, int>> neighbours = getNeighbours(x, y);
    for (auto &it : neighbours) {
        int x = it.first;
        int y = it.second;
        if (raster[x][y].getType() == Field::waterfield) {
            raster[x][y].setType(Field::emptyfield);
            raster[x][y].setChecked(true);
            activePoints.emplace_back(std::pair<int, int>(x, y));
            emptyFields++;
        }
    }
}

void table::setEmptyFields() {
    activePoints = findEmptyField();
    std::pair<int, int> activePoint;
    while (!table::activePoints.empty()) {
        activePoint = activePoints.front();
        setNeighboursEmpty(activePoint.first, activePoint.second);
        table::activePoints.pop_front();
    }
}

std::list<std::pair<int, int>> table::getNeighbours(int x, int y) {
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
}

std::list<std::pair<int, int>> table::getBlock() {
    int x, y;
    bool hasCheckedField = false, hasUncheckedField = false;
    std::list<std::pair<int, int>> blocks;
    std::list<std::pair<int, int>> neighbours;
    for (int i = 1; i <= maxHeight; i++) {
        for (auto &it : blockslist[i]) {
            x = it->getX();
            y = it->getY();
            if (!raster[x][y].isChecked()) {
                neighbours = getNeighbours(x, y);
                for (auto &iter : neighbours) {
                    int nX = iter.first;
                    int nY = iter.second;
                    if (raster[nX][nY].getType() == Field::emptyfield || x == 0 || x == M - 1 || y == 0 ||
                        y == N - 1) {
                        hasCheckedField = true;
                    }

                    if ((raster[nX][nY].getType() == Field::waterfield || raster[nX][nY].getType() == Field::blockfield)
                        && raster[nX][nY].isChecked()) {
                        hasCheckedField = true;
                    }
                    if (raster[nX][nY].getType() == Field::waterfield && !raster[nX][nY].isChecked()) {
                        hasUncheckedField = true;
                    }
                    if (raster[nX][nY].getType() == Field::blockfield && !raster[nX][nY].isChecked()) {
                        hasUncheckedField = true;
                    }

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


void table::setNeighboursChecked(int x, int y) {
    std::list<std::pair<int, int>> neighbours = getNeighbours(x, y);
    int height = raster[x][y].getHeight();
    bool hasEmptyNeigh = false;
    for (auto &it : neighbours) {
        int nX = it.first;
        int nY = it.second;
        if (!raster[nX][nY].isChecked()) {
            if (raster[nX][nY].getType() == Field::waterfield) {
                raster[nX][nY].setChecked(true);
                raster[nX][nY].setHeight(height);
                leftWaterVolume += height;
                activePoints.emplace_back(std::pair<int, int>(nX, nY));
            }
            if (raster[nX][nY].getType() == Field::blockfield && raster[nX][nY].getHeight() == height) {
                activePoints.push_back(std::make_pair(nX, nY));
                raster[nX][nY].setChecked(true);
            }
            if (raster[nX][nY].getType() == Field::blockfield && raster[nX][nY].getHeight() < height) {
                //raster[nX][nY].setChecked(true);

                if (!(nX == 0 || nX == M - 1 || nY == 0 || nY == N - 1)) {
                    std::list<std::pair<int, int>> neigh = getNeighbours(nX, nY);
                    // może ustawiać na water bloki, za którymi nic nie ma
                    for (auto &it : neigh) {
                        if (raster[it.first][it.second].getType() == Field::emptyfield)
                            hasEmptyNeigh = true;
                    }
                    if (!hasEmptyNeigh) {
                        leftWaterVolume += height - raster[nX][nY].getHeight();
                        raster[nX][nY].setChecked(true);
                        raster[nX][nY].setHeight(height);
                        raster[nX][nY].setType(Field::waterfield);
                        activePoints.emplace_back(std::pair<int, int>(nX, nY));
                    }
                }
            }
        }
    }
}

void table::checkWaterAround2(std::pair<int, int> block) {
    activePoints.push_back(block);
    std::pair<int, int> activePoint;

    while (!activePoints.empty()) {
        activePoint = activePoints.front();
        setNeighboursChecked(activePoint.first, activePoint.second);
        activePoints.pop_front();
    }
}

void table::checkWater() {
    std::list<std::pair<int, int>> block = getBlock();
    while (!block.empty()) {
        checkWaterAround2(block.front());
        int x = block.front().first;
        int y = block.front().second;
        raster[x][y].setChecked(true);
        block = getBlock();
    }
}

int table::getLeftWaterVolume() {
    return leftWaterVolume;
}

int table::getEmptyFields() {
    return emptyFields;
}

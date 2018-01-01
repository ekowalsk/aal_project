//
// Created by ewa on 27.12.17.
//

#include <iostream>
#include <fstream>
#include "Table.h"

table::table(int m, int n, int maxHeight, std::list<Block *> *blockList) : M(m), N(n) {
    this->maxHeight = maxHeight;
    emptyFields = 0;

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
            raster[x][y].setType(cube::blockfield);
            raster[x][y].setHeight(it->getHeight());
        }
    }
};

std::list<Block *> *createFromFile(std::string fname, int maxHeight) {
    std::fstream file;
    file.open(fname, std::ios::in);
    if (file.good()) {
        std::list<Block *> *blockList = new std::list<Block *>[maxHeight + 1];
        int x, y;
        int height;
        while (!file.eof()) {
            file >> x;
            // EOF mark just after the last character
            if (file.eof())
                break;
            file >> y;
            file >> height;
            Block *newBlock = new Block(x, y, height);
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
            std::cout << "h: " << raster[x][y].getHeight() << " t: " << raster[x][y].getType()
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
        if (raster[x][y].getType() == cube::waterfield) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }
// y = 1 aby uniknąć redundancji przy dodawaniu narożników
    for (y = 1, x = M - 1; y < N; y++) {
        if (raster[x][y].getType() == cube::waterfield) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (x = 0, y = N - 1; x < M - 1; x++) {
        if (raster[x][y].getType() == cube::waterfield) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (y = 1, x = 0; y < N - 1; y++) {
        if (raster[x][y].getType() == cube::waterfield) {
            emptyFields.emplace_back(std::make_pair(x, y));
        }
    }

    for (auto &it : emptyFields) {
        raster[it.first][it.second].setType(cube::emptyfield);
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
            table::activePoints.emplace_back(std::pair<int, int>(x, y));
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

std::list<std::pair<int, int>> table::getAllNeighbours(int x, int y) {
    std::list<std::pair<int, int>> neighbours = getNeighbours(x, y);
    int lx = x - 1;
    int rx = x + 1;
    int dy = y - 1;
    int uy = y + 1;

    if (lx >= 0) {
        if (dy >= 0)
            neighbours.emplace_back(std::pair<int, int>(lx, dy));
        if (uy < N)
            neighbours.emplace_back(std::pair<int, int>(lx, uy));
    }

    if (rx < M) {
        if (dy >= 0)
            neighbours.emplace_back(std::pair<int, int>(rx, dy));
        if (uy < N)
            neighbours.emplace_back(std::pair<int, int>(rx, uy));
    }
    return neighbours;
};


std::list<std::pair<int, int>> table::getWaterBlocks() {
    int x, y;
    std::list<std::pair<int, int>> blocks;
    std::list<std::pair<int, int>> neighbours;


    for (int i = 1; i <= maxHeight; i++) {
        for (auto &it : blockslist[i]) {
            x = it->getX();
            y = it->getY();
            neighbours = getNeighbours(x, y);

            for (auto &iter : neighbours) {
                if (raster[iter.first][iter.second].getType() == Field::waterfield) {
                    blocks.push_back(iter);
                    return blocks;
                }
            }
        }
    }
    return blocks;

};

std::list<std::pair<int, int>> table::checkWaterAround(int x, int y) {
    std::list<std::pair<int, int>> neighbours = getNeighbours(x, y);
    for (auto &it : neighbours) {
        int itX = it.first;
        int itY = it.second;
        if (raster[itX][itY].getType() == Field::waterfield &&
            !raster[itX][itY].isChecked()) {
            activePoints.push_back(it);
            raster[itX][itY].setChecked(true);
            if (raster[itX][itY].getHeight() < raster[x][y].getHeight()) {
                raster[itX][itY].setHeight(raster[x][y].getHeight());
            }
        }
        if (raster[itX][itY].getType() == Field::blockfield && !raster[itX][itY].isChecked()) {
            if (raster[itX][itY].getHeight() <= raster[x][y].getHeight() &&
                !(itX == 0 || itX == M - 1 || itY == 0 || itY == N - 1)) {
                std::list<std::pair<int, int>> neigh = getAllNeighbours(itX, itY);
                for (auto &n : neigh) {
                    int nx = n.first;
                    int ny = n.second;
                    if (raster[nx][ny].getType() == Field::emptyfield )
                        break;
                    else {
                        raster[itX][itY].setType(Field::waterfield);
                        raster[itX][itY].setChecked(true);
                        if (raster[itX][itY].getHeight() < raster[x][y].getHeight()) {
                            raster[itX][itY].setHeight(raster[x][y].getHeight());
                            activePoints.push_back(it);
                        }
                    }

                }
            }
        }
    }
    neighbours.emplace_back(std::make_pair(x, y));
    return neighbours;
}

void table::checkWaterLevel(std::pair<int, int> block) {
    std::list<std::pair<int, int>> checked;
    std::list<std::pair<int, int>> toBeUnchecked = checkWaterAround(block.first, block.second);
    std::pair<int, int> activePoint;
    int movements = 0;

    while (!activePoints.empty()) {
        activePoint = activePoints.front();
        int x = activePoint.first;
        int y = activePoint.second;
        movements++;
        checked = checkWaterAround(x, y);
        for (auto &it : checked) {
            toBeUnchecked.push_back(it);
        }
        activePoints.pop_front();
    }
    for (auto &it : toBeUnchecked) {
        raster[it.first][it.second].setChecked(false);
    }
    std::cout << "movements: " << movements << std::endl;
}

void table::checkWater()
{

}

/*int table::checkWaterfieldNeighbour(int x, int y, int minheight, std::pair<int, int> previous, bool * setToWater)
{
    int underWaterFields = 0;
    bool filled = false;

    std::list<std::pair<int, int>> neighbours = getNeighbours(x, y);
    for (auto &it : neighbours)
    {
        int neighX = it.first;
        int neighY = it.second;
        int prevX = previous.first;
        int prevY = previous.second;
        if (!(neighX == previous.first && neighY == previous.second))
        {
            if (raster[neighX][neighY].getType() == cube::waterfield)
            {
                if (raster[neighX][neighY].isChecked() != true)
                {
                    table::activePoints.emplace_back(std::pair<int, int>(neighX, neighY));
                    raster[neighX][neighY].setChecked(true);
                    underWaterFields++;
                    filled  = true;
                }
            }

            if (raster[neighX][neighY].getType() == cube::blockfield)
            {
                if (raster[neighX][neighY].getHeight() <= minheight)
                {
                    if (raster[neighX][neighY].isChecked() != true)
                    {
                        table::activePoints.emplace_back(std::pair<int, int>(neighX, neighY));
                        raster[neighX][neighY].setChecked(true);
                        underWaterFields++;
                        underWaterVolume += raster[neighX][neighY].getHeight();
                        filled = true;
                    }
                }
            }
        }
    }
    if (filled && !(x == 0 || y == 0 || x == (M-1) || y == (N-1)) && raster[x][y].getType() == cube::blockfield)
    {
        raster[x][y].setType(cube::waterfield);
        raster[x][y].setChecked(true);
        underWaterVolume += raster[x][y].getHeight();
        *setToWater = true;
    }
   // activePoints.pop_front();
    return underWaterFields;
}

void table::checkWater()
{
    std::multimap<float, block*> blocks = createFromFile("block.txt");
    table myTable = table(M, N, blocks);
    myTable.setEmptyFields();
    bool setToWater;

    std::pair<int, int> smallestBlock = getSmallestBlock();
    float minHeight = raster[smallestBlock.first][smallestBlock.second].getHeight();
    table::activePoints.push_back(smallestBlock);
    std::pair<int, int> activePoint;
    std::pair<int, int> previousPoint = std::make_pair(-1,-1);

    int underwaterfields = 0;
    while (!table::activePoints.empty())
    {
        activePoint = activePoints.front();
        int activeX = activePoint.first;
        int activeY = activePoint.second;

        underwaterfields += myTable.checkWaterfieldNeighbour(activePoint.first, activePoint.second, minHeight, previousPoint, &setToWater);
        if (setToWater)
        {
            float height = raster[activeX][activeY].getHeight();
            raster[activeX][activeY].setType(cube::waterfield);
            underWaterVolume += raster[activeX][activeY].getHeight();
            blocksMap.erase(height);
        }
        previousPoint = activePoint;
        std::cout << previousPoint.first << " " << previousPoint.second << std::endl;
        table::activePoints.pop_front();
    }
    underWaterFields = underwaterfields;
}
*/

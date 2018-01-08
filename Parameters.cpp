//
// Created by ewa on 07.01.18.
//
#include <iostream>
#include "Parameters.h"

Parameters::Parameters() {
    mode = " ";
    width = 0;
    length = 0;
    height = 0;
    density = 0;
    nProblems = 0;
    nInstances = 0;
    step = 0;
}

bool Parameters::checkParameters(int argc, char *argv[]) {
    if (argc == 5) {
        std::string strMode("-m1");
        if (strMode.compare(argv[1]) != 0)
            return false;

        mode = strMode;
        width = std::stoi(argv[2], nullptr, 10);
        length = std::stoi(argv[3], nullptr, 10);
        height = std::stoi(argv[4], nullptr, 10);
        return true;
    }
    if (argc == 6) {
        std::string strMode("-m2");
        if (strMode.compare(argv[1]) != 0)
            return false;

        mode = strMode;
        width = std::stoi(argv[2], nullptr, 10);
        length = std::stoi(argv[3], nullptr, 10);
        height = std::stoi(argv[4], nullptr, 10);
        density = std::stoi(argv[5], nullptr, 10);
        return true;
    }
    if (argc == 9) {
        std::string strMode("-m3");
        if (strMode.compare(argv[1]) != 0)
            return false;

        mode = strMode;
        width = std::stoi(argv[2], nullptr, 10);
        length = std::stoi(argv[3], nullptr, 10);
        height = std::stoi(argv[4], nullptr, 10);
        density = std::stoi(argv[5], nullptr, 10);
        nProblems = std::stoi(argv[6], nullptr, 10);
        step = std::stoi(argv[7], nullptr, 10);
        nInstances = std::stoi(argv[8], nullptr, 10);

        return true;
    } else {
        std::cerr << "Usage: " << std::endl;
        std::cerr
                << "-m1 - read data from file data.txt and write solution to solution.txt,"
                        " parameters: width, length, maximum height " << std::endl;
        std::cerr << "example: " << argv[0] << " -m1 100 120 60 << data.txt >> solution.txt" << std::endl;
        std::cerr << "-m2 - generate data and write solution to file, parameters: width, length,"
                " maximum height, density percent" << std::endl;
        std::cerr << "example " << argv[0] << " -m2 100 120 60 80 >> solution.txt " << std::endl;
        std::cerr << "-m3 - test" << std::endl;
        std::cerr << argv[0] << "-m3" << std::endl;

        return false;
    }
}


//
// Created by ewa on 07.01.18.
//

#ifndef AAL_INOUT_H
#define AAL_INOUT_H


class Parameters {
public:
    std::string mode;
    int width;
    int length;
    int height;
    int density;
    int nProblems;
    int nInstances;
    int step;
    Parameters();
    bool checkParameters(int argc, char * argv[]);
};

#endif //AAL_INOUT_H
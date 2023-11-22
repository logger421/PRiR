//
// Created by Wojciech Lotko on 12/11/2023.
//

#ifndef PRIR_LIFEPARALLELIMPLEMENTATION_H
#define PRIR_LIFEPARALLELIMPLEMENTATION_H
#include "Life.h"
#include "mpi.h"
#include "algorithm"
#include <iostream>

class LifeParallelImplementation: public Life {
protected:
    void realStep();
public:
    LifeParallelImplementation();
    int procs;
    int rank;
    int startRow;
    int endRow;
    int numberOfLivingCells();
    double averagePollution();
    void oneStep();
    void syncData();
    void beforeFirstStep() override;
    void afterLastStep() override;
};


#endif //PRIR_LIFEPARALLELIMPLEMENTATION_H

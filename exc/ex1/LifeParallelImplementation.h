//
// Created by Wojciech Lotko on 12/11/2023.
//

#ifndef PRIR_LIFEPARALLELIMPLEMENTATION_H
#define PRIR_LIFEPARALLELIMPLEMENTATION_H
#include "Life.h"

class LifeParallelImplementation: public Life {
protected:
    void realStep();
public:
    LifeParallelImplementation();
    int numberOfLivingCells();
    double averagePollution();
    void oneStep();
};


#endif //PRIR_LIFEPARALLELIMPLEMENTATION_H

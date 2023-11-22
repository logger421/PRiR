//
// Created by Wojciech Lotko on 12/11/2023.
//

#include "LifeParallelImplementation.h"
#include "mpi.h"

LifeParallelImplementation::LifeParallelImplementation() {}

void LifeParallelImplementation::realStep() {
    int currentState, currentPollution;

    for (int row = startRow + 1; row < endRow; row++) {
        for (int col = 1; col < size_1; col++) {
            currentState = cells[row][col];
            currentPollution = pollution[row][col];
            cellsNext[row][col] = rules->cellNextState(currentState,
                                                       liveNeighbours(row, col),
                                                       currentPollution);
            pollutionNext[row][col] = rules->nextPollution(currentState, currentPollution,
                                                           pollution[row + 1][col] + pollution[row - 1][col] + pollution[row][col - 1] + pollution[row][col + 1],
                                                           pollution[row - 1][col - 1] + pollution[row - 1][col + 1] + pollution[row + 1][col - 1] + pollution[row + 1][col + 1]);
        }
    }
}

void LifeParallelImplementation::oneStep() {
    realStep();
    swapTables();
}

void LifeParallelImplementation::beforeFirstStep() {
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    startRow = (rank * size) / procs;
    endRow = ((rank + 1) * size) / procs;
    endRow = std::min(endRow, size_1);
}

void LifeParallelImplementation::afterLastStep() {
    MPI_Barrier(MPI_COMM_WORLD);
}

int LifeParallelImplementation::numberOfLivingCells() {
    return sumTable( cells );
}

double LifeParallelImplementation::averagePollution() {
    return (double)sumTable( pollution ) / size_1_squared / rules->getMaxPollution();
}

void LifeParallelImplementation::syncData() {
    int destination = (rank + 1) % procs;
    int source = (rank - 1 + procs) % procs;

    MPI_Send(cellsNext[endRow], size, MPI_INT, destination, 0, MPI_COMM_WORLD);
    MPI_Send(pollutionNext[endRow], size, MPI_INT, destination, 0, MPI_COMM_WORLD);

    MPI_Recv(cellsNext[startRow - 1], size, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(pollutionNext[startRow - 1], size, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    startRow = (startRow - 1 + size) % size;
    endRow = (endRow - 1 + size) % size;
}
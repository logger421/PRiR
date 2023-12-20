//
// Created by Wojciech Lotko on 12/11/2023.
//

#include "LifeParallelImplementation.h"
#include "mpi.h"

LifeParallelImplementation::LifeParallelImplementation() {}

void LifeParallelImplementation::realStep() {
    int currentState, currentPollution;
    for (int row = startRow; row <= endRow; row++) {
        if(row == size_1) break;
        for (int col = 1; col < size_1; col++) {
            currentState = cells[row][col];
            currentPollution = pollution[row][col];
            cellsNext[row][col] = rules->cellNextState(currentState, liveNeighbours(row, col), currentPollution);

            pollutionNext[row][col] = rules->nextPollution(currentState, currentPollution,
                                                           pollution[row + 1][col] + pollution[row - 1][col] +
                                                           pollution[row][col - 1] + pollution[row][col + 1],
                                                           pollution[row - 1][col - 1] + pollution[row - 1][col + 1] +
                                                           pollution[row + 1][col - 1] + pollution[row + 1][col + 1]);
        }
    }
}

void LifeParallelImplementation::oneStep() {
    realStep();
    swapTables();
    syncData();
}

void LifeParallelImplementation::beforeFirstStep() {
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    startRow = (rank * size) / procs;
    endRow = ((rank + 1) * size) / procs;
    rowsTotal = (endRow - startRow);

    if (!rank)
        startRow = 1;
    endRow = std::min(endRow, size_1);

    if (procs > 1) {
        int *flattenedCells = new int[size * size];
        if (!rank) {
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    flattenedCells[i * size + j] = cells[i][j];
                }
            }
        }
        MPI_Bcast(flattenedCells, size * size, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank) {
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    cells[i][j] = flattenedCells[i * size + j];
                    pollution[i][j] = flattenedCells[i * size + j];
                }
            }
        }
        delete[] flattenedCells;
    }
}

void LifeParallelImplementation::afterLastStep() {
    if (procs > 1) {
        if (!rank) {
            for (int proc_num = 1; proc_num < procs; proc_num++) {
                int localStart = (proc_num * size) / procs;
                int localEnd = ((proc_num + 1) * size) / procs;
                localEnd = std::min(localEnd, size_1);
                for (int i = localStart; i <= localEnd; i++) {
                    MPI_Recv(cells[i], size, MPI_INT, proc_num, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Recv(pollution[i], size, MPI_INT, proc_num, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }
        }

        if (rank) {
            for (int i = startRow; i <= endRow; i++) {
                MPI_Send(cells[i], size, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Send(pollution[i], size, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

int LifeParallelImplementation::numberOfLivingCells() {
    return sumTable(cells);
}

double LifeParallelImplementation::averagePollution() {
    return (double) sumTable(pollution) / size_1_squared / rules->getMaxPollution();
}

void LifeParallelImplementation::syncData() {
    if (procs > 1) {
        if (rank == 0) {
            MPI_Send(cells[endRow], size, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Send(pollution[endRow], size, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

            MPI_Recv(cells[endRow + 1], size, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(pollution[endRow + 1], size, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else if ((rank > 0 && rank < procs - 1)) {
            MPI_Recv(cells[startRow - 1], size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(pollution[startRow - 1], size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(cells[startRow], size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
            MPI_Send(pollution[startRow], size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

            MPI_Send(cells[endRow], size, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Send(pollution[endRow], size, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(cells[endRow + 1], size, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(pollution[endRow + 1], size, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else if (rank == procs - 1) {
            MPI_Recv(cells[startRow - 1], size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(pollution[startRow - 1], size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(cells[startRow], size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
            MPI_Send(pollution[startRow], size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
        }
    }
}
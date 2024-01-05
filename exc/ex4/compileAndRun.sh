#!/bin/bash
export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"
export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"
export PATH="/usr/local/opt/llvm/bin:$PATH"
export OMP_NUM_THREADS=6
g++-13 -O2 -fopenmp DataSupplier.cpp Force.cpp main.cpp MyForce.cpp SimpleDataSupplier.cpp Simulation.cpp && ./a.out

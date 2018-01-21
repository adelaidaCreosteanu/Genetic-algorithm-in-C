#include <stdio.h>

void AllocateMemory();

int StartSimulation();

int main() {
    AllocateMemory();
    int nGenerations = StartSimulation();
    printf("The simulation took %d generations.", nGenerations);
}

void AllocateMemory() {

}

int StartSimulation() {
    return 0;
}
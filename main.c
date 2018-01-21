#include <stdio.h>

#define POPULATION_SIZE 100
#define GOAL "To be or not to be"

struct Generation {
    int number;
    char *population[POPULATION_SIZE];
    int *fitness[POPULATION_SIZE];
};

/* Function prototypes */
struct Generation InitialisePopulation();
int               ReachedGoal(struct Generation gen);
struct Generation Reproduce(struct Generation parents);

int main() {
    struct Generation parents = InitialisePopulation();
    struct Generation children;

    while (!ReachedGoal(parents)) {
        children = Reproduce(parents);
        parents = children;
    }

    printf("To get to goal \"%s\", it took %d generations.", GOAL, parents.number);
}

struct Generation InitialisePopulation() {
    // Create random population of same length as goal
    // Needs to also evaluate fitness
}

int ReachedGoal(struct Generation gen) {
    // Just checks if the highest fitness individual is perfect
}

struct Generation Reproduce(struct Generation parents) {
    // This is where the magic happens

    // Evaluate fitness of parents
    // Create new generation by choosing from most fit individuals with a higher probability
    // Crossing over two partners and mutating
    // Returns children

    // Notes: make separate functions for: evaluating fitness, producing an individual
}
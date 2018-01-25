#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define POPULATION_SIZE 20
#define GOAL            "To be or not to be"
#define NUMBER_GENES    (sizeof GOAL - 1)
#define GENE_ALPHABET   "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "
#define ALLELES         (sizeof GENE_ALPHABET - 1)

#define TRUE 1
#define FALSE 0

#define MUTATION_RATE 0.02

struct Generation {
    int number;
    char population[POPULATION_SIZE][NUMBER_GENES];
    int fitness[POPULATION_SIZE];
};

/* Function prototypes */
void initialise_population(struct Generation *genZero);
    void random_strings(char pop[][NUMBER_GENES]);
void evaluate_fitness(char pop[][NUMBER_GENES], int fit[]);
void print_generation(struct Generation *gen);
int reached_goal(struct Generation *gen);
void reproduce(struct Generation *parents, struct Generation *children);
    int select_parent(struct Generation *gen, int totalFitness);

int main() {
    size_t memoryNeeded = sizeof(char) * NUMBER_GENES * POPULATION_SIZE + sizeof(int) * (POPULATION_SIZE+1);
    struct Generation *parents = malloc(memoryNeeded);
    initialise_population(parents);
//    struct Generation *children = malloc(memoryNeeded);
//
//    do {
//        reproduce(parents, children);
//        parents = children;
//    } while (!reached_goal(parents));
//
//    printf("To get to goal \"%s\", it took %d generations.", GOAL, parents->number);

    print_generation(parents);
}

void initialise_population(struct Generation *genZero) {
    // Create random population of same length as goal
    // Needs to also evaluate fitness
    genZero->number = 0;

    srand(time(0));
    random_strings(genZero->population);

    evaluate_fitness(genZero->population, genZero->fitness);
}

void random_strings(char pop[][NUMBER_GENES]) {
    for (int i = 0; i < POPULATION_SIZE; i ++) {
        // generate an individual
        for (int c = 0; c < NUMBER_GENES; c ++) {
            int selectedChar = rand() % ALLELES;
            pop[i][c] = GENE_ALPHABET[selectedChar];
        }
    }
}

void evaluate_fitness(char pop[][NUMBER_GENES], int fit[]) {
    for (int i = 0; i < POPULATION_SIZE; i ++) {
        fit[i] = 0;

        // calculate fitness for this individual
        for (int c = 0; c < NUMBER_GENES; c ++) {
            if (pop[i][c] == GOAL[c]) {
                fit[i] ++;
            }
        }
    }
}

// Just checks if any individual has perfect fitness
int reached_goal(struct Generation *gen) {
    for (int i = 0; i < POPULATION_SIZE; i ++) {
        if (gen->fitness[i] == NUMBER_GENES) {
            return TRUE;
        }
    }

    return FALSE;
}

void reproduce(struct Generation *parents, struct Generation *children) {
    // This is where the magic happens

    // Evaluate fitness of parents
    // Create new generation by choosing from most fit individuals with a higher probability
    // Crossing over two partners and mutating
    // Returns children

    // Notes: make separate functions for: evaluating fitness, producing an individual
}

int select_parent(struct Generation *gen, int totalFitness) {
    int choice = rand() % totalFitness + 1;
    int individual = 0;

    while (choice > 0) {
        choice -= gen->fitness[individual];
        individual ++;
    }

    return individual - 1;
}

void print_generation(struct Generation *gen) {
    printf("Generation number: %d\nIndividuals:\n", gen->number);

    for (int i = 0; i < POPULATION_SIZE; i ++) {
        // Print individual's genes:
        for (int c = 0; c < NUMBER_GENES; c ++) {
            printf("%c", gen->population[i][c]);
        }

        // Print their fitness score:
        printf("    fitness: %d\n", gen->fitness[i]);
    }

    if(reached_goal(gen)) {
        printf("Generation reached goal.");
    } else {
        printf("Generation is imperfect.");
    }
}

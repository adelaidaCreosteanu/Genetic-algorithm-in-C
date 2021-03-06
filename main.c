#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define POPULATION_SIZE 100
#define GOAL            "unicorn"
#define NUMBER_GENES    (sizeof GOAL - 1)
#define GENE_ALPHABET   "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "
#define ALLELES         (sizeof GENE_ALPHABET - 1)
#define MUTATION_RATE 0.02

#define TRUE 1
#define FALSE 0

struct Organism {
    float fitness;
    char genes[NUMBER_GENES];
};

struct Generation {
    int number;
    struct Organism population[POPULATION_SIZE];
};

/* Function prototypes */
void initialise_population(struct Generation *genZero);
    void random_strings(struct Organism pop[]);
void reproduce_generation(struct Generation *lastGen, struct Generation *newGen);
    int select_parent(struct Organism pop[], float totalFitness);
    void reproduce_two_organisms(const char *parentA, const char *parentB, char *siblingA, char *siblingB);
    void apply_mutation(char child[]);
void evaluate_fitness(struct Organism pop[]);
float total_fitness(struct Organism pop[]);
int reached_goal(struct Organism pop[]);
void print_generation(struct Generation *gen);

int main() {
    struct Generation *parents = malloc(sizeof(struct Generation));
    struct Generation *children = malloc(sizeof(struct Generation));
    initialise_population(parents);

    while (!reached_goal(parents->population)) {
        reproduce_generation(parents, children);
        parents = children;
    }

    printf("To get to goal \"%s\", it took %d generations.", GOAL, parents->number);
    print_generation(parents);

    free(parents);
    free(children);

    return 1;
}

void initialise_population(struct Generation *genZero) {
    // Create random population of same length as goal
    genZero->number = 0;

    // Seed the pseudo-random generator
    srand(time(0));
    random_strings(genZero->population);
}

void random_strings(struct Organism pop[]) {
    for (int o = 0; o < POPULATION_SIZE; o ++) {
        // generate an organism's genes
        for (int c = 0; c < NUMBER_GENES; c ++) {
            int selection = rand() % ALLELES;
            pop[o].genes[c] = GENE_ALPHABET[selection];
        }
    }
}

void evaluate_fitness(struct Organism pop[]) {
    float current;

    for (int o = 0; o < POPULATION_SIZE; o ++) {
        current = 0;

        // calculate fitness for this individual
        for (int c = 0; c < NUMBER_GENES; c ++) {
            if (pop[o].genes[c] == GOAL[c]) {
                current ++;
            }
        }

        // In case this is a sucker, at least he doesn't have 0 chance
        if (current == 0) {
            current = 0.1;
        }
        pop[o].fitness = current;
    }
}

float total_fitness(struct Organism *pop) {
    float fit = 0;

    for (int o = 0; o < POPULATION_SIZE; o ++) {
        fit += pop[o].fitness;
    }

    return fit;
}

// This is where the magic happens
void reproduce_generation(struct Generation *lastGen, struct Generation *newGen) {
    newGen->number = lastGen->number + 1;

    // Setting variables for the two populations for convenience
    struct Organism *parents = lastGen->population;
    struct Organism *children = newGen->population;

    float totalFitness = total_fitness(parents);

    int sanityCheck = 0;

    for (int i = 0; i < (float) POPULATION_SIZE/2; i ++) {
        int pA = select_parent(parents, totalFitness);
        int pB = select_parent(parents, totalFitness);

        while (pA == pB) {
            pB = select_parent(parents, totalFitness);
            sanityCheck ++;

            if (sanityCheck > 1000) {
                printf("WE WENT INSANE!");
                print_generation(lastGen);
                print_generation(newGen);
                exit(0);
            }
        }

        // Produce two children at the two ends of the population array
        int evilTwin = POPULATION_SIZE - i - 1;
        reproduce_two_organisms(parents[pA].genes, parents[pB].genes, children[i].genes, children[evilTwin].genes);
    }
}

// Returns index of selected parent
int select_parent(struct Organism pop[], float totalFitness) {
    int choice = rand() % (int) floor(totalFitness);

    for (int parent = 0; parent < POPULATION_SIZE; parent ++) {
        choice -= pop[parent].fitness;

        if (choice <= 0) {
            return parent;
        }
    }
}

// Applies crossover with a middle cutoff and applies mutation
void reproduce_two_organisms(const char *parentA, const char *parentB, char *siblingA, char *siblingB) {
    int cutoff = NUMBER_GENES/2;

    for (int c = 0; c < cutoff; c ++) {
        siblingA[c] = parentA[c];
        siblingB[c] = parentB[c];
    }
    for (int c = cutoff; c < NUMBER_GENES; c ++) {
        siblingA[c] = parentB[c];
        siblingB[c] = parentA[c];
    }
    apply_mutation(siblingA);
    apply_mutation(siblingB);
}

void apply_mutation(char child[]) {
    for (int c = 0; c < NUMBER_GENES; c ++) {
        double prob = (double) rand()/ RAND_MAX;    // between 0 and 1

        if (prob < MUTATION_RATE) {
            // We're mutating this gene!
            int selection = rand() % ALLELES;
            child[c] = GENE_ALPHABET[selection];
        }
    }
}

// Just checks if any individual has perfect fitness
int reached_goal(struct Organism pop[]) {
    evaluate_fitness(pop);

    for (int o = 0; o < POPULATION_SIZE; o ++) {
        if (pop[o].fitness == NUMBER_GENES) {      // NEEDS TO BE MODIFIED if fitness function is changed
            return TRUE;
        }
    }

    return FALSE;
}

void print_generation(struct Generation *gen) {
    printf("Generation number: %d\nIndividuals:\n", gen->number);

    for (int i = 0; i < POPULATION_SIZE; i ++) {
        struct Organism *org = &(gen->population[i]);

        // Print individual's genes:
        for (int c = 0; c < NUMBER_GENES; c ++) {
            printf("%c", org->genes[c]);
        }

        // Print their fitness score:
        printf("    fitness: %3.1f\n", org->fitness);
    }

    if(reached_goal(gen->population)) {
        printf("Generation reached goal.\n\n");
    } else {
        printf("Generation is imperfect.\n\n");
    }
}

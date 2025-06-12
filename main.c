#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/Particle.h"
#include "include/Nbody.h"
#include "include/data_generator.h"
#include <omp.h>

int main(int argc, char **argv) {
    // Simulation parameters
    int n ;                // Number of particles
    double dt = 0.01;            // Time step
    int n_steps;           // Number of steps
    double theta = 0.5;          // Barnes-Hut opening angle
    char distribution[20];       // Buffer for initial distribution type



    printf("Select initial distribution (uniform/gaussian/plummer): ");
    if (scanf("%19s", distribution) != 1) {
        fprintf(stderr, "Error: Invalid input.\n");
        return EXIT_FAILURE;
    }

    printf("Enter number of particles (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Error: Invalid number of particles.\n");
        return EXIT_FAILURE;
    }

    printf("Enter number of steps (n_steps): ");
    if (scanf("%d", &n_steps) != 1 || n_steps <= 0) {
        fprintf(stderr, "Error: Invalid number of steps.\n");
        return EXIT_FAILURE;
    }




    // Allocate memory for particles
    Particle *particles = (Particle *)malloc(n * sizeof(Particle));
    if (particles == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for particles.\n");
        return EXIT_FAILURE;
    }

    // Generate initial particle distribution
    if (strcmp(distribution, "uniform") == 0) {
        generate_uniform_distribution(particles, n);
    } else if (strcmp(distribution, "gaussian") == 0) {
        generate_gaussian_distribution(particles, n);
    } else if (strcmp(distribution, "plummer") == 0) {
        generate_plummer_distribution(particles, n);
    } else {
        fprintf(stderr, "Error: Invalid distribution type.\n");
        free(particles);  // Free allocated memory
        return EXIT_FAILURE;
    }

    double start_time = omp_get_wtime();  
    simulate(particles, n, dt, n_steps, theta);
    double end_time = omp_get_wtime();    

    printf("Simulation completed in %.3f seconds.\n", end_time - start_time);

    free(particles);
    return EXIT_SUCCESS;
}


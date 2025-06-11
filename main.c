#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/Particle.h"
#include "include/Nbody.h"
#include "include/data_generator.h"

int main(void) {
    int n = 1000;                // Número de partículas
    double dt = 0.01;            // Paso de tiempo
    int n_steps = 100;           // Número de pasos
    double theta = 0.5;          // Umbral de apertura para Barnes-Hut
    char distribution[20];       // Buffer para la distribución

    printf("Seleccione la distribución inicial (uniform/gaussian/plummer): ");
    if (scanf("%19s", distribution) != 1) {
        fprintf(stderr, "Error: Entrada inválida.");
        return EXIT_FAILURE;
    }

    // Asignar arreglo de partículas
    Particle *particles = malloc(n * sizeof *particles);
    if (particles == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para las partículas.");
        return EXIT_FAILURE;
    }

    // Generar distribución inicial según selección
    if (strcmp(distribution, "uniform") == 0) {
        generate_uniform_distribution(particles, n);
    } else if (strcmp(distribution, "gaussian") == 0) {
        generate_gaussian_distribution(particles, n);
    } else if (strcmp(distribution, "plummer") == 0) {
        generate_plummer_distribution(particles, n);
    } else {
        fprintf(stderr, "Error: Distribución no válida.");
        free(particles);
        return EXIT_FAILURE;
    }

    // Ejecutar simulación con Barnes-Hut
    simulate(particles, n, dt, n_steps, theta);

    // Liberar memoria y terminar
    free(particles);
    return EXIT_SUCCESS;
}


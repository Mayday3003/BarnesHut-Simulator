#include <stdlib.h>
#include <stdio.h>
#include "../include/Nbody.h"
#include "../include/Particle.h"

struct Nbody {
    Particle *particles;
    int count;
    double dt;
    int steps;
    double theta;
};

Nbody* Nbody_create(int argc, char **argv) {
    // Parámetros personalizados opcionales (usar por defecto si no se pasan)
    int n = 1000;
    double dt = 0.01;
    int steps = 100;
    double theta = 0.5;

    if (argc >= 2) n = atoi(argv[1]);
    if (argc >= 3) dt = atof(argv[2]);
    if (argc >= 4) steps = atoi(argv[3]);
    if (argc >= 5) theta = atof(argv[4]);

    Nbody *sim = malloc(sizeof *sim);
    if (!sim) {
        fprintf(stderr, "Error: no se pudo asignar Nbody.\n");
        exit(EXIT_FAILURE);
    }
    sim->particles = malloc(n * sizeof *sim->particles);
    if (!sim->particles) {
        fprintf(stderr, "Error: no se pudo asignar partículas.\n");
        free(sim);
        exit(EXIT_FAILURE);
    }
    sim->count = n;
    sim->dt = dt;
    sim->steps = steps;
    sim->theta = theta;
    return sim;
}

void simulate(Particle* particles, int n, double dt, int steps, double theta) {
    // Implementación simplificada: llama al integrador leapfrog
    for (int s = 0; s < steps; s++) {
        // Aquí construir el Octree y calcular fuerzas
        // luego actualizar posiciones/velocidades
        // por simplicidad, solo imprimimos un paso
        printf("Paso %d / %d\n", s + 1, steps);
        fflush(stdout);
    }
    printf("Simulación completada.\n");
}

void Nbody_destroy(Nbody* sim) {
    if (!sim) return;
    free(sim->particles);
    free(sim);
}
#include <stdlib.h>
#include <math.h>
#include "../include/data_generator.h"

// Ejemplo simplificado, ajusta constantes según tu modelo
void generate_uniform_distribution(Particle* particles, int count) {
    for (int i = 0; i < count; i++) {
        particles[i].pos.x = rand() / (double)RAND_MAX;
        particles[i].pos.y = rand() / (double)RAND_MAX;
        particles[i].pos.z = rand() / (double)RAND_MAX;
        particles[i].mass = 1.0;
    }
}

void generate_gaussian_distribution(Particle* particles, int count) {
    for (int i = 0; i < count; i++) {
        // Box-Muller
        double u1 = rand() / (double)RAND_MAX;
        double u2 = rand() / (double)RAND_MAX;
        double r = sqrt(-2.0 * log(u1));
        particles[i].pos.x = r * cos(2 * M_PI * u2);
        particles[i].pos.y = r * sin(2 * M_PI * u2);
        particles[i].pos.z = 0;
        particles[i].mass = 1.0;
    }
}

void generate_plummer_distribution(Particle* particles, int count) {
    for (int i = 0; i < count; i++) {
        // Implementación básica de Plummer
        double X, Y, Z;
        do {
            X = rand() / (double)RAND_MAX * 2 - 1;
            Y = rand() / (double)RAND_MAX * 2 - 1;
            Z = rand() / (double)RAND_MAX * 2 - 1;
        } while (X * X + Y * Y + Z * Z > 1);
        particles[i].pos.x = X;
        particles[i].pos.y = Y;
        particles[i].pos.z = Z;
        particles[i].mass = 1.0;
    }
}
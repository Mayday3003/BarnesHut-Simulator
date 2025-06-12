#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "../include/data_generator.h"


void generate_uniform_distribution(Particle* particles, int count) {
    #pragma omp parallel for 
    for (int i = 0; i < count; i++) {
        unsigned int seed = (unsigned int)(i + 1234 * omp_get_thread_num());
        particles[i].pos.x = rand_r(&seed) / (double)RAND_MAX;
        particles[i].pos.y = rand_r(&seed) / (double)RAND_MAX;
        particles[i].pos.z = rand_r(&seed) / (double)RAND_MAX;
        particles[i].mass = 1.0;
    }
}

void generate_gaussian_distribution(Particle* particles, int count) {
    #pragma omp parallel for 
    for (int i = 0; i < count; i++) {
        unsigned int seed = (unsigned int)(i + 5678 * omp_get_thread_num());
        double u1 = rand_r(&seed) / (double)RAND_MAX;
        double u2 = rand_r(&seed) / (double)RAND_MAX;
        double r = sqrt(-2.0 * log(u1));
        particles[i].pos.x = r * cos(2 * M_PI * u2);
        particles[i].pos.y = r * sin(2 * M_PI * u2);
        particles[i].pos.z = 0;
        particles[i].mass = 1.0;
    }
}

void generate_plummer_distribution(Particle* particles, int count) {
    #pragma omp parallel for
    for (int i = 0; i < count; i++) {
        unsigned int seed = (unsigned int)(i + 91011 * omp_get_thread_num());
        double X, Y, Z;
        do {
            X = rand_r(&seed) / (double)RAND_MAX * 2 - 1;
            Y = rand_r(&seed) / (double)RAND_MAX * 2 - 1;
            Z = rand_r(&seed) / (double)RAND_MAX * 2 - 1;
        } while (X * X + Y * Y + Z * Z > 1);
        particles[i].pos.x = X;
        particles[i].pos.y = Y;
        particles[i].pos.z = Z;
        particles[i].mass = 1.0;
    }
}
// ...existing code...
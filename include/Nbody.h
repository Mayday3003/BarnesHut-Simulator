#ifndef NBODY_H
#define NBODY_H

#include "Particle.h"

// Simula n partículas usando algoritmo Barnes-Hut
typedef struct Nbody Nbody;

Nbody* Nbody_create(int argc, char **argv);
void simulate(Particle* particles, int n, double dt, int steps, double theta);
void Nbody_destroy(Nbody* sim);

#endif // NBODY_H
#ifndef LEAPFROG_H
#define LEAPFROG_H

#include "Particle.h"

// Actualiza posiciones y velocidades usando integrador Leapfrog
void leapfrog_step(Particle *particles, int count, double dt);

#endif // LEAPFROG_H
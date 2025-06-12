#ifndef LEAPFROG_H
#define LEAPFROG_H

#include "Particle.h"

#ifdef __cplusplus
extern "C" {
#endif

// Actualiza posiciones y velocidades usando integrador Leapfrog
void leapfrog_drift(Particle *particles, int count, double dt);
void leapfrog_kick_drift(Particle *particles, int count, double dt);

#ifdef __cplusplus
}
#endif

#endif // LEAPFROG_H
#ifndef LEAPFROG_H
#define LEAPFROG_H

#include "Particle.h"

#ifdef __cplusplus
extern "C" {
#endif

void leapfrog_drift(Particle *particles, int count, double dt);
void leapfrog_kick_drift(Particle *particles, int count, double dt);

#ifdef __cplusplus
}
#endif

#endif
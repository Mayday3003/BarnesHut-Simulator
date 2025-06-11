#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include "Particle.h"

// Genera distribuciones iniciales de partículas
void generate_uniform_distribution(Particle* bodies, int count);
void generate_gaussian_distribution(Particle* bodies, int count);
void generate_plummer_distribution(Particle* bodies, int count);

#endif // DATA_GENERATOR_H
#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
    double x, y, z;
} Vector3;

typedef struct {
    Vector3 pos;  // Posición
    Vector3 vel;  // Velocidad
    Vector3 acc;  // Aceleración
    double mass;  // Masa
} Particle;

#endif // PARTICLE_H
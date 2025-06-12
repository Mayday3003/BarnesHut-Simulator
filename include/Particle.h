#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
    double x, y, z;
} Vector3;

typedef struct {
    Vector3 pos;  // Position
    Vector3 vel;  // Velocity
    Vector3 acc;  // Acceleration
    double mass;  // Mass
} Particle;

#endif 
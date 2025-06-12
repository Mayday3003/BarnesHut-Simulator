#include <omp.h>
#include "../include/Leapfrog.h"
#include "../include/Particle.h"

// Update particle positions using half-step drift
void leapfrog_drift(Particle* particles, int n, double dt) {
    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < n; i++) {
        particles[i].pos.x += dt * particles[i].vel.x * 0.5;
        particles[i].pos.y += dt * particles[i].vel.y * 0.5;
        particles[i].pos.z += dt * particles[i].vel.z * 0.5;
    }
}

// Update particle velocities and positions using kick-drift
void leapfrog_kick_drift(Particle* particles, int n, double dt) {
    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < n; i++) {
        // Update velocity (Kick)
        particles[i].vel.x += dt * particles[i].acc.x;
        particles[i].vel.y += dt * particles[i].acc.y;
        particles[i].vel.z += dt * particles[i].acc.z;

        // Update position (Drift)
        particles[i].pos.x += dt * particles[i].vel.x * 0.5;
        particles[i].pos.y += dt * particles[i].vel.y * 0.5;
        particles[i].pos.z += dt * particles[i].vel.z * 0.5;
    }
}

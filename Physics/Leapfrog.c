#include <omp.h>
#include "Particle.h" 

void leapfrog_drift(Particle* particles, int n, double dt) {
   #pragma omp parallel for schedule(guided)
   for (int i = 0; i < n; i++) {
       particles[i].position.x += dt * particles[i].velocity.x * 0.5;
       particles[i].position.y += dt * particles[i].velocity.y * 0.5;
       particles[i].position.z += dt * particles[i].velocity.z * 0.5;
   }
}


void leapfrog_kick_drift(Particle* particles, int n, double dt) {
   #pragma omp parallel for schedule(guided)
   for (int i = 0; i < n; i++) {
       // Update velocity (Kick)
       particles[i].velocity.x += dt * particles[i].acceleration.x;
       particles[i].velocity.y += dt * particles[i].acceleration.y;
       particles[i].velocity.z += dt * particles[i].acceleration.z;
      
       // Update position (Drift)
       particles[i].position.x += dt * particles[i].velocity.x * 0.5;
       particles[i].position.y += dt * particles[i].velocity.y * 0.5;
       particles[i].position.z += dt * particles[i].velocity.z * 0.5;
   }
}

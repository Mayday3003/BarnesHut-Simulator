#include <iostream>
#include "../include/Nbody.h"
#include "../include/Particle.h"

struct Nbody {
    Particle *particles;
    int count;
    double dt;
    int steps;
    double theta;
};


int main_cpp(int argc, char **argv) {
    Nbody *sim = Nbody_create(argc, argv);
    simulate(sim->particles, sim->count, sim->dt, sim->steps, sim->theta);
    Nbody_destroy(sim);
    return 0;
}
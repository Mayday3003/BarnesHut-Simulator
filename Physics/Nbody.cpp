#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../include/Nbody.h"
#include "../include/Particle.h"
#include "../include/Leapfrog.h"
#include "../include/Octree.h"

struct Nbody {
    Particle *particles;
    int count;
    double dt;
    int steps;
    double theta;
};

Nbody* Nbody_create(int argc, char **argv) {
    int n = 1000;
    double dt = 0.01;
    int steps = 100;
    double theta = 0.5;

    if (argc >= 2) n = atoi(argv[1]);
    if (argc >= 3) dt = atof(argv[2]);
    if (argc >= 4) steps = atoi(argv[3]);
    if (argc >= 5) theta = atof(argv[4]);

    Nbody *sim = new Nbody;
    sim->particles = new Particle[n];
    sim->count = n;
    sim->dt = dt;
    sim->steps = steps;
    sim->theta = theta;
    return sim;
}

void save_positions(const Particle* particles, int n, int step) {
    char filename[100];
    snprintf(filename, sizeof(filename), "datos/posiciones/step_%d.txt", step);

    FILE* file = fopen(filename, "w");
    if (!file) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << " para escribir.\n";
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "Particle %d: %.6f %.6f %.6f\n", i, particles[i].pos.x, particles[i].pos.y, particles[i].pos.z);
    }

    fclose(file);
}

void simulate(Particle* particles, int n, double dt, int steps, double theta) {
    for (int s = 0; s < steps; s++) {
        OctreeNode root({0, 0, 0}, 100.0);
        for (int i = 0; i < n; i++) {
            root.insert(particles[i]);
        }
        root.NodeMass();

        for (int i = 0; i < n; i++) {
            particles[i].acc = {0, 0, 0};
            calculate_force(&root, particles[i], theta);
        }

        leapfrog_drift(particles, n, dt);
        leapfrog_kick_drift(particles, n, dt);

        save_positions(particles, n, s + 1);
    }
    std::cout << "Simulación completada.\n";
}

void Nbody_destroy(Nbody* sim) {
    if (!sim) return;
    delete[] sim->particles;
    delete sim;
}

int main_cpp(int argc, char **argv) {
    Nbody *sim = Nbody_create(argc, argv);
    simulate(sim->particles, sim->count, sim->dt, sim->steps, sim->theta);
    Nbody_destroy(sim);
    return 0;
}
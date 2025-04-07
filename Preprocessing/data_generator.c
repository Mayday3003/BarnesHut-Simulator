#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


#define MAX_FILENAME_LENGTH 100
#define MASS_DEFAULT 1.0
#define PLUMMER_SCALE 100.0

typedef struct {
    double x, y, z;
    double vx, vy, vz;
    double mass;
} Body;

void generate_uniform_distribution(Body* bodies, int count);
void generate_gaussian_distribution(Body* bodies, int count);
void generate_plummer_distribution(Body* bodies, int count);
void write_to_file(const Body* bodies, int count, const char* filename, const char* format);
double random_normal(unsigned int* seed);
double random_uniform(unsigned int* seed);

int main() {
    int body_count;
    char distribution[20];
    char format[10];

    printf("Enter the number of bodies: ");
    scanf("%d", &body_count);

    printf("Distribution type (uniform/gaussian/plummer): ");
    scanf("%s", distribution);

    printf("Output format (csv/txt): ");
    scanf("%s", format);

    Body* bodies = malloc(sizeof(Body) * body_count);
    if (!bodies) {
        fprintf(stderr, "Error: Unable to allocate memory.\n");
        return EXIT_FAILURE;
    }

    if (strcmp(distribution, "uniform") == 0) {
        generate_uniform_distribution(bodies, body_count);
    } else if (strcmp(distribution, "gaussian") == 0) {
        generate_gaussian_distribution(bodies, body_count);
    } else if (strcmp(distribution, "plummer") == 0) {
        generate_plummer_distribution(bodies, body_count);
    } else {
        fprintf(stderr, "Error: Invalid distribution type.\n");
        free(bodies);
        return EXIT_FAILURE;
    }

    char filename[MAX_FILENAME_LENGTH];
    snprintf(filename, MAX_FILENAME_LENGTH, "generated_data.%s", format);
    write_to_file(bodies, body_count, filename, format);

    printf("Data successfully written to '%s'.\n", filename);
    free(bodies);
    return EXIT_SUCCESS;
}

void generate_uniform_distribution(Body* bodies, int count) {
    #pragma omp parallel for
    for (int i = 0; i < count; ++i) {
        unsigned int seed = (unsigned int)(time(NULL) + i);
        bodies[i].x = 2.0 * random_uniform(&seed) - 1.0;
        bodies[i].y = 2.0 * random_uniform(&seed) - 1.0;
        bodies[i].z = 2.0 * random_uniform(&seed) - 1.0;
        bodies[i].vx = bodies[i].vy = bodies[i].vz = 0.0;
        bodies[i].mass = MASS_DEFAULT;
    }
}

void generate_gaussian_distribution(Body* bodies, int count) {
    #pragma omp parallel for
    for (int i = 0; i < count; ++i) {
        unsigned int seed = (unsigned int)(time(NULL) + i);
        bodies[i].x = random_normal(&seed) * 0.5;
        bodies[i].y = random_normal(&seed) * 0.5;
        bodies[i].z = random_normal(&seed) * 0.5;
        bodies[i].vx = bodies[i].vy = bodies[i].vz = 0.0;
        bodies[i].mass = MASS_DEFAULT;
    }
}

void generate_plummer_distribution(Body* bodies, int count) {
    #pragma omp parallel for
    for (int i = 0; i < count; ++i) {
        unsigned int seed = (unsigned int)(time(NULL) + i);
        double X1, X2, radius;

        do {
            X1 = random_uniform(&seed);
            X2 = random_uniform(&seed);
            radius = 1.0 / sqrt(pow(X1, -2.0 / 3.0) - 1.0);
        } while (radius > PLUMMER_SCALE);

        double z = (1.0 - 2.0 * random_uniform(&seed)) * radius;
        double theta = 2 * M_PI * random_uniform(&seed);
        double r_xy = sqrt(radius * radius - z * z);
        double x = r_xy * cos(theta);
        double y = r_xy * sin(theta);

        bodies[i].x = x;
        bodies[i].y = y;
        bodies[i].z = z;
        bodies[i].vx = bodies[i].vy = bodies[i].vz = 0.0;
        bodies[i].mass = MASS_DEFAULT;
    }
}

void write_to_file(const Body* bodies, int count, const char* filename, const char* format) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s' for writing.\n", filename);
        return;
    }

    for (int i = 0; i < count; ++i) {
        if (strcmp(format, "csv") == 0) {
            fprintf(file, "%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                bodies[i].x, bodies[i].y, bodies[i].z,
                bodies[i].vx, bodies[i].vy, bodies[i].vz,
                bodies[i].mass);
        } else {
            fprintf(file, "%.6f %.6f %.6f %.6f %.6f %.6f %.6f\n",
                bodies[i].x, bodies[i].y, bodies[i].z,
                bodies[i].vx, bodies[i].vy, bodies[i].vz,
                bodies[i].mass);
        }
    }

    fclose(file);
}

// Generate uniform [0, 1)
double random_uniform(unsigned int* seed) {
    return rand_r(seed) / (double)RAND_MAX;
}

// Box-Muller method for Gaussian
double random_normal(unsigned int* seed) {
    double u1 = random_uniform(seed);
    double u2 = random_uniform(seed);
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}
